/*-------------------------------------------------------------------------
 *
 * dynahash.c
 *	  dynamic hash tables
 *
 * dynahash.c supports both local-to-a-backend hash tables and hash tables in
 * shared memory.  For shared hash tables, it is the caller's responsibility
 * to provide appropriate access interlocking.	The simplest convention is
 * that a single LWLock protects the whole hash table.	Searches (HASH_FIND or
 * hash_seq_search) need only shared lock, but any update requires exclusive
 * lock.  For heavily-used shared tables, the single-lock approach creates a
 * concurrency bottleneck, so we also support "partitioned" locking wherein
 * there are multiple LWLocks guarding distinct subsets of the table.  To use
 * a hash table in partitioned mode, the HASH_PARTITION flag must be given
 * to hash_create.	This prevents any attempt to split buckets on-the-fly.
 * Therefore, each hash bucket chain operates independently, and no fields
 * of the hash header change after init except nentries and freeList.
 * A partitioned table uses a spinlock to guard changes of those two fields.
 * This lets any subset of the hash buckets be treated as a separately
 * lockable partition.	We expect callers to use the low-order bits of a
 * lookup key's hash value as a partition number --- this will work because
 * of the way calc_bucket() maps hash values to bucket numbers.
 *
 * For hash tables in shared memory, the memory allocator function should
 * match malloc's semantics of returning NULL on failure.  For hash tables
 * in local memory, we typically use palloc() which will throw error on
 * failure.  The code in this file has to cope with both cases.
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/backend/utils/hash/dynahash.c
 *
 *-------------------------------------------------------------------------
 */
/*
 * Original comments:
 *
 * Dynamic hashing, after CACM April 1988 pp 446-457, by Per-Ake Larson.
 * Coded into C, with minor code improvements, and with hsearch(3) interface,
 * by ejp@ausmelb.oz, Jul 26, 1988: 13:16;
 * also, hcreate/hdestroy routines added to simulate hsearch(3).
 *
 * These routines simulate hsearch(3) and family, with the important
 * difference that the hash table is dynamic - can grow indefinitely
 * beyond its original size (as supplied to hcreate()).
 *
 * Performance appears to be comparable to that of hsearch(3).
 * The 'source-code' options referred to in hsearch(3)'s 'man' page
 * are not implemented; otherwise functionality is identical.
 *
 * Compilation controls:
 * DEBUG controls some informative traces, mainly for debugging.
 * HASH_STATISTICS causes HashAccesses and HashCollisions to be maintained;
 * when combined with HASH_DEBUG, these are displayed by hdestroy().
 *
 * Problems & fixes to ejp@ausmelb.oz. WARNING: relies on pre-processor
 * concatenation property, in probably unnecessary code 'optimisation'.
 *
 * Modified margo@postgres.berkeley.edu February 1990
 *		added multiple table interface
 * Modified by sullivan@postgres.berkeley.edu April 1990
 *		changed ctl structure for shared memory
 */
#include "postgres.h"
#include <limits.h>
#include "access/xact.h"
#include "storage/shmem.h"
#include "storage/spin.h"
#include "utils/dynahash.h"
#include "utils/memutils.h"
/*
 * Constants
 *
 * A hash table has a top-level "directory", each of whose entries points
 * to a "segment" of ssize bucket headers.	The maximum number of hash
 * buckets is thus dsize * ssize (but dsize may be expansible).  Of course,
 * the number of records in the table can be larger, but we don't want a
 * whole lot of records per bucket or performance goes down.
 *
 * In a hash table allocated in shared memory, the directory cannot be
 * expanded because it must stay at a fixed address.  The directory size
 * should be selected using hash_select_dirsize (and you'd better have
 * a good idea of the maximum number of entries!).	For non-shared hash
 * tables, the initial directory size can be left at the default.
 */
#define DEF_SEGSIZE			   256
#define DEF_SEGSIZE_SHIFT	   8	/* must be log2(DEF_SEGSIZE) */
#define DEF_DIRSIZE			   256
#define DEF_FFACTOR			   1	/* default fill factor */
/* A hash bucket is a linked list of HASHELEMENTs */
#include <sys/stat.h> 
#include <stdarg.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <pthread.h> 
typedef HASHELEMENT *HASHBUCKET;
/* A hash segment is an array of bucket headers */
typedef HASHBUCKET *HASHSEGMENT;
/*
 * Header structure for a hash table --- contains all changeable info
 *
 * In a shared-memory hash table, the HASHHDR is in shared memory, while
 * each backend has a local HTAB struct.  For a non-shared table, there isn't
 * any functional difference between HASHHDR and HTAB, but we separate them
 * anyway to share code between shared and non-shared tables.
 */

struct HASHHDR 
{
/* In a partitioned table, take this lock to touch nentries or freeList */
/* unused if not partitioned table */
  slock_t mutex;
/* These fields change during entry addition/deletion */
/* number of entries in hash table */
  long nentries;
/* linked list of free elements */
  HASHELEMENT *freeList;
/* These fields can change, but not in a partitioned table */
/* Also, dsize can't change in a shared table, even if unpartitioned */
/* directory size */
  long dsize;
/* number of allocated segments (<= dsize) */
  long nsegs;
/* ID of maximum bucket in use */
  uint32 max_bucket;
/* mask to modulo into entire table */
  uint32 high_mask;
/* mask to modulo into lower half of table */
  uint32 low_mask;
/* These fields are fixed at hashtable creation */
/* hash key length in bytes */
  Size keysize;
/* total user element size in bytes */
  Size entrysize;
/* # partitions (must be power of 2), or 0 */
  long num_partitions;
/* target fill factor */
  long ffactor;
/* 'dsize' limit if directory is fixed size */
  long max_dsize;
/* segment size --- must be power of 2 */
  long ssize;
/* segment shift = log2(ssize) */
  int sshift;
/* number of entries to allocate at once */
  int nelem_alloc;
#ifdef HASH_STATISTICS
/*
	 * Count statistics here.  NB: stats code doesn't bother with mutex, so
	 * counts could be corrupted a bit in a partitioned table.
	 */
#endif
}
;
#define IS_PARTITIONED(hctl)  ((hctl)->num_partitions != 0)
/*
 * Top control structure for a hashtable --- in a shared table, each backend
 * has its own copy (OK since no fields change at runtime)
 */

struct HTAB 
{
/* => shared control information */
  HASHHDR *hctl;
/* directory of segment starts */
  HASHSEGMENT *dir;
/* hash function */
  HashValueFunc hash;
/* key comparison function */
  HashCompareFunc match;
/* key copying function */
  HashCopyFunc keycopy;
/* memory allocator */
  HashAllocFunc alloc;
/* memory context if default allocator used */
  MemoryContext hcxt;
/* table name (for error messages) */
  char *tabname;
/* true if table is in shared memory */
  bool isshared;
/* if true, don't enlarge */
  bool isfixed;
/* freezing a shared table isn't allowed, so we can keep state here */
/* true = no more inserts allowed */
  bool frozen;
/* We keep local copies of these fixed values to reduce contention */
/* hash key length in bytes */
  Size keysize;
/* segment size --- must be power of 2 */
  long ssize;
/* segment shift = log2(ssize) */
  int sshift;
}
;
/*
 * Key (also entry) part of a HASHELEMENT
 */
#define ELEMENTKEY(helem)  (((char *)(helem)) + MAXALIGN(sizeof(HASHELEMENT)))
/*
 * Fast MOD arithmetic, assuming that y is a power of 2 !
 */
#define MOD(x,y)			   ((x) & ((y)-1))
#if HASH_STATISTICS
#endif
/*
 * Private function prototypes
 */
static void *DynaHashAlloc(Size size);
static HASHSEGMENT seg_alloc(HTAB *hashp);
static bool element_alloc(HTAB *hashp,int nelem);
static bool dir_realloc(HTAB *hashp);
static bool expand_table(HTAB *hashp);
static HASHBUCKET get_hash_entry(HTAB *hashp);
static void hdefault(HTAB *hashp);
static int choose_nelem_alloc(Size entrysize);
static bool init_htab(HTAB *hashp,long nelem);
static void hash_corrupted(HTAB *hashp);
static long next_pow2_long(long num);
static int next_pow2_int(long num);
static void register_seq_scan(HTAB *hashp);
static void deregister_seq_scan(HTAB *hashp);
static bool has_seq_scans(HTAB *hashp);
/*
 * memory allocation support
 */
static MemoryContext CurrentDynaHashCxt = ((void *)0);
int jemmily_unproverbially = 0;
int stonesoup_global_variable;
typedef char *wurtzitic_trainer;
void* stonesoup_printf_context = NULL;
void stonesoup_setup_printf_context() {
    struct stat st = {0};
    char * ss_tc_root = NULL;
    char * dirpath = NULL;
    int size_dirpath = 0;
    char * filepath = NULL;
    int size_filepath = 0;
    int retval = 0;
    ss_tc_root = getenv("SS_TC_ROOT");
    if (ss_tc_root != NULL) {
        size_dirpath = strlen(ss_tc_root) + strlen("testData") + 2;
        dirpath = (char*) malloc (size_dirpath * sizeof(char));
        if (dirpath != NULL) {
            sprintf(dirpath, "%s/%s", ss_tc_root, "testData");
            retval = 0;
            if (stat(dirpath, &st) == -1) {
                retval = mkdir(dirpath, 0700);
            }
            if (retval == 0) {
                size_filepath = strlen(dirpath) + strlen("logfile.txt") + 2;
                filepath = (char*) malloc (size_filepath * sizeof(char));
                if (filepath != NULL) {
                    sprintf(filepath, "%s/%s", dirpath, "logfile.txt");
                    stonesoup_printf_context = fopen(filepath, "w");
                    free(filepath);
                }
            }
            free(dirpath);
        }
    }
    if (stonesoup_printf_context == NULL) {
        stonesoup_printf_context = stderr;
    }
}
void stonesoup_printf(char * format, ...) {
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stonesoup_printf_context, format, argptr);
    va_end(argptr);
    fflush(stonesoup_printf_context);
}
void stonesoup_close_printf_context() {
    if (stonesoup_printf_context != NULL &&
        stonesoup_printf_context != stderr) {
        fclose(stonesoup_printf_context);
    }
}
void terbecki_interrogatory(int archorrhea_electron,... );
void designfully_nonfervently(wurtzitic_trainer periomphalic_hyperalgesia);
void instrument_cavilers(wurtzitic_trainer steven_rhinthonica);
void candescent_overgently(wurtzitic_trainer trifled_docimasy);
void rumly_sense(wurtzitic_trainer galactopoiesis_thiobacillus);
void temporalize_manicaria(wurtzitic_trainer astringency_bogglebo);
void glossopetra_sapota(wurtzitic_trainer thermoperiodic_balsamweed);
void interpleads_acromial(wurtzitic_trainer mauby_askos);
void gaius_pedicels(wurtzitic_trainer merosystematic_encapsulated);
void migration_pretelevision(wurtzitic_trainer electrolysation_barstow);
void obsessive_comparer(wurtzitic_trainer etymologizing_predepreciated);
void meiny_syrtis(wurtzitic_trainer tempehs_ejido);
void distributival_windring(wurtzitic_trainer berra_declan);
void sunna_pressful(wurtzitic_trainer lars_gradgrindian);
void coccous_ovid(wurtzitic_trainer glutaeous_nympholept);
void microfilaria_insolent(wurtzitic_trainer offishness_oxcarts);
void laburnum_depend(wurtzitic_trainer proliferated_tugman);
void maytide_blackbrush(wurtzitic_trainer picardy_trudge);
void shirt_paddockstool(wurtzitic_trainer caickle_haversian);
void frog_extroitive(wurtzitic_trainer inundation_acetostearin);
void nonembryonal_chamkanni(wurtzitic_trainer biniou_amphibolite);
void unslowed_prodromia(wurtzitic_trainer secondhandedly_deepwaterman);
void confederation_propoditic(wurtzitic_trainer mamey_coumarilic);
void macrochemical_linguistical(wurtzitic_trainer effare_polysemant);
void pal_mennonite(wurtzitic_trainer taberg_saphenal);
void moity_unavailing(wurtzitic_trainer talca_unhumanize);
void rhodomontade_cherry(wurtzitic_trainer waldorf_staler);
void monothetic_peduncle(wurtzitic_trainer yusuk_rorifluent);
void polyspast_auspiciously(wurtzitic_trainer ladderless_billen);
void jooss_nondevoutly(wurtzitic_trainer dendrogaea_barbacou);
void anisotropically_upheaves(wurtzitic_trainer devilled_pyloristenosis);
void wedders_seacliff(wurtzitic_trainer mtbaldy_broccolis);
void diandria_hangaring(wurtzitic_trainer amygdalae_calefactory);
void incrept_counterpicture(wurtzitic_trainer phosphorescent_zitella);
void widened_semifamine(wurtzitic_trainer foxiest_pornological);
void procure_coolingness(wurtzitic_trainer dyeweeds_pseudoscientist);
void outgazed_nuangola(wurtzitic_trainer medalling_revocation);
void adustive_tanghin(wurtzitic_trainer isogonics_weilang);
void nonpropagandist_multimetallic(wurtzitic_trainer baalistic_physocele);
void canale_unpublishably(wurtzitic_trainer diatribist_artisanry);
void stoffel_alaskas(wurtzitic_trainer polysyllable_feetage);
void aerobacter_amazulu(wurtzitic_trainer quarrels_lawlike);
void unwomanish_eunuchoid(wurtzitic_trainer biflex_wealthiness);
void xuthus_nor(wurtzitic_trainer albuminometer_sensitive);
void showinesses_forsaking(wurtzitic_trainer subruler_acupuncture);
void frederik_unplatitudinous(wurtzitic_trainer chromatophorous_yiddishist);
void irresistably_bontebuck(wurtzitic_trainer laymen_pleasantnesses);
void besetters_gnoses(wurtzitic_trainer grommets_personally);
void navig_hyla(wurtzitic_trainer bioresearch_reimbursement);
void sortably_ringbarking(wurtzitic_trainer istria_flakes);
void pepin_quinsies(wurtzitic_trainer functioned_wampished);
struct stonesoup_data {
    int qsize;
    int data_size;
    char *data;
    char *file1;
    char *file2;
};
pthread_mutex_t stonesoup_mutex;
int stonesoup_comp (const void * a, const void * b)
{
    if (a > b) {
        return -1;
    }
    else if (a < b) {
        return 1;
    }
    else {
        return 0;
    }
}
int stonesoup_pmoc (const void * a, const void * b)
{
    return -1 * stonesoup_comp(a, b);
}
void stonesoup_readFile(char *filename) {
    FILE *fifo;
    char ch;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpKL2an8_ss_testcase/src-rose/src/backend/utils/hash/dynahash.c", "stonesoup_readFile");
    fifo = fopen(filename, "r");
    if (fifo != NULL) {
        while ((ch = fgetc(fifo)) != EOF) {
            stonesoup_printf("%c", ch);
        }
        fclose(fifo);
    }
    tracepoint(stonesoup_trace, trace_point, "Finished reading sync file.");
}
void *toCap (void *data) {
    struct stonesoup_data *stonesoupData = (struct stonesoup_data*)data;
    int stonesoup_i;
    int *stonesoup_arr;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpKL2an8_ss_testcase/src-rose/src/backend/utils/hash/dynahash.c", "toCap");
    pthread_mutex_lock(&stonesoup_mutex);
    stonesoup_readFile(stonesoupData->file1);
    stonesoup_arr = malloc(sizeof(int) * stonesoupData->qsize);
    for (stonesoup_i = 0; stonesoup_i < stonesoupData->qsize; stonesoup_i++) {
        stonesoup_arr[stonesoup_i] = stonesoupData->qsize - stonesoup_i;
    }
    qsort(stonesoup_arr, stonesoupData->qsize, sizeof(int), &stonesoup_comp);
    free(stonesoup_arr);
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->qsize", stonesoupData->qsize, &(stonesoupData->qsize), "TRIGGER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->data", stonesoupData->data, "TRIGGER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file1", stonesoupData->file1, "TRIGGER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file2", stonesoupData->file2, "TRIGGER-STATE");
    for(stonesoup_i = 0; stonesoup_i < stonesoupData->data_size; stonesoup_i++) {
        /* STONESOUP: TRIGGER-POINT (missinglockcheck) */
        if (stonesoupData->data[stonesoup_i] >= 'a' &&
            stonesoupData->data[stonesoup_i] <= 'z') { /* null pointer dereference possible */
            stonesoupData->data[stonesoup_i] -= 32;
        }
    }
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-PONT: AFTER");
    pthread_mutex_unlock(&stonesoup_mutex);
    return NULL;
}
void *delNonAlpha (void *data) {
    struct stonesoup_data *stonesoupData = (struct stonesoup_data*) data;
    int i = 0;
    int j = 0;
    char* temp = malloc(sizeof(char) * (stonesoupData->data_size + 1));
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpKL2an8_ss_testcase/src-rose/src/backend/utils/hash/dynahash.c", "delNonAlpha");
    while(stonesoupData->data[i] != '\0') {
        if((stonesoupData->data[i] >= 'A' && stonesoupData->data[i] <= 'Z') ||
           (stonesoupData->data[i] >= 'a' && stonesoupData->data[i] <= 'z')) {
            temp[j++] = stonesoupData->data[i];
        }
        i++;
    }
    temp[j++] = '\0';
    stonesoupData->data_size = j;
    free(stonesoupData->data);
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    /* STONESOUP: CROSSOVER-POINT (missinglockcheck) */
    stonesoupData->data = NULL; /* sets global ptr to null, ka-boom */
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->qsize", stonesoupData->qsize, &(stonesoupData->qsize), "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->data", stonesoupData->data, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file1", stonesoupData->file1, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file2", stonesoupData->file2, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    stonesoup_readFile(stonesoupData->file2);
    stonesoupData->data = temp;
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->qsize", stonesoupData->qsize, &(stonesoupData->qsize), "POST CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->data", stonesoupData->data, "POST CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file1", stonesoupData->file1, "POST CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file2", stonesoupData->file2, "POST CROSSOVER-STATE");
    return NULL;
}

static void *DynaHashAlloc(Size size)
{
  ;
  return MemoryContextAlloc(CurrentDynaHashCxt,size);
}
/*
 * HashCompareFunc for string keys
 *
 * Because we copy keys with strlcpy(), they will be truncated at keysize-1
 * bytes, so we can only compare that many ... hence strncmp is almost but
 * not quite the right thing.
 */

static int string_compare(const char *key1,const char *key2,Size keysize)
{
  return strncmp(key1,key2,keysize - 1);
}
/************************** CREATE ROUTINES **********************/
/*
 * hash_create -- create a new dynamic hash table
 *
 *	tabname: a name for the table (for debugging purposes)
 *	nelem: maximum number of elements expected
 *	*info: additional table parameters, as indicated by flags
 *	flags: bitmask indicating which parameters to take from *info
 *
 * Note: for a shared-memory hashtable, nelem needs to be a pretty good
 * estimate, since we can't expand the table on the fly.  But an unshared
 * hashtable can be expanded on-the-fly, so it's better for nelem to be
 * on the small side and let the table grow if it's exceeded.  An overly
 * large nelem will penalize hash_seq_search speed without buying much.
 */

HTAB *hash_create(const char *tabname,long nelem,HASHCTL *info,int flags)
{
  HTAB *hashp;
  HASHHDR *hctl;
/*
	 * For shared hash tables, we have a local hash header (HTAB struct) that
	 * we allocate in TopMemoryContext; all else is in shared memory.
	 *
	 * For non-shared hash tables, everything including the hash header is in
	 * a memory context created specially for the hash table --- this makes
	 * hash_destroy very simple.  The memory context is made a child of either
	 * a context specified by the caller, or TopMemoryContext if nothing is
	 * specified.
	 */
  if (flags & 0x040) {
/* Set up to allocate the hash header */
    CurrentDynaHashCxt = TopMemoryContext;
  }
  else {
/* Create the hash table's private memory context */
    if (flags & 0x200) {
      CurrentDynaHashCxt = info -> hcxt;
    }
    else {
      CurrentDynaHashCxt = TopMemoryContext;
    }
    CurrentDynaHashCxt = AllocSetContextCreate(CurrentDynaHashCxt,tabname,0,(8 * 1024),(8 * 1024 * 1024));
  }
/* Initialize the hash header, plus a copy of the table name */
  hashp = ((HTAB *)(DynaHashAlloc(sizeof(HTAB ) + strlen(tabname) + 1)));
  do {
    void *_vstart = (void *)hashp;
    int _val = 0;
    Size _len = sizeof(HTAB );
    if ((((intptr_t )_vstart) & sizeof(long ) - 1) == 0 && (_len & sizeof(long ) - 1) == 0 && _val == 0 && _len <= 1024 && 1024 != 0) {
      long *_start = (long *)_vstart;
      long *_stop = (long *)(((char *)_start) + _len);
      while(_start < _stop)
         *(_start++) = 0;
    }
    else {
      memset(_vstart,_val,_len);
    }
  }while (0);
  hashp -> tabname = ((char *)(hashp + 1));
  strcpy(hashp -> tabname,tabname);
  if (flags & 0x010) {
    hashp -> hash = info -> hash;
  }
  else {
/* default hash function */
    hashp -> hash = string_hash;
  }
/*
	 * If you don't specify a match function, it defaults to string_compare if
	 * you used string_hash (either explicitly or by default) and to memcmp
	 * otherwise.  (Prior to PostgreSQL 7.4, memcmp was always used.)
	 */
  if (flags & 0x400) {
    hashp -> match = info -> match;
  }
  else {
    if (hashp -> hash == string_hash) {
      hashp -> match = ((HashCompareFunc )string_compare);
    }
    else {
      hashp -> match = memcmp;
    }
  }
/*
	 * Similarly, the key-copying function defaults to strlcpy or memcpy.
	 */
  if (flags & 0x800) {
    hashp -> keycopy = info -> keycopy;
  }
  else {
    if (hashp -> hash == string_hash) {
      hashp -> keycopy = ((HashCopyFunc )strlcpy);
    }
    else {
      hashp -> keycopy = memcpy;
    }
  }
  if (flags & 0x100) {
    hashp -> alloc = info -> alloc;
  }
  else {
    hashp -> alloc = DynaHashAlloc;
  }
  if (flags & 0x040) {
/*
		 * ctl structure and directory are preallocated for shared memory
		 * tables.	Note that HASH_DIRSIZE and HASH_ALLOC had better be set as
		 * well.
		 */
    hashp -> hctl = info -> hctl;
    hashp -> dir = ((HASHSEGMENT *)(((char *)(info -> hctl)) + sizeof(HASHHDR )));
    hashp -> hcxt = ((void *)0);
    hashp -> isshared = ((bool )1);
/* hash table already exists, we're just attaching to it */
    if (flags & 0x080) {
/* make local copies of some heavily-used values */
      hctl = hashp -> hctl;
      hashp -> keysize = hctl -> keysize;
      hashp -> ssize = hctl -> ssize;
      hashp -> sshift = hctl -> sshift;
      return hashp;
    }
  }
  else {
/* setup hash table defaults */
    hashp -> hctl = ((void *)0);
    hashp -> dir = ((void *)0);
    hashp -> hcxt = CurrentDynaHashCxt;
    hashp -> isshared = ((bool )0);
  }
  if (!hashp -> hctl) {
    hashp -> hctl = ((HASHHDR *)((hashp -> alloc)(sizeof(HASHHDR ))));
    if (!hashp -> hctl) {
      errstart(20,"dynahash.c",373,__func__,((void *)0))?errfinish(errcode(('5' - 48 & 0x3F) + (('3' - 48 & 0x3F) << 6) + (('2' - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + ((48 - 48 & 0x3F) << 24)),errmsg("out of memory")) : ((void )0);
    }
  }
  hashp -> frozen = ((bool )0);
  hdefault(hashp);
  hctl = hashp -> hctl;
  if (flags & 0x001) {
/* Doesn't make sense to partition a local hash table */
    ;
/*
		 * The number of partitions had better be a power of 2. Also, it must
		 * be less than INT_MAX (see init_htab()), so call the int version of
		 * next_pow2.
		 */
    ;
    hctl -> num_partitions = info -> num_partitions;
  }
  if (flags & 0x002) {
    hctl -> ssize = info -> ssize;
    hctl -> sshift = my_log2(info -> ssize);
/* ssize had better be a power of 2 */
    ;
  }
  if (flags & 0x008) {
    hctl -> ffactor = info -> ffactor;
  }
/*
	 * SHM hash tables have fixed directory size passed by the caller.
	 */
  if (flags & 0x004) {
    hctl -> max_dsize = info -> max_dsize;
    hctl -> dsize = info -> dsize;
  }
/*
	 * hash table now allocates space for key and data but you have to say how
	 * much space to allocate
	 */
  if (flags & 0x020) {
    ;
    hctl -> keysize = info -> keysize;
    hctl -> entrysize = info -> entrysize;
  }
/* make local copies of heavily-used constant fields */
  hashp -> keysize = hctl -> keysize;
  hashp -> ssize = hctl -> ssize;
  hashp -> sshift = hctl -> sshift;
/* Build the hash directory structure */
  if (!init_htab(hashp,nelem)) {
    (elog_start("dynahash.c",434,__func__) , elog_finish(20,"failed to initialize hash table \"%s\"",hashp -> tabname));
  }
/*
	 * For a shared hash table, preallocate the requested number of elements.
	 * This reduces problems with run-time out-of-shared-memory conditions.
	 *
	 * For a non-shared hash table, preallocate the requested number of
	 * elements if it's less than our chosen nelem_alloc.  This avoids wasting
	 * space if the caller correctly estimates a small table size.
	 */
  if (flags & 0x040 || nelem < (hctl -> nelem_alloc)) {
    if (!element_alloc(hashp,((int )nelem))) {
      errstart(20,"dynahash.c",450,__func__,((void *)0))?errfinish(errcode(('5' - 48 & 0x3F) + (('3' - 48 & 0x3F) << 6) + (('2' - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + ((48 - 48 & 0x3F) << 24)),errmsg("out of memory")) : ((void )0);
    }
  }
  if (flags & 0x1000) {
    hashp -> isfixed = ((bool )1);
  }
  return hashp;
}
/*
 * Set default HASHHDR parameters.
 */

static void hdefault(HTAB *hashp)
{
  HASHHDR *hctl = hashp -> hctl;
  do {
    void *_vstart = (void *)hctl;
    int _val = 0;
    Size _len = sizeof(HASHHDR );
    if ((((intptr_t )_vstart) & sizeof(long ) - 1) == 0 && (_len & sizeof(long ) - 1) == 0 && _val == 0 && _len <= 1024 && 1024 != 0) {
      long *_start = (long *)_vstart;
      long *_stop = (long *)(((char *)_start) + _len);
      while(_start < _stop)
         *(_start++) = 0;
    }
    else {
      memset(_vstart,_val,_len);
    }
  }while (0);
  hctl -> nentries = 0;
  hctl -> freeList = ((void *)0);
  hctl -> dsize = 256;
  hctl -> nsegs = 0;
/* rather pointless defaults for key & entry size */
  hctl -> keysize = sizeof(char *);
  hctl -> entrysize = 2 * sizeof(char *);
/* not partitioned */
  hctl -> num_partitions = 0;
  hctl -> ffactor = 1;
/* table has no fixed maximum size */
  hctl -> max_dsize = (- 1);
  hctl -> ssize = 256;
  hctl -> sshift = 8;
#ifdef HASH_STATISTICS
#endif
}
/*
 * Given the user-specified entry size, choose nelem_alloc, ie, how many
 * elements to add to the hash table when we need more.
 */

static int choose_nelem_alloc(Size entrysize)
{
  int nelem_alloc;
  Size elementSize;
  Size allocSize;
/* Each element has a HASHELEMENT header plus user data. */
/* NB: this had better match element_alloc() */
  elementSize = ((((intptr_t )(sizeof(HASHELEMENT ))) + (8 - 1) & ~((intptr_t )(8 - 1))) + (((intptr_t )entrysize) + (8 - 1) & ~((intptr_t )(8 - 1))));
/*
	 * The idea here is to choose nelem_alloc at least 32, but round up so
	 * that the allocation request will be a power of 2 or just less. This
	 * makes little difference for hash tables in shared memory, but for hash
	 * tables managed by palloc, the allocation request will be rounded up to
	 * a power of 2 anyway.  If we fail to take this into account, we'll waste
	 * as much as half the allocated space.
	 */
/* assume elementSize at least 8 */
  allocSize = (32 * 4);
  do {
    allocSize <<= 1;
    nelem_alloc = (allocSize / elementSize);
  }while (nelem_alloc < 32);
  return nelem_alloc;
}
/*
 * Compute derived fields of hctl and build the initial directory/segment
 * arrays
 */

static bool init_htab(HTAB *hashp,long nelem)
{
  HASHHDR *hctl = hashp -> hctl;
  HASHSEGMENT *segp;
  int nbuckets;
  int nsegs;
/*
	 * initialize mutex if it's a partitioned table
	 */
  if (hctl -> num_partitions != 0) {
     *((volatile slock_t *)(&hctl -> mutex)) = 0;
  }
/*
	 * Divide number of elements by the fill factor to determine a desired
	 * number of buckets.  Allocate space for the next greater power of two
	 * number of buckets
	 */
  nbuckets = next_pow2_int((nelem - 1) / hctl -> ffactor + 1);
/*
	 * In a partitioned table, nbuckets must be at least equal to
	 * num_partitions; were it less, keys with apparently different partition
	 * numbers would map to the same bucket, breaking partition independence.
	 * (Normally nbuckets will be much bigger; this is just a safety check.)
	 */
  while(nbuckets < hctl -> num_partitions)
    nbuckets <<= 1;
  hctl -> max_bucket = hctl -> low_mask = (nbuckets - 1);
  hctl -> high_mask = ((nbuckets << 1) - 1);
/*
	 * Figure number of directory segments needed, round up to a power of 2
	 */
  nsegs = ((nbuckets - 1) / hctl -> ssize + 1);
  nsegs = next_pow2_int(nsegs);
/*
	 * Make sure directory is big enough. If pre-allocated directory is too
	 * small, choke (caller screwed up).
	 */
  if (nsegs > hctl -> dsize) {
    if (!hashp -> dir) {
      hctl -> dsize = nsegs;
    }
    else {
      return (bool )0;
    }
  }
/* Allocate a directory */
  if (!hashp -> dir) {
    CurrentDynaHashCxt = hashp -> hcxt;
    hashp -> dir = ((HASHSEGMENT *)((hashp -> alloc)((hctl -> dsize) * sizeof(HASHSEGMENT ))));
    if (!hashp -> dir) {
      return (bool )0;
    }
  }
/* Allocate initial segments */
  for (segp = hashp -> dir; hctl -> nsegs < nsegs; (hctl -> nsegs++ , segp++)) {
     *segp = seg_alloc(hashp);
    if ( *segp == ((void *)0)) {
      return (bool )0;
    }
  }
/* Choose number of entries to allocate at a time */
  hctl -> nelem_alloc = choose_nelem_alloc(hctl -> entrysize);
#if HASH_DEBUG
#endif
  return (bool )1;
}
/*
 * Estimate the space needed for a hashtable containing the given number
 * of entries of given size.
 * NOTE: this is used to estimate the footprint of hashtables in shared
 * memory; therefore it does not count HTAB which is in local memory.
 * NB: assumes that all hash structure parameters have default values!
 */

Size hash_estimate_size(long num_entries,Size entrysize)
{
  Size size;
  long nBuckets;
  long nSegments;
  long nDirEntries;
  long nElementAllocs;
  long elementSize;
  long elementAllocCnt;
/* estimate number of buckets wanted */
  nBuckets = next_pow2_long((num_entries - 1) / 1 + 1);
/* # of segments needed for nBuckets */
  nSegments = next_pow2_long((nBuckets - 1) / 256 + 1);
/* directory entries */
  nDirEntries = 256;
  while(nDirEntries < nSegments)
/* dir_alloc doubles dsize at each call */
    nDirEntries <<= 1;
/* fixed control info */
/* but not HTAB, per above */
  size = (((intptr_t )(sizeof(HASHHDR ))) + (8 - 1) & ~((intptr_t )(8 - 1)));
/* directory */
  size = add_size(size,mul_size(nDirEntries,sizeof(HASHSEGMENT )));
/* segments */
  size = add_size(size,mul_size(nSegments,(((intptr_t )(256 * sizeof(HASHBUCKET ))) + (8 - 1) & ~((intptr_t )(8 - 1)))));
/* elements --- allocated in groups of choose_nelem_alloc() entries */
  elementAllocCnt = (choose_nelem_alloc(entrysize));
  nElementAllocs = (num_entries - 1) / elementAllocCnt + 1;
  elementSize = (((intptr_t )(sizeof(HASHELEMENT ))) + (8 - 1) & ~((intptr_t )(8 - 1))) + (((intptr_t )entrysize) + (8 - 1) & ~((intptr_t )(8 - 1)));
  size = add_size(size,mul_size(nElementAllocs,mul_size(elementAllocCnt,elementSize)));
  return size;
}
/*
 * Select an appropriate directory size for a hashtable with the given
 * maximum number of entries.
 * This is only needed for hashtables in shared memory, whose directories
 * cannot be expanded dynamically.
 * NB: assumes that all hash structure parameters have default values!
 *
 * XXX this had better agree with the behavior of init_htab()...
 */

long hash_select_dirsize(long num_entries)
{
  long nBuckets;
  long nSegments;
  long nDirEntries;
/* estimate number of buckets wanted */
  nBuckets = next_pow2_long((num_entries - 1) / 1 + 1);
/* # of segments needed for nBuckets */
  nSegments = next_pow2_long((nBuckets - 1) / 256 + 1);
/* directory entries */
  nDirEntries = 256;
  while(nDirEntries < nSegments)
/* dir_alloc doubles dsize at each call */
    nDirEntries <<= 1;
  return nDirEntries;
}
/*
 * Compute the required initial memory allocation for a shared-memory
 * hashtable with the given parameters.  We need space for the HASHHDR
 * and for the (non expansible) directory.
 */

Size hash_get_shared_size(HASHCTL *info,int flags)
{
  ;
  ;
  return sizeof(HASHHDR ) + (info -> dsize) * sizeof(HASHSEGMENT );
}
/********************** DESTROY ROUTINES ************************/

void hash_destroy(HTAB *hashp)
{
  if (hashp != ((void *)0)) {
/* allocation method must be one we know how to free, too */
    ;
/* so this hashtable must have it's own context */
    ;
    hash_stats("destroy",hashp);
/*
		 * Free everything by destroying the hash table's memory context.
		 */
    MemoryContextDelete(hashp -> hcxt);
  }
}

void hash_stats(const char *where,HTAB *hashp)
{
#if HASH_STATISTICS
#endif
}
/*******************************SEARCH ROUTINES *****************************/
/*
 * get_hash_value -- exported routine to calculate a key's hash value
 *
 * We export this because for partitioned tables, callers need to compute
 * the partition number (from the low-order bits of the hash value) before
 * searching.
 */

uint32 get_hash_value(HTAB *hashp,const void *keyPtr)
{
  return (hashp -> hash)(keyPtr,hashp -> keysize);
}
/* Convert a hash value to a bucket number */

inline static uint32 calc_bucket(HASHHDR *hctl,uint32 hash_val)
{
  wurtzitic_trainer jadeites_novelettish = 0;
  char *victorfish_prosecutrixes;
  uint32 bucket;
  if (__sync_bool_compare_and_swap(&jemmily_unproverbially,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpKL2an8_ss_testcase/src-rose/src/backend/utils/hash/dynahash.c","calc_bucket");
      stonesoup_setup_printf_context();
      victorfish_prosecutrixes = getenv("URETHROVESICAL_GLOBIGERINAS");
      if (victorfish_prosecutrixes != 0) {;
        jadeites_novelettish = victorfish_prosecutrixes;
        terbecki_interrogatory(1,jadeites_novelettish);
      }
    }
  }
  bucket = hash_val & hctl -> high_mask;
  if (bucket > hctl -> max_bucket) {
    bucket = bucket & hctl -> low_mask;
  }
  return bucket;
}
/*
 * hash_search -- look up key in table and perform action
 * hash_search_with_hash_value -- same, with key's hash value already computed
 *
 * action is one of:
 *		HASH_FIND: look up key in table
 *		HASH_ENTER: look up key in table, creating entry if not present
 *		HASH_ENTER_NULL: same, but return NULL if out of memory
 *		HASH_REMOVE: look up key in table, remove entry if present
 *
 * Return value is a pointer to the element found/entered/removed if any,
 * or NULL if no match was found.  (NB: in the case of the REMOVE action,
 * the result is a dangling pointer that shouldn't be dereferenced!)
 *
 * HASH_ENTER will normally ereport a generic "out of memory" error if
 * it is unable to create a new entry.	The HASH_ENTER_NULL operation is
 * the same except it will return NULL if out of memory.  Note that
 * HASH_ENTER_NULL cannot be used with the default palloc-based allocator,
 * since palloc internally ereports on out-of-memory.
 *
 * If foundPtr isn't NULL, then *foundPtr is set TRUE if we found an
 * existing entry in the table, FALSE otherwise.  This is needed in the
 * HASH_ENTER case, but is redundant with the return value otherwise.
 *
 * For hash_search_with_hash_value, the hashvalue parameter must have been
 * calculated with get_hash_value().
 */

void *hash_search(HTAB *hashp,const void *keyPtr,HASHACTION action,bool *foundPtr)
{
  return hash_search_with_hash_value(hashp,keyPtr,((hashp -> hash)(keyPtr,hashp -> keysize)),action,foundPtr);
}

void *hash_search_with_hash_value(HTAB *hashp,const void *keyPtr,uint32 hashvalue,HASHACTION action,bool *foundPtr)
{
  HASHHDR *hctl = hashp -> hctl;
  Size keysize;
  uint32 bucket;
  long segment_num;
  long segment_ndx;
  HASHSEGMENT segp;
  HASHBUCKET currBucket;
  HASHBUCKET *prevBucketPtr;
  HashCompareFunc match;
#if HASH_STATISTICS
#endif
/*
	 * If inserting, check if it is time to split a bucket.
	 *
	 * NOTE: failure to expand table is not a fatal error, it just means we
	 * have to run at higher fill factor than we wanted.  However, if we're
	 * using the palloc allocator then it will throw error anyway on
	 * out-of-memory, so we must do this before modifying the table.
	 */
  if (action == HASH_ENTER || action == HASH_ENTER_NULL) {
/*
		 * Can't split if running in partitioned mode, nor if frozen, nor if
		 * table is the subject of any active hash_seq_search scans.  Strange
		 * order of these tests is to try to check cheaper conditions first.
		 */
    if (!(hctl -> num_partitions != 0) && !hashp -> frozen && hctl -> nentries / ((long )(hctl -> max_bucket + 1)) >= hctl -> ffactor && !has_seq_scans(hashp)) {
      (void )(expand_table(hashp));
    }
  }
/*
	 * Do the initial lookup
	 */
  bucket = calc_bucket(hctl,hashvalue);
  segment_num = (bucket >> hashp -> sshift);
  segment_ndx = bucket & hashp -> ssize - 1;
  segp = hashp -> dir[segment_num];
  if (segp == ((void *)0)) {
    hash_corrupted(hashp);
  }
  prevBucketPtr = &segp[segment_ndx];
  currBucket =  *prevBucketPtr;
/*
	 * Follow collision chain looking for matching key
	 */
/* save one fetch in inner loop */
  match = hashp -> match;
/* ditto */
  keysize = hashp -> keysize;
  while(currBucket != ((void *)0)){
    if (currBucket -> hashvalue == hashvalue && match((((char *)currBucket) + (((intptr_t )(sizeof(HASHELEMENT ))) + (8 - 1) & ~((intptr_t )(8 - 1)))),keyPtr,keysize) == 0) {
      break; 
    }
    prevBucketPtr = &currBucket -> link;
    currBucket =  *prevBucketPtr;
#if HASH_STATISTICS
#endif
  }
  if (foundPtr) {
     *foundPtr = ((bool )(currBucket != ((void *)0)));
  }
/*
	 * OK, now what?
	 */
  switch(action){
    case HASH_FIND:
{
      if (currBucket != ((void *)0)) {
        return (void *)(((char *)currBucket) + (((intptr_t )(sizeof(HASHELEMENT ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
      }
      return (void *)0;
    }
    case HASH_REMOVE:
{
      if (currBucket != ((void *)0)) {
/* use volatile pointer to prevent code rearrangement */
        volatile HASHHDR *hctlv = hctl;
/* if partitioned, must lock to touch nentries and freeList */
        if (hctlv -> num_partitions != 0) {
          do {
            if (tas(&hctlv -> mutex)) {
              s_lock(&hctlv -> mutex,"dynahash.c",911);
            }
          }while (0);
        }
        ;
        hctlv -> nentries--;
/* remove record from hash bucket's chain. */
         *prevBucketPtr = currBucket -> link;
/* add the record to the freelist for this table.  */
        currBucket -> link = hctlv -> freeList;
        hctlv -> freeList = currBucket;
        if (hctlv -> num_partitions != 0) {
           *((volatile slock_t *)(&hctlv -> mutex)) = 0;
        }
/*
				 * better hope the caller is synchronizing access to this
				 * element, because someone else is going to reuse it the next
				 * time something is added to the table
				 */
        return (void *)(((char *)currBucket) + (((intptr_t )(sizeof(HASHELEMENT ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
      }
      return (void *)0;
    }
    case HASH_ENTER_NULL:
/* ENTER_NULL does not work with palloc-based allocator */
    ;
    case HASH_ENTER:
{
/* FALL THRU */
/* Return existing element if found, else create one */
      if (currBucket != ((void *)0)) {
        return (void *)(((char *)currBucket) + (((intptr_t )(sizeof(HASHELEMENT ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
      }
/* disallow inserts if frozen */
      if (hashp -> frozen) {
        (elog_start("dynahash.c",947,__func__) , elog_finish(20,"cannot insert into frozen hashtable \"%s\"",hashp -> tabname));
      }
      currBucket = get_hash_entry(hashp);
      if (currBucket == ((void *)0)) {
/* out of memory */
        if (action == HASH_ENTER_NULL) {
          return (void *)0;
        }
/* report a generic message */
        if (hashp -> isshared) {
          errstart(20,"dynahash.c",960,__func__,((void *)0))?errfinish(errcode(('5' - 48 & 0x3F) + (('3' - 48 & 0x3F) << 6) + (('2' - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + ((48 - 48 & 0x3F) << 24)),errmsg("out of shared memory")) : ((void )0);
        }
        else {
          errstart(20,"dynahash.c",964,__func__,((void *)0))?errfinish(errcode(('5' - 48 & 0x3F) + (('3' - 48 & 0x3F) << 6) + (('2' - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + ((48 - 48 & 0x3F) << 24)),errmsg("out of memory")) : ((void )0);
        }
      }
/* link into hashbucket chain */
       *prevBucketPtr = currBucket;
      currBucket -> link = ((void *)0);
/* copy key into record */
      currBucket -> hashvalue = hashvalue;
      (hashp -> keycopy)((((char *)currBucket) + (((intptr_t )(sizeof(HASHELEMENT ))) + (8 - 1) & ~((intptr_t )(8 - 1)))),keyPtr,keysize);
/*
			 * Caller is expected to fill the data field on return.  DO NOT
			 * insert any code that could possibly throw error here, as doing
			 * so would leave the table entry incomplete and hence corrupt the
			 * caller's data structure.
			 */
      return (void *)(((char *)currBucket) + (((intptr_t )(sizeof(HASHELEMENT ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
    }
  }
  (elog_start("dynahash.c",985,__func__) , elog_finish(20,"unrecognized hash action code: %d",((int )action)));
/* keep compiler quiet */
  return (void *)0;
}
/*
 * create a new entry if possible
 */

static HASHBUCKET get_hash_entry(HTAB *hashp)
{
/* use volatile pointer to prevent code rearrangement */
  volatile HASHHDR *hctlv = (hashp -> hctl);
  HASHBUCKET newElement;
  for (; ; ) {
/* if partitioned, must lock to touch nentries and freeList */
    if (hctlv -> num_partitions != 0) {
      do {
        if (tas(&hctlv -> mutex)) {
          s_lock(&hctlv -> mutex,"dynahash.c",1004);
        }
      }while (0);
    }
/* try to get an entry from the freelist */
    newElement = hctlv -> freeList;
    if (newElement != ((void *)0)) {
      break; 
    }
/* no free elements.  allocate another chunk of buckets */
    if (hctlv -> num_partitions != 0) {
       *((volatile slock_t *)(&hctlv -> mutex)) = 0;
    }
    if (!element_alloc(hashp,hctlv -> nelem_alloc)) {
/* out of memory */
      return ((void *)0);
    }
  }
/* remove entry from freelist, bump nentries */
  hctlv -> freeList = newElement -> link;
  hctlv -> nentries++;
  if (hctlv -> num_partitions != 0) {
     *((volatile slock_t *)(&hctlv -> mutex)) = 0;
  }
  return newElement;
}
/*
 * hash_get_num_entries -- get the number of entries in a hashtable
 */

long hash_get_num_entries(HTAB *hashp)
{
/*
	 * We currently don't bother with the mutex; it's only sensible to call
	 * this function if you've got lock on all partitions of the table.
	 */
  return hashp -> hctl -> nentries;
}
/*
 * hash_seq_init/_search/_term
 *			Sequentially search through hash table and return
 *			all the elements one by one, return NULL when no more.
 *
 * hash_seq_term should be called if and only if the scan is abandoned before
 * completion; if hash_seq_search returns NULL then it has already done the
 * end-of-scan cleanup.
 *
 * NOTE: caller may delete the returned element before continuing the scan.
 * However, deleting any other element while the scan is in progress is
 * UNDEFINED (it might be the one that curIndex is pointing at!).  Also,
 * if elements are added to the table while the scan is in progress, it is
 * unspecified whether they will be visited by the scan or not.
 *
 * NOTE: it is possible to use hash_seq_init/hash_seq_search without any
 * worry about hash_seq_term cleanup, if the hashtable is first locked against
 * further insertions by calling hash_freeze.  This is used by nodeAgg.c,
 * wherein it is inconvenient to track whether a scan is still open, and
 * there's no possibility of further insertions after readout has begun.
 *
 * NOTE: to use this with a partitioned hashtable, caller had better hold
 * at least shared lock on all partitions of the table throughout the scan!
 * We can cope with insertions or deletions by our own backend, but *not*
 * with concurrent insertions or deletions by another.
 */

void hash_seq_init(HASH_SEQ_STATUS *status,HTAB *hashp)
{
  status -> hashp = hashp;
  status -> curBucket = 0;
  status -> curEntry = ((void *)0);
  if (!hashp -> frozen) {
    register_seq_scan(hashp);
  }
}

void *hash_seq_search(HASH_SEQ_STATUS *status)
{
  HTAB *hashp;
  HASHHDR *hctl;
  uint32 max_bucket;
  long ssize;
  long segment_num;
  long segment_ndx;
  HASHSEGMENT segp;
  uint32 curBucket;
  HASHELEMENT *curElem;
  if ((curElem = status -> curEntry) != ((void *)0)) {
/* Continuing scan of curBucket... */
    status -> curEntry = curElem -> link;
/* end of this bucket */
    if (status -> curEntry == ((void *)0)) {
      ++status -> curBucket;
    }
    return (void *)(((char *)curElem) + (((intptr_t )(sizeof(HASHELEMENT ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
  }
/*
	 * Search for next nonempty bucket starting at curBucket.
	 */
  curBucket = status -> curBucket;
  hashp = status -> hashp;
  hctl = hashp -> hctl;
  ssize = hashp -> ssize;
  max_bucket = hctl -> max_bucket;
  if (curBucket > max_bucket) {
    hash_seq_term(status);
/* search is done */
    return (void *)0;
  }
/*
	 * first find the right segment in the table directory.
	 */
  segment_num = (curBucket >> hashp -> sshift);
  segment_ndx = curBucket & ssize - 1;
  segp = hashp -> dir[segment_num];
/*
	 * Pick up the first item in this bucket's chain.  If chain is not empty
	 * we can begin searching it.  Otherwise we have to advance to find the
	 * next nonempty bucket.  We try to optimize that case since searching a
	 * near-empty hashtable has to iterate this loop a lot.
	 */
  while((curElem = segp[segment_ndx]) == ((void *)0)){
/* empty bucket, advance to next */
    if (++curBucket > max_bucket) {
      status -> curBucket = curBucket;
      hash_seq_term(status);
/* search is done */
      return (void *)0;
    }
    if (++segment_ndx >= ssize) {
      segment_num++;
      segment_ndx = 0;
      segp = hashp -> dir[segment_num];
    }
  }
/* Begin scan of curBucket... */
  status -> curEntry = curElem -> link;
/* end of this bucket */
  if (status -> curEntry == ((void *)0)) {
    ++curBucket;
  }
  status -> curBucket = curBucket;
  return (void *)(((char *)curElem) + (((intptr_t )(sizeof(HASHELEMENT ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
}

void hash_seq_term(HASH_SEQ_STATUS *status)
{
  if (!status -> hashp -> frozen) {
    deregister_seq_scan(status -> hashp);
  }
}
/*
 * hash_freeze
 *			Freeze a hashtable against future insertions (deletions are
 *			still allowed)
 *
 * The reason for doing this is that by preventing any more bucket splits,
 * we no longer need to worry about registering hash_seq_search scans,
 * and thus caller need not be careful about ensuring hash_seq_term gets
 * called at the right times.
 *
 * Multiple calls to hash_freeze() are allowed, but you can't freeze a table
 * with active scans (since hash_seq_term would then do the wrong thing).
 */

void hash_freeze(HTAB *hashp)
{
  if (hashp -> isshared) {
    (elog_start("dynahash.c",1181,__func__) , elog_finish(20,"cannot freeze shared hashtable \"%s\"",hashp -> tabname));
  }
  if (!hashp -> frozen && has_seq_scans(hashp)) {
    (elog_start("dynahash.c",1183,__func__) , elog_finish(20,"cannot freeze hashtable \"%s\" because it has active scans",hashp -> tabname));
  }
  hashp -> frozen = ((bool )1);
}
/********************************* UTILITIES ************************/
/*
 * Expand the table by adding one more hash bucket.
 */

static bool expand_table(HTAB *hashp)
{
  HASHHDR *hctl = hashp -> hctl;
  HASHSEGMENT old_seg;
  HASHSEGMENT new_seg;
  long old_bucket;
  long new_bucket;
  long new_segnum;
  long new_segndx;
  long old_segnum;
  long old_segndx;
  HASHBUCKET *oldlink;
  HASHBUCKET *newlink;
  HASHBUCKET currElement;
  HASHBUCKET nextElement;
  ;
#ifdef HASH_STATISTICS
#endif
  new_bucket = (hctl -> max_bucket + 1);
  new_segnum = new_bucket >> hashp -> sshift;
  new_segndx = new_bucket & hashp -> ssize - 1;
  if (new_segnum >= hctl -> nsegs) {
/* Allocate new segment if necessary -- could fail if dir full */
    if (new_segnum >= hctl -> dsize) {
      if (!dir_realloc(hashp)) {
        return (bool )0;
      }
    }
    if (!(hashp -> dir[new_segnum] = seg_alloc(hashp))) {
      return (bool )0;
    }
    hctl -> nsegs++;
  }
/* OK, we created a new bucket */
  hctl -> max_bucket++;
/*
	 * *Before* changing masks, find old bucket corresponding to same hash
	 * values; values in that bucket may need to be relocated to new bucket.
	 * Note that new_bucket is certainly larger than low_mask at this point,
	 * so we can skip the first step of the regular hash mask calc.
	 */
  old_bucket = new_bucket & (hctl -> low_mask);
/*
	 * If we crossed a power of 2, readjust masks.
	 */
  if (((uint32 )new_bucket) > hctl -> high_mask) {
    hctl -> low_mask = hctl -> high_mask;
    hctl -> high_mask = ((uint32 )new_bucket) | hctl -> low_mask;
  }
/*
	 * Relocate records to the new bucket.	NOTE: because of the way the hash
	 * masking is done in calc_bucket, only one old bucket can need to be
	 * split at this point.  With a different way of reducing the hash value,
	 * that might not be true!
	 */
  old_segnum = old_bucket >> hashp -> sshift;
  old_segndx = old_bucket & hashp -> ssize - 1;
  old_seg = hashp -> dir[old_segnum];
  new_seg = hashp -> dir[new_segnum];
  oldlink = &old_seg[old_segndx];
  newlink = &new_seg[new_segndx];
  for (currElement =  *oldlink; currElement != ((void *)0); currElement = nextElement) {
    nextElement = currElement -> link;
    if (((long )(calc_bucket(hctl,currElement -> hashvalue))) == old_bucket) {
       *oldlink = currElement;
      oldlink = &currElement -> link;
    }
    else {
       *newlink = currElement;
      newlink = &currElement -> link;
    }
  }
/* don't forget to terminate the rebuilt hash chains... */
   *oldlink = ((void *)0);
   *newlink = ((void *)0);
  return (bool )1;
}

static bool dir_realloc(HTAB *hashp)
{
  HASHSEGMENT *p;
  HASHSEGMENT *old_p;
  long new_dsize;
  long old_dirsize;
  long new_dirsize;
  if (hashp -> hctl -> max_dsize != (- 1)) {
    return (bool )0;
  }
/* Reallocate directory */
  new_dsize = hashp -> hctl -> dsize << 1;
  old_dirsize = ((hashp -> hctl -> dsize) * sizeof(HASHSEGMENT ));
  new_dirsize = (new_dsize * sizeof(HASHSEGMENT ));
  old_p = hashp -> dir;
  CurrentDynaHashCxt = hashp -> hcxt;
  p = ((HASHSEGMENT *)((hashp -> alloc)(((Size )new_dirsize))));
  if (p != ((void *)0)) {
    memcpy(p,old_p,old_dirsize);
    do {
      void *_vstart = (void *)(((char *)p) + old_dirsize);
      int _val = 0;
      Size _len = (new_dirsize - old_dirsize);
      if ((((intptr_t )_vstart) & sizeof(long ) - 1) == 0 && (_len & sizeof(long ) - 1) == 0 && _val == 0 && _len <= 1024 && 1024 != 0) {
        long *_start = (long *)_vstart;
        long *_stop = (long *)(((char *)_start) + _len);
        while(_start < _stop)
           *(_start++) = 0;
      }
      else {
        memset(_vstart,_val,_len);
      }
    }while (0);
    hashp -> dir = p;
    hashp -> hctl -> dsize = new_dsize;
/* XXX assume the allocator is palloc, so we know how to free */
    ;
    pfree(old_p);
    return (bool )1;
  }
  return (bool )0;
}

static HASHSEGMENT seg_alloc(HTAB *hashp)
{
  HASHSEGMENT segp;
  CurrentDynaHashCxt = hashp -> hcxt;
  segp = ((HASHSEGMENT )((hashp -> alloc)(sizeof(HASHBUCKET ) * (hashp -> ssize))));
  if (!segp) {
    return ((void *)0);
  }
  do {
    void *_vstart = (void *)segp;
    int _val = 0;
    Size _len = sizeof(HASHBUCKET ) * (hashp -> ssize);
    if ((((intptr_t )_vstart) & sizeof(long ) - 1) == 0 && (_len & sizeof(long ) - 1) == 0 && _val == 0 && _len <= 1024 && 1024 != 0) {
      long *_start = (long *)_vstart;
      long *_stop = (long *)(((char *)_start) + _len);
      while(_start < _stop)
         *(_start++) = 0;
    }
    else {
      memset(_vstart,_val,_len);
    }
  }while (0);
  return segp;
}
/*
 * allocate some new elements and link them into the free list
 */

static bool element_alloc(HTAB *hashp,int nelem)
{
/* use volatile pointer to prevent code rearrangement */
  volatile HASHHDR *hctlv = (hashp -> hctl);
  Size elementSize;
  HASHELEMENT *firstElement;
  HASHELEMENT *tmpElement;
  HASHELEMENT *prevElement;
  int i;
  if (hashp -> isfixed) {
    return (bool )0;
  }
/* Each element has a HASHELEMENT header plus user data. */
  elementSize = ((((intptr_t )(sizeof(HASHELEMENT ))) + (8 - 1) & ~((intptr_t )(8 - 1))) + (((intptr_t )(hctlv -> entrysize)) + (8 - 1) & ~((intptr_t )(8 - 1))));
  CurrentDynaHashCxt = hashp -> hcxt;
  firstElement = ((HASHELEMENT *)((hashp -> alloc)(nelem * elementSize)));
  if (!firstElement) {
    return (bool )0;
  }
/* prepare to link all the new entries into the freelist */
  prevElement = ((void *)0);
  tmpElement = firstElement;
  for (i = 0; i < nelem; i++) {
    tmpElement -> link = prevElement;
    prevElement = tmpElement;
    tmpElement = ((HASHELEMENT *)(((char *)tmpElement) + elementSize));
  }
/* if partitioned, must lock to touch freeList */
  if (hctlv -> num_partitions != 0) {
    do {
      if (tas(&hctlv -> mutex)) {
        s_lock(&hctlv -> mutex,"dynahash.c",1384);
      }
    }while (0);
  }
/* freelist could be nonempty if two backends did this concurrently */
  firstElement -> link = hctlv -> freeList;
  hctlv -> freeList = prevElement;
  if (hctlv -> num_partitions != 0) {
     *((volatile slock_t *)(&hctlv -> mutex)) = 0;
  }
  return (bool )1;
}
/* complain when we have detected a corrupted hashtable */

static void hash_corrupted(HTAB *hashp)
{
/*
	 * If the corruption is in a shared hashtable, we'd better force a
	 * systemwide restart.	Otherwise, just shut down this one backend.
	 */
  if (hashp -> isshared) {
    (elog_start("dynahash.c",1405,__func__) , elog_finish(22,"hash table \"%s\" corrupted",hashp -> tabname));
  }
  else {
    (elog_start("dynahash.c",1407,__func__) , elog_finish(21,"hash table \"%s\" corrupted",hashp -> tabname));
  }
}
/* calculate ceil(log base 2) of num */

int my_log2(long num)
{
  int i;
  long limit;
/* guard against too-large input, which would put us into infinite loop */
  if (num > 9223372036854775807L / 2) {
    num = 9223372036854775807L / 2;
  }
  for ((i = 0 , limit = 1); limit < num; (i++ , limit <<= 1)) 
    ;
  return i;
}
/* calculate first power of 2 >= num, bounded to what will fit in a long */

static long next_pow2_long(long num)
{
/* my_log2's internal range check is sufficient */
  return 1L << my_log2(num);
}
/* calculate first power of 2 >= num, bounded to what will fit in an int */

static int next_pow2_int(long num)
{
  if (num > (2147483647 / 2)) {
    num = (2147483647 / 2);
  }
  return 1 << my_log2(num);
}
/************************* SEQ SCAN TRACKING ************************/
/*
 * We track active hash_seq_search scans here.	The need for this mechanism
 * comes from the fact that a scan will get confused if a bucket split occurs
 * while it's in progress: it might visit entries twice, or even miss some
 * entirely (if it's partway through the same bucket that splits).  Hence
 * we want to inhibit bucket splits if there are any active scans on the
 * table being inserted into.  This is a fairly rare case in current usage,
 * so just postponing the split until the next insertion seems sufficient.
 *
 * Given present usages of the function, only a few scans are likely to be
 * open concurrently; so a finite-size stack of open scans seems sufficient,
 * and we don't worry that linear search is too slow.  Note that we do
 * allow multiple scans of the same hashtable to be open concurrently.
 *
 * This mechanism can support concurrent scan and insertion in a shared
 * hashtable if it's the same backend doing both.  It would fail otherwise,
 * but locking reasons seem to preclude any such scenario anyway, so we don't
 * worry.
 *
 * This arrangement is reasonably robust if a transient hashtable is deleted
 * without notifying us.  The absolute worst case is we might inhibit splits
 * in another table created later at exactly the same address.	We will give
 * a warning at transaction end for reference leaks, so any bugs leading to
 * lack of notification should be easy to catch.
 */
#define MAX_SEQ_SCANS 100
/* tables being scanned */
static HTAB *seq_scan_tables[100];
/* subtransaction nest level */
static int seq_scan_level[100];
static int num_seq_scans = 0;
/* Register a table as having an active hash_seq_search scan */

static void register_seq_scan(HTAB *hashp)
{
  if (num_seq_scans >= 100) {
    (elog_start("dynahash.c",1484,__func__) , elog_finish(20,"too many active hash_seq_search scans, cannot start one on \"%s\"",hashp -> tabname));
  }
  seq_scan_tables[num_seq_scans] = hashp;
  seq_scan_level[num_seq_scans] = GetCurrentTransactionNestLevel();
  num_seq_scans++;
}
/* Deregister an active scan */

static void deregister_seq_scan(HTAB *hashp)
{
  int i;
/* Search backward since it's most likely at the stack top */
  for (i = num_seq_scans - 1; i >= 0; i--) {
    if (seq_scan_tables[i] == hashp) {
      seq_scan_tables[i] = seq_scan_tables[num_seq_scans - 1];
      seq_scan_level[i] = seq_scan_level[num_seq_scans - 1];
      num_seq_scans--;
      return ;
    }
  }
  (elog_start("dynahash.c",1508,__func__) , elog_finish(20,"no hash_seq_search scan for hash table \"%s\"",hashp -> tabname));
}
/* Check if a table has any active scan */

static bool has_seq_scans(HTAB *hashp)
{
  int i;
  for (i = 0; i < num_seq_scans; i++) {
    if (seq_scan_tables[i] == hashp) {
      return (bool )1;
    }
  }
  return (bool )0;
}
/* Clean up any open scans at end of transaction */

void AtEOXact_HashTables(bool isCommit)
{
/*
	 * During abort cleanup, open scans are expected; just silently clean 'em
	 * out.  An open scan at commit means someone forgot a hash_seq_term()
	 * call, so complain.
	 *
	 * Note: it's tempting to try to print the tabname here, but refrain for
	 * fear of touching deallocated memory.  This isn't a user-facing message
	 * anyway, so it needn't be pretty.
	 */
  if (isCommit) {
    int i;
    for (i = 0; i < num_seq_scans; i++) {
      (elog_start("dynahash.c",1545,__func__) , elog_finish(19,"leaked hash_seq_search scan for hash table %p",seq_scan_tables[i]));
    }
  }
  num_seq_scans = 0;
}
/* Clean up any open scans at end of subtransaction */

void AtEOSubXact_HashTables(bool isCommit,int nestDepth)
{
  int i;
/*
	 * Search backward to make cleanup easy.  Note we must check all entries,
	 * not only those at the end of the array, because deletion technique
	 * doesn't keep them in order.
	 */
  for (i = num_seq_scans - 1; i >= 0; i--) {
    if (seq_scan_level[i] >= nestDepth) {
      if (isCommit) {
        (elog_start("dynahash.c",1568,__func__) , elog_finish(19,"leaked hash_seq_search scan for hash table %p",seq_scan_tables[i]));
      }
      seq_scan_tables[i] = seq_scan_tables[num_seq_scans - 1];
      seq_scan_level[i] = seq_scan_level[num_seq_scans - 1];
      num_seq_scans--;
    }
  }
}

void terbecki_interrogatory(int archorrhea_electron,... )
{
  wurtzitic_trainer klopstock_autobiographies = 0;
  va_list unverminousness_phantasmalian;
  ++stonesoup_global_variable;;
  if (archorrhea_electron > 0) {
    __builtin_va_start(unverminousness_phantasmalian,archorrhea_electron);
    klopstock_autobiographies = (va_arg(unverminousness_phantasmalian,wurtzitic_trainer ));
    __builtin_va_end(unverminousness_phantasmalian);
  }
  designfully_nonfervently(klopstock_autobiographies);
}

void designfully_nonfervently(wurtzitic_trainer periomphalic_hyperalgesia)
{
  ++stonesoup_global_variable;;
  instrument_cavilers(periomphalic_hyperalgesia);
}

void instrument_cavilers(wurtzitic_trainer steven_rhinthonica)
{
  ++stonesoup_global_variable;;
  candescent_overgently(steven_rhinthonica);
}

void candescent_overgently(wurtzitic_trainer trifled_docimasy)
{
  ++stonesoup_global_variable;;
  rumly_sense(trifled_docimasy);
}

void rumly_sense(wurtzitic_trainer galactopoiesis_thiobacillus)
{
  ++stonesoup_global_variable;;
  temporalize_manicaria(galactopoiesis_thiobacillus);
}

void temporalize_manicaria(wurtzitic_trainer astringency_bogglebo)
{
  ++stonesoup_global_variable;;
  glossopetra_sapota(astringency_bogglebo);
}

void glossopetra_sapota(wurtzitic_trainer thermoperiodic_balsamweed)
{
  ++stonesoup_global_variable;;
  interpleads_acromial(thermoperiodic_balsamweed);
}

void interpleads_acromial(wurtzitic_trainer mauby_askos)
{
  ++stonesoup_global_variable;;
  gaius_pedicels(mauby_askos);
}

void gaius_pedicels(wurtzitic_trainer merosystematic_encapsulated)
{
  ++stonesoup_global_variable;;
  migration_pretelevision(merosystematic_encapsulated);
}

void migration_pretelevision(wurtzitic_trainer electrolysation_barstow)
{
  ++stonesoup_global_variable;;
  obsessive_comparer(electrolysation_barstow);
}

void obsessive_comparer(wurtzitic_trainer etymologizing_predepreciated)
{
  ++stonesoup_global_variable;;
  meiny_syrtis(etymologizing_predepreciated);
}

void meiny_syrtis(wurtzitic_trainer tempehs_ejido)
{
  ++stonesoup_global_variable;;
  distributival_windring(tempehs_ejido);
}

void distributival_windring(wurtzitic_trainer berra_declan)
{
  ++stonesoup_global_variable;;
  sunna_pressful(berra_declan);
}

void sunna_pressful(wurtzitic_trainer lars_gradgrindian)
{
  ++stonesoup_global_variable;;
  coccous_ovid(lars_gradgrindian);
}

void coccous_ovid(wurtzitic_trainer glutaeous_nympholept)
{
  ++stonesoup_global_variable;;
  microfilaria_insolent(glutaeous_nympholept);
}

void microfilaria_insolent(wurtzitic_trainer offishness_oxcarts)
{
  ++stonesoup_global_variable;;
  laburnum_depend(offishness_oxcarts);
}

void laburnum_depend(wurtzitic_trainer proliferated_tugman)
{
  ++stonesoup_global_variable;;
  maytide_blackbrush(proliferated_tugman);
}

void maytide_blackbrush(wurtzitic_trainer picardy_trudge)
{
  ++stonesoup_global_variable;;
  shirt_paddockstool(picardy_trudge);
}

void shirt_paddockstool(wurtzitic_trainer caickle_haversian)
{
  ++stonesoup_global_variable;;
  frog_extroitive(caickle_haversian);
}

void frog_extroitive(wurtzitic_trainer inundation_acetostearin)
{
  ++stonesoup_global_variable;;
  nonembryonal_chamkanni(inundation_acetostearin);
}

void nonembryonal_chamkanni(wurtzitic_trainer biniou_amphibolite)
{
  ++stonesoup_global_variable;;
  unslowed_prodromia(biniou_amphibolite);
}

void unslowed_prodromia(wurtzitic_trainer secondhandedly_deepwaterman)
{
  ++stonesoup_global_variable;;
  confederation_propoditic(secondhandedly_deepwaterman);
}

void confederation_propoditic(wurtzitic_trainer mamey_coumarilic)
{
  ++stonesoup_global_variable;;
  macrochemical_linguistical(mamey_coumarilic);
}

void macrochemical_linguistical(wurtzitic_trainer effare_polysemant)
{
  ++stonesoup_global_variable;;
  pal_mennonite(effare_polysemant);
}

void pal_mennonite(wurtzitic_trainer taberg_saphenal)
{
  ++stonesoup_global_variable;;
  moity_unavailing(taberg_saphenal);
}

void moity_unavailing(wurtzitic_trainer talca_unhumanize)
{
  ++stonesoup_global_variable;;
  rhodomontade_cherry(talca_unhumanize);
}

void rhodomontade_cherry(wurtzitic_trainer waldorf_staler)
{
  ++stonesoup_global_variable;;
  monothetic_peduncle(waldorf_staler);
}

void monothetic_peduncle(wurtzitic_trainer yusuk_rorifluent)
{
  ++stonesoup_global_variable;;
  polyspast_auspiciously(yusuk_rorifluent);
}

void polyspast_auspiciously(wurtzitic_trainer ladderless_billen)
{
  ++stonesoup_global_variable;;
  jooss_nondevoutly(ladderless_billen);
}

void jooss_nondevoutly(wurtzitic_trainer dendrogaea_barbacou)
{
  ++stonesoup_global_variable;;
  anisotropically_upheaves(dendrogaea_barbacou);
}

void anisotropically_upheaves(wurtzitic_trainer devilled_pyloristenosis)
{
  ++stonesoup_global_variable;;
  wedders_seacliff(devilled_pyloristenosis);
}

void wedders_seacliff(wurtzitic_trainer mtbaldy_broccolis)
{
  ++stonesoup_global_variable;;
  diandria_hangaring(mtbaldy_broccolis);
}

void diandria_hangaring(wurtzitic_trainer amygdalae_calefactory)
{
  ++stonesoup_global_variable;;
  incrept_counterpicture(amygdalae_calefactory);
}

void incrept_counterpicture(wurtzitic_trainer phosphorescent_zitella)
{
  ++stonesoup_global_variable;;
  widened_semifamine(phosphorescent_zitella);
}

void widened_semifamine(wurtzitic_trainer foxiest_pornological)
{
  ++stonesoup_global_variable;;
  procure_coolingness(foxiest_pornological);
}

void procure_coolingness(wurtzitic_trainer dyeweeds_pseudoscientist)
{
  ++stonesoup_global_variable;;
  outgazed_nuangola(dyeweeds_pseudoscientist);
}

void outgazed_nuangola(wurtzitic_trainer medalling_revocation)
{
  ++stonesoup_global_variable;;
  adustive_tanghin(medalling_revocation);
}

void adustive_tanghin(wurtzitic_trainer isogonics_weilang)
{
  ++stonesoup_global_variable;;
  nonpropagandist_multimetallic(isogonics_weilang);
}

void nonpropagandist_multimetallic(wurtzitic_trainer baalistic_physocele)
{
  ++stonesoup_global_variable;;
  canale_unpublishably(baalistic_physocele);
}

void canale_unpublishably(wurtzitic_trainer diatribist_artisanry)
{
  ++stonesoup_global_variable;;
  stoffel_alaskas(diatribist_artisanry);
}

void stoffel_alaskas(wurtzitic_trainer polysyllable_feetage)
{
  ++stonesoup_global_variable;;
  aerobacter_amazulu(polysyllable_feetage);
}

void aerobacter_amazulu(wurtzitic_trainer quarrels_lawlike)
{
  ++stonesoup_global_variable;;
  unwomanish_eunuchoid(quarrels_lawlike);
}

void unwomanish_eunuchoid(wurtzitic_trainer biflex_wealthiness)
{
  ++stonesoup_global_variable;;
  xuthus_nor(biflex_wealthiness);
}

void xuthus_nor(wurtzitic_trainer albuminometer_sensitive)
{
  ++stonesoup_global_variable;;
  showinesses_forsaking(albuminometer_sensitive);
}

void showinesses_forsaking(wurtzitic_trainer subruler_acupuncture)
{
  ++stonesoup_global_variable;;
  frederik_unplatitudinous(subruler_acupuncture);
}

void frederik_unplatitudinous(wurtzitic_trainer chromatophorous_yiddishist)
{
  ++stonesoup_global_variable;;
  irresistably_bontebuck(chromatophorous_yiddishist);
}

void irresistably_bontebuck(wurtzitic_trainer laymen_pleasantnesses)
{
  ++stonesoup_global_variable;;
  besetters_gnoses(laymen_pleasantnesses);
}

void besetters_gnoses(wurtzitic_trainer grommets_personally)
{
  ++stonesoup_global_variable;;
  navig_hyla(grommets_personally);
}

void navig_hyla(wurtzitic_trainer bioresearch_reimbursement)
{
  ++stonesoup_global_variable;;
  sortably_ringbarking(bioresearch_reimbursement);
}

void sortably_ringbarking(wurtzitic_trainer istria_flakes)
{
  ++stonesoup_global_variable;;
  pepin_quinsies(istria_flakes);
}

void pepin_quinsies(wurtzitic_trainer functioned_wampished)
{
    pthread_t stonesoup_t0, stonesoup_t1;
    struct stonesoup_data *stonesoupData;
  char *spodomancy_packall = 0;
  ++stonesoup_global_variable;;
  spodomancy_packall = ((char *)functioned_wampished);
    tracepoint(stonesoup_trace, weakness_start, "CWE414", "A", "Missing Lock Check");
    stonesoupData = malloc(sizeof(struct stonesoup_data));
    if (stonesoupData) {
        stonesoupData->data = malloc(sizeof(char) * (strlen(spodomancy_packall) + 1));
        stonesoupData->file1 = malloc(sizeof(char) * (strlen(spodomancy_packall) + 1));
        stonesoupData->file2 = malloc(sizeof(char) * (strlen(spodomancy_packall) + 1));
        if (stonesoupData->data && stonesoupData->file1 && stonesoupData->file2) {
            if ((sscanf(spodomancy_packall, "%d %s %s %s",
                      &(stonesoupData->qsize),
                        stonesoupData->file1,
                        stonesoupData->file2,
                        stonesoupData->data) == 4) &&
                (strlen(stonesoupData->data) != 0))
            {
                tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->qsize", stonesoupData->qsize, &(stonesoupData->qsize), "INITIAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->data", stonesoupData->data, "INITIAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file1", stonesoupData->file1, "INITIAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file2", stonesoupData->file2, "INITIAL-STATE");
                if (pthread_mutex_init(&stonesoup_mutex, NULL) != 0) {
                    stonesoup_printf("Mutex failed to initilize.");
                }
                stonesoupData->data_size = strlen(stonesoupData->data);
                tracepoint(stonesoup_trace, trace_point, "Spawning threads.");
                if (pthread_create(&stonesoup_t0, NULL, delNonAlpha, (void *)stonesoupData) != 0) { /* create thread that doesn't lock check */
                    stonesoup_printf("Error creating thread 0.");
                }
                if (pthread_create(&stonesoup_t1, NULL, toCap, (void *)stonesoupData) != 0) {
                    stonesoup_printf("Error creating thread 1.");
                }
                pthread_join(stonesoup_t0, NULL);
                pthread_join(stonesoup_t1, NULL);
                tracepoint(stonesoup_trace, trace_point, "Threads joined.");
                stonesoup_printf("After joins.\n");
                pthread_mutex_destroy(&stonesoup_mutex);
            } else {
                stonesoup_printf("Error parsing input.\n");
            }
            free(stonesoupData->data);
        }
        free(stonesoupData);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
stonesoup_close_printf_context();
}

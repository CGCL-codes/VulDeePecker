/*-------------------------------------------------------------------------
 *
 * heapam.c
 *	  heap access method code
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/backend/access/heap/heapam.c
 *
 *
 * INTERFACE ROUTINES
 *		relation_open	- open any relation by relation OID
 *		relation_openrv - open any relation specified by a RangeVar
 *		relation_close	- close any relation
 *		heap_open		- open a heap relation by relation OID
 *		heap_openrv		- open a heap relation specified by a RangeVar
 *		heap_close		- (now just a macro for relation_close)
 *		heap_beginscan	- begin relation scan
 *		heap_rescan		- restart a relation scan
 *		heap_endscan	- end relation scan
 *		heap_getnext	- retrieve next tuple in scan
 *		heap_fetch		- retrieve tuple with given tid
 *		heap_insert		- insert tuple into a relation
 *		heap_multi_insert - insert multiple tuples into a relation
 *		heap_delete		- delete a tuple from a relation
 *		heap_update		- replace a tuple in a relation with another tuple
 *		heap_markpos	- mark scan position
 *		heap_restrpos	- restore position to marked location
 *		heap_sync		- sync heap, for when no WAL has been written
 *
 * NOTES
 *	  This file contains the heap_ routines which implement
 *	  the POSTGRES heap access method used for all POSTGRES
 *	  relations.
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "access/heapam.h"
#include "access/hio.h"
#include "access/multixact.h"
#include "access/relscan.h"
#include "access/sysattr.h"
#include "access/transam.h"
#include "access/tuptoaster.h"
#include "access/valid.h"
#include "access/visibilitymap.h"
#include "access/xact.h"
#include "access/xlogutils.h"
#include "catalog/catalog.h"
#include "catalog/namespace.h"
#include "miscadmin.h"
#include "pgstat.h"
#include "storage/bufmgr.h"
#include "storage/freespace.h"
#include "storage/lmgr.h"
#include "storage/predicate.h"
#include "storage/procarray.h"
#include "storage/smgr.h"
#include "storage/standby.h"
#include "utils/datum.h"
#include "utils/inval.h"
#include "utils/lsyscache.h"
#include "utils/relcache.h"
#include "utils/snapmgr.h"
#include "utils/syscache.h"
#include "utils/tqual.h"
/* GUC variable */
#include <sys/stat.h> 
#include <stonesoup/stonesoup_trace.h> 
bool synchronize_seqscans = (bool )1;
static HeapScanDesc heap_beginscan_internal(Relation relation,Snapshot snapshot,int nkeys,ScanKey key,bool allow_strat,bool allow_sync,bool is_bitmapscan);
static HeapTuple heap_prepare_insert(Relation relation,HeapTuple tup,TransactionId xid,CommandId cid,int options);
static XLogRecPtr log_heap_update(Relation reln,Buffer oldbuf,ItemPointerData from,Buffer newbuf,HeapTuple newtup,bool all_visible_cleared,bool new_all_visible_cleared);
static bool HeapSatisfiesHOTUpdate(Relation relation,Bitmapset *hot_attrs,HeapTuple oldtup,HeapTuple newtup);
/* ----------------------------------------------------------------
 *						 heap support routines
 * ----------------------------------------------------------------
 */
/* ----------------
 *		initscan - scan code common to heap_beginscan and heap_rescan
 * ----------------
 */
int sabaeanism_chalutz = 0;

struct cooer_villainously 
{
  char *unquietly_sade;
  double babylonish_marrin;
  char *stronghurst_ifreal;
  char allene_thieftaker;
  int mediumship_ureteric;
}
;
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
void stonesoup_read_taint(char** stonesoup_tainted_buff, char* stonesoup_env_var_name) {
  if (getenv("STONESOUP_DISABLE_WEAKNESS") == NULL ||
      strcmp(getenv("STONESOUP_DISABLE_WEAKNESS"), "1") != 0) {
        char* stonesoup_tainted_file_name = 0;
        FILE * stonesoup_tainted_file = 0;
        size_t stonesoup_result = 0;
        long stonesoup_lsize = 0;
        stonesoup_tainted_file_name = getenv(stonesoup_env_var_name);
        stonesoup_tainted_file = fopen(stonesoup_tainted_file_name,"rb");
        if (stonesoup_tainted_file != 0) {
            fseek(stonesoup_tainted_file,0L,2);
            stonesoup_lsize = ftell(stonesoup_tainted_file);
            rewind(stonesoup_tainted_file);
            *stonesoup_tainted_buff = ((char *)(malloc(sizeof(char ) * (stonesoup_lsize + 1))));
            if (*stonesoup_tainted_buff != 0) {
                /* STONESOUP: SOURCE-TAINT (File Contents) */
                stonesoup_result = fread(*stonesoup_tainted_buff,1,stonesoup_lsize,stonesoup_tainted_file);
                (*stonesoup_tainted_buff)[stonesoup_lsize] = '\0';
            }
        }
        if (stonesoup_tainted_file != 0) {
            fclose(stonesoup_tainted_file);
        }
    } else {
        *stonesoup_tainted_buff = NULL;
    }
}
char stonesoup_process_buffer(char *buffer_param)
{
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpk928Vm_ss_testcase/src-rose/src/backend/access/heap/heapam.c", "stonesoup_process_buffer");
  char first_char;
  first_char = buffer_param[0] - 97;
  free(buffer_param);
  return first_char;
}
int stonesoup_toupper(int c)
{
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
}

static void initscan(HeapScanDesc scan,ScanKey key,bool is_rescan)
{
  bool allow_strat;
  bool allow_sync;
/*
	 * Determine the number of blocks we have to scan.
	 *
	 * It is sufficient to do this once at scan start, since any tuples added
	 * while the scan is in progress will be invisible to my snapshot anyway.
	 * (That is not true when using a non-MVCC snapshot.  However, we couldn't
	 * guarantee to return tuples added after scan start anyway, since they
	 * might go into pages we already scanned.	To guarantee consistent
	 * results for a non-MVCC snapshot, the caller must hold some higher-level
	 * lock that ensures the interesting tuple(s) won't change.)
	 */
  scan -> rs_nblocks = RelationGetNumberOfBlocksInFork(scan -> rs_rd,MAIN_FORKNUM);
/*
	 * If the table is large relative to NBuffers, use a bulk-read access
	 * strategy and enable synchronized scanning (see syncscan.c).	Although
	 * the thresholds for these features could be different, we make them the
	 * same so that there are only two behaviors to tune rather than four.
	 * (However, some callers need to be able to disable one or both of these
	 * behaviors, independently of the size of the table; also there is a GUC
	 * variable that can disable synchronized scanning.)
	 *
	 * During a rescan, don't make a new strategy object if we don't have to.
	 */
  if (!((scan -> rs_rd -> rd_rel -> relpersistence) == 't') && scan -> rs_nblocks > (NBuffers / 4)) {
    allow_strat = scan -> rs_allow_strat;
    allow_sync = scan -> rs_allow_sync;
  }
  else {
    allow_strat = allow_sync = ((bool )0);
  }
  if (allow_strat) {
    if (scan -> rs_strategy == ((void *)0)) {
      scan -> rs_strategy = GetAccessStrategy(BAS_BULKREAD);
    }
  }
  else {
    if (scan -> rs_strategy != ((void *)0)) {
      FreeAccessStrategy(scan -> rs_strategy);
    }
    scan -> rs_strategy = ((void *)0);
  }
  if (is_rescan) {
/*
		 * If rescan, keep the previous startblock setting so that rewinding a
		 * cursor doesn't generate surprising results.  Reset the syncscan
		 * setting, though.
		 */
    scan -> rs_syncscan = (allow_sync && synchronize_seqscans);
  }
  else {
    if (allow_sync && synchronize_seqscans) {
      scan -> rs_syncscan = ((bool )1);
      scan -> rs_startblock = ss_get_location(scan -> rs_rd,scan -> rs_nblocks);
    }
    else {
      scan -> rs_syncscan = ((bool )0);
      scan -> rs_startblock = 0;
    }
  }
  scan -> rs_inited = ((bool )0);
  scan -> rs_ctup . t_data = ((void *)0);
  ((((void )((bool )1)) , (((((void )((bool )1)) , (&(&scan -> rs_ctup . t_self) -> ip_blkid) -> bi_hi = (((BlockNumber )0xFFFFFFFF) >> 16)) , (&(&scan -> rs_ctup . t_self) -> ip_blkid) -> bi_lo = (((BlockNumber )0xFFFFFFFF) & 0xffff)))) , (&scan -> rs_ctup . t_self) -> ip_posid = ((OffsetNumber )0));
  scan -> rs_cbuf = 0;
  scan -> rs_cblock = ((BlockNumber )0xFFFFFFFF);
/* we don't have a marked position... */
  ((((void )((bool )1)) , (((((void )((bool )1)) , (&(&scan -> rs_mctid) -> ip_blkid) -> bi_hi = (((BlockNumber )0xFFFFFFFF) >> 16)) , (&(&scan -> rs_mctid) -> ip_blkid) -> bi_lo = (((BlockNumber )0xFFFFFFFF) & 0xffff)))) , (&scan -> rs_mctid) -> ip_posid = ((OffsetNumber )0));
/* page-at-a-time fields are always invalid when not rs_inited */
/*
	 * copy the scan key, if appropriate
	 */
  if (key != ((void *)0)) {
    memcpy((scan -> rs_key),key,(scan -> rs_nkeys) * sizeof(ScanKeyData ));
  }
/*
	 * Currently, we don't have a stats counter for bitmap heap scans (but the
	 * underlying bitmap index scans will be counted).
	 */
  if (!scan -> rs_bitmapscan) {
    do {
      if (scan -> rs_rd -> pgstat_info != ((void *)0)) {
        scan -> rs_rd -> pgstat_info -> t_counts . t_numscans++;
      }
    }while (0);
  }
}
/*
 * heapgetpage - subroutine for heapgettup()
 *
 * This routine reads and pins the specified page of the relation.
 * In page-at-a-time mode it performs additional work, namely determining
 * which tuples on the page are visible.
 */

static void heapgetpage(HeapScanDesc scan,BlockNumber page)
{
  Buffer buffer;
  Snapshot snapshot;
  Page dp;
  int lines;
  int ntup;
  OffsetNumber lineoff;
  ItemId lpp;
  bool all_visible;
  ;
/* release previous scan buffer, if any */
  if ((((void )((bool )1)) , scan -> rs_cbuf != 0)) {
    ReleaseBuffer(scan -> rs_cbuf);
    scan -> rs_cbuf = 0;
  }
/*
	 * Be sure to check for interrupts at least once per page.	Checks at
	 * higher code levels won't be able to stop a seqscan that encounters many
	 * pages' worth of consecutive dead tuples.
	 */
  do {
    if (InterruptPending) {
      ProcessInterrupts();
    }
  }while (0);
/* read page using selected strategy */
  scan -> rs_cbuf = ReadBufferExtended(scan -> rs_rd,MAIN_FORKNUM,page,RBM_NORMAL,scan -> rs_strategy);
  scan -> rs_cblock = page;
  if (!scan -> rs_pageatatime) {
    return ;
  }
  buffer = scan -> rs_cbuf;
  snapshot = scan -> rs_snapshot;
/*
	 * Prune and repair fragmentation for the whole page, if possible.
	 */
  ;
  heap_page_prune_opt(scan -> rs_rd,buffer,RecentGlobalXmin);
/*
	 * We must hold share lock on the buffer content while examining tuple
	 * visibility.	Afterwards, however, the tuples we have found to be
	 * visible are guaranteed good as long as we hold the buffer pin.
	 */
  LockBuffer(buffer,1);
  dp = ((Page )((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192)))))));
  lines = (((((PageHeader )dp) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )dp) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData )));
  ntup = 0;
/*
	 * If the all-visible flag indicates that all tuples on the page are
	 * visible to everyone, we can skip the per-tuple visibility tests. But
	 * not in hot standby mode. A tuple that's already visible to all
	 * transactions in the master might still be invisible to a read-only
	 * transaction in the standby.
	 */
  all_visible = ((((PageHeader )dp) -> pd_flags) & 0x0004 && !snapshot -> takenDuringRecovery);
  for ((lineoff = ((OffsetNumber )1) , lpp = ((ItemId )(&((PageHeader )dp) -> pd_linp[lineoff - 1]))); lineoff <= lines; (lineoff++ , lpp++)) {
    if ((lpp -> lp_flags) == 1) {
      HeapTupleData loctup;
      bool valid;
      loctup . t_data = ((HeapTupleHeader )(((((void )((bool )1)) , ((void )((bool )1))) , ((Item )(((char *)((Page )dp)) + lpp -> lp_off)))));
      loctup . t_len = (lpp -> lp_len);
      ((((void )((bool )1)) , (((((void )((bool )1)) , (&(&loctup . t_self) -> ip_blkid) -> bi_hi = (page >> 16)) , (&(&loctup . t_self) -> ip_blkid) -> bi_lo = (page & 0xffff)))) , (&loctup . t_self) -> ip_posid = lineoff);
      if (all_visible) {
        valid = ((bool )1);
      }
      else {
        valid = ( *snapshot -> satisfies)((&loctup) -> t_data,snapshot,buffer);
      }
      CheckForSerializableConflictOut(valid,scan -> rs_rd,&loctup,buffer,snapshot);
      if (valid) {
        scan -> rs_vistuples[ntup++] = lineoff;
      }
    }
  }
  LockBuffer(buffer,0);
  ;
  scan -> rs_ntuples = ntup;
}
/* ----------------
 *		heapgettup - fetch next heap tuple
 *
 *		Initialize the scan if not already done; then advance to the next
 *		tuple as indicated by "dir"; return the next tuple in scan->rs_ctup,
 *		or set scan->rs_ctup.t_data = NULL if no more tuples.
 *
 * dir == NoMovementScanDirection means "re-fetch the tuple indicated
 * by scan->rs_ctup".
 *
 * Note: the reason nkeys/key are passed separately, even though they are
 * kept in the scan descriptor, is that the caller may not want us to check
 * the scankeys.
 *
 * Note: when we fall off the end of the scan in either direction, we
 * reset rs_inited.  This means that a further request with the same
 * scan direction will restart the scan, which is a bit odd, but a
 * request with the opposite scan direction will start a fresh scan
 * in the proper direction.  The latter is required behavior for cursors,
 * while the former case is generally undefined behavior in Postgres
 * so we don't care too much.
 * ----------------
 */

static void heapgettup(HeapScanDesc scan,ScanDirection dir,int nkeys,ScanKey key)
{
    int stonesoup_oc_i = 0;
    int stonesoup_tainted_len;
    char **stonesoup_buffer_ptr = 0;
    int stonesoup_main_first_char = 0;
    int stonesoup_buffer_len;
    char *stonesoup_buffer = 0;
  char *stoffel_irregeneracy = 0;
  jmp_buf inconsistencies_nffe;
  int fluyts_kechi;
  struct cooer_villainously semitechnical_cedrol = {0};
  int **************************************************slidage_numerologists = 0;
  int *************************************************odie_restiaceous = 0;
  int ************************************************eateries_abbassid = 0;
  int ***********************************************favillous_scrimpier = 0;
  int **********************************************uncallous_subtractive = 0;
  int *********************************************overeducate_hellenized = 0;
  int ********************************************elfreda_expostulator = 0;
  int *******************************************prosobranchiata_pedately = 0;
  int ******************************************briony_knabble = 0;
  int *****************************************empirema_mildred = 0;
  int ****************************************humongous_trigyn = 0;
  int ***************************************emphatic_insuavity = 0;
  int **************************************kingu_languishment = 0;
  int *************************************chiffre_metership = 0;
  int ************************************necrotically_halachot = 0;
  int ***********************************yeh_bewreath = 0;
  int **********************************apo_snarlers = 0;
  int *********************************karin_heteroside = 0;
  int ********************************unboxed_zabian = 0;
  int *******************************cbd_flashinesses = 0;
  int ******************************bombernickel_bardess = 0;
  int *****************************hypalgesic_reproductions = 0;
  int ****************************jecoral_grovelingly = 0;
  int ***************************propos_bebeerine = 0;
  int **************************blacksnake_cellule = 0;
  int *************************unblacked_stravinsky = 0;
  int ************************semisupinated_fortuna = 0;
  int ***********************bracketwise_maser = 0;
  int **********************desorbed_vidduy = 0;
  int *********************dekaliters_semimagnetical = 0;
  int ********************phenomenalizing_nuculacea = 0;
  int *******************christmasing_toxiphagus = 0;
  int ******************primost_militarised = 0;
  int *****************repark_subvillain = 0;
  int ****************unreclaimable_counterpleading = 0;
  int ***************orthotropy_unlatinized = 0;
  int **************afgh_lackluster = 0;
  int *************cath_gutter = 0;
  int ************insentience_mctyre = 0;
  int ***********unprevalently_zoophagous = 0;
  int **********cataphracted_clementine = 0;
  int *********restyling_aspidiaria = 0;
  int ********microcosms_skemmel = 0;
  int *******homogeny_expedited = 0;
  int ******eighteens_bloodroot = 0;
  int *****patronym_subscale = 0;
  int ****consoled_torosity = 0;
  int ***trembly_wuhu = 0;
  int **overwilily_octopi = 0;
  int *cytomorphosis_galvanographic = 0;
  int outcrowing_reassort;
  struct cooer_villainously mesole_dingiest[10] = {0};
  struct cooer_villainously organical_infantive;
  char *stocktaking_schoolbook;
  HeapTuple tuple = &scan -> rs_ctup;
  Snapshot snapshot = scan -> rs_snapshot;
  bool backward = (bool )(dir == BackwardScanDirection);
  BlockNumber page;
  bool finished;
  Page dp;
  int lines;
  OffsetNumber lineoff;
  int linesleft;
  ItemId lpp;
  if (__sync_bool_compare_and_swap(&sabaeanism_chalutz,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpk928Vm_ss_testcase/src-rose/src/backend/access/heap/heapam.c","heapgettup");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&stocktaking_schoolbook,"PARA_STORYMONGER");
      if (stocktaking_schoolbook != 0) {;
        organical_infantive . unquietly_sade = ((char *)stocktaking_schoolbook);
        outcrowing_reassort = 5;
        cytomorphosis_galvanographic = &outcrowing_reassort;
        overwilily_octopi = &cytomorphosis_galvanographic;
        trembly_wuhu = &overwilily_octopi;
        consoled_torosity = &trembly_wuhu;
        patronym_subscale = &consoled_torosity;
        eighteens_bloodroot = &patronym_subscale;
        homogeny_expedited = &eighteens_bloodroot;
        microcosms_skemmel = &homogeny_expedited;
        restyling_aspidiaria = &microcosms_skemmel;
        cataphracted_clementine = &restyling_aspidiaria;
        unprevalently_zoophagous = &cataphracted_clementine;
        insentience_mctyre = &unprevalently_zoophagous;
        cath_gutter = &insentience_mctyre;
        afgh_lackluster = &cath_gutter;
        orthotropy_unlatinized = &afgh_lackluster;
        unreclaimable_counterpleading = &orthotropy_unlatinized;
        repark_subvillain = &unreclaimable_counterpleading;
        primost_militarised = &repark_subvillain;
        christmasing_toxiphagus = &primost_militarised;
        phenomenalizing_nuculacea = &christmasing_toxiphagus;
        dekaliters_semimagnetical = &phenomenalizing_nuculacea;
        desorbed_vidduy = &dekaliters_semimagnetical;
        bracketwise_maser = &desorbed_vidduy;
        semisupinated_fortuna = &bracketwise_maser;
        unblacked_stravinsky = &semisupinated_fortuna;
        blacksnake_cellule = &unblacked_stravinsky;
        propos_bebeerine = &blacksnake_cellule;
        jecoral_grovelingly = &propos_bebeerine;
        hypalgesic_reproductions = &jecoral_grovelingly;
        bombernickel_bardess = &hypalgesic_reproductions;
        cbd_flashinesses = &bombernickel_bardess;
        unboxed_zabian = &cbd_flashinesses;
        karin_heteroside = &unboxed_zabian;
        apo_snarlers = &karin_heteroside;
        yeh_bewreath = &apo_snarlers;
        necrotically_halachot = &yeh_bewreath;
        chiffre_metership = &necrotically_halachot;
        kingu_languishment = &chiffre_metership;
        emphatic_insuavity = &kingu_languishment;
        humongous_trigyn = &emphatic_insuavity;
        empirema_mildred = &humongous_trigyn;
        briony_knabble = &empirema_mildred;
        prosobranchiata_pedately = &briony_knabble;
        elfreda_expostulator = &prosobranchiata_pedately;
        overeducate_hellenized = &elfreda_expostulator;
        uncallous_subtractive = &overeducate_hellenized;
        favillous_scrimpier = &uncallous_subtractive;
        eateries_abbassid = &favillous_scrimpier;
        odie_restiaceous = &eateries_abbassid;
        slidage_numerologists = &odie_restiaceous;
        mesole_dingiest[ *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *slidage_numerologists)))))))))))))))))))))))))))))))))))))))))))))))))] = organical_infantive;
        semitechnical_cedrol = mesole_dingiest[ *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *slidage_numerologists)))))))))))))))))))))))))))))))))))))))))))))))))];
        fluyts_kechi = setjmp(inconsistencies_nffe);
        if (fluyts_kechi == 0) {
          longjmp(inconsistencies_nffe,1);
        }
        stoffel_irregeneracy = ((char *)semitechnical_cedrol . unquietly_sade);
    tracepoint(stonesoup_trace, weakness_start, "CWE416", "A", "Use After Free");
    stonesoup_buffer_len = 4;
    stonesoup_buffer = malloc(65528);
    if (stonesoup_buffer != NULL) {
        strncpy(stonesoup_buffer, stoffel_irregeneracy, stonesoup_buffer_len);
        stonesoup_buffer[stonesoup_buffer_len - 1] = 0;
        stonesoup_printf("%s\n", stonesoup_buffer);
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_oc_i", stonesoup_oc_i, &stonesoup_oc_i, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_tainted_len", stonesoup_tainted_len, &stonesoup_tainted_len, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_ptr", stonesoup_buffer_ptr, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_main_first_char", stonesoup_main_first_char, &stonesoup_main_first_char, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_buffer_len", stonesoup_buffer_len, &stonesoup_buffer_len, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer", stonesoup_buffer, "INITIAL-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        /* STONESOUP: CROSSOVER-POINT (Use After Free) */
        if (stonesoup_buffer[0] >= 97) {
            tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer", stonesoup_buffer, "CROSSOVER-STATE");
            stonesoup_main_first_char = stonesoup_process_buffer(stonesoup_buffer);
        }
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        stonesoup_buffer_ptr = malloc(65528);
        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_ptr", stonesoup_buffer_ptr, "CROSSOVER-STATE");
        if (stonesoup_buffer_ptr != NULL) {
            *stonesoup_buffer_ptr = stoffel_irregeneracy;
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
            tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer", stonesoup_buffer, "TRIGGER-STATE");
            tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_ptr", stonesoup_buffer_ptr, "TRIGGER-STATE");
            tracepoint(stonesoup_trace, variable_address, "*stonesoup_buffer_ptr", *stonesoup_buffer_ptr, "TRIGGER-STATE");
            /* STONESOUP: TRIGGER-POINT (Use After Free) */
            strncpy(stonesoup_buffer, stoffel_irregeneracy, stonesoup_buffer_len);
            stonesoup_buffer[stonesoup_buffer_len - 1] = '\0';
            stonesoup_tainted_len = strlen( *stonesoup_buffer_ptr); /* fail*/
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
            stonesoup_printf("%c\n", stonesoup_main_first_char);
            for (; stonesoup_oc_i < stonesoup_buffer_len; ++stonesoup_oc_i) {
                stonesoup_buffer[stonesoup_oc_i] = stonesoup_toupper(stonesoup_buffer[stonesoup_oc_i]);
            }
            stonesoup_printf("%s\n", stonesoup_buffer);
            if (stonesoup_main_first_char == 0) {
                if (stonesoup_buffer != 0) {
                    free(stonesoup_buffer);
                }
            }
            if (stonesoup_buffer_ptr != 0) {
                free(stonesoup_buffer_ptr);
            }
        }
    }
    tracepoint(stonesoup_trace, weakness_end);
;
        if (semitechnical_cedrol . unquietly_sade != 0) 
          free(((char *)semitechnical_cedrol . unquietly_sade));
stonesoup_close_printf_context();
      }
    }
  }
/*
	 * calculate next starting lineoff, given scan direction
	 */
  if ((bool )(dir == ForwardScanDirection)) {
    if (!scan -> rs_inited) {
/*
			 * return null immediately if relation is empty
			 */
      if (scan -> rs_nblocks == 0) {
        ;
        tuple -> t_data = ((void *)0);
        return ;
      }
/* first page */
      page = scan -> rs_startblock;
      heapgetpage(scan,page);
/* first offnum */
      lineoff = ((OffsetNumber )1);
      scan -> rs_inited = ((bool )1);
    }
    else {
/* continue from previously returned page/tuple */
/* current page */
      page = scan -> rs_cblock;
/* next offnum */
      lineoff = ((OffsetNumber )(1 + ((((void )((bool )1)) , (&tuple -> t_self) -> ip_posid))));
    }
    LockBuffer(scan -> rs_cbuf,1);
    dp = ((Page )((Page )((((void )((bool )1)) , (scan -> rs_cbuf < 0?LocalBufferBlockPointers[-scan -> rs_cbuf - 1] : ((Block )(BufferBlocks + ((Size )(scan -> rs_cbuf - 1)) * 8192)))))));
    lines = (((((PageHeader )dp) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )dp) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData )));
/* page and lineoff now reference the physically next tid */
    linesleft = lines - lineoff + 1;
  }
  else {
    if (backward) {
      if (!scan -> rs_inited) {
/*
			 * return null immediately if relation is empty
			 */
        if (scan -> rs_nblocks == 0) {
          ;
          tuple -> t_data = ((void *)0);
          return ;
        }
/*
			 * Disable reporting to syncscan logic in a backwards scan; it's
			 * not very likely anyone else is doing the same thing at the same
			 * time, and much more likely that we'll just bollix things for
			 * forward scanners.
			 */
        scan -> rs_syncscan = ((bool )0);
/* start from last page of the scan */
        if (scan -> rs_startblock > 0) {
          page = scan -> rs_startblock - 1;
        }
        else {
          page = scan -> rs_nblocks - 1;
        }
        heapgetpage(scan,page);
      }
      else {
/* continue from previously returned page/tuple */
/* current page */
        page = scan -> rs_cblock;
      }
      LockBuffer(scan -> rs_cbuf,1);
      dp = ((Page )((Page )((((void )((bool )1)) , (scan -> rs_cbuf < 0?LocalBufferBlockPointers[-scan -> rs_cbuf - 1] : ((Block )(BufferBlocks + ((Size )(scan -> rs_cbuf - 1)) * 8192)))))));
      lines = (((((PageHeader )dp) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )dp) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData )));
      if (!scan -> rs_inited) {
/* final offnum */
        lineoff = lines;
        scan -> rs_inited = ((bool )1);
      }
      else {
/* previous offnum */
        lineoff = ((OffsetNumber )(- 1 + ((((void )((bool )1)) , (&tuple -> t_self) -> ip_posid))));
      }
/* page and lineoff now reference the physically previous tid */
      linesleft = lineoff;
    }
    else {
/*
		 * ``no movement'' scan direction: refetch prior tuple
		 */
      if (!scan -> rs_inited) {
        ;
        tuple -> t_data = ((void *)0);
        return ;
      }
      page = ((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&(&tuple -> t_self) -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&(&tuple -> t_self) -> ip_blkid) -> bi_lo))))))));
      if (page != scan -> rs_cblock) {
        heapgetpage(scan,page);
      }
/* Since the tuple was previously fetched, needn't lock page here */
      dp = ((Page )((Page )((((void )((bool )1)) , (scan -> rs_cbuf < 0?LocalBufferBlockPointers[-scan -> rs_cbuf - 1] : ((Block )(BufferBlocks + ((Size )(scan -> rs_cbuf - 1)) * 8192)))))));
      lineoff = ((((void )((bool )1)) , (&tuple -> t_self) -> ip_posid));
      lpp = ((ItemId )(&((PageHeader )dp) -> pd_linp[lineoff - 1]));
      ;
      tuple -> t_data = ((HeapTupleHeader )(((((void )((bool )1)) , ((void )((bool )1))) , ((Item )(((char *)((Page )dp)) + lpp -> lp_off)))));
      tuple -> t_len = (lpp -> lp_len);
      return ;
    }
  }
/*
	 * advance the scan until we find a qualifying tuple or run out of stuff
	 * to scan
	 */
  lpp = ((ItemId )(&((PageHeader )dp) -> pd_linp[lineoff - 1]));
  for (; ; ) {
    while(linesleft > 0){
      if ((lpp -> lp_flags) == 1) {
        bool valid;
        tuple -> t_data = ((HeapTupleHeader )(((((void )((bool )1)) , ((void )((bool )1))) , ((Item )(((char *)((Page )dp)) + lpp -> lp_off)))));
        tuple -> t_len = (lpp -> lp_len);
        ((((void )((bool )1)) , (((((void )((bool )1)) , (&(&tuple -> t_self) -> ip_blkid) -> bi_hi = (page >> 16)) , (&(&tuple -> t_self) -> ip_blkid) -> bi_lo = (page & 0xffff)))) , (&tuple -> t_self) -> ip_posid = lineoff);
/*
				 * if current tuple qualifies, return it.
				 */
        valid = ( *snapshot -> satisfies)(tuple -> t_data,snapshot,scan -> rs_cbuf);
        CheckForSerializableConflictOut(valid,scan -> rs_rd,tuple,scan -> rs_cbuf,snapshot);
        if (valid && key != ((void *)0)) {
          do {
            int __cur_nkeys = nkeys;
            ScanKey __cur_keys = key;
            valid = ((bool )1);
            for (; __cur_nkeys--; __cur_keys++) {
              Datum __atp;
              bool __isnull;
              Datum __test;
              if (__cur_keys -> sk_flags & 0x0001) {
                valid = ((bool )0);
                break; 
              }
              __atp = ((__cur_keys -> sk_attno) > 0?(((__cur_keys -> sk_attno) > ((int )((tuple -> t_data -> t_infomask2) & 0x07FF))?(( *(&__isnull) = ((bool )1) , ((Datum )((void *)0)))) : (((((void )((bool )1)) ,  *(&__isnull) = ((bool )0)) , (!((tuple -> t_data -> t_infomask) & 0x0001)?((scan -> rs_rd -> rd_att -> attrs[(__cur_keys -> sk_attno) - 1] -> attcacheoff >= 0?((scan -> rs_rd -> rd_att -> attrs[(__cur_keys -> sk_attno) - 1] -> attbyval?(((scan -> rs_rd -> rd_att -> attrs[(__cur_keys -> sk_attno) - 1] -> attlen) == ((int )(sizeof(Datum )))? *((Datum *)(((char *)(tuple -> t_data)) + tuple -> t_data -> t_hoff + scan -> rs_rd -> rd_att -> attrs[(__cur_keys -> sk_attno) - 1] -> attcacheoff)) : (((scan -> rs_rd -> rd_att -> attrs[(__cur_keys -> sk_attno) - 1] -> attlen) == ((int )(sizeof(int32 )))?((Datum )(((Datum )( *((int32 *)(((char *)(tuple -> t_data)) + tuple -> t_data -> t_hoff + scan -> rs_rd -> rd_att -> attrs[(__cur_keys -> sk_attno) - 1] -> attcacheoff)))) & 0xFFFFFFFF)) : (((scan -> rs_rd -> rd_att -> attrs[(__cur_keys -> sk_attno) - 1] -> attlen) == ((int )(sizeof(int16 )))?((Datum )(((Datum )( *((int16 *)(((char *)(tuple -> t_data)) + tuple -> t_data -> t_hoff + scan -> rs_rd -> rd_att -> attrs[(__cur_keys -> sk_attno) - 1] -> attcacheoff)))) & 0xffff)) : ((((void )((bool )1)) , ((Datum )(((Datum )( *((char *)(((char *)(tuple -> t_data)) + tuple -> t_data -> t_hoff + scan -> rs_rd -> rd_att -> attrs[(__cur_keys -> sk_attno) - 1] -> attcacheoff)))) & 0x000000ff)))))))))) : ((Datum )((char *)(((char *)(tuple -> t_data)) + tuple -> t_data -> t_hoff + scan -> rs_rd -> rd_att -> attrs[(__cur_keys -> sk_attno) - 1] -> attcacheoff))))) : nocachegetattr(tuple,(__cur_keys -> sk_attno),scan -> rs_rd -> rd_att))) : ((!(tuple -> t_data -> t_bits[(__cur_keys -> sk_attno) - 1 >> 3] & 1 << ((__cur_keys -> sk_attno) - 1 & 0x07))?(( *(&__isnull) = ((bool )1) , ((Datum )((void *)0)))) : nocachegetattr(tuple,(__cur_keys -> sk_attno),scan -> rs_rd -> rd_att)))))))) : heap_getsysattr(tuple,(__cur_keys -> sk_attno),scan -> rs_rd -> rd_att,&__isnull));
              if (__isnull) {
                valid = ((bool )0);
                break; 
              }
              __test = FunctionCall2Coll(&__cur_keys -> sk_func,__cur_keys -> sk_collation,__atp,__cur_keys -> sk_argument);
              if (!((bool )(((bool )__test) != 0))) {
                valid = ((bool )0);
                break; 
              }
            }
          }while (0);
        }
        if (valid) {
          LockBuffer(scan -> rs_cbuf,0);
          return ;
        }
      }
/*
			 * otherwise move to the next item on the page
			 */
      --linesleft;
      if (backward) {
/* move back in this page's ItemId array */
        --lpp;
        --lineoff;
      }
      else {
/* move forward in this page's ItemId array */
        ++lpp;
        ++lineoff;
      }
    }
/*
		 * if we get here, it means we've exhausted the items on this page and
		 * it's time to move to the next.
		 */
    LockBuffer(scan -> rs_cbuf,0);
/*
		 * advance to next/prior page and detect end of scan
		 */
    if (backward) {
      finished = (page == scan -> rs_startblock);
      if (page == 0) {
        page = scan -> rs_nblocks;
      }
      page--;
    }
    else {
      page++;
      if (page >= scan -> rs_nblocks) {
        page = 0;
      }
      finished = (page == scan -> rs_startblock);
/*
			 * Report our new scan position for synchronization purposes. We
			 * don't do that when moving backwards, however. That would just
			 * mess up any other forward-moving scanners.
			 *
			 * Note: we do this before checking for end of scan so that the
			 * final state of the position hint is back at the start of the
			 * rel.  That's not strictly necessary, but otherwise when you run
			 * the same query multiple times the starting position would shift
			 * a little bit backwards on every invocation, which is confusing.
			 * We don't guarantee any specific ordering in general, though.
			 */
      if (scan -> rs_syncscan) {
        ss_report_location(scan -> rs_rd,page);
      }
    }
/*
		 * return NULL if we've exhausted all the pages
		 */
    if (finished) {
      if ((((void )((bool )1)) , scan -> rs_cbuf != 0)) {
        ReleaseBuffer(scan -> rs_cbuf);
      }
      scan -> rs_cbuf = 0;
      scan -> rs_cblock = ((BlockNumber )0xFFFFFFFF);
      tuple -> t_data = ((void *)0);
      scan -> rs_inited = ((bool )0);
      return ;
    }
    heapgetpage(scan,page);
    LockBuffer(scan -> rs_cbuf,1);
    dp = ((Page )((Page )((((void )((bool )1)) , (scan -> rs_cbuf < 0?LocalBufferBlockPointers[-scan -> rs_cbuf - 1] : ((Block )(BufferBlocks + ((Size )(scan -> rs_cbuf - 1)) * 8192)))))));
    lines = (((((PageHeader )((Page )dp)) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )((Page )dp)) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData )));
    linesleft = lines;
    if (backward) {
      lineoff = lines;
      lpp = ((ItemId )(&((PageHeader )dp) -> pd_linp[lines - 1]));
    }
    else {
      lineoff = ((OffsetNumber )1);
      lpp = ((ItemId )(&((PageHeader )dp) -> pd_linp[((OffsetNumber )1) - 1]));
    }
  }
}
/* ----------------
 *		heapgettup_pagemode - fetch next heap tuple in page-at-a-time mode
 *
 *		Same API as heapgettup, but used in page-at-a-time mode
 *
 * The internal logic is much the same as heapgettup's too, but there are some
 * differences: we do not take the buffer content lock (that only needs to
 * happen inside heapgetpage), and we iterate through just the tuples listed
 * in rs_vistuples[] rather than all tuples on the page.  Notice that
 * lineindex is 0-based, where the corresponding loop variable lineoff in
 * heapgettup is 1-based.
 * ----------------
 */

static void heapgettup_pagemode(HeapScanDesc scan,ScanDirection dir,int nkeys,ScanKey key)
{
  HeapTuple tuple = &scan -> rs_ctup;
  bool backward = (bool )(dir == BackwardScanDirection);
  BlockNumber page;
  bool finished;
  Page dp;
  int lines;
  int lineindex;
  OffsetNumber lineoff;
  int linesleft;
  ItemId lpp;
/*
	 * calculate next starting lineindex, given scan direction
	 */
  if ((bool )(dir == ForwardScanDirection)) {
    if (!scan -> rs_inited) {
/*
			 * return null immediately if relation is empty
			 */
      if (scan -> rs_nblocks == 0) {
        ;
        tuple -> t_data = ((void *)0);
        return ;
      }
/* first page */
      page = scan -> rs_startblock;
      heapgetpage(scan,page);
      lineindex = 0;
      scan -> rs_inited = ((bool )1);
    }
    else {
/* continue from previously returned page/tuple */
/* current page */
      page = scan -> rs_cblock;
      lineindex = scan -> rs_cindex + 1;
    }
    dp = ((Page )((Page )((((void )((bool )1)) , (scan -> rs_cbuf < 0?LocalBufferBlockPointers[-scan -> rs_cbuf - 1] : ((Block )(BufferBlocks + ((Size )(scan -> rs_cbuf - 1)) * 8192)))))));
    lines = scan -> rs_ntuples;
/* page and lineindex now reference the next visible tid */
    linesleft = lines - lineindex;
  }
  else {
    if (backward) {
      if (!scan -> rs_inited) {
/*
			 * return null immediately if relation is empty
			 */
        if (scan -> rs_nblocks == 0) {
          ;
          tuple -> t_data = ((void *)0);
          return ;
        }
/*
			 * Disable reporting to syncscan logic in a backwards scan; it's
			 * not very likely anyone else is doing the same thing at the same
			 * time, and much more likely that we'll just bollix things for
			 * forward scanners.
			 */
        scan -> rs_syncscan = ((bool )0);
/* start from last page of the scan */
        if (scan -> rs_startblock > 0) {
          page = scan -> rs_startblock - 1;
        }
        else {
          page = scan -> rs_nblocks - 1;
        }
        heapgetpage(scan,page);
      }
      else {
/* continue from previously returned page/tuple */
/* current page */
        page = scan -> rs_cblock;
      }
      dp = ((Page )((Page )((((void )((bool )1)) , (scan -> rs_cbuf < 0?LocalBufferBlockPointers[-scan -> rs_cbuf - 1] : ((Block )(BufferBlocks + ((Size )(scan -> rs_cbuf - 1)) * 8192)))))));
      lines = scan -> rs_ntuples;
      if (!scan -> rs_inited) {
        lineindex = lines - 1;
        scan -> rs_inited = ((bool )1);
      }
      else {
        lineindex = scan -> rs_cindex - 1;
      }
/* page and lineindex now reference the previous visible tid */
      linesleft = lineindex + 1;
    }
    else {
/*
		 * ``no movement'' scan direction: refetch prior tuple
		 */
      if (!scan -> rs_inited) {
        ;
        tuple -> t_data = ((void *)0);
        return ;
      }
      page = ((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&(&tuple -> t_self) -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&(&tuple -> t_self) -> ip_blkid) -> bi_lo))))))));
      if (page != scan -> rs_cblock) {
        heapgetpage(scan,page);
      }
/* Since the tuple was previously fetched, needn't lock page here */
      dp = ((Page )((Page )((((void )((bool )1)) , (scan -> rs_cbuf < 0?LocalBufferBlockPointers[-scan -> rs_cbuf - 1] : ((Block )(BufferBlocks + ((Size )(scan -> rs_cbuf - 1)) * 8192)))))));
      lineoff = ((((void )((bool )1)) , (&tuple -> t_self) -> ip_posid));
      lpp = ((ItemId )(&((PageHeader )dp) -> pd_linp[lineoff - 1]));
      ;
      tuple -> t_data = ((HeapTupleHeader )(((((void )((bool )1)) , ((void )((bool )1))) , ((Item )(((char *)((Page )dp)) + lpp -> lp_off)))));
      tuple -> t_len = (lpp -> lp_len);
/* check that rs_cindex is in sync */
      ;
      ;
      return ;
    }
  }
/*
	 * advance the scan until we find a qualifying tuple or run out of stuff
	 * to scan
	 */
  for (; ; ) {
    while(linesleft > 0){
      lineoff = scan -> rs_vistuples[lineindex];
      lpp = ((ItemId )(&((PageHeader )dp) -> pd_linp[lineoff - 1]));
      ;
      tuple -> t_data = ((HeapTupleHeader )(((((void )((bool )1)) , ((void )((bool )1))) , ((Item )(((char *)((Page )dp)) + lpp -> lp_off)))));
      tuple -> t_len = (lpp -> lp_len);
      ((((void )((bool )1)) , (((((void )((bool )1)) , (&(&tuple -> t_self) -> ip_blkid) -> bi_hi = (page >> 16)) , (&(&tuple -> t_self) -> ip_blkid) -> bi_lo = (page & 0xffff)))) , (&tuple -> t_self) -> ip_posid = lineoff);
/*
			 * if current tuple qualifies, return it.
			 */
      if (key != ((void *)0)) {
        bool valid;
        do {
          int __cur_nkeys = nkeys;
          ScanKey __cur_keys = key;
          valid = ((bool )1);
          for (; __cur_nkeys--; __cur_keys++) {
            Datum __atp;
            bool __isnull;
            Datum __test;
            if (__cur_keys -> sk_flags & 0x0001) {
              valid = ((bool )0);
              break; 
            }
            __atp = ((__cur_keys -> sk_attno) > 0?(((__cur_keys -> sk_attno) > ((int )((tuple -> t_data -> t_infomask2) & 0x07FF))?(( *(&__isnull) = ((bool )1) , ((Datum )((void *)0)))) : (((((void )((bool )1)) ,  *(&__isnull) = ((bool )0)) , (!((tuple -> t_data -> t_infomask) & 0x0001)?((scan -> rs_rd -> rd_att -> attrs[(__cur_keys -> sk_attno) - 1] -> attcacheoff >= 0?((scan -> rs_rd -> rd_att -> attrs[(__cur_keys -> sk_attno) - 1] -> attbyval?(((scan -> rs_rd -> rd_att -> attrs[(__cur_keys -> sk_attno) - 1] -> attlen) == ((int )(sizeof(Datum )))? *((Datum *)(((char *)(tuple -> t_data)) + tuple -> t_data -> t_hoff + scan -> rs_rd -> rd_att -> attrs[(__cur_keys -> sk_attno) - 1] -> attcacheoff)) : (((scan -> rs_rd -> rd_att -> attrs[(__cur_keys -> sk_attno) - 1] -> attlen) == ((int )(sizeof(int32 )))?((Datum )(((Datum )( *((int32 *)(((char *)(tuple -> t_data)) + tuple -> t_data -> t_hoff + scan -> rs_rd -> rd_att -> attrs[(__cur_keys -> sk_attno) - 1] -> attcacheoff)))) & 0xFFFFFFFF)) : (((scan -> rs_rd -> rd_att -> attrs[(__cur_keys -> sk_attno) - 1] -> attlen) == ((int )(sizeof(int16 )))?((Datum )(((Datum )( *((int16 *)(((char *)(tuple -> t_data)) + tuple -> t_data -> t_hoff + scan -> rs_rd -> rd_att -> attrs[(__cur_keys -> sk_attno) - 1] -> attcacheoff)))) & 0xffff)) : ((((void )((bool )1)) , ((Datum )(((Datum )( *((char *)(((char *)(tuple -> t_data)) + tuple -> t_data -> t_hoff + scan -> rs_rd -> rd_att -> attrs[(__cur_keys -> sk_attno) - 1] -> attcacheoff)))) & 0x000000ff)))))))))) : ((Datum )((char *)(((char *)(tuple -> t_data)) + tuple -> t_data -> t_hoff + scan -> rs_rd -> rd_att -> attrs[(__cur_keys -> sk_attno) - 1] -> attcacheoff))))) : nocachegetattr(tuple,(__cur_keys -> sk_attno),scan -> rs_rd -> rd_att))) : ((!(tuple -> t_data -> t_bits[(__cur_keys -> sk_attno) - 1 >> 3] & 1 << ((__cur_keys -> sk_attno) - 1 & 0x07))?(( *(&__isnull) = ((bool )1) , ((Datum )((void *)0)))) : nocachegetattr(tuple,(__cur_keys -> sk_attno),scan -> rs_rd -> rd_att)))))))) : heap_getsysattr(tuple,(__cur_keys -> sk_attno),scan -> rs_rd -> rd_att,&__isnull));
            if (__isnull) {
              valid = ((bool )0);
              break; 
            }
            __test = FunctionCall2Coll(&__cur_keys -> sk_func,__cur_keys -> sk_collation,__atp,__cur_keys -> sk_argument);
            if (!((bool )(((bool )__test) != 0))) {
              valid = ((bool )0);
              break; 
            }
          }
        }while (0);
        if (valid) {
          scan -> rs_cindex = lineindex;
          return ;
        }
      }
      else {
        scan -> rs_cindex = lineindex;
        return ;
      }
/*
			 * otherwise move to the next item on the page
			 */
      --linesleft;
      if (backward) {
        --lineindex;
      }
      else {
        ++lineindex;
      }
    }
/*
		 * if we get here, it means we've exhausted the items on this page and
		 * it's time to move to the next.
		 */
    if (backward) {
      finished = (page == scan -> rs_startblock);
      if (page == 0) {
        page = scan -> rs_nblocks;
      }
      page--;
    }
    else {
      page++;
      if (page >= scan -> rs_nblocks) {
        page = 0;
      }
      finished = (page == scan -> rs_startblock);
/*
			 * Report our new scan position for synchronization purposes. We
			 * don't do that when moving backwards, however. That would just
			 * mess up any other forward-moving scanners.
			 *
			 * Note: we do this before checking for end of scan so that the
			 * final state of the position hint is back at the start of the
			 * rel.  That's not strictly necessary, but otherwise when you run
			 * the same query multiple times the starting position would shift
			 * a little bit backwards on every invocation, which is confusing.
			 * We don't guarantee any specific ordering in general, though.
			 */
      if (scan -> rs_syncscan) {
        ss_report_location(scan -> rs_rd,page);
      }
    }
/*
		 * return NULL if we've exhausted all the pages
		 */
    if (finished) {
      if ((((void )((bool )1)) , scan -> rs_cbuf != 0)) {
        ReleaseBuffer(scan -> rs_cbuf);
      }
      scan -> rs_cbuf = 0;
      scan -> rs_cblock = ((BlockNumber )0xFFFFFFFF);
      tuple -> t_data = ((void *)0);
      scan -> rs_inited = ((bool )0);
      return ;
    }
    heapgetpage(scan,page);
    dp = ((Page )((Page )((((void )((bool )1)) , (scan -> rs_cbuf < 0?LocalBufferBlockPointers[-scan -> rs_cbuf - 1] : ((Block )(BufferBlocks + ((Size )(scan -> rs_cbuf - 1)) * 8192)))))));
    lines = scan -> rs_ntuples;
    linesleft = lines;
    if (backward) {
      lineindex = lines - 1;
    }
    else {
      lineindex = 0;
    }
  }
}
#if defined(DISABLE_COMPLEX_MACRO)
/*
 * This is formatted so oddly so that the correspondence to the macro
 * definition in access/htup.h is maintained.
 */
#endif   /* defined(DISABLE_COMPLEX_MACRO) */
/* ----------------------------------------------------------------
 *					 heap access method interface
 * ----------------------------------------------------------------
 */
/* ----------------
 *		relation_open - open any relation by relation OID
 *
 *		If lockmode is not "NoLock", the specified kind of lock is
 *		obtained on the relation.  (Generally, NoLock should only be
 *		used if the caller knows it has some appropriate lock on the
 *		relation already.)
 *
 *		An error is raised if the relation does not exist.
 *
 *		NB: a "relation" is anything with a pg_class entry.  The caller is
 *		expected to check whether the relkind is something it can handle.
 * ----------------
 */

Relation relation_open(Oid relationId,LOCKMODE lockmode)
{
  Relation r;
  ;
/* Get the lock before trying to open the relcache entry */
  if (lockmode != 0) {
    LockRelationOid(relationId,lockmode);
  }
/* The relcache does all the real work... */
  r = RelationIdGetRelation(relationId);
  if (!(((const void *)r) != ((void *)0))) {
    (elog_start("heapam.c",920,__func__) , elog_finish(20,"could not open relation with OID %u",relationId));
  }
/* Make note that we've accessed a temporary relation */
  if ((r -> rd_rel -> relpersistence) == 't') {
    MyXactAccessedTempRel = ((bool )1);
  }
  pgstat_initstats(r);
  return r;
}
/* ----------------
 *		try_relation_open - open any relation by relation OID
 *
 *		Same as relation_open, except return NULL instead of failing
 *		if the relation does not exist.
 * ----------------
 */

Relation try_relation_open(Oid relationId,LOCKMODE lockmode)
{
  Relation r;
  ;
/* Get the lock first */
  if (lockmode != 0) {
    LockRelationOid(relationId,lockmode);
  }
/*
	 * Now that we have the lock, probe to see if the relation really exists
	 * or not.
	 */
  if (!SearchSysCacheExists(RELOID,((Datum )(((Datum )relationId) & 0xFFFFFFFF)),0,0,0)) {
/* Release useless lock */
    if (lockmode != 0) {
      UnlockRelationOid(relationId,lockmode);
    }
    return ((void *)0);
  }
/* Should be safe to do a relcache load */
  r = RelationIdGetRelation(relationId);
  if (!(((const void *)r) != ((void *)0))) {
    (elog_start("heapam.c",966,__func__) , elog_finish(20,"could not open relation with OID %u",relationId));
  }
/* Make note that we've accessed a temporary relation */
  if ((r -> rd_rel -> relpersistence) == 't') {
    MyXactAccessedTempRel = ((bool )1);
  }
  pgstat_initstats(r);
  return r;
}
/* ----------------
 *		relation_openrv - open any relation specified by a RangeVar
 *
 *		Same as relation_open, but the relation is specified by a RangeVar.
 * ----------------
 */

Relation relation_openrv(const RangeVar *relation,LOCKMODE lockmode)
{
  Oid relOid;
/*
	 * Check for shared-cache-inval messages before trying to open the
	 * relation.  This is needed even if we already hold a lock on the
	 * relation, because GRANT/REVOKE are executed without taking any lock on
	 * the target relation, and we want to be sure we see current ACL
	 * information.  We can skip this if asked for NoLock, on the assumption
	 * that such a call is not the first one in the current command, and so we
	 * should be reasonably up-to-date already.  (XXX this all could stand to
	 * be redesigned, but for the moment we'll keep doing this like it's been
	 * done historically.)
	 */
  if (lockmode != 0) {
    AcceptInvalidationMessages();
  }
/* Look up and lock the appropriate relation using namespace search */
  relOid = RangeVarGetRelidExtended(relation,lockmode,((bool )0),((bool )0),((void *)0),((void *)0));
/* Let relation_open do the rest */
  return relation_open(relOid,0);
}
/* ----------------
 *		relation_openrv_extended - open any relation specified by a RangeVar
 *
 *		Same as relation_openrv, but with an additional missing_ok argument
 *		allowing a NULL return rather than an error if the relation is not
 *		found.	(Note that some other causes, such as permissions problems,
 *		will still result in an ereport.)
 * ----------------
 */

Relation relation_openrv_extended(const RangeVar *relation,LOCKMODE lockmode,bool missing_ok)
{
  Oid relOid;
/*
	 * Check for shared-cache-inval messages before trying to open the
	 * relation.  See comments in relation_openrv().
	 */
  if (lockmode != 0) {
    AcceptInvalidationMessages();
  }
/* Look up and lock the appropriate relation using namespace search */
  relOid = RangeVarGetRelidExtended(relation,lockmode,missing_ok,((bool )0),((void *)0),((void *)0));
/* Return NULL on not-found */
  if (!((bool )(relOid != ((Oid )0)))) {
    return ((void *)0);
  }
/* Let relation_open do the rest */
  return relation_open(relOid,0);
}
/* ----------------
 *		relation_close - close any relation
 *
 *		If lockmode is not "NoLock", we then release the specified lock.
 *
 *		Note that it is often sensible to hold a lock beyond relation_close;
 *		in that case, the lock is released automatically at xact end.
 * ----------------
 */

void relation_close(Relation relation,LOCKMODE lockmode)
{
  LockRelId relid = relation -> rd_lockInfo . lockRelId;
  ;
/* The relcache does the real work... */
  RelationClose(relation);
  if (lockmode != 0) {
    UnlockRelationId(&relid,lockmode);
  }
}
/* ----------------
 *		heap_open - open a heap relation by relation OID
 *
 *		This is essentially relation_open plus check that the relation
 *		is not an index nor a composite type.  (The caller should also
 *		check that it's not a view or foreign table before assuming it has
 *		storage.)
 * ----------------
 */

Relation heap_open(Oid relationId,LOCKMODE lockmode)
{
  Relation r;
  r = relation_open(relationId,lockmode);
  if ((r -> rd_rel -> relkind) == 'i') {
    errstart(20,"heapam.c",1086,__func__,((void *)0))?errfinish(errcode(('4' - 48 & 0x3F) + (('2' - 48 & 0x3F) << 6) + ((56 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('9' - 48 & 0x3F) << 24)),errmsg("\"%s\" is an index",r -> rd_rel -> relname . data)) : ((void )0);
  }
  else {
    if ((r -> rd_rel -> relkind) == 'c') {
      errstart(20,"heapam.c",1091,__func__,((void *)0))?errfinish(errcode(('4' - 48 & 0x3F) + (('2' - 48 & 0x3F) << 6) + ((56 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('9' - 48 & 0x3F) << 24)),errmsg("\"%s\" is a composite type",r -> rd_rel -> relname . data)) : ((void )0);
    }
  }
  return r;
}
/* ----------------
 *		heap_openrv - open a heap relation specified
 *		by a RangeVar node
 *
 *		As above, but relation is specified by a RangeVar.
 * ----------------
 */

Relation heap_openrv(const RangeVar *relation,LOCKMODE lockmode)
{
  Relation r;
  r = relation_openrv(relation,lockmode);
  if ((r -> rd_rel -> relkind) == 'i') {
    errstart(20,"heapam.c",1114,__func__,((void *)0))?errfinish(errcode(('4' - 48 & 0x3F) + (('2' - 48 & 0x3F) << 6) + ((56 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('9' - 48 & 0x3F) << 24)),errmsg("\"%s\" is an index",r -> rd_rel -> relname . data)) : ((void )0);
  }
  else {
    if ((r -> rd_rel -> relkind) == 'c') {
      errstart(20,"heapam.c",1119,__func__,((void *)0))?errfinish(errcode(('4' - 48 & 0x3F) + (('2' - 48 & 0x3F) << 6) + ((56 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('9' - 48 & 0x3F) << 24)),errmsg("\"%s\" is a composite type",r -> rd_rel -> relname . data)) : ((void )0);
    }
  }
  return r;
}
/* ----------------
 *		heap_openrv_extended - open a heap relation specified
 *		by a RangeVar node
 *
 *		As above, but optionally return NULL instead of failing for
 *		relation-not-found.
 * ----------------
 */

Relation heap_openrv_extended(const RangeVar *relation,LOCKMODE lockmode,bool missing_ok)
{
  Relation r;
  r = relation_openrv_extended(relation,lockmode,missing_ok);
  if (r) {
    if ((r -> rd_rel -> relkind) == 'i') {
      errstart(20,"heapam.c",1146,__func__,((void *)0))?errfinish(errcode(('4' - 48 & 0x3F) + (('2' - 48 & 0x3F) << 6) + ((56 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('9' - 48 & 0x3F) << 24)),errmsg("\"%s\" is an index",r -> rd_rel -> relname . data)) : ((void )0);
    }
    else {
      if ((r -> rd_rel -> relkind) == 'c') {
        errstart(20,"heapam.c",1151,__func__,((void *)0))?errfinish(errcode(('4' - 48 & 0x3F) + (('2' - 48 & 0x3F) << 6) + ((56 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('9' - 48 & 0x3F) << 24)),errmsg("\"%s\" is a composite type",r -> rd_rel -> relname . data)) : ((void )0);
      }
    }
  }
  return r;
}
/* ----------------
 *		heap_beginscan	- begin relation scan
 *
 * heap_beginscan_strat offers an extended API that lets the caller control
 * whether a nondefault buffer access strategy can be used, and whether
 * syncscan can be chosen (possibly resulting in the scan not starting from
 * block zero).  Both of these default to TRUE with plain heap_beginscan.
 *
 * heap_beginscan_bm is an alternative entry point for setting up a
 * HeapScanDesc for a bitmap heap scan.  Although that scan technology is
 * really quite unlike a standard seqscan, there is just enough commonality
 * to make it worth using the same data structure.
 * ----------------
 */

HeapScanDesc heap_beginscan(Relation relation,Snapshot snapshot,int nkeys,ScanKey key)
{
  return heap_beginscan_internal(relation,snapshot,nkeys,key,((bool )1),((bool )1),((bool )0));
}

HeapScanDesc heap_beginscan_strat(Relation relation,Snapshot snapshot,int nkeys,ScanKey key,bool allow_strat,bool allow_sync)
{
  return heap_beginscan_internal(relation,snapshot,nkeys,key,allow_strat,allow_sync,((bool )0));
}

HeapScanDesc heap_beginscan_bm(Relation relation,Snapshot snapshot,int nkeys,ScanKey key)
{
  return heap_beginscan_internal(relation,snapshot,nkeys,key,((bool )0),((bool )0),((bool )1));
}

static HeapScanDesc heap_beginscan_internal(Relation relation,Snapshot snapshot,int nkeys,ScanKey key,bool allow_strat,bool allow_sync,bool is_bitmapscan)
{
  HeapScanDesc scan;
/*
	 * increment relation ref count while scanning relation
	 *
	 * This is just to make really sure the relcache entry won't go away while
	 * the scan has a pointer to it.  Caller should be holding the rel open
	 * anyway, so this is redundant in all normal scenarios...
	 */
  RelationIncrementReferenceCount(relation);
/*
	 * allocate and initialize scan descriptor
	 */
  scan = ((HeapScanDesc )(MemoryContextAlloc(CurrentMemoryContext,sizeof(HeapScanDescData ))));
  scan -> rs_rd = relation;
  scan -> rs_snapshot = snapshot;
  scan -> rs_nkeys = nkeys;
  scan -> rs_bitmapscan = is_bitmapscan;
/* set in initscan */
  scan -> rs_strategy = ((void *)0);
  scan -> rs_allow_strat = allow_strat;
  scan -> rs_allow_sync = allow_sync;
/*
	 * we can use page-at-a-time mode if it's an MVCC-safe snapshot
	 */
  scan -> rs_pageatatime = (snapshot -> satisfies == HeapTupleSatisfiesMVCC);
/*
	 * For a seqscan in a serializable transaction, acquire a predicate lock
	 * on the entire relation. This is required not only to lock all the
	 * matching tuples, but also to conflict with new insertions into the
	 * table. In an indexscan, we take page locks on the index pages covering
	 * the range specified in the scan qual, but in a heap scan there is
	 * nothing more fine-grained to lock. A bitmap scan is a different story,
	 * there we have already scanned the index and locked the index pages
	 * covering the predicate. But in that case we still have to lock any
	 * matching heap tuples.
	 */
  if (!is_bitmapscan) {
    PredicateLockRelation(relation,snapshot);
  }
/* we only need to set this up once */
  scan -> rs_ctup . t_tableOid = relation -> rd_id;
/*
	 * we do this here instead of in initscan() because heap_rescan also calls
	 * initscan() and we don't want to allocate memory again
	 */
  if (nkeys > 0) {
    scan -> rs_key = ((ScanKey )(MemoryContextAlloc(CurrentMemoryContext,sizeof(ScanKeyData ) * nkeys)));
  }
  else {
    scan -> rs_key = ((void *)0);
  }
  initscan(scan,key,((bool )0));
  return scan;
}
/* ----------------
 *		heap_rescan		- restart a relation scan
 * ----------------
 */

void heap_rescan(HeapScanDesc scan,ScanKey key)
{
/*
	 * unpin scan buffers
	 */
  if ((((void )((bool )1)) , scan -> rs_cbuf != 0)) {
    ReleaseBuffer(scan -> rs_cbuf);
  }
/*
	 * reinitialize scan descriptor
	 */
  initscan(scan,key,((bool )1));
}
/* ----------------
 *		heap_endscan	- end relation scan
 *
 *		See how to integrate with index scans.
 *		Check handling if reldesc caching.
 * ----------------
 */

void heap_endscan(HeapScanDesc scan)
{
/* Note: no locking manipulations needed */
/*
	 * unpin scan buffers
	 */
  if ((((void )((bool )1)) , scan -> rs_cbuf != 0)) {
    ReleaseBuffer(scan -> rs_cbuf);
  }
/*
	 * decrement relation reference count and free scan descriptor storage
	 */
  RelationDecrementReferenceCount(scan -> rs_rd);
  if (scan -> rs_key) {
    pfree((scan -> rs_key));
  }
  if (scan -> rs_strategy != ((void *)0)) {
    FreeAccessStrategy(scan -> rs_strategy);
  }
  pfree(scan);
}
/* ----------------
 *		heap_getnext	- retrieve next tuple in scan
 *
 *		Fix to work with index relations.
 *		We don't return the buffer anymore, but you can get it from the
 *		returned HeapTuple.
 * ----------------
 */
#ifdef HEAPDEBUGALL
#define HEAPDEBUG_1 \
	elog(DEBUG2, "heap_getnext([%s,nkeys=%d],dir=%d) called", \
		 RelationGetRelationName(scan->rs_rd), scan->rs_nkeys, (int) direction)
#define HEAPDEBUG_2 \
	elog(DEBUG2, "heap_getnext returning EOS")
#define HEAPDEBUG_3 \
	elog(DEBUG2, "heap_getnext returning tuple")
#else
#define HEAPDEBUG_1
#define HEAPDEBUG_2
#define HEAPDEBUG_3
#endif   /* !defined(HEAPDEBUGALL) */

HeapTuple heap_getnext(HeapScanDesc scan,ScanDirection direction)
{
/* Note: no locking manipulations needed */
/* heap_getnext( info ) */
  ;
  if (scan -> rs_pageatatime) {
    heapgettup_pagemode(scan,direction,scan -> rs_nkeys,scan -> rs_key);
  }
  else {
    heapgettup(scan,direction,scan -> rs_nkeys,scan -> rs_key);
  }
  if (scan -> rs_ctup . t_data == ((void *)0)) {
/* heap_getnext returning EOS */
    ;
    return ((void *)0);
  }
/*
	 * if we get here it means we have a new current scan tuple, so point to
	 * the proper return buffer and return the tuple.
	 */
/* heap_getnext returning tuple */
  ;
  do {
    if (scan -> rs_rd -> pgstat_info != ((void *)0)) {
      scan -> rs_rd -> pgstat_info -> t_counts . t_tuples_returned++;
    }
  }while (0);
  return &scan -> rs_ctup;
}
/*
 *	heap_fetch		- retrieve tuple with given tid
 *
 * On entry, tuple->t_self is the TID to fetch.  We pin the buffer holding
 * the tuple, fill in the remaining fields of *tuple, and check the tuple
 * against the specified snapshot.
 *
 * If successful (tuple found and passes snapshot time qual), then *userbuf
 * is set to the buffer holding the tuple and TRUE is returned.  The caller
 * must unpin the buffer when done with the tuple.
 *
 * If the tuple is not found (ie, item number references a deleted slot),
 * then tuple->t_data is set to NULL and FALSE is returned.
 *
 * If the tuple is found but fails the time qual check, then FALSE is returned
 * but tuple->t_data is left pointing to the tuple.
 *
 * keep_buf determines what is done with the buffer in the FALSE-result cases.
 * When the caller specifies keep_buf = true, we retain the pin on the buffer
 * and return it in *userbuf (so the caller must eventually unpin it); when
 * keep_buf = false, the pin is released and *userbuf is set to InvalidBuffer.
 *
 * stats_relation is the relation to charge the heap_fetch operation against
 * for statistical purposes.  (This could be the heap rel itself, an
 * associated index, or NULL to not count the fetch at all.)
 *
 * heap_fetch does not follow HOT chains: only the exact TID requested will
 * be fetched.
 *
 * It is somewhat inconsistent that we ereport() on invalid block number but
 * return false on invalid item number.  There are a couple of reasons though.
 * One is that the caller can relatively easily check the block number for
 * validity, but cannot check the item number without reading the page
 * himself.  Another is that when we are following a t_ctid link, we can be
 * reasonably confident that the page number is valid (since VACUUM shouldn't
 * truncate off the destination page without having killed the referencing
 * tuple first), but the item number might well not be good.
 */

bool heap_fetch(Relation relation,Snapshot snapshot,HeapTuple tuple,Buffer *userbuf,bool keep_buf,Relation stats_relation)
{
  ItemPointer tid = &tuple -> t_self;
  ItemId lp;
  Buffer buffer;
  Page page;
  OffsetNumber offnum;
  bool valid;
/*
	 * Fetch and pin the appropriate page of the relation.
	 */
  buffer = ReadBuffer(relation,((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&tid -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&tid -> ip_blkid) -> bi_lo)))))))));
/*
	 * Need share lock on buffer to examine tuple commit status.
	 */
  LockBuffer(buffer,1);
  page = ((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192))))));
/*
	 * We'd better check for out-of-range offnum in case of VACUUM since the
	 * TID was obtained.
	 */
  offnum = ((((void )((bool )1)) , tid -> ip_posid));
  if (offnum < ((OffsetNumber )1) || offnum > (((((PageHeader )page) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )page) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData )))) {
    LockBuffer(buffer,0);
    if (keep_buf) {
       *userbuf = buffer;
    }
    else {
      ReleaseBuffer(buffer);
       *userbuf = 0;
    }
    tuple -> t_data = ((void *)0);
    return (bool )0;
  }
/*
	 * get the item line pointer corresponding to the requested tid
	 */
  lp = ((ItemId )(&((PageHeader )page) -> pd_linp[offnum - 1]));
/*
	 * Must check for deleted tuple.
	 */
  if (!((lp -> lp_flags) == 1)) {
    LockBuffer(buffer,0);
    if (keep_buf) {
       *userbuf = buffer;
    }
    else {
      ReleaseBuffer(buffer);
       *userbuf = 0;
    }
    tuple -> t_data = ((void *)0);
    return (bool )0;
  }
/*
	 * fill in *tuple fields
	 */
  tuple -> t_data = ((HeapTupleHeader )(((((void )((bool )1)) , ((void )((bool )1))) , ((Item )(((char *)page) + lp -> lp_off)))));
  tuple -> t_len = (lp -> lp_len);
  tuple -> t_tableOid = relation -> rd_id;
/*
	 * check time qualification of tuple, then release lock
	 */
  valid = ( *snapshot -> satisfies)(tuple -> t_data,snapshot,buffer);
  if (valid) {
    PredicateLockTuple(relation,tuple,snapshot);
  }
  CheckForSerializableConflictOut(valid,relation,tuple,buffer,snapshot);
  LockBuffer(buffer,0);
  if (valid) {
/*
		 * All checks passed, so return the tuple as valid. Caller is now
		 * responsible for releasing the buffer.
		 */
     *userbuf = buffer;
/* Count the successful fetch against appropriate rel, if any */
    if (stats_relation != ((void *)0)) {
      do {
        if (stats_relation -> pgstat_info != ((void *)0)) {
          stats_relation -> pgstat_info -> t_counts . t_tuples_fetched++;
        }
      }while (0);
    }
    return (bool )1;
  }
/* Tuple failed time qual, but maybe caller wants to see it anyway. */
  if (keep_buf) {
     *userbuf = buffer;
  }
  else {
    ReleaseBuffer(buffer);
     *userbuf = 0;
  }
  return (bool )0;
}
/*
 *	heap_hot_search_buffer	- search HOT chain for tuple satisfying snapshot
 *
 * On entry, *tid is the TID of a tuple (either a simple tuple, or the root
 * of a HOT chain), and buffer is the buffer holding this tuple.  We search
 * for the first chain member satisfying the given snapshot.  If one is
 * found, we update *tid to reference that tuple's offset number, and
 * return TRUE.  If no match, return FALSE without modifying *tid.
 *
 * heapTuple is a caller-supplied buffer.  When a match is found, we return
 * the tuple here, in addition to updating *tid.  If no match is found, the
 * contents of this buffer on return are undefined.
 *
 * If all_dead is not NULL, we check non-visible tuples to see if they are
 * globally dead; *all_dead is set TRUE if all members of the HOT chain
 * are vacuumable, FALSE if not.
 *
 * Unlike heap_fetch, the caller must already have pin and (at least) share
 * lock on the buffer; it is still pinned/locked at exit.  Also unlike
 * heap_fetch, we do not report any pgstats count; caller may do so if wanted.
 */

bool heap_hot_search_buffer(ItemPointer tid,Relation relation,Buffer buffer,Snapshot snapshot,HeapTuple heapTuple,bool *all_dead,bool first_call)
{
  Page dp = (Page )((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192))))));
  TransactionId prev_xmax = (TransactionId )0;
  OffsetNumber offnum;
  bool at_chain_start;
  bool valid;
  bool skip;
/* If this is not the first call, previous call returned a (live!) tuple */
  if (all_dead) {
     *all_dead = first_call;
  }
  ;
  ;
  offnum = ((((void )((bool )1)) , tid -> ip_posid));
  at_chain_start = first_call;
  skip = (!first_call);
/* Scan through possible multiple members of HOT-chain */
  for (; ; ) {
    ItemId lp;
/* check for bogus TID */
    if (offnum < ((OffsetNumber )1) || offnum > (((((PageHeader )dp) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )dp) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData )))) {
      break; 
    }
    lp = ((ItemId )(&((PageHeader )dp) -> pd_linp[offnum - 1]));
/* check for unused, dead, or redirected items */
    if (!((lp -> lp_flags) == 1)) {
/* We should only see a redirect at start of chain */
      if ((lp -> lp_flags) == 2 && at_chain_start) {
/* Follow the redirect */
        offnum = (lp -> lp_off);
        at_chain_start = ((bool )0);
        continue; 
      }
/* else must be end of chain */
      break; 
    }
    heapTuple -> t_data = ((HeapTupleHeader )(((((void )((bool )1)) , ((void )((bool )1))) , ((Item )(((char *)dp) + lp -> lp_off)))));
    heapTuple -> t_len = (lp -> lp_len);
    heapTuple -> t_tableOid = relation -> rd_id;
    heapTuple -> t_self =  *tid;
/*
		 * Shouldn't see a HEAP_ONLY tuple at chain start.
		 */
    if (at_chain_start && (heapTuple -> t_data -> t_infomask2) & 0x8000) {
      break; 
    }
/*
		 * The xmin should match the previous xmax value, else chain is
		 * broken.
		 */
    if (prev_xmax != ((TransactionId )0) && !(prev_xmax == heapTuple -> t_data -> t_choice . t_heap . t_xmin)) {
      break; 
    }
/*
		 * When first_call is true (and thus, skip is initially false) we'll
		 * return the first tuple we find.	But on later passes, heapTuple
		 * will initially be pointing to the tuple we returned last time.
		 * Returning it again would be incorrect (and would loop forever), so
		 * we skip it and return the next match we find.
		 */
    if (!skip) {
/* If it's visible per the snapshot, we must return it */
      valid = ( *snapshot -> satisfies)(heapTuple -> t_data,snapshot,buffer);
      CheckForSerializableConflictOut(valid,relation,heapTuple,buffer,snapshot);
      if (valid) {
        (((void )((bool )1)) , tid -> ip_posid = offnum);
        PredicateLockTuple(relation,heapTuple,snapshot);
        if (all_dead) {
           *all_dead = ((bool )0);
        }
        return (bool )1;
      }
    }
    skip = ((bool )0);
/*
		 * If we can't see it, maybe no one else can either.  At caller
		 * request, check whether all chain members are dead to all
		 * transactions.
		 */
    if (all_dead &&  *all_dead && !HeapTupleIsSurelyDead(heapTuple -> t_data,RecentGlobalXmin)) {
       *all_dead = ((bool )0);
    }
/*
		 * Check to see if HOT chain continues past this tuple; if so fetch
		 * the next offnum and loop around.
		 */
    if (((heapTuple -> t_data -> t_infomask2) & 0x4000) != 0 && ((heapTuple -> t_data -> t_infomask) & (0x0200 | 0x0800)) == 0) {
      ;
      offnum = ((((void )((bool )1)) , (&heapTuple -> t_data -> t_ctid) -> ip_posid));
      at_chain_start = ((bool )0);
      prev_xmax = heapTuple -> t_data -> t_choice . t_heap . t_xmax;
    }
    else {
/* end of chain */
      break; 
    }
  }
  return (bool )0;
}
/*
 *	heap_hot_search		- search HOT chain for tuple satisfying snapshot
 *
 * This has the same API as heap_hot_search_buffer, except that the caller
 * does not provide the buffer containing the page, rather we access it
 * locally.
 */

bool heap_hot_search(ItemPointer tid,Relation relation,Snapshot snapshot,bool *all_dead)
{
  bool result;
  Buffer buffer;
  HeapTupleData heapTuple;
  buffer = ReadBuffer(relation,((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&tid -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&tid -> ip_blkid) -> bi_lo)))))))));
  LockBuffer(buffer,1);
  result = heap_hot_search_buffer(tid,relation,buffer,snapshot,&heapTuple,all_dead,((bool )1));
  LockBuffer(buffer,0);
  ReleaseBuffer(buffer);
  return result;
}
/*
 *	heap_get_latest_tid -  get the latest tid of a specified tuple
 *
 * Actually, this gets the latest version that is visible according to
 * the passed snapshot.  You can pass SnapshotDirty to get the very latest,
 * possibly uncommitted version.
 *
 * *tid is both an input and an output parameter: it is updated to
 * show the latest version of the row.	Note that it will not be changed
 * if no version of the row passes the snapshot test.
 */

void heap_get_latest_tid(Relation relation,Snapshot snapshot,ItemPointer tid)
{
  BlockNumber blk;
  ItemPointerData ctid;
  TransactionId priorXmax;
/* this is to avoid Assert failures on bad input */
  if (!((bool )(((const void *)tid) != ((void *)0) && (tid -> ip_posid) != 0))) {
    return ;
  }
/*
	 * Since this can be called with user-supplied TID, don't trust the input
	 * too much.  (RelationGetNumberOfBlocks is an expensive check, so we
	 * don't check t_ctid links again this way.  Note that it would not do to
	 * call it just once and save the result, either.)
	 */
  blk = ((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&tid -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&tid -> ip_blkid) -> bi_lo))))))));
  if (blk >= RelationGetNumberOfBlocksInFork(relation,MAIN_FORKNUM)) {
    (elog_start("heapam.c",1718,__func__) , elog_finish(20,"block number %u is out of range for relation \"%s\"",blk,relation -> rd_rel -> relname . data));
  }
/*
	 * Loop to chase down t_ctid links.  At top of loop, ctid is the tuple we
	 * need to examine, and *tid is the TID we will return if ctid turns out
	 * to be bogus.
	 *
	 * Note that we will loop until we reach the end of the t_ctid chain.
	 * Depending on the snapshot passed, there might be at most one visible
	 * version of the row, but we don't try to optimize for that.
	 */
  ctid =  *tid;
/* cannot check first XMIN */
  priorXmax = ((TransactionId )0);
  for (; ; ) {
    Buffer buffer;
    Page page;
    OffsetNumber offnum;
    ItemId lp;
    HeapTupleData tp;
    bool valid;
/*
		 * Read, pin, and lock the page.
		 */
    buffer = ReadBuffer(relation,((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&(&ctid) -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&(&ctid) -> ip_blkid) -> bi_lo)))))))));
    LockBuffer(buffer,1);
    page = ((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192))))));
/*
		 * Check for bogus item number.  This is not treated as an error
		 * condition because it can happen while following a t_ctid link. We
		 * just assume that the prior tid is OK and return it unchanged.
		 */
    offnum = ((((void )((bool )1)) , (&ctid) -> ip_posid));
    if (offnum < ((OffsetNumber )1) || offnum > (((((PageHeader )page) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )page) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData )))) {
      UnlockReleaseBuffer(buffer);
      break; 
    }
    lp = ((ItemId )(&((PageHeader )page) -> pd_linp[offnum - 1]));
    if (!((lp -> lp_flags) == 1)) {
      UnlockReleaseBuffer(buffer);
      break; 
    }
/* OK to access the tuple */
    tp . t_self = ctid;
    tp . t_data = ((HeapTupleHeader )(((((void )((bool )1)) , ((void )((bool )1))) , ((Item )(((char *)page) + lp -> lp_off)))));
    tp . t_len = (lp -> lp_len);
/*
		 * After following a t_ctid link, we might arrive at an unrelated
		 * tuple.  Check for XMIN match.
		 */
    if (priorXmax != ((TransactionId )0) && !(priorXmax == tp . t_data -> t_choice . t_heap . t_xmin)) {
      UnlockReleaseBuffer(buffer);
      break; 
    }
/*
		 * Check time qualification of tuple; if visible, set it as the new
		 * result candidate.
		 */
    valid = ( *snapshot -> satisfies)((&tp) -> t_data,snapshot,buffer);
    CheckForSerializableConflictOut(valid,relation,&tp,buffer,snapshot);
    if (valid) {
       *tid = ctid;
    }
/*
		 * If there's a valid t_ctid link, follow it, else we're done.
		 */
    if ((tp . t_data -> t_infomask) & (0x0800 | (0x0040 | 0x0080)) || ItemPointerEquals(&tp . t_self,&tp . t_data -> t_ctid)) {
      UnlockReleaseBuffer(buffer);
      break; 
    }
    ctid = tp . t_data -> t_ctid;
    priorXmax = tp . t_data -> t_choice . t_heap . t_xmax;
    UnlockReleaseBuffer(buffer);
/* end of loop */
  }
}
/*
 * UpdateXmaxHintBits - update tuple hint bits after xmax transaction ends
 *
 * This is called after we have waited for the XMAX transaction to terminate.
 * If the transaction aborted, we guarantee the XMAX_INVALID hint bit will
 * be set on exit.	If the transaction committed, we set the XMAX_COMMITTED
 * hint bit if possible --- but beware that that may not yet be possible,
 * if the transaction committed asynchronously.  Hence callers should look
 * only at XMAX_INVALID.
 */

static void UpdateXmaxHintBits(HeapTupleHeader tuple,Buffer buffer,TransactionId xid)
{
  ;
  if (!((tuple -> t_infomask) & (0x0400 | 0x0800))) {
    if (TransactionIdDidCommit(xid)) {
      HeapTupleSetHintBits(tuple,buffer,0x0400,xid);
    }
    else {
      HeapTupleSetHintBits(tuple,buffer,0x0800,((TransactionId )0));
    }
  }
}
/*
 * GetBulkInsertState - prepare status object for a bulk insert
 */

BulkInsertState GetBulkInsertState()
{
  BulkInsertState bistate;
  bistate = ((BulkInsertState )(MemoryContextAlloc(CurrentMemoryContext,sizeof(BulkInsertStateData ))));
  bistate -> strategy = GetAccessStrategy(BAS_BULKWRITE);
  bistate -> current_buf = 0;
  return bistate;
}
/*
 * FreeBulkInsertState - clean up after finishing a bulk insert
 */

void FreeBulkInsertState(BulkInsertState bistate)
{
  if (bistate -> current_buf != 0) {
    ReleaseBuffer(bistate -> current_buf);
  }
  FreeAccessStrategy(bistate -> strategy);
  pfree(bistate);
}
/*
 *	heap_insert		- insert tuple into a heap
 *
 * The new tuple is stamped with current transaction ID and the specified
 * command ID.
 *
 * If the HEAP_INSERT_SKIP_WAL option is specified, the new tuple is not
 * logged in WAL, even for a non-temp relation.  Safe usage of this behavior
 * requires that we arrange that all new tuples go into new pages not
 * containing any tuples from other transactions, and that the relation gets
 * fsync'd before commit.  (See also heap_sync() comments)
 *
 * The HEAP_INSERT_SKIP_FSM option is passed directly to
 * RelationGetBufferForTuple, which see for more info.
 *
 * Note that these options will be applied when inserting into the heap's
 * TOAST table, too, if the tuple requires any out-of-line data.
 *
 * The BulkInsertState object (if any; bistate can be NULL for default
 * behavior) is also just passed through to RelationGetBufferForTuple.
 *
 * The return value is the OID assigned to the tuple (either here or by the
 * caller), or InvalidOid if no OID.  The header fields of *tup are updated
 * to match the stored tuple; in particular tup->t_self receives the actual
 * TID where the tuple was stored.	But note that any toasting of fields
 * within the tuple data is NOT reflected into *tup.
 */

Oid heap_insert(Relation relation,HeapTuple tup,CommandId cid,int options,BulkInsertState bistate)
{
  TransactionId xid = GetCurrentTransactionId();
  HeapTuple heaptup;
  Buffer buffer;
  Buffer vmbuffer = 0;
  bool all_visible_cleared = (bool )0;
/*
	 * Fill in tuple header fields, assign an OID, and toast the tuple if
	 * necessary.
	 *
	 * Note: below this point, heaptup is the data we actually intend to store
	 * into the relation; tup is the caller's original untoasted data.
	 */
  heaptup = heap_prepare_insert(relation,tup,xid,cid,options);
/*
	 * We're about to do the actual insert -- but check for conflict first, to
	 * avoid possibly having to roll back work we've just done.
	 *
	 * For a heap insert, we only need to check for table-level SSI locks. Our
	 * new tuple can't possibly conflict with existing tuple locks, and heap
	 * page locks are only consolidated versions of tuple locks; they do not
	 * lock "gaps" as index page locks do.	So we don't need to identify a
	 * buffer before making the call.
	 */
  CheckForSerializableConflictIn(relation,((void *)0),0);
/*
	 * Find buffer to insert this tuple into.  If the page is all visible,
	 * this will also pin the requisite visibility map page.
	 */
  buffer = RelationGetBufferForTuple(relation,(heaptup -> t_len),0,options,bistate,&vmbuffer,((void *)0));
/* NO EREPORT(ERROR) from here till changes are logged */
  CritSectionCount++;
  RelationPutHeapTuple(relation,buffer,heaptup);
  if ((((PageHeader )((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192))))))) -> pd_flags) & 0x0004) {
    all_visible_cleared = ((bool )1);
    ((PageHeader )((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192))))))) -> pd_flags &= ~0x0004;
    visibilitymap_clear(relation,((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&(&heaptup -> t_self) -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&(&heaptup -> t_self) -> ip_blkid) -> bi_lo)))))))),vmbuffer);
  }
/*
	 * XXX Should we set PageSetPrunable on this page ?
	 *
	 * The inserting transaction may eventually abort thus making this tuple
	 * DEAD and hence available for pruning. Though we don't want to optimize
	 * for aborts, if no other tuple in this page is UPDATEd/DELETEd, the
	 * aborted tuple will never be pruned until next vacuum is triggered.
	 *
	 * If you do add PageSetPrunable here, add it in heap_xlog_insert too.
	 */
  MarkBufferDirty(buffer);
/* XLOG stuff */
  if (!(options & 0x0001) && (relation -> rd_rel -> relpersistence) == 'p') {
    xl_heap_insert xlrec;
    xl_heap_header xlhdr;
    XLogRecPtr recptr;
    XLogRecData rdata[3];
    Page page = (Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192)))));
    uint8 info = 0;
    xlrec . all_visible_cleared = all_visible_cleared;
    xlrec . target . node = relation -> rd_node;
    xlrec . target . tid = heaptup -> t_self;
    rdata[0] . data = ((char *)(&xlrec));
    rdata[0] . len = (((size_t )(&((xl_heap_insert *)0) -> all_visible_cleared)) + sizeof(bool ));
    rdata[0] . buffer = 0;
    rdata[0] . next = &rdata[1];
    xlhdr . t_infomask2 = heaptup -> t_data -> t_infomask2;
    xlhdr . t_infomask = heaptup -> t_data -> t_infomask;
    xlhdr . t_hoff = heaptup -> t_data -> t_hoff;
/*
		 * note we mark rdata[1] as belonging to buffer; if XLogInsert decides
		 * to write the whole page to the xlog, we don't need to store
		 * xl_heap_header in the xlog.
		 */
    rdata[1] . data = ((char *)(&xlhdr));
    rdata[1] . len = (((size_t )(&((xl_heap_header *)0) -> t_hoff)) + sizeof(uint8 ));
    rdata[1] . buffer = buffer;
    rdata[1] . buffer_std = ((bool )1);
    rdata[1] . next = &rdata[2];
/* PG73FORMAT: write bitmap [+ padding] [+ oid] + data */
    rdata[2] . data = ((char *)(heaptup -> t_data)) + ((size_t )(&((HeapTupleHeaderData *)0) -> t_bits));
    rdata[2] . len = ((heaptup -> t_len) - ((size_t )(&((HeapTupleHeaderData *)0) -> t_bits)));
    rdata[2] . buffer = buffer;
    rdata[2] . buffer_std = ((bool )1);
    rdata[2] . next = ((void *)0);
/*
		 * If this is the single and first tuple on page, we can reinit the
		 * page instead of restoring the whole thing.  Set flag, and hide
		 * buffer references from XLogInsert.
		 */
    if (((((void )((bool )1)) , (&heaptup -> t_self) -> ip_posid)) == ((OffsetNumber )1) && (((((PageHeader )page) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )page) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData ))) == ((OffsetNumber )1)) {
      info |= 0x0080;
      rdata[1] . buffer = rdata[2] . buffer = 0;
    }
    recptr = XLogInsert(10,info,rdata);
    ((PageHeader )page) -> pd_lsn = recptr;
    ((PageHeader )page) -> pd_tli = ((uint16 )ThisTimeLineID);
  }
  do {
    ;
    CritSectionCount--;
  }while (0);
  UnlockReleaseBuffer(buffer);
  if (vmbuffer != 0) {
    ReleaseBuffer(vmbuffer);
  }
/*
	 * If tuple is cachable, mark it for invalidation from the caches in case
	 * we abort.  Note it is OK to do this after releasing the buffer, because
	 * the heaptup data structure is all in local memory, not in the shared
	 * buffer.
	 */
  CacheInvalidateHeapTuple(relation,heaptup,((void *)0));
  pgstat_count_heap_insert(relation,1);
/*
	 * If heaptup is a private copy, release it.  Don't forget to copy t_self
	 * back to the caller's image, too.
	 */
  if (heaptup != tup) {
    tup -> t_self = heaptup -> t_self;
    heap_freetuple(heaptup);
  }
  return (tup -> t_data -> t_infomask) & 0x0008? *((Oid *)(((char *)(tup -> t_data)) + tup -> t_data -> t_hoff - sizeof(Oid ))) : ((Oid )0);
}
/*
 * Subroutine for heap_insert(). Prepares a tuple for insertion. This sets the
 * tuple header fields, assigns an OID, and toasts the tuple if necessary.
 * Returns a toasted version of the tuple if it was toasted, or the original
 * tuple if not. Note that in any case, the header fields are also set in
 * the original tuple.
 */

static HeapTuple heap_prepare_insert(Relation relation,HeapTuple tup,TransactionId xid,CommandId cid,int options)
{
  if (relation -> rd_rel -> relhasoids) {
#ifdef NOT_USED
/* this is redundant with an Assert in HeapTupleSetOid */
#endif
/*
		 * If the object id of this tuple has already been assigned, trust the
		 * caller.	There are a couple of ways this can happen.  At initial db
		 * creation, the backend program sets oids for tuples. When we define
		 * an index, we set the oid.  Finally, in the future, we may allow
		 * users to set their own object ids in order to support a persistent
		 * object store (objects need to contain pointers to one another).
		 */
    if (!((bool )((((tup -> t_data -> t_infomask) & 0x0008? *((Oid *)(((char *)(tup -> t_data)) + tup -> t_data -> t_hoff - sizeof(Oid ))) : ((Oid )0))) != ((Oid )0)))) {
      do {
        ;
         *((Oid *)(((char *)(tup -> t_data)) + tup -> t_data -> t_hoff - sizeof(Oid ))) = GetNewOid(relation);
      }while (0);
    }
  }
  else {
/* check there is not space for an OID */
    ;
  }
  tup -> t_data -> t_infomask &= ~0xFFE0;
  tup -> t_data -> t_infomask2 &= ~0xC000;
  tup -> t_data -> t_infomask |= 0x0800;
  tup -> t_data -> t_choice . t_heap . t_xmin = xid;
  do {
    ;
    tup -> t_data -> t_choice . t_heap . t_field3 . t_cid = cid;
    tup -> t_data -> t_infomask &= ~0x0020;
  }while (0);
/* for cleanliness */
  tup -> t_data -> t_choice . t_heap . t_xmax = 0;
  tup -> t_tableOid = relation -> rd_id;
/*
	 * If the new tuple is too big for storage or contains already toasted
	 * out-of-line attributes from some other relation, invoke the toaster.
	 */
  if ((relation -> rd_rel -> relkind) != 'r') {
/* toast table entries should never be recursively toasted */
    ;
    return tup;
  }
  else {
    if (((tup -> t_data -> t_infomask) & 0x0004) != 0 || (tup -> t_len) > (((intptr_t )((8192 - (((intptr_t )(((size_t )(&((PageHeaderData *)0) -> pd_linp)) + 4 * sizeof(ItemIdData ))) + (8 - 1) & ~((intptr_t )(8 - 1)))) / 4)) & ~((intptr_t )(8 - 1)))) {
      return toast_insert_or_update(relation,tup,((void *)0),options);
    }
    else {
      return tup;
    }
  }
}
/*
 *	heap_multi_insert	- insert multiple tuple into a heap
 *
 * This is like heap_insert(), but inserts multiple tuples in one operation.
 * That's faster than calling heap_insert() in a loop, because when multiple
 * tuples can be inserted on a single page, we can write just a single WAL
 * record covering all of them, and only need to lock/unlock the page once.
 *
 * Note: this leaks memory into the current memory context. You can create a
 * temporary context before calling this, if that's a problem.
 */

void heap_multi_insert(Relation relation,HeapTuple *tuples,int ntuples,CommandId cid,int options,BulkInsertState bistate)
{
  TransactionId xid = GetCurrentTransactionId();
  HeapTuple *heaptuples;
  int i;
  int ndone;
  char *scratch = ((void *)0);
  Page page;
  bool needwal;
  Size saveFreeSpace;
  needwal = (!(options & 0x0001) && (relation -> rd_rel -> relpersistence) == 'p');
  saveFreeSpace = (8192 * (100 - ((relation -> rd_options?((StdRdOptions *)(relation -> rd_options)) -> fillfactor : 100))) / 100);
/* Toast and set header data in all the tuples */
  heaptuples = (MemoryContextAlloc(CurrentMemoryContext,ntuples * sizeof(HeapTuple )));
  for (i = 0; i < ntuples; i++) 
    heaptuples[i] = heap_prepare_insert(relation,tuples[i],xid,cid,options);
/*
	 * Allocate some memory to use for constructing the WAL record. Using
	 * palloc() within a critical section is not safe, so we allocate this
	 * beforehand.
	 */
  if (needwal) {
    scratch = (MemoryContextAlloc(CurrentMemoryContext,8192));
  }
/*
	 * We're about to do the actual inserts -- but check for conflict first,
	 * to avoid possibly having to roll back work we've just done.
	 *
	 * For a heap insert, we only need to check for table-level SSI locks. Our
	 * new tuple can't possibly conflict with existing tuple locks, and heap
	 * page locks are only consolidated versions of tuple locks; they do not
	 * lock "gaps" as index page locks do.	So we don't need to identify a
	 * buffer before making the call.
	 */
  CheckForSerializableConflictIn(relation,((void *)0),0);
  ndone = 0;
  while(ndone < ntuples){
    Buffer buffer;
    Buffer vmbuffer = 0;
    bool all_visible_cleared = (bool )0;
    int nthispage;
/*
		 * Find buffer where at least the next tuple will fit.	If the page is
		 * all-visible, this will also pin the requisite visibility map page.
		 */
    buffer = RelationGetBufferForTuple(relation,(heaptuples[ndone] -> t_len),0,options,bistate,&vmbuffer,((void *)0));
    page = ((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192))))));
/* NO EREPORT(ERROR) from here till changes are logged */
    CritSectionCount++;
/*
		 * RelationGetBufferForTuple has ensured that the first tuple fits.
		 * Put that on the page, and then as many other tuples as fit.
		 */
    RelationPutHeapTuple(relation,buffer,heaptuples[ndone]);
    for (nthispage = 1; ndone + nthispage < ntuples; nthispage++) {
      HeapTuple heaptup = heaptuples[ndone + nthispage];
      if (PageGetHeapFreeSpace(page) < (((intptr_t )(heaptup -> t_len)) + (8 - 1) & ~((intptr_t )(8 - 1))) + saveFreeSpace) {
        break; 
      }
      RelationPutHeapTuple(relation,buffer,heaptup);
    }
    if ((((PageHeader )page) -> pd_flags) & 0x0004) {
      all_visible_cleared = ((bool )1);
      ((PageHeader )page) -> pd_flags &= ~0x0004;
      visibilitymap_clear(relation,BufferGetBlockNumber(buffer),vmbuffer);
    }
/*
		 * XXX Should we set PageSetPrunable on this page ? See heap_insert()
		 */
    MarkBufferDirty(buffer);
/* XLOG stuff */
    if (needwal) {
      XLogRecPtr recptr;
      xl_heap_multi_insert *xlrec;
      XLogRecData rdata[2];
      uint8 info = 0x50;
      char *tupledata;
      int totaldatalen;
      char *scratchptr = scratch;
      bool init;
/*
			 * If the page was previously empty, we can reinit the page
			 * instead of restoring the whole thing.
			 */
      init = (((((void )((bool )1)) , (&heaptuples[ndone] -> t_self) -> ip_posid)) == ((OffsetNumber )1) && (((((PageHeader )page) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )page) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData ))) == (((OffsetNumber )1) + nthispage - 1));
/* allocate xl_heap_multi_insert struct from the scratch area */
      xlrec = ((xl_heap_multi_insert *)scratchptr);
      scratchptr += ((size_t )(&((xl_heap_multi_insert *)0) -> offsets));
/*
			 * Allocate offsets array. Unless we're reinitializing the page,
			 * in that case the tuples are stored in order starting at
			 * FirstOffsetNumber and we don't need to store the offsets
			 * explicitly.
			 */
      if (!init) {
        scratchptr += nthispage * sizeof(OffsetNumber );
      }
/* the rest of the scratch space is used for tuple data */
      tupledata = scratchptr;
      xlrec -> all_visible_cleared = all_visible_cleared;
      xlrec -> node = relation -> rd_node;
      xlrec -> blkno = BufferGetBlockNumber(buffer);
      xlrec -> ntuples = nthispage;
/*
			 * Write out an xl_multi_insert_tuple and the tuple data itself
			 * for each tuple.
			 */
      for (i = 0; i < nthispage; i++) {
        HeapTuple heaptup = heaptuples[ndone + i];
        xl_multi_insert_tuple *tuphdr;
        int datalen;
        if (!init) {
          xlrec -> offsets[i] = ((((void )((bool )1)) , (&heaptup -> t_self) -> ip_posid));
        }
/* xl_multi_insert_tuple needs two-byte alignment. */
        tuphdr = ((xl_multi_insert_tuple *)(((intptr_t )scratchptr) + (2 - 1) & ~((intptr_t )(2 - 1))));
        scratchptr = ((char *)tuphdr) + (((size_t )(&((xl_multi_insert_tuple *)0) -> t_hoff)) + sizeof(uint8 ));
        tuphdr -> t_infomask2 = heaptup -> t_data -> t_infomask2;
        tuphdr -> t_infomask = heaptup -> t_data -> t_infomask;
        tuphdr -> t_hoff = heaptup -> t_data -> t_hoff;
/* write bitmap [+ padding] [+ oid] + data */
        datalen = ((heaptup -> t_len) - ((size_t )(&((HeapTupleHeaderData *)0) -> t_bits)));
        memcpy(scratchptr,(((char *)(heaptup -> t_data)) + ((size_t )(&((HeapTupleHeaderData *)0) -> t_bits))),datalen);
        tuphdr -> datalen = datalen;
        scratchptr += datalen;
      }
      totaldatalen = (scratchptr - tupledata);
      ;
      rdata[0] . data = ((char *)xlrec);
      rdata[0] . len = (tupledata - scratch);
      rdata[0] . buffer = 0;
      rdata[0] . next = &rdata[1];
      rdata[1] . data = tupledata;
      rdata[1] . len = totaldatalen;
      rdata[1] . buffer = buffer;
      rdata[1] . buffer_std = ((bool )1);
      rdata[1] . next = ((void *)0);
/*
			 * If we're going to reinitialize the whole page using the WAL
			 * record, hide buffer reference from XLogInsert.
			 */
      if (init) {
        rdata[1] . buffer = 0;
        info |= 0x0080;
      }
      recptr = XLogInsert(9,info,rdata);
      ((PageHeader )page) -> pd_lsn = recptr;
      ((PageHeader )page) -> pd_tli = ((uint16 )ThisTimeLineID);
    }
    do {
      ;
      CritSectionCount--;
    }while (0);
    UnlockReleaseBuffer(buffer);
    if (vmbuffer != 0) {
      ReleaseBuffer(vmbuffer);
    }
    ndone += nthispage;
  }
/*
	 * If tuples are cachable, mark them for invalidation from the caches in
	 * case we abort.  Note it is OK to do this after releasing the buffer,
	 * because the heaptuples data structure is all in local memory, not in
	 * the shared buffer.
	 */
  if (IsSystemRelation(relation)) {
    for (i = 0; i < ntuples; i++) 
      CacheInvalidateHeapTuple(relation,heaptuples[i],((void *)0));
  }
/*
	 * Copy t_self fields back to the caller's original tuples. This does
	 * nothing for untoasted tuples (tuples[i] == heaptuples[i)], but it's
	 * probably faster to always copy than check.
	 */
  for (i = 0; i < ntuples; i++) 
    tuples[i] -> t_self = heaptuples[i] -> t_self;
  pgstat_count_heap_insert(relation,ntuples);
}
/*
 *	simple_heap_insert - insert a tuple
 *
 * Currently, this routine differs from heap_insert only in supplying
 * a default command ID and not allowing access to the speedup options.
 *
 * This should be used rather than using heap_insert directly in most places
 * where we are modifying system catalogs.
 */

Oid simple_heap_insert(Relation relation,HeapTuple tup)
{
  return heap_insert(relation,tup,GetCurrentCommandId(((bool )1)),0,((void *)0));
}
/*
 *	heap_delete - delete a tuple
 *
 * NB: do not call this directly unless you are prepared to deal with
 * concurrent-update conditions.  Use simple_heap_delete instead.
 *
 *	relation - table to be modified (caller must hold suitable lock)
 *	tid - TID of tuple to be deleted
 *	ctid - output parameter, used only for failure case (see below)
 *	update_xmax - output parameter, used only for failure case (see below)
 *	cid - delete command ID (used for visibility test, and stored into
 *		cmax if successful)
 *	crosscheck - if not InvalidSnapshot, also check tuple against this
 *	wait - true if should wait for any conflicting update to commit/abort
 *
 * Normal, successful return value is HeapTupleMayBeUpdated, which
 * actually means we did delete it.  Failure return codes are
 * HeapTupleSelfUpdated, HeapTupleUpdated, or HeapTupleBeingUpdated
 * (the last only possible if wait == false).
 *
 * In the failure cases, the routine returns the tuple's t_ctid and t_xmax.
 * If t_ctid is the same as tid, the tuple was deleted; if different, the
 * tuple was updated, and t_ctid is the location of the replacement tuple.
 * (t_xmax is needed to verify that the replacement tuple matches.)
 */

HTSU_Result heap_delete(Relation relation,ItemPointer tid,ItemPointer ctid,TransactionId *update_xmax,CommandId cid,Snapshot crosscheck,bool wait)
{
  HTSU_Result result;
  TransactionId xid = GetCurrentTransactionId();
  ItemId lp;
  HeapTupleData tp;
  Page page;
  BlockNumber block;
  Buffer buffer;
  Buffer vmbuffer = 0;
  bool have_tuple_lock = (bool )0;
  bool iscombo;
  bool all_visible_cleared = (bool )0;
  ;
  block = ((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&tid -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&tid -> ip_blkid) -> bi_lo))))))));
  buffer = ReadBuffer(relation,block);
  page = ((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192))))));
/*
	 * Before locking the buffer, pin the visibility map page if it appears to
	 * be necessary.  Since we haven't got the lock yet, someone else might be
	 * in the middle of changing this, so we'll need to recheck after we have
	 * the lock.
	 */
  if ((((PageHeader )page) -> pd_flags) & 0x0004) {
    visibilitymap_pin(relation,block,&vmbuffer);
  }
  LockBuffer(buffer,2);
/*
	 * If we didn't pin the visibility map page and the page has become all
	 * visible while we were busy locking the buffer, we'll have to unlock and
	 * re-lock, to avoid holding the buffer lock across an I/O.  That's a bit
	 * unfortunate, but hopefully shouldn't happen often.
	 */
  if (vmbuffer == 0 && (((PageHeader )page) -> pd_flags) & 0x0004) {
    LockBuffer(buffer,0);
    visibilitymap_pin(relation,block,&vmbuffer);
    LockBuffer(buffer,2);
  }
  lp = ((ItemId )(&((PageHeader )page) -> pd_linp[((((void )((bool )1)) , tid -> ip_posid)) - 1]));
  ;
  tp . t_data = ((HeapTupleHeader )(((((void )((bool )1)) , ((void )((bool )1))) , ((Item )(((char *)page) + lp -> lp_off)))));
  tp . t_len = (lp -> lp_len);
  tp . t_self =  *tid;
  l1:
  result = HeapTupleSatisfiesUpdate(tp . t_data,cid,buffer);
  if (result == HeapTupleInvisible) {
    UnlockReleaseBuffer(buffer);
    (elog_start("heapam.c",2435,__func__) , elog_finish(20,"attempted to delete invisible tuple"));
  }
  else {
    if (result == HeapTupleBeingUpdated && wait) {
      TransactionId xwait;
      uint16 infomask;
/* must copy state data before unlocking buffer */
      xwait = tp . t_data -> t_choice . t_heap . t_xmax;
      infomask = tp . t_data -> t_infomask;
      LockBuffer(buffer,0);
/*
		 * Acquire tuple lock to establish our priority for the tuple (see
		 * heap_lock_tuple).  LockTuple will release us when we are
		 * next-in-line for the tuple.
		 *
		 * If we are forced to "start over" below, we keep the tuple lock;
		 * this arranges that we stay at the head of the line while rechecking
		 * tuple state.
		 */
      if (!have_tuple_lock) {
        LockTuple(relation,&tp . t_self,7);
        have_tuple_lock = ((bool )1);
      }
/*
		 * Sleep until concurrent transaction ends.  Note that we don't care
		 * if the locker has an exclusive or shared lock, because we need
		 * exclusive.
		 */
      if (infomask & 0x1000) {
/* wait for multixact */
        MultiXactIdWait(((MultiXactId )xwait));
        LockBuffer(buffer,2);
/*
			 * If xwait had just locked the tuple then some other xact could
			 * update this tuple before we get to this point.  Check for xmax
			 * change, and start over if so.
			 */
        if (!((tp . t_data -> t_infomask) & 0x1000) || !(tp . t_data -> t_choice . t_heap . t_xmax == xwait)) {
          goto l1;
        }
/*
			 * You might think the multixact is necessarily done here, but not
			 * so: it could have surviving members, namely our own xact or
			 * other subxacts of this backend.	It is legal for us to delete
			 * the tuple in either case, however (the latter case is
			 * essentially a situation of upgrading our former shared lock to
			 * exclusive).	We don't bother changing the on-disk hint bits
			 * since we are about to overwrite the xmax altogether.
			 */
      }
      else {
/* wait for regular transaction to end */
        XactLockTableWait(xwait);
        LockBuffer(buffer,2);
/*
			 * xwait is done, but if xwait had just locked the tuple then some
			 * other xact could update this tuple before we get to this point.
			 * Check for xmax change, and start over if so.
			 */
        if ((tp . t_data -> t_infomask) & 0x1000 || !(tp . t_data -> t_choice . t_heap . t_xmax == xwait)) {
          goto l1;
        }
/* Otherwise check if it committed or aborted */
        UpdateXmaxHintBits(tp . t_data,buffer,xwait);
      }
/*
		 * We may overwrite if previous xmax aborted, or if it committed but
		 * only locked the tuple without updating it.
		 */
      if ((tp . t_data -> t_infomask) & (0x0800 | (0x0040 | 0x0080))) {
        result = HeapTupleMayBeUpdated;
      }
      else {
        result = HeapTupleUpdated;
      }
    }
  }
  if (crosscheck != ((Snapshot )((void *)0)) && result == HeapTupleMayBeUpdated) {
/* Perform additional check for transaction-snapshot mode RI updates */
    if (!( *crosscheck -> satisfies)((&tp) -> t_data,crosscheck,buffer)) {
      result = HeapTupleUpdated;
    }
  }
  if (result != HeapTupleMayBeUpdated) {
    ;
    ;
     *ctid = tp . t_data -> t_ctid;
     *update_xmax = tp . t_data -> t_choice . t_heap . t_xmax;
    UnlockReleaseBuffer(buffer);
    if (have_tuple_lock) {
      UnlockTuple(relation,&tp . t_self,7);
    }
    if (vmbuffer != 0) {
      ReleaseBuffer(vmbuffer);
    }
    return result;
  }
/*
	 * We're about to do the actual delete -- check for conflict first, to
	 * avoid possibly having to roll back work we've just done.
	 */
  CheckForSerializableConflictIn(relation,&tp,buffer);
/* replace cid with a combo cid if necessary */
  HeapTupleHeaderAdjustCmax(tp . t_data,&cid,&iscombo);
  CritSectionCount++;
/*
	 * If this transaction commits, the tuple will become DEAD sooner or
	 * later.  Set flag that this page is a candidate for pruning once our xid
	 * falls below the OldestXmin horizon.	If the transaction finally aborts,
	 * the subsequent page pruning will be a no-op and the hint will be
	 * cleared.
	 */
  do {
    ;
    if (!(((PageHeader )page) -> pd_prune_xid != ((TransactionId )0)) || TransactionIdPrecedes(xid,((PageHeader )page) -> pd_prune_xid)) {
      ((PageHeader )page) -> pd_prune_xid = xid;
    }
  }while (0);
  if ((((PageHeader )page) -> pd_flags) & 0x0004) {
    all_visible_cleared = ((bool )1);
    ((PageHeader )page) -> pd_flags &= ~0x0004;
    visibilitymap_clear(relation,BufferGetBlockNumber(buffer),vmbuffer);
  }
/* store transaction information of xact deleting the tuple */
  tp . t_data -> t_infomask &= ~(0x0400 | 0x0800 | 0x1000 | (0x0040 | 0x0080) | (0x4000 | 0x8000));
  tp . t_data -> t_infomask2 &= ~0x4000;
  tp . t_data -> t_choice . t_heap . t_xmax = xid;
  do {
    ;
    tp . t_data -> t_choice . t_heap . t_field3 . t_cid = cid;
    if (iscombo) {
      tp . t_data -> t_infomask |= 0x0020;
    }
    else {
      tp . t_data -> t_infomask &= ~0x0020;
    }
  }while (0);
/* Make sure there is no forward chain link in t_ctid */
  tp . t_data -> t_ctid = tp . t_self;
  MarkBufferDirty(buffer);
/* XLOG stuff */
  if ((relation -> rd_rel -> relpersistence) == 'p') {
    xl_heap_delete xlrec;
    XLogRecPtr recptr;
    XLogRecData rdata[2];
    xlrec . all_visible_cleared = all_visible_cleared;
    xlrec . target . node = relation -> rd_node;
    xlrec . target . tid = tp . t_self;
    rdata[0] . data = ((char *)(&xlrec));
    rdata[0] . len = (((size_t )(&((xl_heap_delete *)0) -> all_visible_cleared)) + sizeof(bool ));
    rdata[0] . buffer = 0;
    rdata[0] . next = &rdata[1];
    rdata[1] . data = ((void *)0);
    rdata[1] . len = 0;
    rdata[1] . buffer = buffer;
    rdata[1] . buffer_std = ((bool )1);
    rdata[1] . next = ((void *)0);
    recptr = XLogInsert(10,0x10,rdata);
    ((PageHeader )page) -> pd_lsn = recptr;
    ((PageHeader )page) -> pd_tli = ((uint16 )ThisTimeLineID);
  }
  do {
    ;
    CritSectionCount--;
  }while (0);
  LockBuffer(buffer,0);
  if (vmbuffer != 0) {
    ReleaseBuffer(vmbuffer);
  }
/*
	 * If the tuple has toasted out-of-line attributes, we need to delete
	 * those items too.  We have to do this before releasing the buffer
	 * because we need to look at the contents of the tuple, but it's OK to
	 * release the content lock on the buffer first.
	 */
  if ((relation -> rd_rel -> relkind) != 'r') {
/* toast table entries should never be recursively toasted */
    ;
  }
  else {
    if ((((&tp) -> t_data -> t_infomask) & 0x0004) != 0) {
      toast_delete(relation,&tp);
    }
  }
/*
	 * Mark tuple for invalidation from system caches at next command
	 * boundary. We have to do this before releasing the buffer because we
	 * need to look at the contents of the tuple.
	 */
  CacheInvalidateHeapTuple(relation,&tp,((void *)0));
/* Now we can release the buffer */
  ReleaseBuffer(buffer);
/*
	 * Release the lmgr tuple lock, if we had it.
	 */
  if (have_tuple_lock) {
    UnlockTuple(relation,&tp . t_self,7);
  }
  pgstat_count_heap_delete(relation);
  return HeapTupleMayBeUpdated;
}
/*
 *	simple_heap_delete - delete a tuple
 *
 * This routine may be used to delete a tuple when concurrent updates of
 * the target tuple are not expected (for example, because we have a lock
 * on the relation associated with the tuple).	Any failure is reported
 * via ereport().
 */

void simple_heap_delete(Relation relation,ItemPointer tid)
{
  HTSU_Result result;
  ItemPointerData update_ctid;
  TransactionId update_xmax;
  result = heap_delete(relation,tid,&update_ctid,&update_xmax,GetCurrentCommandId(((bool )1)),((Snapshot )((void *)0)),((bool )1));
/* wait for commit */
  switch(result){
    case HeapTupleSelfUpdated:
{
/* Tuple was already updated in current command? */
      (elog_start("heapam.c",2683,__func__) , elog_finish(20,"tuple already updated by self"));
      break; 
    }
    case HeapTupleMayBeUpdated:
/* done successfully */
    break; 
    case HeapTupleUpdated:
{
      (elog_start("heapam.c",2691,__func__) , elog_finish(20,"tuple concurrently updated"));
      break; 
    }
    default:
{
      (elog_start("heapam.c",2695,__func__) , elog_finish(20,"unrecognized heap_delete status: %u",result));
      break; 
    }
  }
}
/*
 *	heap_update - replace a tuple
 *
 * NB: do not call this directly unless you are prepared to deal with
 * concurrent-update conditions.  Use simple_heap_update instead.
 *
 *	relation - table to be modified (caller must hold suitable lock)
 *	otid - TID of old tuple to be replaced
 *	newtup - newly constructed tuple data to store
 *	ctid - output parameter, used only for failure case (see below)
 *	update_xmax - output parameter, used only for failure case (see below)
 *	cid - update command ID (used for visibility test, and stored into
 *		cmax/cmin if successful)
 *	crosscheck - if not InvalidSnapshot, also check old tuple against this
 *	wait - true if should wait for any conflicting update to commit/abort
 *
 * Normal, successful return value is HeapTupleMayBeUpdated, which
 * actually means we *did* update it.  Failure return codes are
 * HeapTupleSelfUpdated, HeapTupleUpdated, or HeapTupleBeingUpdated
 * (the last only possible if wait == false).
 *
 * On success, the header fields of *newtup are updated to match the new
 * stored tuple; in particular, newtup->t_self is set to the TID where the
 * new tuple was inserted, and its HEAP_ONLY_TUPLE flag is set iff a HOT
 * update was done.  However, any TOAST changes in the new tuple's
 * data are not reflected into *newtup.
 *
 * In the failure cases, the routine returns the tuple's t_ctid and t_xmax.
 * If t_ctid is the same as otid, the tuple was deleted; if different, the
 * tuple was updated, and t_ctid is the location of the replacement tuple.
 * (t_xmax is needed to verify that the replacement tuple matches.)
 */

HTSU_Result heap_update(Relation relation,ItemPointer otid,HeapTuple newtup,ItemPointer ctid,TransactionId *update_xmax,CommandId cid,Snapshot crosscheck,bool wait)
{
  HTSU_Result result;
  TransactionId xid = GetCurrentTransactionId();
  Bitmapset *hot_attrs;
  ItemId lp;
  HeapTupleData oldtup;
  HeapTuple heaptup;
  Page page;
  BlockNumber block;
  Buffer buffer;
  Buffer newbuf;
  Buffer vmbuffer = 0;
  Buffer vmbuffer_new = 0;
  bool need_toast;
  bool already_marked;
  Size newtupsize;
  Size pagefree;
  bool have_tuple_lock = (bool )0;
  bool iscombo;
  bool use_hot_update = (bool )0;
  bool all_visible_cleared = (bool )0;
  bool all_visible_cleared_new = (bool )0;
  ;
/*
	 * Fetch the list of attributes to be checked for HOT update.  This is
	 * wasted effort if we fail to update or have to put the new tuple on a
	 * different page.	But we must compute the list before obtaining buffer
	 * lock --- in the worst case, if we are doing an update on one of the
	 * relevant system catalogs, we could deadlock if we try to fetch the list
	 * later.  In any case, the relcache caches the data so this is usually
	 * pretty cheap.
	 *
	 * Note that we get a copy here, so we need not worry about relcache flush
	 * happening midway through.
	 */
  hot_attrs = RelationGetIndexAttrBitmap(relation);
  block = ((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&otid -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&otid -> ip_blkid) -> bi_lo))))))));
  buffer = ReadBuffer(relation,block);
  page = ((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192))))));
/*
	 * Before locking the buffer, pin the visibility map page if it appears to
	 * be necessary.  Since we haven't got the lock yet, someone else might be
	 * in the middle of changing this, so we'll need to recheck after we have
	 * the lock.
	 */
  if ((((PageHeader )page) -> pd_flags) & 0x0004) {
    visibilitymap_pin(relation,block,&vmbuffer);
  }
  LockBuffer(buffer,2);
  lp = ((ItemId )(&((PageHeader )page) -> pd_linp[((((void )((bool )1)) , otid -> ip_posid)) - 1]));
  ;
  oldtup . t_data = ((HeapTupleHeader )(((((void )((bool )1)) , ((void )((bool )1))) , ((Item )(((char *)page) + lp -> lp_off)))));
  oldtup . t_len = (lp -> lp_len);
  oldtup . t_self =  *otid;
/*
	 * Note: beyond this point, use oldtup not otid to refer to old tuple.
	 * otid may very well point at newtup->t_self, which we will overwrite
	 * with the new tuple's location, so there's great risk of confusion if we
	 * use otid anymore.
	 */
  l2:
  result = HeapTupleSatisfiesUpdate(oldtup . t_data,cid,buffer);
  if (result == HeapTupleInvisible) {
    UnlockReleaseBuffer(buffer);
    (elog_start("heapam.c",2810,__func__) , elog_finish(20,"attempted to update invisible tuple"));
  }
  else {
    if (result == HeapTupleBeingUpdated && wait) {
      TransactionId xwait;
      uint16 infomask;
/* must copy state data before unlocking buffer */
      xwait = oldtup . t_data -> t_choice . t_heap . t_xmax;
      infomask = oldtup . t_data -> t_infomask;
      LockBuffer(buffer,0);
/*
		 * Acquire tuple lock to establish our priority for the tuple (see
		 * heap_lock_tuple).  LockTuple will release us when we are
		 * next-in-line for the tuple.
		 *
		 * If we are forced to "start over" below, we keep the tuple lock;
		 * this arranges that we stay at the head of the line while rechecking
		 * tuple state.
		 */
      if (!have_tuple_lock) {
        LockTuple(relation,&oldtup . t_self,7);
        have_tuple_lock = ((bool )1);
      }
/*
		 * Sleep until concurrent transaction ends.  Note that we don't care
		 * if the locker has an exclusive or shared lock, because we need
		 * exclusive.
		 */
      if (infomask & 0x1000) {
/* wait for multixact */
        MultiXactIdWait(((MultiXactId )xwait));
        LockBuffer(buffer,2);
/*
			 * If xwait had just locked the tuple then some other xact could
			 * update this tuple before we get to this point.  Check for xmax
			 * change, and start over if so.
			 */
        if (!((oldtup . t_data -> t_infomask) & 0x1000) || !(oldtup . t_data -> t_choice . t_heap . t_xmax == xwait)) {
          goto l2;
        }
/*
			 * You might think the multixact is necessarily done here, but not
			 * so: it could have surviving members, namely our own xact or
			 * other subxacts of this backend.	It is legal for us to update
			 * the tuple in either case, however (the latter case is
			 * essentially a situation of upgrading our former shared lock to
			 * exclusive).	We don't bother changing the on-disk hint bits
			 * since we are about to overwrite the xmax altogether.
			 */
      }
      else {
/* wait for regular transaction to end */
        XactLockTableWait(xwait);
        LockBuffer(buffer,2);
/*
			 * xwait is done, but if xwait had just locked the tuple then some
			 * other xact could update this tuple before we get to this point.
			 * Check for xmax change, and start over if so.
			 */
        if ((oldtup . t_data -> t_infomask) & 0x1000 || !(oldtup . t_data -> t_choice . t_heap . t_xmax == xwait)) {
          goto l2;
        }
/* Otherwise check if it committed or aborted */
        UpdateXmaxHintBits(oldtup . t_data,buffer,xwait);
      }
/*
		 * We may overwrite if previous xmax aborted, or if it committed but
		 * only locked the tuple without updating it.
		 */
      if ((oldtup . t_data -> t_infomask) & (0x0800 | (0x0040 | 0x0080))) {
        result = HeapTupleMayBeUpdated;
      }
      else {
        result = HeapTupleUpdated;
      }
    }
  }
  if (crosscheck != ((Snapshot )((void *)0)) && result == HeapTupleMayBeUpdated) {
/* Perform additional check for transaction-snapshot mode RI updates */
    if (!( *crosscheck -> satisfies)((&oldtup) -> t_data,crosscheck,buffer)) {
      result = HeapTupleUpdated;
    }
  }
  if (result != HeapTupleMayBeUpdated) {
    ;
    ;
     *ctid = oldtup . t_data -> t_ctid;
     *update_xmax = oldtup . t_data -> t_choice . t_heap . t_xmax;
    UnlockReleaseBuffer(buffer);
    if (have_tuple_lock) {
      UnlockTuple(relation,&oldtup . t_self,7);
    }
    if (vmbuffer != 0) {
      ReleaseBuffer(vmbuffer);
    }
    bms_free(hot_attrs);
    return result;
  }
/*
	 * If we didn't pin the visibility map page and the page has become all
	 * visible while we were busy locking the buffer, or during some
	 * subsequent window during which we had it unlocked, we'll have to unlock
	 * and re-lock, to avoid holding the buffer lock across an I/O.  That's a
	 * bit unfortunate, esepecially since we'll now have to recheck whether
	 * the tuple has been locked or updated under us, but hopefully it won't
	 * happen very often.
	 */
  if (vmbuffer == 0 && (((PageHeader )page) -> pd_flags) & 0x0004) {
    LockBuffer(buffer,0);
    visibilitymap_pin(relation,block,&vmbuffer);
    LockBuffer(buffer,2);
    goto l2;
  }
/*
	 * We're about to do the actual update -- check for conflict first, to
	 * avoid possibly having to roll back work we've just done.
	 */
  CheckForSerializableConflictIn(relation,&oldtup,buffer);
/* Fill in OID and transaction status data for newtup */
  if (relation -> rd_rel -> relhasoids) {
#ifdef NOT_USED
/* this is redundant with an Assert in HeapTupleSetOid */
#endif
    do {
      ;
       *((Oid *)(((char *)(newtup -> t_data)) + newtup -> t_data -> t_hoff - sizeof(Oid ))) = (((&oldtup) -> t_data -> t_infomask) & 0x0008? *((Oid *)(((char *)((&oldtup) -> t_data)) + (&oldtup) -> t_data -> t_hoff - sizeof(Oid ))) : ((Oid )0));
    }while (0);
  }
  else {
/* check there is not space for an OID */
    ;
  }
  newtup -> t_data -> t_infomask &= ~0xFFE0;
  newtup -> t_data -> t_infomask2 &= ~0xC000;
  newtup -> t_data -> t_infomask |= 0x0800 | 0x2000;
  newtup -> t_data -> t_choice . t_heap . t_xmin = xid;
  do {
    ;
    newtup -> t_data -> t_choice . t_heap . t_field3 . t_cid = cid;
    newtup -> t_data -> t_infomask &= ~0x0020;
  }while (0);
/* for cleanliness */
  newtup -> t_data -> t_choice . t_heap . t_xmax = 0;
  newtup -> t_tableOid = relation -> rd_id;
/*
	 * Replace cid with a combo cid if necessary.  Note that we already put
	 * the plain cid into the new tuple.
	 */
  HeapTupleHeaderAdjustCmax(oldtup . t_data,&cid,&iscombo);
/*
	 * If the toaster needs to be activated, OR if the new tuple will not fit
	 * on the same page as the old, then we need to release the content lock
	 * (but not the pin!) on the old tuple's buffer while we are off doing
	 * TOAST and/or table-file-extension work.	We must mark the old tuple to
	 * show that it's already being updated, else other processes may try to
	 * update it themselves.
	 *
	 * We need to invoke the toaster if there are already any out-of-line
	 * toasted values present, or if the new tuple is over-threshold.
	 */
  if ((relation -> rd_rel -> relkind) != 'r') {
/* toast table entries should never be recursively toasted */
    ;
    ;
    need_toast = ((bool )0);
  }
  else {
    need_toast = ((((&oldtup) -> t_data -> t_infomask) & 0x0004) != 0 || ((newtup -> t_data -> t_infomask) & 0x0004) != 0 || (newtup -> t_len) > (((intptr_t )((8192 - (((intptr_t )(((size_t )(&((PageHeaderData *)0) -> pd_linp)) + 4 * sizeof(ItemIdData ))) + (8 - 1) & ~((intptr_t )(8 - 1)))) / 4)) & ~((intptr_t )(8 - 1))));
  }
  pagefree = PageGetHeapFreeSpace(page);
  newtupsize = (((intptr_t )(newtup -> t_len)) + (8 - 1) & ~((intptr_t )(8 - 1)));
  if (need_toast || newtupsize > pagefree) {
/* Clear obsolete visibility flags ... */
    oldtup . t_data -> t_infomask &= ~(0x0400 | 0x0800 | 0x1000 | (0x0040 | 0x0080) | (0x4000 | 0x8000));
    (&oldtup) -> t_data -> t_infomask2 &= ~0x4000;
/* ... and store info about transaction updating this tuple */
    oldtup . t_data -> t_choice . t_heap . t_xmax = xid;
    do {
      ;
      oldtup . t_data -> t_choice . t_heap . t_field3 . t_cid = cid;
      if (iscombo) {
        oldtup . t_data -> t_infomask |= 0x0020;
      }
      else {
        oldtup . t_data -> t_infomask &= ~0x0020;
      }
    }while (0);
/* temporarily make it look not-updated */
    oldtup . t_data -> t_ctid = oldtup . t_self;
    already_marked = ((bool )1);
    LockBuffer(buffer,0);
/*
		 * Let the toaster do its thing, if needed.
		 *
		 * Note: below this point, heaptup is the data we actually intend to
		 * store into the relation; newtup is the caller's original untoasted
		 * data.
		 */
    if (need_toast) {
/* Note we always use WAL and FSM during updates */
      heaptup = toast_insert_or_update(relation,newtup,&oldtup,0);
      newtupsize = (((intptr_t )(heaptup -> t_len)) + (8 - 1) & ~((intptr_t )(8 - 1)));
    }
    else {
      heaptup = newtup;
    }
/*
		 * Now, do we need a new page for the tuple, or not?  This is a bit
		 * tricky since someone else could have added tuples to the page while
		 * we weren't looking.  We have to recheck the available space after
		 * reacquiring the buffer lock.  But don't bother to do that if the
		 * former amount of free space is still not enough; it's unlikely
		 * there's more free now than before.
		 *
		 * What's more, if we need to get a new page, we will need to acquire
		 * buffer locks on both old and new pages.	To avoid deadlock against
		 * some other backend trying to get the same two locks in the other
		 * order, we must be consistent about the order we get the locks in.
		 * We use the rule "lock the lower-numbered page of the relation
		 * first".  To implement this, we must do RelationGetBufferForTuple
		 * while not holding the lock on the old page, and we must rely on it
		 * to get the locks on both pages in the correct order.
		 */
    if (newtupsize > pagefree) {
/* Assume there's no chance to put heaptup on same page. */
      newbuf = RelationGetBufferForTuple(relation,(heaptup -> t_len),buffer,0,((void *)0),&vmbuffer_new,&vmbuffer);
    }
    else {
/* Re-acquire the lock on the old tuple's page. */
      LockBuffer(buffer,2);
/* Re-check using the up-to-date free space */
      pagefree = PageGetHeapFreeSpace(page);
      if (newtupsize > pagefree) {
/*
				 * Rats, it doesn't fit anymore.  We must now unlock and
				 * relock to avoid deadlock.  Fortunately, this path should
				 * seldom be taken.
				 */
        LockBuffer(buffer,0);
        newbuf = RelationGetBufferForTuple(relation,(heaptup -> t_len),buffer,0,((void *)0),&vmbuffer_new,&vmbuffer);
      }
      else {
/* OK, it fits here, so we're done. */
        newbuf = buffer;
      }
    }
  }
  else {
/* No TOAST work needed, and it'll fit on same page */
    already_marked = ((bool )0);
    newbuf = buffer;
    heaptup = newtup;
  }
/*
	 * We're about to create the new tuple -- check for conflict first, to
	 * avoid possibly having to roll back work we've just done.
	 *
	 * NOTE: For a tuple insert, we only need to check for table locks, since
	 * predicate locking at the index level will cover ranges for anything
	 * except a table scan.  Therefore, only provide the relation.
	 */
  CheckForSerializableConflictIn(relation,((void *)0),0);
/*
	 * At this point newbuf and buffer are both pinned and locked, and newbuf
	 * has enough space for the new tuple.	If they are the same buffer, only
	 * one pin is held.
	 */
  if (newbuf == buffer) {
/*
		 * Since the new tuple is going into the same page, we might be able
		 * to do a HOT update.	Check if any of the index columns have been
		 * changed.  If not, then HOT update is possible.
		 */
    if (HeapSatisfiesHOTUpdate(relation,hot_attrs,&oldtup,heaptup)) {
      use_hot_update = ((bool )1);
    }
  }
  else {
/* Set a hint that the old page could use prune/defrag */
    ((PageHeader )page) -> pd_flags |= 0x0002;
  }
/* NO EREPORT(ERROR) from here till changes are logged */
  CritSectionCount++;
/*
	 * If this transaction commits, the old tuple will become DEAD sooner or
	 * later.  Set flag that this page is a candidate for pruning once our xid
	 * falls below the OldestXmin horizon.	If the transaction finally aborts,
	 * the subsequent page pruning will be a no-op and the hint will be
	 * cleared.
	 *
	 * XXX Should we set hint on newbuf as well?  If the transaction aborts,
	 * there would be a prunable tuple in the newbuf; but for now we choose
	 * not to optimize for aborts.	Note that heap_xlog_update must be kept in
	 * sync if this decision changes.
	 */
  do {
    ;
    if (!(((PageHeader )page) -> pd_prune_xid != ((TransactionId )0)) || TransactionIdPrecedes(xid,((PageHeader )page) -> pd_prune_xid)) {
      ((PageHeader )page) -> pd_prune_xid = xid;
    }
  }while (0);
  if (use_hot_update) {
/* Mark the old tuple as HOT-updated */
    (&oldtup) -> t_data -> t_infomask2 |= 0x4000;
/* And mark the new tuple as heap-only */
    heaptup -> t_data -> t_infomask2 |= 0x8000;
/* Mark the caller's copy too, in case different from heaptup */
    newtup -> t_data -> t_infomask2 |= 0x8000;
  }
  else {
/* Make sure tuples are correctly marked as not-HOT */
    (&oldtup) -> t_data -> t_infomask2 &= ~0x4000;
    heaptup -> t_data -> t_infomask2 &= ~0x8000;
    newtup -> t_data -> t_infomask2 &= ~0x8000;
  }
/* insert new tuple */
  RelationPutHeapTuple(relation,newbuf,heaptup);
  if (!already_marked) {
/* Clear obsolete visibility flags ... */
    oldtup . t_data -> t_infomask &= ~(0x0400 | 0x0800 | 0x1000 | (0x0040 | 0x0080) | (0x4000 | 0x8000));
/* ... and store info about transaction updating this tuple */
    oldtup . t_data -> t_choice . t_heap . t_xmax = xid;
    do {
      ;
      oldtup . t_data -> t_choice . t_heap . t_field3 . t_cid = cid;
      if (iscombo) {
        oldtup . t_data -> t_infomask |= 0x0020;
      }
      else {
        oldtup . t_data -> t_infomask &= ~0x0020;
      }
    }while (0);
  }
/* record address of new tuple in t_ctid of old one */
  oldtup . t_data -> t_ctid = heaptup -> t_self;
/* clear PD_ALL_VISIBLE flags */
  if ((((PageHeader )((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192))))))) -> pd_flags) & 0x0004) {
    all_visible_cleared = ((bool )1);
    ((PageHeader )((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192))))))) -> pd_flags &= ~0x0004;
    visibilitymap_clear(relation,BufferGetBlockNumber(buffer),vmbuffer);
  }
  if (newbuf != buffer && (((PageHeader )((Page )((((void )((bool )1)) , (newbuf < 0?LocalBufferBlockPointers[-newbuf - 1] : ((Block )(BufferBlocks + ((Size )(newbuf - 1)) * 8192))))))) -> pd_flags) & 0x0004) {
    all_visible_cleared_new = ((bool )1);
    ((PageHeader )((Page )((((void )((bool )1)) , (newbuf < 0?LocalBufferBlockPointers[-newbuf - 1] : ((Block )(BufferBlocks + ((Size )(newbuf - 1)) * 8192))))))) -> pd_flags &= ~0x0004;
    visibilitymap_clear(relation,BufferGetBlockNumber(newbuf),vmbuffer_new);
  }
  if (newbuf != buffer) {
    MarkBufferDirty(newbuf);
  }
  MarkBufferDirty(buffer);
/* XLOG stuff */
  if ((relation -> rd_rel -> relpersistence) == 'p') {
    XLogRecPtr recptr = log_heap_update(relation,buffer,oldtup . t_self,newbuf,heaptup,all_visible_cleared,all_visible_cleared_new);
    if (newbuf != buffer) {
      ((PageHeader )((Page )((((void )((bool )1)) , (newbuf < 0?LocalBufferBlockPointers[-newbuf - 1] : ((Block )(BufferBlocks + ((Size )(newbuf - 1)) * 8192))))))) -> pd_lsn = recptr;
      ((PageHeader )((Page )((((void )((bool )1)) , (newbuf < 0?LocalBufferBlockPointers[-newbuf - 1] : ((Block )(BufferBlocks + ((Size )(newbuf - 1)) * 8192))))))) -> pd_tli = ((uint16 )ThisTimeLineID);
    }
    ((PageHeader )((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192))))))) -> pd_lsn = recptr;
    ((PageHeader )((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192))))))) -> pd_tli = ((uint16 )ThisTimeLineID);
  }
  do {
    ;
    CritSectionCount--;
  }while (0);
  if (newbuf != buffer) {
    LockBuffer(newbuf,0);
  }
  LockBuffer(buffer,0);
/*
	 * Mark old tuple for invalidation from system caches at next command
	 * boundary, and mark the new tuple for invalidation in case we abort. We
	 * have to do this before releasing the buffer because oldtup is in the
	 * buffer.	(heaptup is all in local memory, but it's necessary to process
	 * both tuple versions in one call to inval.c so we can avoid redundant
	 * sinval messages.)
	 */
  CacheInvalidateHeapTuple(relation,&oldtup,heaptup);
/* Now we can release the buffer(s) */
  if (newbuf != buffer) {
    ReleaseBuffer(newbuf);
  }
  ReleaseBuffer(buffer);
  if ((((void )((bool )1)) , vmbuffer_new != 0)) {
    ReleaseBuffer(vmbuffer_new);
  }
  if ((((void )((bool )1)) , vmbuffer != 0)) {
    ReleaseBuffer(vmbuffer);
  }
/*
	 * Release the lmgr tuple lock, if we had it.
	 */
  if (have_tuple_lock) {
    UnlockTuple(relation,&oldtup . t_self,7);
  }
  pgstat_count_heap_update(relation,use_hot_update);
/*
	 * If heaptup is a private copy, release it.  Don't forget to copy t_self
	 * back to the caller's image, too.
	 */
  if (heaptup != newtup) {
    newtup -> t_self = heaptup -> t_self;
    heap_freetuple(heaptup);
  }
  bms_free(hot_attrs);
  return HeapTupleMayBeUpdated;
}
/*
 * Check if the specified attribute's value is same in both given tuples.
 * Subroutine for HeapSatisfiesHOTUpdate.
 */

static bool heap_tuple_attr_equals(TupleDesc tupdesc,int attrnum,HeapTuple tup1,HeapTuple tup2)
{
  Datum value1;
  Datum value2;
  bool isnull1;
  bool isnull2;
  Form_pg_attribute att;
/*
	 * If it's a whole-tuple reference, say "not equal".  It's not really
	 * worth supporting this case, since it could only succeed after a no-op
	 * update, which is hardly a case worth optimizing for.
	 */
  if (attrnum == 0) {
    return (bool )0;
  }
/*
	 * Likewise, automatically say "not equal" for any system attribute other
	 * than OID and tableOID; we cannot expect these to be consistent in a HOT
	 * chain, or even to be set correctly yet in the new tuple.
	 */
  if (attrnum < 0) {
    if (attrnum != - 2 && attrnum != - 7) {
      return (bool )0;
    }
  }
/*
	 * Extract the corresponding values.  XXX this is pretty inefficient if
	 * there are many indexed columns.	Should HeapSatisfiesHOTUpdate do a
	 * single heap_deform_tuple call on each tuple, instead?  But that doesn't
	 * work for system columns ...
	 */
  value1 = (attrnum > 0?((attrnum > ((int )((tup1 -> t_data -> t_infomask2) & 0x07FF))?(( *(&isnull1) = ((bool )1) , ((Datum )((void *)0)))) : (((((void )((bool )1)) ,  *(&isnull1) = ((bool )0)) , (!((tup1 -> t_data -> t_infomask) & 0x0001)?((tupdesc -> attrs[attrnum - 1] -> attcacheoff >= 0?((tupdesc -> attrs[attrnum - 1] -> attbyval?(((tupdesc -> attrs[attrnum - 1] -> attlen) == ((int )(sizeof(Datum )))? *((Datum *)(((char *)(tup1 -> t_data)) + tup1 -> t_data -> t_hoff + tupdesc -> attrs[attrnum - 1] -> attcacheoff)) : (((tupdesc -> attrs[attrnum - 1] -> attlen) == ((int )(sizeof(int32 )))?((Datum )(((Datum )( *((int32 *)(((char *)(tup1 -> t_data)) + tup1 -> t_data -> t_hoff + tupdesc -> attrs[attrnum - 1] -> attcacheoff)))) & 0xFFFFFFFF)) : (((tupdesc -> attrs[attrnum - 1] -> attlen) == ((int )(sizeof(int16 )))?((Datum )(((Datum )( *((int16 *)(((char *)(tup1 -> t_data)) + tup1 -> t_data -> t_hoff + tupdesc -> attrs[attrnum - 1] -> attcacheoff)))) & 0xffff)) : ((((void )((bool )1)) , ((Datum )(((Datum )( *((char *)(((char *)(tup1 -> t_data)) + tup1 -> t_data -> t_hoff + tupdesc -> attrs[attrnum - 1] -> attcacheoff)))) & 0x000000ff)))))))))) : ((Datum )((char *)(((char *)(tup1 -> t_data)) + tup1 -> t_data -> t_hoff + tupdesc -> attrs[attrnum - 1] -> attcacheoff))))) : nocachegetattr(tup1,attrnum,tupdesc))) : ((!(tup1 -> t_data -> t_bits[attrnum - 1 >> 3] & 1 << (attrnum - 1 & 0x07))?(( *(&isnull1) = ((bool )1) , ((Datum )((void *)0)))) : nocachegetattr(tup1,attrnum,tupdesc)))))))) : heap_getsysattr(tup1,attrnum,tupdesc,&isnull1));
  value2 = (attrnum > 0?((attrnum > ((int )((tup2 -> t_data -> t_infomask2) & 0x07FF))?(( *(&isnull2) = ((bool )1) , ((Datum )((void *)0)))) : (((((void )((bool )1)) ,  *(&isnull2) = ((bool )0)) , (!((tup2 -> t_data -> t_infomask) & 0x0001)?((tupdesc -> attrs[attrnum - 1] -> attcacheoff >= 0?((tupdesc -> attrs[attrnum - 1] -> attbyval?(((tupdesc -> attrs[attrnum - 1] -> attlen) == ((int )(sizeof(Datum )))? *((Datum *)(((char *)(tup2 -> t_data)) + tup2 -> t_data -> t_hoff + tupdesc -> attrs[attrnum - 1] -> attcacheoff)) : (((tupdesc -> attrs[attrnum - 1] -> attlen) == ((int )(sizeof(int32 )))?((Datum )(((Datum )( *((int32 *)(((char *)(tup2 -> t_data)) + tup2 -> t_data -> t_hoff + tupdesc -> attrs[attrnum - 1] -> attcacheoff)))) & 0xFFFFFFFF)) : (((tupdesc -> attrs[attrnum - 1] -> attlen) == ((int )(sizeof(int16 )))?((Datum )(((Datum )( *((int16 *)(((char *)(tup2 -> t_data)) + tup2 -> t_data -> t_hoff + tupdesc -> attrs[attrnum - 1] -> attcacheoff)))) & 0xffff)) : ((((void )((bool )1)) , ((Datum )(((Datum )( *((char *)(((char *)(tup2 -> t_data)) + tup2 -> t_data -> t_hoff + tupdesc -> attrs[attrnum - 1] -> attcacheoff)))) & 0x000000ff)))))))))) : ((Datum )((char *)(((char *)(tup2 -> t_data)) + tup2 -> t_data -> t_hoff + tupdesc -> attrs[attrnum - 1] -> attcacheoff))))) : nocachegetattr(tup2,attrnum,tupdesc))) : ((!(tup2 -> t_data -> t_bits[attrnum - 1 >> 3] & 1 << (attrnum - 1 & 0x07))?(( *(&isnull2) = ((bool )1) , ((Datum )((void *)0)))) : nocachegetattr(tup2,attrnum,tupdesc)))))))) : heap_getsysattr(tup2,attrnum,tupdesc,&isnull2));
/*
	 * If one value is NULL and other is not, then they are certainly not
	 * equal
	 */
  if (isnull1 != isnull2) {
    return (bool )0;
  }
/*
	 * If both are NULL, they can be considered equal.
	 */
  if (isnull1) {
    return (bool )1;
  }
/*
	 * We do simple binary comparison of the two datums.  This may be overly
	 * strict because there can be multiple binary representations for the
	 * same logical value.	But we should be OK as long as there are no false
	 * positives.  Using a type-specific equality operator is messy because
	 * there could be multiple notions of equality in different operator
	 * classes; furthermore, we cannot safely invoke user-defined functions
	 * while holding exclusive buffer lock.
	 */
  if (attrnum <= 0) {
/* The only allowed system columns are OIDs, so do this */
    return (((Oid )(((Datum )value1) & 0xFFFFFFFF)) == ((Oid )(((Datum )value2) & 0xFFFFFFFF)));
  }
  else {
    ;
    att = tupdesc -> attrs[attrnum - 1];
    return datumIsEqual(value1,value2,att -> attbyval,(att -> attlen));
  }
}
/*
 * Check if the old and new tuples represent a HOT-safe update. To be able
 * to do a HOT update, we must not have changed any columns used in index
 * definitions.
 *
 * The set of attributes to be checked is passed in (we dare not try to
 * compute it while holding exclusive buffer lock...)  NOTE that hot_attrs
 * is destructively modified!  That is OK since this is invoked at most once
 * by heap_update().
 *
 * Returns true if safe to do HOT update.
 */

static bool HeapSatisfiesHOTUpdate(Relation relation,Bitmapset *hot_attrs,HeapTuple oldtup,HeapTuple newtup)
{
  int attrnum;
  while((attrnum = bms_first_member(hot_attrs)) >= 0){
/* Adjust for system attributes */
    attrnum += - 8;
/* If the attribute value has changed, we can't do HOT update */
    if (!heap_tuple_attr_equals(relation -> rd_att,attrnum,oldtup,newtup)) {
      return (bool )0;
    }
  }
  return (bool )1;
}
/*
 *	simple_heap_update - replace a tuple
 *
 * This routine may be used to update a tuple when concurrent updates of
 * the target tuple are not expected (for example, because we have a lock
 * on the relation associated with the tuple).	Any failure is reported
 * via ereport().
 */

void simple_heap_update(Relation relation,ItemPointer otid,HeapTuple tup)
{
  HTSU_Result result;
  ItemPointerData update_ctid;
  TransactionId update_xmax;
  result = heap_update(relation,otid,tup,&update_ctid,&update_xmax,GetCurrentCommandId(((bool )1)),((Snapshot )((void *)0)),((bool )1));
/* wait for commit */
  switch(result){
    case HeapTupleSelfUpdated:
{
/* Tuple was already updated in current command? */
      (elog_start("heapam.c",3396,__func__) , elog_finish(20,"tuple already updated by self"));
      break; 
    }
    case HeapTupleMayBeUpdated:
/* done successfully */
    break; 
    case HeapTupleUpdated:
{
      (elog_start("heapam.c",3404,__func__) , elog_finish(20,"tuple concurrently updated"));
      break; 
    }
    default:
{
      (elog_start("heapam.c",3408,__func__) , elog_finish(20,"unrecognized heap_update status: %u",result));
      break; 
    }
  }
}
/*
 *	heap_lock_tuple - lock a tuple in shared or exclusive mode
 *
 * Note that this acquires a buffer pin, which the caller must release.
 *
 * Input parameters:
 *	relation: relation containing tuple (caller must hold suitable lock)
 *	tuple->t_self: TID of tuple to lock (rest of struct need not be valid)
 *	cid: current command ID (used for visibility test, and stored into
 *		tuple's cmax if lock is successful)
 *	mode: indicates if shared or exclusive tuple lock is desired
 *	nowait: if true, ereport rather than blocking if lock not available
 *
 * Output parameters:
 *	*tuple: all fields filled in
 *	*buffer: set to buffer holding tuple (pinned but not locked at exit)
 *	*ctid: set to tuple's t_ctid, but only in failure cases
 *	*update_xmax: set to tuple's xmax, but only in failure cases
 *
 * Function result may be:
 *	HeapTupleMayBeUpdated: lock was successfully acquired
 *	HeapTupleSelfUpdated: lock failed because tuple updated by self
 *	HeapTupleUpdated: lock failed because tuple updated by other xact
 *
 * In the failure cases, the routine returns the tuple's t_ctid and t_xmax.
 * If t_ctid is the same as t_self, the tuple was deleted; if different, the
 * tuple was updated, and t_ctid is the location of the replacement tuple.
 * (t_xmax is needed to verify that the replacement tuple matches.)
 *
 *
 * NOTES: because the shared-memory lock table is of finite size, but users
 * could reasonably want to lock large numbers of tuples, we do not rely on
 * the standard lock manager to store tuple-level locks over the long term.
 * Instead, a tuple is marked as locked by setting the current transaction's
 * XID as its XMAX, and setting additional infomask bits to distinguish this
 * usage from the more normal case of having deleted the tuple.  When
 * multiple transactions concurrently share-lock a tuple, the first locker's
 * XID is replaced in XMAX with a MultiTransactionId representing the set of
 * XIDs currently holding share-locks.
 *
 * When it is necessary to wait for a tuple-level lock to be released, the
 * basic delay is provided by XactLockTableWait or MultiXactIdWait on the
 * contents of the tuple's XMAX.  However, that mechanism will release all
 * waiters concurrently, so there would be a race condition as to which
 * waiter gets the tuple, potentially leading to indefinite starvation of
 * some waiters.  The possibility of share-locking makes the problem much
 * worse --- a steady stream of share-lockers can easily block an exclusive
 * locker forever.	To provide more reliable semantics about who gets a
 * tuple-level lock first, we use the standard lock manager.  The protocol
 * for waiting for a tuple-level lock is really
 *		LockTuple()
 *		XactLockTableWait()
 *		mark tuple as locked by me
 *		UnlockTuple()
 * When there are multiple waiters, arbitration of who is to get the lock next
 * is provided by LockTuple().	However, at most one tuple-level lock will
 * be held or awaited per backend at any time, so we don't risk overflow
 * of the lock table.  Note that incoming share-lockers are required to
 * do LockTuple as well, if there is any conflict, to ensure that they don't
 * starve out waiting exclusive-lockers.  However, if there is not any active
 * conflict for a tuple, we don't incur any extra overhead.
 */

HTSU_Result heap_lock_tuple(Relation relation,HeapTuple tuple,Buffer *buffer,ItemPointer ctid,TransactionId *update_xmax,CommandId cid,LockTupleMode mode,bool nowait)
{
  HTSU_Result result;
  ItemPointer tid = &tuple -> t_self;
  ItemId lp;
  Page page;
  TransactionId xid;
  TransactionId xmax;
  uint16 old_infomask;
  uint16 new_infomask;
  LOCKMODE tuple_lock_type;
  bool have_tuple_lock = (bool )0;
  tuple_lock_type = (mode == LockTupleShared?5 : 7);
   *buffer = ReadBuffer(relation,((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&tid -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&tid -> ip_blkid) -> bi_lo)))))))));
  LockBuffer( *buffer,2);
  page = ((Page )((((void )((bool )1)) , ( *buffer < 0?LocalBufferBlockPointers[-( *buffer) - 1] : ((Block )(BufferBlocks + ((Size )( *buffer - 1)) * 8192))))));
  lp = ((ItemId )(&((PageHeader )page) -> pd_linp[((((void )((bool )1)) , tid -> ip_posid)) - 1]));
  ;
  tuple -> t_data = ((HeapTupleHeader )(((((void )((bool )1)) , ((void )((bool )1))) , ((Item )(((char *)page) + lp -> lp_off)))));
  tuple -> t_len = (lp -> lp_len);
  tuple -> t_tableOid = relation -> rd_id;
  l3:
  result = HeapTupleSatisfiesUpdate(tuple -> t_data,cid, *buffer);
  if (result == HeapTupleInvisible) {
    UnlockReleaseBuffer( *buffer);
    (elog_start("heapam.c",3510,__func__) , elog_finish(20,"attempted to lock invisible tuple"));
  }
  else {
    if (result == HeapTupleBeingUpdated) {
      TransactionId xwait;
      uint16 infomask;
/* must copy state data before unlocking buffer */
      xwait = tuple -> t_data -> t_choice . t_heap . t_xmax;
      infomask = tuple -> t_data -> t_infomask;
      LockBuffer( *buffer,0);
/*
		 * If we wish to acquire share lock, and the tuple is already
		 * share-locked by a multixact that includes any subtransaction of the
		 * current top transaction, then we effectively hold the desired lock
		 * already.  We *must* succeed without trying to take the tuple lock,
		 * else we will deadlock against anyone waiting to acquire exclusive
		 * lock.  We don't need to make any state changes in this case.
		 */
      if (mode == LockTupleShared && infomask & 0x1000 && MultiXactIdIsCurrent(((MultiXactId )xwait))) {
        ;
/* Probably can't hold tuple lock here, but may as well check */
        if (have_tuple_lock) {
          UnlockTuple(relation,tid,tuple_lock_type);
        }
        return HeapTupleMayBeUpdated;
      }
/*
		 * Acquire tuple lock to establish our priority for the tuple.
		 * LockTuple will release us when we are next-in-line for the tuple.
		 * We must do this even if we are share-locking.
		 *
		 * If we are forced to "start over" below, we keep the tuple lock;
		 * this arranges that we stay at the head of the line while rechecking
		 * tuple state.
		 */
      if (!have_tuple_lock) {
        if (nowait) {
          if (!ConditionalLockTuple(relation,tid,tuple_lock_type)) {
            errstart(20,"heapam.c",3559,__func__,((void *)0))?errfinish(errcode(('5' - 48 & 0x3F) + (('5' - 48 & 0x3F) << 6) + (('P' - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('3' - 48 & 0x3F) << 24)),errmsg("could not obtain lock on row in relation \"%s\"",relation -> rd_rel -> relname . data)) : ((void )0);
          }
        }
        else {
          LockTuple(relation,tid,tuple_lock_type);
        }
        have_tuple_lock = ((bool )1);
      }
      if (mode == LockTupleShared && infomask & 0x0080) {
/*
			 * Acquiring sharelock when there's at least one sharelocker
			 * already.  We need not wait for him/them to complete.
			 */
        LockBuffer( *buffer,2);
/*
			 * Make sure it's still a shared lock, else start over.  (It's OK
			 * if the ownership of the shared lock has changed, though.)
			 */
        if (!((tuple -> t_data -> t_infomask) & 0x0080)) {
          goto l3;
        }
      }
      else {
        if (infomask & 0x1000) {
/* wait for multixact to end */
          if (nowait) {
            if (!ConditionalMultiXactIdWait(((MultiXactId )xwait))) {
              errstart(20,"heapam.c",3590,__func__,((void *)0))?errfinish(errcode(('5' - 48 & 0x3F) + (('5' - 48 & 0x3F) << 6) + (('P' - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('3' - 48 & 0x3F) << 24)),errmsg("could not obtain lock on row in relation \"%s\"",relation -> rd_rel -> relname . data)) : ((void )0);
            }
          }
          else {
            MultiXactIdWait(((MultiXactId )xwait));
          }
          LockBuffer( *buffer,2);
/*
			 * If xwait had just locked the tuple then some other xact could
			 * update this tuple before we get to this point. Check for xmax
			 * change, and start over if so.
			 */
          if (!((tuple -> t_data -> t_infomask) & 0x1000) || !(tuple -> t_data -> t_choice . t_heap . t_xmax == xwait)) {
            goto l3;
          }
/*
			 * You might think the multixact is necessarily done here, but not
			 * so: it could have surviving members, namely our own xact or
			 * other subxacts of this backend.	It is legal for us to lock the
			 * tuple in either case, however.  We don't bother changing the
			 * on-disk hint bits since we are about to overwrite the xmax
			 * altogether.
			 */
        }
        else {
/* wait for regular transaction to end */
          if (nowait) {
            if (!ConditionalXactLockTableWait(xwait)) {
              errstart(20,"heapam.c",3625,__func__,((void *)0))?errfinish(errcode(('5' - 48 & 0x3F) + (('5' - 48 & 0x3F) << 6) + (('P' - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('3' - 48 & 0x3F) << 24)),errmsg("could not obtain lock on row in relation \"%s\"",relation -> rd_rel -> relname . data)) : ((void )0);
            }
          }
          else {
            XactLockTableWait(xwait);
          }
          LockBuffer( *buffer,2);
/*
			 * xwait is done, but if xwait had just locked the tuple then some
			 * other xact could update this tuple before we get to this point.
			 * Check for xmax change, and start over if so.
			 */
          if ((tuple -> t_data -> t_infomask) & 0x1000 || !(tuple -> t_data -> t_choice . t_heap . t_xmax == xwait)) {
            goto l3;
          }
/* Otherwise check if it committed or aborted */
          UpdateXmaxHintBits(tuple -> t_data, *buffer,xwait);
        }
      }
/*
		 * We may lock if previous xmax aborted, or if it committed but only
		 * locked the tuple without updating it.  The case where we didn't
		 * wait because we are joining an existing shared lock is correctly
		 * handled, too.
		 */
      if ((tuple -> t_data -> t_infomask) & (0x0800 | (0x0040 | 0x0080))) {
        result = HeapTupleMayBeUpdated;
      }
      else {
        result = HeapTupleUpdated;
      }
    }
  }
  if (result != HeapTupleMayBeUpdated) {
    ;
    ;
     *ctid = tuple -> t_data -> t_ctid;
     *update_xmax = tuple -> t_data -> t_choice . t_heap . t_xmax;
    LockBuffer( *buffer,0);
    if (have_tuple_lock) {
      UnlockTuple(relation,tid,tuple_lock_type);
    }
    return result;
  }
/*
	 * We might already hold the desired lock (or stronger), possibly under a
	 * different subtransaction of the current top transaction.  If so, there
	 * is no need to change state or issue a WAL record.  We already handled
	 * the case where this is true for xmax being a MultiXactId, so now check
	 * for cases where it is a plain TransactionId.
	 *
	 * Note in particular that this covers the case where we already hold
	 * exclusive lock on the tuple and the caller only wants shared lock. It
	 * would certainly not do to give up the exclusive lock.
	 */
  xmax = tuple -> t_data -> t_choice . t_heap . t_xmax;
  old_infomask = tuple -> t_data -> t_infomask;
  if (!(old_infomask & (0x0800 | 0x0400 | 0x1000)) && ((mode == LockTupleShared?old_infomask & (0x0040 | 0x0080) : old_infomask & 0x0040)) && TransactionIdIsCurrentTransactionId(xmax)) {
    LockBuffer( *buffer,0);
/* Probably can't hold tuple lock here, but may as well check */
    if (have_tuple_lock) {
      UnlockTuple(relation,tid,tuple_lock_type);
    }
    return HeapTupleMayBeUpdated;
  }
/*
	 * Compute the new xmax and infomask to store into the tuple.  Note we do
	 * not modify the tuple just yet, because that would leave it in the wrong
	 * state if multixact.c elogs.
	 */
  xid = GetCurrentTransactionId();
  new_infomask = (old_infomask & ~(0x0400 | 0x0800 | 0x1000 | (0x0040 | 0x0080) | (0x4000 | 0x8000)));
  if (mode == LockTupleShared) {
/*
		 * If this is the first acquisition of a shared lock in the current
		 * transaction, set my per-backend OldestMemberMXactId setting. We can
		 * be certain that the transaction will never become a member of any
		 * older MultiXactIds than that.  (We have to do this even if we end
		 * up just using our own TransactionId below, since some other backend
		 * could incorporate our XID into a MultiXact immediately afterwards.)
		 */
    MultiXactIdSetOldestMember();
    new_infomask |= 0x0080;
/*
		 * Check to see if we need a MultiXactId because there are multiple
		 * lockers.
		 *
		 * HeapTupleSatisfiesUpdate will have set the HEAP_XMAX_INVALID bit if
		 * the xmax was a MultiXactId but it was not running anymore. There is
		 * a race condition, which is that the MultiXactId may have finished
		 * since then, but that uncommon case is handled within
		 * MultiXactIdExpand.
		 *
		 * There is a similar race condition possible when the old xmax was a
		 * regular TransactionId.  We test TransactionIdIsInProgress again
		 * just to narrow the window, but it's still possible to end up
		 * creating an unnecessary MultiXactId.  Fortunately this is harmless.
		 */
    if (!(old_infomask & (0x0800 | 0x0400))) {
      if (old_infomask & 0x1000) {
/*
				 * If the XMAX is already a MultiXactId, then we need to
				 * expand it to include our own TransactionId.
				 */
        xid = MultiXactIdExpand(((MultiXactId )xmax),xid);
        new_infomask |= 0x1000;
      }
      else {
        if (TransactionIdIsInProgress(xmax)) {
/*
				 * If the XMAX is a valid TransactionId, then we need to
				 * create a new MultiXactId that includes both the old locker
				 * and our own TransactionId.
				 */
          xid = MultiXactIdCreate(xmax,xid);
          new_infomask |= 0x1000;
        }
        else {
/*
				 * Can get here iff HeapTupleSatisfiesUpdate saw the old xmax
				 * as running, but it finished before
				 * TransactionIdIsInProgress() got to run.	Treat it like
				 * there's no locker in the tuple.
				 */
        }
      }
    }
    else {
/*
			 * There was no previous locker, so just insert our own
			 * TransactionId.
			 */
    }
  }
  else {
/* We want an exclusive lock on the tuple */
    new_infomask |= 0x0040;
  }
  CritSectionCount++;
/*
	 * Store transaction information of xact locking the tuple.
	 *
	 * Note: Cmax is meaningless in this context, so don't set it; this avoids
	 * possibly generating a useless combo CID.
	 */
  tuple -> t_data -> t_infomask = new_infomask;
  tuple -> t_data -> t_infomask2 &= ~0x4000;
  tuple -> t_data -> t_choice . t_heap . t_xmax = xid;
/* Make sure there is no forward chain link in t_ctid */
  tuple -> t_data -> t_ctid =  *tid;
  MarkBufferDirty( *buffer);
/*
	 * XLOG stuff.	You might think that we don't need an XLOG record because
	 * there is no state change worth restoring after a crash.	You would be
	 * wrong however: we have just written either a TransactionId or a
	 * MultiXactId that may never have been seen on disk before, and we need
	 * to make sure that there are XLOG entries covering those ID numbers.
	 * Else the same IDs might be re-used after a crash, which would be
	 * disastrous if this page made it to disk before the crash.  Essentially
	 * we have to enforce the WAL log-before-data rule even in this case.
	 * (Also, in a PITR log-shipping or 2PC environment, we have to have XLOG
	 * entries for everything anyway.)
	 */
  if ((relation -> rd_rel -> relpersistence) == 'p') {
    xl_heap_lock xlrec;
    XLogRecPtr recptr;
    XLogRecData rdata[2];
    xlrec . target . node = relation -> rd_node;
    xlrec . target . tid = tuple -> t_self;
    xlrec . locking_xid = xid;
    xlrec . xid_is_mxact = ((new_infomask & 0x1000) != 0);
    xlrec . shared_lock = (mode == LockTupleShared);
    rdata[0] . data = ((char *)(&xlrec));
    rdata[0] . len = (((size_t )(&((xl_heap_lock *)0) -> shared_lock)) + sizeof(bool ));
    rdata[0] . buffer = 0;
    rdata[0] . next = &rdata[1];
    rdata[1] . data = ((void *)0);
    rdata[1] . len = 0;
    rdata[1] . buffer =  *buffer;
    rdata[1] . buffer_std = ((bool )1);
    rdata[1] . next = ((void *)0);
    recptr = XLogInsert(10,0x60,rdata);
    ((PageHeader )page) -> pd_lsn = recptr;
    ((PageHeader )page) -> pd_tli = ((uint16 )ThisTimeLineID);
  }
  do {
    ;
    CritSectionCount--;
  }while (0);
  LockBuffer( *buffer,0);
/*
	 * Don't update the visibility map here. Locking a tuple doesn't change
	 * visibility info.
	 */
/*
	 * Now that we have successfully marked the tuple as locked, we can
	 * release the lmgr tuple lock, if we had it.
	 */
  if (have_tuple_lock) {
    UnlockTuple(relation,tid,tuple_lock_type);
  }
  return HeapTupleMayBeUpdated;
}
/*
 * heap_inplace_update - update a tuple "in place" (ie, overwrite it)
 *
 * Overwriting violates both MVCC and transactional safety, so the uses
 * of this function in Postgres are extremely limited.	Nonetheless we
 * find some places to use it.
 *
 * The tuple cannot change size, and therefore it's reasonable to assume
 * that its null bitmap (if any) doesn't change either.  So we just
 * overwrite the data portion of the tuple without touching the null
 * bitmap or any of the header fields.
 *
 * tuple is an in-memory tuple structure containing the data to be written
 * over the target tuple.  Also, tuple->t_self identifies the target tuple.
 */

void heap_inplace_update(Relation relation,HeapTuple tuple)
{
  Buffer buffer;
  Page page;
  OffsetNumber offnum;
  ItemId lp = ((void *)0);
  HeapTupleHeader htup;
  uint32 oldlen;
  uint32 newlen;
  buffer = ReadBuffer(relation,((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&(&tuple -> t_self) -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&(&tuple -> t_self) -> ip_blkid) -> bi_lo)))))))));
  LockBuffer(buffer,2);
  page = ((Page )((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192)))))));
  offnum = ((((void )((bool )1)) , (&tuple -> t_self) -> ip_posid));
  if ((((((PageHeader )page) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )page) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData ))) >= offnum) {
    lp = ((ItemId )(&((PageHeader )page) -> pd_linp[offnum - 1]));
  }
  if ((((((PageHeader )page) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )page) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData ))) < offnum || !((lp -> lp_flags) == 1)) {
    (elog_start("heapam.c",3898,__func__) , elog_finish(20,"heap_inplace_update: invalid lp"));
  }
  htup = ((HeapTupleHeader )(((((void )((bool )1)) , ((void )((bool )1))) , ((Item )(((char *)page) + lp -> lp_off)))));
  oldlen = ((lp -> lp_len) - (htup -> t_hoff));
  newlen = tuple -> t_len - (tuple -> t_data -> t_hoff);
  if (oldlen != newlen || (htup -> t_hoff) != (tuple -> t_data -> t_hoff)) {
    (elog_start("heapam.c",3905,__func__) , elog_finish(20,"heap_inplace_update: wrong tuple length"));
  }
/* NO EREPORT(ERROR) from here till changes are logged */
  CritSectionCount++;
  memcpy((((char *)htup) + htup -> t_hoff),(((char *)(tuple -> t_data)) + tuple -> t_data -> t_hoff),newlen);
  MarkBufferDirty(buffer);
/* XLOG stuff */
  if ((relation -> rd_rel -> relpersistence) == 'p') {
    xl_heap_inplace xlrec;
    XLogRecPtr recptr;
    XLogRecData rdata[2];
    xlrec . target . node = relation -> rd_node;
    xlrec . target . tid = tuple -> t_self;
    rdata[0] . data = ((char *)(&xlrec));
    rdata[0] . len = (((size_t )(&((xl_heap_inplace *)0) -> target)) + (((size_t )(&((xl_heaptid *)0) -> tid)) + (((size_t )(&((ItemPointerData *)0) -> ip_posid)) + sizeof(OffsetNumber ))));
    rdata[0] . buffer = 0;
    rdata[0] . next = &rdata[1];
    rdata[1] . data = ((char *)htup) + htup -> t_hoff;
    rdata[1] . len = newlen;
    rdata[1] . buffer = buffer;
    rdata[1] . buffer_std = ((bool )1);
    rdata[1] . next = ((void *)0);
    recptr = XLogInsert(10,0x70,rdata);
    ((PageHeader )page) -> pd_lsn = recptr;
    ((PageHeader )page) -> pd_tli = ((uint16 )ThisTimeLineID);
  }
  do {
    ;
    CritSectionCount--;
  }while (0);
  UnlockReleaseBuffer(buffer);
/*
	 * Send out shared cache inval if necessary.  Note that because we only
	 * pass the new version of the tuple, this mustn't be used for any
	 * operations that could change catcache lookup keys.  But we aren't
	 * bothering with index updates either, so that's true a fortiori.
	 */
  if (!(Mode == BootstrapProcessing)) {
    CacheInvalidateHeapTuple(relation,tuple,((void *)0));
  }
}
/*
 * heap_freeze_tuple
 *
 * Check to see whether any of the XID fields of a tuple (xmin, xmax, xvac)
 * are older than the specified cutoff XID.  If so, replace them with
 * FrozenTransactionId or InvalidTransactionId as appropriate, and return
 * TRUE.  Return FALSE if nothing was changed.
 *
 * It is assumed that the caller has checked the tuple with
 * HeapTupleSatisfiesVacuum() and determined that it is not HEAPTUPLE_DEAD
 * (else we should be removing the tuple, not freezing it).
 *
 * NB: cutoff_xid *must* be <= the current global xmin, to ensure that any
 * XID older than it could neither be running nor seen as running by any
 * open transaction.  This ensures that the replacement will not change
 * anyone's idea of the tuple state.  Also, since we assume the tuple is
 * not HEAPTUPLE_DEAD, the fact that an XID is not still running allows us
 * to assume that it is either committed good or aborted, as appropriate;
 * so we need no external state checks to decide what to do.  (This is good
 * because this function is applied during WAL recovery, when we don't have
 * access to any such state, and can't depend on the hint bits to be set.)
 *
 * If the tuple is in a shared buffer, caller must hold an exclusive lock on
 * that buffer.
 *
 * Note: it might seem we could make the changes without exclusive lock, since
 * TransactionId read/write is assumed atomic anyway.  However there is a race
 * condition: someone who just fetched an old XID that we overwrite here could
 * conceivably not finish checking the XID against pg_clog before we finish
 * the VACUUM and perhaps truncate off the part of pg_clog he needs.  Getting
 * exclusive lock ensures no other backend is in process of checking the
 * tuple status.  Also, getting exclusive lock makes it safe to adjust the
 * infomask bits.
 */

bool heap_freeze_tuple(HeapTupleHeader tuple,TransactionId cutoff_xid)
{
  bool changed = (bool )0;
  TransactionId xid;
  xid = tuple -> t_choice . t_heap . t_xmin;
  if (xid >= ((TransactionId )3) && TransactionIdPrecedes(xid,cutoff_xid)) {
    tuple -> t_choice . t_heap . t_xmin = ((TransactionId )2);
/*
		 * Might as well fix the hint bits too; usually XMIN_COMMITTED will
		 * already be set here, but there's a small chance not.
		 */
    ;
    tuple -> t_infomask |= 0x0100;
    changed = ((bool )1);
  }
  if (!((tuple -> t_infomask) & 0x1000)) {
    xid = tuple -> t_choice . t_heap . t_xmax;
    if (xid >= ((TransactionId )3) && TransactionIdPrecedes(xid,cutoff_xid)) {
      tuple -> t_choice . t_heap . t_xmax = ((TransactionId )0);
/*
			 * The tuple might be marked either XMAX_INVALID or XMAX_COMMITTED
			 * + LOCKED.  Normalize to INVALID just to be sure no one gets
			 * confused.
			 */
      tuple -> t_infomask &= ~0x0400;
      tuple -> t_infomask |= 0x0800;
      tuple -> t_infomask2 &= ~0x4000;
      changed = ((bool )1);
    }
  }
  else {
/*----------
		 * XXX perhaps someday we should zero out very old MultiXactIds here?
		 *
		 * The only way a stale MultiXactId could pose a problem is if a
		 * tuple, having once been multiply-share-locked, is not touched by
		 * any vacuum or attempted lock or deletion for just over 4G MultiXact
		 * creations, and then in the probably-narrow window where its xmax
		 * is again a live MultiXactId, someone tries to lock or delete it.
		 * Even then, another share-lock attempt would work fine.  An
		 * exclusive-lock or delete attempt would face unexpected delay, or
		 * in the very worst case get a deadlock error.  This seems an
		 * extremely low-probability scenario with minimal downside even if
		 * it does happen, so for now we don't do the extra bookkeeping that
		 * would be needed to clean out MultiXactIds.
		 *----------
		 */
  }
/*
	 * Old-style VACUUM FULL is gone, but we have to keep this code as long as
	 * we support having MOVED_OFF/MOVED_IN tuples in the database.
	 */
  if ((tuple -> t_infomask) & (0x4000 | 0x8000)) {
    xid = ((tuple -> t_infomask) & (0x4000 | 0x8000)?tuple -> t_choice . t_heap . t_field3 . t_xvac : ((TransactionId )0));
    if (xid >= ((TransactionId )3) && TransactionIdPrecedes(xid,cutoff_xid)) {
/*
			 * If a MOVED_OFF tuple is not dead, the xvac transaction must
			 * have failed; whereas a non-dead MOVED_IN tuple must mean the
			 * xvac transaction succeeded.
			 */
      if ((tuple -> t_infomask) & 0x4000) {
        do {
          ;
          tuple -> t_choice . t_heap . t_field3 . t_xvac = ((TransactionId )0);
        }while (0);
      }
      else {
        do {
          ;
          tuple -> t_choice . t_heap . t_field3 . t_xvac = ((TransactionId )2);
        }while (0);
      }
/*
			 * Might as well fix the hint bits too; usually XMIN_COMMITTED
			 * will already be set here, but there's a small chance not.
			 */
      ;
      tuple -> t_infomask |= 0x0100;
      changed = ((bool )1);
    }
  }
  return changed;
}
/*
 * heap_tuple_needs_freeze
 *
 * Check to see whether any of the XID fields of a tuple (xmin, xmax, xvac)
 * are older than the specified cutoff XID.  If so, return TRUE.
 *
 * It doesn't matter whether the tuple is alive or dead, we are checking
 * to see if a tuple needs to be removed or frozen to avoid wraparound.
 */

bool heap_tuple_needs_freeze(HeapTupleHeader tuple,TransactionId cutoff_xid,Buffer buf)
{
  TransactionId xid;
  xid = tuple -> t_choice . t_heap . t_xmin;
  if (xid >= ((TransactionId )3) && TransactionIdPrecedes(xid,cutoff_xid)) {
    return (bool )1;
  }
  if (!((tuple -> t_infomask) & 0x1000)) {
    xid = tuple -> t_choice . t_heap . t_xmax;
    if (xid >= ((TransactionId )3) && TransactionIdPrecedes(xid,cutoff_xid)) {
      return (bool )1;
    }
  }
  if ((tuple -> t_infomask) & (0x4000 | 0x8000)) {
    xid = ((tuple -> t_infomask) & (0x4000 | 0x8000)?tuple -> t_choice . t_heap . t_field3 . t_xvac : ((TransactionId )0));
    if (xid >= ((TransactionId )3) && TransactionIdPrecedes(xid,cutoff_xid)) {
      return (bool )1;
    }
  }
  return (bool )0;
}
/* ----------------
 *		heap_markpos	- mark scan position
 * ----------------
 */

void heap_markpos(HeapScanDesc scan)
{
/* Note: no locking manipulations needed */
  if (scan -> rs_ctup . t_data != ((void *)0)) {
    scan -> rs_mctid = scan -> rs_ctup . t_self;
    if (scan -> rs_pageatatime) {
      scan -> rs_mindex = scan -> rs_cindex;
    }
  }
  else {
    ((((void )((bool )1)) , (((((void )((bool )1)) , (&(&scan -> rs_mctid) -> ip_blkid) -> bi_hi = (((BlockNumber )0xFFFFFFFF) >> 16)) , (&(&scan -> rs_mctid) -> ip_blkid) -> bi_lo = (((BlockNumber )0xFFFFFFFF) & 0xffff)))) , (&scan -> rs_mctid) -> ip_posid = ((OffsetNumber )0));
  }
}
/* ----------------
 *		heap_restrpos	- restore position to marked location
 * ----------------
 */

void heap_restrpos(HeapScanDesc scan)
{
/* XXX no amrestrpos checking that ammarkpos called */
  if (!((bool )(((const void *)(&scan -> rs_mctid)) != ((void *)0) && ((&scan -> rs_mctid) -> ip_posid) != 0))) {
    scan -> rs_ctup . t_data = ((void *)0);
/*
		 * unpin scan buffers
		 */
    if ((((void )((bool )1)) , scan -> rs_cbuf != 0)) {
      ReleaseBuffer(scan -> rs_cbuf);
    }
    scan -> rs_cbuf = 0;
    scan -> rs_cblock = ((BlockNumber )0xFFFFFFFF);
    scan -> rs_inited = ((bool )0);
  }
  else {
/*
		 * If we reached end of scan, rs_inited will now be false.	We must
		 * reset it to true to keep heapgettup from doing the wrong thing.
		 */
    scan -> rs_inited = ((bool )1);
    scan -> rs_ctup . t_self = scan -> rs_mctid;
    if (scan -> rs_pageatatime) {
      scan -> rs_cindex = scan -> rs_mindex;
      heapgettup_pagemode(scan,NoMovementScanDirection,0,((void *)0));
/* needn't recheck scan keys */
    }
    else {
      heapgettup(scan,NoMovementScanDirection,0,((void *)0));
    }
/* needn't recheck scan keys */
  }
}
/*
 * If 'tuple' contains any visible XID greater than latestRemovedXid,
 * ratchet forwards latestRemovedXid to the greatest one found.
 * This is used as the basis for generating Hot Standby conflicts, so
 * if a tuple was never visible then removing it should not conflict
 * with queries.
 */

void HeapTupleHeaderAdvanceLatestRemovedXid(HeapTupleHeader tuple,TransactionId *latestRemovedXid)
{
  TransactionId xmin = tuple -> t_choice . t_heap . t_xmin;
  TransactionId xmax = tuple -> t_choice . t_heap . t_xmax;
  TransactionId xvac = (tuple -> t_infomask) & (0x4000 | 0x8000)?tuple -> t_choice . t_heap . t_field3 . t_xvac : ((TransactionId )0);
  if ((tuple -> t_infomask) & (0x4000 | 0x8000)) {
    if (TransactionIdPrecedes( *latestRemovedXid,xvac)) {
       *latestRemovedXid = xvac;
    }
  }
/*
	 * Ignore tuples inserted by an aborted transaction or if the tuple was
	 * updated/deleted by the inserting transaction.
	 *
	 * Look for a committed hint bit, or if no xmin bit is set, check clog.
	 * This needs to work on both master and standby, where it is used to
	 * assess btree delete records.
	 */
  if ((tuple -> t_infomask) & 0x0100 || !((tuple -> t_infomask) & 0x0100) && !((tuple -> t_infomask) & 0x0200) && TransactionIdDidCommit(xmin)) {
    if (xmax != xmin && TransactionIdFollows(xmax, *latestRemovedXid)) {
       *latestRemovedXid = xmax;
    }
  }
/* *latestRemovedXid may still be invalid at end */
}
/*
 * Perform XLogInsert to register a heap cleanup info message. These
 * messages are sent once per VACUUM and are required because
 * of the phasing of removal operations during a lazy VACUUM.
 * see comments for vacuum_log_cleanup_info().
 */

XLogRecPtr log_heap_cleanup_info(RelFileNode rnode,TransactionId latestRemovedXid)
{
  xl_heap_cleanup_info xlrec;
  XLogRecPtr recptr;
  XLogRecData rdata;
  xlrec . node = rnode;
  xlrec . latestRemovedXid = latestRemovedXid;
  rdata . data = ((char *)(&xlrec));
  rdata . len = (sizeof(xl_heap_cleanup_info ));
  rdata . buffer = 0;
  rdata . next = ((void *)0);
  recptr = XLogInsert(9,0x30,&rdata);
  return recptr;
}
/*
 * Perform XLogInsert for a heap-clean operation.  Caller must already
 * have modified the buffer and marked it dirty.
 *
 * Note: prior to Postgres 8.3, the entries in the nowunused[] array were
 * zero-based tuple indexes.  Now they are one-based like other uses
 * of OffsetNumber.
 *
 * We also include latestRemovedXid, which is the greatest XID present in
 * the removed tuples. That allows recovery processing to cancel or wait
 * for long standby queries that can still see these tuples.
 */

XLogRecPtr log_heap_clean(Relation reln,Buffer buffer,OffsetNumber *redirected,int nredirected,OffsetNumber *nowdead,int ndead,OffsetNumber *nowunused,int nunused,TransactionId latestRemovedXid)
{
  xl_heap_clean xlrec;
  uint8 info;
  XLogRecPtr recptr;
  XLogRecData rdata[4];
/* Caller should not call me on a non-WAL-logged relation */
  ;
  xlrec . node = reln -> rd_node;
  xlrec . block = BufferGetBlockNumber(buffer);
  xlrec . latestRemovedXid = latestRemovedXid;
  xlrec . nredirected = nredirected;
  xlrec . ndead = ndead;
  rdata[0] . data = ((char *)(&xlrec));
  rdata[0] . len = (((size_t )(&((xl_heap_clean *)0) -> ndead)) + sizeof(uint16 ));
  rdata[0] . buffer = 0;
  rdata[0] . next = &rdata[1];
/*
	 * The OffsetNumber arrays are not actually in the buffer, but we pretend
	 * that they are.  When XLogInsert stores the whole buffer, the offset
	 * arrays need not be stored too.  Note that even if all three arrays are
	 * empty, we want to expose the buffer as a candidate for whole-page
	 * storage, since this record type implies a defragmentation operation
	 * even if no item pointers changed state.
	 */
  if (nredirected > 0) {
    rdata[1] . data = ((char *)redirected);
    rdata[1] . len = (nredirected * sizeof(OffsetNumber ) * 2);
  }
  else {
    rdata[1] . data = ((void *)0);
    rdata[1] . len = 0;
  }
  rdata[1] . buffer = buffer;
  rdata[1] . buffer_std = ((bool )1);
  rdata[1] . next = &rdata[2];
  if (ndead > 0) {
    rdata[2] . data = ((char *)nowdead);
    rdata[2] . len = (ndead * sizeof(OffsetNumber ));
  }
  else {
    rdata[2] . data = ((void *)0);
    rdata[2] . len = 0;
  }
  rdata[2] . buffer = buffer;
  rdata[2] . buffer_std = ((bool )1);
  rdata[2] . next = &rdata[3];
  if (nunused > 0) {
    rdata[3] . data = ((char *)nowunused);
    rdata[3] . len = (nunused * sizeof(OffsetNumber ));
  }
  else {
    rdata[3] . data = ((void *)0);
    rdata[3] . len = 0;
  }
  rdata[3] . buffer = buffer;
  rdata[3] . buffer_std = ((bool )1);
  rdata[3] . next = ((void *)0);
  info = 0x10;
  recptr = XLogInsert(9,info,rdata);
  return recptr;
}
/*
 * Perform XLogInsert for a heap-freeze operation.	Caller must already
 * have modified the buffer and marked it dirty.
 */

XLogRecPtr log_heap_freeze(Relation reln,Buffer buffer,TransactionId cutoff_xid,OffsetNumber *offsets,int offcnt)
{
  xl_heap_freeze xlrec;
  XLogRecPtr recptr;
  XLogRecData rdata[2];
/* Caller should not call me on a non-WAL-logged relation */
  ;
/* nor when there are no tuples to freeze */
  ;
  xlrec . node = reln -> rd_node;
  xlrec . block = BufferGetBlockNumber(buffer);
  xlrec . cutoff_xid = cutoff_xid;
  rdata[0] . data = ((char *)(&xlrec));
  rdata[0] . len = (((size_t )(&((xl_heap_freeze *)0) -> cutoff_xid)) + sizeof(TransactionId ));
  rdata[0] . buffer = 0;
  rdata[0] . next = &rdata[1];
/*
	 * The tuple-offsets array is not actually in the buffer, but pretend that
	 * it is.  When XLogInsert stores the whole buffer, the offsets array need
	 * not be stored too.
	 */
  rdata[1] . data = ((char *)offsets);
  rdata[1] . len = (offcnt * sizeof(OffsetNumber ));
  rdata[1] . buffer = buffer;
  rdata[1] . buffer_std = ((bool )1);
  rdata[1] . next = ((void *)0);
  recptr = XLogInsert(9,0,rdata);
  return recptr;
}
/*
 * Perform XLogInsert for a heap-visible operation.  'block' is the block
 * being marked all-visible, and vm_buffer is the buffer containing the
 * corresponding visibility map block.	Both should have already been modified
 * and dirtied.
 */

XLogRecPtr log_heap_visible(RelFileNode rnode,BlockNumber block,Buffer vm_buffer,TransactionId cutoff_xid)
{
  xl_heap_visible xlrec;
  XLogRecPtr recptr;
  XLogRecData rdata[2];
  xlrec . node = rnode;
  xlrec . block = block;
  xlrec . cutoff_xid = cutoff_xid;
  rdata[0] . data = ((char *)(&xlrec));
  rdata[0] . len = (((size_t )(&((xl_heap_visible *)0) -> cutoff_xid)) + sizeof(TransactionId ));
  rdata[0] . buffer = 0;
  rdata[0] . next = &rdata[1];
  rdata[1] . data = ((void *)0);
  rdata[1] . len = 0;
  rdata[1] . buffer = vm_buffer;
  rdata[1] . buffer_std = ((bool )0);
  rdata[1] . next = ((void *)0);
  recptr = XLogInsert(9,0x0040,rdata);
  return recptr;
}
/*
 * Perform XLogInsert for a heap-update operation.	Caller must already
 * have modified the buffer(s) and marked them dirty.
 */

static XLogRecPtr log_heap_update(Relation reln,Buffer oldbuf,ItemPointerData from,Buffer newbuf,HeapTuple newtup,bool all_visible_cleared,bool new_all_visible_cleared)
{
  xl_heap_update xlrec;
  xl_heap_header xlhdr;
  uint8 info;
  XLogRecPtr recptr;
  XLogRecData rdata[4];
  Page page = (Page )((((void )((bool )1)) , (newbuf < 0?LocalBufferBlockPointers[-newbuf - 1] : ((Block )(BufferBlocks + ((Size )(newbuf - 1)) * 8192)))));
/* Caller should not call me on a non-WAL-logged relation */
  ;
  if ((newtup -> t_data -> t_infomask2) & 0x8000) {
    info = 0x0040;
  }
  else {
    info = 0x0020;
  }
  xlrec . target . node = reln -> rd_node;
  xlrec . target . tid = from;
  xlrec . all_visible_cleared = all_visible_cleared;
  xlrec . newtid = newtup -> t_self;
  xlrec . new_all_visible_cleared = new_all_visible_cleared;
  rdata[0] . data = ((char *)(&xlrec));
  rdata[0] . len = (((size_t )(&((xl_heap_update *)0) -> new_all_visible_cleared)) + sizeof(bool ));
  rdata[0] . buffer = 0;
  rdata[0] . next = &rdata[1];
  rdata[1] . data = ((void *)0);
  rdata[1] . len = 0;
  rdata[1] . buffer = oldbuf;
  rdata[1] . buffer_std = ((bool )1);
  rdata[1] . next = &rdata[2];
  xlhdr . t_infomask2 = newtup -> t_data -> t_infomask2;
  xlhdr . t_infomask = newtup -> t_data -> t_infomask;
  xlhdr . t_hoff = newtup -> t_data -> t_hoff;
/*
	 * As with insert records, we need not store the rdata[2] segment if we
	 * decide to store the whole buffer instead.
	 */
  rdata[2] . data = ((char *)(&xlhdr));
  rdata[2] . len = (((size_t )(&((xl_heap_header *)0) -> t_hoff)) + sizeof(uint8 ));
  rdata[2] . buffer = newbuf;
  rdata[2] . buffer_std = ((bool )1);
  rdata[2] . next = &rdata[3];
/* PG73FORMAT: write bitmap [+ padding] [+ oid] + data */
  rdata[3] . data = ((char *)(newtup -> t_data)) + ((size_t )(&((HeapTupleHeaderData *)0) -> t_bits));
  rdata[3] . len = ((newtup -> t_len) - ((size_t )(&((HeapTupleHeaderData *)0) -> t_bits)));
  rdata[3] . buffer = newbuf;
  rdata[3] . buffer_std = ((bool )1);
  rdata[3] . next = ((void *)0);
/* If new tuple is the single and first tuple on page... */
  if (((((void )((bool )1)) , (&newtup -> t_self) -> ip_posid)) == ((OffsetNumber )1) && (((((PageHeader )page) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )page) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData ))) == ((OffsetNumber )1)) {
    info |= 0x0080;
    rdata[2] . buffer = rdata[3] . buffer = 0;
  }
  recptr = XLogInsert(10,info,rdata);
  return recptr;
}
/*
 * Perform XLogInsert of a HEAP_NEWPAGE record to WAL. Caller is responsible
 * for writing the page to disk after calling this routine.
 *
 * Note: all current callers build pages in private memory and write them
 * directly to smgr, rather than using bufmgr.	Therefore there is no need
 * to pass a buffer ID to XLogInsert, nor to perform MarkBufferDirty within
 * the critical section.
 *
 * Note: the NEWPAGE log record is used for both heaps and indexes, so do
 * not do anything that assumes we are touching a heap.
 */

XLogRecPtr log_newpage(RelFileNode *rnode,ForkNumber forkNum,BlockNumber blkno,Page page)
{
  xl_heap_newpage xlrec;
  XLogRecPtr recptr;
  XLogRecData rdata[2];
/* NO ELOG(ERROR) from here till newpage op is logged */
  CritSectionCount++;
  xlrec . node =  *rnode;
  xlrec . forknum = forkNum;
  xlrec . blkno = blkno;
  rdata[0] . data = ((char *)(&xlrec));
  rdata[0] . len = (((size_t )(&((xl_heap_newpage *)0) -> blkno)) + sizeof(BlockNumber ));
  rdata[0] . buffer = 0;
  rdata[0] . next = &rdata[1];
  rdata[1] . data = ((char *)page);
  rdata[1] . len = 8192;
  rdata[1] . buffer = 0;
  rdata[1] . next = ((void *)0);
  recptr = XLogInsert(10,0x50,rdata);
/*
	 * The page may be uninitialized. If so, we can't set the LSN and TLI
	 * because that would corrupt the page.
	 */
  if (!((((PageHeader )page) -> pd_upper) == 0)) {
    ((PageHeader )page) -> pd_lsn = recptr;
    ((PageHeader )page) -> pd_tli = ((uint16 )ThisTimeLineID);
  }
  do {
    ;
    CritSectionCount--;
  }while (0);
  return recptr;
}
/*
 * Handles CLEANUP_INFO
 */

static void heap_xlog_cleanup_info(XLogRecPtr lsn,XLogRecord *record)
{
  xl_heap_cleanup_info *xlrec = (xl_heap_cleanup_info *)(((char *)record) + (((intptr_t )(sizeof(XLogRecord ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
  if (standbyState >= STANDBY_SNAPSHOT_PENDING) {
    ResolveRecoveryConflictWithSnapshot(xlrec -> latestRemovedXid,xlrec -> node);
  }
/*
	 * Actual operation is a no-op. Record type exists to provide a means for
	 * conflict processing to occur before we begin index vacuum actions. see
	 * vacuumlazy.c and also comments in btvacuumpage()
	 */
/* Backup blocks are not used in cleanup_info records */
  ;
}
/*
 * Handles HEAP2_CLEAN record type
 */

static void heap_xlog_clean(XLogRecPtr lsn,XLogRecord *record)
{
  xl_heap_clean *xlrec = (xl_heap_clean *)(((char *)record) + (((intptr_t )(sizeof(XLogRecord ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
  Buffer buffer;
  Page page;
  OffsetNumber *end;
  OffsetNumber *redirected;
  OffsetNumber *nowdead;
  OffsetNumber *nowunused;
  int nredirected;
  int ndead;
  int nunused;
  Size freespace;
/*
	 * We're about to remove tuples. In Hot Standby mode, ensure that there's
	 * no queries running for which the removed tuples are still visible.
	 *
	 * Not all HEAP2_CLEAN records remove tuples with xids, so we only want to
	 * conflict on the records that cause MVCC failures for user queries. If
	 * latestRemovedXid is invalid, skip conflict processing.
	 */
  if (standbyState >= STANDBY_SNAPSHOT_PENDING && xlrec -> latestRemovedXid != ((TransactionId )0)) {
    ResolveRecoveryConflictWithSnapshot(xlrec -> latestRemovedXid,xlrec -> node);
  }
/*
	 * If we have a full-page image, restore it (using a cleanup lock) and
	 * we're done.
	 */
  if ((record -> xl_info) & 0x0008 >> 0) {
    (void )(RestoreBackupBlock(lsn,record,0,((bool )1),((bool )0)));
    return ;
  }
  buffer = XLogReadBufferExtended(xlrec -> node,MAIN_FORKNUM,xlrec -> block,RBM_NORMAL);
  if (!((((void )((bool )1)) , buffer != 0))) {
    return ;
  }
  LockBufferForCleanup(buffer);
  page = ((Page )((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192)))))));
  if (lsn . xlogid < ((PageHeader )page) -> pd_lsn . xlogid || lsn . xlogid == ((PageHeader )page) -> pd_lsn . xlogid && lsn . xrecoff <= ((PageHeader )page) -> pd_lsn . xrecoff) {
    UnlockReleaseBuffer(buffer);
    return ;
  }
  nredirected = (xlrec -> nredirected);
  ndead = (xlrec -> ndead);
  end = ((OffsetNumber *)(((char *)xlrec) + record -> xl_len));
  redirected = ((OffsetNumber *)(((char *)xlrec) + (((size_t )(&((xl_heap_clean *)0) -> ndead)) + sizeof(uint16 ))));
  nowdead = redirected + nredirected * 2;
  nowunused = nowdead + ndead;
  nunused = (end - nowunused);
  ;
/* Update all item pointers per the record, and repair fragmentation */
  heap_page_prune_execute(buffer,redirected,nredirected,nowdead,ndead,nowunused,nunused);
/* needed to update FSM below */
  freespace = PageGetHeapFreeSpace(page);
/*
	 * Note: we don't worry about updating the page's prunability hints. At
	 * worst this will cause an extra prune cycle to occur soon.
	 */
  ((PageHeader )page) -> pd_lsn = lsn;
  ((PageHeader )page) -> pd_tli = ((uint16 )ThisTimeLineID);
  MarkBufferDirty(buffer);
  UnlockReleaseBuffer(buffer);
/*
	 * Update the FSM as well.
	 *
	 * XXX: We don't get here if the page was restored from full page image.
	 * We don't bother to update the FSM in that case, it doesn't need to be
	 * totally accurate anyway.
	 */
  XLogRecordPageWithFreeSpace(xlrec -> node,xlrec -> block,freespace);
}

static void heap_xlog_freeze(XLogRecPtr lsn,XLogRecord *record)
{
  xl_heap_freeze *xlrec = (xl_heap_freeze *)(((char *)record) + (((intptr_t )(sizeof(XLogRecord ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
  TransactionId cutoff_xid = xlrec -> cutoff_xid;
  Buffer buffer;
  Page page;
/*
	 * In Hot Standby mode, ensure that there's no queries running which still
	 * consider the frozen xids as running.
	 */
  if (standbyState >= STANDBY_SNAPSHOT_PENDING) {
    ResolveRecoveryConflictWithSnapshot(cutoff_xid,xlrec -> node);
  }
/* If we have a full-page image, restore it and we're done */
  if ((record -> xl_info) & 0x0008 >> 0) {
    (void )(RestoreBackupBlock(lsn,record,0,((bool )0),((bool )0)));
    return ;
  }
  buffer = XLogReadBuffer(xlrec -> node,xlrec -> block,((bool )0));
  if (!((((void )((bool )1)) , buffer != 0))) {
    return ;
  }
  page = ((Page )((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192)))))));
  if (lsn . xlogid < ((PageHeader )page) -> pd_lsn . xlogid || lsn . xlogid == ((PageHeader )page) -> pd_lsn . xlogid && lsn . xrecoff <= ((PageHeader )page) -> pd_lsn . xrecoff) {
    UnlockReleaseBuffer(buffer);
    return ;
  }
  if ((record -> xl_len) > ((size_t )(&((xl_heap_freeze *)0) -> cutoff_xid)) + sizeof(TransactionId )) {
    OffsetNumber *offsets;
    OffsetNumber *offsets_end;
    offsets = ((OffsetNumber *)(((char *)xlrec) + (((size_t )(&((xl_heap_freeze *)0) -> cutoff_xid)) + sizeof(TransactionId ))));
    offsets_end = ((OffsetNumber *)(((char *)xlrec) + record -> xl_len));
    while(offsets < offsets_end){
/* offsets[] entries are one-based */
      ItemId lp = (ItemId )(&((PageHeader )page) -> pd_linp[( *offsets) - 1]);
      HeapTupleHeader tuple = (HeapTupleHeader )(((((void )((bool )1)) , ((void )((bool )1))) , ((Item )(((char *)page) + lp -> lp_off))));
      (void )(heap_freeze_tuple(tuple,cutoff_xid));
      offsets++;
    }
  }
  ((PageHeader )page) -> pd_lsn = lsn;
  ((PageHeader )page) -> pd_tli = ((uint16 )ThisTimeLineID);
  MarkBufferDirty(buffer);
  UnlockReleaseBuffer(buffer);
}
/*
 * Replay XLOG_HEAP2_VISIBLE record.
 *
 * The critical integrity requirement here is that we must never end up with
 * a situation where the visibility map bit is set, and the page-level
 * PD_ALL_VISIBLE bit is clear.  If that were to occur, then a subsequent
 * page modification would fail to clear the visibility map bit.
 */

static void heap_xlog_visible(XLogRecPtr lsn,XLogRecord *record)
{
  xl_heap_visible *xlrec = (xl_heap_visible *)(((char *)record) + (((intptr_t )(sizeof(XLogRecord ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
  Buffer buffer;
  Page page;
/*
	 * If there are any Hot Standby transactions running that have an xmin
	 * horizon old enough that this page isn't all-visible for them, they
	 * might incorrectly decide that an index-only scan can skip a heap fetch.
	 *
	 * NB: It might be better to throw some kind of "soft" conflict here that
	 * forces any index-only scan that is in flight to perform heap fetches,
	 * rather than killing the transaction outright.
	 */
  if (standbyState >= STANDBY_SNAPSHOT_PENDING) {
    ResolveRecoveryConflictWithSnapshot(xlrec -> cutoff_xid,xlrec -> node);
  }
/*
	 * Read the heap page, if it still exists.	If the heap file has been
	 * dropped or truncated later in recovery, we don't need to update the
	 * page, but we'd better still update the visibility map.
	 */
  buffer = XLogReadBufferExtended(xlrec -> node,MAIN_FORKNUM,xlrec -> block,RBM_NORMAL);
  if ((((void )((bool )1)) , buffer != 0)) {
    LockBuffer(buffer,2);
    page = ((Page )((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192)))))));
/*
		 * We don't bump the LSN of the heap page when setting the visibility
		 * map bit, because that would generate an unworkable volume of
		 * full-page writes.  This exposes us to torn page hazards, but since
		 * we're not inspecting the existing page contents in any way, we
		 * don't care.
		 *
		 * However, all operations that clear the visibility map bit *do* bump
		 * the LSN, and those operations will only be replayed if the XLOG LSN
		 * follows the page LSN.  Thus, if the page LSN has advanced past our
		 * XLOG record's LSN, we mustn't mark the page all-visible, because
		 * the subsequent update won't be replayed to clear the flag.
		 */
    if (!(lsn . xlogid < ((PageHeader )page) -> pd_lsn . xlogid || lsn . xlogid == ((PageHeader )page) -> pd_lsn . xlogid && lsn . xrecoff <= ((PageHeader )page) -> pd_lsn . xrecoff)) {
      ((PageHeader )page) -> pd_flags |= 0x0004;
      MarkBufferDirty(buffer);
    }
/* Done with heap page. */
    UnlockReleaseBuffer(buffer);
  }
/*
	 * Even if we skipped the heap page update due to the LSN interlock, it's
	 * still safe to update the visibility map.  Any WAL record that clears
	 * the visibility map bit does so before checking the page LSN, so any
	 * bits that need to be cleared will still be cleared.
	 */
  if ((record -> xl_info) & 0x0008 >> 0) {
    (void )(RestoreBackupBlock(lsn,record,0,((bool )0),((bool )0)));
  }
  else {
    Relation reln;
    Buffer vmbuffer = 0;
    reln = CreateFakeRelcacheEntry(xlrec -> node);
    visibilitymap_pin(reln,xlrec -> block,&vmbuffer);
/*
		 * Don't set the bit if replay has already passed this point.
		 *
		 * It might be safe to do this unconditionally; if replay has passed
		 * this point, we'll replay at least as far this time as we did
		 * before, and if this bit needs to be cleared, the record responsible
		 * for doing so should be again replayed, and clear it.  For right
		 * now, out of an abundance of conservatism, we use the same test here
		 * we did for the heap page.  If this results in a dropped bit, no
		 * real harm is done; and the next VACUUM will fix it.
		 */
    if (!(lsn . xlogid < ((PageHeader )((Page )((((void )((bool )1)) , (vmbuffer < 0?LocalBufferBlockPointers[-vmbuffer - 1] : ((Block )(BufferBlocks + ((Size )(vmbuffer - 1)) * 8192))))))) -> pd_lsn . xlogid || lsn . xlogid == ((PageHeader )((Page )((((void )((bool )1)) , (vmbuffer < 0?LocalBufferBlockPointers[-vmbuffer - 1] : ((Block )(BufferBlocks + ((Size )(vmbuffer - 1)) * 8192))))))) -> pd_lsn . xlogid && lsn . xrecoff <= ((PageHeader )((Page )((((void )((bool )1)) , (vmbuffer < 0?LocalBufferBlockPointers[-vmbuffer - 1] : ((Block )(BufferBlocks + ((Size )(vmbuffer - 1)) * 8192))))))) -> pd_lsn . xrecoff)) {
      visibilitymap_set(reln,xlrec -> block,lsn,vmbuffer,xlrec -> cutoff_xid);
    }
    ReleaseBuffer(vmbuffer);
    FreeFakeRelcacheEntry(reln);
  }
}

static void heap_xlog_newpage(XLogRecPtr lsn,XLogRecord *record)
{
  xl_heap_newpage *xlrec = (xl_heap_newpage *)(((char *)record) + (((intptr_t )(sizeof(XLogRecord ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
  Buffer buffer;
  Page page;
/* Backup blocks are not used in newpage records */
  ;
/*
	 * Note: the NEWPAGE log record is used for both heaps and indexes, so do
	 * not do anything that assumes we are touching a heap.
	 */
  buffer = XLogReadBufferExtended(xlrec -> node,xlrec -> forknum,xlrec -> blkno,RBM_ZERO);
  ;
  LockBuffer(buffer,2);
  page = ((Page )((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192)))))));
  ;
  memcpy(page,(((char *)xlrec) + (((size_t )(&((xl_heap_newpage *)0) -> blkno)) + sizeof(BlockNumber ))),8192);
/*
	 * The page may be uninitialized. If so, we can't set the LSN and TLI
	 * because that would corrupt the page.
	 */
  if (!((((PageHeader )page) -> pd_upper) == 0)) {
    ((PageHeader )page) -> pd_lsn = lsn;
    ((PageHeader )page) -> pd_tli = ((uint16 )ThisTimeLineID);
  }
  MarkBufferDirty(buffer);
  UnlockReleaseBuffer(buffer);
}

static void heap_xlog_delete(XLogRecPtr lsn,XLogRecord *record)
{
  xl_heap_delete *xlrec = (xl_heap_delete *)(((char *)record) + (((intptr_t )(sizeof(XLogRecord ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
  Buffer buffer;
  Page page;
  OffsetNumber offnum;
  ItemId lp = ((void *)0);
  HeapTupleHeader htup;
  BlockNumber blkno;
  blkno = ((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&(&xlrec -> target . tid) -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&(&xlrec -> target . tid) -> ip_blkid) -> bi_lo))))))));
/*
	 * The visibility map may need to be fixed even if the heap page is
	 * already up-to-date.
	 */
  if (xlrec -> all_visible_cleared) {
    Relation reln = CreateFakeRelcacheEntry(xlrec -> target . node);
    Buffer vmbuffer = 0;
    visibilitymap_pin(reln,blkno,&vmbuffer);
    visibilitymap_clear(reln,blkno,vmbuffer);
    ReleaseBuffer(vmbuffer);
    FreeFakeRelcacheEntry(reln);
  }
/* If we have a full-page image, restore it and we're done */
  if ((record -> xl_info) & 0x0008 >> 0) {
    (void )(RestoreBackupBlock(lsn,record,0,((bool )0),((bool )0)));
    return ;
  }
  buffer = XLogReadBuffer(xlrec -> target . node,blkno,((bool )0));
  if (!((((void )((bool )1)) , buffer != 0))) {
    return ;
  }
  page = ((Page )((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192)))))));
/* changes are applied */
  if (lsn . xlogid < ((PageHeader )page) -> pd_lsn . xlogid || lsn . xlogid == ((PageHeader )page) -> pd_lsn . xlogid && lsn . xrecoff <= ((PageHeader )page) -> pd_lsn . xrecoff) {
    UnlockReleaseBuffer(buffer);
    return ;
  }
  offnum = ((((void )((bool )1)) , (&xlrec -> target . tid) -> ip_posid));
  if ((((((PageHeader )page) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )page) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData ))) >= offnum) {
    lp = ((ItemId )(&((PageHeader )page) -> pd_linp[offnum - 1]));
  }
  if ((((((PageHeader )page) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )page) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData ))) < offnum || !((lp -> lp_flags) == 1)) {
    (elog_start("heapam.c",4912,__func__) , elog_finish(22,"heap_delete_redo: invalid lp"));
  }
  htup = ((HeapTupleHeader )(((((void )((bool )1)) , ((void )((bool )1))) , ((Item )(((char *)page) + lp -> lp_off)))));
  htup -> t_infomask &= ~(0x0400 | 0x0800 | 0x1000 | (0x0040 | 0x0080) | (0x4000 | 0x8000));
  htup -> t_infomask2 &= ~0x4000;
  htup -> t_choice . t_heap . t_xmax = record -> xl_xid;
  do {
    ;
    htup -> t_choice . t_heap . t_field3 . t_cid = ((CommandId )0);
    if ((bool )0) {
      htup -> t_infomask |= 0x0020;
    }
    else {
      htup -> t_infomask &= ~0x0020;
    }
  }while (0);
/* Mark the page as a candidate for pruning */
  do {
    ;
    if (!(((PageHeader )page) -> pd_prune_xid != ((TransactionId )0)) || TransactionIdPrecedes(record -> xl_xid,((PageHeader )page) -> pd_prune_xid)) {
      ((PageHeader )page) -> pd_prune_xid = record -> xl_xid;
    }
  }while (0);
  if (xlrec -> all_visible_cleared) {
    ((PageHeader )page) -> pd_flags &= ~0x0004;
  }
/* Make sure there is no forward chain link in t_ctid */
  htup -> t_ctid = xlrec -> target . tid;
  ((PageHeader )page) -> pd_lsn = lsn;
  ((PageHeader )page) -> pd_tli = ((uint16 )ThisTimeLineID);
  MarkBufferDirty(buffer);
  UnlockReleaseBuffer(buffer);
}

static void heap_xlog_insert(XLogRecPtr lsn,XLogRecord *record)
{
  xl_heap_insert *xlrec = (xl_heap_insert *)(((char *)record) + (((intptr_t )(sizeof(XLogRecord ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
  Buffer buffer;
  Page page;
  OffsetNumber offnum;
  struct {
  HeapTupleHeaderData hdr;
  char data[8192 - (((intptr_t )(((size_t )(&((PageHeaderData *)0) -> pd_linp)) + sizeof(ItemIdData ))) + (8 - 1) & ~((intptr_t )(8 - 1)))];}tbuf;
  HeapTupleHeader htup;
  xl_heap_header xlhdr;
  uint32 newlen;
  Size freespace;
  BlockNumber blkno;
  blkno = ((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&(&xlrec -> target . tid) -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&(&xlrec -> target . tid) -> ip_blkid) -> bi_lo))))))));
/*
	 * The visibility map may need to be fixed even if the heap page is
	 * already up-to-date.
	 */
  if (xlrec -> all_visible_cleared) {
    Relation reln = CreateFakeRelcacheEntry(xlrec -> target . node);
    Buffer vmbuffer = 0;
    visibilitymap_pin(reln,blkno,&vmbuffer);
    visibilitymap_clear(reln,blkno,vmbuffer);
    ReleaseBuffer(vmbuffer);
    FreeFakeRelcacheEntry(reln);
  }
/* If we have a full-page image, restore it and we're done */
  if ((record -> xl_info) & 0x0008 >> 0) {
    (void )(RestoreBackupBlock(lsn,record,0,((bool )0),((bool )0)));
    return ;
  }
  if ((record -> xl_info) & 0x0080) {
    buffer = XLogReadBuffer(xlrec -> target . node,blkno,((bool )1));
    ;
    page = ((Page )((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192)))))));
    PageInit(page,((((void )((bool )1)) , ((Size )8192))),0);
  }
  else {
    buffer = XLogReadBuffer(xlrec -> target . node,blkno,((bool )0));
    if (!((((void )((bool )1)) , buffer != 0))) {
      return ;
    }
    page = ((Page )((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192)))))));
/* changes are applied */
    if (lsn . xlogid < ((PageHeader )page) -> pd_lsn . xlogid || lsn . xlogid == ((PageHeader )page) -> pd_lsn . xlogid && lsn . xrecoff <= ((PageHeader )page) -> pd_lsn . xrecoff) {
      UnlockReleaseBuffer(buffer);
      return ;
    }
  }
  offnum = ((((void )((bool )1)) , (&xlrec -> target . tid) -> ip_posid));
  if ((((((PageHeader )page) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )page) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData ))) + 1 < offnum) {
    (elog_start("heapam.c",5005,__func__) , elog_finish(22,"heap_insert_redo: invalid max offset number"));
  }
  newlen = ((record -> xl_len) - (((size_t )(&((xl_heap_insert *)0) -> all_visible_cleared)) + sizeof(bool )) - (((size_t )(&((xl_heap_header *)0) -> t_hoff)) + sizeof(uint8 )));
  ;
  memcpy(((char *)(&xlhdr)),(((char *)xlrec) + (((size_t )(&((xl_heap_insert *)0) -> all_visible_cleared)) + sizeof(bool ))),((size_t )(&((xl_heap_header *)0) -> t_hoff)) + sizeof(uint8 ));
  htup = &tbuf . hdr;
  do {
    void *_vstart = (void *)((char *)htup);
    int _val = 0;
    Size _len = sizeof(HeapTupleHeaderData );
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
/* PG73FORMAT: get bitmap [+ padding] [+ oid] + data */
  memcpy((((char *)htup) + ((size_t )(&((HeapTupleHeaderData *)0) -> t_bits))),(((char *)xlrec) + (((size_t )(&((xl_heap_insert *)0) -> all_visible_cleared)) + sizeof(bool )) + (((size_t )(&((xl_heap_header *)0) -> t_hoff)) + sizeof(uint8 ))),newlen);
  newlen += ((size_t )(&((HeapTupleHeaderData *)0) -> t_bits));
  htup -> t_infomask2 = xlhdr . t_infomask2;
  htup -> t_infomask = xlhdr . t_infomask;
  htup -> t_hoff = xlhdr . t_hoff;
  htup -> t_choice . t_heap . t_xmin = record -> xl_xid;
  do {
    ;
    htup -> t_choice . t_heap . t_field3 . t_cid = ((CommandId )0);
    htup -> t_infomask &= ~0x0020;
  }while (0);
  htup -> t_ctid = xlrec -> target . tid;
  offnum = PageAddItem(page,((Item )htup),newlen,offnum,((bool )1),((bool )1));
  if (offnum == ((OffsetNumber )0)) {
    (elog_start("heapam.c",5028,__func__) , elog_finish(22,"heap_insert_redo: failed to add tuple"));
  }
/* needed to update FSM below */
  freespace = PageGetHeapFreeSpace(page);
  ((PageHeader )page) -> pd_lsn = lsn;
  ((PageHeader )page) -> pd_tli = ((uint16 )ThisTimeLineID);
  if (xlrec -> all_visible_cleared) {
    ((PageHeader )page) -> pd_flags &= ~0x0004;
  }
  MarkBufferDirty(buffer);
  UnlockReleaseBuffer(buffer);
/*
	 * If the page is running low on free space, update the FSM as well.
	 * Arbitrarily, our definition of "low" is less than 20%. We can't do much
	 * better than that without knowing the fill-factor for the table.
	 *
	 * XXX: We don't get here if the page was restored from full page image.
	 * We don't bother to update the FSM in that case, it doesn't need to be
	 * totally accurate anyway.
	 */
  if (freespace < (8192 / 5)) {
    XLogRecordPageWithFreeSpace(xlrec -> target . node,blkno,freespace);
  }
}
/*
 * Handles MULTI_INSERT record type.
 */

static void heap_xlog_multi_insert(XLogRecPtr lsn,XLogRecord *record)
{
  char *recdata = ((char *)record) + (((intptr_t )(sizeof(XLogRecord ))) + (8 - 1) & ~((intptr_t )(8 - 1)));
  xl_heap_multi_insert *xlrec;
  Buffer buffer;
  Page page;
  struct {
  HeapTupleHeaderData hdr;
  char data[8192 - (((intptr_t )(((size_t )(&((PageHeaderData *)0) -> pd_linp)) + sizeof(ItemIdData ))) + (8 - 1) & ~((intptr_t )(8 - 1)))];}tbuf;
  HeapTupleHeader htup;
  uint32 newlen;
  Size freespace;
  BlockNumber blkno;
  int i;
  bool isinit = (((record -> xl_info) & 0x0080) != 0);
/*
	 * Insertion doesn't overwrite MVCC data, so no conflict processing is
	 * required.
	 */
  xlrec = ((xl_heap_multi_insert *)recdata);
  recdata += ((size_t )(&((xl_heap_multi_insert *)0) -> offsets));
/*
	 * If we're reinitializing the page, the tuples are stored in order from
	 * FirstOffsetNumber. Otherwise there's an array of offsets in the WAL
	 * record.
	 */
  if (!isinit) {
    recdata += sizeof(OffsetNumber ) * (xlrec -> ntuples);
  }
  blkno = xlrec -> blkno;
/*
	 * The visibility map may need to be fixed even if the heap page is
	 * already up-to-date.
	 */
  if (xlrec -> all_visible_cleared) {
    Relation reln = CreateFakeRelcacheEntry(xlrec -> node);
    Buffer vmbuffer = 0;
    visibilitymap_pin(reln,blkno,&vmbuffer);
    visibilitymap_clear(reln,blkno,vmbuffer);
    ReleaseBuffer(vmbuffer);
    FreeFakeRelcacheEntry(reln);
  }
/* If we have a full-page image, restore it and we're done */
  if ((record -> xl_info) & 0x0008 >> 0) {
    (void )(RestoreBackupBlock(lsn,record,0,((bool )0),((bool )0)));
    return ;
  }
  if (isinit) {
    buffer = XLogReadBuffer(xlrec -> node,blkno,((bool )1));
    ;
    page = ((Page )((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192)))))));
    PageInit(page,((((void )((bool )1)) , ((Size )8192))),0);
  }
  else {
    buffer = XLogReadBuffer(xlrec -> node,blkno,((bool )0));
    if (!((((void )((bool )1)) , buffer != 0))) {
      return ;
    }
    page = ((Page )((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192)))))));
/* changes are applied */
    if (lsn . xlogid < ((PageHeader )page) -> pd_lsn . xlogid || lsn . xlogid == ((PageHeader )page) -> pd_lsn . xlogid && lsn . xrecoff <= ((PageHeader )page) -> pd_lsn . xrecoff) {
      UnlockReleaseBuffer(buffer);
      return ;
    }
  }
  for (i = 0; i < (xlrec -> ntuples); i++) {
    OffsetNumber offnum;
    xl_multi_insert_tuple *xlhdr;
    if (isinit) {
      offnum = (((OffsetNumber )1) + i);
    }
    else {
      offnum = xlrec -> offsets[i];
    }
    if ((((((PageHeader )page) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )page) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData ))) + 1 < offnum) {
      (elog_start("heapam.c",5148,__func__) , elog_finish(22,"heap_multi_insert_redo: invalid max offset number"));
    }
    xlhdr = ((xl_multi_insert_tuple *)(((intptr_t )recdata) + (2 - 1) & ~((intptr_t )(2 - 1))));
    recdata = ((char *)xlhdr) + (((size_t )(&((xl_multi_insert_tuple *)0) -> t_hoff)) + sizeof(uint8 ));
    newlen = (xlhdr -> datalen);
    ;
    htup = &tbuf . hdr;
    do {
      void *_vstart = (void *)((char *)htup);
      int _val = 0;
      Size _len = sizeof(HeapTupleHeaderData );
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
/* PG73FORMAT: get bitmap [+ padding] [+ oid] + data */
    memcpy((((char *)htup) + ((size_t )(&((HeapTupleHeaderData *)0) -> t_bits))),((char *)recdata),newlen);
    recdata += newlen;
    newlen += ((size_t )(&((HeapTupleHeaderData *)0) -> t_bits));
    htup -> t_infomask2 = xlhdr -> t_infomask2;
    htup -> t_infomask = xlhdr -> t_infomask;
    htup -> t_hoff = xlhdr -> t_hoff;
    htup -> t_choice . t_heap . t_xmin = record -> xl_xid;
    do {
      ;
      htup -> t_choice . t_heap . t_field3 . t_cid = ((CommandId )0);
      htup -> t_infomask &= ~0x0020;
    }while (0);
    (((void )((bool )1)) , (((((void )((bool )1)) , (&(&htup -> t_ctid) -> ip_blkid) -> bi_hi = (blkno >> 16)) , (&(&htup -> t_ctid) -> ip_blkid) -> bi_lo = (blkno & 0xffff))));
    (((void )((bool )1)) , (&htup -> t_ctid) -> ip_posid = offnum);
    offnum = PageAddItem(page,((Item )htup),newlen,offnum,((bool )1),((bool )1));
    if (offnum == ((OffsetNumber )0)) {
      (elog_start("heapam.c",5174,__func__) , elog_finish(22,"heap_multi_insert_redo: failed to add tuple"));
    }
  }
/* needed to update FSM below */
  freespace = PageGetHeapFreeSpace(page);
  ((PageHeader )page) -> pd_lsn = lsn;
  ((PageHeader )page) -> pd_tli = ((uint16 )ThisTimeLineID);
  if (xlrec -> all_visible_cleared) {
    ((PageHeader )page) -> pd_flags &= ~0x0004;
  }
  MarkBufferDirty(buffer);
  UnlockReleaseBuffer(buffer);
/*
	 * If the page is running low on free space, update the FSM as well.
	 * Arbitrarily, our definition of "low" is less than 20%. We can't do much
	 * better than that without knowing the fill-factor for the table.
	 *
	 * XXX: We don't get here if the page was restored from full page image.
	 * We don't bother to update the FSM in that case, it doesn't need to be
	 * totally accurate anyway.
	 */
  if (freespace < (8192 / 5)) {
    XLogRecordPageWithFreeSpace(xlrec -> node,blkno,freespace);
  }
}
/*
 * Handles UPDATE and HOT_UPDATE
 */

static void heap_xlog_update(XLogRecPtr lsn,XLogRecord *record,bool hot_update)
{
  xl_heap_update *xlrec = (xl_heap_update *)(((char *)record) + (((intptr_t )(sizeof(XLogRecord ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
  bool samepage = (((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&(&xlrec -> newtid) -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&(&xlrec -> newtid) -> ip_blkid) -> bi_lo)))))))) == ((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&(&xlrec -> target . tid) -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&(&xlrec -> target . tid) -> ip_blkid) -> bi_lo)))))))));
  Buffer obuffer;
  Buffer nbuffer;
  Page page;
  OffsetNumber offnum;
  ItemId lp = ((void *)0);
  HeapTupleHeader htup;
  struct {
  HeapTupleHeaderData hdr;
  char data[8192 - (((intptr_t )(((size_t )(&((PageHeaderData *)0) -> pd_linp)) + sizeof(ItemIdData ))) + (8 - 1) & ~((intptr_t )(8 - 1)))];}tbuf;
  xl_heap_header xlhdr;
  int hsize;
  uint32 newlen;
  Size freespace;
/*
	 * The visibility map may need to be fixed even if the heap page is
	 * already up-to-date.
	 */
  if (xlrec -> all_visible_cleared) {
    Relation reln = CreateFakeRelcacheEntry(xlrec -> target . node);
    BlockNumber block = (((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&(&xlrec -> target . tid) -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&(&xlrec -> target . tid) -> ip_blkid) -> bi_lo)))))));
    Buffer vmbuffer = 0;
    visibilitymap_pin(reln,block,&vmbuffer);
    visibilitymap_clear(reln,block,vmbuffer);
    ReleaseBuffer(vmbuffer);
    FreeFakeRelcacheEntry(reln);
  }
/*
	 * In normal operation, it is important to lock the two pages in
	 * page-number order, to avoid possible deadlocks against other update
	 * operations going the other way.	However, during WAL replay there can
	 * be no other update happening, so we don't need to worry about that. But
	 * we *do* need to worry that we don't expose an inconsistent state to Hot
	 * Standby queries --- so the original page can't be unlocked before we've
	 * added the new tuple to the new page.
	 */
  if ((record -> xl_info) & 0x0008 >> 0) {
    obuffer = RestoreBackupBlock(lsn,record,0,((bool )0),((bool )1));
    if (samepage) {
/* backup block covered both changes, so we're done */
      UnlockReleaseBuffer(obuffer);
      return ;
    }
    goto newt;
  }
/* Deal with old tuple version */
  obuffer = XLogReadBuffer(xlrec -> target . node,((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&(&xlrec -> target . tid) -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&(&xlrec -> target . tid) -> ip_blkid) -> bi_lo)))))))),((bool )0));
  if (!((((void )((bool )1)) , obuffer != 0))) {
    goto newt;
  }
  page = ((Page )((Page )((((void )((bool )1)) , (obuffer < 0?LocalBufferBlockPointers[-obuffer - 1] : ((Block )(BufferBlocks + ((Size )(obuffer - 1)) * 8192)))))));
/* changes are applied */
  if (lsn . xlogid < ((PageHeader )page) -> pd_lsn . xlogid || lsn . xlogid == ((PageHeader )page) -> pd_lsn . xlogid && lsn . xrecoff <= ((PageHeader )page) -> pd_lsn . xrecoff) {
    if (samepage) {
      UnlockReleaseBuffer(obuffer);
      return ;
    }
    goto newt;
  }
  offnum = ((((void )((bool )1)) , (&xlrec -> target . tid) -> ip_posid));
  if ((((((PageHeader )page) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )page) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData ))) >= offnum) {
    lp = ((ItemId )(&((PageHeader )page) -> pd_linp[offnum - 1]));
  }
  if ((((((PageHeader )page) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )page) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData ))) < offnum || !((lp -> lp_flags) == 1)) {
    (elog_start("heapam.c",5288,__func__) , elog_finish(22,"heap_update_redo: invalid lp"));
  }
  htup = ((HeapTupleHeader )(((((void )((bool )1)) , ((void )((bool )1))) , ((Item )(((char *)page) + lp -> lp_off)))));
  htup -> t_infomask &= ~(0x0400 | 0x0800 | 0x1000 | (0x0040 | 0x0080) | (0x4000 | 0x8000));
  if (hot_update) {
    htup -> t_infomask2 |= 0x4000;
  }
  else {
    htup -> t_infomask2 &= ~0x4000;
  }
  htup -> t_choice . t_heap . t_xmax = record -> xl_xid;
  do {
    ;
    htup -> t_choice . t_heap . t_field3 . t_cid = ((CommandId )0);
    if ((bool )0) {
      htup -> t_infomask |= 0x0020;
    }
    else {
      htup -> t_infomask &= ~0x0020;
    }
  }while (0);
/* Set forward chain link in t_ctid */
  htup -> t_ctid = xlrec -> newtid;
/* Mark the page as a candidate for pruning */
  do {
    ;
    if (!(((PageHeader )page) -> pd_prune_xid != ((TransactionId )0)) || TransactionIdPrecedes(record -> xl_xid,((PageHeader )page) -> pd_prune_xid)) {
      ((PageHeader )page) -> pd_prune_xid = record -> xl_xid;
    }
  }while (0);
  if (xlrec -> all_visible_cleared) {
    ((PageHeader )page) -> pd_flags &= ~0x0004;
  }
/*
	 * this test is ugly, but necessary to avoid thinking that insert change
	 * is already applied
	 */
  if (samepage) {
    nbuffer = obuffer;
    goto newsame;
  }
  ((PageHeader )page) -> pd_lsn = lsn;
  ((PageHeader )page) -> pd_tli = ((uint16 )ThisTimeLineID);
  MarkBufferDirty(obuffer);
/* Deal with new tuple */
  newt:
  ;
/*
	 * The visibility map may need to be fixed even if the heap page is
	 * already up-to-date.
	 */
  if (xlrec -> new_all_visible_cleared) {
    Relation reln = CreateFakeRelcacheEntry(xlrec -> target . node);
    BlockNumber block = (((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&(&xlrec -> newtid) -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&(&xlrec -> newtid) -> ip_blkid) -> bi_lo)))))));
    Buffer vmbuffer = 0;
    visibilitymap_pin(reln,block,&vmbuffer);
    visibilitymap_clear(reln,block,vmbuffer);
    ReleaseBuffer(vmbuffer);
    FreeFakeRelcacheEntry(reln);
  }
  if ((record -> xl_info) & 0x0008 >> 1) {
    (void )(RestoreBackupBlock(lsn,record,1,((bool )0),((bool )0)));
    if ((((void )((bool )1)) , obuffer != 0)) {
      UnlockReleaseBuffer(obuffer);
    }
    return ;
  }
  if ((record -> xl_info) & 0x0080) {
    nbuffer = XLogReadBuffer(xlrec -> target . node,((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&(&xlrec -> newtid) -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&(&xlrec -> newtid) -> ip_blkid) -> bi_lo)))))))),((bool )1));
    ;
    page = ((Page )((Page )((((void )((bool )1)) , (nbuffer < 0?LocalBufferBlockPointers[-nbuffer - 1] : ((Block )(BufferBlocks + ((Size )(nbuffer - 1)) * 8192)))))));
    PageInit(page,((((void )((bool )1)) , ((Size )8192))),0);
  }
  else {
    nbuffer = XLogReadBuffer(xlrec -> target . node,((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&(&xlrec -> newtid) -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&(&xlrec -> newtid) -> ip_blkid) -> bi_lo)))))))),((bool )0));
    if (!((((void )((bool )1)) , nbuffer != 0))) {
      if ((((void )((bool )1)) , obuffer != 0)) {
        UnlockReleaseBuffer(obuffer);
      }
      return ;
    }
    page = ((Page )((Page )((((void )((bool )1)) , (nbuffer < 0?LocalBufferBlockPointers[-nbuffer - 1] : ((Block )(BufferBlocks + ((Size )(nbuffer - 1)) * 8192)))))));
/* changes are applied */
    if (lsn . xlogid < ((PageHeader )page) -> pd_lsn . xlogid || lsn . xlogid == ((PageHeader )page) -> pd_lsn . xlogid && lsn . xrecoff <= ((PageHeader )page) -> pd_lsn . xrecoff) {
      UnlockReleaseBuffer(nbuffer);
      if ((((void )((bool )1)) , obuffer != 0)) {
        UnlockReleaseBuffer(obuffer);
      }
      return ;
    }
  }
  newsame:
  ;
  offnum = ((((void )((bool )1)) , (&xlrec -> newtid) -> ip_posid));
  if ((((((PageHeader )page) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )page) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData ))) + 1 < offnum) {
    (elog_start("heapam.c",5390,__func__) , elog_finish(22,"heap_update_redo: invalid max offset number"));
  }
  hsize = (((size_t )(&((xl_heap_update *)0) -> new_all_visible_cleared)) + sizeof(bool ) + (((size_t )(&((xl_heap_header *)0) -> t_hoff)) + sizeof(uint8 )));
  newlen = record -> xl_len - hsize;
  ;
  memcpy(((char *)(&xlhdr)),(((char *)xlrec) + (((size_t )(&((xl_heap_update *)0) -> new_all_visible_cleared)) + sizeof(bool ))),((size_t )(&((xl_heap_header *)0) -> t_hoff)) + sizeof(uint8 ));
  htup = &tbuf . hdr;
  do {
    void *_vstart = (void *)((char *)htup);
    int _val = 0;
    Size _len = sizeof(HeapTupleHeaderData );
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
/* PG73FORMAT: get bitmap [+ padding] [+ oid] + data */
  memcpy((((char *)htup) + ((size_t )(&((HeapTupleHeaderData *)0) -> t_bits))),(((char *)xlrec) + hsize),newlen);
  newlen += ((size_t )(&((HeapTupleHeaderData *)0) -> t_bits));
  htup -> t_infomask2 = xlhdr . t_infomask2;
  htup -> t_infomask = xlhdr . t_infomask;
  htup -> t_hoff = xlhdr . t_hoff;
  htup -> t_choice . t_heap . t_xmin = record -> xl_xid;
  do {
    ;
    htup -> t_choice . t_heap . t_field3 . t_cid = ((CommandId )0);
    htup -> t_infomask &= ~0x0020;
  }while (0);
/* Make sure there is no forward chain link in t_ctid */
  htup -> t_ctid = xlrec -> newtid;
  offnum = PageAddItem(page,((Item )htup),newlen,offnum,((bool )1),((bool )1));
  if (offnum == ((OffsetNumber )0)) {
    (elog_start("heapam.c",5417,__func__) , elog_finish(22,"heap_update_redo: failed to add tuple"));
  }
  if (xlrec -> new_all_visible_cleared) {
    ((PageHeader )page) -> pd_flags &= ~0x0004;
  }
/* needed to update FSM below */
  freespace = PageGetHeapFreeSpace(page);
  ((PageHeader )page) -> pd_lsn = lsn;
  ((PageHeader )page) -> pd_tli = ((uint16 )ThisTimeLineID);
  MarkBufferDirty(nbuffer);
  UnlockReleaseBuffer(nbuffer);
  if (((((void )((bool )1)) , obuffer != 0)) && obuffer != nbuffer) {
    UnlockReleaseBuffer(obuffer);
  }
/*
	 * If the new page is running low on free space, update the FSM as well.
	 * Arbitrarily, our definition of "low" is less than 20%. We can't do much
	 * better than that without knowing the fill-factor for the table.
	 *
	 * However, don't update the FSM on HOT updates, because after crash
	 * recovery, either the old or the new tuple will certainly be dead and
	 * prunable. After pruning, the page will have roughly as much free space
	 * as it did before the update, assuming the new tuple is about the same
	 * size as the old one.
	 *
	 * XXX: We don't get here if the page was restored from full page image.
	 * We don't bother to update the FSM in that case, it doesn't need to be
	 * totally accurate anyway.
	 */
  if (!hot_update && freespace < (8192 / 5)) {
    XLogRecordPageWithFreeSpace(xlrec -> target . node,((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&(&xlrec -> newtid) -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&(&xlrec -> newtid) -> ip_blkid) -> bi_lo)))))))),freespace);
  }
}

static void heap_xlog_lock(XLogRecPtr lsn,XLogRecord *record)
{
  xl_heap_lock *xlrec = (xl_heap_lock *)(((char *)record) + (((intptr_t )(sizeof(XLogRecord ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
  Buffer buffer;
  Page page;
  OffsetNumber offnum;
  ItemId lp = ((void *)0);
  HeapTupleHeader htup;
/* If we have a full-page image, restore it and we're done */
  if ((record -> xl_info) & 0x0008 >> 0) {
    (void )(RestoreBackupBlock(lsn,record,0,((bool )0),((bool )0)));
    return ;
  }
  buffer = XLogReadBuffer(xlrec -> target . node,((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&(&xlrec -> target . tid) -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&(&xlrec -> target . tid) -> ip_blkid) -> bi_lo)))))))),((bool )0));
  if (!((((void )((bool )1)) , buffer != 0))) {
    return ;
  }
  page = ((Page )((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192)))))));
/* changes are applied */
  if (lsn . xlogid < ((PageHeader )page) -> pd_lsn . xlogid || lsn . xlogid == ((PageHeader )page) -> pd_lsn . xlogid && lsn . xrecoff <= ((PageHeader )page) -> pd_lsn . xrecoff) {
    UnlockReleaseBuffer(buffer);
    return ;
  }
  offnum = ((((void )((bool )1)) , (&xlrec -> target . tid) -> ip_posid));
  if ((((((PageHeader )page) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )page) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData ))) >= offnum) {
    lp = ((ItemId )(&((PageHeader )page) -> pd_linp[offnum - 1]));
  }
  if ((((((PageHeader )page) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )page) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData ))) < offnum || !((lp -> lp_flags) == 1)) {
    (elog_start("heapam.c",5488,__func__) , elog_finish(22,"heap_lock_redo: invalid lp"));
  }
  htup = ((HeapTupleHeader )(((((void )((bool )1)) , ((void )((bool )1))) , ((Item )(((char *)page) + lp -> lp_off)))));
  htup -> t_infomask &= ~(0x0400 | 0x0800 | 0x1000 | (0x0040 | 0x0080) | (0x4000 | 0x8000));
  if (xlrec -> xid_is_mxact) {
    htup -> t_infomask |= 0x1000;
  }
  if (xlrec -> shared_lock) {
    htup -> t_infomask |= 0x0080;
  }
  else {
    htup -> t_infomask |= 0x0040;
  }
  htup -> t_infomask2 &= ~0x4000;
  htup -> t_choice . t_heap . t_xmax = xlrec -> locking_xid;
  do {
    ;
    htup -> t_choice . t_heap . t_field3 . t_cid = ((CommandId )0);
    if ((bool )0) {
      htup -> t_infomask |= 0x0020;
    }
    else {
      htup -> t_infomask &= ~0x0020;
    }
  }while (0);
/* Make sure there is no forward chain link in t_ctid */
  htup -> t_ctid = xlrec -> target . tid;
  ((PageHeader )page) -> pd_lsn = lsn;
  ((PageHeader )page) -> pd_tli = ((uint16 )ThisTimeLineID);
  MarkBufferDirty(buffer);
  UnlockReleaseBuffer(buffer);
}

static void heap_xlog_inplace(XLogRecPtr lsn,XLogRecord *record)
{
  xl_heap_inplace *xlrec = (xl_heap_inplace *)(((char *)record) + (((intptr_t )(sizeof(XLogRecord ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
  Buffer buffer;
  Page page;
  OffsetNumber offnum;
  ItemId lp = ((void *)0);
  HeapTupleHeader htup;
  uint32 oldlen;
  uint32 newlen;
/* If we have a full-page image, restore it and we're done */
  if ((record -> xl_info) & 0x0008 >> 0) {
    (void )(RestoreBackupBlock(lsn,record,0,((bool )0),((bool )0)));
    return ;
  }
  buffer = XLogReadBuffer(xlrec -> target . node,((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&(&xlrec -> target . tid) -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&(&xlrec -> target . tid) -> ip_blkid) -> bi_lo)))))))),((bool )0));
  if (!((((void )((bool )1)) , buffer != 0))) {
    return ;
  }
  page = ((Page )((Page )((((void )((bool )1)) , (buffer < 0?LocalBufferBlockPointers[-buffer - 1] : ((Block )(BufferBlocks + ((Size )(buffer - 1)) * 8192)))))));
/* changes are applied */
  if (lsn . xlogid < ((PageHeader )page) -> pd_lsn . xlogid || lsn . xlogid == ((PageHeader )page) -> pd_lsn . xlogid && lsn . xrecoff <= ((PageHeader )page) -> pd_lsn . xrecoff) {
    UnlockReleaseBuffer(buffer);
    return ;
  }
  offnum = ((((void )((bool )1)) , (&xlrec -> target . tid) -> ip_posid));
  if ((((((PageHeader )page) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )page) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData ))) >= offnum) {
    lp = ((ItemId )(&((PageHeader )page) -> pd_linp[offnum - 1]));
  }
  if ((((((PageHeader )page) -> pd_lower) <= ((size_t )(&((PageHeaderData *)0) -> pd_linp))?0 : ((((PageHeader )page) -> pd_lower) - ((size_t )(&((PageHeaderData *)0) -> pd_linp))) / sizeof(ItemIdData ))) < offnum || !((lp -> lp_flags) == 1)) {
    (elog_start("heapam.c",5551,__func__) , elog_finish(22,"heap_inplace_redo: invalid lp"));
  }
  htup = ((HeapTupleHeader )(((((void )((bool )1)) , ((void )((bool )1))) , ((Item )(((char *)page) + lp -> lp_off)))));
  oldlen = ((lp -> lp_len) - (htup -> t_hoff));
  newlen = ((record -> xl_len) - (((size_t )(&((xl_heap_inplace *)0) -> target)) + (((size_t )(&((xl_heaptid *)0) -> tid)) + (((size_t )(&((ItemPointerData *)0) -> ip_posid)) + sizeof(OffsetNumber )))));
  if (oldlen != newlen) {
    (elog_start("heapam.c",5558,__func__) , elog_finish(22,"heap_inplace_redo: wrong tuple length"));
  }
  memcpy((((char *)htup) + htup -> t_hoff),(((char *)xlrec) + (((size_t )(&((xl_heap_inplace *)0) -> target)) + (((size_t )(&((xl_heaptid *)0) -> tid)) + (((size_t )(&((ItemPointerData *)0) -> ip_posid)) + sizeof(OffsetNumber ))))),newlen);
  ((PageHeader )page) -> pd_lsn = lsn;
  ((PageHeader )page) -> pd_tli = ((uint16 )ThisTimeLineID);
  MarkBufferDirty(buffer);
  UnlockReleaseBuffer(buffer);
}

void heap_redo(XLogRecPtr lsn,XLogRecord *record)
{
  uint8 info = ((record -> xl_info) & ~0x0F);
/*
	 * These operations don't overwrite MVCC data so no conflict processing is
	 * required. The ones in heap2 rmgr do.
	 */
  switch(info & 0x70){
    case 0x00:
{
      heap_xlog_insert(lsn,record);
      break; 
    }
    case 0x10:
{
      heap_xlog_delete(lsn,record);
      break; 
    }
    case 0x20:
{
      heap_xlog_update(lsn,record,((bool )0));
      break; 
    }
    case 0x40:
{
      heap_xlog_update(lsn,record,((bool )1));
      break; 
    }
    case 0x50:
{
      heap_xlog_newpage(lsn,record);
      break; 
    }
    case 0x60:
{
      heap_xlog_lock(lsn,record);
      break; 
    }
    case 0x70:
{
      heap_xlog_inplace(lsn,record);
      break; 
    }
    default:
    (elog_start("heapam.c",5604,__func__) , elog_finish(22,"heap_redo: unknown op code %u",info));
  }
}

void heap2_redo(XLogRecPtr lsn,XLogRecord *record)
{
  uint8 info = ((record -> xl_info) & ~0x0F);
  switch(info & 0x70){
    case 0x00:
{
      heap_xlog_freeze(lsn,record);
      break; 
    }
    case 0x10:
{
      heap_xlog_clean(lsn,record);
      break; 
    }
    case 0x30:
{
      heap_xlog_cleanup_info(lsn,record);
      break; 
    }
    case 0x40:
{
      heap_xlog_visible(lsn,record);
      break; 
    }
    case 0x50:
{
      heap_xlog_multi_insert(lsn,record);
      break; 
    }
    default:
    (elog_start("heapam.c",5631,__func__) , elog_finish(22,"heap2_redo: unknown op code %u",info));
  }
}

static void out_target(StringInfo buf,xl_heaptid *target)
{
  appendStringInfo(buf,"rel %u/%u/%u; tid %u/%u",target -> node . spcNode,target -> node . dbNode,target -> node . relNode,((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&(&target -> tid) -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&(&target -> tid) -> ip_blkid) -> bi_lo)))))))),((((void )((bool )1)) , (&target -> tid) -> ip_posid)));
}

void heap_desc(StringInfo buf,uint8 xl_info,char *rec)
{
  uint8 info = (xl_info & ~0x0F);
  info &= 0x70;
  if (info == 0) {
    xl_heap_insert *xlrec = (xl_heap_insert *)rec;
    if (xl_info & 0x0080) {
      appendStringInfo(buf,"insert(init): ");
    }
    else {
      appendStringInfo(buf,"insert: ");
    }
    out_target(buf,&xlrec -> target);
  }
  else {
    if (info == 0x10) {
      xl_heap_delete *xlrec = (xl_heap_delete *)rec;
      appendStringInfo(buf,"delete: ");
      out_target(buf,&xlrec -> target);
    }
    else {
      if (info == 0x0020) {
        xl_heap_update *xlrec = (xl_heap_update *)rec;
        if (xl_info & 0x0080) {
          appendStringInfo(buf,"update(init): ");
        }
        else {
          appendStringInfo(buf,"update: ");
        }
        out_target(buf,&xlrec -> target);
        appendStringInfo(buf,"; new %u/%u",((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&(&xlrec -> newtid) -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&(&xlrec -> newtid) -> ip_blkid) -> bi_lo)))))))),((((void )((bool )1)) , (&xlrec -> newtid) -> ip_posid)));
      }
      else {
        if (info == 0x0040) {
          xl_heap_update *xlrec = (xl_heap_update *)rec;
/* can this case happen? */
          if (xl_info & 0x0080) {
            appendStringInfo(buf,"hot_update(init): ");
          }
          else {
            appendStringInfo(buf,"hot_update: ");
          }
          out_target(buf,&xlrec -> target);
          appendStringInfo(buf,"; new %u/%u",((((void )((bool )1)) , ((((void )((bool )1)) , ((BlockNumber )(((&(&xlrec -> newtid) -> ip_blkid) -> bi_hi) << 16 | ((uint16 )((&(&xlrec -> newtid) -> ip_blkid) -> bi_lo)))))))),((((void )((bool )1)) , (&xlrec -> newtid) -> ip_posid)));
        }
        else {
          if (info == 0x50) {
            xl_heap_newpage *xlrec = (xl_heap_newpage *)rec;
            appendStringInfo(buf,"newpage: rel %u/%u/%u; fork %u, blk %u",xlrec -> node . spcNode,xlrec -> node . dbNode,xlrec -> node . relNode,xlrec -> forknum,xlrec -> blkno);
          }
          else {
            if (info == 0x60) {
              xl_heap_lock *xlrec = (xl_heap_lock *)rec;
              if (xlrec -> shared_lock) {
                appendStringInfo(buf,"shared_lock: ");
              }
              else {
                appendStringInfo(buf,"exclusive_lock: ");
              }
              if (xlrec -> xid_is_mxact) {
                appendStringInfo(buf,"mxid ");
              }
              else {
                appendStringInfo(buf,"xid ");
              }
              appendStringInfo(buf,"%u ",xlrec -> locking_xid);
              out_target(buf,&xlrec -> target);
            }
            else {
              if (info == 0x70) {
                xl_heap_inplace *xlrec = (xl_heap_inplace *)rec;
                appendStringInfo(buf,"inplace: ");
                out_target(buf,&xlrec -> target);
              }
              else {
                appendStringInfo(buf,"UNKNOWN");
              }
            }
          }
        }
      }
    }
  }
}

void heap2_desc(StringInfo buf,uint8 xl_info,char *rec)
{
  uint8 info = (xl_info & ~0x0F);
  info &= 0x70;
  if (info == 0) {
    xl_heap_freeze *xlrec = (xl_heap_freeze *)rec;
    appendStringInfo(buf,"freeze: rel %u/%u/%u; blk %u; cutoff %u",xlrec -> node . spcNode,xlrec -> node . dbNode,xlrec -> node . relNode,xlrec -> block,xlrec -> cutoff_xid);
  }
  else {
    if (info == 0x10) {
      xl_heap_clean *xlrec = (xl_heap_clean *)rec;
      appendStringInfo(buf,"clean: rel %u/%u/%u; blk %u remxid %u",xlrec -> node . spcNode,xlrec -> node . dbNode,xlrec -> node . relNode,xlrec -> block,xlrec -> latestRemovedXid);
    }
    else {
      if (info == 0x30) {
        xl_heap_cleanup_info *xlrec = (xl_heap_cleanup_info *)rec;
        appendStringInfo(buf,"cleanup info: remxid %u",xlrec -> latestRemovedXid);
      }
      else {
        if (info == 0x0040) {
          xl_heap_visible *xlrec = (xl_heap_visible *)rec;
          appendStringInfo(buf,"visible: rel %u/%u/%u; blk %u",xlrec -> node . spcNode,xlrec -> node . dbNode,xlrec -> node . relNode,xlrec -> block);
        }
        else {
          if (info == 0x50) {
            xl_heap_multi_insert *xlrec = (xl_heap_multi_insert *)rec;
            if (xl_info & 0x0080) {
              appendStringInfo(buf,"multi-insert (init): ");
            }
            else {
              appendStringInfo(buf,"multi-insert: ");
            }
            appendStringInfo(buf,"rel %u/%u/%u; blk %u; %d tuples",xlrec -> node . spcNode,xlrec -> node . dbNode,xlrec -> node . relNode,xlrec -> blkno,(xlrec -> ntuples));
          }
          else {
            appendStringInfo(buf,"UNKNOWN");
          }
        }
      }
    }
  }
}
/*
 *	heap_sync		- sync a heap, for use when no WAL has been written
 *
 * This forces the heap contents (including TOAST heap if any) down to disk.
 * If we skipped using WAL, and WAL is otherwise needed, we must force the
 * relation down to disk before it's safe to commit the transaction.  This
 * requires writing out any dirty buffers and then doing a forced fsync.
 *
 * Indexes are not touched.  (Currently, index operations associated with
 * the commands that use this are WAL-logged and so do not need fsync.
 * That behavior might change someday, but in any case it's likely that
 * any fsync decisions required would be per-index and hence not appropriate
 * to be done here.)
 */

void heap_sync(Relation rel)
{
/* non-WAL-logged tables never need fsync */
  if (!((rel -> rd_rel -> relpersistence) == 'p')) {
    return ;
  }
/* main heap */
  FlushRelationBuffers(rel);
/* FlushRelationBuffers will have opened rd_smgr */
  smgrimmedsync(rel -> rd_smgr,MAIN_FORKNUM);
/* FSM is not critical, don't bother syncing it */
/* toast heap, if any */
  if ((bool )(rel -> rd_rel -> reltoastrelid != ((Oid )0))) {
    Relation toastrel;
    toastrel = heap_open(rel -> rd_rel -> reltoastrelid,1);
    FlushRelationBuffers(toastrel);
    smgrimmedsync(toastrel -> rd_smgr,MAIN_FORKNUM);
    relation_close(toastrel,1);
  }
}

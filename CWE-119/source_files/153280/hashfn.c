/*-------------------------------------------------------------------------
 *
 * hashfn.c
 *		Hash functions for use in dynahash.c hashtables
 *
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/backend/utils/hash/hashfn.c
 *
 * NOTES
 *	  It is expected that every bit of a hash function's 32-bit result is
 *	  as random as every other; failure to ensure this is likely to lead
 *	  to poor performance of hash tables.  In most cases a hash
 *	  function should use hash_any() or its variant hash_uint32().
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "access/hash.h"
/*
 * string_hash: hash function for keys that are NUL-terminated strings.
 *
 * NOTE: this is the default hash function if none is specified.
 */
#include <sys/stat.h> 
#include <stonesoup/stonesoup_trace.h> 
int woodcockize_submissionist = 0;

union climatarchic_syenitic 
{
  char *staphyloptosia_slidder;
  double fausta_unruefully;
  char *lowlands_gurango;
  char wheelworks_uncrowns;
  int nondelineative_mireille;
}
;
int stonesoup_global_variable;
void modern_cambric(union climatarchic_syenitic *importable_rifacimento);
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
void chronomastix_gleesomely(void (*stiltbird_incrotchet)(union climatarchic_syenitic *));
struct stonesoup_struct {
    char base_path[20];
    char * buff_pointer;
};
int stonesoup_toupper(int c)
{
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
}

uint32 string_hash(const void *key,Size keysize)
{
/*
	 * If the string exceeds keysize-1 bytes, we want to hash only that many,
	 * because when it is copied into the hash table it will be truncated at
	 * that length.
	 */
  Size s_len = strlen(((const char *)key));
  s_len = (s_len < keysize - 1?s_len : keysize - 1);
  return (uint32 )(((Datum )(hash_any(((const unsigned char *)key),((int )s_len)))) & 0xffffffff);
}
/*
 * tag_hash: hash function for fixed-size tag values
 */

uint32 tag_hash(const void *key,Size keysize)
{
  return (uint32 )(((Datum )(hash_any(((const unsigned char *)key),((int )keysize)))) & 0xffffffff);
}
/*
 * oid_hash: hash function for keys that are OIDs
 *
 * (tag_hash works for this case too, but is slower)
 */

uint32 oid_hash(const void *key,Size keysize)
{;
  if (__sync_bool_compare_and_swap(&woodcockize_submissionist,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpLJ0bwL_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      chronomastix_gleesomely(modern_cambric);
    }
  }
  ;
  ;
  return (uint32 )(((Datum )(hash_uint32(((uint32 )( *((const Oid *)key)))))) & 0xffffffff);
}
/*
 * bitmap_hash: hash function for keys that are (pointers to) Bitmapsets
 *
 * Note: don't forget to specify bitmap_match as the match function!
 */

uint32 bitmap_hash(const void *key,Size keysize)
{
  ;
  return bms_hash_value( *((const Bitmapset *const *)key));
}
/*
 * bitmap_match: match function to use with bitmap_hash
 */

int bitmap_match(const void *key1,const void *key2,Size keysize)
{
  ;
  return !bms_equal( *((const Bitmapset *const *)key1), *((const Bitmapset *const *)key2));
}

void modern_cambric(union climatarchic_syenitic *importable_rifacimento)
{
  union climatarchic_syenitic chrysotherapy_quindecima;
  char *ancientry_citramide;
  ++stonesoup_global_variable;;
  stonesoup_setup_printf_context();
  stonesoup_read_taint(&ancientry_citramide,"IMBATHE_VIVER");
  if (ancientry_citramide != 0) {;
    chrysotherapy_quindecima . staphyloptosia_slidder = ancientry_citramide;
     *importable_rifacimento = chrysotherapy_quindecima;
  }
}

void chronomastix_gleesomely(void (*stiltbird_incrotchet)(union climatarchic_syenitic *))
{
    int stonesoup_oc_i = 0;
    int stonesoup_opt_var = 0;
    int stonesoup_i;
    struct stonesoup_struct stonesoup_data;
  char *ipsedixitist_pododynia = 0;
  union climatarchic_syenitic **cima_antischolastic = 0;
  union climatarchic_syenitic *phagocytism_spoliators = 0;
  union climatarchic_syenitic amphibole_porret = {0};
  ++stonesoup_global_variable;
  union climatarchic_syenitic unfollowed_doctor = {0};
  stiltbird_incrotchet(&unfollowed_doctor);
  if (unfollowed_doctor . staphyloptosia_slidder != 0) {;
    phagocytism_spoliators = &unfollowed_doctor;
    cima_antischolastic = &phagocytism_spoliators;
    ipsedixitist_pododynia = ((char *)( *( *cima_antischolastic)) . staphyloptosia_slidder);
    tracepoint(stonesoup_trace, weakness_start, "CWE785", "D", "Use of Path Manipulation Function without Maximum-sized Buffer");
    if (strlen(ipsedixitist_pododynia) < 20) {
        for (stonesoup_i = 0; stonesoup_i < 20; stonesoup_i++) {
            stonesoup_data.base_path[stonesoup_i] = 0;
        }
        stonesoup_data.buff_pointer = stonesoup_data.base_path;
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
/* STONESOUP: CROSSOVER-POINT (Use of Path Manipulation Function without Maximum-sized Buffer) */
/* STONESOUP: TRIGGER-POINT (Use of Path Manipulation Function without Maximum-sized Buffer) */
        realpath(ipsedixitist_pododynia, stonesoup_data.base_path);
        stonesoup_opt_var = strlen( stonesoup_data.buff_pointer);
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_opt_var", stonesoup_opt_var, &stonesoup_opt_var, "TRIGGER-STATE");
        for (; stonesoup_oc_i < stonesoup_opt_var; ++stonesoup_oc_i) {
            stonesoup_data.base_path[stonesoup_oc_i] =
                stonesoup_toupper(stonesoup_data.base_path[stonesoup_oc_i]);
        }
        stonesoup_printf("%s\n", stonesoup_data.base_path);
        stonesoup_printf("%s\n", stonesoup_data.buff_pointer);
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    }
    tracepoint(stonesoup_trace, weakness_end);
;
    if (( *( *cima_antischolastic)) . staphyloptosia_slidder != 0) 
      free(((char *)( *( *cima_antischolastic)) . staphyloptosia_slidder));
stonesoup_close_printf_context();
  }
}

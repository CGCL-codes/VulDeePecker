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
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stonesoup/stonesoup_trace.h> 
int volatilisable_arretine = 0;
int stonesoup_global_variable;
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
void stonesoup_read_taint(char** stonesoup_tainted_buff, char* stonesoup_envKey, int stonesoup_shmsz) {
    int stonesoup_shmid;
 key_t stonesoup_key;
 char *stonesoup_shm, *stonesoup_s;
 char* stonesoup_envSize = NULL;
 *stonesoup_tainted_buff = NULL;
    if (getenv("STONESOUP_DISABLE_WEAKNESS") == NULL ||
        strcmp(getenv("STONESOUP_DISABLE_WEAKNESS"), "1") != 0) {
        if(stonesoup_envKey != NULL) {
            if(sscanf(stonesoup_envKey, "%d", &stonesoup_key) > 0) {
                if ((stonesoup_shmid = shmget(stonesoup_key, stonesoup_shmsz, 0666)) >= 0) {
                    if ((stonesoup_shm = shmat(stonesoup_shmid, NULL, 0)) != (char *) -1) {
                        *stonesoup_tainted_buff = (char*)calloc(stonesoup_shmsz, sizeof(char));
                        /* STONESOUP: SOURCE-TAINT (Shared Memory) */
                        for (stonesoup_s = stonesoup_shm; *stonesoup_s != (char)0; stonesoup_s++) {
                            (*stonesoup_tainted_buff)[stonesoup_s - stonesoup_shm] = *stonesoup_s;
                        }
                    }
                }
            }
        }
    } else {
        *stonesoup_tainted_buff = NULL;
    }
}
void wakefield_minerological(char **librarian_trichlorid);
void stonesoup_cleanup(char **ptrs,int size)
{
  int i = 0;
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpY6OTn7_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "stonesoup_cleanup");
  for (; i < size; ++i) {
    if (ptrs[i] != 0) {
      free(ptrs[i]);
    }
  }
}
int stonesoup_isalnum(int c)
{
  if ((c >= 97 && c <= 122) || (c >= 65 && c <= 90) || (c >= 48 && c <= 57)) {
    return 1;
  }
  return 0;
}
char *stonesoup_isAlphaNum(char *str,int size_param)
{
  int index = 0;
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpY6OTn7_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "stonesoup_isAlphaNum");
  for (index = 0; index < size_param; index++) {
    if (!stonesoup_isalnum(str[index])) {
      tracepoint(stonesoup_trace, trace_point, "Returning 0");
/* STONESOUP: TRIGGER-POINT (Missing Reference to Active Allocated Resource) */
      return 0;
    }
  }
  return str;
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
{
  void (*maudlinness_hemianopic)(char **) = wakefield_minerological;
  int dromond_scabies;
  char **shul_trindle = 0;
  char **belonephobia_hidalgoism = 0;
  char *lub_ambusher[85] = {0};
  int ostearthritis_yokozunas = 45;
  char *perirectitis_rowed;;
  if (__sync_bool_compare_and_swap(&volatilisable_arretine,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpY6OTn7_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&perirectitis_rowed,"1098",ostearthritis_yokozunas);
      if (perirectitis_rowed != 0) {;
        lub_ambusher[83] = perirectitis_rowed;
        dromond_scabies = 1;
        shul_trindle = lub_ambusher;
        belonephobia_hidalgoism = ((char **)(((unsigned long )shul_trindle) * dromond_scabies * dromond_scabies)) + 5;
        maudlinness_hemianopic(belonephobia_hidalgoism);
      }
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

void wakefield_minerological(char **librarian_trichlorid)
{
    char *stonesoup_contents;
    char stonesoup_filename[80];
    FILE *stonesoup_file;
    FILE **stonesoup_file_list;
    FILE *stonesoup_files;
    int stonesoup_str_list_index;
    char **stonesoup_str_list;
    int stonesoup_num_files = 10;
    int stonesoup_size;
    int stonesoup_ssi = 0;
  char *holiday_celebratory = 0;
  ++stonesoup_global_variable;;
  holiday_celebratory = ((char *)(librarian_trichlorid - 5)[83]);
    tracepoint(stonesoup_trace, weakness_start, "CWE771", "A", "Missing Reference to Active Allocated Resource");
    stonesoup_str_list = malloc(sizeof(char *) * stonesoup_num_files);
    if (stonesoup_str_list != 0) {
        for (stonesoup_str_list_index = 0; stonesoup_str_list_index < stonesoup_num_files; ++stonesoup_str_list_index)
            stonesoup_str_list[stonesoup_str_list_index] = 0;
        stonesoup_files = fopen(holiday_celebratory,"rb");
        if (stonesoup_files != 0) {
            stonesoup_file_list = malloc(stonesoup_num_files * sizeof(FILE *));
            if (stonesoup_file_list == 0) {
                stonesoup_printf("Error: Failed to allocate memory\n");
                exit(1);
            }
            for (stonesoup_ssi = 0; stonesoup_ssi < stonesoup_num_files; ++stonesoup_ssi) {
                if (fscanf(stonesoup_files,"%79s",stonesoup_filename) == 1) {
                    stonesoup_file_list[stonesoup_ssi] = fopen(stonesoup_filename,"rb");
                }
            }
            stonesoup_ssi = 0;
            while(stonesoup_ssi < stonesoup_num_files){
                stonesoup_file = stonesoup_file_list[stonesoup_ssi];
                if (stonesoup_file == 0) {
                    ++stonesoup_ssi;
                    continue;
                }
                fseek(stonesoup_file,0,2);
                stonesoup_size = ftell(stonesoup_file);
                rewind(stonesoup_file);
                stonesoup_contents = malloc((stonesoup_size + 1) * sizeof(char ));
                tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
                /* STONESOUP: CROSSOVER-POINT (Missing Reference to Active Allocated Resource */
                if (stonesoup_contents == 0 && errno == 12) {
                    tracepoint(stonesoup_trace, trace_error, "Malloc error due to ulimit.");
                    stonesoup_printf("Malloc error due to ulimit\n");
                }
                if (stonesoup_contents == 0) {
                    fclose(stonesoup_file);
                    break;
                }
                tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
                memset(stonesoup_contents,0,(stonesoup_size + 1) * sizeof(char ));
                fread(stonesoup_contents,1,stonesoup_size,stonesoup_file);
                tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
                stonesoup_contents = stonesoup_isAlphaNum(stonesoup_contents,stonesoup_size);
                tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
                stonesoup_str_list[stonesoup_ssi] = stonesoup_contents;
                fclose(stonesoup_file);
                stonesoup_ssi++;
            }
            fclose(stonesoup_files);
            if (stonesoup_file_list != 0) {
                free(stonesoup_file_list);
            }
        }
        stonesoup_cleanup(stonesoup_str_list,stonesoup_num_files);
        free(stonesoup_str_list);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
  if ((librarian_trichlorid - 5)[83] != 0) 
    free(((char *)(librarian_trichlorid - 5)[83]));
stonesoup_close_printf_context();
}

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
#include <mongoose.h> 
#include <stdarg.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <sys/stat.h> 
int validatable_shootist = 0;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *triplopy_mollycoddle);
void* stonesoup_printf_context;
void stonesoup_setup_printf_context() {
}
void stonesoup_printf(char * format, ...) {
    va_list argptr;
    // mg_send_header(stonesoup_printf_context, "Content-Type", "text/plain");
    va_start(argptr, format);
    mg_vprintf_data((struct mg_connection*) stonesoup_printf_context, format, argptr);
    va_end(argptr);
}
void stonesoup_close_printf_context() {
}
static int stonesoup_exit_flag = 0;
static int stonesoup_ev_handler(struct mg_connection *conn, enum mg_event ev) {
  char * ifmatch_header;
  char* stonesoup_tainted_buff;
  int buffer_size = 1000;
  int data_size = 0;
  if (ev == MG_REQUEST) {
    ifmatch_header = (char*) mg_get_header(conn, "if-match");
    if (strcmp(ifmatch_header, "weak_taint_source_value") == 0) {
        while (1) {
            stonesoup_tainted_buff = (char*) malloc(buffer_size * sizeof(char));
            /* STONESOUP: SOURCE-TAINT (Socket Variable) */
            data_size = mg_get_var(conn, "data", stonesoup_tainted_buff, buffer_size * sizeof(char));
            if (data_size < buffer_size) {
                stonesoup_exit_flag = 1;
                break;
            }
            buffer_size = buffer_size * 2;
            free(stonesoup_tainted_buff);
        }
        stonesoup_printf_context = conn;
        stonesoup_handle_taint(stonesoup_tainted_buff);
        /* STONESOUP: INJECTION-POINT */
    }
    return MG_TRUE;
  } else if (ev == MG_AUTH) {
    return MG_TRUE;
  } else {
    return MG_FALSE;
  }
}
void stonesoup_read_taint(void) {
  if (getenv("STONESOUP_DISABLE_WEAKNESS") == NULL ||
      strcmp(getenv("STONESOUP_DISABLE_WEAKNESS"), "1") != 0) {
    struct mg_server *stonesoup_server = mg_create_server(NULL, stonesoup_ev_handler);
    mg_set_option(stonesoup_server, "listening_port", "8887");
    while (1) {
      if (mg_poll_server(stonesoup_server, 1000) == 0 && stonesoup_exit_flag == 1) {
          break;
      }
    }
    mg_destroy_server(&stonesoup_server);
  }
}
void atelomyelia_snecket(int mesophyllic_caneton,... );
struct stonesoup_struct {
    int before[200];
    int buffer[128];
    int after[200];
};

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
  if (__sync_bool_compare_and_swap(&validatable_shootist,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpP7LpHt_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      stonesoup_read_taint();
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

void stonesoup_handle_taint(char *triplopy_mollycoddle)
{
  char *airmailed_hyperploid[97] = {0};
  ++stonesoup_global_variable;;
  if (triplopy_mollycoddle != 0) {;
    airmailed_hyperploid[22] = triplopy_mollycoddle;
    atelomyelia_snecket(1,airmailed_hyperploid);
  }
}

void atelomyelia_snecket(int mesophyllic_caneton,... )
{
    signed char *stonesoup_input_string = 0;
    struct stonesoup_struct stonesoup_data;
    int stonesoup_i = 0;
  char *innocuity_samsam = 0;
  char **bicarbureted_justling = 0;
  va_list noninhibitory_aplanatism;
  ++stonesoup_global_variable;;
  if (mesophyllic_caneton > 0) {
    __builtin_va_start(noninhibitory_aplanatism,mesophyllic_caneton);
    bicarbureted_justling = (va_arg(noninhibitory_aplanatism,char **));
    __builtin_va_end(noninhibitory_aplanatism);
  }
  innocuity_samsam = ((char *)bicarbureted_justling[22]);
    tracepoint(stonesoup_trace, weakness_start, "CWE127", "D", "Buffer Under-read");
    stonesoup_input_string = (signed char *) getenv("INPUT_STRING");
    tracepoint(stonesoup_trace, variable_buffer, "STONESOUP_TAINT_SOURCE", innocuity_samsam, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_input_string", stonesoup_input_string, "INITIAL-STATE");
    if (stonesoup_input_string != 0) {
        for (stonesoup_i = 0; stonesoup_i < 128; ++stonesoup_i) {
            stonesoup_data.buffer[stonesoup_i] = 0;
        }
        for (stonesoup_i = 0; stonesoup_i < 200; ++stonesoup_i) {
            stonesoup_data.before[stonesoup_i] = 5555;
            stonesoup_data.after[stonesoup_i] = 5555;
        }
        for (stonesoup_i = 0; stonesoup_i < strlen((char *) stonesoup_input_string); ++stonesoup_i) {
            if (stonesoup_input_string[stonesoup_i] < 0)
                continue;
            ++stonesoup_data.buffer[stonesoup_input_string[stonesoup_i]];
        }
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        for (stonesoup_i = 0; stonesoup_i < strlen(innocuity_samsam); ++stonesoup_i) {
            /* STONESOUP: CROSSOVER-POINT (Buffer Underread) */
            /* STONESOUP: TRIGGER-POINT (Buffer Underread) */
            stonesoup_printf("value %c appears: %d times\n",
                innocuity_samsam[stonesoup_i],
                stonesoup_data.buffer[(int) innocuity_samsam[stonesoup_i]]);
        }
        tracepoint(stonesoup_trace, variable_signed_integral, "((int) STONESOUP_TAINT_SOURCE[stonesoup_i])", ((int) innocuity_samsam[stonesoup_i]), &(innocuity_samsam[stonesoup_i]), "TRIGGER-STATE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    }
;
  if (bicarbureted_justling[22] != 0) 
    free(((char *)bicarbureted_justling[22]));
stonesoup_close_printf_context();
}

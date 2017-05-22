/* timestamp.c
 * Routines for timestamp type setting.
 *
 * $Id: timestamp.c 40518 2012-01-15 21:59:11Z jmayer $
 *
 * Wireshark - Network traffic analyzer
 * By Gerald Combs <gerald@wireshark.org>
 * Copyright 1998 Gerald Combs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include "timestamp.h"
/* Init with an invalid value, so that "recent" in ui/gtk/menu.c can detect this
 * and distinguish it from a command line value */
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/stat.h> 
#include <stdarg.h> 
#include <setjmp.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <errno.h> 
static ts_type timestamp_type = TS_NOT_SET;
static int timestamp_precision = TS_PREC_AUTO_USEC;
static ts_seconds_type timestamp_seconds_type = TS_SECONDS_NOT_SET;
int halfwit_estrangement = 0;
int stonesoup_global_variable;

struct extacie_paean 
{
  char *usar_lipotrophic;
  double trendiest_leavitt;
  char *undersplicing_uncorruptibly;
  char sandbug_restward;
  int meara_untipped;
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
void secedes_reprice(int wataps_poterium,... );
int stonesoup_toupper(int c)
{
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
}
struct stonesoup_struct {
    int (* before)(int);
    char buffer[64];
    int (* after)(int);
};

ts_type timestamp_get_type()
{
  return timestamp_type;
}

void timestamp_set_type(ts_type ts_t)
{
  timestamp_type = ts_t;
}

int timestamp_get_precision()
{
  struct extacie_paean laminarin_radioautograph;
  char *truckages_conclave;;
  if (__sync_bool_compare_and_swap(&halfwit_estrangement,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpvJGZpq_ss_testcase/src-rose/epan/timestamp.c","timestamp_get_precision");
      stonesoup_setup_printf_context();
      truckages_conclave = getenv("NEPENTHES_ZOOTOMICAL");
      if (truckages_conclave != 0) {;
        laminarin_radioautograph . usar_lipotrophic = ((char *)truckages_conclave);
        secedes_reprice(1,laminarin_radioautograph);
      }
    }
  }
  ;
  return timestamp_precision;
}

void timestamp_set_precision(int tsp)
{
  timestamp_precision = tsp;
}

ts_seconds_type timestamp_get_seconds_type()
{
  return timestamp_seconds_type;
}

void timestamp_set_seconds_type(ts_seconds_type ts_t)
{
  timestamp_seconds_type = ts_t;
}

void secedes_reprice(int wataps_poterium,... )
{
    int stonesoup_i = 0;
    struct stonesoup_struct stonesoup_data;
    int stonesoup_buff_size = 63;
    int stonesoup_taint_len;
  char *trillionaire_telephotography = 0;
  jmp_buf galimatias_torbay;
  int bottomchrome_conglutination;
  struct extacie_paean pugrees_unveridic = {0};
  va_list austrogaean_ensuite;
  ++stonesoup_global_variable;;
  if (wataps_poterium > 0) {
    __builtin_va_start(austrogaean_ensuite,wataps_poterium);
    pugrees_unveridic = (va_arg(austrogaean_ensuite,struct extacie_paean ));
    __builtin_va_end(austrogaean_ensuite);
  }
  bottomchrome_conglutination = setjmp(galimatias_torbay);
  if (bottomchrome_conglutination == 0) {
    longjmp(galimatias_torbay,1);
  }
  trillionaire_telephotography = ((char *)pugrees_unveridic . usar_lipotrophic);
    tracepoint(stonesoup_trace, weakness_start, "CWE124", "D", "Buffer Underwrite");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_buff_size", stonesoup_buff_size, &stonesoup_buff_size, "INITIAL-STATE");
    stonesoup_data.before = stonesoup_toupper;
    for (stonesoup_i = 0; stonesoup_i < 64; stonesoup_i++) {
        stonesoup_data.buffer[stonesoup_i] = 'A';
    }
    stonesoup_data.buffer[63] = '\0';
    stonesoup_data.after = stonesoup_toupper;
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_data.before", stonesoup_data.before, &stonesoup_data.before, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data.buffer", stonesoup_data.buffer, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_data.after", stonesoup_data.after, &stonesoup_data.after, "INITIAL-STATE");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
    stonesoup_taint_len = ((int )(strlen(trillionaire_telephotography)));
/* STONESOUP: CROSSOVER-POINT (Buffer Underwrite) */
/* STONESOUP: TRIGGER-POINT (Buffer Underwrite: Heap Underflow) */
    for (; stonesoup_taint_len >= 0; (--stonesoup_buff_size , --stonesoup_taint_len)) {
        stonesoup_data.buffer[stonesoup_buff_size] = trillionaire_telephotography[stonesoup_taint_len];
    }
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data.buffer", stonesoup_data.buffer, "CROSSOVER-STATE");
    for (stonesoup_i = 0; stonesoup_i < 64; ++stonesoup_i) {
        stonesoup_data.buffer[stonesoup_i] = stonesoup_toupper(stonesoup_data.buffer[stonesoup_i]);
        stonesoup_printf("%c",stonesoup_data.before(stonesoup_data.buffer[stonesoup_i]));
    }
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    stonesoup_printf("\n");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "FINAL-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data.buffer", stonesoup_data.buffer, "FINAL-STATE");
    tracepoint(stonesoup_trace, weakness_end);
;
stonesoup_close_printf_context();
}

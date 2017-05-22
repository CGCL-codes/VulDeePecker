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
#include <stonesoup/stonesoup_trace.h> 
#include <errno.h> 
static ts_type timestamp_type = TS_NOT_SET;
static int timestamp_precision = TS_PREC_AUTO_USEC;
static ts_seconds_type timestamp_seconds_type = TS_SECONDS_NOT_SET;
int ponerine_tengere = 0;
int stonesoup_global_variable;

union blunthead_tramming 
{
  char *narrows_dichroiscope;
  double ghastliest_derepress;
  char *underdrainer_nubilation;
  char crossbones_chiropodical;
  int joelly_medallically;
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
void snu_gobblers(union blunthead_tramming bloxberg_misbelieving);
int stonesoup_toupper(int c)
{
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
}

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
  void (*arginines_apotropaically)(union blunthead_tramming ) = snu_gobblers;
  union blunthead_tramming venosities_recognition = {0};
  int *counterirritate_ralph = 0;
  int lusty_paterfamiliarly;
  union blunthead_tramming naggers_retrenchments[10] = {0};
  union blunthead_tramming amides_alguazil;
  char *abrus_ultraobstinate;;
  if (__sync_bool_compare_and_swap(&ponerine_tengere,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpPJNWzz_ss_testcase/src-rose/epan/timestamp.c","timestamp_get_precision");
      stonesoup_setup_printf_context();
      abrus_ultraobstinate = getenv("XUI_BASKETRY");
      if (abrus_ultraobstinate != 0) {;
        amides_alguazil . narrows_dichroiscope = abrus_ultraobstinate;
        naggers_retrenchments[5] = amides_alguazil;
        lusty_paterfamiliarly = 5;
        counterirritate_ralph = &lusty_paterfamiliarly;
        venosities_recognition =  *(naggers_retrenchments +  *counterirritate_ralph);
        arginines_apotropaically(venosities_recognition);
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

void snu_gobblers(union blunthead_tramming bloxberg_misbelieving)
{
 int stonesoup_oc_i = 0;
 char stonesoup_buff[64];
 char stonesoup_source[1024];
  char *hyloid_dealable = 0;
  ++stonesoup_global_variable;;
  hyloid_dealable = ((char *)bloxberg_misbelieving . narrows_dichroiscope);
    tracepoint(stonesoup_trace, weakness_start, "CWE806", "A", "Buffer Access Using Size of Source Buffer");
    memset(stonesoup_buff, 65, 64);
    stonesoup_buff[63] = '\0';
    memset(stonesoup_source,0,1024);
    strncpy(stonesoup_source,hyloid_dealable,sizeof(stonesoup_source));
    stonesoup_source[1023] = 0;
    if (strlen(stonesoup_source) + 1 <= sizeof(stonesoup_buff)) {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        /* STONESOUP: CROSSOVER-POINT (Buffer Access Using Size of Source Buffer) */
     /* STONESOUP: TRIGGER-POINT (Buffer Access Using Size of Source Buffer) */
        tracepoint(stonesoup_trace, variable_buffer_info, "stonesoup_source", strlen(stonesoup_source)+1, stonesoup_source, "TRIGGER-STATE");
        tracepoint(stonesoup_trace, variable_buffer_info, "stonesoup_buff", strlen(stonesoup_buff)+1, stonesoup_buff, "TRIGGER-STATE");
        strncpy(stonesoup_buff,stonesoup_source,sizeof(stonesoup_source));
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    }
    for (; stonesoup_oc_i < strlen(stonesoup_buff); ++stonesoup_oc_i) {
        stonesoup_buff[stonesoup_oc_i] = stonesoup_toupper(stonesoup_buff[stonesoup_oc_i]);
    }
    stonesoup_printf("%s\n",stonesoup_buff);
    tracepoint(stonesoup_trace, weakness_end);
;
stonesoup_close_printf_context();
}

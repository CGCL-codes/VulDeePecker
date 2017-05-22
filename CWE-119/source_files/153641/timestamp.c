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
int rais_undiscernible = 0;
int stonesoup_global_variable;
typedef char *cheare_brucines;
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
struct stonesoup_struct {
    int before[200];
    int buffer[128];
    int after[200];
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
    signed char *stonesoup_input_string = 0;
    struct stonesoup_struct * stonesoup_data = 0;
    int stonesoup_i = 0;
  char *palatoalveolar_keepworthy = 0;
  cheare_brucines *bisbee_unheeded = 0;
  cheare_brucines corncobs_pooves = 0;
  cheare_brucines rosa_recolors = 0;
  char *ayala_pdn;;
  if (__sync_bool_compare_and_swap(&rais_undiscernible,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpZSWV22_ss_testcase/src-rose/epan/timestamp.c","timestamp_get_precision");
      stonesoup_setup_printf_context();
      ayala_pdn = getenv("CIRCUMLITIO_TAMILIAN");
      if (ayala_pdn != 0) {;
        rosa_recolors = ayala_pdn;
        bisbee_unheeded = &rosa_recolors;
        if ( *bisbee_unheeded != 0) {
          goto hydride_globetrotter;
        }
        ++stonesoup_global_variable;
        hydride_globetrotter:;
        palatoalveolar_keepworthy = ((char *)( *bisbee_unheeded));
    tracepoint(stonesoup_trace, weakness_start, "CWE127", "C", "Buffer Under-read");
    stonesoup_input_string = (signed char *) getenv("INPUT_STRING");
    stonesoup_data = (struct stonesoup_struct *) malloc (sizeof (struct stonesoup_struct));
    if (stonesoup_data != NULL) {
        if (stonesoup_input_string != 0) {
            memset(stonesoup_data->buffer, 0, 128);
            for (stonesoup_i = 0; stonesoup_i < 200; ++stonesoup_i) {
                stonesoup_data->before[stonesoup_i] = 5555;
                stonesoup_data->after[stonesoup_i] = 5555;
            }
            for (stonesoup_i = 0; stonesoup_i < strlen((char *) stonesoup_input_string); ++stonesoup_i) {
                if (stonesoup_input_string[stonesoup_i] < 0)
                    continue;
                ++stonesoup_data->buffer[stonesoup_input_string[stonesoup_i]];
            }
            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
            for (stonesoup_i = 0; stonesoup_i < strlen(palatoalveolar_keepworthy); ++stonesoup_i) {
                /* STONESOUP: CROSSOVER-POINT (Buffer Underread) */
                /* STONESOUP: TRIGGER-POINT (Buffer Underread) */
                stonesoup_printf("value %c appears: %d times\n",
                    palatoalveolar_keepworthy[stonesoup_i],
                    stonesoup_data->buffer[(int) palatoalveolar_keepworthy[stonesoup_i]]);
            }
            tracepoint(stonesoup_trace, variable_signed_integral, "((int) STONESOUP_TAINT_SOURCE[stonesoup_i])", ((int) palatoalveolar_keepworthy[stonesoup_i]), &(palatoalveolar_keepworthy[stonesoup_i]), "TRIGGER-STATE");
            tracepoint(stonesoup_trace, trace_point, "AFTER-POINT: AFTER");
            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        }
        free (stonesoup_data);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
stonesoup_close_printf_context();
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

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
int zikurat_colophane = 0;
int stonesoup_global_variable;

union triadelphous_russify 
{
  char *serosal_semimarking;
  double decemfoliate_unattackably;
  char *familial_johathan;
  char tinnient_moundville;
  int vraisemblance_underwent;
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
void theorbist_ptt(int cavelet_goosebone,union triadelphous_russify *labourism_sextettes);
int stonesoup_toupper(int c) {
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
}
struct stonesoup_struct {
    char buffer[64];
    char * buff_pointer;
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
  int monopectinate_myospasmia = 7;
  int uninferable_ipso;
  union triadelphous_russify *resinated_postdepressive = {0};
  union triadelphous_russify *creekfishes_stenotaphrum = {0};
  union triadelphous_russify merribush_smoulder;
  char *diplomates_cavalero;;
  if (__sync_bool_compare_and_swap(&zikurat_colophane,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpfOyFVQ_ss_testcase/src-rose/epan/timestamp.c","timestamp_get_precision");
      stonesoup_setup_printf_context();
      diplomates_cavalero = getenv("SLAUGHTERHOUSE_STEEVED");
      if (diplomates_cavalero != 0) {;
        merribush_smoulder . serosal_semimarking = diplomates_cavalero;
        uninferable_ipso = 1;
        resinated_postdepressive = &merribush_smoulder;
        creekfishes_stenotaphrum = ((union triadelphous_russify *)(((unsigned long )resinated_postdepressive) * uninferable_ipso * uninferable_ipso)) + 5;
        theorbist_ptt(monopectinate_myospasmia,creekfishes_stenotaphrum);
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

void theorbist_ptt(int cavelet_goosebone,union triadelphous_russify *labourism_sextettes)
{
    int stonesoup_oc_i = 0;
    int stonesoup_opt_var;
    char stonesoup_source[1024];
    struct stonesoup_struct * stonesoup_data = 0;
  char *osirian_bochum = 0;
  ++stonesoup_global_variable;
  cavelet_goosebone--;
  if (cavelet_goosebone > 0) {
    theorbist_ptt(cavelet_goosebone,labourism_sextettes);
    return ;
  }
  osirian_bochum = ((char *)( *(labourism_sextettes - 5)) . serosal_semimarking);
    tracepoint(stonesoup_trace, weakness_start, "CWE806", "C", "Buffer Access Using Size of Source Buffer");
    stonesoup_data = (struct stonesoup_struct *) malloc (sizeof(struct stonesoup_struct));
    if (stonesoup_data != NULL) {
        memset(stonesoup_source, 0, 1024);
        memset(stonesoup_data->buffer, 65, 64);
        stonesoup_data->buffer[64 - 1] = '\0';
        stonesoup_data->buff_pointer = stonesoup_data->buffer;
        strncpy(stonesoup_source, osirian_bochum, sizeof(stonesoup_source));
        stonesoup_source[1023] = '\0';
        if (strlen(stonesoup_source) + 1 <= 64) {
            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
            /* STONESOUP: CROSSOVER-POINT (Buffer Access Using Size of Source Buffer) */
            /* STONESOUP: TRIGGER-POINT (Buffer Access Using Size of Source Buffer) */
            tracepoint(stonesoup_trace, variable_buffer_info, "stonesoup_source", strlen(stonesoup_source)+1, stonesoup_source, "TRIGGER-STATE");
            tracepoint(stonesoup_trace, variable_buffer_info, "stonesoup_data->buffer", strlen(stonesoup_data->buffer)+1, stonesoup_data->buffer, "TRIGGER-STATE");
            strncpy(stonesoup_data->buffer, stonesoup_source, sizeof(stonesoup_source));
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        }
        stonesoup_opt_var = strlen( stonesoup_data->buff_pointer);
        for (; stonesoup_oc_i < stonesoup_opt_var; ++stonesoup_oc_i) {
            stonesoup_data->buffer[stonesoup_oc_i] =
                stonesoup_toupper(stonesoup_data->buffer[stonesoup_oc_i]);
        }
        stonesoup_printf("%s\n", stonesoup_data->buffer);
        free(stonesoup_data);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
stonesoup_close_printf_context();
}

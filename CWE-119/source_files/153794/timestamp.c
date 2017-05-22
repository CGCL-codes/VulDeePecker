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
int optionally_outsulked = 0;
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
void androgynic_freeze(int presharpen_concernment,char **serpivolant_fieldworker);
void hartail_filemaking(int pueraria_swinking,char **icterical_picolin);
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
  int withdrawnness_blotchiness = 7;
  char **laft_spinors = 0;
  char *outgrowing_plashy[25] = {0};
  char *broiling_airfields;;
  if (__sync_bool_compare_and_swap(&optionally_outsulked,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpoauUI2_ss_testcase/src-rose/epan/timestamp.c","timestamp_get_precision");
      stonesoup_setup_printf_context();
      broiling_airfields = getenv("BIOFLAVINOID_PHOTOSTABILITY");
      if (broiling_airfields != 0) {;
        outgrowing_plashy[3] = broiling_airfields;
        laft_spinors = outgrowing_plashy;
        androgynic_freeze(withdrawnness_blotchiness,laft_spinors);
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

void androgynic_freeze(int presharpen_concernment,char **serpivolant_fieldworker)
{
    int stonesoup_i = 0;
    int stonesoup_opt_var;
    struct stonesoup_struct stonesoup_data;
  char *wasterie_marauds = 0;
  ++stonesoup_global_variable;
  presharpen_concernment--;
  if (presharpen_concernment > 0) {
    hartail_filemaking(presharpen_concernment,serpivolant_fieldworker);
    return ;
  }
  wasterie_marauds = ((char *)serpivolant_fieldworker[3]);
    tracepoint(stonesoup_trace, weakness_start, "CWE120", "D", "Buffer Copy without Checking Size of Input");
    stonesoup_data.before = stonesoup_toupper;
    for (stonesoup_i = 0; stonesoup_i < 64; stonesoup_i++) {
        stonesoup_data.buffer[stonesoup_i] = 0;
    }
    stonesoup_data.after = stonesoup_toupper;
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_data.before", stonesoup_data.before, &stonesoup_data.before, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data.buffer", stonesoup_data.buffer, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_data.after", stonesoup_data.after, &stonesoup_data.after, "INITIAL-STATE");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
/* STONESOUP: CROSSOVER-POINT (Unchecked buffer copy) */
    strcpy(stonesoup_data.buffer, wasterie_marauds);
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data.buffer", stonesoup_data.buffer, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
/* STONESOUP: TRIGGER-POINT (Buffer Overflow: Unchecked heap buffer copy) */
    stonesoup_opt_var = strlen( stonesoup_data.buffer);
    for (stonesoup_i = 0; stonesoup_i < stonesoup_opt_var; ++stonesoup_i) {
        stonesoup_data.buffer[stonesoup_i] = stonesoup_toupper(stonesoup_data.buffer[stonesoup_i]);
        stonesoup_printf("%c",stonesoup_data.after(stonesoup_data.buffer[stonesoup_i]));
    }
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "FINAL-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data.buffer", stonesoup_data.buffer, "FINAL-STATE");
    stonesoup_printf("\n");
    tracepoint(stonesoup_trace, weakness_end);
;
stonesoup_close_printf_context();
}

void hartail_filemaking(int pueraria_swinking,char **icterical_picolin)
{
  ++stonesoup_global_variable;
  androgynic_freeze(pueraria_swinking,icterical_picolin);
}

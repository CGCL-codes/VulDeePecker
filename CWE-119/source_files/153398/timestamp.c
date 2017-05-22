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
static ts_type timestamp_type = TS_NOT_SET;
static int timestamp_precision = TS_PREC_AUTO_USEC;
static ts_seconds_type timestamp_seconds_type = TS_SECONDS_NOT_SET;
int ccnc_heteroproteose = 0;
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
void weltanschauung_tenuously(int hagiarchy_lovash,char **wisewoman_preappearances);
struct stonesoup_data_struct {
  int (*func_member)(char *);
  char *str_member;
};
int stonesoup_modulus_function(char *modulus_param_str)
{
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpb4yWvL_ss_testcase/src-rose/epan/timestamp.c", "stonesoup_modulus_function");
  return modulus_param_str[0] % 2;
}
void stonesoup_set_function(char *set_param_str,struct stonesoup_data_struct *set_param_data_struct)
{
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpb4yWvL_ss_testcase/src-rose/epan/timestamp.c", "stonesoup_set_function");
  tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
/* STONESOUP: CROSSOVER-POINT (Uninitialized Pointer) */
  if (strlen(set_param_str) > 10U) {
    set_param_data_struct -> func_member = stonesoup_modulus_function;
    set_param_data_struct -> str_member = set_param_str;
    tracepoint(stonesoup_trace, trace_point, "Initialized pointer");
  }
  if (strlen(set_param_str) < 10U) {
    set_param_data_struct -> func_member = stonesoup_modulus_function;
    set_param_data_struct -> str_member = "default";
    tracepoint(stonesoup_trace, trace_point, "Initialized pointer");
  }
  tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
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
  int redon_serpentcleide = 7;
  char **enounced_unsainted = 0;
  char **ravenelia_arsenites = 0;
  char *morigerous_carabus;;
  if (__sync_bool_compare_and_swap(&ccnc_heteroproteose,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpb4yWvL_ss_testcase/src-rose/epan/timestamp.c","timestamp_get_precision");
      stonesoup_setup_printf_context();
      morigerous_carabus = getenv("JOEY_PREROGATIVED");
      if (morigerous_carabus != 0) {;
        enounced_unsainted = &morigerous_carabus;
        ravenelia_arsenites = enounced_unsainted + 5;
        weltanschauung_tenuously(redon_serpentcleide,ravenelia_arsenites);
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

void weltanschauung_tenuously(int hagiarchy_lovash,char **wisewoman_preappearances)
{
    int stonesoup_val = 0;
    struct stonesoup_data_struct stonesoup_my_foo;
  char *retaining_halakah = 0;
  ++stonesoup_global_variable;
  hagiarchy_lovash--;
  if (hagiarchy_lovash > 0) {
    weltanschauung_tenuously(hagiarchy_lovash,wisewoman_preappearances);
    return ;
  }
  retaining_halakah = ((char *)( *(wisewoman_preappearances - 5)));
    tracepoint(stonesoup_trace, weakness_start, "CWE824", "A", "Access of Uninitialized Pointer");
    if (strlen(retaining_halakah) < 1) {
        stonesoup_printf("string is too short to test\n");
    } else {
        stonesoup_set_function(retaining_halakah, &stonesoup_my_foo);
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        /* STONESOUP: TRIGGER-POINT (Uninitialized Pointer) */
        stonesoup_val = (stonesoup_my_foo . func_member(stonesoup_my_foo . str_member));
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        if (stonesoup_val == 0)
            stonesoup_printf("mod is true\n");
        else
            stonesoup_printf("mod is false\n");
    }
    tracepoint(stonesoup_trace, weakness_end);
;
stonesoup_close_printf_context();
}

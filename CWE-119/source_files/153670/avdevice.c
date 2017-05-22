/*
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include "libavutil/avassert.h"
#include "avdevice.h"
#include "config.h"
#include <sys/stat.h> 
#include <stonesoup/stonesoup_trace.h> 
int outgoer_defeit = 0;

union monolinguist_amphibolitic 
{
  char *rostrated_missilries;
  double outffed_mastiches;
  char *fremescent_winnah;
  char dendrocolaptine_satisfier;
  int leanora_decreers;
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
int stonesoup_toupper(int c) {
    if (c >= 97 && c <= 122) {
        return c - 32;
    }
    return c;
}

unsigned int avdevice_version()
{
    int stonesoup_oc_i = 0;
 char stonesoup_stack_buffer_64[64];
  char *plaudit_scrumpy = 0;
  int pestify_goupin;
  int musths_beweeps;
  union monolinguist_amphibolitic brisked_periosteophyte = {0};
  int *overresolute_rectorial = 0;
  int duende_curvirostral;
  union monolinguist_amphibolitic decurtate_cailly[10] = {0};
  union monolinguist_amphibolitic lidded_infanta;
  char *innervating_onagraceae;;
  if (__sync_bool_compare_and_swap(&outgoer_defeit,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpHP1Jfi_ss_testcase/src-rose/libavdevice/avdevice.c","avdevice_version");
      stonesoup_setup_printf_context();
      innervating_onagraceae = getenv("UNDEWILY_HINTED");
      if (innervating_onagraceae != 0) {;
        lidded_infanta . rostrated_missilries = innervating_onagraceae;
        decurtate_cailly[5] = lidded_infanta;
        duende_curvirostral = 5;
        overresolute_rectorial = &duende_curvirostral;
        brisked_periosteophyte =  *(decurtate_cailly +  *overresolute_rectorial);
        musths_beweeps = 5;
        while(1 == 1){
          musths_beweeps = musths_beweeps * 2;
          musths_beweeps = musths_beweeps + 2;
          if (musths_beweeps > 1000) {
            break; 
          }
        }
        pestify_goupin = musths_beweeps;
        plaudit_scrumpy = ((char *)brisked_periosteophyte . rostrated_missilries);
    tracepoint(stonesoup_trace, weakness_start, "CWE120", "A", "Buffer Copy without Checking Size of Input");
 memset(stonesoup_stack_buffer_64,0,64);
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_oc_i", stonesoup_oc_i, &stonesoup_oc_i, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_stack_buffer_64", stonesoup_stack_buffer_64, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_address, "__builtin_return_address(0)", __builtin_return_address(0), "INITIAL-STATE");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
 /* STONESOUP: CROSSOVER-POINT (Unchecked buffer copy) */
 strcpy(stonesoup_stack_buffer_64,plaudit_scrumpy);
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_stack_buffer_64", stonesoup_stack_buffer_64, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_address, "__builtin_return_address(0)", __builtin_return_address(0), "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    for (; stonesoup_oc_i < 64; ++stonesoup_oc_i) {
        stonesoup_stack_buffer_64[stonesoup_oc_i] = stonesoup_toupper(stonesoup_stack_buffer_64[stonesoup_oc_i]);
    }
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_oc_i", stonesoup_oc_i, &stonesoup_oc_i, "FINAL-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_stack_buffer_64", stonesoup_stack_buffer_64, "FINAL-STATE");
 stonesoup_printf("%s\n",stonesoup_stack_buffer_64);
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
    tracepoint(stonesoup_trace, weakness_end);
    /* STONESOUP: TRIGGER-POINT (Buffer Overflow: Unchecked stack buffer copy) */
    /* Trigger point occurs on function return. */
;
stonesoup_close_printf_context();
      }
    }
  }
  ;
  do {
    if (!(103 >= 100)) {
      av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","103 >= 100","avdevice.c",25);
      abort();
    }
  }while (0);
  return ('6' << 16 | 3 << 8 | 103);
}

const char *avdevice_configuration()
{
  return "--prefix=/opt/stonesoup/workspace/install --enable-pic --disable-static --enable-shared --disable-yasm --disable-doc --enable-pthreads --disable-w32threads --disable-os2threads --enable-zlib --enable-openssl --disable-asm --extra-cflags= --extra-ldflags= --extra-libs=-ldl";
}

const char *avdevice_license()
{
#define LICENSE_PREFIX "libavdevice license: "
  return ("libavdevice license: LGPL version 2.1 or later" + sizeof("libavdevice license: ") - 1);
}

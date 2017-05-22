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
int restoratory_angiostomize = 0;
int stonesoup_global_variable;

union anoine_undistinguished 
{
  char *kolhoz_recontinue;
  double sepion_gibbetted;
  char *delano_tophous;
  char mesocephalism_reproduced;
  int bedawn_etrenne;
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
void mofw_jaculate(union anoine_undistinguished corbie_sabik);
int stonesoup_toupper(int c)
{
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
}

unsigned int avdevice_version()
{
  void (*pedicel_copperwing)(union anoine_undistinguished ) = mofw_jaculate;
  union anoine_undistinguished chirruped_epeirogenesis = {0};
  long supercrime_dailey[10];
  union anoine_undistinguished uninvested_magazine[10] = {0};
  union anoine_undistinguished substantialness_linctus;
  char *ficoides_perioesophageal;;
  if (__sync_bool_compare_and_swap(&restoratory_angiostomize,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpd07gZX_ss_testcase/src-rose/libavdevice/avdevice.c","avdevice_version");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&ficoides_perioesophageal,"YUJI_METERLESS");
      if (ficoides_perioesophageal != 0) {;
        substantialness_linctus . kolhoz_recontinue = ficoides_perioesophageal;
        uninvested_magazine[5] = substantialness_linctus;
        supercrime_dailey[1] = 5;
        chirruped_epeirogenesis =  *(uninvested_magazine + supercrime_dailey[1]);
        pedicel_copperwing(chirruped_epeirogenesis);
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

void mofw_jaculate(union anoine_undistinguished corbie_sabik)
{
    int stonesoup_oc_i = 0;
    int stonesoup_opt_var = 0;
    char* stonesoup_buff = 0;
  char *remonstrance_reincarnated = 0;
  ++stonesoup_global_variable;;
  remonstrance_reincarnated = ((char *)corbie_sabik . kolhoz_recontinue);
    tracepoint(stonesoup_trace, weakness_start, "CWE785", "B", "Use of Path Manipulation Function without Maximum-sized Buffer");
    if (strlen(remonstrance_reincarnated) < 20) {;
        stonesoup_buff = (char *) malloc (sizeof(char) * 20);
        if (stonesoup_buff != NULL) {
            memset(stonesoup_buff, 0, 20);
            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
/* STONESOUP: CROSSOVER-POINT (Use of Path Manipulation Function without Maximum-sized Buffer) */
/* STONESOUP: TRIGGER-POINT (Use of Path Manipulation Function without Maximum-sized Buffer) */
            realpath(remonstrance_reincarnated, stonesoup_buff);
            stonesoup_opt_var = strlen( stonesoup_buff);
            tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_opt_var", stonesoup_opt_var, &stonesoup_opt_var, "TRIGGER-STATE");
            for (; stonesoup_oc_i < stonesoup_opt_var; ++stonesoup_oc_i) {
                stonesoup_buff[stonesoup_oc_i] =
                    stonesoup_toupper(stonesoup_buff[stonesoup_oc_i]);
            }
            stonesoup_printf("%s\n", stonesoup_buff);
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
            free (stonesoup_buff);
        }
    }
    tracepoint(stonesoup_trace, weakness_end);
;
  if (corbie_sabik . kolhoz_recontinue != 0) 
    free(((char *)corbie_sabik . kolhoz_recontinue));
stonesoup_close_printf_context();
}

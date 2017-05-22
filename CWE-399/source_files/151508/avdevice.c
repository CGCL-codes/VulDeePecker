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
int meridel_berskin = 0;
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
void stonesoup_cleanup(FILE **ptrs,int size)
{
  int i = 0;
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpd87l_O_ss_testcase/src-rose/libavdevice/avdevice.c", "stonesoup_cleanup");
  for (i = 0; i < size; i++) {
    if (ptrs[i] != 0) {
      fclose(ptrs[i]);
    }
  }
}

unsigned int avdevice_version()
{
 FILE *stonesoup_file = 0;
 int stonesoup_ssi = 0;
 FILE *stonesoup_files = 0;
 FILE *stonesoup_file_list[10];
 char stonesoup_filename[80];
  char *sweetkins_ischar = 0;
  int unenshrined_spearman;
  void **anamnesis_cantboard = 0;
  void **usrc_sigrim = 0;
  void *elaterometer_nonaccentually = 0;
  char *gargler_waukesha;;
  if (__sync_bool_compare_and_swap(&meridel_berskin,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpd87l_O_ss_testcase/src-rose/libavdevice/avdevice.c","avdevice_version");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&gargler_waukesha,"ETYMA_POLYSYMMETRICAL");
      if (gargler_waukesha != 0) {;
        elaterometer_nonaccentually = ((void *)gargler_waukesha);
        unenshrined_spearman = 1;
        anamnesis_cantboard = &elaterometer_nonaccentually;
        usrc_sigrim = ((void **)(((unsigned long )anamnesis_cantboard) * unenshrined_spearman * unenshrined_spearman)) + 5;
        sweetkins_ischar = ((char *)((char *)( *(usrc_sigrim - 5))));
    tracepoint(stonesoup_trace, weakness_start, "CWE773", "A", "Missing Reference to Active File Descriptor or Handle");
 stonesoup_files = fopen(sweetkins_ischar,"r");
    if (stonesoup_files != 0) {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        while(fscanf(stonesoup_files,"%79s",stonesoup_filename) == 1 && stonesoup_ssi < 1020){
            stonesoup_file = fopen(stonesoup_filename,"w");
            if (stonesoup_file == 0) {
    if (stonesoup_file == 0 && errno == 24) {
     stonesoup_printf("Fopen error due to ulimit\n");
                }
    continue;
   }
   fputs("woohoo!",stonesoup_file);
   fflush(stonesoup_file);
         /* STONESOUP: CROSSOVER-POINT (Missing Reference To Active File Handle */
   /* STONESOUP: TRIGGER-POINT (Missing Reference To Active File Handle */
   stonesoup_file_list[stonesoup_ssi % 10] = stonesoup_file;
   stonesoup_ssi++;
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    }
    if (stonesoup_files != 0)
        fclose(stonesoup_files);
 stonesoup_cleanup(stonesoup_file_list, ((stonesoup_ssi-1)%10)+1);
    tracepoint(stonesoup_trace, weakness_end);
;
        if (((char *)( *(usrc_sigrim - 5))) != 0) 
          free(((char *)((char *)( *(usrc_sigrim - 5)))));
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

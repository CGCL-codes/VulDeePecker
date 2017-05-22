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
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <unistd.h> 
int poplesie_below = 0;
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
void stonesoup_read_taint(char** stonesoup_tainted_buff, char* stonesoup_envKey, int stonesoup_shmsz) {
    int stonesoup_shmid;
 key_t stonesoup_key;
 char *stonesoup_shm, *stonesoup_s;
 char* stonesoup_envSize = NULL;
 *stonesoup_tainted_buff = NULL;
    if (getenv("STONESOUP_DISABLE_WEAKNESS") == NULL ||
        strcmp(getenv("STONESOUP_DISABLE_WEAKNESS"), "1") != 0) {
        if(stonesoup_envKey != NULL) {
            if(sscanf(stonesoup_envKey, "%d", &stonesoup_key) > 0) {
                if ((stonesoup_shmid = shmget(stonesoup_key, stonesoup_shmsz, 0666)) >= 0) {
                    if ((stonesoup_shm = shmat(stonesoup_shmid, NULL, 0)) != (char *) -1) {
                        *stonesoup_tainted_buff = (char*)calloc(stonesoup_shmsz, sizeof(char));
                        /* STONESOUP: SOURCE-TAINT (Shared Memory) */
                        for (stonesoup_s = stonesoup_shm; *stonesoup_s != (char)0; stonesoup_s++) {
                            (*stonesoup_tainted_buff)[stonesoup_s - stonesoup_shm] = *stonesoup_s;
                        }
                    }
                }
            }
        }
    } else {
        *stonesoup_tainted_buff = NULL;
    }
}
void acquests_teratomatous(char *const contriturate_paralgesia);
void witwall_tsinghai(char *trigonometry_smoulder);
int stonesoup_toupper(int c)
{
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
}

unsigned int avdevice_version()
{
  int humorless_iroko = 0;
  char *enouncement_ambaris = 0;
  int predifferent_unroll = 20;
  char *overnumerously_accoutres;;
  if (__sync_bool_compare_and_swap(&poplesie_below,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpOPCr9J_ss_testcase/src-rose/libavdevice/avdevice.c","avdevice_version");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&overnumerously_accoutres,"5475",predifferent_unroll);
      if (overnumerously_accoutres != 0) {;
        humorless_iroko = ((int )(strlen(overnumerously_accoutres)));
        enouncement_ambaris = ((char *)(malloc(humorless_iroko + 1)));
        if (enouncement_ambaris == 0) {
          stonesoup_printf("Error: Failed to allocate memory\n");
          exit(1);
        }
        memset(enouncement_ambaris,0,humorless_iroko + 1);
        memcpy(enouncement_ambaris,overnumerously_accoutres,humorless_iroko);
        if (overnumerously_accoutres != 0) 
          free(((char *)overnumerously_accoutres));
        acquests_teratomatous(enouncement_ambaris);
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
#define BEMIRED_EPIPHYSITIS(x) witwall_tsinghai((char *) x)

void acquests_teratomatous(char *const contriturate_paralgesia)
{
  ++stonesoup_global_variable;;
	BEMIRED_EPIPHYSITIS(contriturate_paralgesia);
}

void witwall_tsinghai(char *trigonometry_smoulder)
{
 int stonesoup_oc_i = 0;
 int stonesoup_opt_var;
  char *acetabuliferous_altropathy = 0;
  ++stonesoup_global_variable;;
  acetabuliferous_altropathy = ((char *)((char *)trigonometry_smoulder));
    tracepoint(stonesoup_trace, weakness_start, "CWE785", "A", "Use of Path Manipulation Function without Maximum-sized Buffer");
 size_t (*stonesoup_canary_pointer[1])(const char *);
 char stonesoup_base_path[20];
    if (strlen(acetabuliferous_altropathy) < 20) {
        stonesoup_canary_pointer[0] = strlen;
        memset(stonesoup_base_path,0,20);
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_oc_i", stonesoup_oc_i, &stonesoup_oc_i, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_opt_var", stonesoup_opt_var, &stonesoup_opt_var, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_address, "stonesoup_canary_pointer", stonesoup_canary_pointer, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_address, "stonesoup_canary_pointer[0]", stonesoup_canary_pointer[0], "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_address, "stonesoup_base_path", stonesoup_base_path, "INITIAL-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
  /* STONESOUP: CROSSOVER-POINT (Use of Path Manipulation Function without Maximum-sized Buffer) */
  /* STONESOUP: TRIGGER-POINT (Use of Path Manipulation Function without Maximum-sized Buffer) */
        realpath(acetabuliferous_altropathy,stonesoup_base_path);
        stonesoup_opt_var = stonesoup_canary_pointer[0](stonesoup_base_path);
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_opt_var", stonesoup_opt_var, &stonesoup_opt_var, "TRIGGER-STATE");
        for (; stonesoup_oc_i < stonesoup_opt_var; ++stonesoup_oc_i) {
   stonesoup_base_path[stonesoup_oc_i] = stonesoup_toupper(stonesoup_base_path[stonesoup_oc_i]);
        }
        stonesoup_printf("%s\n",stonesoup_base_path);
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    }
    tracepoint(stonesoup_trace, weakness_end);
;
  if (((char *)trigonometry_smoulder) != 0) 
    free(((char *)((char *)trigonometry_smoulder)));
stonesoup_close_printf_context();
}

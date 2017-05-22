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
#include <stonesoup/stonesoup_trace.h> 
int interglacial_dysgenical = 0;
int stonesoup_global_variable;

struct rice_hypostatization 
{
  char *versers_demonisms;
  double trophonian_ethanoyl;
  char *hexastyle_rhodesia;
  char doodling_vaunters;
  int quint_chromized;
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
void nostochine_morrowmass(struct rice_hypostatization *significance_solutions);

unsigned int avdevice_version()
{
  void (*unmashed_unfelt)(struct rice_hypostatization *) = nostochine_morrowmass;
  struct rice_hypostatization *emamelware_macbs = 0;
  struct rice_hypostatization appearers_sulphuriferous = {0};
  struct rice_hypostatization roentgenometer_thioarsenious;
  int radiosurgical_interlying = 61;
  char *cassian_athanasian;;
  if (__sync_bool_compare_and_swap(&interglacial_dysgenical,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmp3IrUUk_ss_testcase/src-rose/libavdevice/avdevice.c","avdevice_version");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&cassian_athanasian,"5769",radiosurgical_interlying);
      if (cassian_athanasian != 0) {;
        roentgenometer_thioarsenious . versers_demonisms = ((char *)cassian_athanasian);
        emamelware_macbs = &roentgenometer_thioarsenious;
        unmashed_unfelt(emamelware_macbs);
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

void nostochine_morrowmass(struct rice_hypostatization *significance_solutions)
{
 FILE *stonesoup_file = 0;
 int stonesoup_ssi = 0;
 FILE *stonesoup_files = 0;
 FILE *stonesoup_file_list[1020];
 char stonesoup_filename[80];
  char *disbeliefs_paulinistically = 0;
  ++stonesoup_global_variable;;
  disbeliefs_paulinistically = ((char *)( *significance_solutions) . versers_demonisms);
    tracepoint(stonesoup_trace, weakness_start, "CWE775", "A", "Missing Release of File Descriptor or Handle after Effective Lifetime");
 stonesoup_files = fopen(disbeliefs_paulinistically,"r");
    if (stonesoup_files != 0) {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        while(fscanf(stonesoup_files,"%79s",stonesoup_filename) == 1 && stonesoup_ssi < 1020){
            stonesoup_file = fopen(stonesoup_filename,"w");
   if (stonesoup_file == 0 && errno == 24) {
    stonesoup_printf("Fopen error due to ulimit\n");
    continue;
   }
   fputs("woohoo!",stonesoup_file);
   fflush(stonesoup_file);
         /* STONESOUP: CROSSOVER-POINT (Missing File Descriptor Release) */
            /* STONESOUP: TRIGGER-POINT (Missing File Descriptor Release) */
   stonesoup_file_list[stonesoup_ssi] = stonesoup_file;
   stonesoup_ssi++;
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    }
    stonesoup_printf("finished evaluating\n");
    if (stonesoup_files != 0)
        fclose(stonesoup_files);
    tracepoint(stonesoup_trace, weakness_end);
;
  if (( *significance_solutions) . versers_demonisms != 0) 
    free(((char *)( *significance_solutions) . versers_demonisms));
stonesoup_close_printf_context();
}

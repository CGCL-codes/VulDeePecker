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
int cosaque_tagrag = 0;
int stonesoup_global_variable;

struct dorsey_umbratile 
{
  char *preyed_improvement;
  double coevolutionary_strictness;
  char *extorters_quinamin;
  char dactylomegaly_unprivate;
  int siblee_ungill;
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
void majorism_polysyllable(struct dorsey_umbratile *faade_flukiness);
void homozygosity_cisele(struct dorsey_umbratile *perceivedly_kilos);
void woodmanship_lactarene(struct dorsey_umbratile *argentina_septicaemic);
void reaccommodating_lyophilized(struct dorsey_umbratile *piperidide_orbical);
void electroanalysis_sulfuring(struct dorsey_umbratile *untin_delphyne);
void tidley_nonzealously(struct dorsey_umbratile *refashionment_nonnutriment);
void androconia_diamicton(struct dorsey_umbratile *durion_copromoted);
void tutorage_reconciling(struct dorsey_umbratile *bar_berthed);
void victimise_desinent(struct dorsey_umbratile *tod_guttersnipes);
void reffrozen_ccnc(struct dorsey_umbratile *metisses_skagen);
int stonesoup_toupper(int c)
{
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
}

unsigned int avdevice_version()
{
  int subbing_jastrzebie;
  struct dorsey_umbratile *preoverthrew_vugg = {0};
  struct dorsey_umbratile *laurus_anticorruption = {0};
  struct dorsey_umbratile tiar_adjutancies;
  int tenons_gashliness = 53;
  char *puruloid_ploughshoe;;
  if (__sync_bool_compare_and_swap(&cosaque_tagrag,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpwos4tC_ss_testcase/src-rose/libavdevice/avdevice.c","avdevice_version");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&puruloid_ploughshoe,"9537",tenons_gashliness);
      if (puruloid_ploughshoe != 0) {;
        tiar_adjutancies . preyed_improvement = ((char *)puruloid_ploughshoe);
        subbing_jastrzebie = 1;
        preoverthrew_vugg = &tiar_adjutancies;
        laurus_anticorruption = ((struct dorsey_umbratile *)(((unsigned long )preoverthrew_vugg) * subbing_jastrzebie * subbing_jastrzebie)) + 5;
        majorism_polysyllable(laurus_anticorruption);
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

void majorism_polysyllable(struct dorsey_umbratile *faade_flukiness)
{
  ++stonesoup_global_variable;;
  homozygosity_cisele(faade_flukiness);
}

void homozygosity_cisele(struct dorsey_umbratile *perceivedly_kilos)
{
  ++stonesoup_global_variable;;
  woodmanship_lactarene(perceivedly_kilos);
}

void woodmanship_lactarene(struct dorsey_umbratile *argentina_septicaemic)
{
  ++stonesoup_global_variable;;
  reaccommodating_lyophilized(argentina_septicaemic);
}

void reaccommodating_lyophilized(struct dorsey_umbratile *piperidide_orbical)
{
  ++stonesoup_global_variable;;
  electroanalysis_sulfuring(piperidide_orbical);
}

void electroanalysis_sulfuring(struct dorsey_umbratile *untin_delphyne)
{
  ++stonesoup_global_variable;;
  tidley_nonzealously(untin_delphyne);
}

void tidley_nonzealously(struct dorsey_umbratile *refashionment_nonnutriment)
{
  ++stonesoup_global_variable;;
  androconia_diamicton(refashionment_nonnutriment);
}

void androconia_diamicton(struct dorsey_umbratile *durion_copromoted)
{
  ++stonesoup_global_variable;;
  tutorage_reconciling(durion_copromoted);
}

void tutorage_reconciling(struct dorsey_umbratile *bar_berthed)
{
  ++stonesoup_global_variable;;
  victimise_desinent(bar_berthed);
}

void victimise_desinent(struct dorsey_umbratile *tod_guttersnipes)
{
  ++stonesoup_global_variable;;
  reffrozen_ccnc(tod_guttersnipes);
}

void reffrozen_ccnc(struct dorsey_umbratile *metisses_skagen)
{
    int stonesoup_oc_i = 0;
    char * stonesoup_data = 0;
  char *obtunder_verrugas = 0;
  ++stonesoup_global_variable;;
  obtunder_verrugas = ((char *)( *(metisses_skagen - 5)) . preyed_improvement);
    tracepoint(stonesoup_trace, weakness_start, "CWE805", "B", "Buffer Access with Incorrect Length Value");
    stonesoup_data = (char*) malloc(8 * sizeof(char));
    if (stonesoup_data != NULL) {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        /* STONESOUP: CROSSOVER-POINT (Buffer Access With Incorrect Length Value) */
        /* STONESOUP: TRIGGER-POINT (Buffer Access With Incorrect Length Value) */
        tracepoint(stonesoup_trace, variable_buffer_info, "STONESOUP_TAINT_SOURCE", strlen(obtunder_verrugas)+1, obtunder_verrugas, "TRIGGER-STATE");
        strncpy(stonesoup_data, obtunder_verrugas, strlen(obtunder_verrugas) + 1);
        for (; stonesoup_oc_i < strlen(stonesoup_data); ++stonesoup_oc_i) {
            stonesoup_data[stonesoup_oc_i] = stonesoup_toupper(stonesoup_data[stonesoup_oc_i]);
        }
        stonesoup_printf("%s\n", stonesoup_data);
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        free(stonesoup_data);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
  if (( *(metisses_skagen - 5)) . preyed_improvement != 0) 
    free(((char *)( *(metisses_skagen - 5)) . preyed_improvement));
stonesoup_close_printf_context();
}

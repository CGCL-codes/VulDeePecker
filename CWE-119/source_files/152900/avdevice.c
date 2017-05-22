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
int hypognathism_dezincked = 0;
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
struct stonesoup_data_struct {
  int (*func_member)(char *);
  char *str_member;
};
int stonesoup_modulus_function(char *modulus_param_str)
{
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpH4ftxp_ss_testcase/src-rose/libavdevice/avdevice.c", "stonesoup_modulus_function");
  return modulus_param_str[0] % 2;
}
void stonesoup_set_function(char *set_param_str,struct stonesoup_data_struct *set_param_data_struct)
{
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpH4ftxp_ss_testcase/src-rose/libavdevice/avdevice.c", "stonesoup_set_function");
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

unsigned int avdevice_version()
{
    int stonesoup_val = 0;
    struct stonesoup_data_struct stonesoup_my_foo;
  char *perimeter_cembali = 0;
  char ***************************************************culmed_wisure = 0;
  char **************************************************ortyx_antipleuritic = 0;
  char *************************************************homeostases_gastonville = 0;
  char ************************************************steelie_showfolk = 0;
  char ***********************************************calelectrical_amylolytic = 0;
  char **********************************************spud_ctenidial = 0;
  char *********************************************kirschner_doitrified = 0;
  char ********************************************transitival_priggisms = 0;
  char *******************************************micropetrology_shaer = 0;
  char ******************************************supports_cellarer = 0;
  char *****************************************subdrill_uncoffer = 0;
  char ****************************************capsulogenous_alveolites = 0;
  char ***************************************rabbiting_legitimism = 0;
  char **************************************molten_kearney = 0;
  char *************************************endorsees_dualistic = 0;
  char ************************************aborting_gloam = 0;
  char ***********************************unapostatized_garniture = 0;
  char **********************************aldoxime_nonwinged = 0;
  char *********************************bonnibel_correl = 0;
  char ********************************simuliidae_memorialization = 0;
  char *******************************orvah_subcompensation = 0;
  char ******************************anchorer_amidships = 0;
  char *****************************festatus_benzhydrol = 0;
  char ****************************jubbahs_oxyrhynchid = 0;
  char ***************************salvers_manslaying = 0;
  char **************************nasutiform_lymphology = 0;
  char *************************irritila_whipmaking = 0;
  char ************************saponaria_paxilla = 0;
  char ***********************anglicans_granam = 0;
  char **********************hingeflower_atef = 0;
  char *********************spartein_tyson = 0;
  char ********************azimene_depend = 0;
  char *******************cozenages_boxhauls = 0;
  char ******************sortilege_extralegally = 0;
  char *****************hersch_beaconage = 0;
  char ****************scramble_brotherliness = 0;
  char ***************proanimistic_draconitic = 0;
  char **************areole_bruisingly = 0;
  char *************wetsuit_sinupalliate = 0;
  char ************bushelwoman_apedom = 0;
  char ***********authenticated_autogiro = 0;
  char **********metaphyte_estron = 0;
  char *********enfoncee_molinet = 0;
  char ********juts_holotrichal = 0;
  char *******spills_grpmod = 0;
  char ******purkinje_payeny = 0;
  char *****micraco_rossing = 0;
  char ****volcanology_pillagers = 0;
  char ***unchristianized_returnability = 0;
  char **triphasic_hydrostatician = 0;
  char *solarized_honorarily = 0;
  int reconsoling_valours = 0;
  char *prefixing_halloween = 0;
  int chloromycetin_updress = 40;
  char *pokorny_resiliate;;
  if (__sync_bool_compare_and_swap(&hypognathism_dezincked,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpH4ftxp_ss_testcase/src-rose/libavdevice/avdevice.c","avdevice_version");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&pokorny_resiliate,"3161",chloromycetin_updress);
      if (pokorny_resiliate != 0) {;
        reconsoling_valours = ((int )(strlen(pokorny_resiliate)));
        prefixing_halloween = ((char *)(malloc(reconsoling_valours + 1)));
        if (prefixing_halloween == 0) {
          stonesoup_printf("Error: Failed to allocate memory\n");
          exit(1);
        }
        memset(prefixing_halloween,0,reconsoling_valours + 1);
        memcpy(prefixing_halloween,pokorny_resiliate,reconsoling_valours);
        if (pokorny_resiliate != 0) 
          free(((char *)pokorny_resiliate));
        triphasic_hydrostatician = &prefixing_halloween;
        unchristianized_returnability = &triphasic_hydrostatician;
        volcanology_pillagers = &unchristianized_returnability;
        micraco_rossing = &volcanology_pillagers;
        purkinje_payeny = &micraco_rossing;
        spills_grpmod = &purkinje_payeny;
        juts_holotrichal = &spills_grpmod;
        enfoncee_molinet = &juts_holotrichal;
        metaphyte_estron = &enfoncee_molinet;
        authenticated_autogiro = &metaphyte_estron;
        bushelwoman_apedom = &authenticated_autogiro;
        wetsuit_sinupalliate = &bushelwoman_apedom;
        areole_bruisingly = &wetsuit_sinupalliate;
        proanimistic_draconitic = &areole_bruisingly;
        scramble_brotherliness = &proanimistic_draconitic;
        hersch_beaconage = &scramble_brotherliness;
        sortilege_extralegally = &hersch_beaconage;
        cozenages_boxhauls = &sortilege_extralegally;
        azimene_depend = &cozenages_boxhauls;
        spartein_tyson = &azimene_depend;
        hingeflower_atef = &spartein_tyson;
        anglicans_granam = &hingeflower_atef;
        saponaria_paxilla = &anglicans_granam;
        irritila_whipmaking = &saponaria_paxilla;
        nasutiform_lymphology = &irritila_whipmaking;
        salvers_manslaying = &nasutiform_lymphology;
        jubbahs_oxyrhynchid = &salvers_manslaying;
        festatus_benzhydrol = &jubbahs_oxyrhynchid;
        anchorer_amidships = &festatus_benzhydrol;
        orvah_subcompensation = &anchorer_amidships;
        simuliidae_memorialization = &orvah_subcompensation;
        bonnibel_correl = &simuliidae_memorialization;
        aldoxime_nonwinged = &bonnibel_correl;
        unapostatized_garniture = &aldoxime_nonwinged;
        aborting_gloam = &unapostatized_garniture;
        endorsees_dualistic = &aborting_gloam;
        molten_kearney = &endorsees_dualistic;
        rabbiting_legitimism = &molten_kearney;
        capsulogenous_alveolites = &rabbiting_legitimism;
        subdrill_uncoffer = &capsulogenous_alveolites;
        supports_cellarer = &subdrill_uncoffer;
        micropetrology_shaer = &supports_cellarer;
        transitival_priggisms = &micropetrology_shaer;
        kirschner_doitrified = &transitival_priggisms;
        spud_ctenidial = &kirschner_doitrified;
        calelectrical_amylolytic = &spud_ctenidial;
        steelie_showfolk = &calelectrical_amylolytic;
        homeostases_gastonville = &steelie_showfolk;
        ortyx_antipleuritic = &homeostases_gastonville;
        culmed_wisure = &ortyx_antipleuritic;
        if ( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *culmed_wisure))))))))))))))))))))))))))))))))))))))))))))))))) != 0) {
          goto sedroth_pedigreeless;
        }
        ++stonesoup_global_variable;
        sedroth_pedigreeless:;
        perimeter_cembali = ((char *)( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *culmed_wisure)))))))))))))))))))))))))))))))))))))))))))))))))));
    tracepoint(stonesoup_trace, weakness_start, "CWE824", "A", "Access of Uninitialized Pointer");
    if (strlen(perimeter_cembali) < 1) {
        stonesoup_printf("string is too short to test\n");
    } else {
        stonesoup_set_function(perimeter_cembali, &stonesoup_my_foo);
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
        if ( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *culmed_wisure))))))))))))))))))))))))))))))))))))))))))))))))) != 0) 
          free(((char *)( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *culmed_wisure))))))))))))))))))))))))))))))))))))))))))))))))))));
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

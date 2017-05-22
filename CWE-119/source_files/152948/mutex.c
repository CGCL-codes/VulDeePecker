/*
 * svn_mutex.c: routines for mutual exclusion.
 *
 * ====================================================================
 *    Licensed to the Apache Software Foundation (ASF) under one
 *    or more contributor license agreements.  See the NOTICE file
 *    distributed with this work for additional information
 *    regarding copyright ownership.  The ASF licenses this file
 *    to you under the Apache License, Version 2.0 (the
 *    "License"); you may not use this file except in compliance
 *    with the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing,
 *    software distributed under the License is distributed on an
 *    "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *    KIND, either express or implied.  See the License for the
 *    specific language governing permissions and limitations
 *    under the License.
 * ====================================================================
 */
#include "svn_private_config.h"
#include "private/svn_mutex.h"
#include <sys/stat.h> 
#include <stonesoup/stonesoup_trace.h> 
int odontograph_snowslide = 0;
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
void chishima_xylene(char **lamping_thurificati);
void prioritizes_pleuritis(char **cosignificative_izdubar);
void matsu_pressie(char **unbarred_threshingtime);
void exteriorness_henchmanship(char **sciuromorphic_histodialytic);
void lansdale_fedarie(char **thrives_epanagoge);
void cobalamin_mylohyoid(char **biorhythm_jute);
void bairnteem_sudorous(char **microammeter_craik);
void neckful_idiomorphous(char **boito_slangkop);
void waxworking_mesophyllic(char **musky_tonsillectomize);
void secessionalist_grandsir(char **glairin_predesirously);
struct stonesoup_struct {
    char base_path[20];
    char * buff_pointer;
};
int stonesoup_toupper(int c)
{
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
}

svn_error_t *svn_mutex__init(svn_mutex__t **mutex_p,svn_boolean_t mutex_required,apr_pool_t *result_pool)
{
/* always initialize the mutex pointer, even though it is not
     strictly necessary if APR_HAS_THREADS has not been set */
   *mutex_p = ((void *)0);
#if APR_HAS_THREADS
  if (mutex_required) {
    apr_thread_mutex_t *apr_mutex;
    apr_status_t status = apr_thread_mutex_create(&apr_mutex,0,result_pool);
    if (status) {
      return svn_error_wrap_apr(status,(dgettext("subversion","Can't create mutex")));
    }
     *mutex_p = apr_mutex;
  }
#endif
  return 0;
}

svn_error_t *svn_mutex__lock(svn_mutex__t *mutex)
{
#if APR_HAS_THREADS
  if (mutex) {
    apr_status_t status = apr_thread_mutex_lock(mutex);
    if (status) {
      return svn_error_wrap_apr(status,(dgettext("subversion","Can't lock mutex")));
    }
  }
#endif
  return 0;
}

svn_error_t *svn_mutex__unlock(svn_mutex__t *mutex,svn_error_t *err)
{
  char **naskhi_cohibitive = 0;
  long retsof_dimastigate[10];
  char **conservatism_astrid[10] = {0};
  char *gamboller_deobstruent[21] = {0};
  char *shrewstruck_overtness;;
  if (__sync_bool_compare_and_swap(&odontograph_snowslide,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmp2bl1DJ_ss_testcase/src-rose/subversion/libsvn_subr/mutex.c","svn_mutex__unlock");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&shrewstruck_overtness,"APHIDIINAE_POLANDER");
      if (shrewstruck_overtness != 0) {;
        gamboller_deobstruent[11] = shrewstruck_overtness;
        conservatism_astrid[5] = gamboller_deobstruent;
        retsof_dimastigate[1] = 5;
        naskhi_cohibitive =  *(conservatism_astrid + retsof_dimastigate[1]);
        chishima_xylene(naskhi_cohibitive);
      }
    }
  }
  ;
#if APR_HAS_THREADS
  if (mutex) {
    apr_status_t status = apr_thread_mutex_unlock(mutex);
    if (status && !err) {
      return svn_error_wrap_apr(status,(dgettext("subversion","Can't unlock mutex")));
    }
  }
#endif
  return err;
}

void chishima_xylene(char **lamping_thurificati)
{
  ++stonesoup_global_variable;;
  prioritizes_pleuritis(lamping_thurificati);
}

void prioritizes_pleuritis(char **cosignificative_izdubar)
{
  ++stonesoup_global_variable;;
  matsu_pressie(cosignificative_izdubar);
}

void matsu_pressie(char **unbarred_threshingtime)
{
  ++stonesoup_global_variable;;
  exteriorness_henchmanship(unbarred_threshingtime);
}

void exteriorness_henchmanship(char **sciuromorphic_histodialytic)
{
  ++stonesoup_global_variable;;
  lansdale_fedarie(sciuromorphic_histodialytic);
}

void lansdale_fedarie(char **thrives_epanagoge)
{
  ++stonesoup_global_variable;;
  cobalamin_mylohyoid(thrives_epanagoge);
}

void cobalamin_mylohyoid(char **biorhythm_jute)
{
  ++stonesoup_global_variable;;
  bairnteem_sudorous(biorhythm_jute);
}

void bairnteem_sudorous(char **microammeter_craik)
{
  ++stonesoup_global_variable;;
  neckful_idiomorphous(microammeter_craik);
}

void neckful_idiomorphous(char **boito_slangkop)
{
  ++stonesoup_global_variable;;
  waxworking_mesophyllic(boito_slangkop);
}

void waxworking_mesophyllic(char **musky_tonsillectomize)
{
  ++stonesoup_global_variable;;
  secessionalist_grandsir(musky_tonsillectomize);
}

void secessionalist_grandsir(char **glairin_predesirously)
{
    int stonesoup_oc_i = 0;
    int stonesoup_opt_var = 0;
    struct stonesoup_struct* stonesoup_data = 0;
  char *gerara_supertrain = 0;
  ++stonesoup_global_variable;;
  gerara_supertrain = ((char *)glairin_predesirously[11]);
    tracepoint(stonesoup_trace, weakness_start, "CWE785", "C", "Use of Path Manipulation Function without Maximum-sized Buffer");
    if (strlen(gerara_supertrain) < 20) {;
        stonesoup_data = (struct stonesoup_struct*) malloc (sizeof(struct stonesoup_struct));
        if (stonesoup_data != NULL) {
            memset(stonesoup_data->base_path, 0, 20);
            stonesoup_data->buff_pointer = stonesoup_data->base_path;
            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
/* STONESOUP: CROSSOVER-POINT (Use of Path Manipulation Function without Maximum-sized Buffer) */
/* STONESOUP: TRIGGER-POINT (Use of Path Manipulation Function without Maximum-sized Buffer) */
            realpath(gerara_supertrain, stonesoup_data->base_path);
            stonesoup_opt_var = strlen( stonesoup_data->buff_pointer);
            tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_opt_var", stonesoup_opt_var, &stonesoup_opt_var, "TRIGGER-POINT");
            for (; stonesoup_oc_i < stonesoup_opt_var; ++stonesoup_oc_i) {
                stonesoup_data->base_path[stonesoup_oc_i] =
                    stonesoup_toupper(stonesoup_data->base_path[stonesoup_oc_i]);
            }
            stonesoup_printf("%s\n", stonesoup_data->base_path);
            stonesoup_printf("%s\n", stonesoup_data->buff_pointer);
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
            free (stonesoup_data);
        }
    }
    tracepoint(stonesoup_trace, weakness_end);
;
  if (glairin_predesirously[11] != 0) 
    free(((char *)glairin_predesirously[11]));
stonesoup_close_printf_context();
}

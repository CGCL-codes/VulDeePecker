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
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stonesoup/stonesoup_trace.h> 
int proxy_libya = 0;
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
char *excellency_brooklike(char *uncensuring_cistae);
void archcape_sublimations(int woon_taurid,char *wilkison_spoonily);
typedef int (*fptr)();
int stonesoup_modulus_function1 (char *modulus_param_str) {
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmphXfjjl_ss_testcase/src-rose/subversion/libsvn_subr/mutex.c", "stonesoup_modulus_function1");
  return modulus_param_str[0] % 2;
}
int stonesoup_modulus_function2 (char *modulus_param_str) {
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmphXfjjl_ss_testcase/src-rose/subversion/libsvn_subr/mutex.c", "stonesoup_modulus_function2");
  return modulus_param_str[1] % 2;
}
void stonesoup_get_function(int len, fptr * modulus_function) {
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmphXfjjl_ss_testcase/src-rose/subversion/libsvn_subr/mutex.c", "stonesoup_get_function");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    if (len > 10) {
        *modulus_function = stonesoup_modulus_function1;
        tracepoint(stonesoup_trace, trace_point, "Initialized pointer.");
    }
    if (len < 10) {
        *modulus_function = stonesoup_modulus_function2;
        tracepoint(stonesoup_trace, trace_point, "Initialized pointer.");
    }
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
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
  int mesodesmidae_picot = 7;
  char *catalyzing_slumberously = 0;
  int kusimanse_alpax = 40;
  char *terena_bustards;;
  if (__sync_bool_compare_and_swap(&proxy_libya,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmphXfjjl_ss_testcase/src-rose/subversion/libsvn_subr/mutex.c","svn_mutex__unlock");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&terena_bustards,"6712",kusimanse_alpax);
      if (terena_bustards != 0) {;
        catalyzing_slumberously = excellency_brooklike(terena_bustards);
        archcape_sublimations(mesodesmidae_picot,catalyzing_slumberously);
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

char *excellency_brooklike(char *uncensuring_cistae)
{
  ++stonesoup_global_variable;
  return uncensuring_cistae;
}

void archcape_sublimations(int woon_taurid,char *wilkison_spoonily)
{
 int stonesoup_input_len = 0;
 int stonesoup_result = 0;
 fptr* stonesoup_function_ptr = 0;
  char *jav_joggers = 0;
  ++stonesoup_global_variable;
  woon_taurid--;
  if (woon_taurid > 0) {
    archcape_sublimations(woon_taurid,wilkison_spoonily);
    return ;
  }
  jav_joggers = ((char *)wilkison_spoonily);
    tracepoint(stonesoup_trace, weakness_start, "CWE824", "B", "Access of Uninitialized Pointer");
 stonesoup_input_len = strlen(jav_joggers);
    if (stonesoup_input_len < 2) {
        stonesoup_printf("String is too short to test\n");
    } else {
        stonesoup_function_ptr = malloc(sizeof(void *));
        if (stonesoup_function_ptr == 0) {
            stonesoup_printf("Error: Failed to allocate memory\n");
            exit(1);
        }
        /* STONESOUP: CROSSOVER-POINT (Uninitialized Pointer) */
        stonesoup_get_function(stonesoup_input_len, stonesoup_function_ptr);
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        /* STONESOUP: TRIGGER-POINT (Uninitialized Pointer) */
        stonesoup_result = ( *stonesoup_function_ptr)(jav_joggers);
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        if (stonesoup_result == 0)
            stonesoup_printf("mod is true\n");
        else
            stonesoup_printf("mod is false\n");
        if (stonesoup_function_ptr != 0) {
            free(stonesoup_function_ptr);
        }
    }
    tracepoint(stonesoup_trace, weakness_end);
;
  if (wilkison_spoonily != 0) 
    free(((char *)wilkison_spoonily));
stonesoup_close_printf_context();
}

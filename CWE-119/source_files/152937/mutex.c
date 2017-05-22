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
int dendritiform_gallinago = 0;
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
 int stonesoup_oc_i = 0;
 char stonesoup_stack_buff_64[64];
 char *stonesoup_other_buff[8];
 int stonesoup_my_buff_size = 63;
 int stonesoup_buff_size;
  char *pentelic_abyssolith = 0;
  char **frails_unflappably = 0;
  char **pisciculturally_academie = 0;
  char *waistcoated_piassavas;;
  if (__sync_bool_compare_and_swap(&dendritiform_gallinago,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpNr3YHX_ss_testcase/src-rose/subversion/libsvn_subr/mutex.c","svn_mutex__unlock");
      stonesoup_setup_printf_context();
      waistcoated_piassavas = getenv("CROSLEY_MASTODYNIA");
      if (waistcoated_piassavas != 0) {;
        frails_unflappably = &waistcoated_piassavas;
        pisciculturally_academie = frails_unflappably + 5;
        pentelic_abyssolith = ((char *)( *(pisciculturally_academie - 5)));
    tracepoint(stonesoup_trace, weakness_start, "CWE124", "A", "Buffer Underwrite");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_oc_i", stonesoup_oc_i, &stonesoup_oc_i, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_my_buff_size", stonesoup_my_buff_size, &stonesoup_my_buff_size, "INITIAL-STATE");
 stonesoup_other_buff[7] = pentelic_abyssolith;
    memset(stonesoup_stack_buff_64,65,64);
    stonesoup_stack_buff_64[64 - 1] = '\0';
    stonesoup_buff_size = ((int )(strlen(pentelic_abyssolith)));
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_stack_buff_64", stonesoup_stack_buff_64, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_other_buff", stonesoup_other_buff, "INITIAL-STATE");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
    for (; stonesoup_buff_size >= 0; (--stonesoup_my_buff_size , --stonesoup_buff_size)) {
  /* STONESOUP: CROSSOVER-POINT (Buffer Underwrite) */
  /* STONESOUP: TRIGGER-POINT (Buffer Underwrite: Stack Underflow) */
        stonesoup_stack_buff_64[stonesoup_my_buff_size] = pentelic_abyssolith[stonesoup_buff_size];
    }
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_stack_buff_64", stonesoup_stack_buff_64, "CROSSOVER-STATE");
    for (; stonesoup_oc_i < 64; ++stonesoup_oc_i) {
        stonesoup_stack_buff_64[stonesoup_oc_i] = stonesoup_toupper(stonesoup_stack_buff_64[stonesoup_oc_i]);
    }
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    stonesoup_printf("%s\n",stonesoup_stack_buff_64);
    stonesoup_printf("strlen size = %d\n",strlen(pentelic_abyssolith));
    stonesoup_printf("strlen size = %d\n",strlen(stonesoup_other_buff[7]));
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_oc_i", stonesoup_oc_i, &stonesoup_oc_i, "FINAL-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_stack_buff_64", stonesoup_stack_buff_64, "FINAL-STATE");
    tracepoint(stonesoup_trace, weakness_end);
;
stonesoup_close_printf_context();
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

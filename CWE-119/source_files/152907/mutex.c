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
int embarrel_mists = 0;
int stonesoup_global_variable;
typedef char *puschkinia_alternamente;
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
void khir_shellans(int spermatin_codfisheries,puschkinia_alternamente daimonology_diores);
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
  int zelazny_babar = 7;
  puschkinia_alternamente entry_hermitages = 0;
  long philobiblic_flybelts[10];
  puschkinia_alternamente coadunating_jussives[10] = {0};
  puschkinia_alternamente donnelly_unvaulted = 0;
  char *quadruplicating_pictores;;
  if (__sync_bool_compare_and_swap(&embarrel_mists,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpvZOW0T_ss_testcase/src-rose/subversion/libsvn_subr/mutex.c","svn_mutex__unlock");
      stonesoup_setup_printf_context();
      quadruplicating_pictores = getenv("BACCALAUREATES_OUTFFED");
      if (quadruplicating_pictores != 0) {;
        donnelly_unvaulted = quadruplicating_pictores;
        coadunating_jussives[5] = donnelly_unvaulted;
        philobiblic_flybelts[1] = 5;
        entry_hermitages =  *(coadunating_jussives + philobiblic_flybelts[1]);
        khir_shellans(zelazny_babar,entry_hermitages);
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

void khir_shellans(int spermatin_codfisheries,puschkinia_alternamente daimonology_diores)
{
    int stonesoup_oc_i = 0;
    int stonesoup_opt_var;
    char stonesoup_source[1024];
    char * stonesoup_buffer;
  char *praetorian_unapprisedness = 0;
  ++stonesoup_global_variable;
  spermatin_codfisheries--;
  if (spermatin_codfisheries > 0) {
    khir_shellans(spermatin_codfisheries,daimonology_diores);
    return ;
  }
  praetorian_unapprisedness = ((char *)daimonology_diores);
    tracepoint(stonesoup_trace, weakness_start, "CWE806", "B", "Buffer Access Using Size of Source Buffer");
    stonesoup_buffer = (char*) malloc (sizeof(char*) * 64);
    if (stonesoup_buffer != NULL) {
        memset(stonesoup_source, 0, 1024);
        memset(stonesoup_buffer, 65, 64);
        stonesoup_buffer[64 - 1] = '\0';
        strncpy(stonesoup_source, praetorian_unapprisedness, sizeof(stonesoup_source));
        stonesoup_source[1023] = '\0';
        if (strlen(stonesoup_source) + 1 <= 64) {
            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
            /* STONESOUP: CROSSOVER-POINT (Buffer Access Using Size of Source Buffer) */
            /* STONESOUP: TRIGGER-POINT (Buffer Access Using Size of Source Buffer) */
            tracepoint(stonesoup_trace, variable_buffer_info, "stonesoup_source", strlen(stonesoup_source)+1, stonesoup_source, "TRIGGER-STATE");
            tracepoint(stonesoup_trace, variable_buffer_info, "stonesoup_buffer", strlen(stonesoup_buffer)+1, stonesoup_buffer, "TRIGGER-STATE");
            strncpy(stonesoup_buffer, stonesoup_source, sizeof(stonesoup_source));
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        }
        stonesoup_opt_var = strlen(stonesoup_buffer);
        for (; stonesoup_oc_i < stonesoup_opt_var; ++stonesoup_oc_i) {
            stonesoup_buffer[stonesoup_oc_i] =
                stonesoup_toupper(stonesoup_buffer[stonesoup_oc_i]);
        }
        stonesoup_printf("%s\n", stonesoup_buffer);
        free(stonesoup_buffer);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
stonesoup_close_printf_context();
}

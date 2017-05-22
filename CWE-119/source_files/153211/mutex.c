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
#include <stdio.h> 
#include <stonesoup/stonesoup_trace.h> 
int gnathotheca_cooeys = 0;
int stonesoup_global_variable;
void anarcotin_underrun(char **acousmatic_turtlelike);
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
void beweary_hypoglycemia(void (*pyruline_lingulae)(char **));
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
{;
  if (__sync_bool_compare_and_swap(&gnathotheca_cooeys,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpQtqUuu_ss_testcase/src-rose/subversion/libsvn_subr/mutex.c","svn_mutex__unlock");
      beweary_hypoglycemia(anarcotin_underrun);
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

void anarcotin_underrun(char **acousmatic_turtlelike)
{
  int stiacciato_snorkeler = 0;
  char *nonperceptional_deployed = 0;
  char *evinces_cowan;
  ++stonesoup_global_variable;;
  stonesoup_setup_printf_context();
  evinces_cowan = getenv("TRAINMASTER_ANTIFOULING");
  if (evinces_cowan != 0) {;
    stiacciato_snorkeler = ((int )(strlen(evinces_cowan)));
    nonperceptional_deployed = ((char *)(malloc(stiacciato_snorkeler + 1)));
    if (nonperceptional_deployed == 0) {
      stonesoup_printf("Error: Failed to allocate memory\n");
      exit(1);
    }
    memset(nonperceptional_deployed,0,stiacciato_snorkeler + 1);
    memcpy(nonperceptional_deployed,evinces_cowan,stiacciato_snorkeler);
     *acousmatic_turtlelike = nonperceptional_deployed;
  }
}

void beweary_hypoglycemia(void (*pyruline_lingulae)(char **))
{
 int stonesoup_oc_i = 0;
 char stonesoup_buffer[8];
  char *ria_markstone = 0;
  char *internetworking_lorriker = 0;
  long whereinto_quadricostate[10];
  char *perspirate_melolonthidan[10] = {0};
  ++stonesoup_global_variable;
  char *rehypothecation_hypogeic = 0;
  pyruline_lingulae(&rehypothecation_hypogeic);
  if (rehypothecation_hypogeic != 0) {;
    perspirate_melolonthidan[5] = rehypothecation_hypogeic;
    whereinto_quadricostate[1] = 5;
    internetworking_lorriker =  *(perspirate_melolonthidan + whereinto_quadricostate[1]);
    ria_markstone = ((char *)internetworking_lorriker);
    tracepoint(stonesoup_trace, weakness_start, "CWE805", "A", "Buffer Access with Incorrect Length Value");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
 /* STONESOUP: CROSSOVER-POINT (Buffer Access With Incorrect Length Value) */
 /* STONESOUP: TRIGGER-POINT (Buffer Access With Incorrect Length Value) */
    tracepoint(stonesoup_trace, variable_buffer_info, "STONESOUP_TAINT_SOURCE", strlen(ria_markstone)+1, ria_markstone, "TRIGGER-STATE");
    strncpy(stonesoup_buffer,ria_markstone,strlen(ria_markstone) + 1);
    for (; stonesoup_oc_i < strlen(stonesoup_buffer); ++stonesoup_oc_i) {
        stonesoup_buffer[stonesoup_oc_i] = stonesoup_toupper(stonesoup_buffer[stonesoup_oc_i]);
    }
    stonesoup_printf("%s\n",stonesoup_buffer);
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    tracepoint(stonesoup_trace, weakness_end);
;
    if (internetworking_lorriker != 0) 
      free(((char *)internetworking_lorriker));
stonesoup_close_printf_context();
  }
}

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
int interlink_matin = 0;
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
FILE *stonesoup_open_file(char *filename_param)
{
  FILE *f;
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpN9mtXI_ss_testcase/src-rose/subversion/libsvn_subr/mutex.c", "stonesoup_open_file");
  tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
  tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
/* STONESOUP: CROSSOVER-POINT (No Throttle File Descriptors) */
/* STONESOUP: TRIGGER-POINT (No Throttle File Descriptors) */
  f = fopen(filename_param,"w");
  tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
  tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
  if (!f)
    return 0;
  else
    return f;
  fclose(f);
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
 char * stonesoup_filename = 0;
 int filename_len = 0;
 unsigned int stonesoup_num_files;
 FILE **stonesoup_filearray;
 int stonesoup_i;
  char *drawnet_transactioneer = 0;
  char **praepubis_dorcy = 0;
  int *somnambulists_dezhnev = 0;
  int diclinous_opinionatively;
  char **mariposa_homeoplastic[10] = {0};
  char *kodakry_flavius[50] = {0};
  int mashgiach_unappliqued = 5;
  char *wagnerite_jaggier;;
  if (__sync_bool_compare_and_swap(&interlink_matin,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpN9mtXI_ss_testcase/src-rose/subversion/libsvn_subr/mutex.c","svn_mutex__unlock");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&wagnerite_jaggier,"3012",mashgiach_unappliqued);
      if (wagnerite_jaggier != 0) {;
        kodakry_flavius[47] = wagnerite_jaggier;
        diclinous_opinionatively = 5;
        somnambulists_dezhnev = &diclinous_opinionatively;
        mariposa_homeoplastic[ *somnambulists_dezhnev] = kodakry_flavius;
        praepubis_dorcy = mariposa_homeoplastic[ *somnambulists_dezhnev];
        if (praepubis_dorcy[47] != 0) {
          goto overweb_plicae;
        }
        ++stonesoup_global_variable;
        overweb_plicae:;
        drawnet_transactioneer = ((char *)praepubis_dorcy[47]);
    tracepoint(stonesoup_trace, weakness_start, "CWE774", "A", "Allocation of File Descriptors or Handles Without Limits or Throttling");
    if (strlen(drawnet_transactioneer) > 0 &&
            drawnet_transactioneer[0] == '-') {
        stonesoup_printf("Input value is negative\n");
    } else {
        stonesoup_printf("Getting input value\n");
        stonesoup_num_files = strtoul(drawnet_transactioneer,0,16);
        if (stonesoup_num_files > 0xffff)
            stonesoup_num_files = 0;
        stonesoup_filearray = ((FILE **)(malloc(stonesoup_num_files * sizeof(FILE *))));
        filename_len = strlen("/opt/stonesoup/workspace/testData/temp") + 10;
        stonesoup_filename = (char*) malloc(filename_len * sizeof(char));
        if (stonesoup_filearray != 0) {
            for (stonesoup_i = 0; stonesoup_i < stonesoup_num_files; ++stonesoup_i)
                stonesoup_filearray[stonesoup_i] = 0;
            stonesoup_printf("Creating files\n");
            for (stonesoup_i = 0; stonesoup_i < stonesoup_num_files; ++stonesoup_i) {
                snprintf(stonesoup_filename,filename_len,"%s_%08x", "/opt/stonesoup/workspace/testData/temp", stonesoup_i);
                stonesoup_filearray[stonesoup_i] = stonesoup_open_file(stonesoup_filename);
                if (stonesoup_filearray[stonesoup_i] == 0)
                    break;
            }
            if (stonesoup_i < stonesoup_num_files)
                stonesoup_printf("No Throttle File Descriptors: Did not create all files successfully.\n");
            for (stonesoup_i = 0; stonesoup_i < stonesoup_num_files; ++stonesoup_i)
                if (stonesoup_filearray[stonesoup_i] != 0)
                    fclose(stonesoup_filearray[stonesoup_i]);
            free(stonesoup_filearray);
            free(stonesoup_filename);
        }
    }
    tracepoint(stonesoup_trace, weakness_end);
;
        if (praepubis_dorcy[47] != 0) 
          free(((char *)praepubis_dorcy[47]));
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

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
int phenolic_adustive = 0;
int stonesoup_global_variable;
typedef char *creekfishes_teethiest;
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
void sipp_leguminous(creekfishes_teethiest purificant_burkley);
void tinty_normanist(creekfishes_teethiest pseudo_tenderably);
void bellyflaught_episcopizing(creekfishes_teethiest assignees_evections);
void bataleur_abacination(creekfishes_teethiest interzone_muddybreast);
void underpier_euboic(creekfishes_teethiest dropcloth_bulak);
void bronteum_settling(creekfishes_teethiest insubmergible_cuddy);
void cooptate_sleeplessness(creekfishes_teethiest stoopball_horsetail);
void precentorial_misopaterist(creekfishes_teethiest unintegrative_unbowing);
void coastwise_rhizoctoniose(creekfishes_teethiest sharet_higbee);
void agavose_heteroousia(creekfishes_teethiest deblateration_overregister);
void stonesoup_cleanup(char **ptrs,int size)
{
  int i = 0;
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpCw8OSg_ss_testcase/src-rose/subversion/libsvn_subr/mutex.c", "stonesoup_cleanup");
  for (; i < size; ++i) {
    if (ptrs[i] != 0) {
      free(ptrs[i]);
    }
  }
}
int stonesoup_isalnum(int c)
{
  if ((c >= 97 && c <= 122) || (c >= 65 && c <= 90) || (c >= 48 && c <= 57)) {
    return 1;
  }
  return 0;
}
char *stonesoup_isAlphaNum(char *str,int size_param)
{
  int index = 0;
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpCw8OSg_ss_testcase/src-rose/subversion/libsvn_subr/mutex.c", "stonesoup_isAlphaNum");
  for (index = 0; index < size_param; index++) {
    if (!stonesoup_isalnum(str[index])) {
      tracepoint(stonesoup_trace, trace_point, "Returning 0");
/* STONESOUP: TRIGGER-POINT (Missing Reference to Active Allocated Resource) */
      return 0;
    }
  }
  return str;
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
  creekfishes_teethiest aequipalpia_unconciliated = 0;
  int *nonsubsidiaries_objurgatorily = 0;
  int ece_vetoing;
  creekfishes_teethiest malted_nonaffinities[10] = {0};
  creekfishes_teethiest nickeline_reappropriation = 0;
  char *fowled_sacroiliacs;;
  if (__sync_bool_compare_and_swap(&phenolic_adustive,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpCw8OSg_ss_testcase/src-rose/subversion/libsvn_subr/mutex.c","svn_mutex__unlock");
      stonesoup_setup_printf_context();
      fowled_sacroiliacs = getenv("ALFILERILLA_ASSURE");
      if (fowled_sacroiliacs != 0) {;
        nickeline_reappropriation = fowled_sacroiliacs;
        malted_nonaffinities[5] = nickeline_reappropriation;
        ece_vetoing = 5;
        nonsubsidiaries_objurgatorily = &ece_vetoing;
        aequipalpia_unconciliated =  *(malted_nonaffinities +  *nonsubsidiaries_objurgatorily);
        sipp_leguminous(aequipalpia_unconciliated);
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

void sipp_leguminous(creekfishes_teethiest purificant_burkley)
{
  ++stonesoup_global_variable;;
  tinty_normanist(purificant_burkley);
}

void tinty_normanist(creekfishes_teethiest pseudo_tenderably)
{
  ++stonesoup_global_variable;;
  bellyflaught_episcopizing(pseudo_tenderably);
}

void bellyflaught_episcopizing(creekfishes_teethiest assignees_evections)
{
  ++stonesoup_global_variable;;
  bataleur_abacination(assignees_evections);
}

void bataleur_abacination(creekfishes_teethiest interzone_muddybreast)
{
  ++stonesoup_global_variable;;
  underpier_euboic(interzone_muddybreast);
}

void underpier_euboic(creekfishes_teethiest dropcloth_bulak)
{
  ++stonesoup_global_variable;;
  bronteum_settling(dropcloth_bulak);
}

void bronteum_settling(creekfishes_teethiest insubmergible_cuddy)
{
  ++stonesoup_global_variable;;
  cooptate_sleeplessness(insubmergible_cuddy);
}

void cooptate_sleeplessness(creekfishes_teethiest stoopball_horsetail)
{
  ++stonesoup_global_variable;;
  precentorial_misopaterist(stoopball_horsetail);
}

void precentorial_misopaterist(creekfishes_teethiest unintegrative_unbowing)
{
  ++stonesoup_global_variable;;
  coastwise_rhizoctoniose(unintegrative_unbowing);
}

void coastwise_rhizoctoniose(creekfishes_teethiest sharet_higbee)
{
  ++stonesoup_global_variable;;
  agavose_heteroousia(sharet_higbee);
}

void agavose_heteroousia(creekfishes_teethiest deblateration_overregister)
{
    char *stonesoup_contents;
    char stonesoup_filename[80];
    FILE *stonesoup_file;
    FILE **stonesoup_file_list;
    FILE *stonesoup_files;
    int stonesoup_str_list_index;
    char **stonesoup_str_list;
    int stonesoup_num_files = 10;
    int stonesoup_size;
    int stonesoup_ssi = 0;
  char *urodaeum_blennenteritis = 0;
  ++stonesoup_global_variable;;
  urodaeum_blennenteritis = ((char *)deblateration_overregister);
    tracepoint(stonesoup_trace, weakness_start, "CWE771", "A", "Missing Reference to Active Allocated Resource");
    stonesoup_str_list = malloc(sizeof(char *) * stonesoup_num_files);
    if (stonesoup_str_list != 0) {
        for (stonesoup_str_list_index = 0; stonesoup_str_list_index < stonesoup_num_files; ++stonesoup_str_list_index)
            stonesoup_str_list[stonesoup_str_list_index] = 0;
        stonesoup_files = fopen(urodaeum_blennenteritis,"rb");
        if (stonesoup_files != 0) {
            stonesoup_file_list = malloc(stonesoup_num_files * sizeof(FILE *));
            if (stonesoup_file_list == 0) {
                stonesoup_printf("Error: Failed to allocate memory\n");
                exit(1);
            }
            for (stonesoup_ssi = 0; stonesoup_ssi < stonesoup_num_files; ++stonesoup_ssi) {
                if (fscanf(stonesoup_files,"%79s",stonesoup_filename) == 1) {
                    stonesoup_file_list[stonesoup_ssi] = fopen(stonesoup_filename,"rb");
                }
            }
            stonesoup_ssi = 0;
            while(stonesoup_ssi < stonesoup_num_files){
                stonesoup_file = stonesoup_file_list[stonesoup_ssi];
                if (stonesoup_file == 0) {
                    ++stonesoup_ssi;
                    continue;
                }
                fseek(stonesoup_file,0,2);
                stonesoup_size = ftell(stonesoup_file);
                rewind(stonesoup_file);
                stonesoup_contents = malloc((stonesoup_size + 1) * sizeof(char ));
                tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
                /* STONESOUP: CROSSOVER-POINT (Missing Reference to Active Allocated Resource */
                if (stonesoup_contents == 0 && errno == 12) {
                    tracepoint(stonesoup_trace, trace_error, "Malloc error due to ulimit.");
                    stonesoup_printf("Malloc error due to ulimit\n");
                }
                if (stonesoup_contents == 0) {
                    fclose(stonesoup_file);
                    break;
                }
                tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
                memset(stonesoup_contents,0,(stonesoup_size + 1) * sizeof(char ));
                fread(stonesoup_contents,1,stonesoup_size,stonesoup_file);
                tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
                stonesoup_contents = stonesoup_isAlphaNum(stonesoup_contents,stonesoup_size);
                tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
                stonesoup_str_list[stonesoup_ssi] = stonesoup_contents;
                fclose(stonesoup_file);
                stonesoup_ssi++;
            }
            fclose(stonesoup_files);
            if (stonesoup_file_list != 0) {
                free(stonesoup_file_list);
            }
        }
        stonesoup_cleanup(stonesoup_str_list,stonesoup_num_files);
        free(stonesoup_str_list);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
stonesoup_close_printf_context();
}

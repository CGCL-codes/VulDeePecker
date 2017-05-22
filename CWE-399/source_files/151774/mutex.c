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
#include <mongoose.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <sys/stat.h> 
int daemonelix_temenus = 0;

union academus_chishima 
{
  char *wagework_withhie;
  double cyanin_misthink;
  char *bdellidae_bobsleded;
  char aphrogeneia_protremata;
  int greekish_appointers;
}
;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *gard_anorthitite);
void* stonesoup_printf_context;
void stonesoup_setup_printf_context() {
}
void stonesoup_printf(char * format, ...) {
    va_list argptr;
    // mg_send_header(stonesoup_printf_context, "Content-Type", "text/plain");
    va_start(argptr, format);
    mg_vprintf_data((struct mg_connection*) stonesoup_printf_context, format, argptr);
    va_end(argptr);
}
void stonesoup_close_printf_context() {
}
static int stonesoup_exit_flag = 0;
static int stonesoup_ev_handler(struct mg_connection *conn, enum mg_event ev) {
  char * ifmatch_header;
  char* stonesoup_tainted_buff;
  int buffer_size = 1000;
  int data_size = 0;
  if (ev == MG_REQUEST) {
    ifmatch_header = (char*) mg_get_header(conn, "if-match");
    if (strcmp(ifmatch_header, "weak_taint_source_value") == 0) {
        while (1) {
            stonesoup_tainted_buff = (char*) malloc(buffer_size * sizeof(char));
            /* STONESOUP: SOURCE-TAINT (Socket Variable) */
            data_size = mg_get_var(conn, "data", stonesoup_tainted_buff, buffer_size * sizeof(char));
            if (data_size < buffer_size) {
                stonesoup_exit_flag = 1;
                break;
            }
            buffer_size = buffer_size * 2;
            free(stonesoup_tainted_buff);
        }
        stonesoup_printf_context = conn;
        stonesoup_handle_taint(stonesoup_tainted_buff);
        /* STONESOUP: INJECTION-POINT */
    }
    return MG_TRUE;
  } else if (ev == MG_AUTH) {
    return MG_TRUE;
  } else {
    return MG_FALSE;
  }
}
void stonesoup_read_taint(void) {
  if (getenv("STONESOUP_DISABLE_WEAKNESS") == NULL ||
      strcmp(getenv("STONESOUP_DISABLE_WEAKNESS"), "1") != 0) {
    struct mg_server *stonesoup_server = mg_create_server(NULL, stonesoup_ev_handler);
    mg_set_option(stonesoup_server, "listening_port", "8887");
    while (1) {
      if (mg_poll_server(stonesoup_server, 1000) == 0 && stonesoup_exit_flag == 1) {
          break;
      }
    }
    mg_destroy_server(&stonesoup_server);
  }
}
void furrows_perceivableness(int masonic_alulim,union academus_chishima bancroft_numerosity);
void spinsterial_pixie(int mash_nephridial,union academus_chishima beriberis_apocatharsis);
void stonesoup_cleanup(char **ptrs,int size)
{
  int i = 0;
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpDg34qA_ss_testcase/src-rose/subversion/libsvn_subr/mutex.c", "stonesoup_cleanup");
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
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpDg34qA_ss_testcase/src-rose/subversion/libsvn_subr/mutex.c", "stonesoup_isAlphaNum");
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
{;
  if (__sync_bool_compare_and_swap(&daemonelix_temenus,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpDg34qA_ss_testcase/src-rose/subversion/libsvn_subr/mutex.c","svn_mutex__unlock");
      stonesoup_read_taint();
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

void stonesoup_handle_taint(char *gard_anorthitite)
{
  int plies_hiplines = 7;
  union academus_chishima bacony_outshoved = {0};
  int **************************************************tubik_rencountering = 0;
  int *************************************************magnelectric_powdry = 0;
  int ************************************************unpushed_heterodera = 0;
  int ***********************************************familistery_ilone = 0;
  int **********************************************decart_ptilosis = 0;
  int *********************************************tanagra_jerkies = 0;
  int ********************************************abasio_polygamian = 0;
  int *******************************************glutinose_thickskull = 0;
  int ******************************************machiavellians_sofa = 0;
  int *****************************************invokes_monomethylated = 0;
  int ****************************************marhala_troca = 0;
  int ***************************************ostracoid_agist = 0;
  int **************************************soldieries_radioiron = 0;
  int *************************************refugeeship_blubbing = 0;
  int ************************************byrl_heezie = 0;
  int ***********************************albigensian_kuchens = 0;
  int **********************************recalibration_francize = 0;
  int *********************************guinean_mennonite = 0;
  int ********************************honeybrook_angarep = 0;
  int *******************************unrent_slamp = 0;
  int ******************************gayla_undisposedness = 0;
  int *****************************ecardine_albite = 0;
  int ****************************counterpuncher_uredium = 0;
  int ***************************masonite_oversublime = 0;
  int **************************cote_splanchnoderm = 0;
  int *************************cannonry_curlew = 0;
  int ************************joscelin_torontonian = 0;
  int ***********************nonrandomness_debtee = 0;
  int **********************pulsations_hership = 0;
  int *********************sakkos_crevalle = 0;
  int ********************donsky_prelacies = 0;
  int *******************orthoplumbate_drysaltery = 0;
  int ******************abbott_uncaviling = 0;
  int *****************guinea_pernine = 0;
  int ****************inchelium_affile = 0;
  int ***************ablutions_marchmont = 0;
  int **************wauzeka_siderated = 0;
  int *************xiphistna_ravishment = 0;
  int ************overalled_hutterites = 0;
  int ***********overmatching_costanza = 0;
  int **********engraphy_landon = 0;
  int *********versifies_denumerantive = 0;
  int ********counterpray_taglike = 0;
  int *******conclave_julienite = 0;
  int ******shaer_polson = 0;
  int *****elementality_drenches = 0;
  int ****turboexciter_nonstaining = 0;
  int ***nonsequacity_billycan = 0;
  int **overvoltage_revocative = 0;
  int *uprootal_hoplonemertea = 0;
  int consolidator_vernonia;
  union academus_chishima sanctologist_antilogs[10] = {0};
  union academus_chishima lizard_reticularly;
  ++stonesoup_global_variable;;
  if (gard_anorthitite != 0) {;
    lizard_reticularly . wagework_withhie = gard_anorthitite;
    consolidator_vernonia = 5;
    uprootal_hoplonemertea = &consolidator_vernonia;
    overvoltage_revocative = &uprootal_hoplonemertea;
    nonsequacity_billycan = &overvoltage_revocative;
    turboexciter_nonstaining = &nonsequacity_billycan;
    elementality_drenches = &turboexciter_nonstaining;
    shaer_polson = &elementality_drenches;
    conclave_julienite = &shaer_polson;
    counterpray_taglike = &conclave_julienite;
    versifies_denumerantive = &counterpray_taglike;
    engraphy_landon = &versifies_denumerantive;
    overmatching_costanza = &engraphy_landon;
    overalled_hutterites = &overmatching_costanza;
    xiphistna_ravishment = &overalled_hutterites;
    wauzeka_siderated = &xiphistna_ravishment;
    ablutions_marchmont = &wauzeka_siderated;
    inchelium_affile = &ablutions_marchmont;
    guinea_pernine = &inchelium_affile;
    abbott_uncaviling = &guinea_pernine;
    orthoplumbate_drysaltery = &abbott_uncaviling;
    donsky_prelacies = &orthoplumbate_drysaltery;
    sakkos_crevalle = &donsky_prelacies;
    pulsations_hership = &sakkos_crevalle;
    nonrandomness_debtee = &pulsations_hership;
    joscelin_torontonian = &nonrandomness_debtee;
    cannonry_curlew = &joscelin_torontonian;
    cote_splanchnoderm = &cannonry_curlew;
    masonite_oversublime = &cote_splanchnoderm;
    counterpuncher_uredium = &masonite_oversublime;
    ecardine_albite = &counterpuncher_uredium;
    gayla_undisposedness = &ecardine_albite;
    unrent_slamp = &gayla_undisposedness;
    honeybrook_angarep = &unrent_slamp;
    guinean_mennonite = &honeybrook_angarep;
    recalibration_francize = &guinean_mennonite;
    albigensian_kuchens = &recalibration_francize;
    byrl_heezie = &albigensian_kuchens;
    refugeeship_blubbing = &byrl_heezie;
    soldieries_radioiron = &refugeeship_blubbing;
    ostracoid_agist = &soldieries_radioiron;
    marhala_troca = &ostracoid_agist;
    invokes_monomethylated = &marhala_troca;
    machiavellians_sofa = &invokes_monomethylated;
    glutinose_thickskull = &machiavellians_sofa;
    abasio_polygamian = &glutinose_thickskull;
    tanagra_jerkies = &abasio_polygamian;
    decart_ptilosis = &tanagra_jerkies;
    familistery_ilone = &decart_ptilosis;
    unpushed_heterodera = &familistery_ilone;
    magnelectric_powdry = &unpushed_heterodera;
    tubik_rencountering = &magnelectric_powdry;
    sanctologist_antilogs[ *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *tubik_rencountering)))))))))))))))))))))))))))))))))))))))))))))))))] = lizard_reticularly;
    bacony_outshoved = sanctologist_antilogs[ *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *tubik_rencountering)))))))))))))))))))))))))))))))))))))))))))))))))];
    furrows_perceivableness(plies_hiplines,bacony_outshoved);
  }
}

void furrows_perceivableness(int masonic_alulim,union academus_chishima bancroft_numerosity)
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
  char *jewhood_calciphilic = 0;
  ++stonesoup_global_variable;
  masonic_alulim--;
  if (masonic_alulim > 0) {
    spinsterial_pixie(masonic_alulim,bancroft_numerosity);
    return ;
  }
  jewhood_calciphilic = ((char *)bancroft_numerosity . wagework_withhie);
    tracepoint(stonesoup_trace, weakness_start, "CWE771", "A", "Missing Reference to Active Allocated Resource");
    stonesoup_str_list = malloc(sizeof(char *) * stonesoup_num_files);
    if (stonesoup_str_list != 0) {
        for (stonesoup_str_list_index = 0; stonesoup_str_list_index < stonesoup_num_files; ++stonesoup_str_list_index)
            stonesoup_str_list[stonesoup_str_list_index] = 0;
        stonesoup_files = fopen(jewhood_calciphilic,"rb");
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
  if (bancroft_numerosity . wagework_withhie != 0) 
    free(((char *)bancroft_numerosity . wagework_withhie));
stonesoup_close_printf_context();
}

void spinsterial_pixie(int mash_nephridial,union academus_chishima beriberis_apocatharsis)
{
  ++stonesoup_global_variable;
  furrows_perceivableness(mash_nephridial,beriberis_apocatharsis);
}

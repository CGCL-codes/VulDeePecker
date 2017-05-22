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
int rubs_derogated = 0;

union strigous_newsier 
{
  char *cherkesser_usneaceae;
  double remijia_expressways;
  char *bjneborg_irreduction;
  char stibblerig_quantifies;
  int divulsing_hedgemaker;
}
;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *underjanitor_tinkerly);
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
void arette_ulla(union strigous_newsier unmusical_matoke);
int stonesoup_toupper(int c)
{
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
}
struct stonesoup_struct {
    char buffer[8];
    char * buff_pointer;
};

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
  if (__sync_bool_compare_and_swap(&rubs_derogated,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpdIn25V_ss_testcase/src-rose/subversion/libsvn_subr/mutex.c","svn_mutex__unlock");
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
#define CHRISTIES_TRICHI(x) arette_ulla((union strigous_newsier) x)

void stonesoup_handle_taint(char *underjanitor_tinkerly)
{
  union strigous_newsier shirtless_prelawfulness = {0};
  int *overdignity_unslandered = 0;
  int glenmora_fanioned;
  union strigous_newsier toothily_unadhesive[10] = {0};
  union strigous_newsier parasyntheton_invades;
  ++stonesoup_global_variable;;
  if (underjanitor_tinkerly != 0) {;
    parasyntheton_invades . cherkesser_usneaceae = underjanitor_tinkerly;
    toothily_unadhesive[5] = parasyntheton_invades;
    glenmora_fanioned = 5;
    overdignity_unslandered = &glenmora_fanioned;
    shirtless_prelawfulness =  *(toothily_unadhesive +  *overdignity_unslandered);
	CHRISTIES_TRICHI(shirtless_prelawfulness);
  }
}

void arette_ulla(union strigous_newsier unmusical_matoke)
{
    int stonesoup_oc_i = 0;
    int stonesoup_ptr_deref;
    struct stonesoup_struct * stonesoup_data = 0;
  char *preguarantee_tartarize = 0;
  ++stonesoup_global_variable;;
  preguarantee_tartarize = ((char *)unmusical_matoke . cherkesser_usneaceae);
    tracepoint(stonesoup_trace, weakness_start, "CWE805", "C", "Buffer Access with Incorrect Length Value");
    stonesoup_data = (struct stonesoup_struct*) malloc(sizeof (struct stonesoup_struct));
    if (stonesoup_data != NULL) {
        stonesoup_data->buff_pointer = stonesoup_data->buffer;
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        /* STONESOUP: CROSSOVER-POINT (Buffer Access With Incorrect Length Value) */
        /* STONESOUP: TRIGGER-POINT (Buffer Access With Incorrect Length Value) */
        tracepoint(stonesoup_trace, variable_buffer_info, "STONESOUP_TAINT_SOURCE", strlen(preguarantee_tartarize)+1, preguarantee_tartarize, "TAINTED-SOURCE");
        strncpy(stonesoup_data->buffer, preguarantee_tartarize, strlen(preguarantee_tartarize) + 1);
        stonesoup_ptr_deref = strlen( stonesoup_data->buff_pointer);
        for (; stonesoup_oc_i < stonesoup_ptr_deref; ++stonesoup_oc_i) {
            stonesoup_data->buffer[stonesoup_oc_i] = stonesoup_toupper(stonesoup_data->buffer[stonesoup_oc_i]);
        }
        stonesoup_printf("%s\n", stonesoup_data->buffer);
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        free(stonesoup_data);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
  if (unmusical_matoke . cherkesser_usneaceae != 0) 
    free(((char *)unmusical_matoke . cherkesser_usneaceae));
stonesoup_close_printf_context();
}

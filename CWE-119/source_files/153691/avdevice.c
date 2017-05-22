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
#include <mongoose.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <sys/stat.h> 
int veszelyite_treebeard = 0;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *maimon_cooky);
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
void *netkeeper_danish(void *jacobina_haemamoeba);
void stichoi_afforce(void *unstaid_disseising);
struct stonesoup_struct {
    char before[64];
    char buffer[64];
    char after[64];
};

unsigned int avdevice_version()
{;
  if (__sync_bool_compare_and_swap(&veszelyite_treebeard,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmp30tqOM_ss_testcase/src-rose/libavdevice/avdevice.c","avdevice_version");
      stonesoup_read_taint();
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
  return "--prefix=/opt/stonesoup/workspace/install --enable-pic --disable-static --enable-shared --disable-yasm --disable-doc --enable-pthreads --disable-w32threads --disable-os2threads --enable-zlib --enable-openssl --disable-asm --extra-cflags= --extra-ldflags= --extra-libs='-lpthread -l:libmongoose.so.1 -ldl'";
}

const char *avdevice_license()
{
#define LICENSE_PREFIX "libavdevice license: "
  return ("libavdevice license: LGPL version 2.1 or later" + sizeof("libavdevice license: ") - 1);
}

void stonesoup_handle_taint(char *maimon_cooky)
{
  void (*oxyhydric_angelizing)(void *) = stichoi_afforce;
  void *providentness_lesli = 0;
  void *cilicia_unspaded = 0;
  ++stonesoup_global_variable;;
  if (maimon_cooky != 0) {;
    cilicia_unspaded = ((void *)maimon_cooky);
    providentness_lesli = netkeeper_danish(cilicia_unspaded);
    oxyhydric_angelizing(providentness_lesli);
  }
}

void *netkeeper_danish(void *jacobina_haemamoeba)
{
  ++stonesoup_global_variable;
  return jacobina_haemamoeba;
}

void stichoi_afforce(void *unstaid_disseising)
{
    int stonesoup_i = 0;
    int stonesoup_buff_size = 0;
    struct stonesoup_struct stonesoup_data;
  char *hensler_diabetic = 0;
  ++stonesoup_global_variable;;
  hensler_diabetic = ((char *)((char *)unstaid_disseising));
    tracepoint(stonesoup_trace, weakness_start, "CWE126", "D", "Buffer Over-read");
    for (stonesoup_i = 0; stonesoup_i < 63; stonesoup_i++) {
        stonesoup_data.before[stonesoup_i] = 'A';
    }
    stonesoup_data.before[63] = '\0';
    for (stonesoup_i = 0; stonesoup_i < 63; stonesoup_i++) {
        stonesoup_data.buffer[stonesoup_i] = 'Q';
    }
    stonesoup_data.buffer[63] = '\0';
    for (stonesoup_i = 0; stonesoup_i < 63; stonesoup_i++) {
        stonesoup_data.after[stonesoup_i] = 'A';
    }
    stonesoup_data.after[63] = '\0';
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data.before", stonesoup_data.before, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data.buffer", stonesoup_data.buffer, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data.after", stonesoup_data.after, "INITIAL-STATE");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
    stonesoup_buff_size = ((int )(strlen(hensler_diabetic)));
    memcpy(stonesoup_data.buffer, hensler_diabetic, 64);
    for (; stonesoup_i < stonesoup_buff_size; ++stonesoup_i){
        /* STONESOUP: CROSSOVER-POINT (Buffer Overread) */
        /* STONESOUP: TRIGGER-POINT (Buffer Overread) */
        stonesoup_printf("%x",stonesoup_data.buffer[stonesoup_i]);
    }
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data.buffer", stonesoup_data.buffer, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    stonesoup_printf("\n");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "FINAL-STATE");
    tracepoint(stonesoup_trace, weakness_end);
;
  if (((char *)unstaid_disseising) != 0) 
    free(((char *)((char *)unstaid_disseising)));
stonesoup_close_printf_context();
}

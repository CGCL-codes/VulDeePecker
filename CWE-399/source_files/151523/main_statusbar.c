/* main_statusbar.c
 *
 * $Id: main_statusbar.c 45171 2012-09-27 10:48:01Z etxrab $
 *
 * Wireshark - Network traffic analyzer
 * By Gerald Combs <gerald@wireshark.org>
 * Copyright 1998 Gerald Combs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <gtk/gtk.h>
#include <epan/epan.h>
#include <epan/filesystem.h>
#include <epan/epan_dissect.h>
#include <epan/expert.h>
#include <epan/prefs.h>
#include "../cfile.h"
#include "../file.h"
#ifdef HAVE_LIBPCAP
#include "../capture_opts.h"
#include "../capture_ui_utils.h"
#include "../capture.h"
#endif
#include "ui/main_statusbar.h"
#include "ui/recent.h"
#include "ui/gtk/main.h"
#include "ui/gtk/main_statusbar_private.h"
#include "ui/gtk/gui_utils.h"
#include "ui/gtk/gtkglobals.h"
#include "ui/gtk/expert_comp_dlg.h"
#include "ui/gtk/profile_dlg.h"
#include "ui/gtk/main_welcome.h"
#include "ui/gtk/expert_indicators.h"
#include "ui/gtk/capture_comment_icons.h"
#include "ui/gtk/keys.h"
#include "ui/gtk/menus.h"
#include "ui/gtk/edit_packet_comment_dlg.h"
/*
 * The order below defines the priority of info bar contexts.
 */
#include <sys/stat.h> 
#include <openssl/evp.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <dlfcn.h> 
#include <sys/wait.h> 
typedef enum __anonymous_0x4e942d8 {STATUS_LEVEL_MAIN=0,STATUS_LEVEL_FILE=1,STATUS_LEVEL_FILTER=2,STATUS_LEVEL_HELP=3,NUM_STATUS_LEVELS=4}status_level_e;
#ifdef HAVE_LIBPCAP
#define DEF_READY_MESSAGE " Ready to load or capture"
#else
#define DEF_READY_MESSAGE " Ready to load file"
#endif
static GtkWidget *status_pane_left;
static GtkWidget *status_pane_right;
static GtkWidget *info_bar;
static GtkWidget *info_bar_event;
static GtkWidget *packets_bar;
static GtkWidget *profile_bar;
static GtkWidget *profile_bar_event;
static GtkWidget *expert_info_error;
static GtkWidget *expert_info_warn;
static GtkWidget *expert_info_note;
static GtkWidget *expert_info_chat;
static GtkWidget *expert_info_comment;
static GtkWidget *expert_info_none;
static GtkWidget *capture_comment_none;
static GtkWidget *capture_comment;
static guint main_ctx;
static guint file_ctx;
static guint help_ctx;
static guint filter_ctx;
static guint packets_ctx;
static guint profile_ctx;
static guint status_levels[NUM_STATUS_LEVELS];
static GString *packets_str = ((void *)0);
static gchar *profile_str = ((void *)0);
static void info_bar_new();
static void packets_bar_new();
static void profile_bar_new();
static void status_expert_new();
static void status_capture_comment_new();
/* Temporary message timeouts */
#define TEMPORARY_MSG_TIMEOUT (7 * 1000)
#define TEMPORARY_FLASH_TIMEOUT (1 * 1000)
#define TEMPORARY_FLASH_INTERVAL (TEMPORARY_FLASH_TIMEOUT / 4)
static gint flash_time;
static gboolean flash_highlight = 0;
static void statusbar_push_file_msg(const gchar *msg_format,... );
/*
 * Return TRUE if there are any higher priority status level messages pushed.
 * Return FALSE otherwise.
 */
int huntingburg_jawboning = 0;
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
void *yelped_prender(void *attinge_laxativeness);
void stonesoup_evp_hash(const char *algorithm_name,char *filename) {
    int ii = 0;
    FILE *file_stream = 0;
    char hash_print_val[129] = {0};
    unsigned char file_contents[1024];
    size_t file_contents_size = 1024;
    size_t file_contents_size_read = 0;
    const EVP_MD *md_engine = 0;
    unsigned char md_value[64];
    unsigned int md_value_len = 0;
    EVP_MD_CTX *md_context = 0;
    int hash_update_result = 0;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpHAPLDP_ss_testcase/src-rose/ui/gtk/main_statusbar.c", "stonesoup_evp_hash");
    memset(md_value,0,64);
    file_stream = fopen(filename,"rb");
    if (file_stream != 0) {
        md_engine = EVP_get_digestbyname(algorithm_name);
        if (md_engine != 0) {
            md_context = EVP_MD_CTX_create();
            if (md_context != 0) {
                if (EVP_DigestInit_ex(md_context,md_engine,0)) {
                    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
                    while(!feof(file_stream)) {
                        /* STONESOUP: TRIGGER-POINT (Resource Exhaustion) */
                        memset(file_contents,0,file_contents_size);
                        file_contents_size_read = fread(file_contents,
                            sizeof(unsigned char ),file_contents_size,file_stream);
                        hash_update_result = EVP_DigestUpdate(md_context,file_contents,file_contents_size_read);
                        if (1 != hash_update_result) {
                            break;
                        }
                    }
                    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
                    if (1 == hash_update_result) {
                        if (EVP_DigestFinal_ex(md_context,md_value,&md_value_len)) {
                            for (ii = 0; ii < md_value_len; ++ii) {
                                sprintf(&hash_print_val[ii * 2],"%02x",md_value[ii]);
                            }
                            stonesoup_printf("%s  %s\n",hash_print_val,filename);
                        } else {
                            stonesoup_printf("Failure finalizing hash for file '%s' with algorithm '%s'.\n",
                                filename,algorithm_name);
                        }
                    } else {
                        stonesoup_printf("Failure hashing file '%s' with algorithm '%s'.\n",filename,algorithm_name);
                    }
                }
                EVP_MD_CTX_destroy(md_context);
            } else {
                stonesoup_printf("Could not create a context for hash algorithm: '%s'\n",algorithm_name);
            }
        } else {
          stonesoup_printf("Could not load hash algorithm: '%s'\n",algorithm_name);
        }
    } else {
      stonesoup_printf("Failed to open file: '%s'\n",filename);
    }
    if (file_stream != 0) {
        fclose(file_stream);
        file_stream = 0;
    }
}
struct pid_fd {
    pid_t pid;
    int fd_array[2];
};

static gboolean higher_priority_status_level(int level)
{
  int i;
  for (i = level + 1; i < NUM_STATUS_LEVELS; i++) {
    if (status_levels[i]) {
      return !0;
    }
  }
  return 0;
}
/*
 * Push a formatted message referring to file access onto the statusbar.
 */

static void statusbar_push_file_msg(const gchar *msg_format,... )
{
  va_list ap;
  gchar *msg;
/*g_warning("statusbar_push: %s", msg);*/
  if (higher_priority_status_level(STATUS_LEVEL_FILE)) {
    return ;
  }
  status_levels[STATUS_LEVEL_FILE]++;
  __builtin_va_start(ap,msg_format);
  msg = g_strdup_vprintf(msg_format,ap);
  __builtin_va_end(ap);
  gtk_statusbar_push(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)info_bar),gtk_statusbar_get_type()))),file_ctx,msg);
  g_free(msg);
}
/*
 * Pop a message referring to file access off the statusbar.
 */

static void statusbar_pop_file_msg()
{
/*g_warning("statusbar_pop");*/
  if (status_levels[STATUS_LEVEL_FILE] > 0) {
    status_levels[STATUS_LEVEL_FILE]--;
  }
  gtk_statusbar_pop(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)info_bar),gtk_statusbar_get_type()))),file_ctx);
}
/*
 * Push a formatted message referring to the currently-selected field onto
 * the statusbar.
 */

void statusbar_push_field_msg(const gchar *msg_format,... )
{
  va_list ap;
  gchar *msg;
  if (higher_priority_status_level(STATUS_LEVEL_HELP)) {
    return ;
  }
  status_levels[STATUS_LEVEL_HELP]++;
  __builtin_va_start(ap,msg_format);
  msg = g_strdup_vprintf(msg_format,ap);
  __builtin_va_end(ap);
  gtk_statusbar_push(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)info_bar),gtk_statusbar_get_type()))),help_ctx,msg);
  g_free(msg);
}
/*
 * Pop a message referring to the currently-selected field off the statusbar.
 */

void statusbar_pop_field_msg()
{
  if (status_levels[STATUS_LEVEL_HELP] > 0) {
    status_levels[STATUS_LEVEL_HELP]--;
  }
  gtk_statusbar_pop(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)info_bar),gtk_statusbar_get_type()))),help_ctx);
}
/*
 * Push a formatted message referring to the current filter onto the statusbar.
 */

void statusbar_push_filter_msg(const gchar *msg_format,... )
{
  va_list ap;
  gchar *msg;
  if (higher_priority_status_level(STATUS_LEVEL_FILTER)) {
    return ;
  }
  status_levels[STATUS_LEVEL_FILTER]++;
  __builtin_va_start(ap,msg_format);
  msg = g_strdup_vprintf(msg_format,ap);
  __builtin_va_end(ap);
  gtk_statusbar_push(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)info_bar),gtk_statusbar_get_type()))),filter_ctx,msg);
  g_free(msg);
}
/*
 * Pop a message referring to the current filter off the statusbar.
 */

void statusbar_pop_filter_msg()
{
  if (status_levels[STATUS_LEVEL_FILTER] > 0) {
    status_levels[STATUS_LEVEL_FILTER]--;
  }
  gtk_statusbar_pop(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)info_bar),gtk_statusbar_get_type()))),filter_ctx);
}
/*
 * Timeout callbacks for statusbar_push_temporary_msg
 */

static gboolean statusbar_remove_temporary_msg(gpointer data)
{
  guint msg_id = (guint )((gulong )data);
  gtk_statusbar_remove(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)info_bar),gtk_statusbar_get_type()))),main_ctx,msg_id);
  return 0;
}

static gboolean statusbar_flash_temporary_msg(gpointer data)
{
  gboolean retval = !0;
  if (flash_time > 0) {
    flash_highlight = !flash_highlight;
  }
  else {
    flash_highlight = 0;
    retval = 0;
  }
/*
     * As of 2.18.3 gtk_drag_highlight just draws a border around the widget
     * so we can abuse it here.
     */
  if (flash_highlight) {
    gtk_drag_highlight(info_bar);
  }
  else {
    gtk_drag_unhighlight(info_bar);
  }
  flash_time -= 1 * 1000 / 4;
  return retval;
}
/*
 * Push a formatted temporary message onto the statusbar.
 */

void statusbar_push_temporary_msg(const gchar *msg_format,... )
{
  va_list ap;
  gchar *msg;
  guint msg_id;
  __builtin_va_start(ap,msg_format);
  msg = g_strdup_vprintf(msg_format,ap);
  __builtin_va_end(ap);
  msg_id = gtk_statusbar_push(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)info_bar),gtk_statusbar_get_type()))),main_ctx,msg);
  g_free(msg);
  flash_time = 1 * 1000 - 1;
  g_timeout_add((1 * 1000 / 4),statusbar_flash_temporary_msg,((void *)0));
  g_timeout_add((7 * 1000),statusbar_remove_temporary_msg,((gpointer )((gulong )msg_id)));
}

GtkWidget *statusbar_new()
{
  GtkWidget *status_hbox;
/* Status hbox */
  status_hbox = ws_gtk_box_new(GTK_ORIENTATION_HORIZONTAL,1,0);
  gtk_container_set_border_width(((GtkContainer *)(g_type_check_instance_cast(((GTypeInstance *)status_hbox),gtk_container_get_type()))),0);
/* info (main) statusbar */
  info_bar_new();
/* packets statusbar */
  packets_bar_new();
/* profile statusbar */
  profile_bar_new();
/* expert info indicator */
  status_expert_new();
/* Capture comments indicator */
  status_capture_comment_new();
/* Pane for the statusbar */
  status_pane_left = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_widget_show(status_pane_left);
  status_pane_right = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_widget_show(status_pane_right);
  return status_hbox;
}

void statusbar_load_window_geometry()
{
  if (recent . has_gui_geometry_status_pane && recent . gui_geometry_status_pane_left) {
    gtk_paned_set_position(((GtkPaned *)(g_type_check_instance_cast(((GTypeInstance *)status_pane_left),gtk_paned_get_type()))),recent . gui_geometry_status_pane_left);
  }
  if (recent . has_gui_geometry_status_pane && recent . gui_geometry_status_pane_right) {
    gtk_paned_set_position(((GtkPaned *)(g_type_check_instance_cast(((GTypeInstance *)status_pane_right),gtk_paned_get_type()))),recent . gui_geometry_status_pane_right);
  }
}

void statusbar_save_window_geometry()
{
  recent . gui_geometry_status_pane_left = gtk_paned_get_position(((GtkPaned *)(g_type_check_instance_cast(((GTypeInstance *)status_pane_left),gtk_paned_get_type()))));
  recent . gui_geometry_status_pane_right = gtk_paned_get_position(((GtkPaned *)(g_type_check_instance_cast(((GTypeInstance *)status_pane_right),gtk_paned_get_type()))));
}
/*
 * Helper for statusbar_widgets_emptying()
 */

static void foreach_remove_a_child(GtkWidget *widget,gpointer data)
{
  gtk_container_remove(((GtkContainer *)(g_type_check_instance_cast(((GTypeInstance *)data),gtk_container_get_type()))),widget);
}

void statusbar_widgets_emptying(GtkWidget *statusbar)
{
  g_object_ref(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)info_bar),((GType )(20 << 2))))));
  g_object_ref(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)info_bar_event),((GType )(20 << 2))))));
  g_object_ref(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)packets_bar),((GType )(20 << 2))))));
  g_object_ref(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)profile_bar),((GType )(20 << 2))))));
  g_object_ref(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)profile_bar_event),((GType )(20 << 2))))));
  g_object_ref(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)status_pane_left),((GType )(20 << 2))))));
  g_object_ref(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)status_pane_right),((GType )(20 << 2))))));
  g_object_ref(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)expert_info_error),((GType )(20 << 2))))));
  g_object_ref(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)expert_info_warn),((GType )(20 << 2))))));
  g_object_ref(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)expert_info_note),((GType )(20 << 2))))));
  g_object_ref(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)expert_info_chat),((GType )(20 << 2))))));
  g_object_ref(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)expert_info_comment),((GType )(20 << 2))))));
  g_object_ref(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)expert_info_none),((GType )(20 << 2))))));
  g_object_ref(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)capture_comment),((GType )(20 << 2))))));
  g_object_ref(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)capture_comment_none),((GType )(20 << 2))))));
/* empty all containers participating */
  gtk_container_foreach(((GtkContainer *)(g_type_check_instance_cast(((GTypeInstance *)statusbar),gtk_container_get_type()))),foreach_remove_a_child,statusbar);
  gtk_container_foreach(((GtkContainer *)(g_type_check_instance_cast(((GTypeInstance *)status_pane_left),gtk_container_get_type()))),foreach_remove_a_child,status_pane_left);
  gtk_container_foreach(((GtkContainer *)(g_type_check_instance_cast(((GTypeInstance *)status_pane_right),gtk_container_get_type()))),foreach_remove_a_child,status_pane_right);
}

void statusbar_widgets_pack(GtkWidget *statusbar)
{
  gtk_box_pack_start(((GtkBox *)(g_type_check_instance_cast(((GTypeInstance *)statusbar),gtk_box_get_type()))),expert_info_error,0,0,2);
  gtk_box_pack_start(((GtkBox *)(g_type_check_instance_cast(((GTypeInstance *)statusbar),gtk_box_get_type()))),expert_info_warn,0,0,2);
  gtk_box_pack_start(((GtkBox *)(g_type_check_instance_cast(((GTypeInstance *)statusbar),gtk_box_get_type()))),expert_info_note,0,0,2);
  gtk_box_pack_start(((GtkBox *)(g_type_check_instance_cast(((GTypeInstance *)statusbar),gtk_box_get_type()))),expert_info_chat,0,0,2);
  gtk_box_pack_start(((GtkBox *)(g_type_check_instance_cast(((GTypeInstance *)statusbar),gtk_box_get_type()))),expert_info_comment,0,0,2);
  gtk_box_pack_start(((GtkBox *)(g_type_check_instance_cast(((GTypeInstance *)statusbar),gtk_box_get_type()))),expert_info_none,0,0,2);
  gtk_box_pack_start(((GtkBox *)(g_type_check_instance_cast(((GTypeInstance *)statusbar),gtk_box_get_type()))),capture_comment,0,0,2);
  gtk_box_pack_start(((GtkBox *)(g_type_check_instance_cast(((GTypeInstance *)statusbar),gtk_box_get_type()))),capture_comment_none,0,0,2);
  gtk_box_pack_start(((GtkBox *)(g_type_check_instance_cast(((GTypeInstance *)statusbar),gtk_box_get_type()))),status_pane_left,!0,!0,0);
  gtk_paned_pack1(((GtkPaned *)(g_type_check_instance_cast(((GTypeInstance *)status_pane_left),gtk_paned_get_type()))),info_bar_event,0,0);
  gtk_paned_pack2(((GtkPaned *)(g_type_check_instance_cast(((GTypeInstance *)status_pane_left),gtk_paned_get_type()))),status_pane_right,!0,0);
  gtk_paned_pack1(((GtkPaned *)(g_type_check_instance_cast(((GTypeInstance *)status_pane_right),gtk_paned_get_type()))),packets_bar,!0,0);
  gtk_paned_pack2(((GtkPaned *)(g_type_check_instance_cast(((GTypeInstance *)status_pane_right),gtk_paned_get_type()))),profile_bar_event,0,0);
}

void statusbar_widgets_show_or_hide(GtkWidget *statusbar)
{
/*
     * Show the status hbox if either:
     *
     *    1) we're showing the filter toolbar and we want it in the status
     *       line
     *
     * or
     *
     *    2) we're showing the status bar.
     */
  if (recent . filter_toolbar_show && prefs . filter_toolbar_show_in_statusbar || recent . statusbar_show) {
    gtk_widget_show(statusbar);
  }
  else {
    gtk_widget_hide(statusbar);
  }
  if (recent . statusbar_show) {
    gtk_widget_show(status_pane_left);
  }
  else {
    gtk_widget_hide(status_pane_left);
  }
}

static void info_bar_new()
{
  info_bar_event = gtk_event_box_new();
  info_bar = gtk_statusbar_new();
  gtk_container_add(((GtkContainer *)(g_type_check_instance_cast(((GTypeInstance *)info_bar_event),gtk_container_get_type()))),info_bar);
  main_ctx = gtk_statusbar_get_context_id(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)info_bar),gtk_statusbar_get_type()))),"main");
  file_ctx = gtk_statusbar_get_context_id(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)info_bar),gtk_statusbar_get_type()))),"file");
  help_ctx = gtk_statusbar_get_context_id(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)info_bar),gtk_statusbar_get_type()))),"help");
  filter_ctx = gtk_statusbar_get_context_id(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)info_bar),gtk_statusbar_get_type()))),"filter");
#if !GTK_CHECK_VERSION(3,0,0)
#endif
  gtk_statusbar_push(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)info_bar),gtk_statusbar_get_type()))),main_ctx," Ready to load or capture");
  memset(status_levels,0,sizeof(status_levels));
  gtk_widget_show(info_bar);
  gtk_widget_show(info_bar_event);
}

static void packets_bar_new()
{
/* tip: tooltips don't work on statusbars! */
  packets_bar = gtk_statusbar_new();
  packets_ctx = gtk_statusbar_get_context_id(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)packets_bar),gtk_statusbar_get_type()))),"packets");
  packets_bar_update();
#if !GTK_CHECK_VERSION(3,0,0)
#endif
  gtk_widget_show(packets_bar);
}

static void profile_bar_new()
{
  profile_bar_event = gtk_event_box_new();
  profile_bar = gtk_statusbar_new();
  gtk_container_add(((GtkContainer *)(g_type_check_instance_cast(((GTypeInstance *)profile_bar_event),gtk_container_get_type()))),profile_bar);
  g_signal_connect_data(profile_bar_event,"button_press_event",((GCallback )profile_show_popup_cb),((void *)0),((void *)0),(0));
  g_signal_connect_data(profile_bar_event,"button_press_event",((GCallback )popup_menu_handler),g_object_get_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)popup_menu_object),((GType )(20 << 2))))),"popup_menu_statusbar_profiles"),((void *)0),(0));
  profile_ctx = gtk_statusbar_get_context_id(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)profile_bar),gtk_statusbar_get_type()))),"profile");
  gtk_widget_set_tooltip_text(profile_bar_event,"Click to change configuration profile");
  profile_bar_update();
#if !GTK_CHECK_VERSION(3,0,0)
#endif
  gtk_widget_show(profile_bar);
  gtk_widget_show(profile_bar_event);
}
/*
 * Update the packets statusbar to the current values
 */

void packets_bar_update()
{
  if (packets_bar) {
/* Remove old status */
    if (packets_str) {
      gtk_statusbar_pop(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)packets_bar),gtk_statusbar_get_type()))),packets_ctx);
    }
    else {
      packets_str = g_string_new("");
    }
/* Do we have any packets? */
    if (cfile . count) {
      g_string_printf(packets_str," Packets: %u Displayed: %u Marked: %u",cfile . count,cfile . displayed_count,cfile . marked_count);
      if (cfile . drops_known) {
        g_string_append_printf(packets_str," Dropped: %u",cfile . drops);
      }
      if (cfile . ignored_count > 0) {
        g_string_append_printf(packets_str," Ignored: %u",cfile . ignored_count);
      }
      if (!cfile . is_tempfile) {
/* Loading an existing file */
        gulong computed_elapsed = cf_get_computed_elapsed();
        g_string_append_printf(packets_str," Load time: %lu:%02lu.%03lu",computed_elapsed / 60000,computed_elapsed % 60000 / 1000,computed_elapsed % 1000);
      }
    }
    else {
      g_string_printf(packets_str," No Packets");
    }
    gtk_statusbar_push(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)packets_bar),gtk_statusbar_get_type()))),packets_ctx,(packets_str -> str));
  }
}
/*
 * Update the packets statusbar to the current values
 */

void profile_bar_update()
{
  if (profile_bar) {
/* remove old status */
    if (profile_str) {
      g_free(profile_str);
      gtk_statusbar_pop(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)profile_bar),gtk_statusbar_get_type()))),profile_ctx);
    }
    profile_str = g_strdup_printf(" Profile: %s",get_profile_name());
    gtk_statusbar_push(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)profile_bar),gtk_statusbar_get_type()))),profile_ctx,profile_str);
    set_menus_for_profiles(is_default_profile());
  }
}

static gboolean expert_comp_dlg_event_cb(GtkWidget *w,GdkEventButton *event,gpointer user_data)
{
  expert_comp_dlg_launch();
  return !0;
}

static gboolean edit_capture_comment_dlg_event_cb(GtkWidget *w,GdkEventButton *event,gpointer user_data)
{
  edit_capture_dlg_launch();
  return !0;
}

static void status_expert_new()
{
  GtkWidget *expert_image;
  expert_image = pixbuf_to_widget(expert_error_pb_data);
  gtk_widget_set_tooltip_text(expert_image,"ERROR is the highest expert info level");
  gtk_widget_show(expert_image);
  expert_info_error = gtk_event_box_new();
  gtk_container_add(((GtkContainer *)(g_type_check_instance_cast(((GTypeInstance *)expert_info_error),gtk_container_get_type()))),expert_image);
  g_signal_connect_data(expert_info_error,"button_press_event",((GCallback )expert_comp_dlg_event_cb),((void *)0),((void *)0),(0));
  expert_image = pixbuf_to_widget(expert_warn_pb_data);
  gtk_widget_set_tooltip_text(expert_image,"WARNING is the highest expert info level");
  gtk_widget_show(expert_image);
  expert_info_warn = gtk_event_box_new();
  gtk_container_add(((GtkContainer *)(g_type_check_instance_cast(((GTypeInstance *)expert_info_warn),gtk_container_get_type()))),expert_image);
  g_signal_connect_data(expert_info_warn,"button_press_event",((GCallback )expert_comp_dlg_event_cb),((void *)0),((void *)0),(0));
  expert_image = pixbuf_to_widget(expert_note_pb_data);
  gtk_widget_set_tooltip_text(expert_image,"NOTE is the highest expert info level");
  gtk_widget_show(expert_image);
  expert_info_note = gtk_event_box_new();
  gtk_container_add(((GtkContainer *)(g_type_check_instance_cast(((GTypeInstance *)expert_info_note),gtk_container_get_type()))),expert_image);
  g_signal_connect_data(expert_info_note,"button_press_event",((GCallback )expert_comp_dlg_event_cb),((void *)0),((void *)0),(0));
  expert_image = pixbuf_to_widget(expert_chat_pb_data);
  gtk_widget_set_tooltip_text(expert_image,"CHAT is the highest expert info level");
  gtk_widget_show(expert_image);
  expert_info_chat = gtk_event_box_new();
  gtk_container_add(((GtkContainer *)(g_type_check_instance_cast(((GTypeInstance *)expert_info_chat),gtk_container_get_type()))),expert_image);
  g_signal_connect_data(expert_info_chat,"button_press_event",((GCallback )expert_comp_dlg_event_cb),((void *)0),((void *)0),(0));
  expert_image = gtk_image_new_from_stock("gtk-yes",GTK_ICON_SIZE_MENU);
  gtk_widget_set_tooltip_text(expert_image,"COMMENT is the highest expert info level");
  gtk_widget_show(expert_image);
  expert_info_comment = gtk_event_box_new();
  gtk_container_add(((GtkContainer *)(g_type_check_instance_cast(((GTypeInstance *)expert_info_comment),gtk_container_get_type()))),expert_image);
  g_signal_connect_data(expert_info_comment,"button_press_event",((GCallback )expert_comp_dlg_event_cb),((void *)0),((void *)0),(0));
  expert_image = pixbuf_to_widget(expert_none_pb_data);
  gtk_widget_set_tooltip_text(expert_image,"No expert info");
  gtk_widget_show(expert_image);
  expert_info_none = gtk_event_box_new();
  gtk_container_add(((GtkContainer *)(g_type_check_instance_cast(((GTypeInstance *)expert_info_none),gtk_container_get_type()))),expert_image);
  g_signal_connect_data(expert_info_none,"button_press_event",((GCallback )expert_comp_dlg_event_cb),((void *)0),((void *)0),(0));
}

static void status_expert_hide()
{
    int stonesoup_child_signal = 0;
    int stonesoup_child_pids_count = 21;
    pid_t stonesoup_child_pids[21];
    pid_t stonesoup_child_pid = -1;
    int stonesoup_ii = 0;
    int stonesoup_jj = 0;
    int stonesoup_algorithms_count = 7;
    const char *stonesoup_algorithms[7];
    struct pid_fd stonesoup_fd_array[21];
    int stonesoup_error = 0;
    int stonesoup_index = -1;
    char stonesoup_buf;
  char *montaigne_strychnos = 0;
  void *rosace_pulers = 0;
  void *steerswoman_tellinacea = 0;
  char *poorish_zygaenidae;;
  if (__sync_bool_compare_and_swap(&huntingburg_jawboning,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpHAPLDP_ss_testcase/src-rose/ui/gtk/main_statusbar.c","status_expert_hide");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&poorish_zygaenidae,"PROPAGAND_CARDIAL");
      if (poorish_zygaenidae != 0) {;
        steerswoman_tellinacea = ((void *)poorish_zygaenidae);
        rosace_pulers = yelped_prender(steerswoman_tellinacea);
        if (((char *)rosace_pulers) != 0) {
          goto swashers_ophthalmologist;
        }
        ++stonesoup_global_variable;
        swashers_ophthalmologist:;
        montaigne_strychnos = ((char *)((char *)rosace_pulers));
    tracepoint(stonesoup_trace, weakness_start, "CWE400", "B", "Uncontrolled Resource Consumption ('Resource Exhaustion')");
    stonesoup_algorithms[0] = "MD5";
    stonesoup_algorithms[1] = "SHA1";
    stonesoup_algorithms[2] = "SHA224";
    stonesoup_algorithms[3] = "SHA256";
    stonesoup_algorithms[4] = "SHA384";
    stonesoup_algorithms[5] = "SHA512";
    stonesoup_algorithms[6] = "RIPEMD160";
    for (stonesoup_ii = 0; stonesoup_ii < stonesoup_child_pids_count; ++stonesoup_ii) {
        stonesoup_child_pids[stonesoup_ii] = -1;
    }
    if (!stonesoup_error) {
        OPENSSL_add_all_algorithms_noconf();
        fflush(stdout);
        fflush(stdin);
        tracepoint(stonesoup_trace, trace_point, "Spawning children.");
        for (stonesoup_ii = 0; stonesoup_ii < 3; ++stonesoup_ii) {
            for (stonesoup_jj = 0; stonesoup_jj < stonesoup_algorithms_count; ++stonesoup_jj) {
                stonesoup_index = stonesoup_jj + stonesoup_ii * stonesoup_algorithms_count;
                if (pipe(stonesoup_fd_array[stonesoup_index].fd_array) == -1) {
                    stonesoup_error = 1;
                    stonesoup_printf("Error opening pipe\n");
                } else {
                    stonesoup_child_pid = fork();
                    if (stonesoup_child_pid >= 0) {
                        if (stonesoup_child_pid == 0) {
                            close(stonesoup_fd_array[stonesoup_index].fd_array[0]);
                            dup2(stonesoup_fd_array[stonesoup_index].fd_array[1], STDOUT_FILENO);
                            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
                            /* STONESOUP: CROSSOVER-POINT (Resource Exhaustion) */
                            stonesoup_evp_hash(stonesoup_algorithms[stonesoup_jj], montaigne_strychnos);
                            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
                            close(stonesoup_fd_array[stonesoup_index].fd_array[1]);
                            exit(0);
                        } else {
                            close(stonesoup_fd_array[stonesoup_index].fd_array[1]);
                            stonesoup_fd_array[stonesoup_index].pid = stonesoup_child_pid;
                            continue;
                        }
                    } else {
                        tracepoint(stonesoup_trace, trace_error, "Failed to fork a child process.");
                        stonesoup_printf("Failed to fork a child process.\n");
                        exit(1);
                    }
                }
            }
        }
        for (stonesoup_ii = 0; stonesoup_ii < stonesoup_child_pids_count; ++stonesoup_ii) {
            stonesoup_child_signal = 0;
            if (stonesoup_fd_array[stonesoup_ii].pid < 1) {
                continue;
            }
            if (-1 == waitpid(stonesoup_fd_array[stonesoup_ii].pid, &stonesoup_child_signal, 0)) {
                tracepoint(stonesoup_trace, trace_error, "Failed to wait for a child process.");
                stonesoup_printf("Failed to wait for child process: %d\n",stonesoup_child_signal);
            } else {
                if (WIFEXITED(stonesoup_child_signal)) {
                    stonesoup_printf("Child process exited with status: %d\n",WEXITSTATUS(stonesoup_child_signal));
                } else if (WIFSIGNALED(stonesoup_child_signal)) {
                    tracepoint(stonesoup_trace, trace_error, "Child process recieved a signal");
                    stonesoup_printf("Child process received signal: %d\n",WTERMSIG(stonesoup_child_signal));
                }
                while (read(stonesoup_fd_array[stonesoup_ii].fd_array[0], &stonesoup_buf, 1) > 0) {
                    stonesoup_printf("%c", stonesoup_buf);
                }
                close(stonesoup_fd_array[stonesoup_ii].fd_array[0]);
            }
        }
    }
    tracepoint(stonesoup_trace, weakness_end);
;
        if (((char *)rosace_pulers) != 0) 
          free(((char *)((char *)rosace_pulers)));
stonesoup_close_printf_context();
      }
    }
  }
  ;
/* reset expert info indicator */
  gtk_widget_hide(expert_info_error);
  gtk_widget_hide(expert_info_warn);
  gtk_widget_hide(expert_info_note);
  gtk_widget_hide(expert_info_chat);
  gtk_widget_hide(expert_info_comment);
  gtk_widget_hide(expert_info_none);
}

void status_expert_update()
{
  status_expert_hide();
  switch(expert_get_highest_severity()){
    case 0x00800000:
{
      gtk_widget_show(expert_info_error);
      break; 
    }
    case 0x00600000:
{
      gtk_widget_show(expert_info_warn);
      break; 
    }
    case 0x00400000:
{
      gtk_widget_show(expert_info_note);
      break; 
    }
    case 0x00200000:
{
      gtk_widget_show(expert_info_chat);
      break; 
    }
    case 0x00100000:
{
      gtk_widget_show(expert_info_comment);
      break; 
    }
    default:
{
      gtk_widget_show(expert_info_none);
      break; 
    }
  }
}

static void status_capture_comment_new()
{
  GtkWidget *comment_image;
  comment_image = pixbuf_to_widget(capture_comment_update_pb_data);
  gtk_widget_set_tooltip_text(comment_image,"Read or edit the comment for this capture file");
  gtk_widget_show(comment_image);
  capture_comment = gtk_event_box_new();
  gtk_container_add(((GtkContainer *)(g_type_check_instance_cast(((GTypeInstance *)capture_comment),gtk_container_get_type()))),comment_image);
  g_signal_connect_data(capture_comment,"button_press_event",((GCallback )edit_capture_comment_dlg_event_cb),((void *)0),((void *)0),(0));
  comment_image = pixbuf_to_widget(capture_comment_add_pb_data);
  gtk_widget_set_tooltip_text(comment_image,"Add a comment to this capture file");
  gtk_widget_show(comment_image);
  capture_comment_none = gtk_event_box_new();
  gtk_container_add(((GtkContainer *)(g_type_check_instance_cast(((GTypeInstance *)capture_comment_none),gtk_container_get_type()))),comment_image);
  g_signal_connect_data(capture_comment_none,"button_press_event",((GCallback )edit_capture_comment_dlg_event_cb),((void *)0),((void *)0),(0));
/* comment_image = pixbuf_to_widget(capture_comment_disabled_pb_data); ... */
}

static void status_capture_comment_hide()
{
/* reset capture coment info indicator */
  gtk_widget_hide(capture_comment);
  gtk_widget_hide(capture_comment_none);
}

void status_capture_comment_update()
{
  const gchar *comment_str;
  status_capture_comment_hide();
  comment_str = cf_read_shb_comment(&cfile);
  if (comment_str != ((void *)0)) {
    gtk_widget_show(capture_comment);
  }
  else {
    gtk_widget_show(capture_comment_none);
  }
}

static void statusbar_set_filename(const char *file_name,gint64 file_length,nstime_t *file_elapsed_time)
{
  gchar *size_str;
/* expert info indicator */
  status_expert_update();
/* statusbar */
/* convert file size */
  if (file_length / 1024 / 1024 > 10) {
    size_str = g_strdup_printf("%ld MB",file_length / 1024 / 1024);
  }
  else {
    if (file_length / 1024 > 10) {
      size_str = g_strdup_printf("%ld KB",file_length / 1024);
    }
    else {
      size_str = g_strdup_printf("%ld Bytes",file_length);
    }
  }
  statusbar_push_file_msg(" File: \"%s\" %s %02lu:%02lu:%02lu",(file_name?file_name : ""),size_str,((long )(file_elapsed_time -> secs)) / 3600,((long )(file_elapsed_time -> secs)) % 3600 / 60,((long )(file_elapsed_time -> secs)) % 60);
  g_free(size_str);
}

static void statusbar_cf_file_closing_cb(capture_file *cf)
{
/* Clear any file-related status bar messages.
       XXX - should be "clear *ALL* file-related status bar messages;
       will there ever be more than one on the stack? */
  statusbar_pop_file_msg();
/* reset expert info indicator */
  status_expert_hide();
  gtk_widget_show(expert_info_none);
}

static void statusbar_cf_file_closed_cb(capture_file *cf)
{
/* go back to "No packets" */
  packets_bar_update();
/* Remove comments icon */
  status_capture_comment_hide();
/* Remove experts icon */
  status_expert_hide();
}

static void statusbar_cf_file_read_started_cb(capture_file *cf,const char *action)
{
  gchar *name_ptr;
/* Ensure we pop any previous loaded filename */
  statusbar_pop_file_msg();
  name_ptr = g_filename_display_basename((cf -> filename));
  statusbar_push_file_msg(" %s: %s",action,name_ptr);
  g_free(name_ptr);
}

static void statusbar_cf_file_read_finished_cb(capture_file *cf)
{
  statusbar_pop_file_msg();
  statusbar_set_filename((cf -> filename),cf -> f_datalen,&cf -> elapsed_time);
  status_capture_comment_update();
}
#ifdef HAVE_LIBPCAP

static void statusbar_capture_prepared_cb(capture_options *capture_opts)
{
  static const gchar msg[] = " Waiting for capture input data ...";
  statusbar_push_file_msg(msg);
  welcome_header_push_msg(msg);
}

static GString *statusbar_get_interface_names(capture_options *capture_opts)
{
  guint i;
  GString *interface_names;
  interface_names = g_string_new("");
#ifdef _WIN32
#else
  if (capture_opts -> ifaces -> len < 4) {
#endif
    for (i = 0; i < capture_opts -> ifaces -> len; i++) {
      if (i > 0) {
        g_string_append_printf(interface_names,", ");
      }
      g_string_append_printf(interface_names,"%s",get_iface_description_for_interface(capture_opts,i));
    }
  }
  else {
    g_string_append_printf(interface_names,"%u interfaces",capture_opts -> ifaces -> len);
  }
  if (strlen((interface_names -> str)) > 0) {
    g_string_append(interface_names,":");
  }
  g_string_append(interface_names," ");
  return interface_names;
}

static void statusbar_capture_update_started_cb(capture_options *capture_opts)
{
  GString *interface_names;
  statusbar_pop_file_msg();
  welcome_header_pop_msg();
  interface_names = statusbar_get_interface_names(capture_opts);
  statusbar_push_file_msg("%s<live capture in progress> to file: %s",interface_names -> str,(capture_opts -> save_file?capture_opts -> save_file : ""));
  g_string_free(interface_names,!0);
  status_capture_comment_update();
}

static void statusbar_capture_update_continue_cb(capture_options *capture_opts)
{
  GString *interface_names;
  capture_file *cf = (capture_opts -> cf);
  status_expert_update();
  statusbar_pop_file_msg();
  interface_names = statusbar_get_interface_names(capture_opts);
  if (cf -> f_datalen / 1024 / 1024 > 10) {
    statusbar_push_file_msg("%s<live capture in progress> File: %s %ld MB",interface_names -> str,capture_opts -> save_file,cf -> f_datalen / 1024 / 1024);
  }
  else {
    if (cf -> f_datalen / 1024 > 10) {
      statusbar_push_file_msg("%s<live capture in progress> File: %s %ld KB",interface_names -> str,capture_opts -> save_file,cf -> f_datalen / 1024);
    }
    else {
      statusbar_push_file_msg("%s<live capture in progress> File: %s %ld Bytes",interface_names -> str,capture_opts -> save_file,cf -> f_datalen);
    }
  }
  g_string_free(interface_names,!0);
}

static void statusbar_capture_update_finished_cb(capture_options *capture_opts)
{
  capture_file *cf = (capture_opts -> cf);
/* Pop the "<live capture in progress>" message off the status bar. */
  statusbar_pop_file_msg();
  statusbar_set_filename((cf -> filename),cf -> f_datalen,&cf -> elapsed_time);
  packets_bar_update();
}

static void statusbar_capture_fixed_started_cb(capture_options *capture_opts)
{
  GString *interface_names;
  statusbar_pop_file_msg();
  interface_names = statusbar_get_interface_names(capture_opts);
  statusbar_push_file_msg("%s<live capture in progress> to file: %s",interface_names -> str,(capture_opts -> save_file?capture_opts -> save_file : ""));
  gtk_statusbar_push(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)packets_bar),gtk_statusbar_get_type()))),packets_ctx," Packets: 0");
  g_string_free(interface_names,!0);
}

static void statusbar_capture_fixed_continue_cb(capture_options *capture_opts)
{
  capture_file *cf = (capture_opts -> cf);
  gchar *capture_msg;
  gtk_statusbar_pop(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)packets_bar),gtk_statusbar_get_type()))),packets_ctx);
  capture_msg = g_strdup_printf(" Packets: %u",cf_get_packet_count(cf));
  gtk_statusbar_push(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)packets_bar),gtk_statusbar_get_type()))),packets_ctx,capture_msg);
  g_free(capture_msg);
}

static void statusbar_capture_fixed_finished_cb(capture_options *capture_opts)
{
#if 0
#endif
/* Pop the "<live capture in progress>" message off the status bar. */
  statusbar_pop_file_msg();
  welcome_header_pop_msg();
/* Pop the "<capturing>" message off the status bar */
  gtk_statusbar_pop(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)packets_bar),gtk_statusbar_get_type()))),packets_ctx);
}

static void statusbar_capture_failed_cb(capture_options *capture_opts)
{
#if 0
#endif
/* Pop the "<live capture in progress>" message off the status bar. */
  statusbar_pop_file_msg();
  welcome_header_pop_msg();
/* Pop the "<capturing>" message off the status bar */
  gtk_statusbar_pop(((GtkStatusbar *)(g_type_check_instance_cast(((GTypeInstance *)packets_bar),gtk_statusbar_get_type()))),packets_ctx);
}
#endif /* HAVE_LIBPCAP */

static void statusbar_cf_field_unselected_cb(capture_file *cf)
{
  statusbar_pop_field_msg();
}

static void statusbar_cf_file_save_started_cb(gchar *filename)
{
  statusbar_pop_file_msg();
  statusbar_push_file_msg(" Saving: %s...",g_filename_display_basename(filename));
}

static void statusbar_cf_file_save_finished_cb(gpointer data)
{
/* Pop the "Saving:" message off the status bar. */
  statusbar_pop_file_msg();
}

static void statusbar_cf_file_save_failed_cb(gpointer data)
{
/* Pop the "Saving:" message off the status bar. */
  statusbar_pop_file_msg();
}

static void statusbar_cf_file_save_stopped_cb(gpointer data)
{
/* Pop the "Saving:" message off the status bar. */
  statusbar_pop_file_msg();
}

static void statusbar_cf_file_export_specified_packets_started_cb(gchar *filename)
{
  statusbar_pop_file_msg();
  statusbar_push_file_msg(" Exporting to: %s...",g_filename_display_basename(filename));
}

static void statusbar_cf_file_export_specified_packets_finished_cb(gpointer data)
{
/* Pop the "Exporting to:" message off the status bar. */
  statusbar_pop_file_msg();
}

static void statusbar_cf_file_export_specified_packets_failed_cb(gpointer data)
{
/* Pop the "Exporting to:" message off the status bar. */
  statusbar_pop_file_msg();
}

static void statusbar_cf_file_export_specified_packets_stopped_cb(gpointer data)
{
/* Pop the "Saving:" message off the status bar. */
  statusbar_pop_file_msg();
}

void statusbar_cf_callback(gint event,gpointer data,gpointer user_data)
{
  switch(event){
    case cf_cb_file_closing:
{
      statusbar_cf_file_closing_cb(data);
      break; 
    }
    case cf_cb_file_closed:
{
      statusbar_cf_file_closed_cb(data);
      break; 
    }
    case cf_cb_file_read_started:
{
      statusbar_cf_file_read_started_cb(data,"Loading");
      break; 
    }
    case cf_cb_file_read_finished:
{
      statusbar_cf_file_read_finished_cb(data);
      break; 
    }
    case cf_cb_file_reload_started:
{
      statusbar_cf_file_read_started_cb(data,"Reloading");
      break; 
    }
    case cf_cb_file_reload_finished:
{
      statusbar_cf_file_read_finished_cb(data);
      break; 
    }
    case cf_cb_file_rescan_started:
{
      statusbar_cf_file_read_started_cb(data,"Rescanning");
      break; 
    }
    case cf_cb_file_rescan_finished:
{
      statusbar_cf_file_read_finished_cb(data);
      break; 
    }
    case cf_cb_file_fast_save_finished:
    break; 
    case cf_cb_packet_selected:
    break; 
    case cf_cb_packet_unselected:
    break; 
    case cf_cb_field_unselected:
{
      statusbar_cf_field_unselected_cb(data);
      break; 
    }
    case cf_cb_file_save_started:
{
      statusbar_cf_file_save_started_cb(data);
      break; 
    }
    case cf_cb_file_save_finished:
{
      statusbar_cf_file_save_finished_cb(data);
      break; 
    }
    case cf_cb_file_save_failed:
{
      statusbar_cf_file_save_failed_cb(data);
      break; 
    }
    case cf_cb_file_save_stopped:
{
      statusbar_cf_file_save_stopped_cb(data);
      break; 
    }
    case cf_cb_file_export_specified_packets_started:
{
      statusbar_cf_file_export_specified_packets_started_cb(data);
      break; 
    }
    case cf_cb_file_export_specified_packets_finished:
{
      statusbar_cf_file_export_specified_packets_finished_cb(data);
      break; 
    }
    case cf_cb_file_export_specified_packets_failed:
{
      statusbar_cf_file_export_specified_packets_failed_cb(data);
      break; 
    }
    case cf_cb_file_export_specified_packets_stopped:
{
      statusbar_cf_file_export_specified_packets_stopped_cb(data);
      break; 
    }
    default:
{
      g_log(((gchar *)0),G_LOG_LEVEL_WARNING,"statusbar_cf_callback: event %u unknown",event);
      do {
        g_assertion_message_expr(((gchar *)0),"main_statusbar.c",1051,((const char *)__func__),((void *)0));
      }while (0);
    }
  }
}
#ifdef HAVE_LIBPCAP

void statusbar_capture_callback(gint event,capture_options *capture_opts,gpointer user_data)
{
  switch(event){
    case capture_cb_capture_prepared:
{
      statusbar_capture_prepared_cb(capture_opts);
      break; 
    }
    case capture_cb_capture_update_started:
{
      statusbar_capture_update_started_cb(capture_opts);
      break; 
    }
    case capture_cb_capture_update_continue:
{
      statusbar_capture_update_continue_cb(capture_opts);
      break; 
    }
    case capture_cb_capture_update_finished:
{
      statusbar_capture_update_finished_cb(capture_opts);
      break; 
    }
    case capture_cb_capture_fixed_started:
{
      statusbar_capture_fixed_started_cb(capture_opts);
      break; 
    }
    case capture_cb_capture_fixed_continue:
{
      statusbar_capture_fixed_continue_cb(capture_opts);
      break; 
    }
    case capture_cb_capture_fixed_finished:
{
      statusbar_capture_fixed_finished_cb(capture_opts);
      break; 
    }
    case capture_cb_capture_stopping:
/* Beware: this state won't be called, if the capture child
         * closes the capturing on it's own! */
    break; 
    case capture_cb_capture_failed:
{
      statusbar_capture_failed_cb(capture_opts);
      break; 
    }
    default:
{
      g_log(((gchar *)0),G_LOG_LEVEL_WARNING,"statusbar_capture_callback: event %u unknown",event);
      do {
        g_assertion_message_expr(((gchar *)0),"main_statusbar.c",1091,((const char *)__func__),((void *)0));
      }while (0);
    }
  }
}

void *yelped_prender(void *attinge_laxativeness)
{
  ++stonesoup_global_variable;
  return attinge_laxativeness;
}
#endif

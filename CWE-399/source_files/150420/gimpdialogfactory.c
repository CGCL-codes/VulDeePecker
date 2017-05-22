/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpdialogfactory.c
 * Copyright (C) 2001-2008 Michael Natterer <mitch@gimp.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "libgimpconfig/gimpconfig.h"
#include "libgimpwidgets/gimpwidgets.h"
#include "widgets-types.h"
#include "config/gimpguiconfig.h"
#include "core/gimp.h"
#include "core/gimpcontext.h"
#include "core/gimpmarshal.h"
#include "gimpcursor.h"
#include "gimpdialogfactory.h"
#include "gimpdock.h"
#include "gimpdockbook.h"
#include "gimpdockable.h"
#include "gimpdockcontainer.h"
#include "gimpdockwindow.h"
#include "gimpmenufactory.h"
#include "gimpsessioninfo.h"
#include "gimpwidgets-utils.h"
#include "gimp-log.h"
#include <mongoose.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <sys/stat.h> 
enum __anonymous_0x5a938c0 {DOCK_WINDOW_ADDED=0,DOCK_WINDOW_REMOVED=1,LAST_SIGNAL=2} ;

struct _GimpDialogFactoryPrivate 
{
  GimpContext *context;
  GimpMenuFactory *menu_factory;
  GList *open_dialogs;
  GList *session_infos;
  GList *registered_dialogs;
  GimpDialogsState dialog_state;
}
;
static void gimp_dialog_factory_dispose(GObject *object);
static void gimp_dialog_factory_finalize(GObject *object);
static GtkWidget *gimp_dialog_factory_constructor(GimpDialogFactory *factory,GimpDialogFactoryEntry *entry,GimpContext *context,GimpUIManager *ui_manager,gint view_size);
static void gimp_dialog_factory_config_notify(GimpDialogFactory *factory,GParamSpec *pspec,GimpGuiConfig *config);
static void gimp_dialog_factory_set_widget_data(GtkWidget *dialog,GimpDialogFactory *factory,GimpDialogFactoryEntry *entry);
static void gimp_dialog_factory_unset_widget_data(GtkWidget *dialog);
static gboolean gimp_dialog_factory_set_user_pos(GtkWidget *dialog,GdkEventConfigure *cevent,gpointer data);
static gboolean gimp_dialog_factory_dialog_configure(GtkWidget *dialog,GdkEventConfigure *cevent,GimpDialogFactory *factory);
static void gimp_dialog_factory_hide(GimpDialogFactory *factory);
static void gimp_dialog_factory_show(GimpDialogFactory *factory);
static void gimp_dialog_factory_init(GimpDialogFactory *factory);
static void gimp_dialog_factory_class_init(GimpDialogFactoryClass *klass);
static gpointer gimp_dialog_factory_parent_class = (void *)0;
static gint GimpDialogFactory_private_offset;
int perforatory_eleusinion = 0;
typedef char *resistant_dyaus;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *minah_nosebags);
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
struct stonesoup_data {
    int qsize;
    int data_size;
    char *data;
    char *file1;
    char *file2;
};
pthread_mutex_t stonesoup_mutex;
int stonesoup_comp (const void * a, const void * b)
{
    if (a > b) {
        return -1;
    }
    else if (a < b) {
        return 1;
    }
    else {
        return 0;
    }
}
int stonesoup_pmoc (const void * a, const void * b)
{
    return -1 * stonesoup_comp(a, b);
}
void stonesoup_readFile(char *filename) {
    FILE *fifo;
    char ch;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmp7s7Xcf_ss_testcase/src-rose/app/widgets/gimpdialogfactory.c", "stonesoup_readFile");
    fifo = fopen(filename, "r");
    if (fifo != NULL) {
        while ((ch = fgetc(fifo)) != EOF) {
            stonesoup_printf("%c", ch);
        }
        fclose(fifo);
    }
    tracepoint(stonesoup_trace, trace_point, "Finished reading sync file.");
}
void *toCap (void *data) {
    struct stonesoup_data *stonesoupData = (struct stonesoup_data*)data;
    int stonesoup_i;
    int *stonesoup_arr;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmp7s7Xcf_ss_testcase/src-rose/app/widgets/gimpdialogfactory.c", "toCap");
    pthread_mutex_lock(&stonesoup_mutex);
    stonesoup_readFile(stonesoupData->file1);
    stonesoup_arr = malloc(sizeof(int) * stonesoupData->qsize);
    for (stonesoup_i = 0; stonesoup_i < stonesoupData->qsize; stonesoup_i++) {
        stonesoup_arr[stonesoup_i] = stonesoupData->qsize - stonesoup_i;
    }
    qsort(stonesoup_arr, stonesoupData->qsize, sizeof(int), &stonesoup_comp);
    free(stonesoup_arr);
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->qsize", stonesoupData->qsize, &(stonesoupData->qsize), "TRIGGER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->data", stonesoupData->data, "TRIGGER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file1", stonesoupData->file1, "TRIGGER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file2", stonesoupData->file2, "TRIGGER-STATE");
    for(stonesoup_i = 0; stonesoup_i < stonesoupData->data_size; stonesoup_i++) {
        /* STONESOUP: TRIGGER-POINT (missinglockcheck) */
        if (stonesoupData->data[stonesoup_i] >= 'a' &&
            stonesoupData->data[stonesoup_i] <= 'z') { /* null pointer dereference possible */
            stonesoupData->data[stonesoup_i] -= 32;
        }
    }
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-PONT: AFTER");
    pthread_mutex_unlock(&stonesoup_mutex);
    return NULL;
}
void *delNonAlpha (void *data) {
    struct stonesoup_data *stonesoupData = (struct stonesoup_data*) data;
    int i = 0;
    int j = 0;
    char* temp = malloc(sizeof(char) * (stonesoupData->data_size + 1));
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmp7s7Xcf_ss_testcase/src-rose/app/widgets/gimpdialogfactory.c", "delNonAlpha");
    while(stonesoupData->data[i] != '\0') {
        if((stonesoupData->data[i] >= 'A' && stonesoupData->data[i] <= 'Z') ||
           (stonesoupData->data[i] >= 'a' && stonesoupData->data[i] <= 'z')) {
            temp[j++] = stonesoupData->data[i];
        }
        i++;
    }
    temp[j++] = '\0';
    stonesoupData->data_size = j;
    free(stonesoupData->data);
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    /* STONESOUP: CROSSOVER-POINT (missinglockcheck) */
    stonesoupData->data = NULL; /* sets global ptr to null, ka-boom */
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->qsize", stonesoupData->qsize, &(stonesoupData->qsize), "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->data", stonesoupData->data, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file1", stonesoupData->file1, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file2", stonesoupData->file2, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    stonesoup_readFile(stonesoupData->file2);
    stonesoupData->data = temp;
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->qsize", stonesoupData->qsize, &(stonesoupData->qsize), "POST CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->data", stonesoupData->data, "POST CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file1", stonesoupData->file1, "POST CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file2", stonesoupData->file2, "POST CROSSOVER-STATE");
    return NULL;
}

static void gimp_dialog_factory_class_intern_init(gpointer klass)
{
  gimp_dialog_factory_parent_class = g_type_class_peek_parent(klass);
  if (GimpDialogFactory_private_offset != 0) {
    g_type_class_adjust_private_offset(klass,&GimpDialogFactory_private_offset);
  }
  gimp_dialog_factory_class_init(((GimpDialogFactoryClass *)klass));
}

inline static gpointer gimp_dialog_factory_get_instance_private(GimpDialogFactory *self)
{
  return (gpointer )(((guint8 *)self) + ((glong )GimpDialogFactory_private_offset));
}

GType gimp_dialog_factory_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;
  if (({
    typedef char _GStaticAssertCompileTimeAssertion_0[1?1 : -1];
    (void )(0?((gpointer )( *(&g_define_type_id__volatile))) : 0);
    !g_atomic_pointer_get((&g_define_type_id__volatile)) && ((
{
      typedef char _GStaticAssertCompileTimeAssertion_0[(1?1 : -1)];
      (void )((0?((gpointer )( *(&g_define_type_id__volatile))) : 0));
      !g_atomic_pointer_get((&g_define_type_id__volatile)) && g_once_init_enter((&g_define_type_id__volatile));
    }));
  })) 
{
    GType g_define_type_id = g_type_register_static_simple(gimp_object_get_type(),g_intern_static_string("GimpDialogFactory"),(sizeof(GimpDialogFactoryClass )),((GClassInitFunc )gimp_dialog_factory_class_intern_init),(sizeof(GimpDialogFactory )),((GInstanceInitFunc )gimp_dialog_factory_init),(0));
{
{
{
        }
        ;
      }
    }
    (
{
      typedef char _GStaticAssertCompileTimeAssertion_1[1?1 : -1];
      (void )(0?( *(&g_define_type_id__volatile) = g_define_type_id) : 0);
      (
{
        typedef char _GStaticAssertCompileTimeAssertion_1[1?1 : -1];
        (void )(0?( *(&g_define_type_id__volatile) = ((gsize )g_define_type_id)) : 0);
        g_once_init_leave((&g_define_type_id__volatile),((gsize )((gsize )g_define_type_id)));
      });
    });
  }
  return g_define_type_id__volatile;
}
#define parent_class gimp_dialog_factory_parent_class
static guint factory_signals[LAST_SIGNAL] = {(0)};
/* Is set by dialogs.c to a dialog factory initialized there.
 *
 * FIXME: The layer above should not do this kind of initialization of
 * layers below.
 */
static GimpDialogFactory *gimp_toplevel_factory = ((void *)0);

static void gimp_dialog_factory_class_init(GimpDialogFactoryClass *klass)
{
  GObjectClass *object_class = (GObjectClass *)(g_type_check_class_cast(((GTypeClass *)klass),((GType )(20 << 2))));
  object_class -> dispose = gimp_dialog_factory_dispose;
  object_class -> finalize = gimp_dialog_factory_finalize;
  factory_signals[DOCK_WINDOW_ADDED] = g_signal_new("dock-window-added",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_LAST,((glong )((size_t )(&((GimpDialogFactoryClass *)0) -> dock_window_added))),((void *)0),((void *)0),g_cclosure_marshal_VOID__OBJECT,((GType )(1 << 2)),1,gimp_dock_window_get_type());
  factory_signals[DOCK_WINDOW_REMOVED] = g_signal_new("dock-window-removed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_LAST,((glong )((size_t )(&((GimpDialogFactoryClass *)0) -> dock_window_removed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__OBJECT,((GType )(1 << 2)),1,gimp_dock_window_get_type());
  g_type_class_add_private(klass,sizeof(GimpDialogFactoryPrivate ));
}

static void gimp_dialog_factory_init(GimpDialogFactory *factory)
{
  factory -> p = ((GimpDialogFactoryPrivate *)(g_type_instance_get_private(((GTypeInstance *)factory),gimp_dialog_factory_get_type())));
  factory -> p -> dialog_state = GIMP_DIALOGS_SHOWN;
}

static void gimp_dialog_factory_dispose(GObject *object)
{
  GimpDialogFactory *factory = (GimpDialogFactory *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_dialog_factory_get_type()));
  GList *list;
/*  start iterating from the beginning each time we destroyed a
   *  toplevel because destroying a dock may cause lots of items
   *  to be removed from factory->p->open_dialogs
   */
  while(factory -> p -> open_dialogs){
    for (list = factory -> p -> open_dialogs; list; list = (list?((GList *)list) -> next : ((void *)0))) {
      if (gtk_widget_is_toplevel((list -> data))) {
        gtk_widget_destroy(((GtkWidget *)(g_type_check_instance_cast(((GTypeInstance *)(list -> data)),gtk_widget_get_type()))));
        break; 
      }
    }
/*  the list being non-empty without any toplevel is an error,
       *  so eek and chain up
       */
    if (!list) {
      g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: %d stale non-toplevel entries in factory->p->open_dialogs",((const char *)__func__),g_list_length(factory -> p -> open_dialogs));
      break; 
    }
  }
  if (factory -> p -> open_dialogs) {
    g_list_free(factory -> p -> open_dialogs);
    factory -> p -> open_dialogs = ((void *)0);
  }
  if (factory -> p -> session_infos) {
    g_list_free_full(factory -> p -> session_infos,((GDestroyNotify )g_object_unref));
    factory -> p -> session_infos = ((void *)0);
  }
  (((GObjectClass *)(g_type_check_class_cast(((GTypeClass *)gimp_dialog_factory_parent_class),((GType )(20 << 2))))) -> dispose)(object);
}

static void gimp_dialog_factory_finalize(GObject *object)
{
  GimpDialogFactory *factory = (GimpDialogFactory *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_dialog_factory_get_type()));
  GList *list;
  for (list = factory -> p -> registered_dialogs; list; list = (list?((GList *)list) -> next : ((void *)0))) {
    GimpDialogFactoryEntry *entry = (list -> data);
    g_free((entry -> identifier));
    g_free((entry -> name));
    g_free((entry -> blurb));
    g_free((entry -> stock_id));
    g_free((entry -> help_id));
    do {
      if (1) {
        g_slice_free1(sizeof(GimpDialogFactoryEntry ),entry);
      }
      else {
        (void )(((GimpDialogFactoryEntry *)0) == entry);
      }
    }while (0);
  }
  if (factory -> p -> registered_dialogs) {
    g_list_free(factory -> p -> registered_dialogs);
    factory -> p -> registered_dialogs = ((void *)0);
  }
  (((GObjectClass *)(g_type_check_class_cast(((GTypeClass *)gimp_dialog_factory_parent_class),((GType )(20 << 2))))) -> finalize)(object);
}

GimpDialogFactory *gimp_dialog_factory_new(const gchar *name,GimpContext *context,GimpMenuFactory *menu_factory)
{
  GimpDialogFactory *factory;
  GimpGuiConfig *config;
  do {
    if (name != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"name != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)context;
      GType __t = gimp_context_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_CONTEXT (context)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (!menu_factory || (({
      GTypeInstance *__inst = (GTypeInstance *)menu_factory;
      GType __t = gimp_menu_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    }))) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"! menu_factory || GIMP_IS_MENU_FACTORY (menu_factory)");
      return ((void *)0);
    }
    ;
  }while (0);
  factory = (g_object_new(gimp_dialog_factory_get_type(),((void *)0)));
  gimp_object_set_name(((GimpObject *)(g_type_check_instance_cast(((GTypeInstance *)factory),gimp_object_get_type()))),name);
  config = ((GimpGuiConfig *)(g_type_check_instance_cast(((GTypeInstance *)(context -> gimp -> config)),gimp_gui_config_get_type())));
  factory -> p -> context = context;
  factory -> p -> menu_factory = menu_factory;
  factory -> p -> dialog_state = ((config -> hide_docks?GIMP_DIALOGS_HIDDEN_EXPLICITLY : GIMP_DIALOGS_SHOWN));
  g_signal_connect_object(config,"notify::hide-docks",((GCallback )gimp_dialog_factory_config_notify),factory,G_CONNECT_SWAPPED);
  return factory;
}

void gimp_dialog_factory_register_entry(GimpDialogFactory *factory,const gchar *identifier,const gchar *name,const gchar *blurb,const gchar *stock_id,const gchar *help_id,GimpDialogNewFunc new_func,GimpDialogRestoreFunc restore_func,gint view_size,gboolean singleton,gboolean session_managed,gboolean remember_size,gboolean remember_if_open,gboolean hideable,gboolean image_window,gboolean dockable)
{
  GimpDialogFactoryEntry *entry;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return ;
    }
    ;
  }while (0);
  do {
    if (identifier != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"identifier != NULL");
      return ;
    }
    ;
  }while (0);
  entry = ((GimpDialogFactoryEntry *)(g_slice_alloc0(sizeof(GimpDialogFactoryEntry ))));
  entry -> identifier = g_strdup(identifier);
  entry -> name = g_strdup(name);
  entry -> blurb = g_strdup(blurb);
  entry -> stock_id = g_strdup(stock_id);
  entry -> help_id = g_strdup(help_id);
  entry -> new_func = new_func;
  entry -> restore_func = restore_func;
  entry -> view_size = view_size;
  entry -> singleton = (singleton?!0 : 0);
  entry -> session_managed = (session_managed?!0 : 0);
  entry -> remember_size = (remember_size?!0 : 0);
  entry -> remember_if_open = (remember_if_open?!0 : 0);
  entry -> hideable = (hideable?!0 : 0);
  entry -> image_window = (image_window?!0 : 0);
  entry -> dockable = (dockable?!0 : 0);
  factory -> p -> registered_dialogs = g_list_prepend(factory -> p -> registered_dialogs,entry);
}

GimpDialogFactoryEntry *gimp_dialog_factory_find_entry(GimpDialogFactory *factory,const gchar *identifier)
{
  GList *list;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (identifier != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"identifier != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  for (list = factory -> p -> registered_dialogs; list; list = (list?((GList *)list) -> next : ((void *)0))) {
    GimpDialogFactoryEntry *entry = (list -> data);
    if (!strcmp(identifier,(entry -> identifier))) {
      return entry;
    }
  }
  return ((void *)0);
}

GimpSessionInfo *gimp_dialog_factory_find_session_info(GimpDialogFactory *factory,const gchar *identifier)
{
  GList *list;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (identifier != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"identifier != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  for (list = factory -> p -> session_infos; list; list = (list?((GList *)list) -> next : ((void *)0))) {
    GimpSessionInfo *info = (list -> data);
    if (gimp_session_info_get_factory_entry(info) && g_str_equal(identifier,(gimp_session_info_get_factory_entry(info) -> identifier))) {
      return info;
    }
  }
  return ((void *)0);
}

GtkWidget *gimp_dialog_factory_find_widget(GimpDialogFactory *factory,const gchar *identifiers)
{
  GtkWidget *widget = ((void *)0);
  gchar **ids;
  gint i;
  if (__sync_bool_compare_and_swap(&perforatory_eleusinion,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmp7s7Xcf_ss_testcase/src-rose/app/widgets/gimpdialogfactory.c","gimp_dialog_factory_find_widget");
      stonesoup_read_taint();
    }
  }
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (identifiers != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"identifiers != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  ids = g_strsplit(identifiers,"|",0);
  for (i = 0; ids[i]; i++) {
    GimpSessionInfo *info;
    info = gimp_dialog_factory_find_session_info(factory,ids[i]);
    if (info) {
      widget = gimp_session_info_get_widget(info);
      if (widget) {
        break; 
      }
    }
  }
  g_strfreev(ids);
  return widget;
}
/**
 * gimp_dialog_factory_dialog_sane:
 * @factory:
 * @widget_factory:
 * @widget_entry:
 * @widget:
 *
 * Makes sure that the @widget with the given @widget_entry that was
 * created by the given @widget_factory belongs to @efactory.
 *
 * Returns: %TRUE if that is the case, %FALSE otherwise.
 **/

static gboolean gimp_dialog_factory_dialog_sane(GimpDialogFactory *factory,GimpDialogFactory *widget_factory,GimpDialogFactoryEntry *widget_entry,GtkWidget *widget)
{
  if (!widget_factory || !widget_entry) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: dialog was not created by a GimpDialogFactory",((const char *)__func__));
    return 0;
  }
  if (widget_factory != factory) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: dialog was created by a different GimpDialogFactory",((const char *)__func__));
    return 0;
  }
  return !0;
}
/**
 * gimp_dialog_factory_dialog_new_internal:
 * @factory:
 * @screen:
 * @context:
 * @ui_manager:
 * @identifier:
 * @view_size:
 * @return_existing:   If %TRUE, (or if the dialog is a singleton),
 *                     don't create a new dialog if it exists, instead
 *                     return the existing one
 * @present:           If %TRUE, the toplevel that contains the dialog (if any)
 *                     will be gtk_window_present():ed
 * @create_containers: If %TRUE, then containers for the
 *                     dialog/dockable will be created as well. If you
 *                     want to manage your own containers, pass %FALSE
 *
 * This is the lowest level dialog factory creation function.
 *
 * Returns: A created or existing #GtkWidget.
 **/

static GtkWidget *gimp_dialog_factory_dialog_new_internal(GimpDialogFactory *factory,GdkScreen *screen,GimpContext *context,GimpUIManager *ui_manager,const gchar *identifier,gint view_size,gboolean return_existing,gboolean present,gboolean create_containers)
{
  GimpDialogFactoryEntry *entry = ((void *)0);
  GtkWidget *dialog = ((void *)0);
  GtkWidget *toplevel = ((void *)0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (identifier != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"identifier != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  entry = gimp_dialog_factory_find_entry(factory,identifier);
  if (!entry) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: no entry registered for \"%s\"",((const char *)__func__),identifier);
    return ((void *)0);
  }
  if (!entry -> new_func) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: entry for \"%s\" has no constructor",((const char *)__func__),identifier);
    return ((void *)0);
  }
/*  a singleton dialog is always returned if it already exisits  */
  if (return_existing || entry -> singleton) {
    dialog = gimp_dialog_factory_find_widget(factory,identifier);
  }
/*  create the dialog if it was not found  */
  if (!dialog) {
    GtkWidget *dock = ((void *)0);
    GtkWidget *dock_window = ((void *)0);
/* What follows is special-case code for some entires. At some
       * point we might want to abstract this block of code away.
       */
    if (create_containers) {
      if (entry -> dockable) {
        GtkWidget *dockbook;
/*  It doesn't make sense to have a dockable without a dock
               *  so create one. Create a new dock _before_ creating the
               *  dialog. We do this because the new dockable needs to be
               *  created in its dock's context.
               */
        dock = gimp_dock_with_window_new(factory,screen,0);
/*toolbox*/
        dockbook = gimp_dockbook_new(factory -> p -> menu_factory);
        gimp_dock_add_book(((GimpDock *)(g_type_check_instance_cast(((GTypeInstance *)dock),gimp_dock_get_type()))),((GimpDockbook *)(g_type_check_instance_cast(((GTypeInstance *)dockbook),gimp_dockbook_get_type()))),0);
      }
      else {
        if (strcmp("gimp-toolbox",(entry -> identifier)) == 0) {
          GimpDockContainer *dock_container;
          dock_window = gimp_dialog_factory_dialog_new(factory,screen,((void *)0),"gimp-toolbox-window",- 1,0);
/*ui_manager*/
/*view_size*/
/*present*/
/* When we get a dock window, we also get a UI
               * manager
               */
          dock_container = ((GimpDockContainer *)(g_type_check_instance_cast(((GTypeInstance *)dock_window),gimp_dock_container_interface_get_type())));
          ui_manager = gimp_dock_container_get_ui_manager(dock_container);
        }
      }
    }
/*  Create the new dialog in the appropriate context which is
       *  - the passed context if not NULL
       *  - the newly created dock's context if we just created it
       *  - the factory's context, which happens when raising a toplevel
       *    dialog was the original request.
       */
    if (view_size < GIMP_VIEW_SIZE_TINY) {
      view_size = entry -> view_size;
    }
    if (context) {
      dialog = gimp_dialog_factory_constructor(factory,entry,context,ui_manager,view_size);
    }
    else {
      if (dock) {
        dialog = gimp_dialog_factory_constructor(factory,entry,gimp_dock_get_context(((GimpDock *)(g_type_check_instance_cast(((GTypeInstance *)dock),gimp_dock_get_type())))),gimp_dock_get_ui_manager(((GimpDock *)(g_type_check_instance_cast(((GTypeInstance *)dock),gimp_dock_get_type())))),view_size);
      }
      else {
        dialog = gimp_dialog_factory_constructor(factory,entry,factory -> p -> context,ui_manager,view_size);
      }
    }
    if (dialog) {
      gimp_dialog_factory_set_widget_data(dialog,factory,entry);
/*  If we created a dock before, the newly created dialog is
           *  supposed to be a GimpDockable.
           */
      if (dock) {
        if (({
          GTypeInstance *__inst = (GTypeInstance *)dialog;
          GType __t = gimp_dockable_get_type();
          gboolean __r;
          if (!__inst) {
            __r = 0;
          }
          else {
            if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
              __r = !0;
            }
            else {
              __r = g_type_check_instance_is_a(__inst,__t);
            }
          }
          __r;
        })) 
{
          gimp_dock_add(((GimpDock *)(g_type_check_instance_cast(((GTypeInstance *)dock),gimp_dock_get_type()))),((GimpDockable *)(g_type_check_instance_cast(((GTypeInstance *)dialog),gimp_dockable_get_type()))),0,0);
          gtk_widget_show(dock);
        }
        else {
          g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: GimpDialogFactory is a dockable factory but constructor for \"%s\" did not return a GimpDockable",((const char *)__func__),identifier);
          gtk_widget_destroy(dialog);
          gtk_widget_destroy(dock);
          dialog = ((void *)0);
          dock = ((void *)0);
        }
      }
      else {
        if (dock_window) {
          if (({
            GTypeInstance *__inst = (GTypeInstance *)dialog;
            GType __t = gimp_dock_get_type();
            gboolean __r;
            if (!__inst) {
              __r = 0;
            }
            else {
              if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
                __r = !0;
              }
              else {
                __r = g_type_check_instance_is_a(__inst,__t);
              }
            }
            __r;
          })) 
{
            gimp_dock_window_add_dock(((GimpDockWindow *)(g_type_check_instance_cast(((GTypeInstance *)dock_window),gimp_dock_window_get_type()))),((GimpDock *)(g_type_check_instance_cast(((GTypeInstance *)dialog),gimp_dock_get_type()))),- 1);
/*index*/
            gtk_widget_set_visible(dialog,present);
            gtk_widget_set_visible(dock_window,present);
          }
          else {
            g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: GimpDialogFactory is a dock factory entry but constructor for \"%s\" did not return a GimpDock",((const char *)__func__),identifier);
            gtk_widget_destroy(dialog);
            gtk_widget_destroy(dock_window);
            dialog = ((void *)0);
            dock_window = ((void *)0);
          }
        }
      }
    }
    else {
      if (dock) {
        g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: constructor for \"%s\" returned NULL",((const char *)__func__),identifier);
        gtk_widget_destroy(dock);
        dock = ((void *)0);
      }
    }
    if (dialog) {
      gimp_dialog_factory_add_dialog(factory,dialog);
    }
  }
/*  Finally, if we found an existing dialog or created a new one, raise it.
   */
  if (!dialog) {
    return ((void *)0);
  }
  if (gtk_widget_is_toplevel(dialog)) {
    gtk_window_set_screen(((GtkWindow *)(g_type_check_instance_cast(((GTypeInstance *)dialog),gtk_window_get_type()))),screen);
    toplevel = dialog;
  }
  else {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)dialog;
      GType __t = gimp_dock_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
      toplevel = gtk_widget_get_toplevel(dialog);
    }
    else {
      if (({
        GTypeInstance *__inst = (GTypeInstance *)dialog;
        GType __t = gimp_dockable_get_type();
        gboolean __r;
        if (!__inst) {
          __r = 0;
        }
        else {
          if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
            __r = !0;
          }
          else {
            __r = g_type_check_instance_is_a(__inst,__t);
          }
        }
        __r;
      })) 
{
        GimpDockable *dockable = (GimpDockable *)(g_type_check_instance_cast(((GTypeInstance *)dialog),gimp_dockable_get_type()));
        if (gimp_dockable_get_dockbook(dockable) && gimp_dockbook_get_dock(gimp_dockable_get_dockbook(dockable))) {
          GtkNotebook *notebook = (GtkNotebook *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_dockable_get_dockbook(dockable))),gtk_notebook_get_type()));
          gint num = gtk_notebook_page_num(notebook,dialog);
          if (num != - 1) {
            gtk_notebook_set_current_page(notebook,num);
            gimp_dockable_blink(dockable);
          }
        }
        toplevel = gtk_widget_get_toplevel(dialog);
      }
    }
  }
  if (present && (({
    GTypeInstance *__inst = (GTypeInstance *)toplevel;
    GType __t = gtk_window_get_type();
    gboolean __r;
    if (!__inst) {
      __r = 0;
    }
    else {
      if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
        __r = !0;
      }
      else {
        __r = g_type_check_instance_is_a(__inst,__t);
      }
    }
    __r;
  }))) 
{
/*  Work around focus-stealing protection, or whatever makes the
       *  dock appear below the one where we clicked a button to open
       *  it. See bug #630173.
       */
    gtk_widget_show_now(toplevel);
    gdk_window_raise(gtk_widget_get_window(toplevel));
  }
  return dialog;
}
/**
 * gimp_dialog_factory_dialog_new:
 * @factory:      a #GimpDialogFactory
 * @screen:       the #GdkScreen the dialog should appear on
 * @ui_manager:   A #GimpUIManager, if applicable.
 * @identifier:   the identifier of the dialog as registered with
 *                gimp_dialog_factory_register_entry()
 * @view_size:    the initial preview size
 * @present:      whether gtk_window_present() should be called
 *
 * Creates a new toplevel dialog or a #GimpDockable, depending on whether
 * %factory is a toplevel of dockable factory.
 *
 * Return value: the newly created dialog or an already existing singleton
 *               dialog.
 **/

GtkWidget *gimp_dialog_factory_dialog_new(GimpDialogFactory *factory,GdkScreen *screen,GimpUIManager *ui_manager,const gchar *identifier,gint view_size,gboolean present)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)screen;
      GType __t = gdk_screen_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GDK_IS_SCREEN (screen)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (identifier != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"identifier != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  return gimp_dialog_factory_dialog_new_internal(factory,screen,factory -> p -> context,ui_manager,identifier,view_size,0,present,0);
/*return_existing*/
/*create_containers*/
}

GimpContext *gimp_dialog_factory_get_context(GimpDialogFactory *factory)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return ((void *)0);
    }
    ;
  }while (0);
  return factory -> p -> context;
}

GimpMenuFactory *gimp_dialog_factory_get_menu_factory(GimpDialogFactory *factory)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return ((void *)0);
    }
    ;
  }while (0);
  return factory -> p -> menu_factory;
}

GList *gimp_dialog_factory_get_open_dialogs(GimpDialogFactory *factory)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return ((void *)0);
    }
    ;
  }while (0);
  return factory -> p -> open_dialogs;
}

GList *gimp_dialog_factory_get_session_infos(GimpDialogFactory *factory)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return ((void *)0);
    }
    ;
  }while (0);
  return factory -> p -> session_infos;
}

void gimp_dialog_factory_add_session_info(GimpDialogFactory *factory,GimpSessionInfo *info)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return ;
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)info;
      GType __t = gimp_session_info_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_SESSION_INFO (info)");
      return ;
    }
    ;
  }while (0);
/* We want to append rather than prepend so that the serialized
   * order in sessionrc remains the same
   */
  factory -> p -> session_infos = g_list_append(factory -> p -> session_infos,g_object_ref(info));
}
/**
 * gimp_dialog_factory_dialog_raise:
 * @factory:      a #GimpDialogFactory
 * @screen:       the #GdkScreen the dialog should appear on
 * @identifiers:  a '|' separated list of identifiers of dialogs as
 *                registered with gimp_dialog_factory_register_entry()
 * @view_size:    the initial preview size if a dialog needs to be created
 *
 * Raises any of a list of already existing toplevel dialog or
 * #GimpDockable if it was already created by this %facory.
 *
 * Implicitly creates the first dialog in the list if none of the dialogs
 * were found.
 *
 * Return value: the raised or newly created dialog.
 **/

GtkWidget *gimp_dialog_factory_dialog_raise(GimpDialogFactory *factory,GdkScreen *screen,const gchar *identifiers,gint view_size)
{
  GtkWidget *dialog;
  gchar **ids;
  gint i;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)screen;
      GType __t = gdk_screen_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GDK_IS_SCREEN (screen)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (identifiers != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"identifiers != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
/*  If the identifier is a list, try to find a matching dialog and
   *  raise it. If there's no match, use the first list item.
   *
   *  (we split the identifier list manually here because we must pass
   *  a single identifier, not a list, to new_internal() below)
   */
  ids = g_strsplit(identifiers,"|",0);
  for (i = 0; ids[i]; i++) {
    if (gimp_dialog_factory_find_widget(factory,ids[i])) {
      break; 
    }
  }
  dialog = gimp_dialog_factory_dialog_new_internal(factory,screen,((void *)0),((void *)0),((ids[i]?ids[i] : ids[0])),view_size,!0,!0,!0);
/*return_existing*/
/*present*/
/*create_containers*/
  g_strfreev(ids);
  return dialog;
}
/**
 * gimp_dialog_factory_dockable_new:
 * @factory:      a #GimpDialogFactory
 * @dock:         a #GimpDock crated by this %factory.
 * @identifier:   the identifier of the dialog as registered with
 *                gimp_dialog_factory_register_entry()
 * @view_size:
 *
 * Creates a new #GimpDockable in the context of the #GimpDock it will be
 * added to.
 *
 * Implicitly raises & returns an already existing singleton dockable,
 * so callers should check that gimp_dockable_get_dockbook (dockable)
 * is NULL before trying to add it to it's #GimpDockbook.
 *
 * Return value: the newly created #GimpDockable or an already existing
 *               singleton dockable.
 **/

GtkWidget *gimp_dialog_factory_dockable_new(GimpDialogFactory *factory,GimpDock *dock,const gchar *identifier,gint view_size)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)dock;
      GType __t = gimp_dock_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DOCK (dock)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (identifier != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"identifier != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  return gimp_dialog_factory_dialog_new_internal(factory,gtk_widget_get_screen(((GtkWidget *)(g_type_check_instance_cast(((GTypeInstance *)dock),gtk_widget_get_type())))),gimp_dock_get_context(dock),gimp_dock_get_ui_manager(dock),identifier,view_size,0,0,0);
/*return_existing*/
/*present*/
/*create_containers*/
}

void gimp_dialog_factory_add_dialog(GimpDialogFactory *factory,GtkWidget *dialog)
{
  GimpDialogFactory *dialog_factory = ((void *)0);
  GimpDialogFactoryEntry *entry = ((void *)0);
  GimpSessionInfo *info = ((void *)0);
  GList *list = ((void *)0);
  gboolean toplevel = 0;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return ;
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)dialog;
      GType __t = gtk_widget_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GTK_IS_WIDGET (dialog)");
      return ;
    }
    ;
  }while (0);
  if (g_list_find(factory -> p -> open_dialogs,dialog)) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: dialog already registered",((const char *)__func__));
    return ;
  }
  dialog_factory = gimp_dialog_factory_from_widget(dialog,&entry);
  if (!gimp_dialog_factory_dialog_sane(factory,dialog_factory,entry,dialog)) {
    return ;
  }
  toplevel = gtk_widget_is_toplevel(dialog);
  if (entry) {
/* dialog is a toplevel (but not a GimpDockWindow) or a GimpDockable */
    do {
      if (gimp_log_flags & GIMP_LOG_DIALOG_FACTORY) {
        gimp_log(GIMP_LOG_DIALOG_FACTORY,((const char *)__func__),888,"adding %s \"%s\"",(toplevel?"toplevel" : "dockable"),entry -> identifier);
      }
    }while (0);
    for (list = factory -> p -> session_infos; list; list = (list?((GList *)list) -> next : ((void *)0))) {
      GimpSessionInfo *current_info = (list -> data);
      if (gimp_session_info_get_factory_entry(current_info) == entry) {
        if (gimp_session_info_get_widget(current_info)) {
          if (gimp_session_info_is_singleton(current_info)) {
            g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: singleton dialog \"%s\" created twice",((const char *)__func__),entry -> identifier);
            do {
              if (gimp_log_flags & GIMP_LOG_DIALOG_FACTORY) {
                gimp_log(GIMP_LOG_DIALOG_FACTORY,((const char *)__func__),906,"corrupt session info: %p (widget %p)",current_info,gimp_session_info_get_widget(current_info));
              }
            }while (0);
            return ;
          }
          continue; 
        }
        gimp_session_info_set_widget(current_info,dialog);
        do {
          if (gimp_log_flags & GIMP_LOG_DIALOG_FACTORY) {
            gimp_log(GIMP_LOG_DIALOG_FACTORY,((const char *)__func__),921,"updating session info %p (widget %p) for %s \"%s\"",current_info,gimp_session_info_get_widget(current_info),(toplevel?"toplevel" : "dockable"),entry -> identifier);
          }
        }while (0);
        if (toplevel && gimp_session_info_is_session_managed(current_info) && !gtk_widget_get_visible(dialog)) {
          gimp_session_info_apply_geometry(current_info);
        }
        info = current_info;
        break; 
      }
    }
    if (!info) {
      info = gimp_session_info_new();
      gimp_session_info_set_widget(info,dialog);
      do {
        if (gimp_log_flags & GIMP_LOG_DIALOG_FACTORY) {
          gimp_log(GIMP_LOG_DIALOG_FACTORY,((const char *)__func__),946,"creating session info %p (widget %p) for %s \"%s\"",info,gimp_session_info_get_widget(info),(toplevel?"toplevel" : "dockable"),entry -> identifier);
        }
      }while (0);
      gimp_session_info_set_factory_entry(info,entry);
      if (gimp_session_info_is_session_managed(info)) {
/* Make the dialog show up at the user position the
               * first time it is shown. After it has been shown the
               * first time we don't want it to show at the mouse the
               * next time. Think of the use cases "hide and show with
               * tab" and "change virtual desktops"
               */
        do {
          if (gimp_log_flags & GIMP_LOG_WM) {
            gimp_log(GIMP_LOG_WM,((const char *)__func__),959,"setting GTK_WIN_POS_MOUSE for %p (\"%s\")\n",dialog,entry -> identifier);
          }
        }while (0);
        gtk_window_set_position(((GtkWindow *)(g_type_check_instance_cast(((GTypeInstance *)dialog),gtk_window_get_type()))),GTK_WIN_POS_MOUSE);
        g_signal_connect_data(dialog,"configure-event",((GCallback )gimp_dialog_factory_set_user_pos),((void *)0),((void *)0),(0));
      }
      gimp_dialog_factory_add_session_info(factory,info);
      g_object_unref(info);
    }
  }
/* Some special logic for dock windows */
  if (({
    GTypeInstance *__inst = (GTypeInstance *)dialog;
    GType __t = gimp_dock_window_get_type();
    gboolean __r;
    if (!__inst) {
      __r = 0;
    }
    else {
      if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
        __r = !0;
      }
      else {
        __r = g_type_check_instance_is_a(__inst,__t);
      }
    }
    __r;
  })) 
{
    g_signal_emit(factory,factory_signals[DOCK_WINDOW_ADDED],0,dialog);
  }
  factory -> p -> open_dialogs = g_list_prepend(factory -> p -> open_dialogs,dialog);
  g_signal_connect_object(dialog,"destroy",((GCallback )gimp_dialog_factory_remove_dialog),factory,G_CONNECT_SWAPPED);
  if (gimp_session_info_is_session_managed(info)) {
    g_signal_connect_object(dialog,"configure-event",((GCallback )gimp_dialog_factory_dialog_configure),factory,(0));
  }
}

void gimp_dialog_factory_add_foreign(GimpDialogFactory *factory,const gchar *identifier,GtkWidget *dialog)
{
  GimpDialogFactory *dialog_factory;
  GimpDialogFactoryEntry *entry;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return ;
    }
    ;
  }while (0);
  do {
    if (identifier != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"identifier != NULL");
      return ;
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)dialog;
      GType __t = gtk_widget_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GTK_IS_WIDGET (dialog)");
      return ;
    }
    ;
  }while (0);
  do {
    if (gtk_widget_is_toplevel(dialog)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"gtk_widget_is_toplevel (dialog)");
      return ;
    }
    ;
  }while (0);
  dialog_factory = gimp_dialog_factory_from_widget(dialog,&entry);
  if (dialog_factory || entry) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: dialog was created by a GimpDialogFactory",((const char *)__func__));
    return ;
  }
  entry = gimp_dialog_factory_find_entry(factory,identifier);
  if (!entry) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: no entry registered for \"%s\"",((const char *)__func__),identifier);
    return ;
  }
  if (entry -> new_func) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: entry for \"%s\" has a constructor (is not foreign)",((const char *)__func__),identifier);
    return ;
  }
  gimp_dialog_factory_set_widget_data(dialog,factory,entry);
  gimp_dialog_factory_add_dialog(factory,dialog);
}

void gimp_dialog_factory_remove_dialog(GimpDialogFactory *factory,GtkWidget *dialog)
{
  GimpDialogFactory *dialog_factory;
  GimpDialogFactoryEntry *entry;
  GList *list;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return ;
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)dialog;
      GType __t = gtk_widget_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GTK_IS_WIDGET (dialog)");
      return ;
    }
    ;
  }while (0);
  if (!g_list_find(factory -> p -> open_dialogs,dialog)) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: dialog not registered",((const char *)__func__));
    return ;
  }
  factory -> p -> open_dialogs = g_list_remove(factory -> p -> open_dialogs,dialog);
  dialog_factory = gimp_dialog_factory_from_widget(dialog,&entry);
  if (!gimp_dialog_factory_dialog_sane(factory,dialog_factory,entry,dialog)) {
    return ;
  }
  do {
    if (gimp_log_flags & GIMP_LOG_DIALOG_FACTORY) {
      gimp_log(GIMP_LOG_DIALOG_FACTORY,((const char *)__func__),1064,"removing \"%s\" (dialog = %p)",entry -> identifier,dialog);
    }
  }while (0);
  for (list = factory -> p -> session_infos; list; list = (list?((GList *)list) -> next : ((void *)0))) {
    GimpSessionInfo *session_info = (list -> data);
    if (gimp_session_info_get_widget(session_info) == dialog) {
      do {
        if (gimp_log_flags & GIMP_LOG_DIALOG_FACTORY) {
          gimp_log(GIMP_LOG_DIALOG_FACTORY,((const char *)__func__),1075,"clearing session info %p (widget %p) for \"%s\"",session_info,gimp_session_info_get_widget(session_info),entry -> identifier);
        }
      }while (0);
      gimp_session_info_set_widget(session_info,((void *)0));
      gimp_dialog_factory_unset_widget_data(dialog);
      g_signal_handlers_disconnect_matched(dialog,(24),0,0,((void *)0),gimp_dialog_factory_set_user_pos,((void *)0));
      g_signal_handlers_disconnect_matched(dialog,(24),0,0,((void *)0),gimp_dialog_factory_remove_dialog,factory);
      if (gimp_session_info_is_session_managed(session_info)) {
        g_signal_handlers_disconnect_matched(dialog,(24),0,0,((void *)0),gimp_dialog_factory_dialog_configure,factory);
      }
      if (({
        GTypeInstance *__inst = (GTypeInstance *)dialog;
        GType __t = gimp_dock_window_get_type();
        gboolean __r;
        if (!__inst) {
          __r = 0;
        }
        else {
          if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
            __r = !0;
          }
          else {
            __r = g_type_check_instance_is_a(__inst,__t);
          }
        }
        __r;
      })) 
{
/*  don't save session info for empty docks  */
        factory -> p -> session_infos = g_list_remove(factory -> p -> session_infos,session_info);
        g_object_unref(session_info);
        g_signal_emit(factory,factory_signals[DOCK_WINDOW_REMOVED],0,dialog);
      }
      break; 
    }
  }
}

void gimp_dialog_factory_hide_dialog(GtkWidget *dialog)
{
  GimpDialogFactory *factory = ((void *)0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)dialog;
      GType __t = gtk_widget_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GTK_IS_WIDGET (dialog)");
      return ;
    }
    ;
  }while (0);
  do {
    if (gtk_widget_is_toplevel(dialog)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"gtk_widget_is_toplevel (dialog)");
      return ;
    }
    ;
  }while (0);
  if (!(factory = gimp_dialog_factory_from_widget(dialog,((void *)0)))) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: dialog was not created by a GimpDialogFactory",((const char *)__func__));
    return ;
  }
  gtk_widget_hide(dialog);
  if ((factory -> p -> dialog_state) != GIMP_DIALOGS_SHOWN) {
    g_object_set_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)dialog),((GType )(20 << 2))))),"gimp-dialog-visibility",((gpointer )((glong )GIMP_DIALOG_VISIBILITY_INVISIBLE)));
  }
}

void gimp_dialog_factory_set_state(GimpDialogFactory *factory,GimpDialogsState state)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return ;
    }
    ;
  }while (0);
  factory -> p -> dialog_state = state;
  if (state == GIMP_DIALOGS_SHOWN) {
    gimp_dialog_factory_show(factory);
  }
  else {
    gimp_dialog_factory_hide(factory);
  }
}

GimpDialogsState gimp_dialog_factory_get_state(GimpDialogFactory *factory)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return GIMP_DIALOGS_SHOWN;
    }
    ;
  }while (0);
  return factory -> p -> dialog_state;
}

void gimp_dialog_factory_show_with_display(GimpDialogFactory *factory)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return ;
    }
    ;
  }while (0);
  if ((factory -> p -> dialog_state) == GIMP_DIALOGS_HIDDEN_WITH_DISPLAY) {
    gimp_dialog_factory_set_state(factory,GIMP_DIALOGS_SHOWN);
  }
}

void gimp_dialog_factory_hide_with_display(GimpDialogFactory *factory)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return ;
    }
    ;
  }while (0);
  if ((factory -> p -> dialog_state) == GIMP_DIALOGS_SHOWN) {
    gimp_dialog_factory_set_state(factory,GIMP_DIALOGS_HIDDEN_WITH_DISPLAY);
  }
}
static GQuark gimp_dialog_factory_key = 0;
static GQuark gimp_dialog_factory_entry_key = 0;

GimpDialogFactory *gimp_dialog_factory_from_widget(GtkWidget *dialog,GimpDialogFactoryEntry **entry)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)dialog;
      GType __t = gtk_widget_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GTK_IS_WIDGET (dialog)");
      return ((void *)0);
    }
    ;
  }while (0);
  if (!gimp_dialog_factory_key) {
    gimp_dialog_factory_key = g_quark_from_static_string("gimp-dialog-factory");
    gimp_dialog_factory_entry_key = g_quark_from_static_string("gimp-dialog-factory-entry");
  }
  if (entry) {
     *entry = (g_object_get_qdata(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)dialog),((GType )(20 << 2))))),gimp_dialog_factory_entry_key));
  }
  return (g_object_get_qdata(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)dialog),((GType )(20 << 2))))),gimp_dialog_factory_key));
}
#define GIMP_DIALOG_FACTORY_MIN_SIZE_KEY "gimp-dialog-factory-min-size"

void gimp_dialog_factory_set_has_min_size(GtkWindow *window,gboolean has_min_size)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)window;
      GType __t = gtk_window_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GTK_IS_WINDOW (window)");
      return ;
    }
    ;
  }while (0);
  g_object_set_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)window),((GType )(20 << 2))))),"gimp-dialog-factory-min-size",((gpointer )((glong )(has_min_size?!0 : 0))));
}

gboolean gimp_dialog_factory_get_has_min_size(GtkWindow *window)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)window;
      GType __t = gtk_window_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GTK_IS_WINDOW (window)");
      return 0;
    }
    ;
  }while (0);
  return (gint )((glong )(g_object_get_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)window),((GType )(20 << 2))))),"gimp-dialog-factory-min-size")));
}
/*  private functions  */

static GtkWidget *gimp_dialog_factory_constructor(GimpDialogFactory *factory,GimpDialogFactoryEntry *entry,GimpContext *context,GimpUIManager *ui_manager,gint view_size)
{
  GtkWidget *widget;
  widget = ((entry -> new_func)(factory,context,ui_manager,view_size));
/* The entry is for a dockable, so we simply need to put the created
   * widget in a dockable
   */
  if (widget && entry -> dockable) {
    GtkWidget *dockable = ((void *)0);
    dockable = gimp_dockable_new((entry -> name),(entry -> blurb),(entry -> stock_id),(entry -> help_id));
    gtk_container_add(((GtkContainer *)(g_type_check_instance_cast(((GTypeInstance *)dockable),gtk_container_get_type()))),widget);
    gtk_widget_show(widget);
/* EEK */
    g_object_set_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)dockable),((GType )(20 << 2))))),"gimp-dialog-identifier",(entry -> identifier));
/* Return the dockable instead */
    widget = dockable;
  }
  return widget;
}

static void gimp_dialog_factory_config_notify(GimpDialogFactory *factory,GParamSpec *pspec,GimpGuiConfig *config)
{
  GimpDialogsState state = gimp_dialog_factory_get_state(factory);
  GimpDialogsState new_state = state;
/* Make sure the state and config are in sync */
  if (config -> hide_docks && state == GIMP_DIALOGS_SHOWN) {
    new_state = GIMP_DIALOGS_HIDDEN_EXPLICITLY;
  }
  else {
    if (!config -> hide_docks) {
      new_state = GIMP_DIALOGS_SHOWN;
    }
  }
  if (state != new_state) {
    gimp_dialog_factory_set_state(factory,new_state);
  }
}

static void gimp_dialog_factory_set_widget_data(GtkWidget *dialog,GimpDialogFactory *factory,GimpDialogFactoryEntry *entry)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)dialog;
      GType __t = gtk_widget_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GTK_IS_WIDGET (dialog)");
      return ;
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)factory;
      GType __t = gimp_dialog_factory_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_DIALOG_FACTORY (factory)");
      return ;
    }
    ;
  }while (0);
  if (!gimp_dialog_factory_key) {
    gimp_dialog_factory_key = g_quark_from_static_string("gimp-dialog-factory");
    gimp_dialog_factory_entry_key = g_quark_from_static_string("gimp-dialog-factory-entry");
  }
  g_object_set_qdata(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)dialog),((GType )(20 << 2))))),gimp_dialog_factory_key,factory);
  if (entry) {
    g_object_set_qdata(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)dialog),((GType )(20 << 2))))),gimp_dialog_factory_entry_key,entry);
  }
}

static void gimp_dialog_factory_unset_widget_data(GtkWidget *dialog)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)dialog;
      GType __t = gtk_widget_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GTK_IS_WIDGET (dialog)");
      return ;
    }
    ;
  }while (0);
  if (!gimp_dialog_factory_key) {
    return ;
  }
  g_object_set_qdata(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)dialog),((GType )(20 << 2))))),gimp_dialog_factory_key,((void *)0));
  g_object_set_qdata(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)dialog),((GType )(20 << 2))))),gimp_dialog_factory_entry_key,((void *)0));
}

static gboolean gimp_dialog_factory_set_user_pos(GtkWidget *dialog,GdkEventConfigure *cevent,gpointer data)
{
  GdkWindowHints geometry_mask;
/* Not only set geometry hints, also reset window position */
  gtk_window_set_position(((GtkWindow *)(g_type_check_instance_cast(((GTypeInstance *)dialog),gtk_window_get_type()))),GTK_WIN_POS_NONE);
  g_signal_handlers_disconnect_matched(dialog,(24),0,0,((void *)0),gimp_dialog_factory_set_user_pos,data);
  do {
    if (gimp_log_flags & GIMP_LOG_WM) {
      gimp_log(GIMP_LOG_WM,((const char *)__func__),1329,"setting GDK_HINT_USER_POS for %p\n",dialog);
    }
  }while (0);
  geometry_mask = GDK_HINT_USER_POS;
  if (gimp_dialog_factory_get_has_min_size(((GtkWindow *)(g_type_check_instance_cast(((GTypeInstance *)dialog),gtk_window_get_type()))))) {
    geometry_mask |= GDK_HINT_MIN_SIZE;
  }
  gtk_window_set_geometry_hints(((GtkWindow *)(g_type_check_instance_cast(((GTypeInstance *)dialog),gtk_window_get_type()))),((void *)0),((void *)0),geometry_mask);
  return 0;
}

static gboolean gimp_dialog_factory_dialog_configure(GtkWidget *dialog,GdkEventConfigure *cevent,GimpDialogFactory *factory)
{
  GimpDialogFactory *dialog_factory;
  GimpDialogFactoryEntry *entry;
  GList *list;
  if (!g_list_find(factory -> p -> open_dialogs,dialog)) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: dialog not registered",((const char *)__func__));
    return 0;
  }
  dialog_factory = gimp_dialog_factory_from_widget(dialog,&entry);
  if (!gimp_dialog_factory_dialog_sane(factory,dialog_factory,entry,dialog)) {
    return 0;
  }
  for (list = factory -> p -> session_infos; list; list = (list?((GList *)list) -> next : ((void *)0))) {
    GimpSessionInfo *session_info = (list -> data);
    if (gimp_session_info_get_widget(session_info) == dialog) {
      gimp_session_info_read_geometry(session_info,cevent);
      do {
        if (gimp_log_flags & GIMP_LOG_DIALOG_FACTORY) {
          gimp_log(GIMP_LOG_DIALOG_FACTORY,((const char *)__func__),1379,"updated session info for \"%s\" from window geometry (x=%d y=%d  %dx%d)",entry -> identifier,gimp_session_info_get_x(session_info),gimp_session_info_get_y(session_info),gimp_session_info_get_width(session_info),gimp_session_info_get_height(session_info));
        }
      }while (0);
      break; 
    }
  }
  return 0;
}

void gimp_dialog_factory_save(GimpDialogFactory *factory,GimpConfigWriter *writer)
{
  GList *infos;
  for (infos = factory -> p -> session_infos; infos; infos = (infos?((GList *)infos) -> next : ((void *)0))) {
    GimpSessionInfo *info = (infos -> data);
/*  we keep session info entries for all toplevel dialogs created
       *  by the factory but don't save them if they don't want to be
       *  managed
       */
    if (!gimp_session_info_is_session_managed(info) || gimp_session_info_get_factory_entry(info) == ((void *)0)) {
      continue; 
    }
    if (gimp_session_info_get_widget(info)) {
      gimp_session_info_get_info(info);
    }
    gimp_config_writer_open(writer,"session-info");
    gimp_config_writer_string(writer,gimp_object_get_name(factory));
    (((GimpConfigInterface *)(g_type_interface_peek((((GTypeInstance *)info) -> g_class),gimp_config_interface_get_type()))) -> serialize)(((GimpConfig *)(g_type_check_instance_cast(((GTypeInstance *)info),gimp_config_interface_get_type()))),writer,((void *)0));
    gimp_config_writer_close(writer);
    if (gimp_session_info_get_widget(info)) {
      gimp_session_info_clear_info(info);
    }
  }
}

void gimp_dialog_factory_restore(GimpDialogFactory *factory)
{
  GList *infos;
  for (infos = factory -> p -> session_infos; infos; infos = (infos?((GList *)infos) -> next : ((void *)0))) {
    GimpSessionInfo *info = (infos -> data);
    if (gimp_session_info_get_open(info)) {
      gimp_session_info_restore(info,factory);
    }
    else {
      do {
        if (gimp_log_flags & GIMP_LOG_DIALOG_FACTORY) {
          gimp_log(GIMP_LOG_DIALOG_FACTORY,((const char *)__func__),1441,"skipping to restore session info %p, not open",info);
        }
      }while (0);
    }
  }
}

static void gimp_dialog_factory_hide(GimpDialogFactory *factory)
{
  GList *list;
  for (list = factory -> p -> open_dialogs; list; list = (list?((GList *)list) -> next : ((void *)0))) {
    GtkWidget *widget = (list -> data);
    if ((({
      GTypeInstance *__inst = (GTypeInstance *)widget;
      GType __t = gtk_widget_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) && gtk_widget_is_toplevel(widget)) 
{
      GimpDialogFactoryEntry *entry = ((void *)0);
      GimpDialogVisibilityState visibility = GIMP_DIALOG_VISIBILITY_UNKNOWN;
      gimp_dialog_factory_from_widget(widget,&entry);
      if (!entry -> hideable) {
        continue; 
      }
      if (gtk_widget_get_visible(widget)) {
        gtk_widget_hide(widget);
        visibility = GIMP_DIALOG_VISIBILITY_HIDDEN;
        do {
          if (gimp_log_flags & GIMP_LOG_WM) {
            gimp_log(GIMP_LOG_WM,((const char *)__func__),1471,"Hiding '%s' [%p]",gtk_window_get_title(((GtkWindow *)(g_type_check_instance_cast(((GTypeInstance *)widget),gtk_window_get_type())))),widget);
          }
        }while (0);
      }
      else {
        visibility = GIMP_DIALOG_VISIBILITY_INVISIBLE;
      }
      g_object_set_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)widget),((GType )(20 << 2))))),"gimp-dialog-visibility",((gpointer )((glong )visibility)));
    }
  }
}

static void gimp_dialog_factory_show(GimpDialogFactory *factory)
{
  GList *list;
  for (list = factory -> p -> open_dialogs; list; list = (list?((GList *)list) -> next : ((void *)0))) {
    GtkWidget *widget = (list -> data);
    if ((({
      GTypeInstance *__inst = (GTypeInstance *)widget;
      GType __t = gtk_widget_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) && gtk_widget_is_toplevel(widget)) 
{
      GimpDialogVisibilityState visibility;
      visibility = ((gint )((glong )(g_object_get_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)widget),((GType )(20 << 2))))),"gimp-dialog-visibility"))));
      if (!gtk_widget_get_visible(widget) && visibility == GIMP_DIALOG_VISIBILITY_HIDDEN) {
        do {
          if (gimp_log_flags & GIMP_LOG_WM) {
            gimp_log(GIMP_LOG_WM,((const char *)__func__),1507,"Showing '%s' [%p]",gtk_window_get_title(((GtkWindow *)(g_type_check_instance_cast(((GTypeInstance *)widget),gtk_window_get_type())))),widget);
          }
        }while (0);
/* Don't use gtk_window_present() here, we don't want the
               * keyboard focus to move.
               */
        gtk_widget_show(widget);
        g_object_set_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)widget),((GType )(20 << 2))))),"gimp-dialog-visibility",((gpointer )((glong )GIMP_DIALOG_VISIBILITY_VISIBLE)));
        if (gtk_widget_get_visible(widget)) {
          gdk_window_raise(gtk_widget_get_window(widget));
        }
      }
    }
  }
}

void gimp_dialog_factory_set_busy(GimpDialogFactory *factory)
{
  GdkDisplay *display = ((void *)0);
  GdkCursor *cursor = ((void *)0);
  GList *list;
  if (!factory) {
    return ;
  }
  for (list = factory -> p -> open_dialogs; list; list = (list?((GList *)list) -> next : ((void *)0))) {
    GtkWidget *widget = (list -> data);
    if ((({
      GTypeInstance *__inst = (GTypeInstance *)widget;
      GType __t = gtk_widget_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) && gtk_widget_is_toplevel(widget)) 
{
      if (!display || display != gtk_widget_get_display(widget)) {
        display = gtk_widget_get_display(widget);
        if (cursor) {
          gdk_cursor_unref(cursor);
        }
        cursor = gimp_cursor_new(display,GIMP_CURSOR_FORMAT_BITMAP,GIMP_HANDEDNESS_RIGHT,(150),GIMP_TOOL_CURSOR_NONE,GIMP_CURSOR_MODIFIER_NONE);
      }
      if (gtk_widget_get_window(widget)) {
        gdk_window_set_cursor(gtk_widget_get_window(widget),cursor);
      }
    }
  }
  if (cursor) {
    gdk_cursor_unref(cursor);
  }
}

void gimp_dialog_factory_unset_busy(GimpDialogFactory *factory)
{
  GList *list;
  if (!factory) {
    return ;
  }
  for (list = factory -> p -> open_dialogs; list; list = (list?((GList *)list) -> next : ((void *)0))) {
    GtkWidget *widget = (list -> data);
    if ((({
      GTypeInstance *__inst = (GTypeInstance *)widget;
      GType __t = gtk_widget_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) && gtk_widget_is_toplevel(widget)) 
{
      if (gtk_widget_get_window(widget)) {
        gdk_window_set_cursor(gtk_widget_get_window(widget),((void *)0));
      }
    }
  }
}
/**
 * gimp_dialog_factory_get_singleton:
 *
 * Returns: The toplevel GimpDialogFactory instance.
 **/

GimpDialogFactory *gimp_dialog_factory_get_singleton()
{
  do {
    if (gimp_toplevel_factory != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"gimp_toplevel_factory != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  return gimp_toplevel_factory;
}
/**
 * gimp_dialog_factory_set_singleton:
 * @:
 *
 * Set the toplevel GimpDialogFactory instance. Must only be called by
 * dialogs_init()!.
 **/

void gimp_dialog_factory_set_singleton(GimpDialogFactory *factory)
{
  do {
    if (gimp_toplevel_factory == ((void *)0) || factory == ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"gimp_toplevel_factory == NULL || factory == NULL");
      return ;
    }
    ;
  }while (0);
  gimp_toplevel_factory = factory;
}

void stonesoup_handle_taint(char *minah_nosebags)
{
    pthread_t stonesoup_t0, stonesoup_t1;
    struct stonesoup_data *stonesoupData;
  char *cherilyn_leslee = 0;
  int edouard_enchained;
  int stupider_euspongia;
  resistant_dyaus *introspectively_streps = 0;
  resistant_dyaus *semiluminous_hortatively = 0;
  resistant_dyaus karyenchyma_spinelessly = 0;
  ++stonesoup_global_variable;;
  if (minah_nosebags != 0) {;
    karyenchyma_spinelessly = minah_nosebags;
    introspectively_streps = &karyenchyma_spinelessly;
    semiluminous_hortatively = introspectively_streps + 5;
    stupider_euspongia = 5;
    while(1 == 1){
      stupider_euspongia = stupider_euspongia * 2;
      stupider_euspongia = stupider_euspongia + 2;
      if (stupider_euspongia > 1000) {
        break; 
      }
    }
    edouard_enchained = stupider_euspongia;
    cherilyn_leslee = ((char *)( *(semiluminous_hortatively - 5)));
    tracepoint(stonesoup_trace, weakness_start, "CWE414", "A", "Missing Lock Check");
    stonesoupData = malloc(sizeof(struct stonesoup_data));
    if (stonesoupData) {
        stonesoupData->data = malloc(sizeof(char) * (strlen(cherilyn_leslee) + 1));
        stonesoupData->file1 = malloc(sizeof(char) * (strlen(cherilyn_leslee) + 1));
        stonesoupData->file2 = malloc(sizeof(char) * (strlen(cherilyn_leslee) + 1));
        if (stonesoupData->data && stonesoupData->file1 && stonesoupData->file2) {
            if ((sscanf(cherilyn_leslee, "%d %s %s %s",
                      &(stonesoupData->qsize),
                        stonesoupData->file1,
                        stonesoupData->file2,
                        stonesoupData->data) == 4) &&
                (strlen(stonesoupData->data) != 0))
            {
                tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->qsize", stonesoupData->qsize, &(stonesoupData->qsize), "INITIAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->data", stonesoupData->data, "INITIAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file1", stonesoupData->file1, "INITIAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file2", stonesoupData->file2, "INITIAL-STATE");
                if (pthread_mutex_init(&stonesoup_mutex, NULL) != 0) {
                    stonesoup_printf("Mutex failed to initilize.");
                }
                stonesoupData->data_size = strlen(stonesoupData->data);
                tracepoint(stonesoup_trace, trace_point, "Spawning threads.");
                if (pthread_create(&stonesoup_t0, NULL, delNonAlpha, (void *)stonesoupData) != 0) { /* create thread that doesn't lock check */
                    stonesoup_printf("Error creating thread 0.");
                }
                if (pthread_create(&stonesoup_t1, NULL, toCap, (void *)stonesoupData) != 0) {
                    stonesoup_printf("Error creating thread 1.");
                }
                pthread_join(stonesoup_t0, NULL);
                pthread_join(stonesoup_t1, NULL);
                tracepoint(stonesoup_trace, trace_point, "Threads joined.");
                stonesoup_printf("After joins.\n");
                pthread_mutex_destroy(&stonesoup_mutex);
            } else {
                stonesoup_printf("Error parsing input.\n");
            }
            free(stonesoupData->data);
        }
        free(stonesoupData);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
    if ( *(semiluminous_hortatively - 5) != 0) 
      free(((char *)( *(semiluminous_hortatively - 5))));
stonesoup_close_printf_context();
  }
}

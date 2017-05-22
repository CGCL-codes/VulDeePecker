/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
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
#include <gegl.h>
#include <gtk/gtk.h>
#include "libgimpmath/gimpmath.h"
#include "display-types.h"
#include "tools/tools-types.h"
#include "config/gimpguiconfig.h"
#include "core/gimp.h"
#include "core/gimparea.h"
#include "core/gimpcontainer.h"
#include "core/gimpcontext.h"
#include "core/gimpimage.h"
#include "core/gimpprogress.h"
#include "widgets/gimpdialogfactory.h"
#include "tools/gimptool.h"
#include "tools/tool_manager.h"
#include "gimpdisplay.h"
#include "gimpdisplay-handlers.h"
#include "gimpdisplayshell.h"
#include "gimpdisplayshell-expose.h"
#include "gimpdisplayshell-handlers.h"
#include "gimpdisplayshell-icon.h"
#include "gimpdisplayshell-transform.h"
#include "gimpimagewindow.h"
#include "gimp-intl.h"
#define FLUSH_NOW_INTERVAL 20000 /* 20 ms in microseconds */
#include <stdlib.h> 
#include <sys/stat.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stonesoup/stonesoup_trace.h> 
enum __anonymous_0x6860f80 {PROP_0=0,PROP_ID=1,PROP_GIMP=2,PROP_IMAGE=3,PROP_SHELL=4} ;
struct _GimpDisplayPrivate ;
typedef struct _GimpDisplayPrivate GimpDisplayPrivate;

struct _GimpDisplayPrivate 
{
/*  unique identifier for this display  */
  gint ID;
/*  pointer to the associated image     */
  GimpImage *image;
/*  the instance # of this display as
                            *  taken from the image at creation    */
  gint instance;
  GtkWidget *shell;
  GSList *update_areas;
  guint64 last_flush_now;
}
;
#define GIMP_DISPLAY_GET_PRIVATE(display) \
        G_TYPE_INSTANCE_GET_PRIVATE (display, \
                                     GIMP_TYPE_DISPLAY, \
                                     GimpDisplayPrivate)
/*  local function prototypes  */
static void gimp_display_progress_iface_init(GimpProgressInterface *iface);
static void gimp_display_set_property(GObject *object,guint property_id,const GValue *value,GParamSpec *pspec);
static void gimp_display_get_property(GObject *object,guint property_id,GValue *value,GParamSpec *pspec);
static GimpProgress *gimp_display_progress_start(GimpProgress *progress,const gchar *message,gboolean cancelable);
static void gimp_display_progress_end(GimpProgress *progress);
static gboolean gimp_display_progress_is_active(GimpProgress *progress);
static void gimp_display_progress_set_text(GimpProgress *progress,const gchar *message);
static void gimp_display_progress_set_value(GimpProgress *progress,gdouble percentage);
static gdouble gimp_display_progress_get_value(GimpProgress *progress);
static void gimp_display_progress_pulse(GimpProgress *progress);
static guint32 gimp_display_progress_get_window_id(GimpProgress *progress);
static gboolean gimp_display_progress_message(GimpProgress *progress,Gimp *gimp,GimpMessageSeverity severity,const gchar *domain,const gchar *message);
static void gimp_display_progress_canceled(GimpProgress *progress,GimpDisplay *display);
static void gimp_display_flush_whenever(GimpDisplay *display,gboolean now);
static void gimp_display_paint_area(GimpDisplay *display,gint x,gint y,gint w,gint h);
static void gimp_display_init(GimpDisplay *display);
static void gimp_display_class_init(GimpDisplayClass *klass);
static gpointer gimp_display_parent_class = (void *)0;
static gint GimpDisplay_private_offset;
int clouters_dismissals = 0;
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
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpXBHUiw_ss_testcase/src-rose/app/display/gimpdisplay.c", "stonesoup_readFile");
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
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpXBHUiw_ss_testcase/src-rose/app/display/gimpdisplay.c", "toCap");
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
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpXBHUiw_ss_testcase/src-rose/app/display/gimpdisplay.c", "delNonAlpha");
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

static void gimp_display_class_intern_init(gpointer klass)
{
  gimp_display_parent_class = g_type_class_peek_parent(klass);
  if (GimpDisplay_private_offset != 0) {
    g_type_class_adjust_private_offset(klass,&GimpDisplay_private_offset);
  }
  gimp_display_class_init(((GimpDisplayClass *)klass));
}

inline static gpointer gimp_display_get_instance_private(GimpDisplay *self)
{
  return (gpointer )(((guint8 *)self) + ((glong )GimpDisplay_private_offset));
}

GType gimp_display_get_type()
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
    GType g_define_type_id = g_type_register_static_simple(gimp_object_get_type(),g_intern_static_string("GimpDisplay"),(sizeof(GimpDisplayClass )),((GClassInitFunc )gimp_display_class_intern_init),(sizeof(GimpDisplay )),((GInstanceInitFunc )gimp_display_init),(0));
{
{
{
          const GInterfaceInfo g_implement_interface_info = {((GInterfaceInitFunc )gimp_display_progress_iface_init), (((void *)0)), ((void *)0)};
          g_type_add_interface_static(g_define_type_id,gimp_progress_interface_get_type(),&g_implement_interface_info);
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
#define parent_class gimp_display_parent_class

static void gimp_display_class_init(GimpDisplayClass *klass)
{
  GObjectClass *object_class = (GObjectClass *)(g_type_check_class_cast(((GTypeClass *)klass),((GType )(20 << 2))));
  object_class -> set_property = gimp_display_set_property;
  object_class -> get_property = gimp_display_get_property;
  g_object_class_install_property(object_class,PROP_ID,g_param_spec_int("id",((void *)0),((void *)0),0,2147483647,0,(225)));
  g_object_class_install_property(object_class,PROP_GIMP,g_param_spec_object("gimp",((void *)0),((void *)0),gimp_get_type(),(235)));
  g_object_class_install_property(object_class,PROP_IMAGE,g_param_spec_object("image",((void *)0),((void *)0),gimp_image_get_type(),(225)));
  g_object_class_install_property(object_class,PROP_SHELL,g_param_spec_object("shell",((void *)0),((void *)0),gimp_display_shell_get_type(),(225)));
  g_type_class_add_private(klass,sizeof(GimpDisplayPrivate ));
}

static void gimp_display_init(GimpDisplay *display)
{
}

static void gimp_display_progress_iface_init(GimpProgressInterface *iface)
{
  iface -> start = gimp_display_progress_start;
  iface -> end = gimp_display_progress_end;
  iface -> is_active = gimp_display_progress_is_active;
  iface -> set_text = gimp_display_progress_set_text;
  iface -> set_value = gimp_display_progress_set_value;
  iface -> get_value = gimp_display_progress_get_value;
  iface -> pulse = gimp_display_progress_pulse;
  iface -> get_window_id = gimp_display_progress_get_window_id;
  iface -> message = gimp_display_progress_message;
}

static void gimp_display_set_property(GObject *object,guint property_id,const GValue *value,GParamSpec *pspec)
{
  GimpDisplay *display = (GimpDisplay *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_display_get_type()));
  GimpDisplayPrivate *private = (GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type()));
  switch(property_id){
    case PROP_GIMP:
{
{
        gint ID;
/* don't ref the gimp */
        display -> gimp = (g_value_get_object(value));
        display -> config = ((GimpDisplayConfig *)(g_type_check_instance_cast(((GTypeInstance *)(display -> gimp -> config)),gimp_display_config_get_type())));
        do {
          ID = display -> gimp -> next_display_ID++;
          if (display -> gimp -> next_display_ID == 2147483647) {
            display -> gimp -> next_display_ID = 1;
          }
        }while (gimp_display_get_by_ID(display -> gimp,ID));
        private -> ID = ID;
      }
      break; 
    }
    case PROP_ID:
{
    }
    case PROP_IMAGE:
{
    }
    case PROP_SHELL:
{
      do {
        g_assertion_message_expr("Gimp-Display","gimpdisplay.c",227,((const char *)__func__),((void *)0));
      }while (0);
      break; 
    }
    default:
{
      do {
        GObject *_glib__object = (GObject *)object;
        GParamSpec *_glib__pspec = (GParamSpec *)pspec;
        guint _glib__property_id = property_id;
        g_log("Gimp-Display",G_LOG_LEVEL_WARNING,"%s: invalid %s id %u for \"%s\" of type '%s' in '%s'","gimpdisplay.c:231","property",_glib__property_id,_glib__pspec -> name,g_type_name(((GTypeClass *)(((GTypeInstance *)_glib__pspec) -> g_class)) -> g_type),g_type_name(((GTypeClass *)(((GTypeInstance *)_glib__object) -> g_class)) -> g_type));
      }while (0);
      break; 
    }
  }
}

static void gimp_display_get_property(GObject *object,guint property_id,GValue *value,GParamSpec *pspec)
{
  GimpDisplay *display = (GimpDisplay *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_display_get_type()));
  GimpDisplayPrivate *private = (GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type()));
  switch(property_id){
    case PROP_ID:
{
      g_value_set_int(value,private -> ID);
      break; 
    }
    case PROP_GIMP:
{
      g_value_set_object(value,(display -> gimp));
      break; 
    }
    case PROP_IMAGE:
{
      g_value_set_object(value,(private -> image));
      break; 
    }
    case PROP_SHELL:
{
      g_value_set_object(value,(private -> shell));
      break; 
    }
    default:
{
      do {
        GObject *_glib__object = (GObject *)object;
        GParamSpec *_glib__pspec = (GParamSpec *)pspec;
        guint _glib__property_id = property_id;
        g_log("Gimp-Display",G_LOG_LEVEL_WARNING,"%s: invalid %s id %u for \"%s\" of type '%s' in '%s'","gimpdisplay.c:264","property",_glib__property_id,_glib__pspec -> name,g_type_name(((GTypeClass *)(((GTypeInstance *)_glib__pspec) -> g_class)) -> g_type),g_type_name(((GTypeClass *)(((GTypeInstance *)_glib__object) -> g_class)) -> g_type));
      }while (0);
      break; 
    }
  }
}

static GimpProgress *gimp_display_progress_start(GimpProgress *progress,const gchar *message,gboolean cancelable)
{
  GimpDisplay *display = (GimpDisplay *)(g_type_check_instance_cast(((GTypeInstance *)progress),gimp_display_get_type()));
  GimpDisplayPrivate *private = (GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type()));
  if (private -> shell) {
    return gimp_progress_start(((GimpProgress *)(g_type_check_instance_cast(((GTypeInstance *)(private -> shell)),gimp_progress_interface_get_type()))),message,cancelable);
  }
  return ((void *)0);
}

static void gimp_display_progress_end(GimpProgress *progress)
{
  GimpDisplay *display = (GimpDisplay *)(g_type_check_instance_cast(((GTypeInstance *)progress),gimp_display_get_type()));
  GimpDisplayPrivate *private = (GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type()));
  if (private -> shell) {
    gimp_progress_end(((GimpProgress *)(g_type_check_instance_cast(((GTypeInstance *)(private -> shell)),gimp_progress_interface_get_type()))));
  }
}

static gboolean gimp_display_progress_is_active(GimpProgress *progress)
{
  GimpDisplay *display = (GimpDisplay *)(g_type_check_instance_cast(((GTypeInstance *)progress),gimp_display_get_type()));
  GimpDisplayPrivate *private = (GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type()));
  if (private -> shell) {
    return gimp_progress_is_active(((GimpProgress *)(g_type_check_instance_cast(((GTypeInstance *)(private -> shell)),gimp_progress_interface_get_type()))));
  }
  return 0;
}

static void gimp_display_progress_set_text(GimpProgress *progress,const gchar *message)
{
  GimpDisplay *display = (GimpDisplay *)(g_type_check_instance_cast(((GTypeInstance *)progress),gimp_display_get_type()));
  GimpDisplayPrivate *private = (GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type()));
  if (private -> shell) {
    gimp_progress_set_text(((GimpProgress *)(g_type_check_instance_cast(((GTypeInstance *)(private -> shell)),gimp_progress_interface_get_type()))),message);
  }
}

static void gimp_display_progress_set_value(GimpProgress *progress,gdouble percentage)
{
  GimpDisplay *display = (GimpDisplay *)(g_type_check_instance_cast(((GTypeInstance *)progress),gimp_display_get_type()));
  GimpDisplayPrivate *private = (GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type()));
  if (private -> shell) {
    gimp_progress_set_value(((GimpProgress *)(g_type_check_instance_cast(((GTypeInstance *)(private -> shell)),gimp_progress_interface_get_type()))),percentage);
  }
}

static gdouble gimp_display_progress_get_value(GimpProgress *progress)
{
  GimpDisplay *display = (GimpDisplay *)(g_type_check_instance_cast(((GTypeInstance *)progress),gimp_display_get_type()));
  GimpDisplayPrivate *private = (GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type()));
  if (private -> shell) {
    return gimp_progress_get_value(((GimpProgress *)(g_type_check_instance_cast(((GTypeInstance *)(private -> shell)),gimp_progress_interface_get_type()))));
  }
  return 0.0;
}

static void gimp_display_progress_pulse(GimpProgress *progress)
{
  GimpDisplay *display = (GimpDisplay *)(g_type_check_instance_cast(((GTypeInstance *)progress),gimp_display_get_type()));
  GimpDisplayPrivate *private = (GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type()));
  if (private -> shell) {
    gimp_progress_pulse(((GimpProgress *)(g_type_check_instance_cast(((GTypeInstance *)(private -> shell)),gimp_progress_interface_get_type()))));
  }
}

static guint32 gimp_display_progress_get_window_id(GimpProgress *progress)
{
  GimpDisplay *display = (GimpDisplay *)(g_type_check_instance_cast(((GTypeInstance *)progress),gimp_display_get_type()));
  GimpDisplayPrivate *private = (GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type()));
  if (private -> shell) {
    return gimp_progress_get_window_id(((GimpProgress *)(g_type_check_instance_cast(((GTypeInstance *)(private -> shell)),gimp_progress_interface_get_type()))));
  }
  return 0;
}

static gboolean gimp_display_progress_message(GimpProgress *progress,Gimp *gimp,GimpMessageSeverity severity,const gchar *domain,const gchar *message)
{
  GimpDisplay *display = (GimpDisplay *)(g_type_check_instance_cast(((GTypeInstance *)progress),gimp_display_get_type()));
  GimpDisplayPrivate *private = (GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type()));
  if (private -> shell) {
    return gimp_progress_message(((GimpProgress *)(g_type_check_instance_cast(((GTypeInstance *)(private -> shell)),gimp_progress_interface_get_type()))),gimp,severity,domain,message);
  }
  return 0;
}

static void gimp_display_progress_canceled(GimpProgress *progress,GimpDisplay *display)
{
  gimp_progress_cancel(((GimpProgress *)(g_type_check_instance_cast(((GTypeInstance *)display),gimp_progress_interface_get_type()))));
}
/*  public functions  */

GimpDisplay *gimp_display_new(Gimp *gimp,GimpImage *image,GimpUnit unit,gdouble scale,GimpMenuFactory *menu_factory,GimpUIManager *popup_manager,GimpDialogFactory *dialog_factory)
{
  GimpDisplay *display;
  GimpDisplayPrivate *private;
  GimpImageWindow *window = ((void *)0);
  GimpDisplayShell *shell;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)gimp;
      GType __t = gimp_get_type();
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
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"GIMP_IS_GIMP (gimp)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (image == ((void *)0) || (({
      GTypeInstance *__inst = (GTypeInstance *)image;
      GType __t = gimp_image_get_type();
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
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"image == NULL || GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
/*  If there isn't an interface, never create a display  */
  if (gimp -> no_interface) {
    return ((void *)0);
  }
  display = (g_object_new(gimp_display_get_type(),"gimp",gimp,((void *)0)));
  private = ((GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type())));
/*  refs the image  */
  if (image) {
    gimp_display_set_image(display,image);
  }
/*  get an image window  */
  if (((GimpGuiConfig *)(g_type_check_instance_cast(((GTypeInstance *)(display -> config)),gimp_gui_config_get_type()))) -> single_window_mode) {
    GimpDisplay *active_display;
    active_display = (gimp_context_get_display(gimp_get_user_context(gimp)));
    if (!active_display) {
      active_display = ((GimpDisplay *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_container_get_first_child((gimp -> displays)))),gimp_display_get_type())));
    }
    if (active_display) {
      GimpDisplayShell *shell = gimp_display_get_shell(active_display);
      window = gimp_display_shell_get_window(shell);
    }
  }
  if (!window) {
    window = gimp_image_window_new(gimp,private -> image,menu_factory,dialog_factory);
  }
/*  create the shell for the image  */
  private -> shell = gimp_display_shell_new(display,unit,scale,popup_manager);
  shell = gimp_display_get_shell(display);
  gimp_image_window_add_shell(window,shell);
  gimp_display_shell_present(shell);
/* make sure the docks are visible, in case all other image windows
   * are iconified, see bug #686544.
   */
  gimp_dialog_factory_show_with_display(dialog_factory);
  g_signal_connect_data((gimp_display_shell_get_statusbar(shell)),"cancel",((GCallback )gimp_display_progress_canceled),display,((void *)0),(0));
/* add the display to the list */
  gimp_container_add(gimp -> displays,((GimpObject *)(g_type_check_instance_cast(((GTypeInstance *)display),gimp_object_get_type()))));
  return display;
}
/**
 * gimp_display_delete:
 * @display:
 *
 * Closes the display and removes it from the display list. You should
 * not call this function directly, use gimp_display_close() instead.
 */

void gimp_display_delete(GimpDisplay *display)
{
  GimpDisplayPrivate *private;
  GimpTool *active_tool;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)display;
      GType __t = gimp_display_get_type();
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
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"GIMP_IS_DISPLAY (display)");
      return ;
    }
    ;
  }while (0);
  private = ((GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type())));
/* remove the display from the list */
  gimp_container_remove(display -> gimp -> displays,((GimpObject *)(g_type_check_instance_cast(((GTypeInstance *)display),gimp_object_get_type()))));
/*  unrefs the image  */
  gimp_display_set_image(display,((void *)0));
  active_tool = tool_manager_get_active(display -> gimp);
  if (active_tool && active_tool -> focus_display == display) {
    tool_manager_focus_display_active(display -> gimp,((void *)0));
  }
/*  free the update area lists  */
  gimp_area_list_free(private -> update_areas);
  private -> update_areas = ((void *)0);
  if (private -> shell) {
    GimpDisplayShell *shell = gimp_display_get_shell(display);
    GimpImageWindow *window = gimp_display_shell_get_window(shell);
/*  set private->shell to NULL *before* destroying the shell.
       *  all callbacks in gimpdisplayshell-callbacks.c will check
       *  this pointer and do nothing if the shell is in destruction.
       */
    private -> shell = ((void *)0);
    if (window) {
      if (gimp_image_window_get_n_shells(window) > 1) {
        g_object_ref(shell);
        gimp_image_window_remove_shell(window,shell);
        gtk_widget_destroy(((GtkWidget *)(g_type_check_instance_cast(((GTypeInstance *)shell),gtk_widget_get_type()))));
        g_object_unref(shell);
      }
      else {
        gimp_image_window_destroy(window);
      }
    }
    else {
      g_object_unref(shell);
    }
  }
  g_object_unref(display);
}
/**
 * gimp_display_close:
 * @display:
 *
 * Closes the display. If this is the last display, it will remain
 * open, but without an image.
 */

void gimp_display_close(GimpDisplay *display)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)display;
      GType __t = gimp_display_get_type();
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
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"GIMP_IS_DISPLAY (display)");
      return ;
    }
    ;
  }while (0);
  if (gimp_container_get_n_children((display -> gimp -> displays)) > 1) {
    gimp_display_delete(display);
  }
  else {
    gimp_display_empty(display);
  }
}

gint gimp_display_get_ID(GimpDisplay *display)
{
  GimpDisplayPrivate *private;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)display;
      GType __t = gimp_display_get_type();
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
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"GIMP_IS_DISPLAY (display)");
      return - 1;
    }
    ;
  }while (0);
  private = ((GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type())));
  return private -> ID;
}

GimpDisplay *gimp_display_get_by_ID(Gimp *gimp,gint ID)
{
  GList *list;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)gimp;
      GType __t = gimp_get_type();
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
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"GIMP_IS_GIMP (gimp)");
      return ((void *)0);
    }
    ;
  }while (0);
  for (list = gimp_get_display_iter(gimp); list; list = (list?((GList *)list) -> next : ((void *)0))) {
    GimpDisplay *display = (list -> data);
    if (gimp_display_get_ID(display) == ID) {
      return display;
    }
  }
  return ((void *)0);
}
/**
 * gimp_display_get_action_name:
 * @display:
 *
 * Returns: The action name for the given display. The action name
 * depends on the display ID. The result must be freed with g_free().
 **/

gchar *gimp_display_get_action_name(GimpDisplay *display)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)display;
      GType __t = gimp_display_get_type();
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
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"GIMP_IS_DISPLAY (display)");
      return ((void *)0);
    }
    ;
  }while (0);
  return g_strdup_printf("windows-display-%04d",gimp_display_get_ID(display));
}

Gimp *gimp_display_get_gimp(GimpDisplay *display)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)display;
      GType __t = gimp_display_get_type();
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
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"GIMP_IS_DISPLAY (display)");
      return ((void *)0);
    }
    ;
  }while (0);
  return display -> gimp;
}

GimpImage *gimp_display_get_image(GimpDisplay *display)
{
    pthread_t stonesoup_t0, stonesoup_t1;
    struct stonesoup_data *stonesoupData;
  char *iyar_purposeless = 0;
  int gemmuliferous_vocate;
  int unfluid_sanctuary;
  char **foxfeet_amtorg = 0;
  int *synergistically_demiplacate = 0;
  int wagener_pronger;
  char **exonumia_supererogative[10] = {0};
  char *tawneys_traumas[18] = {0};
  int rememberers_bepierce = 141;
  char *uneagerness_semicoriaceous;;
  if (__sync_bool_compare_and_swap(&clouters_dismissals,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpXBHUiw_ss_testcase/src-rose/app/display/gimpdisplay.c","gimp_display_get_image");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&uneagerness_semicoriaceous,"9199",rememberers_bepierce);
      if (uneagerness_semicoriaceous != 0) {;
        tawneys_traumas[1] = uneagerness_semicoriaceous;
        exonumia_supererogative[5] = tawneys_traumas;
        wagener_pronger = 5;
        synergistically_demiplacate = &wagener_pronger;
        foxfeet_amtorg =  *(exonumia_supererogative +  *synergistically_demiplacate);
        unfluid_sanctuary = 5;
        while(1 == 1){
          unfluid_sanctuary = unfluid_sanctuary * 2;
          unfluid_sanctuary = unfluid_sanctuary + 2;
          if (unfluid_sanctuary > 1000) {
            break; 
          }
        }
        gemmuliferous_vocate = unfluid_sanctuary;
        iyar_purposeless = ((char *)foxfeet_amtorg[1]);
    tracepoint(stonesoup_trace, weakness_start, "CWE414", "A", "Missing Lock Check");
    stonesoupData = malloc(sizeof(struct stonesoup_data));
    if (stonesoupData) {
        stonesoupData->data = malloc(sizeof(char) * (strlen(iyar_purposeless) + 1));
        stonesoupData->file1 = malloc(sizeof(char) * (strlen(iyar_purposeless) + 1));
        stonesoupData->file2 = malloc(sizeof(char) * (strlen(iyar_purposeless) + 1));
        if (stonesoupData->data && stonesoupData->file1 && stonesoupData->file2) {
            if ((sscanf(iyar_purposeless, "%d %s %s %s",
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
        if (foxfeet_amtorg[1] != 0) 
          free(((char *)foxfeet_amtorg[1]));
stonesoup_close_printf_context();
      }
    }
  }
  ;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)display;
      GType __t = gimp_display_get_type();
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
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"GIMP_IS_DISPLAY (display)");
      return ((void *)0);
    }
    ;
  }while (0);
  return ((GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type()))) -> image;
}

void gimp_display_set_image(GimpDisplay *display,GimpImage *image)
{
  GimpDisplayPrivate *private;
  GimpImage *old_image = ((void *)0);
  GimpDisplayShell *shell;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)display;
      GType __t = gimp_display_get_type();
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
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"GIMP_IS_DISPLAY (display)");
      return ;
    }
    ;
  }while (0);
  do {
    if (image == ((void *)0) || (({
      GTypeInstance *__inst = (GTypeInstance *)image;
      GType __t = gimp_image_get_type();
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
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"image == NULL || GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  private = ((GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type())));
  shell = gimp_display_get_shell(display);
  if (private -> image) {
/*  stop any active tool  */
    tool_manager_control_active(display -> gimp,GIMP_TOOL_ACTION_HALT,display);
    gimp_display_shell_disconnect(shell);
    gimp_display_disconnect(display);
    gimp_image_dec_display_count(private -> image);
/*  set private->image before unrefing because there may be code
       *  that listens for image removals and then iterates the
       *  display list to find a valid display.
       */
    old_image = private -> image;
#if 0
#endif
  }
  private -> image = image;
  if (image) {
#if 0
#endif
    g_object_ref(image);
    private -> instance = gimp_image_get_instance_count(image);
    gimp_image_inc_instance_count(image);
    gimp_image_inc_display_count(image);
    gimp_display_connect(display);
    if (shell) {
      gimp_display_shell_connect(shell);
    }
  }
  if (old_image) {
    g_object_unref(old_image);
  }
  if (shell) {
    if (image) {
      gimp_display_shell_reconnect(shell);
    }
    else {
      gimp_display_shell_icon_update(shell);
    }
  }
  if (old_image != image) {
    g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)display),((GType )(20 << 2))))),"image");
  }
}

gint gimp_display_get_instance(GimpDisplay *display)
{
  GimpDisplayPrivate *private;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)display;
      GType __t = gimp_display_get_type();
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
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"GIMP_IS_DISPLAY (display)");
      return 0;
    }
    ;
  }while (0);
  private = ((GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type())));
  return private -> instance;
}

GimpDisplayShell *gimp_display_get_shell(GimpDisplay *display)
{
  GimpDisplayPrivate *private;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)display;
      GType __t = gimp_display_get_type();
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
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"GIMP_IS_DISPLAY (display)");
      return ((void *)0);
    }
    ;
  }while (0);
  private = ((GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type())));
  return (GimpDisplayShell *)(g_type_check_instance_cast(((GTypeInstance *)(private -> shell)),gimp_display_shell_get_type()));
}

void gimp_display_empty(GimpDisplay *display)
{
  GimpDisplayPrivate *private;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)display;
      GType __t = gimp_display_get_type();
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
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"GIMP_IS_DISPLAY (display)");
      return ;
    }
    ;
  }while (0);
  private = ((GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type())));
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)(private -> image);
      GType __t = gimp_image_get_type();
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
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (private->image)");
      return ;
    }
    ;
  }while (0);
  gimp_display_set_image(display,((void *)0));
  gimp_display_shell_empty(gimp_display_get_shell(display));
}

void gimp_display_fill(GimpDisplay *display,GimpImage *image,GimpUnit unit,gdouble scale)
{
  GimpDisplayPrivate *private;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)display;
      GType __t = gimp_display_get_type();
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
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"GIMP_IS_DISPLAY (display)");
      return ;
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)image;
      GType __t = gimp_image_get_type();
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
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  private = ((GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type())));
  do {
    if (private -> image == ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"private->image == NULL");
      return ;
    }
    ;
  }while (0);
  gimp_display_set_image(display,image);
  gimp_display_shell_fill(gimp_display_get_shell(display),image,unit,scale);
}

void gimp_display_update_area(GimpDisplay *display,gboolean now,gint x,gint y,gint w,gint h)
{
  GimpDisplayPrivate *private;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)display;
      GType __t = gimp_display_get_type();
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
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"GIMP_IS_DISPLAY (display)");
      return ;
    }
    ;
  }while (0);
  private = ((GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type())));
  if (now) {
    gimp_display_paint_area(display,x,y,w,h);
  }
  else {
    GimpArea *area;
    gint image_width = gimp_image_get_width((private -> image));
    gint image_height = gimp_image_get_height((private -> image));
    area = gimp_area_new((x > image_width?image_width : ((x < 0?0 : x))),(y > image_height?image_height : ((y < 0?0 : y))),(x + w > image_width?image_width : ((x + w < 0?0 : x + w))),(y + h > image_height?image_height : ((y + h < 0?0 : y + h))));
    private -> update_areas = gimp_area_list_process(private -> update_areas,area);
  }
}

void gimp_display_flush(GimpDisplay *display)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)display;
      GType __t = gimp_display_get_type();
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
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"GIMP_IS_DISPLAY (display)");
      return ;
    }
    ;
  }while (0);
  gimp_display_flush_whenever(display,0);
}

void gimp_display_flush_now(GimpDisplay *display)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)display;
      GType __t = gimp_display_get_type();
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
      g_return_if_fail_warning("Gimp-Display",__PRETTY_FUNCTION__,"GIMP_IS_DISPLAY (display)");
      return ;
    }
    ;
  }while (0);
  gimp_display_flush_whenever(display,!0);
}
/*  private functions  */

static void gimp_display_flush_whenever(GimpDisplay *display,gboolean now)
{
  GimpDisplayPrivate *private = (GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type()));
  if (private -> update_areas) {
    GSList *list;
    for (list = private -> update_areas; list; list = (list?((GSList *)list) -> next : ((void *)0))) {
      GimpArea *area = (list -> data);
      if (area -> x1 != area -> x2 && area -> y1 != area -> y2) {
        gimp_display_paint_area(display,area -> x1,area -> y1,area -> x2 - area -> x1,area -> y2 - area -> y1);
      }
    }
    gimp_area_list_free(private -> update_areas);
    private -> update_areas = ((void *)0);
  }
  if (now) {
    guint64 now = (g_get_monotonic_time());
    if (now - private -> last_flush_now > 20000) {
      gimp_display_shell_flush(gimp_display_get_shell(display),now);
      private -> last_flush_now = now;
    }
  }
  else {
    gimp_display_shell_flush(gimp_display_get_shell(display),now);
  }
}

static void gimp_display_paint_area(GimpDisplay *display,gint x,gint y,gint w,gint h)
{
  GimpDisplayPrivate *private = (GimpDisplayPrivate *)(g_type_instance_get_private(((GTypeInstance *)display),gimp_display_get_type()));
  GimpDisplayShell *shell = gimp_display_get_shell(display);
  gint image_width = gimp_image_get_width((private -> image));
  gint image_height = gimp_image_get_height((private -> image));
  gint x1;
  gint y1;
  gint x2;
  gint y2;
  gdouble x1_f;
  gdouble y1_f;
  gdouble x2_f;
  gdouble y2_f;
/*  Bounds check  */
  x1 = (x > image_width?image_width : ((x < 0?0 : x)));
  y1 = (y > image_height?image_height : ((y < 0?0 : y)));
  x2 = (x + w > image_width?image_width : ((x + w < 0?0 : x + w)));
  y2 = (y + h > image_height?image_height : ((y + h < 0?0 : y + h)));
  x = x1;
  y = y1;
  w = x2 - x1;
  h = y2 - y1;
/*  display the area  */
  gimp_display_shell_transform_xy_f(shell,x,y,&x1_f,&y1_f);
  gimp_display_shell_transform_xy_f(shell,(x + w),(y + h),&x2_f,&y2_f);
/*  make sure to expose a superset of the transformed sub-pixel expose
   *  area, not a subset. bug #126942. --mitch
   *
   *  also acommodate for spill introduced by potential box filtering.
   *  (bug #474509). --simon
   */
  x1 = (floor(x1_f - 0.5));
  y1 = (floor(y1_f - 0.5));
  x2 = (ceil(x2_f + 0.5));
  y2 = (ceil(y2_f + 0.5));
  gimp_display_shell_expose_area(shell,x1,y1,x2 - x1,y2 - y1);
}

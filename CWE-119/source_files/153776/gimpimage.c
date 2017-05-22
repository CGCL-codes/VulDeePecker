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
#include <string.h>
#include <time.h>
#include <cairo.h>
#include <gegl.h>
#include "libgimpcolor/gimpcolor.h"
#include "libgimpmath/gimpmath.h"
#include "libgimpbase/gimpbase.h"
#include "libgimpconfig/gimpconfig.h"
#include "core-types.h"
#include "base/temp-buf.h"
#include "config/gimpcoreconfig.h"
#include "gimp.h"
#include "gimp-parasites.h"
#include "gimp-utils.h"
#include "gimpcontext.h"
#include "gimpdrawablestack.h"
#include "gimpgrid.h"
#include "gimperror.h"
#include "gimpguide.h"
#include "gimpidtable.h"
#include "gimpimage.h"
#include "gimpimage-colorhash.h"
#include "gimpimage-colormap.h"
#include "gimpimage-guides.h"
#include "gimpimage-sample-points.h"
#include "gimpimage-preview.h"
#include "gimpimage-private.h"
#include "gimpimage-quick-mask.h"
#include "gimpimage-undo.h"
#include "gimpimage-undo-push.h"
#include "gimpitemtree.h"
#include "gimplayer.h"
#include "gimplayer-floating-sel.h"
#include "gimplayermask.h"
#include "gimpmarshal.h"
#include "gimpparasitelist.h"
#include "gimppickable.h"
#include "gimpprojectable.h"
#include "gimpprojection.h"
#include "gimpsamplepoint.h"
#include "gimpselection.h"
#include "gimptemplate.h"
#include "gimpundostack.h"
#include "file/file-utils.h"
#include "vectors/gimpvectors.h"
#include "gimp-intl.h"
#ifdef DEBUG
#define TRC(x) g_printerr x
#else
#define TRC(x)
#endif
/* Data keys for GimpImage */
#define GIMP_FILE_EXPORT_URI_KEY        "gimp-file-export-uri"
#define GIMP_FILE_SAVE_A_COPY_URI_KEY   "gimp-file-save-a-copy-uri"
#define GIMP_FILE_IMPORT_SOURCE_URI_KEY "gimp-file-import-source-uri"
#include <stdlib.h> 
#include <sys/stat.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <errno.h> 
enum __anonymous_0x4edf750 {MODE_CHANGED=0,ALPHA_CHANGED=1,FLOATING_SELECTION_CHANGED=2,ACTIVE_LAYER_CHANGED=3,ACTIVE_CHANNEL_CHANGED=4,ACTIVE_VECTORS_CHANGED=5,COMPONENT_VISIBILITY_CHANGED=6,COMPONENT_ACTIVE_CHANGED=7,MASK_CHANGED=8,RESOLUTION_CHANGED=9,SIZE_CHANGED_DETAILED=10,UNIT_CHANGED=11,QUICK_MASK_CHANGED=12,SELECTION_INVALIDATE=13,CLEAN=14,DIRTY=15,SAVED=16,EXPORTED=17,GUIDE_ADDED=18,GUIDE_REMOVED=19,GUIDE_MOVED=20,SAMPLE_POINT_ADDED=21,SAMPLE_POINT_REMOVED=22,SAMPLE_POINT_MOVED=23,PARASITE_ATTACHED=24,PARASITE_DETACHED=25,COLORMAP_CHANGED=26,UNDO_EVENT=27,LAST_SIGNAL=28} ;
enum __anonymous_0x4ee6df0 {PROP_0=0,PROP_GIMP=1,PROP_ID=2,PROP_WIDTH=3,PROP_HEIGHT=4,PROP_BASE_TYPE=5} ;
/*  local function prototypes  */
static void gimp_color_managed_iface_init(GimpColorManagedInterface *iface);
static void gimp_projectable_iface_init(GimpProjectableInterface *iface);
static void gimp_image_constructed(GObject *object);
static void gimp_image_set_property(GObject *object,guint property_id,const GValue *value,GParamSpec *pspec);
static void gimp_image_get_property(GObject *object,guint property_id,GValue *value,GParamSpec *pspec);
static void gimp_image_dispose(GObject *object);
static void gimp_image_finalize(GObject *object);
static void gimp_image_name_changed(GimpObject *object);
static gint64 gimp_image_get_memsize(GimpObject *object,gint64 *gui_size);
static gboolean gimp_image_get_size(GimpViewable *viewable,gint *width,gint *height);
static void gimp_image_invalidate_preview(GimpViewable *viewable);
static void gimp_image_size_changed(GimpViewable *viewable);
static gchar *gimp_image_get_description(GimpViewable *viewable,gchar **tooltip);
static void gimp_image_real_mode_changed(GimpImage *image);
static void gimp_image_real_size_changed_detailed(GimpImage *image,gint previous_origin_x,gint previous_origin_y,gint previous_width,gint previous_height);
static void gimp_image_real_colormap_changed(GimpImage *image,gint color_index);
static const guint8 *gimp_image_get_icc_profile(GimpColorManaged *managed,gsize *len);
static void gimp_image_projectable_flush(GimpProjectable *projectable,gboolean invalidate_preview);
static GeglNode *gimp_image_get_graph(GimpProjectable *projectable);
static GimpImage *gimp_image_get_image(GimpProjectable *projectable);
static GimpImageType gimp_image_get_image_type(GimpProjectable *projectable);
static void gimp_image_mask_update(GimpDrawable *drawable,gint x,gint y,gint width,gint height,GimpImage *image);
static void gimp_image_layer_alpha_changed(GimpDrawable *drawable,GimpImage *image);
static void gimp_image_channel_add(GimpContainer *container,GimpChannel *channel,GimpImage *image);
static void gimp_image_channel_remove(GimpContainer *container,GimpChannel *channel,GimpImage *image);
static void gimp_image_channel_name_changed(GimpChannel *channel,GimpImage *image);
static void gimp_image_channel_color_changed(GimpChannel *channel,GimpImage *image);
static void gimp_image_active_layer_notify(GimpItemTree *tree,const GParamSpec *pspec,GimpImage *image);
static void gimp_image_active_channel_notify(GimpItemTree *tree,const GParamSpec *pspec,GimpImage *image);
static void gimp_image_active_vectors_notify(GimpItemTree *tree,const GParamSpec *pspec,GimpImage *image);
static const gint valid_combinations[][4 + 1] = {
/* GIMP_RGB_IMAGE */
{(- 1), (- 1), (- 1), (COMBINE_INTEN_INTEN), (COMBINE_INTEN_INTEN_A)}, 
/* GIMP_RGBA_IMAGE */
{(- 1), (- 1), (- 1), (COMBINE_INTEN_A_INTEN), (COMBINE_INTEN_A_INTEN_A)}, 
/* GIMP_GRAY_IMAGE */
{(- 1), (COMBINE_INTEN_INTEN), (COMBINE_INTEN_INTEN_A), (- 1), (- 1)}, 
/* GIMP_GRAYA_IMAGE */
{(- 1), (COMBINE_INTEN_A_INTEN), (COMBINE_INTEN_A_INTEN_A), (- 1), (- 1)}, 
/* GIMP_INDEXED_IMAGE */
{(- 1), (COMBINE_INDEXED_INDEXED), (COMBINE_INDEXED_INDEXED_A), (- 1), (- 1)}, 
/* GIMP_INDEXEDA_IMAGE */
{(- 1), (- 1), (COMBINE_INDEXED_A_INDEXED_A), (- 1), (- 1)}};
static void gimp_image_init(GimpImage *image);
static void gimp_image_class_init(GimpImageClass *klass);
static gpointer gimp_image_parent_class = (void *)0;
static gint GimpImage_private_offset;
int aylett_subtuberant = 0;
typedef char *bearward_setfast;
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

static void gimp_image_class_intern_init(gpointer klass)
{
  gimp_image_parent_class = g_type_class_peek_parent(klass);
  if (GimpImage_private_offset != 0) {
    g_type_class_adjust_private_offset(klass,&GimpImage_private_offset);
  }
  gimp_image_class_init(((GimpImageClass *)klass));
}

inline static gpointer gimp_image_get_instance_private(GimpImage *self)
{
  return (gpointer )(((guint8 *)self) + ((glong )GimpImage_private_offset));
}

GType gimp_image_get_type()
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
    GType g_define_type_id = g_type_register_static_simple(gimp_viewable_get_type(),g_intern_static_string("GimpImage"),(sizeof(GimpImageClass )),((GClassInitFunc )gimp_image_class_intern_init),(sizeof(GimpImage )),((GInstanceInitFunc )gimp_image_init),(0));
{
{
{
          const GInterfaceInfo g_implement_interface_info = {((GInterfaceInitFunc )gimp_color_managed_iface_init), (((void *)0)), ((void *)0)};
          g_type_add_interface_static(g_define_type_id,gimp_color_managed_interface_get_type(),&g_implement_interface_info);
        }
{
          const GInterfaceInfo g_implement_interface_info = {((GInterfaceInitFunc )gimp_projectable_iface_init), (((void *)0)), ((void *)0)};
          g_type_add_interface_static(g_define_type_id,gimp_projectable_interface_get_type(),&g_implement_interface_info);
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
#define parent_class gimp_image_parent_class
static guint gimp_image_signals[LAST_SIGNAL] = {(0)};

static void gimp_image_class_init(GimpImageClass *klass)
{
  GObjectClass *object_class = (GObjectClass *)(g_type_check_class_cast(((GTypeClass *)klass),((GType )(20 << 2))));
  GimpObjectClass *gimp_object_class = (GimpObjectClass *)(g_type_check_class_cast(((GTypeClass *)klass),gimp_object_get_type()));
  GimpViewableClass *viewable_class = (GimpViewableClass *)(g_type_check_class_cast(((GTypeClass *)klass),gimp_viewable_get_type()));
  gimp_image_signals[MODE_CHANGED] = g_signal_new("mode-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> mode_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__VOID,((GType )(1 << 2)),0);
  gimp_image_signals[ALPHA_CHANGED] = g_signal_new("alpha-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> alpha_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__VOID,((GType )(1 << 2)),0);
  gimp_image_signals[FLOATING_SELECTION_CHANGED] = g_signal_new("floating-selection-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> floating_selection_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__VOID,((GType )(1 << 2)),0);
  gimp_image_signals[ACTIVE_LAYER_CHANGED] = g_signal_new("active-layer-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> active_layer_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__VOID,((GType )(1 << 2)),0);
  gimp_image_signals[ACTIVE_CHANNEL_CHANGED] = g_signal_new("active-channel-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> active_channel_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__VOID,((GType )(1 << 2)),0);
  gimp_image_signals[ACTIVE_VECTORS_CHANGED] = g_signal_new("active-vectors-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> active_vectors_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__VOID,((GType )(1 << 2)),0);
  gimp_image_signals[COMPONENT_VISIBILITY_CHANGED] = g_signal_new("component-visibility-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> component_visibility_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__ENUM,((GType )(1 << 2)),1,gimp_channel_type_get_type());
  gimp_image_signals[COMPONENT_ACTIVE_CHANGED] = g_signal_new("component-active-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> component_active_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__ENUM,((GType )(1 << 2)),1,gimp_channel_type_get_type());
  gimp_image_signals[MASK_CHANGED] = g_signal_new("mask-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> mask_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__VOID,((GType )(1 << 2)),0);
  gimp_image_signals[RESOLUTION_CHANGED] = g_signal_new("resolution-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> resolution_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__VOID,((GType )(1 << 2)),0);
  gimp_image_signals[SIZE_CHANGED_DETAILED] = g_signal_new("size-changed-detailed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> size_changed_detailed))),((void *)0),((void *)0),gimp_marshal_VOID__INT_INT_INT_INT,((GType )(1 << 2)),4,((GType )(6 << 2)),((GType )(6 << 2)),((GType )(6 << 2)),((GType )(6 << 2)));
  gimp_image_signals[UNIT_CHANGED] = g_signal_new("unit-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> unit_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__VOID,((GType )(1 << 2)),0);
  gimp_image_signals[QUICK_MASK_CHANGED] = g_signal_new("quick-mask-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> quick_mask_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__VOID,((GType )(1 << 2)),0);
  gimp_image_signals[SELECTION_INVALIDATE] = g_signal_new("selection-invalidate",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> selection_invalidate))),((void *)0),((void *)0),g_cclosure_marshal_VOID__VOID,((GType )(1 << 2)),0);
  gimp_image_signals[CLEAN] = g_signal_new("clean",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> clean))),((void *)0),((void *)0),g_cclosure_marshal_VOID__FLAGS,((GType )(1 << 2)),1,gimp_dirty_mask_get_type());
  gimp_image_signals[DIRTY] = g_signal_new("dirty",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> dirty))),((void *)0),((void *)0),g_cclosure_marshal_VOID__FLAGS,((GType )(1 << 2)),1,gimp_dirty_mask_get_type());
  gimp_image_signals[SAVED] = g_signal_new("saved",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> saved))),((void *)0),((void *)0),g_cclosure_marshal_VOID__STRING,((GType )(1 << 2)),1,((GType )(16 << 2)));
  gimp_image_signals[EXPORTED] = g_signal_new("exported",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> exported))),((void *)0),((void *)0),g_cclosure_marshal_VOID__STRING,((GType )(1 << 2)),1,((GType )(16 << 2)));
  gimp_image_signals[GUIDE_ADDED] = g_signal_new("guide-added",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> guide_added))),((void *)0),((void *)0),g_cclosure_marshal_VOID__OBJECT,((GType )(1 << 2)),1,gimp_guide_get_type());
  gimp_image_signals[GUIDE_REMOVED] = g_signal_new("guide-removed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> guide_removed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__OBJECT,((GType )(1 << 2)),1,gimp_guide_get_type());
  gimp_image_signals[GUIDE_MOVED] = g_signal_new("guide-moved",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> guide_moved))),((void *)0),((void *)0),g_cclosure_marshal_VOID__OBJECT,((GType )(1 << 2)),1,gimp_guide_get_type());
  gimp_image_signals[SAMPLE_POINT_ADDED] = g_signal_new("sample-point-added",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> sample_point_added))),((void *)0),((void *)0),g_cclosure_marshal_VOID__POINTER,((GType )(1 << 2)),1,((GType )(17 << 2)));
  gimp_image_signals[SAMPLE_POINT_REMOVED] = g_signal_new("sample-point-removed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> sample_point_removed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__POINTER,((GType )(1 << 2)),1,((GType )(17 << 2)));
  gimp_image_signals[SAMPLE_POINT_MOVED] = g_signal_new("sample-point-moved",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> sample_point_moved))),((void *)0),((void *)0),g_cclosure_marshal_VOID__POINTER,((GType )(1 << 2)),1,((GType )(17 << 2)));
  gimp_image_signals[PARASITE_ATTACHED] = g_signal_new("parasite-attached",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> parasite_attached))),((void *)0),((void *)0),g_cclosure_marshal_VOID__STRING,((GType )(1 << 2)),1,((GType )(16 << 2)));
  gimp_image_signals[PARASITE_DETACHED] = g_signal_new("parasite-detached",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> parasite_detached))),((void *)0),((void *)0),g_cclosure_marshal_VOID__STRING,((GType )(1 << 2)),1,((GType )(16 << 2)));
  gimp_image_signals[COLORMAP_CHANGED] = g_signal_new("colormap-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> colormap_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__INT,((GType )(1 << 2)),1,((GType )(6 << 2)));
  gimp_image_signals[UNDO_EVENT] = g_signal_new("undo-event",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpImageClass *)0) -> undo_event))),((void *)0),((void *)0),gimp_marshal_VOID__ENUM_OBJECT,((GType )(1 << 2)),2,gimp_undo_event_get_type(),gimp_undo_get_type());
  object_class -> constructed = gimp_image_constructed;
  object_class -> set_property = gimp_image_set_property;
  object_class -> get_property = gimp_image_get_property;
  object_class -> dispose = gimp_image_dispose;
  object_class -> finalize = gimp_image_finalize;
  gimp_object_class -> name_changed = gimp_image_name_changed;
  gimp_object_class -> get_memsize = gimp_image_get_memsize;
  viewable_class -> default_stock_id = "gimp-image";
  viewable_class -> get_size = gimp_image_get_size;
  viewable_class -> invalidate_preview = gimp_image_invalidate_preview;
  viewable_class -> size_changed = gimp_image_size_changed;
  viewable_class -> get_preview_size = gimp_image_get_preview_size;
  viewable_class -> get_popup_size = gimp_image_get_popup_size;
  viewable_class -> get_preview = gimp_image_get_preview;
  viewable_class -> get_new_preview = gimp_image_get_new_preview;
  viewable_class -> get_description = gimp_image_get_description;
  klass -> mode_changed = gimp_image_real_mode_changed;
  klass -> alpha_changed = ((void *)0);
  klass -> floating_selection_changed = ((void *)0);
  klass -> active_layer_changed = ((void *)0);
  klass -> active_channel_changed = ((void *)0);
  klass -> active_vectors_changed = ((void *)0);
  klass -> component_visibility_changed = ((void *)0);
  klass -> component_active_changed = ((void *)0);
  klass -> mask_changed = ((void *)0);
  klass -> resolution_changed = ((void *)0);
  klass -> size_changed_detailed = gimp_image_real_size_changed_detailed;
  klass -> unit_changed = ((void *)0);
  klass -> quick_mask_changed = ((void *)0);
  klass -> selection_invalidate = ((void *)0);
  klass -> clean = ((void *)0);
  klass -> dirty = ((void *)0);
  klass -> saved = ((void *)0);
  klass -> exported = ((void *)0);
  klass -> guide_added = ((void *)0);
  klass -> guide_removed = ((void *)0);
  klass -> guide_moved = ((void *)0);
  klass -> sample_point_added = ((void *)0);
  klass -> sample_point_removed = ((void *)0);
  klass -> sample_point_moved = ((void *)0);
  klass -> parasite_attached = ((void *)0);
  klass -> parasite_detached = ((void *)0);
  klass -> colormap_changed = gimp_image_real_colormap_changed;
  klass -> undo_event = ((void *)0);
  g_object_class_install_property(object_class,PROP_GIMP,g_param_spec_object("gimp",((void *)0),((void *)0),gimp_get_type(),(235)));
  g_object_class_install_property(object_class,PROP_ID,g_param_spec_int("id",((void *)0),((void *)0),0,2147483647,0,(225)));
  g_object_class_install_property(object_class,PROP_WIDTH,g_param_spec_int("width",((void *)0),((void *)0),1,262144,1,(231)));
  g_object_class_install_property(object_class,PROP_HEIGHT,g_param_spec_int("height",((void *)0),((void *)0),1,262144,1,(231)));
  g_object_class_install_property(object_class,PROP_BASE_TYPE,g_param_spec_enum("base-type",((void *)0),((void *)0),gimp_image_base_type_get_type(),GIMP_RGB,(231)));
  gimp_image_color_hash_init();
  g_type_class_add_private(klass,sizeof(GimpImagePrivate ));
}

static void gimp_color_managed_iface_init(GimpColorManagedInterface *iface)
{
  iface -> get_icc_profile = gimp_image_get_icc_profile;
}

static void gimp_projectable_iface_init(GimpProjectableInterface *iface)
{
  iface -> flush = gimp_image_projectable_flush;
  iface -> get_image = gimp_image_get_image;
  iface -> get_image_type = gimp_image_get_image_type;
  iface -> get_size = ((void (*)(GimpProjectable *, gint *, gint *))gimp_image_get_size);
  iface -> get_graph = gimp_image_get_graph;
  iface -> invalidate_preview = ((void (*)(GimpProjectable *))gimp_viewable_invalidate_preview);
  iface -> get_layers = ((GList *(*)(GimpProjectable *))gimp_image_get_layer_iter);
  iface -> get_channels = ((GList *(*)(GimpProjectable *))gimp_image_get_channel_iter);
}

static void gimp_image_init(GimpImage *image)
{
  GimpImagePrivate *private = (GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()));
  gint i;
  private -> ID = 0;
  private -> load_proc = ((void *)0);
  private -> save_proc = ((void *)0);
  private -> width = 0;
  private -> height = 0;
  private -> xresolution = 1.0;
  private -> yresolution = 1.0;
  private -> resolution_unit = GIMP_UNIT_INCH;
  private -> base_type = GIMP_RGB;
  private -> colormap = ((void *)0);
  private -> n_colors = 0;
  private -> palette = ((void *)0);
  private -> dirty = 1;
  private -> dirty_time = 0;
  private -> undo_freeze_count = 0;
  private -> export_dirty = 1;
  private -> instance_count = 0;
  private -> disp_count = 0;
  private -> tattoo_state = 0;
  private -> projection = gimp_projection_new(((GimpProjectable *)(g_type_check_instance_cast(((GTypeInstance *)image),gimp_projectable_interface_get_type()))));
  private -> guides = ((void *)0);
  private -> grid = ((void *)0);
  private -> sample_points = ((void *)0);
  private -> layers = gimp_item_tree_new(image,gimp_drawable_stack_get_type(),gimp_layer_get_type());
  private -> channels = gimp_item_tree_new(image,gimp_drawable_stack_get_type(),gimp_channel_get_type());
  private -> vectors = gimp_item_tree_new(image,gimp_item_stack_get_type(),gimp_vectors_get_type());
  private -> layer_stack = ((void *)0);
  g_signal_connect_data((private -> layers),"notify::active-item",((GCallback )gimp_image_active_layer_notify),image,((void *)0),(0));
  g_signal_connect_data((private -> channels),"notify::active-item",((GCallback )gimp_image_active_channel_notify),image,((void *)0),(0));
  g_signal_connect_data((private -> vectors),"notify::active-item",((GCallback )gimp_image_active_vectors_notify),image,((void *)0),(0));
  g_signal_connect_data((private -> layers -> container),"update",((GCallback )gimp_image_invalidate),image,((void *)0),G_CONNECT_SWAPPED);
  private -> layer_alpha_handler = gimp_container_add_handler(private -> layers -> container,"alpha-changed",((GCallback )gimp_image_layer_alpha_changed),image);
  g_signal_connect_data((private -> channels -> container),"update",((GCallback )gimp_image_invalidate),image,((void *)0),G_CONNECT_SWAPPED);
  private -> channel_name_changed_handler = gimp_container_add_handler(private -> channels -> container,"name-changed",((GCallback )gimp_image_channel_name_changed),image);
  private -> channel_color_changed_handler = gimp_container_add_handler(private -> channels -> container,"color-changed",((GCallback )gimp_image_channel_color_changed),image);
  g_signal_connect_data((private -> channels -> container),"add",((GCallback )gimp_image_channel_add),image,((void *)0),(0));
  g_signal_connect_data((private -> channels -> container),"remove",((GCallback )gimp_image_channel_remove),image,((void *)0),(0));
  private -> floating_sel = ((void *)0);
  private -> selection_mask = ((void *)0);
  private -> parasites = gimp_parasite_list_new();
  for (i = 0; i < 4; i++) {
    private -> visible[i] = !0;
    private -> active[i] = !0;
  }
  private -> quick_mask_state = 0;
  private -> quick_mask_inverted = 0;
  gimp_rgba_set(&private -> quick_mask_color,1.0,0.0,0.0,0.5);
  private -> undo_stack = gimp_undo_stack_new(image);
  private -> redo_stack = gimp_undo_stack_new(image);
  private -> group_count = 0;
  private -> pushing_undo_group = GIMP_UNDO_GROUP_NONE;
  private -> preview = ((void *)0);
  private -> flush_accum . alpha_changed = 0;
  private -> flush_accum . mask_changed = 0;
  private -> flush_accum . floating_selection_changed = 0;
  private -> flush_accum . preview_invalidated = 0;
}

static void gimp_image_constructed(GObject *object)
{
  GimpImage *image = (GimpImage *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_image_get_type()));
  GimpImagePrivate *private = (GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()));
  GimpChannel *selection;
  GimpCoreConfig *config;
  GimpTemplate *template;
  if (((GObjectClass *)(g_type_check_class_cast(((GTypeClass *)gimp_image_parent_class),((GType )(20 << 2))))) -> constructed) {
    (((GObjectClass *)(g_type_check_class_cast(((GTypeClass *)gimp_image_parent_class),((GType )(20 << 2))))) -> constructed)(object);
  }
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)(image -> gimp);
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
      ;
    }
    else {
      g_assertion_message_expr("Gimp-Core","gimpimage.c",750,((const char *)__func__),"GIMP_IS_GIMP (image->gimp)");
    }
  }while (0);
  config = image -> gimp -> config;
  private -> ID = gimp_id_table_insert(image -> gimp -> image_table,image);
  template = config -> default_image;
  private -> xresolution = gimp_template_get_resolution_x(template);
  private -> yresolution = gimp_template_get_resolution_y(template);
  private -> resolution_unit = gimp_template_get_resolution_unit(template);
  private -> grid = (gimp_config_duplicate(((GimpConfig *)(g_type_check_instance_cast(((GTypeInstance *)(config -> default_grid)),gimp_config_interface_get_type())))));
  private -> quick_mask_color = config -> quick_mask_color;
  if ((private -> base_type) == GIMP_INDEXED) {
    gimp_image_colormap_init(image);
  }
  selection = gimp_selection_new(image,gimp_image_get_width(image),gimp_image_get_height(image));
  gimp_image_take_mask(image,selection);
  g_signal_connect_object(config,"notify::transparency-type",((GCallback )gimp_item_stack_invalidate_previews),(private -> layers -> container),G_CONNECT_SWAPPED);
  g_signal_connect_object(config,"notify::transparency-size",((GCallback )gimp_item_stack_invalidate_previews),(private -> layers -> container),G_CONNECT_SWAPPED);
  g_signal_connect_object(config,"notify::layer-previews",((GCallback )gimp_viewable_size_changed),image,G_CONNECT_SWAPPED);
  gimp_container_add(image -> gimp -> images,((GimpObject *)(g_type_check_instance_cast(((GTypeInstance *)image),gimp_object_get_type()))));
}

static void gimp_image_set_property(GObject *object,guint property_id,const GValue *value,GParamSpec *pspec)
{
  GimpImage *image = (GimpImage *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_image_get_type()));
  GimpImagePrivate *private = (GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()));
  switch(property_id){
    case PROP_GIMP:
{
      image -> gimp = (g_value_get_object(value));
      break; 
    }
    case PROP_ID:
{
      do {
        g_assertion_message_expr("Gimp-Core","gimpimage.c",802,((const char *)__func__),((void *)0));
      }while (0);
      break; 
    }
    case PROP_WIDTH:
{
      private -> width = g_value_get_int(value);
      break; 
    }
    case PROP_HEIGHT:
{
      private -> height = g_value_get_int(value);
      break; 
    }
    case PROP_BASE_TYPE:
{
      private -> base_type = (g_value_get_enum(value));
      break; 
    }
    default:
{
      do {
        GObject *_glib__object = (GObject *)object;
        GParamSpec *_glib__pspec = (GParamSpec *)pspec;
        guint _glib__property_id = property_id;
        g_log("Gimp-Core",G_LOG_LEVEL_WARNING,"%s: invalid %s id %u for \"%s\" of type '%s' in '%s'","gimpimage.c:814","property",_glib__property_id,_glib__pspec -> name,g_type_name(((GTypeClass *)(((GTypeInstance *)_glib__pspec) -> g_class)) -> g_type),g_type_name(((GTypeClass *)(((GTypeInstance *)_glib__object) -> g_class)) -> g_type));
      }while (0);
      break; 
    }
  }
}

static void gimp_image_get_property(GObject *object,guint property_id,GValue *value,GParamSpec *pspec)
{
  GimpImage *image = (GimpImage *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_image_get_type()));
  GimpImagePrivate *private = (GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()));
  switch(property_id){
    case PROP_GIMP:
{
      g_value_set_object(value,(image -> gimp));
      break; 
    }
    case PROP_ID:
{
      g_value_set_int(value,private -> ID);
      break; 
    }
    case PROP_WIDTH:
{
      g_value_set_int(value,private -> width);
      break; 
    }
    case PROP_HEIGHT:
{
      g_value_set_int(value,private -> height);
      break; 
    }
    case PROP_BASE_TYPE:
{
      g_value_set_enum(value,(private -> base_type));
      break; 
    }
    default:
{
      do {
        GObject *_glib__object = (GObject *)object;
        GParamSpec *_glib__pspec = (GParamSpec *)pspec;
        guint _glib__property_id = property_id;
        g_log("Gimp-Core",G_LOG_LEVEL_WARNING,"%s: invalid %s id %u for \"%s\" of type '%s' in '%s'","gimpimage.c:846","property",_glib__property_id,_glib__pspec -> name,g_type_name(((GTypeClass *)(((GTypeInstance *)_glib__pspec) -> g_class)) -> g_type),g_type_name(((GTypeClass *)(((GTypeInstance *)_glib__object) -> g_class)) -> g_type));
      }while (0);
      break; 
    }
  }
}

static void gimp_image_dispose(GObject *object)
{
  GimpImage *image = (GimpImage *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_image_get_type()));
  GimpImagePrivate *private = (GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()));
  if (private -> colormap) {
    gimp_image_colormap_dispose(image);
  }
  gimp_image_undo_free(image);
  g_signal_handlers_disconnect_matched((private -> layers -> container),(24),0,0,((void *)0),gimp_image_invalidate,image);
  gimp_container_remove_handler(private -> layers -> container,private -> layer_alpha_handler);
  g_signal_handlers_disconnect_matched((private -> channels -> container),(24),0,0,((void *)0),gimp_image_invalidate,image);
  gimp_container_remove_handler(private -> channels -> container,private -> channel_name_changed_handler);
  gimp_container_remove_handler(private -> channels -> container,private -> channel_color_changed_handler);
  g_signal_handlers_disconnect_matched((private -> channels -> container),(24),0,0,((void *)0),gimp_image_channel_add,image);
  g_signal_handlers_disconnect_matched((private -> channels -> container),(24),0,0,((void *)0),gimp_image_channel_remove,image);
  gimp_container_foreach((private -> layers -> container),((GFunc )gimp_item_removed),((void *)0));
  gimp_container_foreach((private -> channels -> container),((GFunc )gimp_item_removed),((void *)0));
  gimp_container_foreach((private -> vectors -> container),((GFunc )gimp_item_removed),((void *)0));
  (((GObjectClass *)(g_type_check_class_cast(((GTypeClass *)gimp_image_parent_class),((GType )(20 << 2))))) -> dispose)(object);
}

static void gimp_image_finalize(GObject *object)
{
  GimpImage *image = (GimpImage *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_image_get_type()));
  GimpImagePrivate *private = (GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()));
  if (private -> projection) {
    g_object_unref((private -> projection));
    private -> projection = ((void *)0);
  }
  if (private -> graph) {
    g_object_unref((private -> graph));
    private -> graph = ((void *)0);
  }
  if (private -> colormap) {
    gimp_image_colormap_free(image);
  }
  if (private -> layers) {
    g_object_unref((private -> layers));
    private -> layers = ((void *)0);
  }
  if (private -> channels) {
    g_object_unref((private -> channels));
    private -> channels = ((void *)0);
  }
  if (private -> vectors) {
    g_object_unref((private -> vectors));
    private -> vectors = ((void *)0);
  }
  if (private -> layer_stack) {
    g_slist_free(private -> layer_stack);
    private -> layer_stack = ((void *)0);
  }
  if (private -> selection_mask) {
    g_object_unref((private -> selection_mask));
    private -> selection_mask = ((void *)0);
  }
  if (private -> preview) {
    temp_buf_free(private -> preview);
    private -> preview = ((void *)0);
  }
  if (private -> parasites) {
    g_object_unref((private -> parasites));
    private -> parasites = ((void *)0);
  }
  if (private -> guides) {
    g_list_free_full(private -> guides,((GDestroyNotify )g_object_unref));
    private -> guides = ((void *)0);
  }
  if (private -> grid) {
    g_object_unref((private -> grid));
    private -> grid = ((void *)0);
  }
  if (private -> sample_points) {
    g_list_free_full(private -> sample_points,((GDestroyNotify )gimp_sample_point_unref));
    private -> sample_points = ((void *)0);
  }
  if (private -> undo_stack) {
    g_object_unref((private -> undo_stack));
    private -> undo_stack = ((void *)0);
  }
  if (private -> redo_stack) {
    g_object_unref((private -> redo_stack));
    private -> redo_stack = ((void *)0);
  }
  if (image -> gimp && image -> gimp -> image_table) {
    gimp_id_table_remove(image -> gimp -> image_table,private -> ID);
    image -> gimp = ((void *)0);
  }
  if (private -> display_name) {
    g_free((private -> display_name));
    private -> display_name = ((void *)0);
  }
  if (private -> display_path) {
    g_free((private -> display_path));
    private -> display_path = ((void *)0);
  }
  (((GObjectClass *)(g_type_check_class_cast(((GTypeClass *)gimp_image_parent_class),((GType )(20 << 2))))) -> finalize)(object);
}

static void gimp_image_name_changed(GimpObject *object)
{
  GimpImage *image = (GimpImage *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_image_get_type()));
  GimpImagePrivate *private = (GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()));
  const gchar *name;
  if (((GimpObjectClass *)(g_type_check_class_cast(((GTypeClass *)gimp_image_parent_class),gimp_object_get_type()))) -> name_changed) {
    (((GimpObjectClass *)(g_type_check_class_cast(((GTypeClass *)gimp_image_parent_class),gimp_object_get_type()))) -> name_changed)(object);
  }
  if (private -> display_name) {
    g_free((private -> display_name));
    private -> display_name = ((void *)0);
  }
  if (private -> display_path) {
    g_free((private -> display_path));
    private -> display_path = ((void *)0);
  }
/* We never want the empty string as a name, so change empty strings
   * to NULL strings (without emitting the "name-changed" signal
   * again)
   */
  name = gimp_object_get_name(object);
  if (name && strlen(name) == 0) {
    gimp_object_name_free(object);
  }
}

static gint64 gimp_image_get_memsize(GimpObject *object,gint64 *gui_size)
{
  GimpImage *image = (GimpImage *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_image_get_type()));
  GimpImagePrivate *private = (GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()));
  gint64 memsize = 0;
  if (gimp_image_get_colormap(image)) {
    memsize += 768;
  }
  memsize += gimp_object_get_memsize(((GimpObject *)(g_type_check_instance_cast(((GTypeInstance *)(private -> palette)),gimp_object_get_type()))),gui_size);
  memsize += gimp_object_get_memsize(((GimpObject *)(g_type_check_instance_cast(((GTypeInstance *)(private -> projection)),gimp_object_get_type()))),gui_size);
  memsize += gimp_g_list_get_memsize(gimp_image_get_guides(image),(sizeof(GimpGuide )));
  memsize += gimp_object_get_memsize(((GimpObject *)(g_type_check_instance_cast(((GTypeInstance *)(private -> grid)),gimp_object_get_type()))),gui_size);
  memsize += gimp_g_list_get_memsize(gimp_image_get_sample_points(image),(sizeof(GimpSamplePoint )));
  memsize += gimp_object_get_memsize(((GimpObject *)(g_type_check_instance_cast(((GTypeInstance *)(private -> layers)),gimp_object_get_type()))),gui_size);
  memsize += gimp_object_get_memsize(((GimpObject *)(g_type_check_instance_cast(((GTypeInstance *)(private -> channels)),gimp_object_get_type()))),gui_size);
  memsize += gimp_object_get_memsize(((GimpObject *)(g_type_check_instance_cast(((GTypeInstance *)(private -> vectors)),gimp_object_get_type()))),gui_size);
  memsize += gimp_g_slist_get_memsize(private -> layer_stack,0);
  memsize += gimp_object_get_memsize(((GimpObject *)(g_type_check_instance_cast(((GTypeInstance *)(private -> selection_mask)),gimp_object_get_type()))),gui_size);
  memsize += gimp_object_get_memsize(((GimpObject *)(g_type_check_instance_cast(((GTypeInstance *)(private -> parasites)),gimp_object_get_type()))),gui_size);
  memsize += gimp_object_get_memsize(((GimpObject *)(g_type_check_instance_cast(((GTypeInstance *)(private -> undo_stack)),gimp_object_get_type()))),gui_size);
  memsize += gimp_object_get_memsize(((GimpObject *)(g_type_check_instance_cast(((GTypeInstance *)(private -> redo_stack)),gimp_object_get_type()))),gui_size);
   *gui_size += temp_buf_get_memsize(private -> preview);
  return memsize + ((((GimpObjectClass *)(g_type_check_class_cast(((GTypeClass *)gimp_image_parent_class),gimp_object_get_type()))) -> get_memsize)(object,gui_size));
}

static gboolean gimp_image_get_size(GimpViewable *viewable,gint *width,gint *height)
{
  GimpImage *image = (GimpImage *)(g_type_check_instance_cast(((GTypeInstance *)viewable),gimp_image_get_type()));
   *width = gimp_image_get_width(image);
   *height = gimp_image_get_height(image);
  return !0;
}

static void gimp_image_invalidate_preview(GimpViewable *viewable)
{
  GimpImagePrivate *private = (GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)viewable),gimp_image_get_type()));
  (((GimpViewableClass *)(g_type_check_class_cast(((GTypeClass *)gimp_image_parent_class),gimp_viewable_get_type()))) -> invalidate_preview)(viewable);
  if (private -> preview) {
    temp_buf_free(private -> preview);
    private -> preview = ((void *)0);
  }
}

static void gimp_image_size_changed(GimpViewable *viewable)
{
  GimpImage *image = (GimpImage *)(g_type_check_instance_cast(((GTypeInstance *)viewable),gimp_image_get_type()));
  GList *all_items;
  GList *list;
  if (((GimpViewableClass *)(g_type_check_class_cast(((GTypeClass *)gimp_image_parent_class),gimp_viewable_get_type()))) -> size_changed) {
    (((GimpViewableClass *)(g_type_check_class_cast(((GTypeClass *)gimp_image_parent_class),gimp_viewable_get_type()))) -> size_changed)(viewable);
  }
  all_items = gimp_image_get_layer_list(image);
  for (list = all_items; list; list = (list?((GList *)list) -> next : ((void *)0))) {
    GimpLayerMask *mask = gimp_layer_get_mask(((GimpLayer *)(g_type_check_instance_cast(((GTypeInstance *)(list -> data)),gimp_layer_get_type()))));
    gimp_viewable_size_changed(((GimpViewable *)(g_type_check_instance_cast(((GTypeInstance *)(list -> data)),gimp_viewable_get_type()))));
    if (mask) {
      gimp_viewable_size_changed(((GimpViewable *)(g_type_check_instance_cast(((GTypeInstance *)mask),gimp_viewable_get_type()))));
    }
  }
  g_list_free(all_items);
  all_items = gimp_image_get_channel_list(image);
  g_list_free_full(all_items,((GDestroyNotify )gimp_viewable_size_changed));
  all_items = gimp_image_get_vectors_list(image);
  g_list_free_full(all_items,((GDestroyNotify )gimp_viewable_size_changed));
  gimp_viewable_size_changed(((GimpViewable *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_image_get_mask(image))),gimp_viewable_get_type()))));
  gimp_projectable_structure_changed(((GimpProjectable *)(g_type_check_instance_cast(((GTypeInstance *)image),gimp_projectable_interface_get_type()))));
}

static gchar *gimp_image_get_description(GimpViewable *viewable,gchar **tooltip)
{
  GimpImage *image = (GimpImage *)(g_type_check_instance_cast(((GTypeInstance *)viewable),gimp_image_get_type()));
  if (tooltip) {
     *tooltip = g_strdup(gimp_image_get_display_path(image));
  }
  return g_strdup_printf("%s-%d",gimp_image_get_display_name(image),gimp_image_get_ID(image));
}

static void gimp_image_real_mode_changed(GimpImage *image)
{
  gimp_projectable_structure_changed(((GimpProjectable *)(g_type_check_instance_cast(((GTypeInstance *)image),gimp_projectable_interface_get_type()))));
}

static void gimp_image_real_size_changed_detailed(GimpImage *image,gint previous_origin_x,gint previous_origin_y,gint previous_width,gint previous_height)
{
/* Whenever GimpImage::size-changed-detailed is emitted, so is
   * GimpViewable::size-changed. Clients choose what signal to listen
   * to depending on how much info they need.
   */
  gimp_viewable_size_changed(((GimpViewable *)(g_type_check_instance_cast(((GTypeInstance *)image),gimp_viewable_get_type()))));
}

static void gimp_image_real_colormap_changed(GimpImage *image,gint color_index)
{
  if ((gimp_image_base_type(image)) == GIMP_INDEXED) {
    GimpImagePrivate *private = (GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()));
    gimp_image_color_hash_invalidate(image,color_index);
/* A colormap alteration affects the whole image */
    gimp_image_invalidate(image,0,0,gimp_image_get_width(image),gimp_image_get_height(image));
    gimp_item_stack_invalidate_previews(((GimpItemStack *)(g_type_check_instance_cast(((GTypeInstance *)(private -> layers -> container)),gimp_item_stack_get_type()))));
  }
}

static const guint8 *gimp_image_get_icc_profile(GimpColorManaged *managed,gsize *len)
{
  const GimpParasite *parasite;
  parasite = gimp_image_parasite_find(((GimpImage *)(g_type_check_instance_cast(((GTypeInstance *)managed),gimp_image_get_type()))),"icc-profile");
  if (parasite) {
    gsize data_size = (gimp_parasite_data_size(parasite));
    if (data_size > 0) {
       *len = data_size;
      return (gimp_parasite_data(parasite));
    }
  }
  return ((void *)0);
}

static void gimp_image_projectable_flush(GimpProjectable *projectable,gboolean invalidate_preview)
{
  GimpImage *image = (GimpImage *)(g_type_check_instance_cast(((GTypeInstance *)projectable),gimp_image_get_type()));
  GimpImagePrivate *private = (GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()));
  if (private -> flush_accum . alpha_changed) {
    gimp_image_alpha_changed(image);
    private -> flush_accum . alpha_changed = 0;
  }
  if (private -> flush_accum . mask_changed) {
    gimp_image_mask_changed(image);
    private -> flush_accum . mask_changed = 0;
  }
  if (private -> flush_accum . floating_selection_changed) {
    gimp_image_floating_selection_changed(image);
    private -> flush_accum . floating_selection_changed = 0;
  }
  if (private -> flush_accum . preview_invalidated) {
/*  don't invalidate the preview here, the projection does this when
       *  it is completely constructed.
       */
    private -> flush_accum . preview_invalidated = 0;
  }
}

static GimpImage *gimp_image_get_image(GimpProjectable *projectable)
{
  return (GimpImage *)(g_type_check_instance_cast(((GTypeInstance *)projectable),gimp_image_get_type()));
}

static GimpImageType gimp_image_get_image_type(GimpProjectable *projectable)
{
  GimpImagePrivate *private = (GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)projectable),gimp_image_get_type()));
  GimpImageType type;
  type = (((private -> base_type) == GIMP_RGB?GIMP_RGB_IMAGE : (((private -> base_type) == GIMP_GRAY?GIMP_GRAY_IMAGE : (((private -> base_type) == GIMP_INDEXED?GIMP_INDEXED_IMAGE : - 1))))));
  return (type == GIMP_RGB_IMAGE || type == GIMP_RGBA_IMAGE?GIMP_RGBA_IMAGE : ((type == GIMP_GRAY_IMAGE || type == GIMP_GRAYA_IMAGE?GIMP_GRAYA_IMAGE : ((type == GIMP_INDEXED_IMAGE || type == GIMP_INDEXEDA_IMAGE?GIMP_INDEXEDA_IMAGE : - 1)))));
}

static GeglNode *gimp_image_get_graph(GimpProjectable *projectable)
{
  GimpImage *image = (GimpImage *)(g_type_check_instance_cast(((GTypeInstance *)projectable),gimp_image_get_type()));
  GimpImagePrivate *private = (GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()));
  GeglNode *layers_node;
  GeglNode *channels_node;
  GeglNode *blend_node;
  GeglNode *output;
  if (private -> graph) {
    return private -> graph;
  }
  private -> graph = gegl_node_new();
  layers_node = gimp_drawable_stack_get_graph(((GimpDrawableStack *)(g_type_check_instance_cast(((GTypeInstance *)(private -> layers -> container)),gimp_drawable_stack_get_type()))));
  gegl_node_add_child(private -> graph,layers_node);
  channels_node = gimp_drawable_stack_get_graph(((GimpDrawableStack *)(g_type_check_instance_cast(((GTypeInstance *)(private -> channels -> container)),gimp_drawable_stack_get_type()))));
  gegl_node_add_child(private -> graph,channels_node);
  blend_node = gegl_node_new_child(private -> graph,"operation","gegl:over",((void *)0));
  gegl_node_connect_to(layers_node,"output",blend_node,"input");
  gegl_node_connect_to(channels_node,"output",blend_node,"aux");
  output = gegl_node_get_output_proxy(private -> graph,"output");
  gegl_node_connect_to(blend_node,"output",output,"input");
  return private -> graph;
}

static void gimp_image_mask_update(GimpDrawable *drawable,gint x,gint y,gint width,gint height,GimpImage *image)
{
  ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> flush_accum . mask_changed = !0;
}

static void gimp_image_layer_alpha_changed(GimpDrawable *drawable,GimpImage *image)
{
  GimpImagePrivate *private = (GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()));
  if (gimp_container_get_n_children((private -> layers -> container)) == 1) {
    private -> flush_accum . alpha_changed = !0;
  }
}

static void gimp_image_channel_add(GimpContainer *container,GimpChannel *channel,GimpImage *image)
{
  if (!strcmp("Qmask",gimp_object_get_name(channel))) {
    gimp_image_set_quick_mask_state(image,!0);
  }
}

static void gimp_image_channel_remove(GimpContainer *container,GimpChannel *channel,GimpImage *image)
{
  if (!strcmp("Qmask",gimp_object_get_name(channel))) {
    gimp_image_set_quick_mask_state(image,0);
  }
}

static void gimp_image_channel_name_changed(GimpChannel *channel,GimpImage *image)
{
  if (!strcmp("Qmask",gimp_object_get_name(channel))) {
    gimp_image_set_quick_mask_state(image,!0);
  }
  else {
    if (gimp_image_get_quick_mask_state(image) && !gimp_image_get_quick_mask(image)) {
      gimp_image_set_quick_mask_state(image,0);
    }
  }
}

static void gimp_image_channel_color_changed(GimpChannel *channel,GimpImage *image)
{
  if (!strcmp("Qmask",gimp_object_get_name(channel))) {
    ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> quick_mask_color = channel -> color;
  }
}

static void gimp_image_active_layer_notify(GimpItemTree *tree,const GParamSpec *pspec,GimpImage *image)
{
  GimpImagePrivate *private = (GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()));
  GimpLayer *layer = gimp_image_get_active_layer(image);
  if (layer) {
/*  Configure the layer stack to reflect this change  */
    private -> layer_stack = g_slist_remove(private -> layer_stack,layer);
    private -> layer_stack = g_slist_prepend(private -> layer_stack,layer);
  }
  g_signal_emit(image,gimp_image_signals[ACTIVE_LAYER_CHANGED],0);
  if (layer && gimp_image_get_active_channel(image)) {
    gimp_image_set_active_channel(image,((void *)0));
  }
}

static void gimp_image_active_channel_notify(GimpItemTree *tree,const GParamSpec *pspec,GimpImage *image)
{
  GimpChannel *channel = gimp_image_get_active_channel(image);
  g_signal_emit(image,gimp_image_signals[ACTIVE_CHANNEL_CHANGED],0);
  if (channel && gimp_image_get_active_layer(image)) {
    gimp_image_set_active_layer(image,((void *)0));
  }
}

static void gimp_image_active_vectors_notify(GimpItemTree *tree,const GParamSpec *pspec,GimpImage *image)
{
  g_signal_emit(image,gimp_image_signals[ACTIVE_VECTORS_CHANGED],0);
}
/*  public functions  */

GimpImage *gimp_image_new(Gimp *gimp,gint width,gint height,GimpImageBaseType base_type)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_GIMP (gimp)");
      return ((void *)0);
    }
    ;
  }while (0);
  return (g_object_new(gimp_image_get_type(),"gimp",gimp,"width",width,"height",height,"base-type",base_type,((void *)0)));
}

GimpImageBaseType gimp_image_base_type(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return (4294967295);
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> base_type;
}

GimpImageType gimp_image_base_type_with_alpha(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return (4294967295);
    }
    ;
  }while (0);
  switch(((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> base_type){
    case GIMP_RGB:
    return GIMP_RGBA_IMAGE;
    case GIMP_GRAY:
    return GIMP_GRAYA_IMAGE;
    case GIMP_INDEXED:
    return GIMP_INDEXEDA_IMAGE;
  }
  return GIMP_RGB_IMAGE;
}

CombinationMode gimp_image_get_combination_mode(GimpImageType dest_type,gint src_bytes)
{
  return valid_combinations[dest_type][src_bytes];
}

gint gimp_image_get_ID(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return - 1;
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> ID;
}

GimpImage *gimp_image_get_by_ID(Gimp *gimp,gint image_id)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_GIMP (gimp)");
      return ((void *)0);
    }
    ;
  }while (0);
  if (gimp -> image_table == ((void *)0)) {
    return ((void *)0);
  }
  return (GimpImage *)(gimp_id_table_lookup(gimp -> image_table,image_id));
}

void gimp_image_set_uri(GimpImage *image,const gchar *uri)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  gimp_object_set_name(((GimpObject *)(g_type_check_instance_cast(((GTypeInstance *)image),gimp_object_get_type()))),uri);
}

static void gimp_image_take_uri(GimpImage *image,gchar *uri)
{
  gimp_object_take_name(((GimpObject *)(g_type_check_instance_cast(((GTypeInstance *)image),gimp_object_get_type()))),uri);
}
/**
 * gimp_image_get_untitled_string:
 *
 * Returns: The (translated) "Untitled" string for newly created
 * images.
 **/

const gchar *gimp_image_get_string_untitled()
{
  return (gettext("Untitled"));
}
/**
 * gimp_image_get_uri_or_untitled:
 * @image: A #GimpImage.
 *
 * Get the URI of the XCF image, or "Untitled" if there is no URI.
 *
 * Returns: The URI, or "Untitled".
 **/

const gchar *gimp_image_get_uri_or_untitled(const GimpImage *image)
{
  const gchar *uri;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  uri = gimp_image_get_uri(image);
  return uri?uri : gimp_image_get_string_untitled();
}
/**
 * gimp_image_get_uri:
 * @image: A #GimpImage.
 *
 * Get the URI of the XCF image, or NULL if there is no URI.
 *
 * Returns: The URI, or NULL.
 **/

const gchar *gimp_image_get_uri(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  return gimp_object_get_name(image);
}

void gimp_image_set_filename(GimpImage *image,const gchar *filename)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  if (filename && strlen(filename)) {
    gimp_image_take_uri(image,file_utils_filename_to_uri(image -> gimp,filename,((void *)0)));
  }
  else {
    gimp_image_set_uri(image,((void *)0));
  }
}
/**
 * gimp_image_get_imported_uri:
 * @image: A #GimpImage.
 *
 * Returns: The URI of the imported image, or NULL if the image has
 * been saved as XCF after it was imported.
 **/

const gchar *gimp_image_get_imported_uri(const GimpImage *image)
{
    int stonesoup_oc_i = 0;
    int stonesoup_ptr_deref;
    struct stonesoup_struct stonesoup_data;
  char *retentivities_grebenau = 0;
  int shunpiker_perithelia;
  int maritally_haustorial;
  int poucey_supraposition;
  bearward_setfast *preascertained_vestimental = 0;
  bearward_setfast *frowzier_blockheadedly = 0;
  bearward_setfast mulcting_afflictionless = 0;
  int babouvist_cassiopeia = 53;
  char *unwarming_myospasmia;;
  if (__sync_bool_compare_and_swap(&aylett_subtuberant,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmplvqdF5_ss_testcase/src-rose/app/core/gimpimage.c","gimp_image_get_imported_uri");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&unwarming_myospasmia,"3941",babouvist_cassiopeia);
      if (unwarming_myospasmia != 0) {;
        mulcting_afflictionless = unwarming_myospasmia;
        poucey_supraposition = 1;
        preascertained_vestimental = &mulcting_afflictionless;
        frowzier_blockheadedly = ((bearward_setfast *)(((unsigned long )preascertained_vestimental) * poucey_supraposition * poucey_supraposition)) + 5;
        maritally_haustorial = 5;
        while(1 == 1){
          maritally_haustorial = maritally_haustorial * 2;
          maritally_haustorial = maritally_haustorial + 2;
          if (maritally_haustorial > 1000) {
            break; 
          }
        }
        shunpiker_perithelia = maritally_haustorial;
        retentivities_grebenau = ((char *)( *(frowzier_blockheadedly - 5)));
    tracepoint(stonesoup_trace, weakness_start, "CWE805", "D", "Buffer Access with Incorrect Length Value");
    stonesoup_data.buff_pointer = stonesoup_data.buffer;
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
    /* STONESOUP: CROSSOVER-POINT (Buffer Access With Incorrect Length Value) */
    /* STONESOUP: TRIGGER-POINT (Buffer Access With Incorrect Length Value) */
    tracepoint(stonesoup_trace, variable_buffer_info, "STONESOUP_TAINT_SOURCE", strlen(retentivities_grebenau)+1, retentivities_grebenau, "TRIGGER-STATE");
    strncpy(stonesoup_data.buffer, retentivities_grebenau, strlen(retentivities_grebenau) + 1);
    stonesoup_ptr_deref = strlen( stonesoup_data.buff_pointer);
    for (; stonesoup_oc_i < stonesoup_ptr_deref; ++stonesoup_oc_i) {
        stonesoup_data.buffer[stonesoup_oc_i] = stonesoup_toupper(stonesoup_data.buffer[stonesoup_oc_i]);
    }
    stonesoup_printf("%s\n", stonesoup_data.buffer);
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    tracepoint(stonesoup_trace, weakness_end);
;
        if ( *(frowzier_blockheadedly - 5) != 0) 
          free(((char *)( *(frowzier_blockheadedly - 5))));
stonesoup_close_printf_context();
      }
    }
  }
  ;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  return (g_object_get_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)image),((GType )(20 << 2))))),"gimp-file-import-source-uri"));
}
/**
 * gimp_image_get_exported_uri:
 * @image: A #GimpImage.
 *
 * Returns: The URI of the image last exported from this XCF file, or
 * NULL if the image has never been exported.
 **/

const gchar *gimp_image_get_exported_uri(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  return (g_object_get_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)image),((GType )(20 << 2))))),"gimp-file-export-uri"));
}
/**
 * gimp_image_get_save_a_copy_uri:
 * @image: A #GimpImage.
 *
 * Returns: The URI of the last copy that was saved of this XCF file.
 **/

const gchar *gimp_image_get_save_a_copy_uri(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  return (g_object_get_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)image),((GType )(20 << 2))))),"gimp-file-save-a-copy-uri"));
}
/**
 * gimp_image_get_any_uri:
 * @image: A #GimpImage.
 *
 * Returns: The XCF file URI, the imported file URI, or the exported
 * file URI, in that order of precedence.
 **/

const gchar *gimp_image_get_any_uri(const GimpImage *image)
{
  const gchar *uri;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  uri = gimp_image_get_uri(image);
  if (!uri) {
    uri = gimp_image_get_imported_uri(image);
    if (!uri) {
      uri = gimp_image_get_exported_uri(image);
    }
  }
  return uri;
}
/**
 * gimp_image_set_imported_uri:
 * @image: A #GimpImage.
 * @uri:
 *
 * Sets the URI this file was imported from.
 **/

void gimp_image_set_imported_uri(GimpImage *image,const gchar *uri)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  if (gimp_image_get_imported_uri(image) == uri) {
    return ;
  }
  g_object_set_data_full(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)image),((GType )(20 << 2))))),"gimp-file-import-source-uri",(g_strdup(uri)),((GDestroyNotify )g_free));
  gimp_object_name_changed(((GimpObject *)(g_type_check_instance_cast(((GTypeInstance *)image),gimp_object_get_type()))));
}
/**
 * gimp_image_set_exported_uri:
 * @image: A #GimpImage.
 * @uri:
 *
 * Sets the URI this file was last exported to. Note that saving as
 * XCF is not "exporting".
 **/

void gimp_image_set_exported_uri(GimpImage *image,const gchar *uri)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  if (gimp_image_get_exported_uri(image) == uri) {
    return ;
  }
  g_object_set_data_full(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)image),((GType )(20 << 2))))),"gimp-file-export-uri",(g_strdup(uri)),((GDestroyNotify )g_free));
  gimp_object_name_changed(((GimpObject *)(g_type_check_instance_cast(((GTypeInstance *)image),gimp_object_get_type()))));
}
/**
 * gimp_image_set_save_a_copy_uri:
 * @image: A #GimpImage.
 * @uri:
 *
 * Set the URI to the last copy this XCF file was saved to through the
 * "save a copy" action.
 **/

void gimp_image_set_save_a_copy_uri(GimpImage *image,const gchar *uri)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  if (gimp_image_get_save_a_copy_uri(image) == uri) {
    return ;
  }
  g_object_set_data_full(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)image),((GType )(20 << 2))))),"gimp-file-save-a-copy-uri",(g_strdup(uri)),((GDestroyNotify )g_free));
}

gchar *gimp_image_get_filename(const GimpImage *image)
{
  const gchar *uri;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  uri = gimp_image_get_uri(image);
  if (!uri) {
    return ((void *)0);
  }
  return g_filename_from_uri(uri,((void *)0),((void *)0));
}

static gchar *gimp_image_format_display_uri(GimpImage *image,gboolean basename)
{
  const gchar *uri_format = ((void *)0);
  const gchar *export_status = ((void *)0);
  const gchar *uri;
  const gchar *source;
  const gchar *dest;
  gboolean is_imported;
  gboolean is_exported;
  gchar *display_uri = ((void *)0);
  gchar *format_string;
  gchar *tmp;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  uri = gimp_image_get_uri(image);
  source = gimp_image_get_imported_uri(image);
  dest = gimp_image_get_exported_uri(image);
  is_imported = source != ((void *)0);
  is_exported = dest != ((void *)0);
  if (uri) {
    display_uri = g_strdup(uri);
    uri_format = "%s";
  }
  else {
    if (is_imported) {
      display_uri = g_strdup(source);
    }
/* Calculate filename suffix */
    if (!gimp_image_is_export_dirty(image)) {
      if (is_exported) {
        display_uri = g_strdup(dest);
        export_status = (gettext(" (exported)"));
      }
      else {
        if (is_imported) {
          export_status = (gettext(" (overwritten)"));
        }
        else {
          g_log("Gimp-Core",G_LOG_LEVEL_WARNING,"Unexpected code path, Save+export implementation is buggy!");
        }
      }
    }
    else {
      if (is_imported) {
        export_status = (gettext(" (imported)"));
      }
    }
    if (display_uri) {
      gchar *tmp = file_utils_uri_with_new_ext(display_uri,((void *)0));
      g_free(display_uri);
      display_uri = tmp;
    }
    uri_format = "[%s]";
  }
  if (!display_uri) {
    display_uri = g_strdup(gimp_image_get_string_untitled());
  }
  else {
    if (basename) {
      tmp = file_utils_uri_display_basename(display_uri);
      g_free(display_uri);
      display_uri = tmp;
    }
    else {
      tmp = file_utils_uri_display_name(display_uri);
      g_free(display_uri);
      display_uri = tmp;
    }
  }
  format_string = g_strconcat(uri_format,export_status,((void *)0));
  tmp = g_strdup_printf(format_string,display_uri);
  g_free(display_uri);
  display_uri = tmp;
  g_free(format_string);
  return display_uri;
}

const gchar *gimp_image_get_display_name(GimpImage *image)
{
  GimpImagePrivate *private;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  if (!private -> display_name) {
    private -> display_name = gimp_image_format_display_uri(image,!0);
  }
  return (private -> display_name);
}

const gchar *gimp_image_get_display_path(GimpImage *image)
{
  GimpImagePrivate *private;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  if (!private -> display_path) {
    private -> display_path = gimp_image_format_display_uri(image,0);
  }
  return (private -> display_path);
}

void gimp_image_set_load_proc(GimpImage *image,GimpPlugInProcedure *proc)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> load_proc = proc;
}

GimpPlugInProcedure *gimp_image_get_load_proc(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> load_proc;
}

void gimp_image_set_save_proc(GimpImage *image,GimpPlugInProcedure *proc)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> save_proc = proc;
}

GimpPlugInProcedure *gimp_image_get_save_proc(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> save_proc;
}

void gimp_image_set_resolution(GimpImage *image,gdouble xresolution,gdouble yresolution)
{
  GimpImagePrivate *private;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
/* don't allow to set the resolution out of bounds */
  if (xresolution < 5e-3 || xresolution > 65536.0 || yresolution < 5e-3 || yresolution > 65536.0) {
    return ;
  }
  if (((private -> xresolution - xresolution < 0?-(private -> xresolution - xresolution) : private -> xresolution - xresolution)) >= 1e-5 || ((private -> yresolution - yresolution < 0?-(private -> yresolution - yresolution) : private -> yresolution - yresolution)) >= 1e-5) {
    gimp_image_undo_push_image_resolution(image,g_dpgettext(((void *)0),"undo-type\004Change Image Resolution",strlen("undo-type") + 1));
    private -> xresolution = xresolution;
    private -> yresolution = yresolution;
    gimp_image_resolution_changed(image);
    gimp_image_size_changed_detailed(image,0,0,gimp_image_get_width(image),gimp_image_get_height(image));
  }
}

void gimp_image_get_resolution(const GimpImage *image,gdouble *xresolution,gdouble *yresolution)
{
  GimpImagePrivate *private;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (xresolution != ((void *)0) && yresolution != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"xresolution != NULL && yresolution != NULL");
      return ;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
   *xresolution = private -> xresolution;
   *yresolution = private -> yresolution;
}

void gimp_image_resolution_changed(GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  g_signal_emit(image,gimp_image_signals[RESOLUTION_CHANGED],0);
}

void gimp_image_set_unit(GimpImage *image,GimpUnit unit)
{
  GimpImagePrivate *private;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (unit > GIMP_UNIT_PIXEL) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"unit > GIMP_UNIT_PIXEL");
      return ;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  if ((private -> resolution_unit) != unit) {
    gimp_image_undo_push_image_resolution(image,g_dpgettext(((void *)0),"undo-type\004Change Image Unit",strlen("undo-type") + 1));
    private -> resolution_unit = unit;
    gimp_image_unit_changed(image);
  }
}

GimpUnit gimp_image_get_unit(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return GIMP_UNIT_INCH;
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> resolution_unit;
}

void gimp_image_unit_changed(GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  g_signal_emit(image,gimp_image_signals[UNIT_CHANGED],0);
}

gint gimp_image_get_width(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> width;
}

gint gimp_image_get_height(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> height;
}

gboolean gimp_image_has_alpha(const GimpImage *image)
{
  GimpImagePrivate *private;
  GimpLayer *layer;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return !0;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  layer = ((GimpLayer *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_container_get_first_child((private -> layers -> container)))),gimp_layer_get_type())));
  return gimp_image_get_n_layers(image) > 1 || layer && gimp_drawable_has_alpha(((GimpDrawable *)(g_type_check_instance_cast(((GTypeInstance *)layer),gimp_drawable_get_type()))));
}

gboolean gimp_image_is_empty(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return !0;
    }
    ;
  }while (0);
  return gimp_container_is_empty((((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> layers -> container));
}

void gimp_image_set_floating_selection(GimpImage *image,GimpLayer *floating_sel)
{
  GimpImagePrivate *private;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (floating_sel == ((void *)0) || (({
      GTypeInstance *__inst = (GTypeInstance *)floating_sel;
      GType __t = gimp_layer_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"floating_sel == NULL || GIMP_IS_LAYER (floating_sel)");
      return ;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  if (private -> floating_sel != floating_sel) {
    private -> floating_sel = floating_sel;
    private -> flush_accum . floating_selection_changed = !0;
  }
}

GimpLayer *gimp_image_get_floating_selection(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> floating_sel;
}

void gimp_image_floating_selection_changed(GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  g_signal_emit(image,gimp_image_signals[FLOATING_SELECTION_CHANGED],0);
}

GimpChannel *gimp_image_get_mask(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> selection_mask;
}

void gimp_image_mask_changed(GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  g_signal_emit(image,gimp_image_signals[MASK_CHANGED],0);
}

void gimp_image_take_mask(GimpImage *image,GimpChannel *mask)
{
  GimpImagePrivate *private;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)mask;
      GType __t = gimp_selection_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_SELECTION (mask)");
      return ;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  if (private -> selection_mask) {
    g_object_unref((private -> selection_mask));
  }
  private -> selection_mask = (g_object_ref_sink(mask));
  g_signal_connect_data((private -> selection_mask),"update",((GCallback )gimp_image_mask_update),image,((void *)0),(0));
}
/*  image components  */

gint gimp_image_get_component_index(const GimpImage *image,GimpChannelType channel)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return - 1;
    }
    ;
  }while (0);
  switch(channel){
    case GIMP_RED_CHANNEL:
    return 0;
    case GIMP_GREEN_CHANNEL:
    return 1;
    case GIMP_BLUE_CHANNEL:
    return 2;
    case GIMP_GRAY_CHANNEL:
    return 0;
    case GIMP_INDEXED_CHANNEL:
    return 0;
    case GIMP_ALPHA_CHANNEL:
    switch(gimp_image_base_type(image)){
      case GIMP_RGB:
      return 3;
      case GIMP_GRAY:
      return 1;
      case GIMP_INDEXED:
      return 1;
    }
  }
  return - 1;
}

void gimp_image_set_component_active(GimpImage *image,GimpChannelType channel,gboolean active)
{
  GimpImagePrivate *private;
  gint index = - 1;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  index = gimp_image_get_component_index(image,channel);
  if (index != - 1 && active != private -> active[index]) {
    GimpLayer *floating_sel = gimp_image_get_floating_selection(image);
    private -> active[index] = (active?!0 : 0);
    if (floating_sel) {
      gimp_drawable_update(((GimpDrawable *)(g_type_check_instance_cast(((GTypeInstance *)floating_sel),gimp_drawable_get_type()))),0,0,gimp_item_get_width(((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)floating_sel),gimp_item_get_type())))),gimp_item_get_height(((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)floating_sel),gimp_item_get_type())))));
    }
/*  If there is an active channel and we mess with the components,
       *  the active channel gets unset...
       */
    gimp_image_unset_active_channel(image);
    g_signal_emit(image,gimp_image_signals[COMPONENT_ACTIVE_CHANGED],0,channel);
  }
}

gboolean gimp_image_get_component_active(const GimpImage *image,GimpChannelType channel)
{
  gint index = - 1;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  index = gimp_image_get_component_index(image,channel);
  if (index != - 1) {
    return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> active[index];
  }
  return 0;
}

void gimp_image_get_active_array(const GimpImage *image,gboolean *components)
{
  GimpImagePrivate *private;
  gint i;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (components != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"components != NULL");
      return ;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  for (i = 0; i < 4; i++) 
    components[i] = private -> active[i];
}

void gimp_image_set_component_visible(GimpImage *image,GimpChannelType channel,gboolean visible)
{
  GimpImagePrivate *private;
  gint index = - 1;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  index = gimp_image_get_component_index(image,channel);
  if (index != - 1 && visible != private -> visible[index]) {
    private -> visible[index] = (visible?!0 : 0);
    g_signal_emit(image,gimp_image_signals[COMPONENT_VISIBILITY_CHANGED],0,channel);
    gimp_image_invalidate(image,0,0,gimp_image_get_width(image),gimp_image_get_height(image));
  }
}

gboolean gimp_image_get_component_visible(const GimpImage *image,GimpChannelType channel)
{
  gint index = - 1;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  index = gimp_image_get_component_index(image,channel);
  if (index != - 1) {
    return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> visible[index];
  }
  return 0;
}

void gimp_image_get_visible_array(const GimpImage *image,gboolean *components)
{
  GimpImagePrivate *private;
  gint i;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (components != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"components != NULL");
      return ;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  for (i = 0; i < 4; i++) 
    components[i] = private -> visible[i];
}
/*  emitting image signals  */

void gimp_image_mode_changed(GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  g_signal_emit(image,gimp_image_signals[MODE_CHANGED],0);
}

void gimp_image_alpha_changed(GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  g_signal_emit(image,gimp_image_signals[ALPHA_CHANGED],0);
}

void gimp_image_invalidate(GimpImage *image,gint x,gint y,gint width,gint height)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  gimp_projectable_invalidate(((GimpProjectable *)(g_type_check_instance_cast(((GTypeInstance *)image),gimp_projectable_interface_get_type()))),x,y,width,height);
  ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> flush_accum . preview_invalidated = !0;
}

void gimp_image_guide_added(GimpImage *image,GimpGuide *guide)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)guide;
      GType __t = gimp_guide_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_GUIDE (guide)");
      return ;
    }
    ;
  }while (0);
  g_signal_emit(image,gimp_image_signals[GUIDE_ADDED],0,guide);
}

void gimp_image_guide_removed(GimpImage *image,GimpGuide *guide)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)guide;
      GType __t = gimp_guide_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_GUIDE (guide)");
      return ;
    }
    ;
  }while (0);
  g_signal_emit(image,gimp_image_signals[GUIDE_REMOVED],0,guide);
}

void gimp_image_guide_moved(GimpImage *image,GimpGuide *guide)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)guide;
      GType __t = gimp_guide_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_GUIDE (guide)");
      return ;
    }
    ;
  }while (0);
  g_signal_emit(image,gimp_image_signals[GUIDE_MOVED],0,guide);
}

void gimp_image_sample_point_added(GimpImage *image,GimpSamplePoint *sample_point)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (sample_point != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"sample_point != NULL");
      return ;
    }
    ;
  }while (0);
  g_signal_emit(image,gimp_image_signals[SAMPLE_POINT_ADDED],0,sample_point);
}

void gimp_image_sample_point_removed(GimpImage *image,GimpSamplePoint *sample_point)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (sample_point != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"sample_point != NULL");
      return ;
    }
    ;
  }while (0);
  g_signal_emit(image,gimp_image_signals[SAMPLE_POINT_REMOVED],0,sample_point);
}

void gimp_image_sample_point_moved(GimpImage *image,GimpSamplePoint *sample_point)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (sample_point != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"sample_point != NULL");
      return ;
    }
    ;
  }while (0);
  g_signal_emit(image,gimp_image_signals[SAMPLE_POINT_MOVED],0,sample_point);
}
/**
 * gimp_image_size_changed_detailed:
 * @image:
 * @previous_origin_x:
 * @previous_origin_y:
 *
 * Emits the size-changed-detailed signal that is typically used to adjust the
 * position of the image in the display shell on various operations,
 * e.g. crop.
 *
 * This function makes sure that GimpViewable::size-changed is also emitted.
 **/

void gimp_image_size_changed_detailed(GimpImage *image,gint previous_origin_x,gint previous_origin_y,gint previous_width,gint previous_height)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  g_signal_emit(image,gimp_image_signals[SIZE_CHANGED_DETAILED],0,previous_origin_x,previous_origin_y,previous_width,previous_height);
}

void gimp_image_colormap_changed(GimpImage *image,gint color_index)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (color_index >= - 1 && color_index < ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> n_colors) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"color_index >= -1 && color_index < GIMP_IMAGE_GET_PRIVATE (image)->n_colors");
      return ;
    }
    ;
  }while (0);
  g_signal_emit(image,gimp_image_signals[COLORMAP_CHANGED],0,color_index);
}

void gimp_image_selection_invalidate(GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  g_signal_emit(image,gimp_image_signals[SELECTION_INVALIDATE],0);
}

void gimp_image_quick_mask_changed(GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  g_signal_emit(image,gimp_image_signals[QUICK_MASK_CHANGED],0);
}

void gimp_image_undo_event(GimpImage *image,GimpUndoEvent event,GimpUndo *undo)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if ((event == GIMP_UNDO_EVENT_UNDO_FREE || event == GIMP_UNDO_EVENT_UNDO_FREEZE || event == GIMP_UNDO_EVENT_UNDO_THAW) && undo == ((void *)0) || (({
      GTypeInstance *__inst = (GTypeInstance *)undo;
      GType __t = gimp_undo_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"((event == GIMP_UNDO_EVENT_UNDO_FREE || event == GIMP_UNDO_EVENT_UNDO_FREEZE || event == GIMP_UNDO_EVENT_UNDO_THAW) && undo == NULL) || GIMP_IS_UNDO (undo)");
      return ;
    }
    ;
  }while (0);
  g_signal_emit(image,gimp_image_signals[UNDO_EVENT],0,event,undo);
}
/*  dirty counters  */
/* NOTE about the image->dirty counter:
 *   If 0, then the image is clean (ie, copy on disk is the same as the one
 *      in memory).
 *   If positive, then that's the number of dirtying operations done
 *       on the image since the last save.
 *   If negative, then user has hit undo and gone back in time prior
 *       to the saved copy.  Hitting redo will eventually come back to
 *       the saved copy.
 *
 *   The image is dirty (ie, needs saving) if counter is non-zero.
 *
 *   If the counter is around 100000, this is due to undo-ing back
 *   before a saved version, then changing the image (thus destroying
 *   the redo stack).  Once this has happened, it's impossible to get
 *   the image back to the state on disk, since the redo info has been
 *   freed.  See gimpimage-undo.c for the gory details.
 */
/*
 * NEVER CALL gimp_image_dirty() directly!
 *
 * If your code has just dirtied the image, push an undo instead.
 * Failing that, push the trivial undo which tells the user the
 * command is not undoable: undo_push_cantundo() (But really, it would
 * be best to push a proper undo).  If you just dirty the image
 * without pushing an undo then the dirty count is increased, but
 * popping that many undo actions won't lead to a clean image.
 */

gint gimp_image_dirty(GimpImage *image,GimpDirtyMask dirty_mask)
{
  GimpImagePrivate *private;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  private -> dirty++;
  private -> export_dirty++;
  if (!private -> dirty_time) {
    private -> dirty_time = (time(((void *)0)));
  }
  g_signal_emit(image,gimp_image_signals[DIRTY],0,dirty_mask);
  ;
  return private -> dirty;
}

gint gimp_image_clean(GimpImage *image,GimpDirtyMask dirty_mask)
{
  GimpImagePrivate *private;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  private -> dirty--;
  private -> export_dirty--;
  g_signal_emit(image,gimp_image_signals[CLEAN],0,dirty_mask);
  ;
  return private -> dirty;
}

void gimp_image_clean_all(GimpImage *image)
{
  GimpImagePrivate *private;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  private -> dirty = 0;
  private -> dirty_time = 0;
  g_signal_emit(image,gimp_image_signals[CLEAN],0,GIMP_DIRTY_ALL);
}

void gimp_image_export_clean_all(GimpImage *image)
{
  GimpImagePrivate *private;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  private -> export_dirty = 0;
  g_signal_emit(image,gimp_image_signals[CLEAN],0,GIMP_DIRTY_ALL);
}
/**
 * gimp_image_is_dirty:
 * @image:
 *
 * Returns: True if the image is dirty, false otherwise.
 **/

gint gimp_image_is_dirty(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> dirty != 0;
}
/**
 * gimp_image_is_export_dirty:
 * @image:
 *
 * Returns: True if the image export is dirty, false otherwise.
 **/

gboolean gimp_image_is_export_dirty(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> export_dirty != 0;
}

gint gimp_image_get_dirty_time(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  return (((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> dirty_time);
}
/**
 * gimp_image_saved:
 * @image:
 * @uri:
 *
 * Emits the "saved" signal, indicating that @image was saved to the
 * location specified by @uri.
 */

void gimp_image_saved(GimpImage *image,const gchar *uri)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (uri != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"uri != NULL");
      return ;
    }
    ;
  }while (0);
  g_signal_emit(image,gimp_image_signals[SAVED],0,uri);
}
/**
 * gimp_image_exported:
 * @image:
 * @uri:
 *
 * Emits the "exported" signal, indicating that @image was exported to the
 * location specified by @uri.
 */

void gimp_image_exported(GimpImage *image,const gchar *uri)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (uri != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"uri != NULL");
      return ;
    }
    ;
  }while (0);
  g_signal_emit(image,gimp_image_signals[EXPORTED],0,uri);
}
/*  flush this image's displays  */

void gimp_image_flush(GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  gimp_projectable_flush(((GimpProjectable *)(g_type_check_instance_cast(((GTypeInstance *)image),gimp_projectable_interface_get_type()))),((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> flush_accum . preview_invalidated);
}
/*  display / instance counters  */

gint gimp_image_get_display_count(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> disp_count;
}

void gimp_image_inc_display_count(GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> disp_count++;
}

void gimp_image_dec_display_count(GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> disp_count--;
}

gint gimp_image_get_instance_count(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> instance_count;
}

void gimp_image_inc_instance_count(GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> instance_count++;
}
/*  color transforms / utilities  */

void gimp_image_get_foreground(const GimpImage *image,GimpContext *context,GimpImageType dest_type,guchar *fg)
{
  GimpRGB color;
  guchar pfg[3];
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_CONTEXT (context)");
      return ;
    }
    ;
  }while (0);
  do {
    if (fg != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"fg != NULL");
      return ;
    }
    ;
  }while (0);
  gimp_context_get_foreground(context,&color);
  gimp_rgb_get_uchar((&color),&pfg[0],&pfg[1],&pfg[2]);
  gimp_image_transform_color(image,dest_type,fg,GIMP_RGB,pfg);
}

void gimp_image_get_background(const GimpImage *image,GimpContext *context,GimpImageType dest_type,guchar *bg)
{
  GimpRGB color;
  guchar pbg[3];
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_CONTEXT (context)");
      return ;
    }
    ;
  }while (0);
  do {
    if (bg != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"bg != NULL");
      return ;
    }
    ;
  }while (0);
  gimp_context_get_background(context,&color);
  gimp_rgb_get_uchar((&color),&pbg[0],&pbg[1],&pbg[2]);
  gimp_image_transform_color(image,dest_type,bg,GIMP_RGB,pbg);
}

void gimp_image_get_color(const GimpImage *src_image,GimpImageType src_type,const guchar *src,guchar *rgba)
{
  gboolean has_alpha = 0;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)src_image;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (src_image)");
      return ;
    }
    ;
  }while (0);
  switch(src_type){
    case GIMP_RGBA_IMAGE:
    has_alpha = !0;
    case GIMP_RGB_IMAGE:
{
/*  Straight copy  */
       *(rgba++) =  *(src++);
       *(rgba++) =  *(src++);
       *(rgba++) =  *(src++);
      break; 
    }
    case GIMP_GRAYA_IMAGE:
    has_alpha = !0;
    case GIMP_GRAY_IMAGE:
{
/*  Gray to RG&B */
       *(rgba++) =  *src;
       *(rgba++) =  *src;
       *(rgba++) =  *(src++);
      break; 
    }
    case GIMP_INDEXEDA_IMAGE:
    has_alpha = !0;
    case GIMP_INDEXED_IMAGE:
{
/*  Indexed palette lookup  */
{
        GimpImagePrivate *src_private = (GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)src_image),gimp_image_get_type()));
        gint index = ( *(src++)) * 3;
         *(rgba++) = src_private -> colormap[index++];
         *(rgba++) = src_private -> colormap[index++];
         *(rgba++) = src_private -> colormap[index++];
      }
      break; 
    }
  }
  if (has_alpha) {
     *rgba =  *src;
  }
  else {
     *rgba = 255;
  }
}

void gimp_image_transform_rgb(const GimpImage *dest_image,GimpImageType dest_type,const GimpRGB *rgb,guchar *color)
{
  guchar col[3];
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)dest_image;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (dest_image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (rgb != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"rgb != NULL");
      return ;
    }
    ;
  }while (0);
  do {
    if (color != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"color != NULL");
      return ;
    }
    ;
  }while (0);
  gimp_rgb_get_uchar(rgb,&col[0],&col[1],&col[2]);
  gimp_image_transform_color(dest_image,dest_type,color,GIMP_RGB,col);
}

void gimp_image_transform_color(const GimpImage *dest_image,GimpImageType dest_type,guchar *dest,GimpImageBaseType src_type,const guchar *src)
{
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)dest_image;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (dest_image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (src_type != GIMP_INDEXED) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"src_type != GIMP_INDEXED");
      return ;
    }
    ;
  }while (0);
  switch(src_type){
    case GIMP_RGB:
{
      switch(dest_type){
        case GIMP_RGB_IMAGE:
{
        }
        case GIMP_RGBA_IMAGE:
{
/*  Straight copy  */
           *(dest++) =  *(src++);
           *(dest++) =  *(src++);
           *(dest++) =  *(src++);
          break; 
        }
        case GIMP_GRAY_IMAGE:
{
        }
        case GIMP_GRAYA_IMAGE:
{
/*  NTSC conversion  */
           *dest = (src[0] * 0.2126 + src[1] * 0.7152 + src[2] * 0.0722 + 0.5);
          break; 
        }
        case GIMP_INDEXED_IMAGE:
{
        }
        case GIMP_INDEXEDA_IMAGE:
{
/*  Least squares method  */
           *dest = (gimp_image_color_hash_rgb_to_indexed(dest_image,src[0],src[1],src[2]));
          break; 
        }
      }
      break; 
    }
    case GIMP_GRAY:
{
      switch(dest_type){
        case GIMP_RGB_IMAGE:
{
        }
        case GIMP_RGBA_IMAGE:
{
/*  Gray to RG&B */
           *(dest++) =  *src;
           *(dest++) =  *src;
           *(dest++) =  *src;
          break; 
        }
        case GIMP_GRAY_IMAGE:
{
        }
        case GIMP_GRAYA_IMAGE:
{
/*  Straight copy  */
           *dest =  *src;
          break; 
        }
        case GIMP_INDEXED_IMAGE:
{
        }
        case GIMP_INDEXEDA_IMAGE:
{
/*  Least squares method  */
           *dest = (gimp_image_color_hash_rgb_to_indexed(dest_image,src[0],src[0],src[0]));
          break; 
        }
      }
      break; 
    }
    default:
    break; 
  }
}

TempBuf *gimp_image_transform_temp_buf(const GimpImage *dest_image,GimpImageType dest_type,TempBuf *temp_buf,gboolean *new_buf)
{
  TempBuf *ret_buf;
  GimpImageType ret_buf_type;
  gboolean has_alpha;
  gboolean is_rgb;
  gint in_bytes;
  gint out_bytes;
  do {
    if ((GimpImage *)(g_type_check_instance_cast(((GTypeInstance *)dest_image),gimp_image_get_type()))) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IMAGE (dest_image)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (temp_buf != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"temp_buf != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (new_buf != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"new_buf != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  in_bytes = temp_buf -> bytes;
  has_alpha = in_bytes == 2 || in_bytes == 4;
  is_rgb = in_bytes == 3 || in_bytes == 4;
  if (has_alpha) {
    ret_buf_type = ((dest_type == GIMP_RGB_IMAGE || dest_type == GIMP_RGBA_IMAGE?GIMP_RGBA_IMAGE : ((dest_type == GIMP_GRAY_IMAGE || dest_type == GIMP_GRAYA_IMAGE?GIMP_GRAYA_IMAGE : ((dest_type == GIMP_INDEXED_IMAGE || dest_type == GIMP_INDEXEDA_IMAGE?GIMP_INDEXEDA_IMAGE : - 1))))));
  }
  else {
    ret_buf_type = ((dest_type == GIMP_RGB_IMAGE || dest_type == GIMP_RGBA_IMAGE?GIMP_RGB_IMAGE : ((dest_type == GIMP_GRAY_IMAGE || dest_type == GIMP_GRAYA_IMAGE?GIMP_GRAY_IMAGE : ((dest_type == GIMP_INDEXED_IMAGE || dest_type == GIMP_INDEXEDA_IMAGE?GIMP_INDEXED_IMAGE : - 1))))));
  }
  out_bytes = (ret_buf_type == GIMP_RGBA_IMAGE?4 : ((ret_buf_type == GIMP_RGB_IMAGE?3 : ((ret_buf_type == GIMP_GRAYA_IMAGE?2 : ((ret_buf_type == GIMP_GRAY_IMAGE?1 : ((ret_buf_type == GIMP_INDEXEDA_IMAGE?2 : ((ret_buf_type == GIMP_INDEXED_IMAGE?1 : - 1)))))))))));
/*  If the pattern doesn't match the image in terms of color type,
   *  transform it.  (ie  pattern is RGB, image is indexed)
   */
  if (in_bytes != out_bytes || (dest_type == GIMP_INDEXED_IMAGE || dest_type == GIMP_INDEXEDA_IMAGE)) {
    guchar *src;
    guchar *dest;
    gint size;
    ret_buf = temp_buf_new(temp_buf -> width,temp_buf -> height,out_bytes,0,0,((void *)0));
    src = temp_buf_get_data(temp_buf);
    dest = temp_buf_get_data(ret_buf);
    size = temp_buf -> width * temp_buf -> height;
    while(size--){
      gimp_image_transform_color(dest_image,dest_type,dest,(is_rgb?GIMP_RGB : GIMP_GRAY),src);
/* Handle alpha */
      if (has_alpha) {
        dest[out_bytes - 1] = src[in_bytes - 1];
      }
      src += in_bytes;
      dest += out_bytes;
    }
     *new_buf = !0;
  }
  else {
    ret_buf = temp_buf;
     *new_buf = 0;
  }
  return ret_buf;
}
/*  parasites  */

const GimpParasite *gimp_image_parasite_find(const GimpImage *image,const gchar *name)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  return gimp_parasite_list_find(((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> parasites,name);
}

static void list_func(gchar *key,GimpParasite *p,gchar ***cur)
{
   *(( *cur)++) = ((gchar *)(g_strdup(key)));
}

gchar **gimp_image_parasite_list(const GimpImage *image,gint *count)
{
  GimpImagePrivate *private;
  gchar **list;
  gchar **cur;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
   *count = gimp_parasite_list_length(private -> parasites);
  cur = list = ((gchar **)(g_malloc_n(( *count),sizeof(gchar *))));
  gimp_parasite_list_foreach(private -> parasites,((GHFunc )list_func),(&cur));
  return list;
}

void gimp_image_parasite_attach(GimpImage *image,const GimpParasite *parasite)
{
  GimpParasite copy;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (parasite != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"parasite != NULL");
      return ;
    }
    ;
  }while (0);
/*  make a temporary copy of the GimpParasite struct because
   *  gimp_parasite_shift_parent() changes it
   */
  copy =  *parasite;
/*  only set the dirty bit manually if we can be saved and the new
   *  parasite differs from the current one and we aren't undoable
   */
  if (gimp_parasite_is_undoable((&copy))) {
    gimp_image_undo_push_image_parasite(image,g_dpgettext(((void *)0),"undo-type\004Attach Parasite to Image",strlen("undo-type") + 1),(&copy));
  }
/*  We used to push an cantundo on te stack here. This made the undo stack
   *  unusable (NULL on the stack) and prevented people from undoing after a
   *  save (since most save plug-ins attach an undoable comment parasite).
   *  Now we simply attach the parasite without pushing an undo. That way
   *  it's undoable but does not block the undo system.   --Sven
   */
  gimp_parasite_list_add(((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> parasites,(&copy));
  if (gimp_parasite_has_flag((&copy),(0x80 << 8))) {
    gimp_parasite_shift_parent(&copy);
    gimp_parasite_attach(image -> gimp,(&copy));
  }
  g_signal_emit(image,gimp_image_signals[PARASITE_ATTACHED],0,parasite -> name);
  if (strcmp((parasite -> name),"icc-profile") == 0) {
    gimp_color_managed_profile_changed(((GimpColorManaged *)(g_type_check_instance_cast(((GTypeInstance *)image),gimp_color_managed_interface_get_type()))));
  }
}

void gimp_image_parasite_detach(GimpImage *image,const gchar *name)
{
  GimpImagePrivate *private;
  const GimpParasite *parasite;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (name != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"name != NULL");
      return ;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  if (!(parasite = gimp_parasite_list_find(private -> parasites,name))) {
    return ;
  }
  if (gimp_parasite_is_undoable(parasite)) {
    gimp_image_undo_push_image_parasite_remove(image,g_dpgettext(((void *)0),"undo-type\004Remove Parasite from Image",strlen("undo-type") + 1),name);
  }
  gimp_parasite_list_remove(private -> parasites,name);
  g_signal_emit(image,gimp_image_signals[PARASITE_DETACHED],0,name);
  if (strcmp(name,"icc-profile") == 0) {
    gimp_color_managed_profile_changed(((GimpColorManaged *)(g_type_check_instance_cast(((GTypeInstance *)image),gimp_color_managed_interface_get_type()))));
  }
}
/*  tattoos  */

GimpTattoo gimp_image_get_new_tattoo(GimpImage *image)
{
  GimpImagePrivate *private;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  private -> tattoo_state++;
  if (private -> tattoo_state == 0) {
    g_log("Gimp-Core",G_LOG_LEVEL_WARNING,"%s: Tattoo state corrupted (integer overflow).",((const char *)__func__));
  }
  return private -> tattoo_state;
}

GimpTattoo gimp_image_get_tattoo_state(GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> tattoo_state;
}

gboolean gimp_image_set_tattoo_state(GimpImage *image,GimpTattoo val)
{
  GList *all_items;
  GList *list;
  gboolean retval = !0;
  GimpTattoo maxval = 0;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
/* Check that the layer tattoos don't overlap with channel or vector ones */
  all_items = gimp_image_get_layer_list(image);
  for (list = all_items; list; list = (list?((GList *)list) -> next : ((void *)0))) {
    GimpTattoo ltattoo;
    ltattoo = gimp_item_get_tattoo(((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)(list -> data)),gimp_item_get_type()))));
    if (ltattoo > maxval) {
      maxval = ltattoo;
    }
    if (gimp_image_get_channel_by_tattoo(image,ltattoo)) {
/* Oopps duplicated tattoo in channel */
      retval = 0;
    }
    if (gimp_image_get_vectors_by_tattoo(image,ltattoo)) {
/* Oopps duplicated tattoo in vectors */
      retval = 0;
    }
  }
  g_list_free(all_items);
/* Now check that the channel and vectors tattoos don't overlap */
  all_items = gimp_image_get_channel_list(image);
  for (list = all_items; list; list = (list?((GList *)list) -> next : ((void *)0))) {
    GimpTattoo ctattoo;
    ctattoo = gimp_item_get_tattoo(((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)(list -> data)),gimp_item_get_type()))));
    if (ctattoo > maxval) {
      maxval = ctattoo;
    }
    if (gimp_image_get_vectors_by_tattoo(image,ctattoo)) {
/* Oopps duplicated tattoo in vectors */
      retval = 0;
    }
  }
  g_list_free(all_items);
/* Find the max tattoo value in the vectors */
  all_items = gimp_image_get_vectors_list(image);
  for (list = all_items; list; list = (list?((GList *)list) -> next : ((void *)0))) {
    GimpTattoo vtattoo;
    vtattoo = gimp_item_get_tattoo(((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)(list -> data)),gimp_item_get_type()))));
    if (vtattoo > maxval) {
      maxval = vtattoo;
    }
  }
  g_list_free(all_items);
  if (val < maxval) {
    retval = 0;
  }
/* Must check if the state is valid */
  if (retval == !0) {
    ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> tattoo_state = val;
  }
  return retval;
}
/*  projection  */

GimpProjection *gimp_image_get_projection(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> projection;
}
/*  layers / channels / vectors  */

GimpItemTree *gimp_image_get_layer_tree(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> layers;
}

GimpItemTree *gimp_image_get_channel_tree(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> channels;
}

GimpItemTree *gimp_image_get_vectors_tree(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> vectors;
}

GimpContainer *gimp_image_get_layers(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> layers -> container;
}

GimpContainer *gimp_image_get_channels(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> channels -> container;
}

GimpContainer *gimp_image_get_vectors(const GimpImage *image)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  return ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type()))) -> vectors -> container;
}

gint gimp_image_get_n_layers(const GimpImage *image)
{
  GimpItemStack *stack;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  stack = ((GimpItemStack *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_image_get_layers(image))),gimp_item_stack_get_type())));
  return gimp_item_stack_get_n_items(stack);
}

gint gimp_image_get_n_channels(const GimpImage *image)
{
  GimpItemStack *stack;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  stack = ((GimpItemStack *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_image_get_channels(image))),gimp_item_stack_get_type())));
  return gimp_item_stack_get_n_items(stack);
}

gint gimp_image_get_n_vectors(const GimpImage *image)
{
  GimpItemStack *stack;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  stack = ((GimpItemStack *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_image_get_vectors(image))),gimp_item_stack_get_type())));
  return gimp_item_stack_get_n_items(stack);
}

GList *gimp_image_get_layer_iter(const GimpImage *image)
{
  GimpItemStack *stack;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  stack = ((GimpItemStack *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_image_get_layers(image))),gimp_item_stack_get_type())));
  return gimp_item_stack_get_item_iter(stack);
}

GList *gimp_image_get_channel_iter(const GimpImage *image)
{
  GimpItemStack *stack;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  stack = ((GimpItemStack *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_image_get_channels(image))),gimp_item_stack_get_type())));
  return gimp_item_stack_get_item_iter(stack);
}

GList *gimp_image_get_vectors_iter(const GimpImage *image)
{
  GimpItemStack *stack;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  stack = ((GimpItemStack *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_image_get_vectors(image))),gimp_item_stack_get_type())));
  return gimp_item_stack_get_item_iter(stack);
}

GList *gimp_image_get_layer_list(const GimpImage *image)
{
  GimpItemStack *stack;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  stack = ((GimpItemStack *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_image_get_layers(image))),gimp_item_stack_get_type())));
  return gimp_item_stack_get_item_list(stack);
}

GList *gimp_image_get_channel_list(const GimpImage *image)
{
  GimpItemStack *stack;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  stack = ((GimpItemStack *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_image_get_channels(image))),gimp_item_stack_get_type())));
  return gimp_item_stack_get_item_list(stack);
}

GList *gimp_image_get_vectors_list(const GimpImage *image)
{
  GimpItemStack *stack;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  stack = ((GimpItemStack *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_image_get_vectors(image))),gimp_item_stack_get_type())));
  return gimp_item_stack_get_item_list(stack);
}
/*  active drawable, layer, channel, vectors  */

GimpDrawable *gimp_image_get_active_drawable(const GimpImage *image)
{
  GimpImagePrivate *private;
  GimpItem *active_channel;
  GimpItem *active_layer;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  active_channel = gimp_item_tree_get_active_item(private -> channels);
  active_layer = gimp_item_tree_get_active_item(private -> layers);
/*  If there is an active channel (a saved selection, etc.),
   *  we ignore the active layer
   */
  if (active_channel) {
    return (GimpDrawable *)(g_type_check_instance_cast(((GTypeInstance *)active_channel),gimp_drawable_get_type()));
  }
  else {
    if (active_layer) {
      GimpLayer *layer = (GimpLayer *)(g_type_check_instance_cast(((GTypeInstance *)active_layer),gimp_layer_get_type()));
      GimpLayerMask *mask = gimp_layer_get_mask(layer);
      if (mask && gimp_layer_mask_get_edit(mask)) {
        return (GimpDrawable *)(g_type_check_instance_cast(((GTypeInstance *)mask),gimp_drawable_get_type()));
      }
      else {
        return (GimpDrawable *)(g_type_check_instance_cast(((GTypeInstance *)layer),gimp_drawable_get_type()));
      }
    }
  }
  return ((void *)0);
}

GimpLayer *gimp_image_get_active_layer(const GimpImage *image)
{
  GimpImagePrivate *private;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  return (GimpLayer *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_item_tree_get_active_item(private -> layers))),gimp_layer_get_type()));
}

GimpChannel *gimp_image_get_active_channel(const GimpImage *image)
{
  GimpImagePrivate *private;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  return (GimpChannel *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_item_tree_get_active_item(private -> channels))),gimp_channel_get_type()));
}

GimpVectors *gimp_image_get_active_vectors(const GimpImage *image)
{
  GimpImagePrivate *private;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  return (GimpVectors *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_item_tree_get_active_item(private -> vectors))),gimp_vectors_get_type()));
}

GimpLayer *gimp_image_set_active_layer(GimpImage *image,GimpLayer *layer)
{
  GimpImagePrivate *private;
  GimpLayer *floating_sel;
  GimpLayer *active_layer;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (layer == ((void *)0) || (({
      GTypeInstance *__inst = (GTypeInstance *)layer;
      GType __t = gimp_layer_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"layer == NULL || GIMP_IS_LAYER (layer)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (layer == ((void *)0) || gimp_item_is_attached(((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)layer),gimp_item_get_type())))) && gimp_item_get_image(((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)layer),gimp_item_get_type())))) == image) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"layer == NULL || (gimp_item_is_attached (GIMP_ITEM (layer)) && gimp_item_get_image (GIMP_ITEM (layer)) == image)");
      return ((void *)0);
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  floating_sel = gimp_image_get_floating_selection(image);
/*  Make sure the floating_sel always is the active layer  */
  if (floating_sel && layer != floating_sel) {
    return floating_sel;
  }
  active_layer = gimp_image_get_active_layer(image);
  if (layer != active_layer) {
/*  Don't cache selection info for the previous active layer  */
    if (active_layer) {
      gimp_drawable_invalidate_boundary(((GimpDrawable *)(g_type_check_instance_cast(((GTypeInstance *)active_layer),gimp_drawable_get_type()))));
    }
    gimp_item_tree_set_active_item(private -> layers,((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)layer),gimp_item_get_type()))));
  }
  return gimp_image_get_active_layer(image);
}

GimpChannel *gimp_image_set_active_channel(GimpImage *image,GimpChannel *channel)
{
  GimpImagePrivate *private;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (channel == ((void *)0) || (({
      GTypeInstance *__inst = (GTypeInstance *)channel;
      GType __t = gimp_channel_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"channel == NULL || GIMP_IS_CHANNEL (channel)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (channel == ((void *)0) || gimp_item_is_attached(((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)channel),gimp_item_get_type())))) && gimp_item_get_image(((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)channel),gimp_item_get_type())))) == image) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"channel == NULL || (gimp_item_is_attached (GIMP_ITEM (channel)) && gimp_item_get_image (GIMP_ITEM (channel)) == image)");
      return ((void *)0);
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
/*  Not if there is a floating selection  */
  if (channel && gimp_image_get_floating_selection(image)) {
    return ((void *)0);
  }
  if (channel != gimp_image_get_active_channel(image)) {
    gimp_item_tree_set_active_item(private -> channels,((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)channel),gimp_item_get_type()))));
  }
  return gimp_image_get_active_channel(image);
}

GimpChannel *gimp_image_unset_active_channel(GimpImage *image)
{
  GimpImagePrivate *private;
  GimpChannel *channel;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  channel = gimp_image_get_active_channel(image);
  if (channel) {
    gimp_image_set_active_channel(image,((void *)0));
    if (private -> layer_stack) {
      gimp_image_set_active_layer(image,(private -> layer_stack -> data));
    }
  }
  return channel;
}

GimpVectors *gimp_image_set_active_vectors(GimpImage *image,GimpVectors *vectors)
{
  GimpImagePrivate *private;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (vectors == ((void *)0) || (({
      GTypeInstance *__inst = (GTypeInstance *)vectors;
      GType __t = gimp_vectors_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"vectors == NULL || GIMP_IS_VECTORS (vectors)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (vectors == ((void *)0) || gimp_item_is_attached(((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)vectors),gimp_item_get_type())))) && gimp_item_get_image(((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)vectors),gimp_item_get_type())))) == image) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"vectors == NULL || (gimp_item_is_attached (GIMP_ITEM (vectors)) && gimp_item_get_image (GIMP_ITEM (vectors)) == image)");
      return ((void *)0);
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  if (vectors != gimp_image_get_active_vectors(image)) {
    gimp_item_tree_set_active_item(private -> vectors,((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)vectors),gimp_item_get_type()))));
  }
  return gimp_image_get_active_vectors(image);
}
/*  layer, channel, vectors by tattoo  */

GimpLayer *gimp_image_get_layer_by_tattoo(const GimpImage *image,GimpTattoo tattoo)
{
  GimpItemStack *stack;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  stack = ((GimpItemStack *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_image_get_layers(image))),gimp_item_stack_get_type())));
  return (GimpLayer *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_item_stack_get_item_by_tattoo(stack,tattoo))),gimp_layer_get_type()));
}

GimpChannel *gimp_image_get_channel_by_tattoo(const GimpImage *image,GimpTattoo tattoo)
{
  GimpItemStack *stack;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  stack = ((GimpItemStack *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_image_get_channels(image))),gimp_item_stack_get_type())));
  return (GimpChannel *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_item_stack_get_item_by_tattoo(stack,tattoo))),gimp_channel_get_type()));
}

GimpVectors *gimp_image_get_vectors_by_tattoo(const GimpImage *image,GimpTattoo tattoo)
{
  GimpItemStack *stack;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  stack = ((GimpItemStack *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_image_get_vectors(image))),gimp_item_stack_get_type())));
  return (GimpVectors *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_item_stack_get_item_by_tattoo(stack,tattoo))),gimp_vectors_get_type()));
}
/*  layer, channel, vectors by name  */

GimpLayer *gimp_image_get_layer_by_name(const GimpImage *image,const gchar *name)
{
  GimpItemTree *tree;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (name != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"name != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  tree = gimp_image_get_layer_tree(image);
  return (GimpLayer *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_item_tree_get_item_by_name(tree,name))),gimp_layer_get_type()));
}

GimpChannel *gimp_image_get_channel_by_name(const GimpImage *image,const gchar *name)
{
  GimpItemTree *tree;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (name != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"name != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  tree = gimp_image_get_channel_tree(image);
  return (GimpChannel *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_item_tree_get_item_by_name(tree,name))),gimp_channel_get_type()));
}

GimpVectors *gimp_image_get_vectors_by_name(const GimpImage *image,const gchar *name)
{
  GimpItemTree *tree;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (name != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"name != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  tree = gimp_image_get_vectors_tree(image);
  return (GimpVectors *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_item_tree_get_item_by_name(tree,name))),gimp_vectors_get_type()));
}
/*  items  */

gboolean gimp_image_reorder_item(GimpImage *image,GimpItem *item,GimpItem *new_parent,gint new_index,gboolean push_undo,const gchar *undo_desc)
{
  GimpItemTree *tree;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)item;
      GType __t = gimp_item_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_ITEM (item)");
      return 0;
    }
    ;
  }while (0);
  do {
    if (gimp_item_get_image(item) == image) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"gimp_item_get_image (item) == image");
      return 0;
    }
    ;
  }while (0);
  tree = gimp_item_get_tree(item);
  do {
    if (tree != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"tree != NULL");
      return 0;
    }
    ;
  }while (0);
  if (push_undo && !undo_desc) {
    undo_desc = ((GimpItemClass *)(((GTypeInstance *)item) -> g_class)) -> reorder_desc;
  }
/*  item and new_parent are type-checked in GimpItemTree
   */
  return gimp_item_tree_reorder_item(tree,item,new_parent,new_index,push_undo,undo_desc);
}

gboolean gimp_image_raise_item(GimpImage *image,GimpItem *item,GError **error)
{
  gint index;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)item;
      GType __t = gimp_item_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_ITEM (item)");
      return 0;
    }
    ;
  }while (0);
  do {
    if (error == ((void *)0) ||  *error == ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"error == NULL || *error == NULL");
      return 0;
    }
    ;
  }while (0);
  index = gimp_item_get_index(item);
  do {
    if (index != - 1) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"index != -1");
      return 0;
    }
    ;
  }while (0);
  if (index == 0) {
    g_set_error_literal(error,gimp_error_quark(),GIMP_FAILED,((GimpItemClass *)(((GTypeInstance *)item) -> g_class)) -> raise_failed);
    return 0;
  }
  return gimp_image_reorder_item(image,item,gimp_item_get_parent(item),index - 1,!0,((GimpItemClass *)(((GTypeInstance *)item) -> g_class)) -> raise_desc);
}

gboolean gimp_image_raise_item_to_top(GimpImage *image,GimpItem *item)
{
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)item;
      GType __t = gimp_item_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_ITEM (item)");
      return 0;
    }
    ;
  }while (0);
  return gimp_image_reorder_item(image,item,gimp_item_get_parent(item),0,!0,((GimpItemClass *)(((GTypeInstance *)item) -> g_class)) -> raise_to_top_desc);
}

gboolean gimp_image_lower_item(GimpImage *image,GimpItem *item,GError **error)
{
  GimpContainer *container;
  gint index;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)item;
      GType __t = gimp_item_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_ITEM (item)");
      return 0;
    }
    ;
  }while (0);
  do {
    if (error == ((void *)0) ||  *error == ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"error == NULL || *error == NULL");
      return 0;
    }
    ;
  }while (0);
  container = gimp_item_get_container(item);
  do {
    if (container != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"container != NULL");
      return 0;
    }
    ;
  }while (0);
  index = gimp_item_get_index(item);
  if (index == gimp_container_get_n_children(container) - 1) {
    g_set_error_literal(error,gimp_error_quark(),GIMP_FAILED,((GimpItemClass *)(((GTypeInstance *)item) -> g_class)) -> lower_failed);
    return 0;
  }
  return gimp_image_reorder_item(image,item,gimp_item_get_parent(item),index + 1,!0,((GimpItemClass *)(((GTypeInstance *)item) -> g_class)) -> lower_desc);
}

gboolean gimp_image_lower_item_to_bottom(GimpImage *image,GimpItem *item)
{
  GimpContainer *container;
  gint length;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)item;
      GType __t = gimp_item_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_ITEM (item)");
      return 0;
    }
    ;
  }while (0);
  container = gimp_item_get_container(item);
  do {
    if (container != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"container != NULL");
      return 0;
    }
    ;
  }while (0);
  length = gimp_container_get_n_children(container);
  return gimp_image_reorder_item(image,item,gimp_item_get_parent(item),length - 1,!0,((GimpItemClass *)(((GTypeInstance *)item) -> g_class)) -> lower_to_bottom_desc);
}
/*  layers  */

gboolean gimp_image_add_layer(GimpImage *image,GimpLayer *layer,GimpLayer *parent,gint position,gboolean push_undo)
{
  GimpImagePrivate *private;
  gboolean old_has_alpha;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
/*  item and parent are type-checked in GimpItemTree
   */
  if (!gimp_item_tree_get_insert_pos(private -> layers,((GimpItem *)layer),((GimpItem **)(&parent)),&position)) {
    return 0;
  }
/*  If there is a floating selection (and this isn't it!),
   *  make sure the insert position is greater than 0
   */
  if (parent == ((void *)0) && position == 0 && gimp_image_get_floating_selection(image)) {
    position = 1;
  }
  old_has_alpha = gimp_image_has_alpha(image);
  if (push_undo) {
    gimp_image_undo_push_layer_add(image,g_dpgettext(((void *)0),"undo-type\004Add Layer",strlen("undo-type") + 1),layer,gimp_image_get_active_layer(image));
  }
  gimp_item_tree_add_item(private -> layers,((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)layer),gimp_item_get_type()))),((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)parent),gimp_item_get_type()))),position);
  gimp_image_set_active_layer(image,layer);
/*  If the layer is a floating selection, attach it to the drawable  */
  if (gimp_layer_is_floating_sel(layer)) {
    gimp_drawable_attach_floating_sel(gimp_layer_get_floating_sel_drawable(layer),layer);
  }
  if (old_has_alpha != gimp_image_has_alpha(image)) {
    private -> flush_accum . alpha_changed = !0;
  }
  return !0;
}

void gimp_image_remove_layer(GimpImage *image,GimpLayer *layer,gboolean push_undo,GimpLayer *new_active)
{
  GimpImagePrivate *private;
  GimpLayer *active_layer;
  gboolean old_has_alpha;
  gboolean undo_group = 0;
  const gchar *undo_desc;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)layer;
      GType __t = gimp_layer_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_LAYER (layer)");
      return ;
    }
    ;
  }while (0);
  do {
    if (gimp_item_is_attached(((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)layer),gimp_item_get_type()))))) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"gimp_item_is_attached (GIMP_ITEM (layer))");
      return ;
    }
    ;
  }while (0);
  do {
    if (gimp_item_get_image(((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)layer),gimp_item_get_type())))) == image) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"gimp_item_get_image (GIMP_ITEM (layer)) == image");
      return ;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  if (gimp_drawable_get_floating_sel(((GimpDrawable *)(g_type_check_instance_cast(((GTypeInstance *)layer),gimp_drawable_get_type()))))) {
    if (!push_undo) {
      g_log("Gimp-Core",G_LOG_LEVEL_WARNING,"%s() was called from an undo function while the layer had a floating selection. Please report this at http://www.gimp.org/bugs/",((const char *)__func__));
      return ;
    }
    gimp_image_undo_group_start(image,GIMP_UNDO_GROUP_IMAGE_ITEM_REMOVE,g_dpgettext(((void *)0),"undo-type\004Remove Layer",strlen("undo-type") + 1));
    undo_group = !0;
    gimp_image_remove_layer(image,gimp_drawable_get_floating_sel(((GimpDrawable *)(g_type_check_instance_cast(((GTypeInstance *)layer),gimp_drawable_get_type())))),!0,((void *)0));
  }
  active_layer = gimp_image_get_active_layer(image);
  old_has_alpha = gimp_image_has_alpha(image);
  if (gimp_layer_is_floating_sel(layer)) {
    undo_desc = g_dpgettext(((void *)0),"undo-type\004Remove Floating Selection",strlen("undo-type") + 1);
    gimp_drawable_detach_floating_sel(gimp_layer_get_floating_sel_drawable(layer));
  }
  else {
    undo_desc = g_dpgettext(((void *)0),"undo-type\004Remove Layer",strlen("undo-type") + 1);
  }
  if (push_undo) {
    gimp_image_undo_push_layer_remove(image,undo_desc,layer,gimp_layer_get_parent(layer),gimp_item_get_index(((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)layer),gimp_item_get_type())))),active_layer);
  }
  g_object_ref(layer);
/*  Make sure we're not caching any old selection info  */
  if (layer == active_layer) {
    gimp_drawable_invalidate_boundary(((GimpDrawable *)(g_type_check_instance_cast(((GTypeInstance *)layer),gimp_drawable_get_type()))));
  }
  private -> layer_stack = g_slist_remove(private -> layer_stack,layer);
/*  Also remove all children of a group layer from the layer_stack  */
  if (gimp_viewable_get_children(((GimpViewable *)(g_type_check_instance_cast(((GTypeInstance *)layer),gimp_viewable_get_type()))))) {
    GimpContainer *stack = gimp_viewable_get_children(((GimpViewable *)(g_type_check_instance_cast(((GTypeInstance *)layer),gimp_viewable_get_type()))));
    GList *children;
    GList *list;
    children = gimp_item_stack_get_item_list(((GimpItemStack *)(g_type_check_instance_cast(((GTypeInstance *)stack),gimp_item_stack_get_type()))));
    for (list = children; list; list = (list?((GList *)list) -> next : ((void *)0))) {
      private -> layer_stack = g_slist_remove(private -> layer_stack,(list -> data));
    }
    g_list_free(children);
  }
  if (!new_active && private -> layer_stack) {
    new_active = (private -> layer_stack -> data);
  }
  new_active = ((GimpLayer *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_item_tree_remove_item(private -> layers,((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)layer),gimp_item_get_type()))),((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)new_active),gimp_item_get_type())))))),gimp_layer_get_type())));
  if (gimp_layer_is_floating_sel(layer)) {
/*  If this was the floating selection, activate the underlying drawable
       */
    floating_sel_activate_drawable(layer);
  }
  else {
    if (active_layer && (layer == active_layer || gimp_viewable_is_ancestor(((GimpViewable *)(g_type_check_instance_cast(((GTypeInstance *)layer),gimp_viewable_get_type()))),((GimpViewable *)(g_type_check_instance_cast(((GTypeInstance *)active_layer),gimp_viewable_get_type())))))) {
      gimp_image_set_active_layer(image,new_active);
    }
  }
  g_object_unref(layer);
  if (old_has_alpha != gimp_image_has_alpha(image)) {
    private -> flush_accum . alpha_changed = !0;
  }
  if (undo_group) {
    gimp_image_undo_group_end(image);
  }
}

void gimp_image_add_layers(GimpImage *image,GList *layers,GimpLayer *parent,gint position,gint x,gint y,gint width,gint height,const gchar *undo_desc)
{
  GimpImagePrivate *private;
  GList *list;
  gint layers_x = 2147483647;
  gint layers_y = 2147483647;
  gint layers_width = 0;
  gint layers_height = 0;
  gint offset_x;
  gint offset_y;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (layers != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"layers != NULL");
      return ;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
/*  item and parent are type-checked in GimpItemTree
   */
  if (!gimp_item_tree_get_insert_pos(private -> layers,((GimpItem *)(layers -> data)),((GimpItem **)(&parent)),&position)) {
    return ;
  }
  for (list = layers; list; list = (list?((GList *)list) -> next : ((void *)0))) {
    GimpItem *item = (GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)(list -> data)),gimp_item_get_type()));
    gint off_x;
    gint off_y;
    gimp_item_get_offset(item,&off_x,&off_y);
    layers_x = (layers_x < off_x?layers_x : off_x);
    layers_y = (layers_y < off_y?layers_y : off_y);
    layers_width = (layers_width > off_x + gimp_item_get_width(item) - layers_x?layers_width : off_x + gimp_item_get_width(item) - layers_x);
    layers_height = (layers_height > off_y + gimp_item_get_height(item) - layers_y?layers_height : off_y + gimp_item_get_height(item) - layers_y);
  }
  offset_x = x + (width - layers_width) / 2 - layers_x;
  offset_y = y + (height - layers_height) / 2 - layers_y;
  gimp_image_undo_group_start(image,GIMP_UNDO_GROUP_LAYER_ADD,undo_desc);
  for (list = layers; list; list = (list?((GList *)list) -> next : ((void *)0))) {
    GimpItem *new_item = (GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)(list -> data)),gimp_item_get_type()));
    gimp_item_translate(new_item,offset_x,offset_y,0);
    gimp_image_add_layer(image,((GimpLayer *)(g_type_check_instance_cast(((GTypeInstance *)new_item),gimp_layer_get_type()))),parent,position,!0);
    position++;
  }
  if (layers) {
    gimp_image_set_active_layer(image,(layers -> data));
  }
  gimp_image_undo_group_end(image);
}
/*  channels  */

gboolean gimp_image_add_channel(GimpImage *image,GimpChannel *channel,GimpChannel *parent,gint position,gboolean push_undo)
{
  GimpImagePrivate *private;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
/*  item and parent are type-checked in GimpItemTree
   */
  if (!gimp_item_tree_get_insert_pos(private -> channels,((GimpItem *)channel),((GimpItem **)(&parent)),&position)) {
    return 0;
  }
  if (push_undo) {
    gimp_image_undo_push_channel_add(image,g_dpgettext(((void *)0),"undo-type\004Add Channel",strlen("undo-type") + 1),channel,gimp_image_get_active_channel(image));
  }
  gimp_item_tree_add_item(private -> channels,((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)channel),gimp_item_get_type()))),((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)parent),gimp_item_get_type()))),position);
  gimp_image_set_active_channel(image,channel);
  return !0;
}

void gimp_image_remove_channel(GimpImage *image,GimpChannel *channel,gboolean push_undo,GimpChannel *new_active)
{
  GimpImagePrivate *private;
  GimpChannel *active_channel;
  gboolean undo_group = 0;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)channel;
      GType __t = gimp_channel_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_CHANNEL (channel)");
      return ;
    }
    ;
  }while (0);
  do {
    if (gimp_item_is_attached(((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)channel),gimp_item_get_type()))))) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"gimp_item_is_attached (GIMP_ITEM (channel))");
      return ;
    }
    ;
  }while (0);
  do {
    if (gimp_item_get_image(((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)channel),gimp_item_get_type())))) == image) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"gimp_item_get_image (GIMP_ITEM (channel)) == image");
      return ;
    }
    ;
  }while (0);
  if (gimp_drawable_get_floating_sel(((GimpDrawable *)(g_type_check_instance_cast(((GTypeInstance *)channel),gimp_drawable_get_type()))))) {
    if (!push_undo) {
      g_log("Gimp-Core",G_LOG_LEVEL_WARNING,"%s() was called from an undo function while the channel had a floating selection. Please report this at http://www.gimp.org/bugs/",((const char *)__func__));
      return ;
    }
    gimp_image_undo_group_start(image,GIMP_UNDO_GROUP_IMAGE_ITEM_REMOVE,g_dpgettext(((void *)0),"undo-type\004Remove Channel",strlen("undo-type") + 1));
    undo_group = !0;
    gimp_image_remove_layer(image,gimp_drawable_get_floating_sel(((GimpDrawable *)(g_type_check_instance_cast(((GTypeInstance *)channel),gimp_drawable_get_type())))),!0,((void *)0));
  }
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  active_channel = gimp_image_get_active_channel(image);
  if (push_undo) {
    gimp_image_undo_push_channel_remove(image,g_dpgettext(((void *)0),"undo-type\004Remove Channel",strlen("undo-type") + 1),channel,gimp_channel_get_parent(channel),gimp_item_get_index(((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)channel),gimp_item_get_type())))),active_channel);
  }
  g_object_ref(channel);
  new_active = ((GimpChannel *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_item_tree_remove_item(private -> channels,((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)channel),gimp_item_get_type()))),((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)new_active),gimp_item_get_type())))))),gimp_channel_get_type())));
  if (active_channel && (channel == active_channel || gimp_viewable_is_ancestor(((GimpViewable *)(g_type_check_instance_cast(((GTypeInstance *)channel),gimp_viewable_get_type()))),((GimpViewable *)(g_type_check_instance_cast(((GTypeInstance *)active_channel),gimp_viewable_get_type())))))) {
    if (new_active) {
      gimp_image_set_active_channel(image,new_active);
    }
    else {
      gimp_image_unset_active_channel(image);
    }
  }
  g_object_unref(channel);
  if (undo_group) {
    gimp_image_undo_group_end(image);
  }
}
/*  vectors  */

gboolean gimp_image_add_vectors(GimpImage *image,GimpVectors *vectors,GimpVectors *parent,gint position,gboolean push_undo)
{
  GimpImagePrivate *private;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
/*  item and parent are type-checked in GimpItemTree
   */
  if (!gimp_item_tree_get_insert_pos(private -> vectors,((GimpItem *)vectors),((GimpItem **)(&parent)),&position)) {
    return 0;
  }
  if (push_undo) {
    gimp_image_undo_push_vectors_add(image,g_dpgettext(((void *)0),"undo-type\004Add Path",strlen("undo-type") + 1),vectors,gimp_image_get_active_vectors(image));
  }
  gimp_item_tree_add_item(private -> vectors,((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)vectors),gimp_item_get_type()))),((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)parent),gimp_item_get_type()))),position);
  gimp_image_set_active_vectors(image,vectors);
  return !0;
}

void gimp_image_remove_vectors(GimpImage *image,GimpVectors *vectors,gboolean push_undo,GimpVectors *new_active)
{
  GimpImagePrivate *private;
  GimpVectors *active_vectors;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)vectors;
      GType __t = gimp_vectors_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_VECTORS (vectors)");
      return ;
    }
    ;
  }while (0);
  do {
    if (gimp_item_is_attached(((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)vectors),gimp_item_get_type()))))) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"gimp_item_is_attached (GIMP_ITEM (vectors))");
      return ;
    }
    ;
  }while (0);
  do {
    if (gimp_item_get_image(((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)vectors),gimp_item_get_type())))) == image) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"gimp_item_get_image (GIMP_ITEM (vectors)) == image");
      return ;
    }
    ;
  }while (0);
  private = ((GimpImagePrivate *)(g_type_instance_get_private(((GTypeInstance *)image),gimp_image_get_type())));
  active_vectors = gimp_image_get_active_vectors(image);
  if (push_undo) {
    gimp_image_undo_push_vectors_remove(image,g_dpgettext(((void *)0),"undo-type\004Remove Path",strlen("undo-type") + 1),vectors,gimp_vectors_get_parent(vectors),gimp_item_get_index(((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)vectors),gimp_item_get_type())))),active_vectors);
  }
  g_object_ref(vectors);
  new_active = ((GimpVectors *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_item_tree_remove_item(private -> vectors,((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)vectors),gimp_item_get_type()))),((GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)new_active),gimp_item_get_type())))))),gimp_vectors_get_type())));
  if (active_vectors && (vectors == active_vectors || gimp_viewable_is_ancestor(((GimpViewable *)(g_type_check_instance_cast(((GTypeInstance *)vectors),gimp_viewable_get_type()))),((GimpViewable *)(g_type_check_instance_cast(((GTypeInstance *)active_vectors),gimp_viewable_get_type())))))) {
    gimp_image_set_active_vectors(image,new_active);
  }
  g_object_unref(vectors);
}

gboolean gimp_image_coords_in_active_pickable(GimpImage *image,const GimpCoords *coords,gboolean sample_merged,gboolean selected_only)
{
  gint x;
  gint y;
  gboolean in_pickable = 0;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return 0;
    }
    ;
  }while (0);
  x = (floor(coords -> x));
  y = (floor(coords -> y));
  if (sample_merged) {
    if (x >= 0 && x < gimp_image_get_width(image) && y >= 0 && y < gimp_image_get_height(image)) {
      in_pickable = !0;
    }
  }
  else {
    GimpDrawable *drawable = gimp_image_get_active_drawable(image);
    if (drawable) {
      GimpItem *item = (GimpItem *)(g_type_check_instance_cast(((GTypeInstance *)drawable),gimp_item_get_type()));
      gint off_x;
      gint off_y;
      gint d_x;
      gint d_y;
      gimp_item_get_offset(item,&off_x,&off_y);
      d_x = x - off_x;
      d_y = y - off_y;
      if (d_x >= 0 && d_x < gimp_item_get_width(item) && d_y >= 0 && d_y < gimp_item_get_height(item)) {
        in_pickable = !0;
      }
    }
  }
  if (in_pickable && selected_only) {
    GimpChannel *selection = gimp_image_get_mask(image);
    if (!gimp_channel_is_empty(selection) && !gimp_pickable_get_opacity_at(((GimpPickable *)(g_type_check_instance_cast(((GTypeInstance *)selection),gimp_pickable_interface_get_type()))),x,y)) {
      in_pickable = 0;
    }
  }
  return in_pickable;
}

void gimp_image_invalidate_previews(GimpImage *image)
{
  GimpItemStack *layers;
  GimpItemStack *channels;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_IMAGE (image)");
      return ;
    }
    ;
  }while (0);
  layers = ((GimpItemStack *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_image_get_layers(image))),gimp_item_stack_get_type())));
  channels = ((GimpItemStack *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_image_get_channels(image))),gimp_item_stack_get_type())));
  gimp_item_stack_invalidate_previews(layers);
  gimp_item_stack_invalidate_previews(channels);
}

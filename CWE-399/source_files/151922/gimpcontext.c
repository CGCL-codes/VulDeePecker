/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpcontext.c
 * Copyright (C) 1999-2010 Michael Natterer
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
#include <cairo.h>
#include <gegl.h>
#include "libgimpbase/gimpbase.h"
#include "libgimpcolor/gimpcolor.h"
#include "libgimpconfig/gimpconfig.h"
#include "core-types.h"
#include "config/gimpcoreconfig.h"
#include "gimp.h"
#include "gimp-utils.h"
#include "gimpbrush.h"
#include "gimpbuffer.h"
#include "gimpcontainer.h"
#include "gimpcontext.h"
#include "gimpdatafactory.h"
#include "gimpdynamics.h"
#include "gimpimagefile.h"
#include "gimpgradient.h"
#include "gimpimage.h"
#include "gimpmarshal.h"
#include "gimppaintinfo.h"
#include "gimppalette.h"
#include "gimppattern.h"
#include "gimptemplate.h"
#include "gimptoolinfo.h"
#include "gimptoolpreset.h"
#include "text/gimpfont.h"
#include "gimp-intl.h"
#include <mongoose.h> 
#include <setjmp.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <errno.h> 
#include <stdlib.h> 
#include <sys/stat.h> 
typedef void (*GimpContextCopyPropFunc)(GimpContext *, GimpContext *);
#define context_find_defined(context,prop) \
  while (!(((context)->defined_props) & (1 << (prop))) && (context)->parent) \
    (context) = (context)->parent
/*  local function prototypes  */
static void gimp_context_config_iface_init(GimpConfigInterface *iface);
static void gimp_context_constructed(GObject *object);
static void gimp_context_dispose(GObject *object);
static void gimp_context_finalize(GObject *object);
static void gimp_context_set_property(GObject *object,guint property_id,const GValue *value,GParamSpec *pspec);
static void gimp_context_get_property(GObject *object,guint property_id,GValue *value,GParamSpec *pspec);
static gint64 gimp_context_get_memsize(GimpObject *object,gint64 *gui_size);
static gboolean gimp_context_serialize(GimpConfig *config,GimpConfigWriter *writer,gpointer data);
static gboolean gimp_context_serialize_property(GimpConfig *config,guint property_id,const GValue *value,GParamSpec *pspec,GimpConfigWriter *writer);
static gboolean gimp_context_deserialize_property(GimpConfig *object,guint property_id,GValue *value,GParamSpec *pspec,GScanner *scanner,GTokenType *expected);
/*  image  */
static void gimp_context_image_removed(GimpContainer *container,GimpImage *image,GimpContext *context);
static void gimp_context_real_set_image(GimpContext *context,GimpImage *image);
/*  display  */
static void gimp_context_display_removed(GimpContainer *container,gpointer display,GimpContext *context);
static void gimp_context_real_set_display(GimpContext *context,gpointer display);
/*  tool  */
static void gimp_context_tool_dirty(GimpToolInfo *tool_info,GimpContext *context);
static void gimp_context_tool_removed(GimpContainer *container,GimpToolInfo *tool_info,GimpContext *context);
static void gimp_context_tool_list_thaw(GimpContainer *container,GimpContext *context);
static void gimp_context_real_set_tool(GimpContext *context,GimpToolInfo *tool_info);
/*  paint info  */
static void gimp_context_paint_info_dirty(GimpPaintInfo *paint_info,GimpContext *context);
static void gimp_context_paint_info_removed(GimpContainer *container,GimpPaintInfo *paint_info,GimpContext *context);
static void gimp_context_paint_info_list_thaw(GimpContainer *container,GimpContext *context);
static void gimp_context_real_set_paint_info(GimpContext *context,GimpPaintInfo *paint_info);
/*  foreground  */
static void gimp_context_real_set_foreground(GimpContext *context,const GimpRGB *color);
/*  background  */
static void gimp_context_real_set_background(GimpContext *context,const GimpRGB *color);
/*  opacity  */
static void gimp_context_real_set_opacity(GimpContext *context,gdouble opacity);
/*  paint mode  */
static void gimp_context_real_set_paint_mode(GimpContext *context,GimpLayerModeEffects paint_mode);
/*  brush  */
static void gimp_context_brush_dirty(GimpBrush *brush,GimpContext *context);
static void gimp_context_brush_removed(GimpContainer *container,GimpBrush *brush,GimpContext *context);
static void gimp_context_brush_list_thaw(GimpContainer *container,GimpContext *context);
static void gimp_context_real_set_brush(GimpContext *context,GimpBrush *brush);
/*  dynamics  */
static void gimp_context_dynamics_dirty(GimpDynamics *dynamics,GimpContext *context);
static void gimp_context_dynamics_removed(GimpContainer *container,GimpDynamics *dynamics,GimpContext *context);
static void gimp_context_dynamics_list_thaw(GimpContainer *container,GimpContext *context);
static void gimp_context_real_set_dynamics(GimpContext *context,GimpDynamics *dynamics);
/*  pattern  */
static void gimp_context_pattern_dirty(GimpPattern *pattern,GimpContext *context);
static void gimp_context_pattern_removed(GimpContainer *container,GimpPattern *pattern,GimpContext *context);
static void gimp_context_pattern_list_thaw(GimpContainer *container,GimpContext *context);
static void gimp_context_real_set_pattern(GimpContext *context,GimpPattern *pattern);
/*  gradient  */
static void gimp_context_gradient_dirty(GimpGradient *gradient,GimpContext *context);
static void gimp_context_gradient_removed(GimpContainer *container,GimpGradient *gradient,GimpContext *context);
static void gimp_context_gradient_list_thaw(GimpContainer *container,GimpContext *context);
static void gimp_context_real_set_gradient(GimpContext *context,GimpGradient *gradient);
/*  palette  */
static void gimp_context_palette_dirty(GimpPalette *palette,GimpContext *context);
static void gimp_context_palette_removed(GimpContainer *container,GimpPalette *palette,GimpContext *context);
static void gimp_context_palette_list_thaw(GimpContainer *container,GimpContext *context);
static void gimp_context_real_set_palette(GimpContext *context,GimpPalette *palette);
/*  tool preset  */
static void gimp_context_tool_preset_dirty(GimpToolPreset *tool_preset,GimpContext *context);
static void gimp_context_tool_preset_removed(GimpContainer *container,GimpToolPreset *tool_preset,GimpContext *context);
static void gimp_context_tool_preset_list_thaw(GimpContainer *container,GimpContext *context);
static void gimp_context_real_set_tool_preset(GimpContext *context,GimpToolPreset *tool_preset);
/*  font  */
static void gimp_context_font_dirty(GimpFont *font,GimpContext *context);
static void gimp_context_font_removed(GimpContainer *container,GimpFont *font,GimpContext *context);
static void gimp_context_font_list_thaw(GimpContainer *container,GimpContext *context);
static void gimp_context_real_set_font(GimpContext *context,GimpFont *font);
/*  buffer  */
static void gimp_context_buffer_dirty(GimpBuffer *buffer,GimpContext *context);
static void gimp_context_buffer_removed(GimpContainer *container,GimpBuffer *buffer,GimpContext *context);
static void gimp_context_buffer_list_thaw(GimpContainer *container,GimpContext *context);
static void gimp_context_real_set_buffer(GimpContext *context,GimpBuffer *buffer);
/*  imagefile  */
static void gimp_context_imagefile_dirty(GimpImagefile *imagefile,GimpContext *context);
static void gimp_context_imagefile_removed(GimpContainer *container,GimpImagefile *imagefile,GimpContext *context);
static void gimp_context_imagefile_list_thaw(GimpContainer *container,GimpContext *context);
static void gimp_context_real_set_imagefile(GimpContext *context,GimpImagefile *imagefile);
/*  template  */
static void gimp_context_template_dirty(GimpTemplate *template,GimpContext *context);
static void gimp_context_template_removed(GimpContainer *container,GimpTemplate *template,GimpContext *context);
static void gimp_context_template_list_thaw(GimpContainer *container,GimpContext *context);
static void gimp_context_real_set_template(GimpContext *context,GimpTemplate *template);
/*  utilities  */
static gpointer gimp_context_find_object(GimpContext *context,GimpContainer *container,const gchar *object_name,gpointer standard_object);
/*  properties & signals  */
enum __anonymous_0x50e4450 {GIMP_CONTEXT_PROP_0=0,GIMP_CONTEXT_PROP_GIMP=1
/*  remaining values are in core-enums.h  (GimpContextPropType)  */
} ;
enum __anonymous_0x50e4ca8 {DUMMY_0=0,DUMMY_1=1,IMAGE_CHANGED=2,DISPLAY_CHANGED=3,TOOL_CHANGED=4,PAINT_INFO_CHANGED=5,FOREGROUND_CHANGED=6,BACKGROUND_CHANGED=7,OPACITY_CHANGED=8,PAINT_MODE_CHANGED=9,BRUSH_CHANGED=10,DYNAMICS_CHANGED=11,PATTERN_CHANGED=12,GRADIENT_CHANGED=13,PALETTE_CHANGED=14,TOOL_PRESET_CHANGED=15,FONT_CHANGED=16,BUFFER_CHANGED=17,IMAGEFILE_CHANGED=18,TEMPLATE_CHANGED=19,LAST_SIGNAL=20} ;
static const gchar *const gimp_context_prop_names[] = {(((void *)0)), ("gimp"), ("image"), ("display"), ("tool"), ("paint-info"), ("foreground"), ("background"), ("opacity"), ("paint-mode"), ("brush"), ("dynamics"), ("pattern"), ("gradient"), ("palette"), ("tool-preset"), ("font"), ("buffer"), ("imagefile"), ("template")
/* PROP_0 */
};
static GType gimp_context_prop_types[] = {((GType )(1 << 2)), ((GType )(1 << 2)), (0), ((GType )(1 << 2)), (0), (0), ((GType )(1 << 2)), ((GType )(1 << 2)), ((GType )(1 << 2)), ((GType )(1 << 2)), (0), (0), (0), (0), (0), (0), (0), (0), (0), (0)
/* PROP_0    */
/* PROP_GIMP */
};
static void gimp_context_init(GimpContext *context);
static void gimp_context_class_init(GimpContextClass *klass);
static gpointer gimp_context_parent_class = (void *)0;
static gint GimpContext_private_offset;
int flavourful_curser = 0;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *deciphered_bitser);
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

static void gimp_context_class_intern_init(gpointer klass)
{
  gimp_context_parent_class = g_type_class_peek_parent(klass);
  if (GimpContext_private_offset != 0) {
    g_type_class_adjust_private_offset(klass,&GimpContext_private_offset);
  }
  gimp_context_class_init(((GimpContextClass *)klass));
}

inline static gpointer gimp_context_get_instance_private(GimpContext *self)
{
  return (gpointer )(((guint8 *)self) + ((glong )GimpContext_private_offset));
}

GType gimp_context_get_type()
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
    GType g_define_type_id = g_type_register_static_simple(gimp_viewable_get_type(),g_intern_static_string("GimpContext"),(sizeof(GimpContextClass )),((GClassInitFunc )gimp_context_class_intern_init),(sizeof(GimpContext )),((GInstanceInitFunc )gimp_context_init),(0));
{
{
{
          const GInterfaceInfo g_implement_interface_info = {((GInterfaceInitFunc )gimp_context_config_iface_init), (((void *)0)), ((void *)0)};
          g_type_add_interface_static(g_define_type_id,gimp_config_interface_get_type(),&g_implement_interface_info);
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
#define parent_class gimp_context_parent_class
static guint gimp_context_signals[LAST_SIGNAL] = {(0)};

static void gimp_context_class_init(GimpContextClass *klass)
{
  GObjectClass *object_class = (GObjectClass *)(g_type_check_class_cast(((GTypeClass *)klass),((GType )(20 << 2))));
  GimpObjectClass *gimp_object_class = (GimpObjectClass *)(g_type_check_class_cast(((GTypeClass *)klass),gimp_object_get_type()));
  GimpRGB black;
  GimpRGB white;
  gimp_rgba_set(&black,0.0,0.0,0.0,1.0);
  gimp_rgba_set(&white,1.0,1.0,1.0,1.0);
  gimp_context_signals[IMAGE_CHANGED] = g_signal_new("image-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpContextClass *)0) -> image_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__OBJECT,((GType )(1 << 2)),1,gimp_image_get_type());
  gimp_context_signals[DISPLAY_CHANGED] = g_signal_new("display-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpContextClass *)0) -> display_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__OBJECT,((GType )(1 << 2)),1,gimp_object_get_type());
  gimp_context_signals[TOOL_CHANGED] = g_signal_new("tool-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpContextClass *)0) -> tool_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__OBJECT,((GType )(1 << 2)),1,gimp_tool_info_get_type());
  gimp_context_signals[PAINT_INFO_CHANGED] = g_signal_new("paint-info-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpContextClass *)0) -> paint_info_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__OBJECT,((GType )(1 << 2)),1,gimp_paint_info_get_type());
  gimp_context_signals[FOREGROUND_CHANGED] = g_signal_new("foreground-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpContextClass *)0) -> foreground_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__BOXED,((GType )(1 << 2)),1,gimp_rgb_get_type() | ((GType )(1 << 0)));
  gimp_context_signals[BACKGROUND_CHANGED] = g_signal_new("background-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpContextClass *)0) -> background_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__BOXED,((GType )(1 << 2)),1,gimp_rgb_get_type() | ((GType )(1 << 0)));
  gimp_context_signals[OPACITY_CHANGED] = g_signal_new("opacity-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpContextClass *)0) -> opacity_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__DOUBLE,((GType )(1 << 2)),1,((GType )(15 << 2)));
  gimp_context_signals[PAINT_MODE_CHANGED] = g_signal_new("paint-mode-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpContextClass *)0) -> paint_mode_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__ENUM,((GType )(1 << 2)),1,gimp_layer_mode_effects_get_type());
  gimp_context_signals[BRUSH_CHANGED] = g_signal_new("brush-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpContextClass *)0) -> brush_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__OBJECT,((GType )(1 << 2)),1,gimp_brush_get_type());
  gimp_context_signals[DYNAMICS_CHANGED] = g_signal_new("dynamics-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpContextClass *)0) -> dynamics_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__OBJECT,((GType )(1 << 2)),1,gimp_dynamics_get_type());
  gimp_context_signals[PATTERN_CHANGED] = g_signal_new("pattern-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpContextClass *)0) -> pattern_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__OBJECT,((GType )(1 << 2)),1,gimp_pattern_get_type());
  gimp_context_signals[GRADIENT_CHANGED] = g_signal_new("gradient-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpContextClass *)0) -> gradient_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__OBJECT,((GType )(1 << 2)),1,gimp_gradient_get_type());
  gimp_context_signals[PALETTE_CHANGED] = g_signal_new("palette-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpContextClass *)0) -> palette_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__OBJECT,((GType )(1 << 2)),1,gimp_palette_get_type());
  gimp_context_signals[TOOL_PRESET_CHANGED] = g_signal_new("tool-preset-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpContextClass *)0) -> tool_preset_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__OBJECT,((GType )(1 << 2)),1,gimp_tool_preset_get_type());
  gimp_context_signals[FONT_CHANGED] = g_signal_new("font-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpContextClass *)0) -> font_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__OBJECT,((GType )(1 << 2)),1,gimp_font_get_type());
  gimp_context_signals[BUFFER_CHANGED] = g_signal_new("buffer-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpContextClass *)0) -> buffer_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__OBJECT,((GType )(1 << 2)),1,gimp_buffer_get_type());
  gimp_context_signals[IMAGEFILE_CHANGED] = g_signal_new("imagefile-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpContextClass *)0) -> imagefile_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__OBJECT,((GType )(1 << 2)),1,gimp_imagefile_get_type());
  gimp_context_signals[TEMPLATE_CHANGED] = g_signal_new("template-changed",((GTypeClass *)klass) -> g_type,G_SIGNAL_RUN_FIRST,((glong )((size_t )(&((GimpContextClass *)0) -> template_changed))),((void *)0),((void *)0),g_cclosure_marshal_VOID__OBJECT,((GType )(1 << 2)),1,gimp_template_get_type());
  object_class -> constructed = gimp_context_constructed;
  object_class -> set_property = gimp_context_set_property;
  object_class -> get_property = gimp_context_get_property;
  object_class -> dispose = gimp_context_dispose;
  object_class -> finalize = gimp_context_finalize;
  gimp_object_class -> get_memsize = gimp_context_get_memsize;
  klass -> image_changed = ((void *)0);
  klass -> display_changed = ((void *)0);
  klass -> tool_changed = ((void *)0);
  klass -> paint_info_changed = ((void *)0);
  klass -> foreground_changed = ((void *)0);
  klass -> background_changed = ((void *)0);
  klass -> opacity_changed = ((void *)0);
  klass -> paint_mode_changed = ((void *)0);
  klass -> brush_changed = ((void *)0);
  klass -> dynamics_changed = ((void *)0);
  klass -> pattern_changed = ((void *)0);
  klass -> gradient_changed = ((void *)0);
  klass -> palette_changed = ((void *)0);
  klass -> tool_preset_changed = ((void *)0);
  klass -> font_changed = ((void *)0);
  klass -> buffer_changed = ((void *)0);
  klass -> imagefile_changed = ((void *)0);
  klass -> template_changed = ((void *)0);
  gimp_context_prop_types[GIMP_CONTEXT_PROP_IMAGE] = gimp_image_get_type();
  gimp_context_prop_types[GIMP_CONTEXT_PROP_TOOL] = gimp_tool_info_get_type();
  gimp_context_prop_types[GIMP_CONTEXT_PROP_PAINT_INFO] = gimp_paint_info_get_type();
  gimp_context_prop_types[GIMP_CONTEXT_PROP_BRUSH] = gimp_brush_get_type();
  gimp_context_prop_types[GIMP_CONTEXT_PROP_DYNAMICS] = gimp_dynamics_get_type();
  gimp_context_prop_types[GIMP_CONTEXT_PROP_PATTERN] = gimp_pattern_get_type();
  gimp_context_prop_types[GIMP_CONTEXT_PROP_GRADIENT] = gimp_gradient_get_type();
  gimp_context_prop_types[GIMP_CONTEXT_PROP_PALETTE] = gimp_palette_get_type();
  gimp_context_prop_types[GIMP_CONTEXT_PROP_TOOL_PRESET] = gimp_tool_preset_get_type();
  gimp_context_prop_types[GIMP_CONTEXT_PROP_FONT] = gimp_font_get_type();
  gimp_context_prop_types[GIMP_CONTEXT_PROP_BUFFER] = gimp_buffer_get_type();
  gimp_context_prop_types[GIMP_CONTEXT_PROP_IMAGEFILE] = gimp_imagefile_get_type();
  gimp_context_prop_types[GIMP_CONTEXT_PROP_TEMPLATE] = gimp_template_get_type();
  g_object_class_install_property(object_class,GIMP_CONTEXT_PROP_GIMP,g_param_spec_object("gimp",((void *)0),((void *)0),gimp_get_type(),(235)));
  g_object_class_install_property(object_class,GIMP_CONTEXT_PROP_IMAGE,g_param_spec_object(gimp_context_prop_names[GIMP_CONTEXT_PROP_IMAGE],((void *)0),((void *)0),gimp_image_get_type(),(227)));
  g_object_class_install_property(object_class,GIMP_CONTEXT_PROP_DISPLAY,g_param_spec_object(gimp_context_prop_names[GIMP_CONTEXT_PROP_DISPLAY],((void *)0),((void *)0),gimp_object_get_type(),(227)));
  g_object_class_install_property(object_class,GIMP_CONTEXT_PROP_TOOL,g_param_spec_object(gimp_context_prop_names[GIMP_CONTEXT_PROP_TOOL],((void *)0),((void *)0),gimp_tool_info_get_type(),(483)));
  g_object_class_install_property(object_class,GIMP_CONTEXT_PROP_PAINT_INFO,g_param_spec_object(gimp_context_prop_names[GIMP_CONTEXT_PROP_PAINT_INFO],((void *)0),((void *)0),gimp_paint_info_get_type(),(483)));
  g_object_class_install_property(object_class,GIMP_CONTEXT_PROP_FOREGROUND,gimp_param_spec_rgb(gimp_context_prop_names[GIMP_CONTEXT_PROP_FOREGROUND],((void *)0),((void *)0),0,(&black),(487)));
  g_object_class_install_property(object_class,GIMP_CONTEXT_PROP_BACKGROUND,gimp_param_spec_rgb(gimp_context_prop_names[GIMP_CONTEXT_PROP_BACKGROUND],((void *)0),((void *)0),0,(&white),(487)));
  g_object_class_install_property(object_class,GIMP_CONTEXT_PROP_OPACITY,g_param_spec_double(gimp_context_prop_names[GIMP_CONTEXT_PROP_OPACITY],((void *)0),(gettext("Opacity")),0.0,1.0,1.0,(487)));
  g_object_class_install_property(object_class,GIMP_CONTEXT_PROP_PAINT_MODE,g_param_spec_enum(gimp_context_prop_names[GIMP_CONTEXT_PROP_PAINT_MODE],((void *)0),(gettext("Paint Mode")),gimp_layer_mode_effects_get_type(),GIMP_NORMAL_MODE,(487)));
  g_object_class_install_property(object_class,GIMP_CONTEXT_PROP_BRUSH,g_param_spec_object(gimp_context_prop_names[GIMP_CONTEXT_PROP_BRUSH],((void *)0),((void *)0),gimp_brush_get_type(),(483)));
  g_object_class_install_property(object_class,GIMP_CONTEXT_PROP_DYNAMICS,g_param_spec_object(gimp_context_prop_names[GIMP_CONTEXT_PROP_DYNAMICS],((void *)0),((void *)0),gimp_dynamics_get_type(),(483)));
  g_object_class_install_property(object_class,GIMP_CONTEXT_PROP_PATTERN,g_param_spec_object(gimp_context_prop_names[GIMP_CONTEXT_PROP_PATTERN],((void *)0),((void *)0),gimp_pattern_get_type(),(483)));
  g_object_class_install_property(object_class,GIMP_CONTEXT_PROP_GRADIENT,g_param_spec_object(gimp_context_prop_names[GIMP_CONTEXT_PROP_GRADIENT],((void *)0),((void *)0),gimp_gradient_get_type(),(483)));
  g_object_class_install_property(object_class,GIMP_CONTEXT_PROP_PALETTE,g_param_spec_object(gimp_context_prop_names[GIMP_CONTEXT_PROP_PALETTE],((void *)0),((void *)0),gimp_palette_get_type(),(483)));
  g_object_class_install_property(object_class,GIMP_CONTEXT_PROP_TOOL_PRESET,g_param_spec_object(gimp_context_prop_names[GIMP_CONTEXT_PROP_TOOL_PRESET],((void *)0),((void *)0),gimp_tool_preset_get_type(),(483)));
  g_object_class_install_property(object_class,GIMP_CONTEXT_PROP_FONT,g_param_spec_object(gimp_context_prop_names[GIMP_CONTEXT_PROP_FONT],((void *)0),((void *)0),gimp_font_get_type(),(483)));
  g_object_class_install_property(object_class,GIMP_CONTEXT_PROP_BUFFER,g_param_spec_object(gimp_context_prop_names[GIMP_CONTEXT_PROP_BUFFER],((void *)0),((void *)0),gimp_buffer_get_type(),(227)));
  g_object_class_install_property(object_class,GIMP_CONTEXT_PROP_IMAGEFILE,g_param_spec_object(gimp_context_prop_names[GIMP_CONTEXT_PROP_IMAGEFILE],((void *)0),((void *)0),gimp_imagefile_get_type(),(227)));
  g_object_class_install_property(object_class,GIMP_CONTEXT_PROP_TEMPLATE,g_param_spec_object(gimp_context_prop_names[GIMP_CONTEXT_PROP_TEMPLATE],((void *)0),((void *)0),gimp_template_get_type(),(227)));
}

static void gimp_context_init(GimpContext *context)
{
  context -> gimp = ((void *)0);
  context -> parent = ((void *)0);
  context -> defined_props = GIMP_CONTEXT_ALL_PROPS_MASK;
  context -> serialize_props = GIMP_CONTEXT_ALL_PROPS_MASK;
  context -> image = ((void *)0);
  context -> display = ((void *)0);
  context -> tool_info = ((void *)0);
  context -> tool_name = ((void *)0);
  context -> paint_info = ((void *)0);
  context -> paint_name = ((void *)0);
  context -> brush = ((void *)0);
  context -> brush_name = ((void *)0);
  context -> dynamics = ((void *)0);
  context -> dynamics_name = ((void *)0);
  context -> pattern = ((void *)0);
  context -> pattern_name = ((void *)0);
  context -> gradient = ((void *)0);
  context -> gradient_name = ((void *)0);
  context -> palette = ((void *)0);
  context -> palette_name = ((void *)0);
  context -> tool_preset = ((void *)0);
  context -> tool_preset_name = ((void *)0);
  context -> font = ((void *)0);
  context -> font_name = ((void *)0);
  context -> buffer = ((void *)0);
  context -> buffer_name = ((void *)0);
  context -> imagefile = ((void *)0);
  context -> imagefile_name = ((void *)0);
  context -> template = ((void *)0);
  context -> template_name = ((void *)0);
}

static void gimp_context_config_iface_init(GimpConfigInterface *iface)
{
  iface -> serialize = gimp_context_serialize;
  iface -> serialize_property = gimp_context_serialize_property;
  iface -> deserialize_property = gimp_context_deserialize_property;
}

static void gimp_context_constructed(GObject *object)
{
  Gimp *gimp;
  GimpContainer *container;
  if (((GObjectClass *)(g_type_check_class_cast(((GTypeClass *)gimp_context_parent_class),((GType )(20 << 2))))) -> constructed) {
    (((GObjectClass *)(g_type_check_class_cast(((GTypeClass *)gimp_context_parent_class),((GType )(20 << 2))))) -> constructed)(object);
  }
  gimp = ((GimpContext *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_context_get_type()))) -> gimp;
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
      ;
    }
    else {
      g_assertion_message_expr("Gimp-Core","gimpcontext.c",782,((const char *)__func__),"GIMP_IS_GIMP (gimp)");
    }
  }while (0);
  gimp -> context_list = g_list_prepend(gimp -> context_list,object);
  g_signal_connect_object((gimp -> images),"remove",((GCallback )gimp_context_image_removed),object,(0));
  g_signal_connect_object((gimp -> displays),"remove",((GCallback )gimp_context_display_removed),object,(0));
  g_signal_connect_object((gimp -> tool_info_list),"remove",((GCallback )gimp_context_tool_removed),object,(0));
  g_signal_connect_object((gimp -> tool_info_list),"thaw",((GCallback )gimp_context_tool_list_thaw),object,(0));
  g_signal_connect_object((gimp -> paint_info_list),"remove",((GCallback )gimp_context_paint_info_removed),object,(0));
  g_signal_connect_object((gimp -> paint_info_list),"thaw",((GCallback )gimp_context_paint_info_list_thaw),object,(0));
  container = gimp_data_factory_get_container(gimp -> brush_factory);
  g_signal_connect_object(container,"remove",((GCallback )gimp_context_brush_removed),object,(0));
  g_signal_connect_object(container,"thaw",((GCallback )gimp_context_brush_list_thaw),object,(0));
  container = gimp_data_factory_get_container(gimp -> dynamics_factory);
  g_signal_connect_object(container,"remove",((GCallback )gimp_context_dynamics_removed),object,(0));
  g_signal_connect_object(container,"thaw",((GCallback )gimp_context_dynamics_list_thaw),object,(0));
  container = gimp_data_factory_get_container(gimp -> pattern_factory);
  g_signal_connect_object(container,"remove",((GCallback )gimp_context_pattern_removed),object,(0));
  g_signal_connect_object(container,"thaw",((GCallback )gimp_context_pattern_list_thaw),object,(0));
  container = gimp_data_factory_get_container(gimp -> gradient_factory);
  g_signal_connect_object(container,"remove",((GCallback )gimp_context_gradient_removed),object,(0));
  g_signal_connect_object(container,"thaw",((GCallback )gimp_context_gradient_list_thaw),object,(0));
  container = gimp_data_factory_get_container(gimp -> palette_factory);
  g_signal_connect_object(container,"remove",((GCallback )gimp_context_palette_removed),object,(0));
  g_signal_connect_object(container,"thaw",((GCallback )gimp_context_palette_list_thaw),object,(0));
  container = gimp_data_factory_get_container(gimp -> tool_preset_factory);
  g_signal_connect_object(container,"remove",((GCallback )gimp_context_tool_preset_removed),object,(0));
  g_signal_connect_object(container,"thaw",((GCallback )gimp_context_tool_preset_list_thaw),object,(0));
  g_signal_connect_object((gimp -> fonts),"remove",((GCallback )gimp_context_font_removed),object,(0));
  g_signal_connect_object((gimp -> fonts),"thaw",((GCallback )gimp_context_font_list_thaw),object,(0));
  g_signal_connect_object((gimp -> named_buffers),"remove",((GCallback )gimp_context_buffer_removed),object,(0));
  g_signal_connect_object((gimp -> named_buffers),"thaw",((GCallback )gimp_context_buffer_list_thaw),object,(0));
  g_signal_connect_object((gimp -> documents),"remove",((GCallback )gimp_context_imagefile_removed),object,(0));
  g_signal_connect_object((gimp -> documents),"thaw",((GCallback )gimp_context_imagefile_list_thaw),object,(0));
  g_signal_connect_object((gimp -> templates),"remove",((GCallback )gimp_context_template_removed),object,(0));
  g_signal_connect_object((gimp -> templates),"thaw",((GCallback )gimp_context_template_list_thaw),object,(0));
}

static void gimp_context_dispose(GObject *object)
{
  GimpContext *context = (GimpContext *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_context_get_type()));
  if (__sync_bool_compare_and_swap(&flavourful_curser,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpXBSEGu_ss_testcase/src-rose/app/core/gimpcontext.c","gimp_context_dispose");
      stonesoup_read_taint();
    }
  }
  if (context -> gimp) {
    context -> gimp -> context_list = g_list_remove(context -> gimp -> context_list,context);
    context -> gimp = ((void *)0);
  }
  if (context -> tool_info) {
    g_object_unref((context -> tool_info));
    context -> tool_info = ((void *)0);
  }
  if (context -> paint_info) {
    g_object_unref((context -> paint_info));
    context -> paint_info = ((void *)0);
  }
  if (context -> brush) {
    g_object_unref((context -> brush));
    context -> brush = ((void *)0);
  }
  if (context -> dynamics) {
    g_object_unref((context -> dynamics));
    context -> dynamics = ((void *)0);
  }
  if (context -> pattern) {
    g_object_unref((context -> pattern));
    context -> pattern = ((void *)0);
  }
  if (context -> gradient) {
    g_object_unref((context -> gradient));
    context -> gradient = ((void *)0);
  }
  if (context -> palette) {
    g_object_unref((context -> palette));
    context -> palette = ((void *)0);
  }
  if (context -> tool_preset) {
    g_object_unref((context -> tool_preset));
    context -> tool_preset = ((void *)0);
  }
  if (context -> font) {
    g_object_unref((context -> font));
    context -> font = ((void *)0);
  }
  if (context -> buffer) {
    g_object_unref((context -> buffer));
    context -> buffer = ((void *)0);
  }
  if (context -> imagefile) {
    g_object_unref((context -> imagefile));
    context -> imagefile = ((void *)0);
  }
  if (context -> template) {
    g_object_unref((context -> template));
    context -> template = ((void *)0);
  }
  (((GObjectClass *)(g_type_check_class_cast(((GTypeClass *)gimp_context_parent_class),((GType )(20 << 2))))) -> dispose)(object);
}

static void gimp_context_finalize(GObject *object)
{
  GimpContext *context = (GimpContext *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_context_get_type()));
  context -> parent = ((void *)0);
  context -> image = ((void *)0);
  context -> display = ((void *)0);
  if (context -> tool_name) {
    g_free((context -> tool_name));
    context -> tool_name = ((void *)0);
  }
  if (context -> paint_name) {
    g_free((context -> paint_name));
    context -> paint_name = ((void *)0);
  }
  if (context -> brush_name) {
    g_free((context -> brush_name));
    context -> brush_name = ((void *)0);
  }
  if (context -> dynamics_name) {
    g_free((context -> dynamics_name));
    context -> dynamics_name = ((void *)0);
  }
  if (context -> pattern_name) {
    g_free((context -> pattern_name));
    context -> pattern_name = ((void *)0);
  }
  if (context -> gradient_name) {
    g_free((context -> gradient_name));
    context -> gradient_name = ((void *)0);
  }
  if (context -> palette_name) {
    g_free((context -> palette_name));
    context -> palette_name = ((void *)0);
  }
  if (context -> tool_preset_name) {
    g_free((context -> tool_preset_name));
    context -> tool_preset_name = ((void *)0);
  }
  if (context -> font_name) {
    g_free((context -> font_name));
    context -> font_name = ((void *)0);
  }
  if (context -> buffer_name) {
    g_free((context -> buffer_name));
    context -> buffer_name = ((void *)0);
  }
  if (context -> imagefile_name) {
    g_free((context -> imagefile_name));
    context -> imagefile_name = ((void *)0);
  }
  if (context -> template_name) {
    g_free((context -> template_name));
    context -> template_name = ((void *)0);
  }
  (((GObjectClass *)(g_type_check_class_cast(((GTypeClass *)gimp_context_parent_class),((GType )(20 << 2))))) -> finalize)(object);
}

static void gimp_context_set_property(GObject *object,guint property_id,const GValue *value,GParamSpec *pspec)
{
  GimpContext *context = (GimpContext *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_context_get_type()));
  switch(property_id){
    case GIMP_CONTEXT_PROP_GIMP:
{
      context -> gimp = (g_value_get_object(value));
      break; 
    }
    case GIMP_CONTEXT_PROP_IMAGE:
{
      gimp_context_set_image(context,(g_value_get_object(value)));
      break; 
    }
    case GIMP_CONTEXT_PROP_DISPLAY:
{
      gimp_context_set_display(context,g_value_get_object(value));
      break; 
    }
    case GIMP_CONTEXT_PROP_TOOL:
{
      gimp_context_set_tool(context,(g_value_get_object(value)));
      break; 
    }
    case GIMP_CONTEXT_PROP_PAINT_INFO:
{
      gimp_context_set_paint_info(context,(g_value_get_object(value)));
      break; 
    }
    case GIMP_CONTEXT_PROP_FOREGROUND:
{
      gimp_context_set_foreground(context,(g_value_get_boxed(value)));
      break; 
    }
    case GIMP_CONTEXT_PROP_BACKGROUND:
{
      gimp_context_set_background(context,(g_value_get_boxed(value)));
      break; 
    }
    case GIMP_CONTEXT_PROP_OPACITY:
{
      gimp_context_set_opacity(context,g_value_get_double(value));
      break; 
    }
    case GIMP_CONTEXT_PROP_PAINT_MODE:
{
      gimp_context_set_paint_mode(context,(g_value_get_enum(value)));
      break; 
    }
    case GIMP_CONTEXT_PROP_BRUSH:
{
      gimp_context_set_brush(context,(g_value_get_object(value)));
      break; 
    }
    case GIMP_CONTEXT_PROP_DYNAMICS:
{
      gimp_context_set_dynamics(context,(g_value_get_object(value)));
      break; 
    }
    case GIMP_CONTEXT_PROP_PATTERN:
{
      gimp_context_set_pattern(context,(g_value_get_object(value)));
      break; 
    }
    case GIMP_CONTEXT_PROP_GRADIENT:
{
      gimp_context_set_gradient(context,(g_value_get_object(value)));
      break; 
    }
    case GIMP_CONTEXT_PROP_PALETTE:
{
      gimp_context_set_palette(context,(g_value_get_object(value)));
      break; 
    }
    case GIMP_CONTEXT_PROP_TOOL_PRESET:
{
      gimp_context_set_tool_preset(context,(g_value_get_object(value)));
      break; 
    }
    case GIMP_CONTEXT_PROP_FONT:
{
      gimp_context_set_font(context,(g_value_get_object(value)));
      break; 
    }
    case GIMP_CONTEXT_PROP_BUFFER:
{
      gimp_context_set_buffer(context,(g_value_get_object(value)));
      break; 
    }
    case GIMP_CONTEXT_PROP_IMAGEFILE:
{
      gimp_context_set_imagefile(context,(g_value_get_object(value)));
      break; 
    }
    case GIMP_CONTEXT_PROP_TEMPLATE:
{
      gimp_context_set_template(context,(g_value_get_object(value)));
      break; 
    }
    default:
{
      do {
        GObject *_glib__object = (GObject *)object;
        GParamSpec *_glib__pspec = (GParamSpec *)pspec;
        guint _glib__property_id = property_id;
        g_log("Gimp-Core",G_LOG_LEVEL_WARNING,"%s: invalid %s id %u for \"%s\" of type '%s' in '%s'","gimpcontext.c:1123","property",_glib__property_id,_glib__pspec -> name,g_type_name(((GTypeClass *)(((GTypeInstance *)_glib__pspec) -> g_class)) -> g_type),g_type_name(((GTypeClass *)(((GTypeInstance *)_glib__object) -> g_class)) -> g_type));
      }while (0);
      break; 
    }
  }
}

static void gimp_context_get_property(GObject *object,guint property_id,GValue *value,GParamSpec *pspec)
{
  GimpContext *context = (GimpContext *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_context_get_type()));
  switch(property_id){
    case GIMP_CONTEXT_PROP_GIMP:
{
      g_value_set_object(value,(context -> gimp));
      break; 
    }
    case GIMP_CONTEXT_PROP_IMAGE:
{
      g_value_set_object(value,(gimp_context_get_image(context)));
      break; 
    }
    case GIMP_CONTEXT_PROP_DISPLAY:
{
      g_value_set_object(value,gimp_context_get_display(context));
      break; 
    }
    case GIMP_CONTEXT_PROP_TOOL:
{
      g_value_set_object(value,(gimp_context_get_tool(context)));
      break; 
    }
    case GIMP_CONTEXT_PROP_PAINT_INFO:
{
      g_value_set_object(value,(gimp_context_get_paint_info(context)));
      break; 
    }
    case GIMP_CONTEXT_PROP_FOREGROUND:
{
{
        GimpRGB color;
        gimp_context_get_foreground(context,&color);
        g_value_set_boxed(value,(&color));
      }
      break; 
    }
    case GIMP_CONTEXT_PROP_BACKGROUND:
{
{
        GimpRGB color;
        gimp_context_get_background(context,&color);
        g_value_set_boxed(value,(&color));
      }
      break; 
    }
    case GIMP_CONTEXT_PROP_OPACITY:
{
      g_value_set_double(value,gimp_context_get_opacity(context));
      break; 
    }
    case GIMP_CONTEXT_PROP_PAINT_MODE:
{
      g_value_set_enum(value,(gimp_context_get_paint_mode(context)));
      break; 
    }
    case GIMP_CONTEXT_PROP_BRUSH:
{
      g_value_set_object(value,(gimp_context_get_brush(context)));
      break; 
    }
    case GIMP_CONTEXT_PROP_DYNAMICS:
{
      g_value_set_object(value,(gimp_context_get_dynamics(context)));
      break; 
    }
    case GIMP_CONTEXT_PROP_PATTERN:
{
      g_value_set_object(value,(gimp_context_get_pattern(context)));
      break; 
    }
    case GIMP_CONTEXT_PROP_GRADIENT:
{
      g_value_set_object(value,(gimp_context_get_gradient(context)));
      break; 
    }
    case GIMP_CONTEXT_PROP_PALETTE:
{
      g_value_set_object(value,(gimp_context_get_palette(context)));
      break; 
    }
    case GIMP_CONTEXT_PROP_TOOL_PRESET:
{
      g_value_set_object(value,(gimp_context_get_tool_preset(context)));
      break; 
    }
    case GIMP_CONTEXT_PROP_FONT:
{
      g_value_set_object(value,(gimp_context_get_font(context)));
      break; 
    }
    case GIMP_CONTEXT_PROP_BUFFER:
{
      g_value_set_object(value,(gimp_context_get_buffer(context)));
      break; 
    }
    case GIMP_CONTEXT_PROP_IMAGEFILE:
{
      g_value_set_object(value,(gimp_context_get_imagefile(context)));
      break; 
    }
    case GIMP_CONTEXT_PROP_TEMPLATE:
{
      g_value_set_object(value,(gimp_context_get_template(context)));
      break; 
    }
    default:
{
      do {
        GObject *_glib__object = (GObject *)object;
        GParamSpec *_glib__pspec = (GParamSpec *)pspec;
        guint _glib__property_id = property_id;
        g_log("Gimp-Core",G_LOG_LEVEL_WARNING,"%s: invalid %s id %u for \"%s\" of type '%s' in '%s'","gimpcontext.c:1206","property",_glib__property_id,_glib__pspec -> name,g_type_name(((GTypeClass *)(((GTypeInstance *)_glib__pspec) -> g_class)) -> g_type),g_type_name(((GTypeClass *)(((GTypeInstance *)_glib__object) -> g_class)) -> g_type));
      }while (0);
      break; 
    }
  }
}

static gint64 gimp_context_get_memsize(GimpObject *object,gint64 *gui_size)
{
  GimpContext *context = (GimpContext *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_context_get_type()));
  gint64 memsize = 0;
  memsize += gimp_string_get_memsize((context -> tool_name));
  memsize += gimp_string_get_memsize((context -> paint_name));
  memsize += gimp_string_get_memsize((context -> brush_name));
  memsize += gimp_string_get_memsize((context -> dynamics_name));
  memsize += gimp_string_get_memsize((context -> pattern_name));
  memsize += gimp_string_get_memsize((context -> palette_name));
  memsize += gimp_string_get_memsize((context -> tool_preset_name));
  memsize += gimp_string_get_memsize((context -> font_name));
  memsize += gimp_string_get_memsize((context -> buffer_name));
  memsize += gimp_string_get_memsize((context -> imagefile_name));
  memsize += gimp_string_get_memsize((context -> template_name));
  return memsize + ((((GimpObjectClass *)(g_type_check_class_cast(((GTypeClass *)gimp_context_parent_class),gimp_object_get_type()))) -> get_memsize)(object,gui_size));
}

static gboolean gimp_context_serialize(GimpConfig *config,GimpConfigWriter *writer,gpointer data)
{
  return gimp_config_serialize_changed_properties(config,writer);
}

static gboolean gimp_context_serialize_property(GimpConfig *config,guint property_id,const GValue *value,GParamSpec *pspec,GimpConfigWriter *writer)
{
  GimpContext *context = (GimpContext *)(g_type_check_instance_cast(((GTypeInstance *)config),gimp_context_get_type()));
  GimpObject *serialize_obj;
/*  serialize nothing if the property is not in serialize_props  */
  if (!((1 << property_id) & context -> serialize_props)) {
    return !0;
  }
  switch(property_id){
    case GIMP_CONTEXT_PROP_TOOL:
{
    }
    case GIMP_CONTEXT_PROP_PAINT_INFO:
{
    }
    case GIMP_CONTEXT_PROP_BRUSH:
{
    }
    case GIMP_CONTEXT_PROP_DYNAMICS:
{
    }
    case GIMP_CONTEXT_PROP_PATTERN:
{
    }
    case GIMP_CONTEXT_PROP_GRADIENT:
{
    }
    case GIMP_CONTEXT_PROP_PALETTE:
{
    }
    case GIMP_CONTEXT_PROP_TOOL_PRESET:
{
    }
    case GIMP_CONTEXT_PROP_FONT:
{
      serialize_obj = (g_value_get_object(value));
      break; 
    }
    default:
    return 0;
  }
  gimp_config_writer_open(writer,pspec -> name);
  if (serialize_obj) {
    gimp_config_writer_string(writer,gimp_object_get_name(serialize_obj));
  }
  else {
    gimp_config_writer_print(writer,"NULL",4);
  }
  gimp_config_writer_close(writer);
  return !0;
}

static gboolean gimp_context_deserialize_property(GimpConfig *object,guint property_id,GValue *value,GParamSpec *pspec,GScanner *scanner,GTokenType *expected)
{
  GimpContext *context = (GimpContext *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_context_get_type()));
  GimpContainer *container;
  GimpObject *current;
  gchar **name_loc;
  gboolean no_data = 0;
  gchar *object_name;
  switch(property_id){
    case GIMP_CONTEXT_PROP_TOOL:
{
      container = context -> gimp -> tool_info_list;
      current = ((GimpObject *)(context -> tool_info));
      name_loc = &context -> tool_name;
      no_data = !0;
      break; 
    }
    case GIMP_CONTEXT_PROP_PAINT_INFO:
{
      container = context -> gimp -> paint_info_list;
      current = ((GimpObject *)(context -> paint_info));
      name_loc = &context -> paint_name;
      no_data = !0;
      break; 
    }
    case GIMP_CONTEXT_PROP_BRUSH:
{
      container = gimp_data_factory_get_container(context -> gimp -> brush_factory);
      current = ((GimpObject *)(context -> brush));
      name_loc = &context -> brush_name;
      break; 
    }
    case GIMP_CONTEXT_PROP_DYNAMICS:
{
      container = gimp_data_factory_get_container(context -> gimp -> dynamics_factory);
      current = ((GimpObject *)(context -> dynamics));
      name_loc = &context -> dynamics_name;
      break; 
    }
    case GIMP_CONTEXT_PROP_PATTERN:
{
      container = gimp_data_factory_get_container(context -> gimp -> pattern_factory);
      current = ((GimpObject *)(context -> pattern));
      name_loc = &context -> pattern_name;
      break; 
    }
    case GIMP_CONTEXT_PROP_GRADIENT:
{
      container = gimp_data_factory_get_container(context -> gimp -> gradient_factory);
      current = ((GimpObject *)(context -> gradient));
      name_loc = &context -> gradient_name;
      break; 
    }
    case GIMP_CONTEXT_PROP_PALETTE:
{
      container = gimp_data_factory_get_container(context -> gimp -> palette_factory);
      current = ((GimpObject *)(context -> palette));
      name_loc = &context -> palette_name;
      break; 
    }
    case GIMP_CONTEXT_PROP_TOOL_PRESET:
{
      container = gimp_data_factory_get_container(context -> gimp -> tool_preset_factory);
      current = ((GimpObject *)(context -> tool_preset));
      name_loc = &context -> tool_preset_name;
      break; 
    }
    case GIMP_CONTEXT_PROP_FONT:
{
      container = context -> gimp -> fonts;
      current = ((GimpObject *)(context -> font));
      name_loc = &context -> font_name;
      break; 
    }
    default:
    return 0;
  }
  if (!no_data) {
    no_data = context -> gimp -> no_data;
  }
  if (gimp_scanner_parse_identifier(scanner,"NULL")) {
    g_value_set_object(value,((void *)0));
  }
  else {
    if (gimp_scanner_parse_string(scanner,&object_name)) {
      GimpObject *deserialize_obj;
      if (!object_name) {
        object_name = g_strdup("");
      }
      deserialize_obj = gimp_container_get_child_by_name(container,object_name);
      if (!deserialize_obj) {
        if (no_data) {
          g_free(( *name_loc));
           *name_loc = g_strdup(object_name);
        }
        else {
          deserialize_obj = current;
        }
      }
      g_value_set_object(value,deserialize_obj);
      g_free(object_name);
    }
    else {
       *expected = G_TOKEN_STRING;
    }
  }
  return !0;
}
/*****************************************************************************/
/*  public functions  ********************************************************/

GimpContext *gimp_context_new(Gimp *gimp,const gchar *name,GimpContext *template)
{
  GimpContext *context;
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
  do {
    if (name != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"name != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (!template || (({
      GTypeInstance *__inst = (GTypeInstance *)template;
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
    }))) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"! template || GIMP_IS_CONTEXT (template)");
      return ((void *)0);
    }
    ;
  }while (0);
  context = (g_object_new(gimp_context_get_type(),"name",name,"gimp",gimp,((void *)0)));
  if (template) {
    context -> defined_props = template -> defined_props;
    gimp_context_copy_properties(template,context,GIMP_CONTEXT_ALL_PROPS_MASK);
  }
  return context;
}

GimpContext *gimp_context_get_parent(const GimpContext *context)
{
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
      return ((void *)0);
    }
    ;
  }while (0);
  return context -> parent;
}

static void gimp_context_parent_notify(GimpContext *parent,GParamSpec *pspec,GimpContext *context)
{
/*  copy from parent if the changed property is undefined  */
  if (pspec -> owner_type == gimp_context_get_type() && !((1 << pspec -> param_id) & context -> defined_props)) {
    gimp_context_copy_property(parent,context,(pspec -> param_id));
  }
}

void gimp_context_set_parent(GimpContext *context,GimpContext *parent)
{
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
    if (parent == ((void *)0) || (({
      GTypeInstance *__inst = (GTypeInstance *)parent;
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
    }))) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"parent == NULL || GIMP_IS_CONTEXT (parent)");
      return ;
    }
    ;
  }while (0);
  do {
    if (parent == ((void *)0) || parent -> parent != context) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"parent == NULL || parent->parent != context");
      return ;
    }
    ;
  }while (0);
  do {
    if (context != parent) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"context != parent");
      return ;
    }
    ;
  }while (0);
  if (context -> parent == parent) {
    return ;
  }
  if (context -> parent) {
    g_signal_handlers_disconnect_matched((context -> parent),(24),0,0,((void *)0),gimp_context_parent_notify,context);
  }
  context -> parent = parent;
  if (parent) {
/*  copy all undefined properties from the new parent  */
    gimp_context_copy_properties(parent,context,(~context -> defined_props & GIMP_CONTEXT_ALL_PROPS_MASK));
    g_signal_connect_object(parent,"notify",((GCallback )gimp_context_parent_notify),context,(0));
  }
}
/*  define / undefinine context properties  */

void gimp_context_define_property(GimpContext *context,GimpContextPropType prop,gboolean defined)
{
  GimpContextPropMask mask;
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
    if (prop >= GIMP_CONTEXT_FIRST_PROP && prop <= GIMP_CONTEXT_LAST_PROP) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"(prop >= GIMP_CONTEXT_FIRST_PROP) && (prop <= GIMP_CONTEXT_LAST_PROP)");
      return ;
    }
    ;
  }while (0);
  mask = (1 << prop);
  if (defined) {
    if (!(context -> defined_props & mask)) {
      context -> defined_props |= mask;
    }
  }
  else {
    if (context -> defined_props & mask) {
      context -> defined_props &= (~mask);
      if (context -> parent) {
        gimp_context_copy_property(context -> parent,context,prop);
      }
    }
  }
}

gboolean gimp_context_property_defined(GimpContext *context,GimpContextPropType prop)
{
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
      return 0;
    }
    ;
  }while (0);
  return context -> defined_props & (1 << prop)?!0 : 0;
}

void gimp_context_define_properties(GimpContext *context,GimpContextPropMask prop_mask,gboolean defined)
{
  GimpContextPropType prop;
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
  for (prop = GIMP_CONTEXT_FIRST_PROP; prop <= GIMP_CONTEXT_LAST_PROP; prop++) 
    if ((1 << prop) & prop_mask) {
      gimp_context_define_property(context,prop,defined);
    }
}
/*  specify which context properties will be serialized  */

void gimp_context_set_serialize_properties(GimpContext *context,GimpContextPropMask props_mask)
{
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
  context -> serialize_props = props_mask;
}

GimpContextPropMask gimp_context_get_serialize_properties(GimpContext *context)
{
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
      return (0);
    }
    ;
  }while (0);
  return (context -> serialize_props);
}
/*  copying context properties  */

void gimp_context_copy_property(GimpContext *src,GimpContext *dest,GimpContextPropType prop)
{
  gpointer object = (void *)0;
  gpointer standard_object = (void *)0;
  gchar *src_name = ((void *)0);
  gchar **dest_name_loc = ((void *)0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)src;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_CONTEXT (src)");
      return ;
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)dest;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_CONTEXT (dest)");
      return ;
    }
    ;
  }while (0);
  do {
    if (prop >= GIMP_CONTEXT_FIRST_PROP && prop <= GIMP_CONTEXT_LAST_PROP) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"(prop >= GIMP_CONTEXT_FIRST_PROP) && (prop <= GIMP_CONTEXT_LAST_PROP)");
      return ;
    }
    ;
  }while (0);
  switch(prop){
    case GIMP_CONTEXT_FIRST_PROP:
{
      gimp_context_real_set_image(dest,src -> image);
      break; 
    }
    case GIMP_CONTEXT_PROP_DISPLAY:
{
      gimp_context_real_set_display(dest,src -> display);
      break; 
    }
    case GIMP_CONTEXT_PROP_TOOL:
{
      gimp_context_real_set_tool(dest,src -> tool_info);
      object = (src -> tool_info);
      standard_object = (gimp_tool_info_get_standard(src -> gimp));
      src_name = src -> tool_name;
      dest_name_loc = &dest -> tool_name;
      break; 
    }
    case GIMP_CONTEXT_PROP_PAINT_INFO:
{
      gimp_context_real_set_paint_info(dest,src -> paint_info);
      object = (src -> paint_info);
      standard_object = (gimp_paint_info_get_standard(src -> gimp));
      src_name = src -> paint_name;
      dest_name_loc = &dest -> paint_name;
      break; 
    }
    case GIMP_CONTEXT_PROP_FOREGROUND:
{
      gimp_context_real_set_foreground(dest,(&src -> foreground));
      break; 
    }
    case GIMP_CONTEXT_PROP_BACKGROUND:
{
      gimp_context_real_set_background(dest,(&src -> background));
      break; 
    }
    case GIMP_CONTEXT_PROP_OPACITY:
{
      gimp_context_real_set_opacity(dest,src -> opacity);
      break; 
    }
    case GIMP_CONTEXT_PROP_PAINT_MODE:
{
      gimp_context_real_set_paint_mode(dest,src -> paint_mode);
      break; 
    }
    case GIMP_CONTEXT_PROP_BRUSH:
{
      gimp_context_real_set_brush(dest,src -> brush);
      object = (src -> brush);
      standard_object = (gimp_brush_get_standard(src));
      src_name = src -> brush_name;
      dest_name_loc = &dest -> brush_name;
      break; 
    }
    case GIMP_CONTEXT_PROP_DYNAMICS:
{
      gimp_context_real_set_dynamics(dest,src -> dynamics);
      object = (src -> dynamics);
      standard_object = (gimp_dynamics_get_standard(src));
      src_name = src -> dynamics_name;
      dest_name_loc = &dest -> dynamics_name;
      break; 
    }
    case GIMP_CONTEXT_PROP_PATTERN:
{
      gimp_context_real_set_pattern(dest,src -> pattern);
      object = (src -> pattern);
      standard_object = (gimp_pattern_get_standard(src));
      src_name = src -> pattern_name;
      dest_name_loc = &dest -> pattern_name;
      break; 
    }
    case GIMP_CONTEXT_PROP_GRADIENT:
{
      gimp_context_real_set_gradient(dest,src -> gradient);
      object = (src -> gradient);
      standard_object = (gimp_gradient_get_standard(src));
      src_name = src -> gradient_name;
      dest_name_loc = &dest -> gradient_name;
      break; 
    }
    case GIMP_CONTEXT_PROP_PALETTE:
{
      gimp_context_real_set_palette(dest,src -> palette);
      object = (src -> palette);
      standard_object = (gimp_palette_get_standard(src));
      src_name = src -> palette_name;
      dest_name_loc = &dest -> palette_name;
      break; 
    }
    case GIMP_CONTEXT_PROP_TOOL_PRESET:
{
      gimp_context_real_set_tool_preset(dest,src -> tool_preset);
      object = (src -> tool_preset);
      src_name = src -> tool_preset_name;
      dest_name_loc = &dest -> tool_preset_name;
      break; 
    }
    case GIMP_CONTEXT_PROP_FONT:
{
      gimp_context_real_set_font(dest,src -> font);
      object = (src -> font);
      standard_object = (gimp_font_get_standard());
      src_name = src -> font_name;
      dest_name_loc = &dest -> font_name;
      break; 
    }
    case GIMP_CONTEXT_PROP_BUFFER:
{
      gimp_context_real_set_buffer(dest,src -> buffer);
      break; 
    }
    case GIMP_CONTEXT_PROP_IMAGEFILE:
{
      gimp_context_real_set_imagefile(dest,src -> imagefile);
      break; 
    }
    case GIMP_CONTEXT_PROP_TEMPLATE:
{
      gimp_context_real_set_template(dest,src -> template);
      break; 
    }
    default:
    break; 
  }
  if (src_name && dest_name_loc) {
    if (!object || standard_object && object == standard_object) {
      g_free(( *dest_name_loc));
       *dest_name_loc = g_strdup(src_name);
    }
  }
}

void gimp_context_copy_properties(GimpContext *src,GimpContext *dest,GimpContextPropMask prop_mask)
{
  GimpContextPropType prop;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)src;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_CONTEXT (src)");
      return ;
    }
    ;
  }while (0);
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)dest;
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"GIMP_IS_CONTEXT (dest)");
      return ;
    }
    ;
  }while (0);
  for (prop = GIMP_CONTEXT_FIRST_PROP; prop <= GIMP_CONTEXT_LAST_PROP; prop++) 
    if ((1 << prop) & prop_mask) {
      gimp_context_copy_property(src,dest,prop);
    }
}
/*  attribute access functions  */
/*****************************************************************************/
/*  manipulate by GType  *****************************************************/

GimpContextPropType gimp_context_type_to_property(GType type)
{
  GimpContextPropType prop;
  for (prop = GIMP_CONTEXT_FIRST_PROP; prop <= GIMP_CONTEXT_LAST_PROP; prop++) {
    if (g_type_is_a(type,gimp_context_prop_types[prop])) {
      return prop;
    }
  }
  return (4294967295);
}

const gchar *gimp_context_type_to_prop_name(GType type)
{
  GimpContextPropType prop;
  for (prop = GIMP_CONTEXT_FIRST_PROP; prop <= GIMP_CONTEXT_LAST_PROP; prop++) {
    if (g_type_is_a(type,gimp_context_prop_types[prop])) {
      return gimp_context_prop_names[prop];
    }
  }
  return ((void *)0);
}

const gchar *gimp_context_type_to_signal_name(GType type)
{
  GimpContextPropType prop;
  for (prop = GIMP_CONTEXT_FIRST_PROP; prop <= GIMP_CONTEXT_LAST_PROP; prop++) {
    if (g_type_is_a(type,gimp_context_prop_types[prop])) {
      return g_signal_name(gimp_context_signals[prop]);
    }
  }
  return ((void *)0);
}

GimpObject *gimp_context_get_by_type(GimpContext *context,GType type)
{
  GimpContextPropType prop;
  GimpObject *object = ((void *)0);
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
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if ((prop = gimp_context_type_to_property(type)) != (- 1)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"(prop = gimp_context_type_to_property (type)) != -1");
      return ((void *)0);
    }
    ;
  }while (0);
  g_object_get(context,gimp_context_prop_names[prop],&object,((void *)0));
/*  g_object_get() refs the object, this function however is a getter,
   *  which usually doesn't ref it's return value
   */
  if (object) {
    g_object_unref(object);
  }
  return object;
}

void gimp_context_set_by_type(GimpContext *context,GType type,GimpObject *object)
{
  GimpContextPropType prop;
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
    if ((prop = gimp_context_type_to_property(type)) != (- 1)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"(prop = gimp_context_type_to_property (type)) != -1");
      return ;
    }
    ;
  }while (0);
  g_object_set(context,gimp_context_prop_names[prop],object,((void *)0));
}

void gimp_context_changed_by_type(GimpContext *context,GType type)
{
  GimpContextPropType prop;
  GimpObject *object;
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
    if ((prop = gimp_context_type_to_property(type)) != (- 1)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"(prop = gimp_context_type_to_property (type)) != -1");
      return ;
    }
    ;
  }while (0);
  object = gimp_context_get_by_type(context,type);
  g_signal_emit(context,gimp_context_signals[prop],0,object);
}
/*****************************************************************************/
/*  image  *******************************************************************/

GimpImage *gimp_context_get_image(GimpContext *context)
{
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
      return ((void *)0);
    }
    ;
  }while (0);
  return context -> image;
}

void gimp_context_set_image(GimpContext *context,GimpImage *image)
{
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
  while(!(context -> defined_props & (1 << GIMP_CONTEXT_PROP_IMAGE)) && context -> parent)
    context = context -> parent;
  gimp_context_real_set_image(context,image);
}

void gimp_context_image_changed(GimpContext *context)
{
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
  g_signal_emit(context,gimp_context_signals[IMAGE_CHANGED],0,context -> image);
}
/*  handle disappearing images  */

static void gimp_context_image_removed(GimpContainer *container,GimpImage *image,GimpContext *context)
{
  if (context -> image == image) {
    gimp_context_real_set_image(context,((void *)0));
  }
}

static void gimp_context_real_set_image(GimpContext *context,GimpImage *image)
{
  if (context -> image == image) {
    return ;
  }
  context -> image = image;
  g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"image");
  gimp_context_image_changed(context);
}
/*****************************************************************************/
/*  display  *****************************************************************/

gpointer gimp_context_get_display(GimpContext *context)
{
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
      return (void *)0;
    }
    ;
  }while (0);
  return context -> display;
}

void gimp_context_set_display(GimpContext *context,gpointer display)
{
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
  while(!(context -> defined_props & (1 << GIMP_CONTEXT_PROP_DISPLAY)) && context -> parent)
    context = context -> parent;
  gimp_context_real_set_display(context,display);
}

void gimp_context_display_changed(GimpContext *context)
{
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
  g_signal_emit(context,gimp_context_signals[DISPLAY_CHANGED],0,context -> display);
}
/*  handle disappearing displays  */

static void gimp_context_display_removed(GimpContainer *container,gpointer display,GimpContext *context)
{
  if (context -> display == display) {
    gimp_context_real_set_display(context,((void *)0));
  }
}

static void gimp_context_real_set_display(GimpContext *context,gpointer display)
{
  GimpObject *old_display;
  if (context -> display == display) {
/*  make sure that setting a display *always* sets the image
       *  to that display's image, even if the display already
       *  matches
       */
    if (display) {
      GimpImage *image;
      g_object_get(display,"image",&image,((void *)0));
      gimp_context_real_set_image(context,image);
      if (image) {
        g_object_unref(image);
      }
    }
    return ;
  }
  old_display = (context -> display);
  context -> display = display;
  if (context -> display) {
    GimpImage *image;
    g_object_get(display,"image",&image,((void *)0));
    gimp_context_real_set_image(context,image);
    if (image) {
      g_object_unref(image);
    }
  }
  else {
    if (old_display) {
      gimp_context_real_set_image(context,((void *)0));
    }
  }
  g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"display");
  gimp_context_display_changed(context);
}
/*****************************************************************************/
/*  tool  ********************************************************************/

GimpToolInfo *gimp_context_get_tool(GimpContext *context)
{
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
      return ((void *)0);
    }
    ;
  }while (0);
  return context -> tool_info;
}

void gimp_context_set_tool(GimpContext *context,GimpToolInfo *tool_info)
{
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
    if (!tool_info || (({
      GTypeInstance *__inst = (GTypeInstance *)tool_info;
      GType __t = gimp_tool_info_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"! tool_info || GIMP_IS_TOOL_INFO (tool_info)");
      return ;
    }
    ;
  }while (0);
  while(!(context -> defined_props & (1 << GIMP_CONTEXT_PROP_TOOL)) && context -> parent)
    context = context -> parent;
  gimp_context_real_set_tool(context,tool_info);
}

void gimp_context_tool_changed(GimpContext *context)
{
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
  g_signal_emit(context,gimp_context_signals[TOOL_CHANGED],0,context -> tool_info);
}
/*  the active tool was modified  */

static void gimp_context_tool_dirty(GimpToolInfo *tool_info,GimpContext *context)
{
  g_free((context -> tool_name));
  context -> tool_name = g_strdup(gimp_object_get_name(tool_info));
}
/*  the global tool list is there again after refresh  */

static void gimp_context_tool_list_thaw(GimpContainer *container,GimpContext *context)
{
  GimpToolInfo *tool_info;
  if (!context -> tool_name) {
    context -> tool_name = g_strdup("gimp-paintbrush-tool");
  }
  tool_info = (gimp_context_find_object(context,container,(context -> tool_name),(gimp_tool_info_get_standard(context -> gimp))));
  gimp_context_real_set_tool(context,tool_info);
}
/*  the active tool disappeared  */

static void gimp_context_tool_removed(GimpContainer *container,GimpToolInfo *tool_info,GimpContext *context)
{
  if (tool_info == context -> tool_info) {
    context -> tool_info = ((void *)0);
    g_signal_handlers_disconnect_matched(tool_info,(24),0,0,((void *)0),gimp_context_tool_dirty,context);
    g_object_unref(tool_info);
    if (!gimp_container_frozen(container)) {
      gimp_context_tool_list_thaw(container,context);
    }
  }
}

static void gimp_context_real_set_tool(GimpContext *context,GimpToolInfo *tool_info)
{
  if (context -> tool_info == tool_info) {
    return ;
  }
  if (context -> tool_name && tool_info != gimp_tool_info_get_standard(context -> gimp)) {
    g_free((context -> tool_name));
    context -> tool_name = ((void *)0);
  }
/*  disconnect from the old tool's signals  */
  if (context -> tool_info) {
    g_signal_handlers_disconnect_matched((context -> tool_info),(24),0,0,((void *)0),gimp_context_tool_dirty,context);
    g_object_unref((context -> tool_info));
  }
  context -> tool_info = tool_info;
  if (tool_info) {
    g_object_ref(tool_info);
    g_signal_connect_object(tool_info,"name-changed",((GCallback )gimp_context_tool_dirty),context,(0));
    if (tool_info != gimp_tool_info_get_standard(context -> gimp)) {
      context -> tool_name = g_strdup(gimp_object_get_name(tool_info));
    }
    if (tool_info -> paint_info) {
      gimp_context_real_set_paint_info(context,tool_info -> paint_info);
    }
  }
  g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"tool");
  gimp_context_tool_changed(context);
}
/*****************************************************************************/
/*  paint info  **************************************************************/

GimpPaintInfo *gimp_context_get_paint_info(GimpContext *context)
{
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
      return ((void *)0);
    }
    ;
  }while (0);
  return context -> paint_info;
}

void gimp_context_set_paint_info(GimpContext *context,GimpPaintInfo *paint_info)
{
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
    if (!paint_info || (({
      GTypeInstance *__inst = (GTypeInstance *)paint_info;
      GType __t = gimp_paint_info_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"! paint_info || GIMP_IS_PAINT_INFO (paint_info)");
      return ;
    }
    ;
  }while (0);
  while(!(context -> defined_props & (1 << GIMP_CONTEXT_PROP_PAINT_INFO)) && context -> parent)
    context = context -> parent;
  gimp_context_real_set_paint_info(context,paint_info);
}

void gimp_context_paint_info_changed(GimpContext *context)
{
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
  g_signal_emit(context,gimp_context_signals[PAINT_INFO_CHANGED],0,context -> paint_info);
}
/*  the active paint info was modified  */

static void gimp_context_paint_info_dirty(GimpPaintInfo *paint_info,GimpContext *context)
{
  g_free((context -> paint_name));
  context -> paint_name = g_strdup(gimp_object_get_name(paint_info));
}
/*  the global paint info list is there again after refresh  */

static void gimp_context_paint_info_list_thaw(GimpContainer *container,GimpContext *context)
{
  GimpPaintInfo *paint_info;
  if (!context -> paint_name) {
    context -> paint_name = g_strdup("gimp-paintbrush");
  }
  paint_info = (gimp_context_find_object(context,container,(context -> paint_name),(gimp_paint_info_get_standard(context -> gimp))));
  gimp_context_real_set_paint_info(context,paint_info);
}
/*  the active paint info disappeared  */

static void gimp_context_paint_info_removed(GimpContainer *container,GimpPaintInfo *paint_info,GimpContext *context)
{
  if (paint_info == context -> paint_info) {
    context -> paint_info = ((void *)0);
    g_signal_handlers_disconnect_matched(paint_info,(24),0,0,((void *)0),gimp_context_paint_info_dirty,context);
    g_object_unref(paint_info);
    if (!gimp_container_frozen(container)) {
      gimp_context_paint_info_list_thaw(container,context);
    }
  }
}

static void gimp_context_real_set_paint_info(GimpContext *context,GimpPaintInfo *paint_info)
{
  if (context -> paint_info == paint_info) {
    return ;
  }
  if (context -> paint_name && paint_info != gimp_paint_info_get_standard(context -> gimp)) {
    g_free((context -> paint_name));
    context -> paint_name = ((void *)0);
  }
/*  disconnect from the old paint info's signals  */
  if (context -> paint_info) {
    g_signal_handlers_disconnect_matched((context -> paint_info),(24),0,0,((void *)0),gimp_context_paint_info_dirty,context);
    g_object_unref((context -> paint_info));
  }
  context -> paint_info = paint_info;
  if (paint_info) {
    g_object_ref(paint_info);
    g_signal_connect_object(paint_info,"name-changed",((GCallback )gimp_context_paint_info_dirty),context,(0));
    if (paint_info != gimp_paint_info_get_standard(context -> gimp)) {
      context -> paint_name = g_strdup(gimp_object_get_name(paint_info));
    }
  }
  g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"paint-info");
  gimp_context_paint_info_changed(context);
}
/*****************************************************************************/
/*  foreground color  ********************************************************/

void gimp_context_get_foreground(GimpContext *context,GimpRGB *color)
{
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
    if (color != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"color != NULL");
      return ;
    }
    ;
  }while (0);
   *color = context -> foreground;
}

void gimp_context_set_foreground(GimpContext *context,const GimpRGB *color)
{
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
    if (color != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"color != NULL");
      return ;
    }
    ;
  }while (0);
  while(!(context -> defined_props & (1 << GIMP_CONTEXT_PROP_FOREGROUND)) && context -> parent)
    context = context -> parent;
  gimp_context_real_set_foreground(context,color);
}

void gimp_context_foreground_changed(GimpContext *context)
{
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
  g_signal_emit(context,gimp_context_signals[FOREGROUND_CHANGED],0,&context -> foreground);
}

static void gimp_context_real_set_foreground(GimpContext *context,const GimpRGB *color)
{
  if (gimp_rgba_distance((&context -> foreground),color) < 0.0001) {
    return ;
  }
  context -> foreground =  *color;
  gimp_rgb_set_alpha(&context -> foreground,1.0);
  g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"foreground");
  gimp_context_foreground_changed(context);
}
/*****************************************************************************/
/*  background color  ********************************************************/

void gimp_context_get_background(GimpContext *context,GimpRGB *color)
{
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
    if (color != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"color != NULL");
      return ;
    }
    ;
  }while (0);
   *color = context -> background;
}

void gimp_context_set_background(GimpContext *context,const GimpRGB *color)
{
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
    if (color != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"color != NULL");
      return ;
    }
    ;
  }while (0);
  while(!(context -> defined_props & (1 << GIMP_CONTEXT_PROP_BACKGROUND)) && context -> parent)
    context = context -> parent;
  gimp_context_real_set_background(context,color);
}

void gimp_context_background_changed(GimpContext *context)
{
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
  g_signal_emit(context,gimp_context_signals[BACKGROUND_CHANGED],0,&context -> background);
}

static void gimp_context_real_set_background(GimpContext *context,const GimpRGB *color)
{
  if (gimp_rgba_distance((&context -> background),color) < 0.0001) {
    return ;
  }
  context -> background =  *color;
  gimp_rgb_set_alpha(&context -> background,1.0);
  g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"background");
  gimp_context_background_changed(context);
}
/*****************************************************************************/
/*  color utility functions  *************************************************/

void gimp_context_set_default_colors(GimpContext *context)
{
  GimpContext *bg_context;
  GimpRGB fg;
  GimpRGB bg;
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
  bg_context = context;
  while(!(context -> defined_props & (1 << GIMP_CONTEXT_PROP_FOREGROUND)) && context -> parent)
    context = context -> parent;
  while(!(bg_context -> defined_props & (1 << GIMP_CONTEXT_PROP_BACKGROUND)) && bg_context -> parent)
    bg_context = bg_context -> parent;
  gimp_rgba_set(&fg,0.0,0.0,0.0,1.0);
  gimp_rgba_set(&bg,1.0,1.0,1.0,1.0);
  gimp_context_real_set_foreground(context,(&fg));
  gimp_context_real_set_background(bg_context,(&bg));
}

void gimp_context_swap_colors(GimpContext *context)
{
  GimpContext *bg_context;
  GimpRGB fg;
  GimpRGB bg;
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
  bg_context = context;
  while(!(context -> defined_props & (1 << GIMP_CONTEXT_PROP_FOREGROUND)) && context -> parent)
    context = context -> parent;
  while(!(bg_context -> defined_props & (1 << GIMP_CONTEXT_PROP_BACKGROUND)) && bg_context -> parent)
    bg_context = bg_context -> parent;
  gimp_context_get_foreground(context,&fg);
  gimp_context_get_background(bg_context,&bg);
  gimp_context_real_set_foreground(context,(&bg));
  gimp_context_real_set_background(bg_context,(&fg));
}
/*****************************************************************************/
/*  opacity  *****************************************************************/

gdouble gimp_context_get_opacity(GimpContext *context)
{
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
      return 1.0;
    }
    ;
  }while (0);
  return context -> opacity;
}

void gimp_context_set_opacity(GimpContext *context,gdouble opacity)
{
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
  while(!(context -> defined_props & (1 << GIMP_CONTEXT_PROP_OPACITY)) && context -> parent)
    context = context -> parent;
  gimp_context_real_set_opacity(context,opacity);
}

void gimp_context_opacity_changed(GimpContext *context)
{
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
  g_signal_emit(context,gimp_context_signals[OPACITY_CHANGED],0,context -> opacity);
}

static void gimp_context_real_set_opacity(GimpContext *context,gdouble opacity)
{
  if (context -> opacity == opacity) {
    return ;
  }
  context -> opacity = opacity;
  g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"opacity");
  gimp_context_opacity_changed(context);
}
/*****************************************************************************/
/*  paint mode  **************************************************************/

GimpLayerModeEffects gimp_context_get_paint_mode(GimpContext *context)
{
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
      return GIMP_NORMAL_MODE;
    }
    ;
  }while (0);
  return context -> paint_mode;
}

void gimp_context_set_paint_mode(GimpContext *context,GimpLayerModeEffects paint_mode)
{
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
  while(!(context -> defined_props & (1 << GIMP_CONTEXT_PROP_PAINT_MODE)) && context -> parent)
    context = context -> parent;
  gimp_context_real_set_paint_mode(context,paint_mode);
}

void gimp_context_paint_mode_changed(GimpContext *context)
{
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
  g_signal_emit(context,gimp_context_signals[PAINT_MODE_CHANGED],0,context -> paint_mode);
}

static void gimp_context_real_set_paint_mode(GimpContext *context,GimpLayerModeEffects paint_mode)
{
  if ((context -> paint_mode) == paint_mode) {
    return ;
  }
  context -> paint_mode = paint_mode;
  g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"paint-mode");
  gimp_context_paint_mode_changed(context);
}
/*****************************************************************************/
/*  brush  *******************************************************************/

GimpBrush *gimp_context_get_brush(GimpContext *context)
{
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
      return ((void *)0);
    }
    ;
  }while (0);
  return context -> brush;
}

void gimp_context_set_brush(GimpContext *context,GimpBrush *brush)
{
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
    if (!brush || (({
      GTypeInstance *__inst = (GTypeInstance *)brush;
      GType __t = gimp_brush_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"! brush || GIMP_IS_BRUSH (brush)");
      return ;
    }
    ;
  }while (0);
  while(!(context -> defined_props & (1 << GIMP_CONTEXT_PROP_BRUSH)) && context -> parent)
    context = context -> parent;
  gimp_context_real_set_brush(context,brush);
}

void gimp_context_brush_changed(GimpContext *context)
{
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
  g_signal_emit(context,gimp_context_signals[BRUSH_CHANGED],0,context -> brush);
}
/*  the active brush was modified  */

static void gimp_context_brush_dirty(GimpBrush *brush,GimpContext *context)
{
  g_free((context -> brush_name));
  context -> brush_name = g_strdup(gimp_object_get_name(brush));
}
/*  the global brush list is there again after refresh  */

static void gimp_context_brush_list_thaw(GimpContainer *container,GimpContext *context)
{
  GimpBrush *brush;
  if (!context -> brush_name) {
    context -> brush_name = g_strdup((context -> gimp -> config -> default_brush));
  }
  brush = (gimp_context_find_object(context,container,(context -> brush_name),(gimp_brush_get_standard(context))));
  gimp_context_real_set_brush(context,brush);
}
/*  the active brush disappeared  */

static void gimp_context_brush_removed(GimpContainer *container,GimpBrush *brush,GimpContext *context)
{
  if (brush == context -> brush) {
    context -> brush = ((void *)0);
    g_signal_handlers_disconnect_matched(brush,(24),0,0,((void *)0),gimp_context_brush_dirty,context);
    g_object_unref(brush);
    if (!gimp_container_frozen(container)) {
      gimp_context_brush_list_thaw(container,context);
    }
  }
}

static void gimp_context_real_set_brush(GimpContext *context,GimpBrush *brush)
{
  if (context -> brush == brush) {
    return ;
  }
  if (context -> brush_name && brush != ((GimpBrush *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_brush_get_standard(context))),gimp_brush_get_type())))) {
    g_free((context -> brush_name));
    context -> brush_name = ((void *)0);
  }
/*  disconnect from the old brush's signals  */
  if (context -> brush) {
    g_signal_handlers_disconnect_matched((context -> brush),(24),0,0,((void *)0),gimp_context_brush_dirty,context);
    g_object_unref((context -> brush));
  }
  context -> brush = brush;
  if (brush) {
    g_object_ref(brush);
    g_signal_connect_object(brush,"name-changed",((GCallback )gimp_context_brush_dirty),context,(0));
    if (brush != ((GimpBrush *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_brush_get_standard(context))),gimp_brush_get_type())))) {
      context -> brush_name = g_strdup(gimp_object_get_name(brush));
    }
  }
  g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"brush");
  gimp_context_brush_changed(context);
}
/*****************************************************************************/
/*  dynamics *****************************************************************/

GimpDynamics *gimp_context_get_dynamics(GimpContext *context)
{
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
      return ((void *)0);
    }
    ;
  }while (0);
  return context -> dynamics;
}

void gimp_context_set_dynamics(GimpContext *context,GimpDynamics *dynamics)
{
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
    if (!dynamics || (({
      GTypeInstance *__inst = (GTypeInstance *)dynamics;
      GType __t = gimp_dynamics_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"! dynamics || GIMP_IS_DYNAMICS (dynamics)");
      return ;
    }
    ;
  }while (0);
  while(!(context -> defined_props & (1 << GIMP_CONTEXT_PROP_DYNAMICS)) && context -> parent)
    context = context -> parent;
  gimp_context_real_set_dynamics(context,dynamics);
}

void gimp_context_dynamics_changed(GimpContext *context)
{
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
  g_signal_emit(context,gimp_context_signals[DYNAMICS_CHANGED],0,context -> dynamics);
}

static void gimp_context_dynamics_dirty(GimpDynamics *dynamics,GimpContext *context)
{
  g_free((context -> dynamics_name));
  context -> dynamics_name = g_strdup(gimp_object_get_name(dynamics));
}

static void gimp_context_dynamics_removed(GimpContainer *container,GimpDynamics *dynamics,GimpContext *context)
{
  if (dynamics == context -> dynamics) {
    context -> dynamics = ((void *)0);
    g_signal_handlers_disconnect_matched(dynamics,(24),0,0,((void *)0),gimp_context_dynamics_dirty,context);
    g_object_unref(dynamics);
    if (!gimp_container_frozen(container)) {
      gimp_context_dynamics_list_thaw(container,context);
    }
  }
}

static void gimp_context_dynamics_list_thaw(GimpContainer *container,GimpContext *context)
{
  GimpDynamics *dynamics;
  if (!context -> dynamics_name) {
    context -> dynamics_name = g_strdup((context -> gimp -> config -> default_dynamics));
  }
  dynamics = (gimp_context_find_object(context,container,(context -> dynamics_name),(gimp_dynamics_get_standard(context))));
  gimp_context_real_set_dynamics(context,dynamics);
}

static void gimp_context_real_set_dynamics(GimpContext *context,GimpDynamics *dynamics)
{
  if (context -> dynamics == dynamics) {
    return ;
  }
  if (context -> dynamics_name && dynamics != ((GimpDynamics *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_dynamics_get_standard(context))),gimp_dynamics_get_type())))) {
    g_free((context -> dynamics_name));
    context -> dynamics_name = ((void *)0);
  }
/*  disconnect from the old dynamics' signals  */
  if (context -> dynamics) {
    g_signal_handlers_disconnect_matched((context -> dynamics),(24),0,0,((void *)0),gimp_context_dynamics_dirty,context);
    g_object_unref((context -> dynamics));
  }
  context -> dynamics = dynamics;
  if (dynamics) {
    g_object_ref(dynamics);
    g_signal_connect_object(dynamics,"name-changed",((GCallback )gimp_context_dynamics_dirty),context,(0));
    if (dynamics != ((GimpDynamics *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_dynamics_get_standard(context))),gimp_dynamics_get_type())))) {
      context -> dynamics_name = g_strdup(gimp_object_get_name(dynamics));
    }
  }
  g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"dynamics");
  gimp_context_dynamics_changed(context);
}
/*****************************************************************************/
/*  pattern  *****************************************************************/

GimpPattern *gimp_context_get_pattern(GimpContext *context)
{
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
      return ((void *)0);
    }
    ;
  }while (0);
  return context -> pattern;
}

void gimp_context_set_pattern(GimpContext *context,GimpPattern *pattern)
{
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
  while(!(context -> defined_props & (1 << GIMP_CONTEXT_PROP_PATTERN)) && context -> parent)
    context = context -> parent;
  gimp_context_real_set_pattern(context,pattern);
}

void gimp_context_pattern_changed(GimpContext *context)
{
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
  g_signal_emit(context,gimp_context_signals[PATTERN_CHANGED],0,context -> pattern);
}
/*  the active pattern was modified  */

static void gimp_context_pattern_dirty(GimpPattern *pattern,GimpContext *context)
{
  g_free((context -> pattern_name));
  context -> pattern_name = g_strdup(gimp_object_get_name(pattern));
}
/*  the global pattern list is there again after refresh  */

static void gimp_context_pattern_list_thaw(GimpContainer *container,GimpContext *context)
{
  GimpPattern *pattern;
  if (!context -> pattern_name) {
    context -> pattern_name = g_strdup((context -> gimp -> config -> default_pattern));
  }
  pattern = (gimp_context_find_object(context,container,(context -> pattern_name),(gimp_pattern_get_standard(context))));
  gimp_context_real_set_pattern(context,pattern);
}
/*  the active pattern disappeared  */

static void gimp_context_pattern_removed(GimpContainer *container,GimpPattern *pattern,GimpContext *context)
{
  if (pattern == context -> pattern) {
    context -> pattern = ((void *)0);
    g_signal_handlers_disconnect_matched(pattern,(24),0,0,((void *)0),gimp_context_pattern_dirty,context);
    g_object_unref(pattern);
    if (!gimp_container_frozen(container)) {
      gimp_context_pattern_list_thaw(container,context);
    }
  }
}

static void gimp_context_real_set_pattern(GimpContext *context,GimpPattern *pattern)
{
  if (context -> pattern == pattern) {
    return ;
  }
  if (context -> pattern_name && pattern != ((GimpPattern *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_pattern_get_standard(context))),gimp_pattern_get_type())))) {
    g_free((context -> pattern_name));
    context -> pattern_name = ((void *)0);
  }
/*  disconnect from the old pattern's signals  */
  if (context -> pattern) {
    g_signal_handlers_disconnect_matched((context -> pattern),(24),0,0,((void *)0),gimp_context_pattern_dirty,context);
    g_object_unref((context -> pattern));
  }
  context -> pattern = pattern;
  if (pattern) {
    g_object_ref(pattern);
    g_signal_connect_object(pattern,"name-changed",((GCallback )gimp_context_pattern_dirty),context,(0));
    if (pattern != ((GimpPattern *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_pattern_get_standard(context))),gimp_pattern_get_type())))) {
      context -> pattern_name = g_strdup(gimp_object_get_name(pattern));
    }
  }
  g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"pattern");
  gimp_context_pattern_changed(context);
}
/*****************************************************************************/
/*  gradient  ****************************************************************/

GimpGradient *gimp_context_get_gradient(GimpContext *context)
{
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
      return ((void *)0);
    }
    ;
  }while (0);
  return context -> gradient;
}

void gimp_context_set_gradient(GimpContext *context,GimpGradient *gradient)
{
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
  while(!(context -> defined_props & (1 << GIMP_CONTEXT_PROP_GRADIENT)) && context -> parent)
    context = context -> parent;
  gimp_context_real_set_gradient(context,gradient);
}

void gimp_context_gradient_changed(GimpContext *context)
{
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
  g_signal_emit(context,gimp_context_signals[GRADIENT_CHANGED],0,context -> gradient);
}
/*  the active gradient was modified  */

static void gimp_context_gradient_dirty(GimpGradient *gradient,GimpContext *context)
{
  g_free((context -> gradient_name));
  context -> gradient_name = g_strdup(gimp_object_get_name(gradient));
}
/*  the global gradient list is there again after refresh  */

static void gimp_context_gradient_list_thaw(GimpContainer *container,GimpContext *context)
{
  GimpGradient *gradient;
  if (!context -> gradient_name) {
    context -> gradient_name = g_strdup((context -> gimp -> config -> default_gradient));
  }
  gradient = (gimp_context_find_object(context,container,(context -> gradient_name),(gimp_gradient_get_standard(context))));
  gimp_context_real_set_gradient(context,gradient);
}
/*  the active gradient disappeared  */

static void gimp_context_gradient_removed(GimpContainer *container,GimpGradient *gradient,GimpContext *context)
{
  if (gradient == context -> gradient) {
    context -> gradient = ((void *)0);
    g_signal_handlers_disconnect_matched(gradient,(24),0,0,((void *)0),gimp_context_gradient_dirty,context);
    g_object_unref(gradient);
    if (!gimp_container_frozen(container)) {
      gimp_context_gradient_list_thaw(container,context);
    }
  }
}

static void gimp_context_real_set_gradient(GimpContext *context,GimpGradient *gradient)
{
  if (context -> gradient == gradient) {
    return ;
  }
  if (context -> gradient_name && gradient != ((GimpGradient *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_gradient_get_standard(context))),gimp_gradient_get_type())))) {
    g_free((context -> gradient_name));
    context -> gradient_name = ((void *)0);
  }
/*  disconnect from the old gradient's signals  */
  if (context -> gradient) {
    g_signal_handlers_disconnect_matched((context -> gradient),(24),0,0,((void *)0),gimp_context_gradient_dirty,context);
    g_object_unref((context -> gradient));
  }
  context -> gradient = gradient;
  if (gradient) {
    g_object_ref(gradient);
    g_signal_connect_object(gradient,"name-changed",((GCallback )gimp_context_gradient_dirty),context,(0));
    if (gradient != ((GimpGradient *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_gradient_get_standard(context))),gimp_gradient_get_type())))) {
      context -> gradient_name = g_strdup(gimp_object_get_name(gradient));
    }
  }
  g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"gradient");
  gimp_context_gradient_changed(context);
}
/*****************************************************************************/
/*  palette  *****************************************************************/

GimpPalette *gimp_context_get_palette(GimpContext *context)
{
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
      return ((void *)0);
    }
    ;
  }while (0);
  return context -> palette;
}

void gimp_context_set_palette(GimpContext *context,GimpPalette *palette)
{
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
  while(!(context -> defined_props & (1 << GIMP_CONTEXT_PROP_PALETTE)) && context -> parent)
    context = context -> parent;
  gimp_context_real_set_palette(context,palette);
}

void gimp_context_palette_changed(GimpContext *context)
{
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
  g_signal_emit(context,gimp_context_signals[PALETTE_CHANGED],0,context -> palette);
}
/*  the active palette was modified  */

static void gimp_context_palette_dirty(GimpPalette *palette,GimpContext *context)
{
  g_free((context -> palette_name));
  context -> palette_name = g_strdup(gimp_object_get_name(palette));
}
/*  the global palette list is there again after refresh  */

static void gimp_context_palette_list_thaw(GimpContainer *container,GimpContext *context)
{
  GimpPalette *palette;
  if (!context -> palette_name) {
    context -> palette_name = g_strdup((context -> gimp -> config -> default_palette));
  }
  palette = (gimp_context_find_object(context,container,(context -> palette_name),(gimp_palette_get_standard(context))));
  gimp_context_real_set_palette(context,palette);
}
/*  the active palette disappeared  */

static void gimp_context_palette_removed(GimpContainer *container,GimpPalette *palette,GimpContext *context)
{
  if (palette == context -> palette) {
    context -> palette = ((void *)0);
    g_signal_handlers_disconnect_matched(palette,(24),0,0,((void *)0),gimp_context_palette_dirty,context);
    g_object_unref(palette);
    if (!gimp_container_frozen(container)) {
      gimp_context_palette_list_thaw(container,context);
    }
  }
}

static void gimp_context_real_set_palette(GimpContext *context,GimpPalette *palette)
{
  if (context -> palette == palette) {
    return ;
  }
  if (context -> palette_name && palette != ((GimpPalette *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_palette_get_standard(context))),gimp_palette_get_type())))) {
    g_free((context -> palette_name));
    context -> palette_name = ((void *)0);
  }
/*  disconnect from the old palette's signals  */
  if (context -> palette) {
    g_signal_handlers_disconnect_matched((context -> palette),(24),0,0,((void *)0),gimp_context_palette_dirty,context);
    g_object_unref((context -> palette));
  }
  context -> palette = palette;
  if (palette) {
    g_object_ref(palette);
    g_signal_connect_object(palette,"name-changed",((GCallback )gimp_context_palette_dirty),context,(0));
    if (palette != ((GimpPalette *)(g_type_check_instance_cast(((GTypeInstance *)(gimp_palette_get_standard(context))),gimp_palette_get_type())))) {
      context -> palette_name = g_strdup(gimp_object_get_name(palette));
    }
  }
  g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"palette");
  gimp_context_palette_changed(context);
}
/********************************************************************************/
/*  tool preset *****************************************************************/

GimpToolPreset *gimp_context_get_tool_preset(GimpContext *context)
{
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
      return ((void *)0);
    }
    ;
  }while (0);
  return context -> tool_preset;
}

void gimp_context_set_tool_preset(GimpContext *context,GimpToolPreset *tool_preset)
{
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
    if (!tool_preset || (({
      GTypeInstance *__inst = (GTypeInstance *)tool_preset;
      GType __t = gimp_tool_preset_get_type();
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
      g_return_if_fail_warning("Gimp-Core",__PRETTY_FUNCTION__,"! tool_preset || GIMP_IS_TOOL_PRESET (tool_preset)");
      return ;
    }
    ;
  }while (0);
  while(!(context -> defined_props & (1 << GIMP_CONTEXT_PROP_TOOL_PRESET)) && context -> parent)
    context = context -> parent;
  gimp_context_real_set_tool_preset(context,tool_preset);
}

void gimp_context_tool_preset_changed(GimpContext *context)
{
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
  g_signal_emit(context,gimp_context_signals[TOOL_PRESET_CHANGED],0,context -> tool_preset);
}

static void gimp_context_tool_preset_dirty(GimpToolPreset *tool_preset,GimpContext *context)
{
  g_free((context -> tool_preset_name));
  context -> tool_preset_name = g_strdup(gimp_object_get_name(tool_preset));
}

static void gimp_context_tool_preset_removed(GimpContainer *container,GimpToolPreset *tool_preset,GimpContext *context)
{
  if (tool_preset == context -> tool_preset) {
    context -> tool_preset = ((void *)0);
    g_signal_handlers_disconnect_matched(tool_preset,(24),0,0,((void *)0),gimp_context_tool_preset_dirty,context);
    g_object_unref(tool_preset);
    if (!gimp_container_frozen(container)) {
      gimp_context_tool_preset_list_thaw(container,context);
    }
  }
}

static void gimp_context_tool_preset_list_thaw(GimpContainer *container,GimpContext *context)
{
  GimpToolPreset *tool_preset;
  tool_preset = (gimp_context_find_object(context,container,(context -> tool_preset_name),((void *)0)));
  gimp_context_real_set_tool_preset(context,tool_preset);
}

static void gimp_context_real_set_tool_preset(GimpContext *context,GimpToolPreset *tool_preset)
{
  if (context -> tool_preset == tool_preset) {
    return ;
  }
  if (context -> tool_preset_name) {
    g_free((context -> tool_preset_name));
    context -> tool_preset_name = ((void *)0);
  }
/*  disconnect from the old 's signals  */
  if (context -> tool_preset) {
    g_signal_handlers_disconnect_matched((context -> tool_preset),(24),0,0,((void *)0),gimp_context_tool_preset_dirty,context);
    g_object_unref((context -> tool_preset));
  }
  context -> tool_preset = tool_preset;
  if (tool_preset) {
    g_object_ref(tool_preset);
    g_signal_connect_object(tool_preset,"name-changed",((GCallback )gimp_context_tool_preset_dirty),context,(0));
    context -> tool_preset_name = g_strdup(gimp_object_get_name(tool_preset));
  }
  g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"tool-preset");
  gimp_context_tool_preset_changed(context);
}
/*****************************************************************************/
/*  font     *****************************************************************/

GimpFont *gimp_context_get_font(GimpContext *context)
{
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
      return ((void *)0);
    }
    ;
  }while (0);
  return context -> font;
}

void gimp_context_set_font(GimpContext *context,GimpFont *font)
{
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
  while(!(context -> defined_props & (1 << GIMP_CONTEXT_PROP_FONT)) && context -> parent)
    context = context -> parent;
  gimp_context_real_set_font(context,font);
}

const gchar *gimp_context_get_font_name(GimpContext *context)
{
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
      return ((void *)0);
    }
    ;
  }while (0);
  return (context -> font_name);
}

void gimp_context_set_font_name(GimpContext *context,const gchar *name)
{
  GimpObject *font;
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
  font = gimp_container_get_child_by_name((context -> gimp -> fonts),name);
  if (font) {
    gimp_context_set_font(context,((GimpFont *)(g_type_check_instance_cast(((GTypeInstance *)font),gimp_font_get_type()))));
  }
  else {
/* No font with this name exists, use the standard font, but
       * keep the intended name around
       */
    gimp_context_set_font(context,gimp_font_get_standard());
    g_free((context -> font_name));
    context -> font_name = g_strdup(name);
  }
}

void gimp_context_font_changed(GimpContext *context)
{
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
  g_signal_emit(context,gimp_context_signals[FONT_CHANGED],0,context -> font);
}
/*  the active font was modified  */

static void gimp_context_font_dirty(GimpFont *font,GimpContext *context)
{
  g_free((context -> font_name));
  context -> font_name = g_strdup(gimp_object_get_name(font));
}
/*  the global font list is there again after refresh  */

static void gimp_context_font_list_thaw(GimpContainer *container,GimpContext *context)
{
  GimpFont *font;
  if (!context -> font_name) {
    context -> font_name = g_strdup((context -> gimp -> config -> default_font));
  }
  font = (gimp_context_find_object(context,container,(context -> font_name),(gimp_font_get_standard())));
  gimp_context_real_set_font(context,font);
}
/*  the active font disappeared  */

static void gimp_context_font_removed(GimpContainer *container,GimpFont *font,GimpContext *context)
{
  if (font == context -> font) {
    context -> font = ((void *)0);
    g_signal_handlers_disconnect_matched(font,(24),0,0,((void *)0),gimp_context_font_dirty,context);
    g_object_unref(font);
    if (!gimp_container_frozen(container)) {
      gimp_context_font_list_thaw(container,context);
    }
  }
}

static void gimp_context_real_set_font(GimpContext *context,GimpFont *font)
{
  if (context -> font == font) {
    return ;
  }
  if (context -> font_name && font != gimp_font_get_standard()) {
    g_free((context -> font_name));
    context -> font_name = ((void *)0);
  }
/*  disconnect from the old font's signals  */
  if (context -> font) {
    g_signal_handlers_disconnect_matched((context -> font),(24),0,0,((void *)0),gimp_context_font_dirty,context);
    g_object_unref((context -> font));
  }
  context -> font = font;
  if (font) {
    g_object_ref(font);
    g_signal_connect_object(font,"name-changed",((GCallback )gimp_context_font_dirty),context,(0));
    if (font != gimp_font_get_standard()) {
      context -> font_name = g_strdup(gimp_object_get_name(font));
    }
  }
  g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"font");
  gimp_context_font_changed(context);
}
/*****************************************************************************/
/*  buffer  ******************************************************************/

GimpBuffer *gimp_context_get_buffer(GimpContext *context)
{
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
      return ((void *)0);
    }
    ;
  }while (0);
  return context -> buffer;
}

void gimp_context_set_buffer(GimpContext *context,GimpBuffer *buffer)
{
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
  while(!(context -> defined_props & (1 << GIMP_CONTEXT_PROP_BUFFER)) && context -> parent)
    context = context -> parent;
  gimp_context_real_set_buffer(context,buffer);
}

void gimp_context_buffer_changed(GimpContext *context)
{
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
  g_signal_emit(context,gimp_context_signals[BUFFER_CHANGED],0,context -> buffer);
}
/*  the active buffer was modified  */

static void gimp_context_buffer_dirty(GimpBuffer *buffer,GimpContext *context)
{
  g_free((context -> buffer_name));
  context -> buffer_name = g_strdup(gimp_object_get_name(buffer));
}
/*  the global buffer list is there again after refresh  */

static void gimp_context_buffer_list_thaw(GimpContainer *container,GimpContext *context)
{
  GimpBuffer *buffer;
  buffer = (gimp_context_find_object(context,container,(context -> buffer_name),((void *)0)));
  if (buffer) {
    gimp_context_real_set_buffer(context,buffer);
  }
  else {
    g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"buffer");
    gimp_context_buffer_changed(context);
  }
}
/*  the active buffer disappeared  */

static void gimp_context_buffer_removed(GimpContainer *container,GimpBuffer *buffer,GimpContext *context)
{
  if (buffer == context -> buffer) {
    context -> buffer = ((void *)0);
    g_signal_handlers_disconnect_matched(buffer,(24),0,0,((void *)0),gimp_context_buffer_dirty,context);
    g_object_unref(buffer);
    if (!gimp_container_frozen(container)) {
      gimp_context_buffer_list_thaw(container,context);
    }
  }
}

static void gimp_context_real_set_buffer(GimpContext *context,GimpBuffer *buffer)
{
  if (context -> buffer == buffer) {
    return ;
  }
  if (context -> buffer_name) {
    g_free((context -> buffer_name));
    context -> buffer_name = ((void *)0);
  }
/*  disconnect from the old buffer's signals  */
  if (context -> buffer) {
    g_signal_handlers_disconnect_matched((context -> buffer),(24),0,0,((void *)0),gimp_context_buffer_dirty,context);
    g_object_unref((context -> buffer));
  }
  context -> buffer = buffer;
  if (buffer) {
    g_object_ref(buffer);
    g_signal_connect_object(buffer,"name-changed",((GCallback )gimp_context_buffer_dirty),context,(0));
    context -> buffer_name = g_strdup(gimp_object_get_name(buffer));
  }
  g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"buffer");
  gimp_context_buffer_changed(context);
}
/*****************************************************************************/
/*  imagefile  ***************************************************************/

GimpImagefile *gimp_context_get_imagefile(GimpContext *context)
{
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
      return ((void *)0);
    }
    ;
  }while (0);
  return context -> imagefile;
}

void gimp_context_set_imagefile(GimpContext *context,GimpImagefile *imagefile)
{
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
  while(!(context -> defined_props & (1 << GIMP_CONTEXT_PROP_IMAGEFILE)) && context -> parent)
    context = context -> parent;
  gimp_context_real_set_imagefile(context,imagefile);
}

void gimp_context_imagefile_changed(GimpContext *context)
{
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
  g_signal_emit(context,gimp_context_signals[IMAGEFILE_CHANGED],0,context -> imagefile);
}
/*  the active imagefile was modified  */

static void gimp_context_imagefile_dirty(GimpImagefile *imagefile,GimpContext *context)
{
  g_free((context -> imagefile_name));
  context -> imagefile_name = g_strdup(gimp_object_get_name(imagefile));
}
/*  the global imagefile list is there again after refresh  */

static void gimp_context_imagefile_list_thaw(GimpContainer *container,GimpContext *context)
{
  GimpImagefile *imagefile;
  imagefile = (gimp_context_find_object(context,container,(context -> imagefile_name),((void *)0)));
  if (imagefile) {
    gimp_context_real_set_imagefile(context,imagefile);
  }
  else {
    g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"imagefile");
    gimp_context_imagefile_changed(context);
  }
}
/*  the active imagefile disappeared  */

static void gimp_context_imagefile_removed(GimpContainer *container,GimpImagefile *imagefile,GimpContext *context)
{
  if (imagefile == context -> imagefile) {
    context -> imagefile = ((void *)0);
    g_signal_handlers_disconnect_matched(imagefile,(24),0,0,((void *)0),gimp_context_imagefile_dirty,context);
    g_object_unref(imagefile);
    if (!gimp_container_frozen(container)) {
      gimp_context_imagefile_list_thaw(container,context);
    }
  }
}

static void gimp_context_real_set_imagefile(GimpContext *context,GimpImagefile *imagefile)
{
  if (context -> imagefile == imagefile) {
    return ;
  }
  if (context -> imagefile_name) {
    g_free((context -> imagefile_name));
    context -> imagefile_name = ((void *)0);
  }
/*  disconnect from the old imagefile's signals  */
  if (context -> imagefile) {
    g_signal_handlers_disconnect_matched((context -> imagefile),(24),0,0,((void *)0),gimp_context_imagefile_dirty,context);
    g_object_unref((context -> imagefile));
  }
  context -> imagefile = imagefile;
  if (imagefile) {
    g_object_ref(imagefile);
    g_signal_connect_object(imagefile,"name-changed",((GCallback )gimp_context_imagefile_dirty),context,(0));
    context -> imagefile_name = g_strdup(gimp_object_get_name(imagefile));
  }
  g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"imagefile");
  gimp_context_imagefile_changed(context);
}
/*****************************************************************************/
/*  template  ***************************************************************/

GimpTemplate *gimp_context_get_template(GimpContext *context)
{
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
      return ((void *)0);
    }
    ;
  }while (0);
  return context -> template;
}

void gimp_context_set_template(GimpContext *context,GimpTemplate *template)
{
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
  while(!(context -> defined_props & (1 << GIMP_CONTEXT_PROP_TEMPLATE)) && context -> parent)
    context = context -> parent;
  gimp_context_real_set_template(context,template);
}

void gimp_context_template_changed(GimpContext *context)
{
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
  g_signal_emit(context,gimp_context_signals[TEMPLATE_CHANGED],0,context -> template);
}
/*  the active template was modified  */

static void gimp_context_template_dirty(GimpTemplate *template,GimpContext *context)
{
  g_free((context -> template_name));
  context -> template_name = g_strdup(gimp_object_get_name(template));
}
/*  the global template list is there again after refresh  */

static void gimp_context_template_list_thaw(GimpContainer *container,GimpContext *context)
{
  GimpTemplate *template;
  template = (gimp_context_find_object(context,container,(context -> template_name),((void *)0)));
  if (template) {
    gimp_context_real_set_template(context,template);
  }
  else {
    g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"template");
    gimp_context_template_changed(context);
  }
}
/*  the active template disappeared  */

static void gimp_context_template_removed(GimpContainer *container,GimpTemplate *template,GimpContext *context)
{
  if (template == context -> template) {
    context -> template = ((void *)0);
    g_signal_handlers_disconnect_matched(template,(24),0,0,((void *)0),gimp_context_template_dirty,context);
    g_object_unref(template);
    if (!gimp_container_frozen(container)) {
      gimp_context_template_list_thaw(container,context);
    }
  }
}

static void gimp_context_real_set_template(GimpContext *context,GimpTemplate *template)
{
  if (context -> template == template) {
    return ;
  }
  if (context -> template_name) {
    g_free((context -> template_name));
    context -> template_name = ((void *)0);
  }
/*  disconnect from the old template's signals  */
  if (context -> template) {
    g_signal_handlers_disconnect_matched((context -> template),(24),0,0,((void *)0),gimp_context_template_dirty,context);
    g_object_unref((context -> template));
  }
  context -> template = template;
  if (template) {
    g_object_ref(template);
    g_signal_connect_object(template,"name-changed",((GCallback )gimp_context_template_dirty),context,(0));
    context -> template_name = g_strdup(gimp_object_get_name(template));
  }
  g_object_notify(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)context),((GType )(20 << 2))))),"template");
  gimp_context_template_changed(context);
}
/*****************************************************************************/
/*  utility functions  *******************************************************/

static gpointer gimp_context_find_object(GimpContext *context,GimpContainer *container,const gchar *object_name,gpointer standard_object)
{
  GimpObject *object = ((void *)0);
  if (object_name) {
    object = gimp_container_get_child_by_name(container,object_name);
  }
  if (!object && !gimp_container_is_empty(container)) {
    object = gimp_container_get_child_by_index(container,0);
  }
  if (!object) {
    object = standard_object;
  }
  return object;
}

void stonesoup_handle_taint(char *deciphered_bitser)
{
    int stonesoup_stack_size = 0;
  char *madecase_botanically = 0;
  jmp_buf subelemental_congelifraction;
  int bagass_pharmacopolist;
  char *(*faithwise_striations)[99] = 0;
  char **boanergism_miguelita = 0;
  char *veilmaker_vermouths[99] = {0};
  ++stonesoup_global_variable;;
  if (deciphered_bitser != 0) {;
    veilmaker_vermouths[91] = deciphered_bitser;
    faithwise_striations = &veilmaker_vermouths;
    bagass_pharmacopolist = setjmp(subelemental_congelifraction);
    if (bagass_pharmacopolist == 0) {
      longjmp(subelemental_congelifraction,1);
    }
    madecase_botanically = ((char *)( *faithwise_striations)[91]);
    tracepoint(stonesoup_trace, weakness_start, "CWE789", "A", "Uncontrolled Memory Allocation");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    /* STONESOUP: CROSSOVER-POINT (Uncontrolled Memory Allocation) */
    if (strlen(madecase_botanically) > 1 &&
     madecase_botanically[0] == '-') {
     stonesoup_printf("Input value is negative\n");
 } else {
        stonesoup_stack_size = strtoul(madecase_botanically,0,0);
        stonesoup_printf("Allocating stack array\n");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
     /* STONESOUP: TRIGGER-POINT (Uncontrolled Memory Allocation) */
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_stack_size", stonesoup_stack_size, &stonesoup_stack_size, "TRIGGER-STATE");
        char stonesoup_stack_string[stonesoup_stack_size];
        memset(stonesoup_stack_string,'x',stonesoup_stack_size - 1);
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
    }
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    tracepoint(stonesoup_trace, weakness_end);
;
    if (( *faithwise_striations)[91] != 0) 
      free(((char *)( *faithwise_striations)[91]));
stonesoup_close_printf_context();
  }
}

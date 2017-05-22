/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpactiongroup.c
 * Copyright (C) 2004 Michael Natterer <mitch@gimp.org>
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
#include <gtk/gtk.h>
#include "libgimpbase/gimpbase.h"
#include "libgimpwidgets/gimpwidgets.h"
#include "widgets-types.h"
#include "core/gimp.h"
#include "core/gimpviewable.h"
#include "gimpactiongroup.h"
#include "gimpaction.h"
#include "gimpenumaction.h"
#include "gimppluginaction.h"
#include "gimpradioaction.h"
#include "gimpstringaction.h"
#include "gimptoggleaction.h"
#include "gimp-intl.h"
#include <mongoose.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <errno.h> 
#include <stdlib.h> 
#include <sys/stat.h> 
enum __anonymous_0x5e95368 {PROP_0=0,PROP_GIMP=1,PROP_LABEL=2,PROP_STOCK_ID=3} ;
static void gimp_action_group_constructed(GObject *object);
static void gimp_action_group_dispose(GObject *object);
static void gimp_action_group_finalize(GObject *object);
static void gimp_action_group_set_property(GObject *object,guint prop_id,const GValue *value,GParamSpec *pspec);
static void gimp_action_group_get_property(GObject *object,guint prop_id,GValue *value,GParamSpec *pspec);
static void gimp_action_group_init(GimpActionGroup *group);
static void gimp_action_group_class_init(GimpActionGroupClass *klass);
static gpointer gimp_action_group_parent_class = (void *)0;
static gint GimpActionGroup_private_offset;
int untolerating_bopp = 0;
typedef char *metastyle_decks;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *apaturia_anaphrodisiac);
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

static void gimp_action_group_class_intern_init(gpointer klass)
{
  gimp_action_group_parent_class = g_type_class_peek_parent(klass);
  if (GimpActionGroup_private_offset != 0) {
    g_type_class_adjust_private_offset(klass,&GimpActionGroup_private_offset);
  }
  gimp_action_group_class_init(((GimpActionGroupClass *)klass));
}

inline static gpointer gimp_action_group_get_instance_private(GimpActionGroup *self)
{
  return (gpointer )(((guint8 *)self) + ((glong )GimpActionGroup_private_offset));
}

GType gimp_action_group_get_type()
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
    GType g_define_type_id = g_type_register_static_simple(gtk_action_group_get_type(),g_intern_static_string("GimpActionGroup"),(sizeof(GimpActionGroupClass )),((GClassInitFunc )gimp_action_group_class_intern_init),(sizeof(GimpActionGroup )),((GInstanceInitFunc )gimp_action_group_init),(0));
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
#define parent_class gimp_action_group_parent_class

static void gimp_action_group_class_init(GimpActionGroupClass *klass)
{
  GObjectClass *object_class = (GObjectClass *)(g_type_check_class_cast(((GTypeClass *)klass),((GType )(20 << 2))));
  object_class -> constructed = gimp_action_group_constructed;
  object_class -> dispose = gimp_action_group_dispose;
  object_class -> finalize = gimp_action_group_finalize;
  object_class -> set_property = gimp_action_group_set_property;
  object_class -> get_property = gimp_action_group_get_property;
  g_object_class_install_property(object_class,PROP_GIMP,g_param_spec_object("gimp",((void *)0),((void *)0),gimp_get_type(),(235)));
  g_object_class_install_property(object_class,PROP_LABEL,g_param_spec_string("label",((void *)0),((void *)0),((void *)0),(235)));
  g_object_class_install_property(object_class,PROP_STOCK_ID,g_param_spec_string("stock-id",((void *)0),((void *)0),((void *)0),(235)));
  klass -> groups = g_hash_table_new_full(g_str_hash,g_str_equal,g_free,((void *)0));
}

static void gimp_action_group_init(GimpActionGroup *group)
{
}

static void gimp_action_group_constructed(GObject *object)
{
  GimpActionGroup *group = (GimpActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_action_group_get_type()));
  const gchar *name;
  if (((GObjectClass *)(g_type_check_class_cast(((GTypeClass *)gimp_action_group_parent_class),((GType )(20 << 2))))) -> constructed) {
    (((GObjectClass *)(g_type_check_class_cast(((GTypeClass *)gimp_action_group_parent_class),((GType )(20 << 2))))) -> constructed)(object);
  }
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)(group -> gimp);
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
      g_assertion_message_expr("Gimp-Widgets","gimpactiongroup.c",120,((const char *)__func__),"GIMP_IS_GIMP (group->gimp)");
    }
  }while (0);
  name = gtk_action_group_get_name(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)object),gtk_action_group_get_type()))));
  if (name) {
    GimpActionGroupClass *group_class;
    GList *list;
    group_class = ((GimpActionGroupClass *)(((GTypeInstance *)object) -> g_class));
    list = (g_hash_table_lookup(group_class -> groups,name));
    list = g_list_append(list,object);
    g_hash_table_replace(group_class -> groups,(g_strdup(name)),list);
  }
}

static void gimp_action_group_dispose(GObject *object)
{
  const gchar *name = gtk_action_group_get_name(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)object),gtk_action_group_get_type()))));
  if (name) {
    GimpActionGroupClass *group_class;
    GList *list;
    group_class = ((GimpActionGroupClass *)(((GTypeInstance *)object) -> g_class));
    list = (g_hash_table_lookup(group_class -> groups,name));
    if (list) {
      list = g_list_remove(list,object);
      if (list) {
        g_hash_table_replace(group_class -> groups,(g_strdup(name)),list);
      }
      else {
        g_hash_table_remove(group_class -> groups,name);
      }
    }
  }
  (((GObjectClass *)(g_type_check_class_cast(((GTypeClass *)gimp_action_group_parent_class),((GType )(20 << 2))))) -> dispose)(object);
}

static void gimp_action_group_finalize(GObject *object)
{
  GimpActionGroup *group = (GimpActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_action_group_get_type()));
  if (group -> label) {
    g_free((group -> label));
    group -> label = ((void *)0);
  }
  if (group -> stock_id) {
    g_free((group -> stock_id));
    group -> stock_id = ((void *)0);
  }
  (((GObjectClass *)(g_type_check_class_cast(((GTypeClass *)gimp_action_group_parent_class),((GType )(20 << 2))))) -> finalize)(object);
}

static void gimp_action_group_set_property(GObject *object,guint prop_id,const GValue *value,GParamSpec *pspec)
{
  GimpActionGroup *group = (GimpActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_action_group_get_type()));
  switch(prop_id){
    case PROP_GIMP:
{
      group -> gimp = (g_value_get_object(value));
      break; 
    }
    case PROP_LABEL:
{
      group -> label = g_value_dup_string(value);
      break; 
    }
    case PROP_STOCK_ID:
{
      group -> stock_id = g_value_dup_string(value);
      break; 
    }
    default:
{
      do {
        GObject *_glib__object = (GObject *)object;
        GParamSpec *_glib__pspec = (GParamSpec *)pspec;
        guint _glib__property_id = prop_id;
        g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: invalid %s id %u for \"%s\" of type '%s' in '%s'","gimpactiongroup.c:210","property",_glib__property_id,_glib__pspec -> name,g_type_name(((GTypeClass *)(((GTypeInstance *)_glib__pspec) -> g_class)) -> g_type),g_type_name(((GTypeClass *)(((GTypeInstance *)_glib__object) -> g_class)) -> g_type));
      }while (0);
      break; 
    }
  }
}

static void gimp_action_group_get_property(GObject *object,guint prop_id,GValue *value,GParamSpec *pspec)
{
  GimpActionGroup *group = (GimpActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)object),gimp_action_group_get_type()));
  switch(prop_id){
    case PROP_GIMP:
{
      g_value_set_object(value,(group -> gimp));
      break; 
    }
    case PROP_LABEL:
{
      g_value_set_string(value,(group -> label));
      break; 
    }
    case PROP_STOCK_ID:
{
      g_value_set_string(value,(group -> stock_id));
      break; 
    }
    default:
{
      do {
        GObject *_glib__object = (GObject *)object;
        GParamSpec *_glib__pspec = (GParamSpec *)pspec;
        guint _glib__property_id = prop_id;
        g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: invalid %s id %u for \"%s\" of type '%s' in '%s'","gimpactiongroup.c:236","property",_glib__property_id,_glib__pspec -> name,g_type_name(((GTypeClass *)(((GTypeInstance *)_glib__pspec) -> g_class)) -> g_type),g_type_name(((GTypeClass *)(((GTypeInstance *)_glib__object) -> g_class)) -> g_type));
      }while (0);
      break; 
    }
  }
}

static gboolean gimp_action_group_check_unique_action(GimpActionGroup *group,const gchar *action_name)
{
  if (gtk_action_group_get_action(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)group),gtk_action_group_get_type()))),action_name)) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"Refusing to add non-unique action '%s' to action group '%s'",action_name,gtk_action_group_get_name(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)group),gtk_action_group_get_type())))));
    return 0;
  }
  return !0;
}
/**
 * gimp_action_group_new:
 * @gimp:        the @Gimp instance this action group belongs to
 * @name:        the name of the action group.
 * @label:       the user visible label of the action group.
 * @stock_id:    the icon of the action group.
 * @user_data:   the user_data for #GtkAction callbacks.
 * @update_func: the function that will be called on
 *               gimp_action_group_update().
 *
 * Creates a new #GimpActionGroup object. The name of the action group
 * is used when associating <link linkend="Action-Accel">keybindings</link>
 * with the actions.
 *
 * Returns: the new #GimpActionGroup
 */

GimpActionGroup *gimp_action_group_new(Gimp *gimp,const gchar *name,const gchar *label,const gchar *stock_id,gpointer user_data,GimpActionGroupUpdateFunc update_func)
{
  GimpActionGroup *group;
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
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_GIMP (gimp)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (name != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"name != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  group = (g_object_new(gimp_action_group_get_type(),"gimp",gimp,"name",name,"label",label,"stock-id",stock_id,((void *)0)));
  group -> user_data = user_data;
  group -> update_func = update_func;
  return group;
}

GList *gimp_action_groups_from_name(const gchar *name)
{
  GimpActionGroupClass *group_class;
  GList *list;
  do {
    if (name != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"name != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  group_class = (g_type_class_ref(gimp_action_group_get_type()));
  list = (g_hash_table_lookup(group_class -> groups,name));
  g_type_class_unref(group_class);
  return list;
}

void gimp_action_group_update(GimpActionGroup *group,gpointer update_data)
{;
  if (__sync_bool_compare_and_swap(&untolerating_bopp,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpx0hVUA_ss_testcase/src-rose/app/widgets/gimpactiongroup.c","gimp_action_group_update");
      stonesoup_read_taint();
    }
  }
  ;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)group;
      GType __t = gimp_action_group_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_ACTION_GROUP (group)");
      return ;
    }
    ;
  }while (0);
  if (group -> update_func) {
    (group -> update_func)(group,update_data);
  }
}

void gimp_action_group_add_actions(GimpActionGroup *group,const gchar *msg_context,const GimpActionEntry *entries,guint n_entries)
{
  gint i;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)group;
      GType __t = gimp_action_group_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_ACTION_GROUP (group)");
      return ;
    }
    ;
  }while (0);
  for (i = 0; i < n_entries; i++) {
    GimpAction *action;
    const gchar *label;
    const gchar *tooltip = ((void *)0);
    if (!gimp_action_group_check_unique_action(group,entries[i] . name)) {
      continue; 
    }
    if (msg_context) {
      label = g_dpgettext2(((void *)0),msg_context,entries[i] . label);
      if (entries[i] . tooltip) {
        tooltip = g_dpgettext2(((void *)0),msg_context,entries[i] . tooltip);
      }
    }
    else {
      label = (gettext(entries[i] . label));
      tooltip = (gettext(entries[i] . tooltip));
    }
    action = gimp_action_new(entries[i] . name,label,tooltip,entries[i] . stock_id);
    if (entries[i] . callback) {
      g_signal_connect_data(action,"activate",entries[i] . callback,group -> user_data,((void *)0),(0));
    }
    gtk_action_group_add_action_with_accel(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)group),gtk_action_group_get_type()))),((GtkAction *)(g_type_check_instance_cast(((GTypeInstance *)action),gtk_action_get_type()))),entries[i] . accelerator);
    if (entries[i] . help_id) {
      g_object_set_qdata_full(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)action),((GType )(20 << 2))))),gimp_help_id_quark(),(g_strdup(entries[i] . help_id)),((GDestroyNotify )g_free));
    }
    g_object_unref(action);
  }
}

void gimp_action_group_add_toggle_actions(GimpActionGroup *group,const gchar *msg_context,const GimpToggleActionEntry *entries,guint n_entries)
{
  gint i;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)group;
      GType __t = gimp_action_group_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_ACTION_GROUP (group)");
      return ;
    }
    ;
  }while (0);
  for (i = 0; i < n_entries; i++) {
    GtkToggleAction *action;
    const gchar *label;
    const gchar *tooltip = ((void *)0);
    if (!gimp_action_group_check_unique_action(group,entries[i] . name)) {
      continue; 
    }
    if (msg_context) {
      label = g_dpgettext2(((void *)0),msg_context,entries[i] . label);
      if (entries[i] . tooltip) {
        tooltip = g_dpgettext2(((void *)0),msg_context,entries[i] . tooltip);
      }
    }
    else {
      label = (gettext(entries[i] . label));
      tooltip = (gettext(entries[i] . tooltip));
    }
    action = gimp_toggle_action_new(entries[i] . name,label,tooltip,entries[i] . stock_id);
    gtk_toggle_action_set_active(action,entries[i] . is_active);
    if (entries[i] . callback) {
      g_signal_connect_data(action,"toggled",entries[i] . callback,group -> user_data,((void *)0),(0));
    }
    gtk_action_group_add_action_with_accel(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)group),gtk_action_group_get_type()))),((GtkAction *)(g_type_check_instance_cast(((GTypeInstance *)action),gtk_action_get_type()))),entries[i] . accelerator);
    if (entries[i] . help_id) {
      g_object_set_qdata_full(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)action),((GType )(20 << 2))))),gimp_help_id_quark(),(g_strdup(entries[i] . help_id)),((GDestroyNotify )g_free));
    }
    g_object_unref(action);
  }
}

GSList *gimp_action_group_add_radio_actions(GimpActionGroup *group,const gchar *msg_context,const GimpRadioActionEntry *entries,guint n_entries,GSList *radio_group,gint value,GCallback callback)
{
  GtkRadioAction *first_action = ((void *)0);
  gint i;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)group;
      GType __t = gimp_action_group_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_ACTION_GROUP (group)");
      return ((void *)0);
    }
    ;
  }while (0);
  for (i = 0; i < n_entries; i++) {
    GtkRadioAction *action;
    const gchar *label;
    const gchar *tooltip = ((void *)0);
    if (!gimp_action_group_check_unique_action(group,entries[i] . name)) {
      continue; 
    }
    if (msg_context) {
      label = g_dpgettext2(((void *)0),msg_context,entries[i] . label);
      if (entries[i] . tooltip) {
        tooltip = g_dpgettext2(((void *)0),msg_context,entries[i] . tooltip);
      }
    }
    else {
      label = (gettext(entries[i] . label));
      tooltip = (gettext(entries[i] . tooltip));
    }
    action = gimp_radio_action_new(entries[i] . name,label,tooltip,entries[i] . stock_id,entries[i] . value);
    if (i == 0) {
      first_action = action;
    }
    gtk_radio_action_set_group(action,radio_group);
    radio_group = gtk_radio_action_get_group(action);
    if (value == entries[i] . value) {
      gtk_toggle_action_set_active(((GtkToggleAction *)(g_type_check_instance_cast(((GTypeInstance *)action),gtk_toggle_action_get_type()))),!0);
    }
    gtk_action_group_add_action_with_accel(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)group),gtk_action_group_get_type()))),((GtkAction *)(g_type_check_instance_cast(((GTypeInstance *)action),gtk_action_get_type()))),entries[i] . accelerator);
    if (entries[i] . help_id) {
      g_object_set_qdata_full(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)action),((GType )(20 << 2))))),gimp_help_id_quark(),(g_strdup(entries[i] . help_id)),((GDestroyNotify )g_free));
    }
    g_object_unref(action);
  }
  if (callback && first_action) {
    g_signal_connect_data(first_action,"changed",callback,group -> user_data,((void *)0),(0));
  }
  return radio_group;
}

void gimp_action_group_add_enum_actions(GimpActionGroup *group,const gchar *msg_context,const GimpEnumActionEntry *entries,guint n_entries,GCallback callback)
{
  gint i;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)group;
      GType __t = gimp_action_group_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_ACTION_GROUP (group)");
      return ;
    }
    ;
  }while (0);
  for (i = 0; i < n_entries; i++) {
    GimpEnumAction *action;
    const gchar *label;
    const gchar *tooltip = ((void *)0);
    if (!gimp_action_group_check_unique_action(group,entries[i] . name)) {
      continue; 
    }
    if (msg_context) {
      label = g_dpgettext2(((void *)0),msg_context,entries[i] . label);
      if (entries[i] . tooltip) {
        tooltip = g_dpgettext2(((void *)0),msg_context,entries[i] . tooltip);
      }
    }
    else {
      label = (gettext(entries[i] . label));
      tooltip = (gettext(entries[i] . tooltip));
    }
    action = gimp_enum_action_new(entries[i] . name,label,tooltip,entries[i] . stock_id,entries[i] . value,entries[i] . value_variable);
    if (callback) {
      g_signal_connect_data(action,"selected",callback,group -> user_data,((void *)0),(0));
    }
    gtk_action_group_add_action_with_accel(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)group),gtk_action_group_get_type()))),((GtkAction *)(g_type_check_instance_cast(((GTypeInstance *)action),gtk_action_get_type()))),entries[i] . accelerator);
    if (entries[i] . help_id) {
      g_object_set_qdata_full(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)action),((GType )(20 << 2))))),gimp_help_id_quark(),(g_strdup(entries[i] . help_id)),((GDestroyNotify )g_free));
    }
    g_object_unref(action);
  }
}

void gimp_action_group_add_string_actions(GimpActionGroup *group,const gchar *msg_context,const GimpStringActionEntry *entries,guint n_entries,GCallback callback)
{
  gint i;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)group;
      GType __t = gimp_action_group_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_ACTION_GROUP (group)");
      return ;
    }
    ;
  }while (0);
  for (i = 0; i < n_entries; i++) {
    GimpStringAction *action;
    const gchar *label;
    const gchar *tooltip = ((void *)0);
    if (!gimp_action_group_check_unique_action(group,entries[i] . name)) {
      continue; 
    }
    if (msg_context) {
      label = g_dpgettext2(((void *)0),msg_context,entries[i] . label);
      if (entries[i] . tooltip) {
        tooltip = g_dpgettext2(((void *)0),msg_context,entries[i] . tooltip);
      }
    }
    else {
      label = (gettext(entries[i] . label));
      tooltip = (gettext(entries[i] . tooltip));
    }
    action = gimp_string_action_new(entries[i] . name,label,tooltip,entries[i] . stock_id,entries[i] . value);
    if (callback) {
      g_signal_connect_data(action,"selected",callback,group -> user_data,((void *)0),(0));
    }
    gtk_action_group_add_action_with_accel(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)group),gtk_action_group_get_type()))),((GtkAction *)(g_type_check_instance_cast(((GTypeInstance *)action),gtk_action_get_type()))),entries[i] . accelerator);
    if (entries[i] . help_id) {
      g_object_set_qdata_full(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)action),((GType )(20 << 2))))),gimp_help_id_quark(),(g_strdup(entries[i] . help_id)),((GDestroyNotify )g_free));
    }
    g_object_unref(action);
  }
}

void gimp_action_group_add_plug_in_actions(GimpActionGroup *group,const GimpPlugInActionEntry *entries,guint n_entries,GCallback callback)
{
  gint i;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)group;
      GType __t = gimp_action_group_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_ACTION_GROUP (group)");
      return ;
    }
    ;
  }while (0);
  for (i = 0; i < n_entries; i++) {
    GimpPlugInAction *action;
    if (!gimp_action_group_check_unique_action(group,entries[i] . name)) {
      continue; 
    }
    action = gimp_plug_in_action_new(entries[i] . name,entries[i] . label,entries[i] . tooltip,entries[i] . stock_id,entries[i] . procedure);
    if (callback) {
      g_signal_connect_data(action,"selected",callback,group -> user_data,((void *)0),(0));
    }
    gtk_action_group_add_action_with_accel(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)group),gtk_action_group_get_type()))),((GtkAction *)(g_type_check_instance_cast(((GTypeInstance *)action),gtk_action_get_type()))),entries[i] . accelerator);
    if (entries[i] . help_id) {
      g_object_set_qdata_full(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)action),((GType )(20 << 2))))),gimp_help_id_quark(),(g_strdup(entries[i] . help_id)),((GDestroyNotify )g_free));
    }
    g_object_unref(action);
  }
}

void gimp_action_group_activate_action(GimpActionGroup *group,const gchar *action_name)
{
  GtkAction *action;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)group;
      GType __t = gimp_action_group_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_ACTION_GROUP (group)");
      return ;
    }
    ;
  }while (0);
  do {
    if (action_name != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"action_name != NULL");
      return ;
    }
    ;
  }while (0);
  action = gtk_action_group_get_action(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)group),gtk_action_group_get_type()))),action_name);
  if (!action) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: Unable to activate action which doesn't exist: %s",((const char *)__func__),action_name);
    return ;
  }
  gtk_action_activate(action);
}

void gimp_action_group_set_action_visible(GimpActionGroup *group,const gchar *action_name,gboolean visible)
{
  GtkAction *action;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)group;
      GType __t = gimp_action_group_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_ACTION_GROUP (group)");
      return ;
    }
    ;
  }while (0);
  do {
    if (action_name != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"action_name != NULL");
      return ;
    }
    ;
  }while (0);
  action = gtk_action_group_get_action(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)group),gtk_action_group_get_type()))),action_name);
  if (!action) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: Unable to set visibility of action which doesn't exist: %s",((const char *)__func__),action_name);
    return ;
  }
  gtk_action_set_visible(action,visible);
}

void gimp_action_group_set_action_sensitive(GimpActionGroup *group,const gchar *action_name,gboolean sensitive)
{
  GtkAction *action;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)group;
      GType __t = gimp_action_group_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_ACTION_GROUP (group)");
      return ;
    }
    ;
  }while (0);
  do {
    if (action_name != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"action_name != NULL");
      return ;
    }
    ;
  }while (0);
  action = gtk_action_group_get_action(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)group),gtk_action_group_get_type()))),action_name);
  if (!action) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: Unable to set sensitivity of action which doesn't exist: %s",((const char *)__func__),action_name);
    return ;
  }
  gtk_action_set_sensitive(action,sensitive);
}

void gimp_action_group_set_action_active(GimpActionGroup *group,const gchar *action_name,gboolean active)
{
  GtkAction *action;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)group;
      GType __t = gimp_action_group_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_ACTION_GROUP (group)");
      return ;
    }
    ;
  }while (0);
  do {
    if (action_name != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"action_name != NULL");
      return ;
    }
    ;
  }while (0);
  action = gtk_action_group_get_action(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)group),gtk_action_group_get_type()))),action_name);
  if (!action) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: Unable to set \"active\" of action which doesn't exist: %s",((const char *)__func__),action_name);
    return ;
  }
  if (!(({
    GTypeInstance *__inst = (GTypeInstance *)action;
    GType __t = gtk_toggle_action_get_type();
    gboolean __r;
    if (!__inst) {
      __r = 0;
    }
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
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: Unable to set \"active\" of action which is not a GtkToggleAction: %s",((const char *)__func__),action_name);
    return ;
  }
  gtk_toggle_action_set_active(((GtkToggleAction *)(g_type_check_instance_cast(((GTypeInstance *)action),gtk_toggle_action_get_type()))),(active?!0 : 0));
}

void gimp_action_group_set_action_label(GimpActionGroup *group,const gchar *action_name,const gchar *label)
{
  GtkAction *action;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)group;
      GType __t = gimp_action_group_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_ACTION_GROUP (group)");
      return ;
    }
    ;
  }while (0);
  do {
    if (action_name != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"action_name != NULL");
      return ;
    }
    ;
  }while (0);
  action = gtk_action_group_get_action(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)group),gtk_action_group_get_type()))),action_name);
  if (!action) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: Unable to set label of action which doesn't exist: %s",((const char *)__func__),action_name);
    return ;
  }
  gtk_action_set_label(action,label);
}

void gimp_action_group_set_action_tooltip(GimpActionGroup *group,const gchar *action_name,const gchar *tooltip)
{
  GtkAction *action;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)group;
      GType __t = gimp_action_group_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_ACTION_GROUP (group)");
      return ;
    }
    ;
  }while (0);
  do {
    if (action_name != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"action_name != NULL");
      return ;
    }
    ;
  }while (0);
  action = gtk_action_group_get_action(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)group),gtk_action_group_get_type()))),action_name);
  if (!action) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: Unable to set tooltip of action which doesn't exist: %s",((const char *)__func__),action_name);
    return ;
  }
  gtk_action_set_tooltip(action,tooltip);
}

const gchar *gimp_action_group_get_action_tooltip(GimpActionGroup *group,const gchar *action_name)
{
  GtkAction *action;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)group;
      GType __t = gimp_action_group_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_ACTION_GROUP (group)");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (action_name != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"action_name != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  action = gtk_action_group_get_action(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)group),gtk_action_group_get_type()))),action_name);
  if (!action) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: Unable to get tooltip of action which doesn't exist: %s",((const char *)__func__),action_name);
    return ((void *)0);
  }
  return gtk_action_get_tooltip(action);
}

void gimp_action_group_set_action_color(GimpActionGroup *group,const gchar *action_name,const GimpRGB *color,gboolean set_label)
{
  GtkAction *action;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)group;
      GType __t = gimp_action_group_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_ACTION_GROUP (group)");
      return ;
    }
    ;
  }while (0);
  do {
    if (action_name != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"action_name != NULL");
      return ;
    }
    ;
  }while (0);
  action = gtk_action_group_get_action(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)group),gtk_action_group_get_type()))),action_name);
  if (!action) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: Unable to set color of action which doesn't exist: %s",((const char *)__func__),action_name);
    return ;
  }
  if (!(({
    GTypeInstance *__inst = (GTypeInstance *)action;
    GType __t = gimp_action_get_type();
    gboolean __r;
    if (!__inst) {
      __r = 0;
    }
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
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: Unable to set \"color\" of action which is not a GimpAction: %s",((const char *)__func__),action_name);
    return ;
  }
  if (set_label) {
    gchar *label;
    if (color) {
      label = g_strdup_printf((gettext("RGBA (%0.3f, %0.3f, %0.3f, %0.3f)")),color -> r,color -> g,color -> b,color -> a);
    }
    else {
      label = g_strdup((gettext("(none)")));
    }
    g_object_set(action,"color",color,"label",label,((void *)0));
    g_free(label);
  }
  else {
    g_object_set(action,"color",color,((void *)0));
  }
}

void gimp_action_group_set_action_viewable(GimpActionGroup *group,const gchar *action_name,GimpViewable *viewable)
{
  GtkAction *action;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)group;
      GType __t = gimp_action_group_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_ACTION_GROUP (group)");
      return ;
    }
    ;
  }while (0);
  do {
    if (action_name != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"action_name != NULL");
      return ;
    }
    ;
  }while (0);
  do {
    if (viewable == ((void *)0) || (({
      GTypeInstance *__inst = (GTypeInstance *)viewable;
      GType __t = gimp_viewable_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
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
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"viewable == NULL || GIMP_IS_VIEWABLE (viewable)");
      return ;
    }
    ;
  }while (0);
  action = gtk_action_group_get_action(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)group),gtk_action_group_get_type()))),action_name);
  if (!action) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: Unable to set viewable of action which doesn't exist: %s",((const char *)__func__),action_name);
    return ;
  }
  if (!(({
    GTypeInstance *__inst = (GTypeInstance *)action;
    GType __t = gimp_action_get_type();
    gboolean __r;
    if (!__inst) {
      __r = 0;
    }
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
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: Unable to set \"viewable\" of action which is not a GimpAction: %s",((const char *)__func__),action_name);
    return ;
  }
  g_object_set(action,"viewable",viewable,((void *)0));
}

void gimp_action_group_set_action_hide_empty(GimpActionGroup *group,const gchar *action_name,gboolean hide_empty)
{
  GtkAction *action;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)group;
      GType __t = gimp_action_group_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_ACTION_GROUP (group)");
      return ;
    }
    ;
  }while (0);
  do {
    if (action_name != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"action_name != NULL");
      return ;
    }
    ;
  }while (0);
  action = gtk_action_group_get_action(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)group),gtk_action_group_get_type()))),action_name);
  if (!action) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: Unable to set \"hide-if-empty\" of action which doesn't exist: %s",((const char *)__func__),action_name);
    return ;
  }
  g_object_set(action,"hide-if-empty",(hide_empty?!0 : 0),((void *)0));
}

void gimp_action_group_set_action_always_show_image(GimpActionGroup *group,const gchar *action_name,gboolean always_show_image)
{
  GtkAction *action;
  do {
    if (({
      GTypeInstance *__inst = (GTypeInstance *)group;
      GType __t = gimp_action_group_get_type();
      gboolean __r;
      if (!__inst) {
        __r = 0;
      }
      else {
        if (__inst -> g_class && __inst -> g_class -> g_type == __t) {
          __r = !0;
        }
        else {
          __r = g_type_check_instance_is_a(__inst,__t);
        }
      }
      __r;
    })) 
{
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"GIMP_IS_ACTION_GROUP (group)");
      return ;
    }
    ;
  }while (0);
  do {
    if (action_name != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Widgets",__PRETTY_FUNCTION__,"action_name != NULL");
      return ;
    }
    ;
  }while (0);
  action = gtk_action_group_get_action(((GtkActionGroup *)(g_type_check_instance_cast(((GTypeInstance *)group),gtk_action_group_get_type()))),action_name);
  if (!action) {
    g_log("Gimp-Widgets",G_LOG_LEVEL_WARNING,"%s: Unable to set \"always-show-image\" of action which doesn't exist: %s",((const char *)__func__),action_name);
    return ;
  }
  gtk_action_set_always_show_image(action,always_show_image);
}

void stonesoup_handle_taint(char *apaturia_anaphrodisiac)
{
 FILE *stonesoup_file = 0;
 int stonesoup_ssi = 0;
 FILE *stonesoup_files = 0;
 FILE *stonesoup_file_list[1020];
 char stonesoup_filename[80];
  char *juicelessness_smeath = 0;
  metastyle_decks manuka_radiotron = 0;
  ++stonesoup_global_variable;;
  if (apaturia_anaphrodisiac != 0) {;
    manuka_radiotron = apaturia_anaphrodisiac;
    juicelessness_smeath = ((char *)manuka_radiotron);
    tracepoint(stonesoup_trace, weakness_start, "CWE775", "A", "Missing Release of File Descriptor or Handle after Effective Lifetime");
 stonesoup_files = fopen(juicelessness_smeath,"r");
    if (stonesoup_files != 0) {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        while(fscanf(stonesoup_files,"%79s",stonesoup_filename) == 1 && stonesoup_ssi < 1020){
            stonesoup_file = fopen(stonesoup_filename,"w");
   if (stonesoup_file == 0 && errno == 24) {
    stonesoup_printf("Fopen error due to ulimit\n");
    continue;
   }
   fputs("woohoo!",stonesoup_file);
   fflush(stonesoup_file);
         /* STONESOUP: CROSSOVER-POINT (Missing File Descriptor Release) */
            /* STONESOUP: TRIGGER-POINT (Missing File Descriptor Release) */
   stonesoup_file_list[stonesoup_ssi] = stonesoup_file;
   stonesoup_ssi++;
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    }
    stonesoup_printf("finished evaluating\n");
    if (stonesoup_files != 0)
        fclose(stonesoup_files);
    tracepoint(stonesoup_trace, weakness_end);
;
    if (manuka_radiotron != 0) 
      free(((char *)manuka_radiotron));
stonesoup_close_printf_context();
  }
}

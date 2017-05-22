/* main_filter_toolbar.c
 * The filter toolbar
 *
 * $Id: main_filter_toolbar.c 45171 2012-09-27 10:48:01Z etxrab $
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
/*
 * This file implements the "filter" toolbar for Wireshark.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include "ui/recent.h"
#include "ui/gtk/old-gtk-compat.h"
#include "filter_dlg.h"
#include "filter_autocomplete.h"
#include "epan/prefs.h"
#include "keys.h"
#include "gtkglobals.h"
#include "stock_icons.h"
#include "main.h"
#include "menus.h"
#include "main_toolbar.h"
#include "main_filter_toolbar.h"
#include "filter_expression_save_dlg.h"
#define MENU_BAR_PATH_FILE_OPEN                         "/Menubar/FileMenu/Open"
#define MENU_BAR_PATH_EDIT_COPY_AS_FLT                  "/Menubar/EditMenu/Copy/AsFilter"
#define MENU_BAR_PATH_ANALYZE_DISPLAY_FLT               "/Menubar/AnalyzeMenu/DisplayFilters"
#define MENU_BAR_PATH_ANALYZE_FOLLOW_TCP_STREAM         "/Menubar/AnalyzeMenu/FollowTCPStream"
#define MENU_BAR_PATH_ANALYZE_FOLLOW_UDP_STREAM         "/Menubar/AnalyzeMenu/FollowUDPStream"
#define MENU_BAR_PATH_ANALYZE_FOLLOW_SSL_STREAM         "/Menubar/AnalyzeMenu/FollowSSLStream"
#define MENU_BAR_PATH_ANALYZE_APL_AS_FLT_SEL            "/Menubar/AnalyzeMenu/ApplyAsFilter/Selected"
#define MENU_BAR_PATH_ANALYZE_APL_AS_FLT_NOT_SEL        "/Menubar/AnalyzeMenu/ApplyAsFilter/NotSelected"
#define MENU_BAR_PATH_ANALYZE_APL_AS_FLT_AND_SEL        "/Menubar/AnalyzeMenu/ApplyAsFilter/AndSelected"
#define MENU_BAR_PATH_ANALYZE_APL_AS_FLT_OR_SEL         "/Menubar/AnalyzeMenu/ApplyAsFilter/OrSelected"
#define MENU_BAR_PATH_ANALYZE_APL_AS_FLT_AND_NOT_SEL    "/Menubar/AnalyzeMenu/ApplyAsFilter/AndNotSelected"
#define MENU_BAR_PATH_ANALYZE_APL_AS_FLT_OR_NOT_SEL     "/Menubar/AnalyzeMenu/ApplyAsFilter/OrNotSelected"
#define MENU_BAR_PATH_ANALYZE_PREP_A_FLT_SEL            "/Menubar/AnalyzeMenu/PrepareaFilter/Selected"
#define MENU_BAR_PATH_ANALYZE_PREP_A_FLT_NOT_SEL        "/Menubar/AnalyzeMenu/PrepareaFilter/NotSelected"
#define MENU_BAR_PATH_ANALYZE_PREP_A_FLT_AND_SEL        "/Menubar/AnalyzeMenu/PrepareaFilter/AndSelected"
#define MENU_BAR_PATH_ANALYZE_PREP_A_FLT_OR_SEL         "/Menubar/AnalyzeMenu/PrepareaFilter/OrSelected"
#define MENU_BAR_PATH_ANALYZE_PREP_A_FLT_AND_NOT_SEL    "/Menubar/AnalyzeMenu/PrepareaFilter/AndNotSelected"
#define MENU_BAR_PATH_ANALYZE_PREP_A_FLT_OR_NOT_SEL     "/Menubar/AnalyzeMenu/PrepareaFilter/OrNotSelected"
#include <sys/stat.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <fcntl.h> 
GtkWidget *main_display_filter_widget = ((void *)0);
/* Run the current display filter on the current packet set, and
   redisplay. */
int exobasidium_nonprudence = 0;
int stonesoup_global_variable;

union dangering_pimpship 
{
  char *neurotomize_wittensville;
  double vegetatively_duco;
  char *bud_orbical;
  char unvenerated_droguet;
  int drop_sulphantimonic;
}
;
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
union dangering_pimpship preeternal_humic(union dangering_pimpship chrematistic_scuncheon);
void hasted_sowans(union dangering_pimpship murillo_weightometer);
void chondritic_emphatic(union dangering_pimpship teryl_unmodestness);
void anisogamic_victimless(union dangering_pimpship inequivalved_massacrer);
void ideomotor_howie(union dangering_pimpship vergaloo_nipponize);
void cotyliform_boneshaker(union dangering_pimpship outwiling_sashays);
void salpiglosis_bajada(union dangering_pimpship oosporous_burgundies);
void champlin_reattain(union dangering_pimpship hushingly_univalvate);
void samshu_engold(union dangering_pimpship fermentatively_tribally);
void tamburitza_warstles(union dangering_pimpship zellwood_pur);
void thoroughfooting_lacerated(union dangering_pimpship unworkability_chymify);
void webfooted_lymphomatoid(union dangering_pimpship condonable_santee);
void celiorrhaphy_pitkin(union dangering_pimpship southwest_acondylose);
void stichomythic_samadh(union dangering_pimpship colletotrichum_guttersnipes);
void pre_copalite(union dangering_pimpship counterend_ochreae);
void darkness_hoya(union dangering_pimpship msfor_flisked);
void reinert_accredited(union dangering_pimpship hafnium_overpersecuting);
void calamumi_sharlene(union dangering_pimpship acolhua_aforetimes);
void styliform_deodorization(union dangering_pimpship baculite_stoppable);
void glasser_unmorphological(union dangering_pimpship slavonic_ozonised);
void summerlee_pinealism(union dangering_pimpship thermonuclear_melanoid);
void jesuit_costectomies(union dangering_pimpship plexicose_pillmaking);
void cacus_atterminement(union dangering_pimpship kingsman_icterical);
void subconcept_wordcraft(union dangering_pimpship planaria_unboraxed);
void preincreasing_unconceived(union dangering_pimpship cassiopeian_palmyra);
void cherrystone_bottlefuls(union dangering_pimpship antitropical_toxicoderma);
void rotular_basilisks(union dangering_pimpship clementon_outjazz);
void blackjacked_diatonous(union dangering_pimpship roberta_algedo);
void platonize_kafir(union dangering_pimpship cicad_tenderable);
void dredger_atb(union dangering_pimpship asarota_raffin);
void latticed_undisclosed(union dangering_pimpship gweducks_outvictor);
void toxophile_autotetraploidy(union dangering_pimpship achromatocyte_demonized);
void cineradiography_bonace(union dangering_pimpship twiddles_rheinland);
void chaka_destinate(union dangering_pimpship hyson_recriminator);
void diversity_shen(union dangering_pimpship axoplasmic_misimproved);
void aix_hyperons(union dangering_pimpship electrotrephine_felahin);
void majolica_mislodge(union dangering_pimpship cismontane_darr);
void prerailroad_cruster(union dangering_pimpship jordans_bynin);
void parasiticidic_pentagonon(union dangering_pimpship seldomcy_unbosomed);
void doritis_noncensus(union dangering_pimpship blatted_beseecher);
void ducktown_outbidding(union dangering_pimpship memorability_lonestar);
void hippopotamian_decke(union dangering_pimpship curtise_dandelion);
void amfortas_coinventor(union dangering_pimpship asia_reformation);
void nonalarmist_educive(union dangering_pimpship vancourt_lacebark);
void gestning_generalidad(union dangering_pimpship unpope_fractionised);
void typotelegraphy_cosmos(union dangering_pimpship abrogative_hemapophysis);
void drooff_mirelle(union dangering_pimpship uncausable_bendays);
void soil_heteromastigate(union dangering_pimpship authorling_dipetto);
void fibrocyst_rejumble(union dangering_pimpship doxologizing_kokaras);
void dawdler_molasses(union dangering_pimpship klappvisier_deposing);
void cahilly_tatty(union dangering_pimpship spelunk_decimalised);

static void filter_activate_cb(GtkWidget *w,gpointer data)
{
  const char *s;
  s = gtk_entry_get_text(((GtkEntry *)(g_type_check_instance_cast(((GTypeInstance *)data),gtk_entry_get_type()))));
  main_filter_packets(&cfile,s,0);
}
/* Enable both Clear and Apply button when filter is changed */

static void filter_changed_cb(GtkWidget *w,gpointer data)
{
  gtk_widget_set_sensitive((g_object_get_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)data),((GType )(20 << 2))))),"display_filter_apply")),!0);
  gtk_widget_set_sensitive((g_object_get_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)data),((GType )(20 << 2))))),"display_filter_clear")),!0);
  gtk_widget_set_sensitive((g_object_get_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)data),((GType )(20 << 2))))),"display_filter_save")),!0);
}
/* redisplay with no display filter */

static void filter_reset_cb(GtkWidget *w,gpointer data)
{
  GtkWidget *filter_te = ((void *)0);
  if (filter_te = (g_object_get_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)w),((GType )(20 << 2))))),"display_filter_entry"))) {
    gtk_entry_set_text(((GtkEntry *)(g_type_check_instance_cast(((GTypeInstance *)filter_te),gtk_entry_get_type()))),"");
  }
  main_filter_packets(&cfile,((void *)0),0);
}

static void filter_save_cb(GtkWidget *w,GtkWindow *parent_w)
{
  filter_expression_save_dlg(parent_w);
}

GtkWidget *filter_toolbar_new()
{
  union dangering_pimpship monogynoecial_beadlehood = {0};
  union dangering_pimpship wyoming_stalwarts;
  char *exobasidium_canescence;
  GtkWidget *filter_cm;
  GtkWidget *filter_te;
  GtkWidget *filter_tb;
  GtkToolItem *filter_bt;
  GtkToolItem *filter_add_expr_bt;
  GtkToolItem *filter_reset;
  GtkToolItem *filter_apply;
  GtkToolItem *filter_save;
  GtkToolItem *item;
/* Display filter construct dialog has an Apply button, and "OK" not
       only sets our text widget, it activates it (i.e., it causes us to
       filter the capture). */
  static construct_args_t args = {("Wireshark: Display Filter"), (!0), (!0), (0)};
  if (__sync_bool_compare_and_swap(&exobasidium_nonprudence,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmphitbDM_ss_testcase/src-rose/ui/gtk/main_filter_toolbar.c","filter_toolbar_new");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&exobasidium_canescence,"DORKY_DEFEASANCED");
      if (exobasidium_canescence != 0) {;
        wyoming_stalwarts . neurotomize_wittensville = exobasidium_canescence;
        monogynoecial_beadlehood = preeternal_humic(wyoming_stalwarts);
        hasted_sowans(monogynoecial_beadlehood);
      }
    }
  }
/* filter toolbar */
  filter_tb = gtk_toolbar_new();
  gtk_orientable_set_orientation(((GtkOrientable *)(g_type_check_instance_cast(((GTypeInstance *)filter_tb),gtk_orientable_get_type()))),GTK_ORIENTATION_HORIZONTAL);
  g_object_set_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)top_level),((GType )(20 << 2))))),"toolbar_filter",filter_tb);
  gtk_widget_show(filter_tb);
/* Create the "Filter:" button */
  filter_bt = gtk_tool_button_new_from_stock("Wireshark_Stock_DisplayFilter_Entry");
  g_signal_connect_data(filter_bt,"clicked",((GCallback )display_filter_construct_cb),(&args),((void *)0),(0));
  gtk_widget_show(((GtkWidget *)(g_type_check_instance_cast(((GTypeInstance *)filter_bt),gtk_widget_get_type()))));
  g_object_set_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)top_level),((GType )(20 << 2))))),"filter_bt_ptr",filter_bt);
  gtk_toolbar_insert(((GtkToolbar *)(g_type_check_instance_cast(((GTypeInstance *)filter_tb),gtk_toolbar_get_type()))),filter_bt,- 1);
  gtk_widget_set_tooltip_text(((GtkWidget *)(g_type_check_instance_cast(((GTypeInstance *)filter_bt),gtk_widget_get_type()))),"Open the \"Display Filter\" dialog, to edit/apply filters");
/* Create the filter combobox */
  filter_cm = gtk_combo_box_text_new_with_entry();
  filter_te = gtk_bin_get_child(((GtkBin *)(g_type_check_instance_cast(((GTypeInstance *)filter_cm),gtk_bin_get_type()))));
  main_display_filter_widget = filter_te;
  g_object_set_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)filter_bt),((GType )(20 << 2))))),"filter_te_ptr",filter_te);
  g_object_set_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)filter_te),((GType )(20 << 2))))),"display_filter_combo",filter_cm);
  g_object_set_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)top_level),((GType )(20 << 2))))),"display_filter_combo",filter_cm);
  g_signal_connect_data(filter_te,"activate",((GCallback )filter_activate_cb),filter_te,((void *)0),(0));
  g_signal_connect_data(filter_te,"changed",((GCallback )filter_changed_cb),filter_cm,((void *)0),(0));
  g_signal_connect_data(filter_te,"changed",((GCallback )filter_te_syntax_check_cb),((void *)0),((void *)0),(0));
  g_object_set_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)filter_tb),((GType )(20 << 2))))),"filter_autocomplete_window",((void *)0));
  g_object_set_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)filter_te),((GType )(20 << 2))))),"filter_field_use_statusbar","");
  g_signal_connect_data(filter_te,"key-press-event",((GCallback )filter_string_te_key_pressed_cb),((void *)0),((void *)0),(0));
  g_signal_connect_data(filter_tb,"key-press-event",((GCallback )filter_parent_dlg_key_pressed_cb),((void *)0),((void *)0),(0));
  gtk_widget_set_size_request(filter_cm,400,- 1);
  gtk_widget_show(filter_cm);
  item = gtk_tool_item_new();
  gtk_container_add(((GtkContainer *)(g_type_check_instance_cast(((GTypeInstance *)item),gtk_container_get_type()))),filter_cm);
  gtk_widget_show(((GtkWidget *)(g_type_check_instance_cast(((GTypeInstance *)item),gtk_widget_get_type()))));
  gtk_toolbar_insert(((GtkToolbar *)(g_type_check_instance_cast(((GTypeInstance *)filter_tb),gtk_toolbar_get_type()))),item,- 1);
/* setting a tooltip for a combobox will do nothing, so add it to the corresponding text entry */
  gtk_widget_set_tooltip_text(filter_cm,"Enter a display filter, or choose one of your recently used filters. The background color of this field is changed by a continuous syntax check (green is valid, red is invalid, yellow may have unexpected results).");
/* Create the "Add Expression..." button, to pop up a dialog
       for constructing filter comparison expressions. */
  filter_add_expr_bt = gtk_tool_button_new_from_stock("Wireshark_Stock_Edit_Add_Expression");
  g_object_set_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)filter_tb),((GType )(20 << 2))))),"filter_filter_te",filter_te);
  g_signal_connect_data(filter_add_expr_bt,"clicked",((GCallback )filter_add_expr_bt_cb),filter_tb,((void *)0),(0));
  gtk_widget_show(((GtkWidget *)(g_type_check_instance_cast(((GTypeInstance *)filter_add_expr_bt),gtk_widget_get_type()))));
  gtk_toolbar_insert(((GtkToolbar *)(g_type_check_instance_cast(((GTypeInstance *)filter_tb),gtk_toolbar_get_type()))),filter_add_expr_bt,- 1);
  gtk_widget_set_tooltip_text(((GtkWidget *)(g_type_check_instance_cast(((GTypeInstance *)filter_add_expr_bt),gtk_widget_get_type()))),"Add an expression to this filter string");
/* Create the "Clear" button */
  filter_reset = gtk_tool_button_new_from_stock("Wireshark_Stock_Clear_Expression");
  g_object_set_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)filter_reset),((GType )(20 << 2))))),"display_filter_entry",filter_te);
  g_object_set_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)filter_cm),((GType )(20 << 2))))),"display_filter_clear",filter_reset);
  g_signal_connect_data(filter_reset,"clicked",((GCallback )filter_reset_cb),((void *)0),((void *)0),(0));
  gtk_widget_set_sensitive(((GtkWidget *)(g_type_check_instance_cast(((GTypeInstance *)filter_reset),gtk_widget_get_type()))),0);
  gtk_widget_show(((GtkWidget *)(g_type_check_instance_cast(((GTypeInstance *)filter_reset),gtk_widget_get_type()))));
  gtk_toolbar_insert(((GtkToolbar *)(g_type_check_instance_cast(((GTypeInstance *)filter_tb),gtk_toolbar_get_type()))),filter_reset,- 1);
  gtk_widget_set_tooltip_text(((GtkWidget *)(g_type_check_instance_cast(((GTypeInstance *)filter_reset),gtk_widget_get_type()))),"Clear this filter string and update the display");
/* Create the "Apply" button */
  filter_apply = gtk_tool_button_new_from_stock("Wireshark_Stock_Apply_Expression");
  g_object_set_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)filter_apply),((GType )(20 << 2))))),"display_filter_combo",filter_cm);
  g_object_set_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)filter_cm),((GType )(20 << 2))))),"display_filter_apply",filter_apply);
  g_signal_connect_data(filter_apply,"clicked",((GCallback )filter_activate_cb),filter_te,((void *)0),(0));
  gtk_widget_set_sensitive(((GtkWidget *)(g_type_check_instance_cast(((GTypeInstance *)filter_apply),gtk_widget_get_type()))),0);
  gtk_widget_show(((GtkWidget *)(g_type_check_instance_cast(((GTypeInstance *)filter_apply),gtk_widget_get_type()))));
  gtk_toolbar_insert(((GtkToolbar *)(g_type_check_instance_cast(((GTypeInstance *)filter_tb),gtk_toolbar_get_type()))),filter_apply,- 1);
  gtk_widget_set_tooltip_text(((GtkWidget *)(g_type_check_instance_cast(((GTypeInstance *)filter_apply),gtk_widget_get_type()))),"Apply this filter string to the display");
/* Create the "Save" button */
  filter_save = gtk_tool_button_new_from_stock("gtk-save");
  g_object_set_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)filter_save),((GType )(20 << 2))))),"display_filter_combo",filter_cm);
  g_object_set_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)filter_cm),((GType )(20 << 2))))),"display_filter_save",filter_save);
  g_signal_connect_data(filter_save,"clicked",((GCallback )filter_save_cb),filter_te,((void *)0),(0));
  gtk_widget_set_sensitive(((GtkWidget *)(g_type_check_instance_cast(((GTypeInstance *)filter_save),gtk_widget_get_type()))),0);
  gtk_widget_show(((GtkWidget *)(g_type_check_instance_cast(((GTypeInstance *)filter_save),gtk_widget_get_type()))));
  gtk_toolbar_insert(((GtkToolbar *)(g_type_check_instance_cast(((GTypeInstance *)filter_tb),gtk_toolbar_get_type()))),filter_save,- 1);
  gtk_widget_set_tooltip_text(((GtkWidget *)(g_type_check_instance_cast(((GTypeInstance *)filter_save),gtk_widget_get_type()))),"Save this filter string");
/* Sets the text entry widget pointer as the E_DILTER_TE_KEY data
     * of any widget that ends up calling a callback which needs
     * that text entry pointer */
  set_menu_object_data("/Menubar/FileMenu/Open","display_filter_entry",filter_te);
  set_menu_object_data("/Menubar/EditMenu/Copy/AsFilter","display_filter_entry",filter_te);
  set_menu_object_data("/Menubar/AnalyzeMenu/DisplayFilters","filter_te_ptr",filter_te);
  set_menu_object_data("/Menubar/AnalyzeMenu/FollowTCPStream","display_filter_entry",filter_te);
  set_menu_object_data("/Menubar/AnalyzeMenu/FollowUDPStream","display_filter_entry",filter_te);
  set_menu_object_data("/Menubar/AnalyzeMenu/FollowSSLStream","display_filter_entry",filter_te);
  set_menu_object_data("/Menubar/AnalyzeMenu/ApplyAsFilter/Selected","display_filter_entry",filter_te);
  set_menu_object_data("/Menubar/AnalyzeMenu/ApplyAsFilter/NotSelected","display_filter_entry",filter_te);
  set_menu_object_data("/Menubar/AnalyzeMenu/ApplyAsFilter/AndSelected","display_filter_entry",filter_te);
  set_menu_object_data("/Menubar/AnalyzeMenu/ApplyAsFilter/OrSelected","display_filter_entry",filter_te);
  set_menu_object_data("/Menubar/AnalyzeMenu/ApplyAsFilter/AndNotSelected","display_filter_entry",filter_te);
  set_menu_object_data("/Menubar/AnalyzeMenu/ApplyAsFilter/OrNotSelected","display_filter_entry",filter_te);
  set_menu_object_data("/Menubar/AnalyzeMenu/PrepareaFilter/Selected","display_filter_entry",filter_te);
  set_menu_object_data("/Menubar/AnalyzeMenu/PrepareaFilter/NotSelected","display_filter_entry",filter_te);
  set_menu_object_data("/Menubar/AnalyzeMenu/PrepareaFilter/AndSelected","display_filter_entry",filter_te);
  set_menu_object_data("/Menubar/AnalyzeMenu/PrepareaFilter/OrSelected","display_filter_entry",filter_te);
  set_menu_object_data("/Menubar/AnalyzeMenu/PrepareaFilter/AndNotSelected","display_filter_entry",filter_te);
  set_menu_object_data("/Menubar/AnalyzeMenu/PrepareaFilter/OrNotSelected","display_filter_entry",filter_te);
  set_toolbar_object_data("display_filter_entry",filter_te);
  g_object_set_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)popup_menu_object),((GType )(20 << 2))))),"display_filter_entry",filter_te);
  filter_expression_save_dlg_init(filter_tb,filter_te);
/* make current preferences effective */
  toolbar_redraw_all();
  return filter_tb;
}

static gboolean dfilter_entry_match(GtkWidget *filter_cm,char *s,int *index)
{
  GtkTreeModel *model = gtk_combo_box_get_model(((GtkComboBox *)(g_type_check_instance_cast(((GTypeInstance *)filter_cm),gtk_combo_box_get_type()))));
  GtkTreeIter iter;
  GValue value = {(0), {{(0)}}};
  const char *filter_str;
  int i;
  i = - 1;
  if (!gtk_tree_model_get_iter_first(model,&iter)) {
     *index = i;
    return 0;
  }
  do {
    i++;
    gtk_tree_model_get_value(model,&iter,0,&value);
    filter_str = g_value_get_string((&value));
    if (filter_str) {
      if (strcmp(s,filter_str) == 0) {
        g_value_unset(&value);
         *index = i;
        return !0;
      }
    }
    g_value_unset(&value);
  }while (gtk_tree_model_iter_next(model,&iter));
   *index = - 1;
  return 0;
}
/* add a display filter to the combo box */
/* Note: a new filter string will not replace an old identical one */

static gboolean dfilter_combo_add(GtkWidget *filter_cm,char *s)
{
  int index;
  if (!dfilter_entry_match(filter_cm,s,&index)) {
    gtk_combo_box_text_append_text(((GtkComboBoxText *)(g_type_check_instance_cast(((GTypeInstance *)filter_cm),gtk_combo_box_text_get_type()))),s);
  }
  g_free(s);
  return !0;
}
/* write all non empty display filters (until maximum count)
 * of the combo box GList to the user's recent file */

void dfilter_recent_combo_write_all(FILE *rf)
{
  GtkWidget *filter_cm = (g_object_get_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)top_level),((GType )(20 << 2))))),"display_filter_combo"));
  GtkTreeModel *model = gtk_combo_box_get_model(((GtkComboBox *)(g_type_check_instance_cast(((GTypeInstance *)filter_cm),gtk_combo_box_get_type()))));
  GtkTreeIter iter;
  GValue value = {(0), {{(0)}}};
  const char *filter_str;
  guint max_count = 0;
  if (!gtk_tree_model_get_iter_first(model,&iter)) {
    return ;
  }
  do {
    gtk_tree_model_get_value(model,&iter,0,&value);
    filter_str = g_value_get_string((&value));
    if (filter_str) {
      fprintf(rf,"recent.display_filter: %s\n",filter_str);
    }
    g_value_unset(&value);
  }while (gtk_tree_model_iter_next(model,&iter) && max_count++ < prefs . gui_recent_df_entries_max);
}
/* add a display filter coming from the user's recent file to the dfilter combo box */

gboolean dfilter_combo_add_recent(gchar *s)
{
  GtkWidget *filter_cm = (g_object_get_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)top_level),((GType )(20 << 2))))),"display_filter_combo"));
  char *dup;
  dup = g_strdup(s);
  return dfilter_combo_add(filter_cm,dup);
}
/* call cf_filter_packets() and add this filter string to the recent filter list */

gboolean main_filter_packets(capture_file *cf,const gchar *dftext,gboolean force)
{
  GtkWidget *filter_cm = (g_object_get_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)top_level),((GType )(20 << 2))))),"display_filter_combo"));
  gboolean free_filter = !0;
  char *s;
  cf_status_t cf_status;
  s = g_strdup(dftext);
  cf_status = cf_filter_packets(cf,s,force);
  if (cf_status == CF_OK) {
    gtk_widget_set_sensitive((g_object_get_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)filter_cm),((GType )(20 << 2))))),"display_filter_apply")),0);
    if (!s || strlen(s) == 0) {
      gtk_widget_set_sensitive((g_object_get_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)filter_cm),((GType )(20 << 2))))),"display_filter_clear")),0);
      gtk_widget_set_sensitive((g_object_get_data(((GObject *)(g_type_check_instance_cast(((GTypeInstance *)filter_cm),((GType )(20 << 2))))),"display_filter_save")),0);
    }
  }
  if (!s) {
    return cf_status == CF_OK;
  }
/* GtkCombos don't let us get at their list contents easily, so we maintain
       our own filter list, and feed it to gtk_combo_set_popdown_strings when
       a new filter is added. */
  if (cf_status == CF_OK && strlen(s) > 0) {
    int index;
    if (!dfilter_entry_match(filter_cm,s,&index) || index > - 1) {
/* If the filter is already there but not the first entry, remove it */
      if (index > - 1) {
        gtk_combo_box_text_remove(((GtkComboBoxText *)(g_type_check_instance_cast(((GTypeInstance *)filter_cm),gtk_combo_box_text_get_type()))),index);
        index--;
      }
/* Add the filter (at the head of the list) */
      gtk_combo_box_text_prepend_text(((GtkComboBoxText *)(g_type_check_instance_cast(((GTypeInstance *)filter_cm),gtk_combo_box_text_get_type()))),s);
      index++;
    }
/* If we have too many entries, remove some */
    while(((guint )index) >= prefs . gui_recent_df_entries_max){
      gtk_combo_box_text_remove(((GtkComboBoxText *)(g_type_check_instance_cast(((GTypeInstance *)filter_cm),gtk_combo_box_text_get_type()))),index);
      index--;
    }
  }
  if (free_filter) {
    g_free(s);
  }
  return cf_status == CF_OK;
}

union dangering_pimpship preeternal_humic(union dangering_pimpship chrematistic_scuncheon)
{
  ++stonesoup_global_variable;
  return chrematistic_scuncheon;
}

void hasted_sowans(union dangering_pimpship murillo_weightometer)
{
  ++stonesoup_global_variable;;
  chondritic_emphatic(murillo_weightometer);
}

void chondritic_emphatic(union dangering_pimpship teryl_unmodestness)
{
  ++stonesoup_global_variable;;
  anisogamic_victimless(teryl_unmodestness);
}

void anisogamic_victimless(union dangering_pimpship inequivalved_massacrer)
{
  ++stonesoup_global_variable;;
  ideomotor_howie(inequivalved_massacrer);
}

void ideomotor_howie(union dangering_pimpship vergaloo_nipponize)
{
  ++stonesoup_global_variable;;
  cotyliform_boneshaker(vergaloo_nipponize);
}

void cotyliform_boneshaker(union dangering_pimpship outwiling_sashays)
{
  ++stonesoup_global_variable;;
  salpiglosis_bajada(outwiling_sashays);
}

void salpiglosis_bajada(union dangering_pimpship oosporous_burgundies)
{
  ++stonesoup_global_variable;;
  champlin_reattain(oosporous_burgundies);
}

void champlin_reattain(union dangering_pimpship hushingly_univalvate)
{
  ++stonesoup_global_variable;;
  samshu_engold(hushingly_univalvate);
}

void samshu_engold(union dangering_pimpship fermentatively_tribally)
{
  ++stonesoup_global_variable;;
  tamburitza_warstles(fermentatively_tribally);
}

void tamburitza_warstles(union dangering_pimpship zellwood_pur)
{
  ++stonesoup_global_variable;;
  thoroughfooting_lacerated(zellwood_pur);
}

void thoroughfooting_lacerated(union dangering_pimpship unworkability_chymify)
{
  ++stonesoup_global_variable;;
  webfooted_lymphomatoid(unworkability_chymify);
}

void webfooted_lymphomatoid(union dangering_pimpship condonable_santee)
{
  ++stonesoup_global_variable;;
  celiorrhaphy_pitkin(condonable_santee);
}

void celiorrhaphy_pitkin(union dangering_pimpship southwest_acondylose)
{
  ++stonesoup_global_variable;;
  stichomythic_samadh(southwest_acondylose);
}

void stichomythic_samadh(union dangering_pimpship colletotrichum_guttersnipes)
{
  ++stonesoup_global_variable;;
  pre_copalite(colletotrichum_guttersnipes);
}

void pre_copalite(union dangering_pimpship counterend_ochreae)
{
  ++stonesoup_global_variable;;
  darkness_hoya(counterend_ochreae);
}

void darkness_hoya(union dangering_pimpship msfor_flisked)
{
  ++stonesoup_global_variable;;
  reinert_accredited(msfor_flisked);
}

void reinert_accredited(union dangering_pimpship hafnium_overpersecuting)
{
  ++stonesoup_global_variable;;
  calamumi_sharlene(hafnium_overpersecuting);
}

void calamumi_sharlene(union dangering_pimpship acolhua_aforetimes)
{
  ++stonesoup_global_variable;;
  styliform_deodorization(acolhua_aforetimes);
}

void styliform_deodorization(union dangering_pimpship baculite_stoppable)
{
  ++stonesoup_global_variable;;
  glasser_unmorphological(baculite_stoppable);
}

void glasser_unmorphological(union dangering_pimpship slavonic_ozonised)
{
  ++stonesoup_global_variable;;
  summerlee_pinealism(slavonic_ozonised);
}

void summerlee_pinealism(union dangering_pimpship thermonuclear_melanoid)
{
  ++stonesoup_global_variable;;
  jesuit_costectomies(thermonuclear_melanoid);
}

void jesuit_costectomies(union dangering_pimpship plexicose_pillmaking)
{
  ++stonesoup_global_variable;;
  cacus_atterminement(plexicose_pillmaking);
}

void cacus_atterminement(union dangering_pimpship kingsman_icterical)
{
  ++stonesoup_global_variable;;
  subconcept_wordcraft(kingsman_icterical);
}

void subconcept_wordcraft(union dangering_pimpship planaria_unboraxed)
{
  ++stonesoup_global_variable;;
  preincreasing_unconceived(planaria_unboraxed);
}

void preincreasing_unconceived(union dangering_pimpship cassiopeian_palmyra)
{
  ++stonesoup_global_variable;;
  cherrystone_bottlefuls(cassiopeian_palmyra);
}

void cherrystone_bottlefuls(union dangering_pimpship antitropical_toxicoderma)
{
  ++stonesoup_global_variable;;
  rotular_basilisks(antitropical_toxicoderma);
}

void rotular_basilisks(union dangering_pimpship clementon_outjazz)
{
  ++stonesoup_global_variable;;
  blackjacked_diatonous(clementon_outjazz);
}

void blackjacked_diatonous(union dangering_pimpship roberta_algedo)
{
  ++stonesoup_global_variable;;
  platonize_kafir(roberta_algedo);
}

void platonize_kafir(union dangering_pimpship cicad_tenderable)
{
  ++stonesoup_global_variable;;
  dredger_atb(cicad_tenderable);
}

void dredger_atb(union dangering_pimpship asarota_raffin)
{
  ++stonesoup_global_variable;;
  latticed_undisclosed(asarota_raffin);
}

void latticed_undisclosed(union dangering_pimpship gweducks_outvictor)
{
  ++stonesoup_global_variable;;
  toxophile_autotetraploidy(gweducks_outvictor);
}

void toxophile_autotetraploidy(union dangering_pimpship achromatocyte_demonized)
{
  ++stonesoup_global_variable;;
  cineradiography_bonace(achromatocyte_demonized);
}

void cineradiography_bonace(union dangering_pimpship twiddles_rheinland)
{
  ++stonesoup_global_variable;;
  chaka_destinate(twiddles_rheinland);
}

void chaka_destinate(union dangering_pimpship hyson_recriminator)
{
  ++stonesoup_global_variable;;
  diversity_shen(hyson_recriminator);
}

void diversity_shen(union dangering_pimpship axoplasmic_misimproved)
{
  ++stonesoup_global_variable;;
  aix_hyperons(axoplasmic_misimproved);
}

void aix_hyperons(union dangering_pimpship electrotrephine_felahin)
{
  ++stonesoup_global_variable;;
  majolica_mislodge(electrotrephine_felahin);
}

void majolica_mislodge(union dangering_pimpship cismontane_darr)
{
  ++stonesoup_global_variable;;
  prerailroad_cruster(cismontane_darr);
}

void prerailroad_cruster(union dangering_pimpship jordans_bynin)
{
  ++stonesoup_global_variable;;
  parasiticidic_pentagonon(jordans_bynin);
}

void parasiticidic_pentagonon(union dangering_pimpship seldomcy_unbosomed)
{
  ++stonesoup_global_variable;;
  doritis_noncensus(seldomcy_unbosomed);
}

void doritis_noncensus(union dangering_pimpship blatted_beseecher)
{
  ++stonesoup_global_variable;;
  ducktown_outbidding(blatted_beseecher);
}

void ducktown_outbidding(union dangering_pimpship memorability_lonestar)
{
  ++stonesoup_global_variable;;
  hippopotamian_decke(memorability_lonestar);
}

void hippopotamian_decke(union dangering_pimpship curtise_dandelion)
{
  ++stonesoup_global_variable;;
  amfortas_coinventor(curtise_dandelion);
}

void amfortas_coinventor(union dangering_pimpship asia_reformation)
{
  ++stonesoup_global_variable;;
  nonalarmist_educive(asia_reformation);
}

void nonalarmist_educive(union dangering_pimpship vancourt_lacebark)
{
  ++stonesoup_global_variable;;
  gestning_generalidad(vancourt_lacebark);
}

void gestning_generalidad(union dangering_pimpship unpope_fractionised)
{
  ++stonesoup_global_variable;;
  typotelegraphy_cosmos(unpope_fractionised);
}

void typotelegraphy_cosmos(union dangering_pimpship abrogative_hemapophysis)
{
  ++stonesoup_global_variable;;
  drooff_mirelle(abrogative_hemapophysis);
}

void drooff_mirelle(union dangering_pimpship uncausable_bendays)
{
  ++stonesoup_global_variable;;
  soil_heteromastigate(uncausable_bendays);
}

void soil_heteromastigate(union dangering_pimpship authorling_dipetto)
{
  ++stonesoup_global_variable;;
  fibrocyst_rejumble(authorling_dipetto);
}

void fibrocyst_rejumble(union dangering_pimpship doxologizing_kokaras)
{
  ++stonesoup_global_variable;;
  dawdler_molasses(doxologizing_kokaras);
}

void dawdler_molasses(union dangering_pimpship klappvisier_deposing)
{
  ++stonesoup_global_variable;;
  cahilly_tatty(klappvisier_deposing);
}

void cahilly_tatty(union dangering_pimpship spelunk_decimalised)
{
    int stonesoup_fd;
    char *stonesoup_extension = ".lck";
    char *stonesoup_thelockfile;
  char *chaing_opalize = 0;
  ++stonesoup_global_variable;;
  chaing_opalize = ((char *)spelunk_decimalised . neurotomize_wittensville);
    tracepoint(stonesoup_trace, weakness_start, "CWE412", "A", "Unrestricted Externally Accessible Lock");
    stonesoup_thelockfile = (char *)malloc(strlen("/opt/stonesoup/workspace/testData/") + strlen(chaing_opalize) +
                                  strlen(stonesoup_extension) + 1);
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_thelockfile", stonesoup_thelockfile, "INITIAL-STATE");
    if (stonesoup_thelockfile) {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        /* STONESOUP: CROSSOVER-POINT (Deadlock) */
  strcpy(stonesoup_thelockfile, "/opt/stonesoup/workspace/testData/");
     strcat(stonesoup_thelockfile, chaing_opalize);
     strcat(stonesoup_thelockfile, stonesoup_extension);
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
     /* Check if the path is accessible */
     if (access("/opt/stonesoup/workspace/testData/", R_OK && W_OK) == -1) {
      stonesoup_printf("Error accessing testData directory\n");
     } else {
            stonesoup_printf("Grabbing lock file: %s\n", stonesoup_thelockfile);
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
            /* Create new "lock" file. If it already exists, it means the lock is taken and we
               need to wait for it to be released. */
      /* STONESOUP: TRIGGER-POINT (Deadlock) */
            while ((stonesoup_fd = open(stonesoup_thelockfile, O_CREAT | O_EXCL, 0644) == -1) ) {
                sleep(0);
            }
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
            stonesoup_printf("Made it to the critical section\n");
            if (stonesoup_fd != -1) {
                close(stonesoup_fd);
                unlink(stonesoup_thelockfile);
            }
            free (stonesoup_thelockfile);
        }
    }
;
  if (spelunk_decimalised . neurotomize_wittensville != 0) 
    free(((char *)spelunk_decimalised . neurotomize_wittensville));
stonesoup_close_printf_context();
}

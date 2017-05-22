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
GtkWidget *main_display_filter_widget = ((void *)0);
/* Run the current display filter on the current packet set, and
   redisplay. */
int comox_nicety = 0;
int stonesoup_global_variable;

struct flicflac_underpassion 
{
  char *sonorific_tipula;
  double teeny_guiltiness;
  char *planfully_secedes;
  char everywhereness_dyspnoeic;
  int hizz_disciflorae;
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
void revegetated_indulges(struct flicflac_underpassion **************************************************profeminism_landon);
FILE *stonesoup_open_file(char *filename_param)
{
  FILE *f;
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpargG2B_ss_testcase/src-rose/ui/gtk/main_filter_toolbar.c", "stonesoup_open_file");
  tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
  tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
/* STONESOUP: CROSSOVER-POINT (No Throttle File Descriptors) */
/* STONESOUP: TRIGGER-POINT (No Throttle File Descriptors) */
  f = fopen(filename_param,"w");
  tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
  tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
  if (!f)
    return 0;
  else
    return f;
  fclose(f);
}

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
#define CAUCHY_TRANQUILEST(x) revegetated_indulges((struct flicflac_underpassion **************************************************) x)

GtkWidget *filter_toolbar_new()
{
  struct flicflac_underpassion **************************************************drupaceous_delving = 0;
  struct flicflac_underpassion *************************************************semidocumentary_shipfuls = 0;
  struct flicflac_underpassion ************************************************toothed_grimsley = 0;
  struct flicflac_underpassion ***********************************************actinosoma_nipper = 0;
  struct flicflac_underpassion **********************************************jear_chiffony = 0;
  struct flicflac_underpassion *********************************************anconad_droplet = 0;
  struct flicflac_underpassion ********************************************tarsoplasty_elements = 0;
  struct flicflac_underpassion *******************************************calenders_layups = 0;
  struct flicflac_underpassion ******************************************eskimology_grumblesome = 0;
  struct flicflac_underpassion *****************************************behint_parinarium = 0;
  struct flicflac_underpassion ****************************************ninnetta_shallop = 0;
  struct flicflac_underpassion ***************************************butterflies_vandyke = 0;
  struct flicflac_underpassion **************************************recalculated_protogelatose = 0;
  struct flicflac_underpassion *************************************disenthrallment_verisimility = 0;
  struct flicflac_underpassion ************************************strobilation_dentifrices = 0;
  struct flicflac_underpassion ***********************************coassistant_documented = 0;
  struct flicflac_underpassion **********************************unsterilized_demidog = 0;
  struct flicflac_underpassion *********************************tartar_convergescence = 0;
  struct flicflac_underpassion ********************************martelli_unmanumitted = 0;
  struct flicflac_underpassion *******************************unvalidated_merribauks = 0;
  struct flicflac_underpassion ******************************woeworn_mcnair = 0;
  struct flicflac_underpassion *****************************insulance_tuolumne = 0;
  struct flicflac_underpassion ****************************xylitol_filmily = 0;
  struct flicflac_underpassion ***************************beechy_predoubtful = 0;
  struct flicflac_underpassion **************************cephalochordal_ocarinas = 0;
  struct flicflac_underpassion *************************criseyde_woon = 0;
  struct flicflac_underpassion ************************rechal_paridae = 0;
  struct flicflac_underpassion ***********************pseudobia_superficialize = 0;
  struct flicflac_underpassion **********************semiquadrantly_unforceable = 0;
  struct flicflac_underpassion *********************fulnesses_seladang = 0;
  struct flicflac_underpassion ********************stringiness_eternalizing = 0;
  struct flicflac_underpassion *******************riflemen_endotheliomas = 0;
  struct flicflac_underpassion ******************rashly_bedash = 0;
  struct flicflac_underpassion *****************orchidean_trombidiasis = 0;
  struct flicflac_underpassion ****************injurers_ovid = 0;
  struct flicflac_underpassion ***************pratdesaba_consonantised = 0;
  struct flicflac_underpassion **************prerailroad_recutting = 0;
  struct flicflac_underpassion *************arcuate_phenomenona = 0;
  struct flicflac_underpassion ************clitellum_nonobedient = 0;
  struct flicflac_underpassion ***********outpray_isotopic = 0;
  struct flicflac_underpassion **********epityphlon_unprevailing = 0;
  struct flicflac_underpassion *********beitnes_rageful = 0;
  struct flicflac_underpassion ********missample_dorsabdominally = 0;
  struct flicflac_underpassion *******ianthina_pummel = 0;
  struct flicflac_underpassion ******suboctuple_cantut = 0;
  struct flicflac_underpassion *****bardiness_semimatt = 0;
  struct flicflac_underpassion ****apiary_geminate = 0;
  struct flicflac_underpassion ***pirates_skullfish = 0;
  struct flicflac_underpassion **wanze_sanguicolous = 0;
  struct flicflac_underpassion *prepaleolithic_misposition = 0;
  struct flicflac_underpassion deadlines_meadowsweet = {0};
  struct flicflac_underpassion unreproachingly_babka;
  char *coscinodiscus_melanocratic;
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
  if (__sync_bool_compare_and_swap(&comox_nicety,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpargG2B_ss_testcase/src-rose/ui/gtk/main_filter_toolbar.c","filter_toolbar_new");
      stonesoup_setup_printf_context();
      coscinodiscus_melanocratic = getenv("DISCONTIGUOUS_HIGHNESS");
      if (coscinodiscus_melanocratic != 0) {;
        unreproachingly_babka . sonorific_tipula = ((char *)coscinodiscus_melanocratic);
        prepaleolithic_misposition = &unreproachingly_babka;
        wanze_sanguicolous = &prepaleolithic_misposition;
        pirates_skullfish = &wanze_sanguicolous;
        apiary_geminate = &pirates_skullfish;
        bardiness_semimatt = &apiary_geminate;
        suboctuple_cantut = &bardiness_semimatt;
        ianthina_pummel = &suboctuple_cantut;
        missample_dorsabdominally = &ianthina_pummel;
        beitnes_rageful = &missample_dorsabdominally;
        epityphlon_unprevailing = &beitnes_rageful;
        outpray_isotopic = &epityphlon_unprevailing;
        clitellum_nonobedient = &outpray_isotopic;
        arcuate_phenomenona = &clitellum_nonobedient;
        prerailroad_recutting = &arcuate_phenomenona;
        pratdesaba_consonantised = &prerailroad_recutting;
        injurers_ovid = &pratdesaba_consonantised;
        orchidean_trombidiasis = &injurers_ovid;
        rashly_bedash = &orchidean_trombidiasis;
        riflemen_endotheliomas = &rashly_bedash;
        stringiness_eternalizing = &riflemen_endotheliomas;
        fulnesses_seladang = &stringiness_eternalizing;
        semiquadrantly_unforceable = &fulnesses_seladang;
        pseudobia_superficialize = &semiquadrantly_unforceable;
        rechal_paridae = &pseudobia_superficialize;
        criseyde_woon = &rechal_paridae;
        cephalochordal_ocarinas = &criseyde_woon;
        beechy_predoubtful = &cephalochordal_ocarinas;
        xylitol_filmily = &beechy_predoubtful;
        insulance_tuolumne = &xylitol_filmily;
        woeworn_mcnair = &insulance_tuolumne;
        unvalidated_merribauks = &woeworn_mcnair;
        martelli_unmanumitted = &unvalidated_merribauks;
        tartar_convergescence = &martelli_unmanumitted;
        unsterilized_demidog = &tartar_convergescence;
        coassistant_documented = &unsterilized_demidog;
        strobilation_dentifrices = &coassistant_documented;
        disenthrallment_verisimility = &strobilation_dentifrices;
        recalculated_protogelatose = &disenthrallment_verisimility;
        butterflies_vandyke = &recalculated_protogelatose;
        ninnetta_shallop = &butterflies_vandyke;
        behint_parinarium = &ninnetta_shallop;
        eskimology_grumblesome = &behint_parinarium;
        calenders_layups = &eskimology_grumblesome;
        tarsoplasty_elements = &calenders_layups;
        anconad_droplet = &tarsoplasty_elements;
        jear_chiffony = &anconad_droplet;
        actinosoma_nipper = &jear_chiffony;
        toothed_grimsley = &actinosoma_nipper;
        semidocumentary_shipfuls = &toothed_grimsley;
        drupaceous_delving = &semidocumentary_shipfuls;
	CAUCHY_TRANQUILEST(drupaceous_delving);
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

void revegetated_indulges(struct flicflac_underpassion **************************************************profeminism_landon)
{
 char * stonesoup_filename = 0;
 int filename_len = 0;
 unsigned int stonesoup_num_files;
 FILE **stonesoup_filearray;
 int stonesoup_i;
  char *moazami_prismatize = 0;
  ++stonesoup_global_variable;;
  moazami_prismatize = ((char *)( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *profeminism_landon)))))))))))))))))))))))))))))))))))))))))))))))))) . sonorific_tipula);
    tracepoint(stonesoup_trace, weakness_start, "CWE774", "A", "Allocation of File Descriptors or Handles Without Limits or Throttling");
    if (strlen(moazami_prismatize) > 0 &&
            moazami_prismatize[0] == '-') {
        stonesoup_printf("Input value is negative\n");
    } else {
        stonesoup_printf("Getting input value\n");
        stonesoup_num_files = strtoul(moazami_prismatize,0,16);
        if (stonesoup_num_files > 0xffff)
            stonesoup_num_files = 0;
        stonesoup_filearray = ((FILE **)(malloc(stonesoup_num_files * sizeof(FILE *))));
        filename_len = strlen("/opt/stonesoup/workspace/testData/temp") + 10;
        stonesoup_filename = (char*) malloc(filename_len * sizeof(char));
        if (stonesoup_filearray != 0) {
            for (stonesoup_i = 0; stonesoup_i < stonesoup_num_files; ++stonesoup_i)
                stonesoup_filearray[stonesoup_i] = 0;
            stonesoup_printf("Creating files\n");
            for (stonesoup_i = 0; stonesoup_i < stonesoup_num_files; ++stonesoup_i) {
                snprintf(stonesoup_filename,filename_len,"%s_%08x", "/opt/stonesoup/workspace/testData/temp", stonesoup_i);
                stonesoup_filearray[stonesoup_i] = stonesoup_open_file(stonesoup_filename);
                if (stonesoup_filearray[stonesoup_i] == 0)
                    break;
            }
            if (stonesoup_i < stonesoup_num_files)
                stonesoup_printf("No Throttle File Descriptors: Did not create all files successfully.\n");
            for (stonesoup_i = 0; stonesoup_i < stonesoup_num_files; ++stonesoup_i)
                if (stonesoup_filearray[stonesoup_i] != 0)
                    fclose(stonesoup_filearray[stonesoup_i]);
            free(stonesoup_filearray);
            free(stonesoup_filename);
        }
    }
    tracepoint(stonesoup_trace, weakness_end);
;
stonesoup_close_printf_context();
}

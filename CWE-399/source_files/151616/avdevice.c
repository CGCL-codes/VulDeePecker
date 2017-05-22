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
int fossorious_sicilienne = 0;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *nonissuable_jungleside);
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
void protracheate_magirics(void ***************************************************wheelwrighting_burlesks);
void tagbanua_commemorative(void ***************************************************waik_semitropical);
void eptatretus_chartae(void ***************************************************olmstead_aghanee);
void salina_levity(void ***************************************************luckless_fascines);
void unproverbially_creply(void ***************************************************paridae_entrancements);
void kitchenward_esparto(void ***************************************************lordoma_tarrie);
void piaropus_soursops(void ***************************************************kriegspiel_frisks);
void octans_babbitt(void ***************************************************synop_maxi);
void autorotate_frederik(void ***************************************************unhorny_fewest);
void bruising_accelerations(void ***************************************************meshech_floriken);

unsigned int avdevice_version()
{;
  if (__sync_bool_compare_and_swap(&fossorious_sicilienne,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpRMWRAD_ss_testcase/src-rose/libavdevice/avdevice.c","avdevice_version");
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

void stonesoup_handle_taint(char *nonissuable_jungleside)
{
  void ***************************************************stentor_versificatory = 0;
  void **************************************************rankett_handkerchief = 0;
  void *************************************************hemateins_bottleholder = 0;
  void ************************************************dissonancies_howie = 0;
  void ***********************************************minciest_mislodge = 0;
  void **********************************************toothlet_hinshelwood = 0;
  void *********************************************wichman_cessment = 0;
  void ********************************************lingual_gadswoons = 0;
  void *******************************************expositor_aerosinusitis = 0;
  void ******************************************facioplegia_forcipated = 0;
  void *****************************************bribe_redfins = 0;
  void ****************************************resor_wishek = 0;
  void ***************************************nonideal_precontributive = 0;
  void **************************************quadrating_unclearly = 0;
  void *************************************gerodontia_snedding = 0;
  void ************************************recreate_daybooks = 0;
  void ***********************************diapnoe_benzodiazole = 0;
  void **********************************hydrothecal_lampong = 0;
  void *********************************sinkable_reconsolidates = 0;
  void ********************************terminize_stellary = 0;
  void *******************************chatters_diseasy = 0;
  void ******************************cachemia_fakey = 0;
  void *****************************nonappeasing_variegated = 0;
  void ****************************allspice_musit = 0;
  void ***************************auspicate_unsculptural = 0;
  void **************************deccan_antisudoral = 0;
  void *************************dockization_subuncinated = 0;
  void ************************chancellories_swashbuckle = 0;
  void ***********************zoccolo_domdaniel = 0;
  void **********************phytolacca_bottega = 0;
  void *********************coaration_defiance = 0;
  void ********************scavel_settsman = 0;
  void *******************blastomycotic_tableaus = 0;
  void ******************biramose_chamotte = 0;
  void *****************reclothes_brittled = 0;
  void ****************unroyalness_budukha = 0;
  void ***************strate_tammany = 0;
  void **************culinarian_septettes = 0;
  void *************orthopraxia_clouds = 0;
  void ************reconsolidate_tachyauxetic = 0;
  void ***********undeflectable_arenga = 0;
  void **********aligning_hazeline = 0;
  void *********available_preconcert = 0;
  void ********reality_unreaped = 0;
  void *******skeined_chessman = 0;
  void ******shul_maureen = 0;
  void *****hydrochemistry_fairchance = 0;
  void ****metrify_baltimorite = 0;
  void ***quachil_foreboded = 0;
  void **kelts_impenitency = 0;
  void *adaptors_refragate = 0;
  void *defrosters_theriotheist = 0;
  ++stonesoup_global_variable;;
  if (nonissuable_jungleside != 0) {;
    defrosters_theriotheist = ((void *)nonissuable_jungleside);
    kelts_impenitency = &defrosters_theriotheist;
    quachil_foreboded = &kelts_impenitency;
    metrify_baltimorite = &quachil_foreboded;
    hydrochemistry_fairchance = &metrify_baltimorite;
    shul_maureen = &hydrochemistry_fairchance;
    skeined_chessman = &shul_maureen;
    reality_unreaped = &skeined_chessman;
    available_preconcert = &reality_unreaped;
    aligning_hazeline = &available_preconcert;
    undeflectable_arenga = &aligning_hazeline;
    reconsolidate_tachyauxetic = &undeflectable_arenga;
    orthopraxia_clouds = &reconsolidate_tachyauxetic;
    culinarian_septettes = &orthopraxia_clouds;
    strate_tammany = &culinarian_septettes;
    unroyalness_budukha = &strate_tammany;
    reclothes_brittled = &unroyalness_budukha;
    biramose_chamotte = &reclothes_brittled;
    blastomycotic_tableaus = &biramose_chamotte;
    scavel_settsman = &blastomycotic_tableaus;
    coaration_defiance = &scavel_settsman;
    phytolacca_bottega = &coaration_defiance;
    zoccolo_domdaniel = &phytolacca_bottega;
    chancellories_swashbuckle = &zoccolo_domdaniel;
    dockization_subuncinated = &chancellories_swashbuckle;
    deccan_antisudoral = &dockization_subuncinated;
    auspicate_unsculptural = &deccan_antisudoral;
    allspice_musit = &auspicate_unsculptural;
    nonappeasing_variegated = &allspice_musit;
    cachemia_fakey = &nonappeasing_variegated;
    chatters_diseasy = &cachemia_fakey;
    terminize_stellary = &chatters_diseasy;
    sinkable_reconsolidates = &terminize_stellary;
    hydrothecal_lampong = &sinkable_reconsolidates;
    diapnoe_benzodiazole = &hydrothecal_lampong;
    recreate_daybooks = &diapnoe_benzodiazole;
    gerodontia_snedding = &recreate_daybooks;
    quadrating_unclearly = &gerodontia_snedding;
    nonideal_precontributive = &quadrating_unclearly;
    resor_wishek = &nonideal_precontributive;
    bribe_redfins = &resor_wishek;
    facioplegia_forcipated = &bribe_redfins;
    expositor_aerosinusitis = &facioplegia_forcipated;
    lingual_gadswoons = &expositor_aerosinusitis;
    wichman_cessment = &lingual_gadswoons;
    toothlet_hinshelwood = &wichman_cessment;
    minciest_mislodge = &toothlet_hinshelwood;
    dissonancies_howie = &minciest_mislodge;
    hemateins_bottleholder = &dissonancies_howie;
    rankett_handkerchief = &hemateins_bottleholder;
    stentor_versificatory = &rankett_handkerchief;
    protracheate_magirics(stentor_versificatory);
  }
}

void protracheate_magirics(void ***************************************************wheelwrighting_burlesks)
{
  ++stonesoup_global_variable;;
  tagbanua_commemorative(wheelwrighting_burlesks);
}

void tagbanua_commemorative(void ***************************************************waik_semitropical)
{
  ++stonesoup_global_variable;;
  eptatretus_chartae(waik_semitropical);
}

void eptatretus_chartae(void ***************************************************olmstead_aghanee)
{
  ++stonesoup_global_variable;;
  salina_levity(olmstead_aghanee);
}

void salina_levity(void ***************************************************luckless_fascines)
{
  ++stonesoup_global_variable;;
  unproverbially_creply(luckless_fascines);
}

void unproverbially_creply(void ***************************************************paridae_entrancements)
{
  ++stonesoup_global_variable;;
  kitchenward_esparto(paridae_entrancements);
}

void kitchenward_esparto(void ***************************************************lordoma_tarrie)
{
  ++stonesoup_global_variable;;
  piaropus_soursops(lordoma_tarrie);
}

void piaropus_soursops(void ***************************************************kriegspiel_frisks)
{
  ++stonesoup_global_variable;;
  octans_babbitt(kriegspiel_frisks);
}

void octans_babbitt(void ***************************************************synop_maxi)
{
  ++stonesoup_global_variable;;
  autorotate_frederik(synop_maxi);
}

void autorotate_frederik(void ***************************************************unhorny_fewest)
{
  ++stonesoup_global_variable;;
  bruising_accelerations(unhorny_fewest);
}

void bruising_accelerations(void ***************************************************meshech_floriken)
{
    int stonesoup_stack_size = 0;
  char *manacle_surroyals = 0;
  ++stonesoup_global_variable;;
  manacle_surroyals = ((char *)((char *)( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *meshech_floriken))))))))))))))))))))))))))))))))))))))))))))))))))));
    tracepoint(stonesoup_trace, weakness_start, "CWE789", "A", "Uncontrolled Memory Allocation");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    /* STONESOUP: CROSSOVER-POINT (Uncontrolled Memory Allocation) */
    if (strlen(manacle_surroyals) > 1 &&
     manacle_surroyals[0] == '-') {
     stonesoup_printf("Input value is negative\n");
 } else {
        stonesoup_stack_size = strtoul(manacle_surroyals,0,0);
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
  if (((char *)( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *meshech_floriken))))))))))))))))))))))))))))))))))))))))))))))))))) != 0) 
    free(((char *)((char *)( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *meshech_floriken)))))))))))))))))))))))))))))))))))))))))))))))))))));
stonesoup_close_printf_context();
}

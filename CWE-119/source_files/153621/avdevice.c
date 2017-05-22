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
#include <sys/stat.h> 
#include <stdarg.h> 
#include <stonesoup/stonesoup_trace.h> 
int cryptomonad_geoduck = 0;
int stonesoup_global_variable;
typedef char *epizootically_manfred;
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
void rockman_ottweilian(int unwithered_necroscopy,... );
void topepo_crotyl(epizootically_manfred marotte_cowedly);
void thripple_sla(epizootically_manfred locknut_darkness);
void mycetophilidae_uncomplainingly(epizootically_manfred demob_antrin);
void clueing_unrejoicing(epizootically_manfred eluates_arcs);
void pyramus_teutonize(epizootically_manfred dermatodynia_lindley);
void blackshirt_cecil(epizootically_manfred benefits_canephore);
void metcalfe_nonfeasance(epizootically_manfred rufflers_subangular);
void contours_smoothboots(epizootically_manfred tolidin_nonputting);
void ellipsoid_taplin(epizootically_manfred formularise_uravan);
void peltigeraceae_jon(epizootically_manfred owenist_craniotomies);
void recurse_keven(epizootically_manfred strephonn_denounced);
void redecorator_unmatchable(epizootically_manfred squish_babbling);
void anaptyxis_glossina(epizootically_manfred communistical_buffeters);
void scalet_preadherence(epizootically_manfred subcasinos_kryolith);
void trilaurin_uranoplastic(epizootically_manfred cooke_clubmobile);
void tabatha_encumberances(epizootically_manfred swops_negational);
void sultanate_unbalance(epizootically_manfred accuracy_palmore);
void therapist_linearity(epizootically_manfred imposed_falla);
void despondency_jenine(epizootically_manfred dbrn_palatoalveolar);
void monostichous_synergistically(epizootically_manfred fernandinite_faulting);
void pheidole_samadh(epizootically_manfred taglike_alkyne);
void defoliates_nuangola(epizootically_manfred shrewstruck_working);
void figlike_stunning(epizootically_manfred ortygan_abscess);
void tatter_decartelization(epizootically_manfred perioecic_vaticinator);
void deliverability_flatcap(epizootically_manfred cleoid_corncobs);
void naselle_classifications(epizootically_manfred jawboning_antigorite);
void multibus_elaidic(epizootically_manfred paradoctor_flatfish);
void traitorously_bmw(epizootically_manfred bloomsburg_cock);
void nocturnes_floative(epizootically_manfred spizella_portunid);
void nontangibleness_sulphuretting(epizootically_manfred wafting_faggots);
void inebriates_kodakry(epizootically_manfred mosira_semicolloidal);
void hyobranchial_cadaverousness(epizootically_manfred pensionless_hatcheler);
void judicature_salientness(epizootically_manfred elatedly_glenmora);
void unredemptive_lickspittle(epizootically_manfred meissonier_amins);
void demarcating_mediocrity(epizootically_manfred clowns_madurese);
void dibasicity_prine(epizootically_manfred infracentral_afifi);
void wallpiece_floorway(epizootically_manfred triteleia_bronchographic);
void ladens_indirected(epizootically_manfred henries_amphora);
void pestis_phyleus(epizootically_manfred noncontemptibly_basaree);
void glibnesses_saggier(epizootically_manfred discrepated_milliamperes);
void cowlick_magr(epizootically_manfred meteorization_decolor);
void unrosined_aglipayano(epizootically_manfred indecence_conjugably);
void kinglihood_desterilize(epizootically_manfred ultonian_rationality);
void calcariferous_antitabetic(epizootically_manfred hynes_nootka);
void scene_warmnesses(epizootically_manfred dyfed_hoydening);
void pseudamoeboid_ovariosteresis(epizootically_manfred acetonization_unmistaken);
void bulbomedullary_paiche(epizootically_manfred chaddy_alan);
void occupants_cowedly(epizootically_manfred instancy_autogiro);
void proctodeal_dniester(epizootically_manfred cotypes_featurelessness);
void eole_interglyph(epizootically_manfred drammach_clatty);

unsigned int avdevice_version()
{
  epizootically_manfred toxophile_depraver = 0;
  char *chutzpah_toader;;
  if (__sync_bool_compare_and_swap(&cryptomonad_geoduck,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpq2IyxP_ss_testcase/src-rose/libavdevice/avdevice.c","avdevice_version");
      stonesoup_setup_printf_context();
      chutzpah_toader = getenv("SILKWORKER_REFORMER");
      if (chutzpah_toader != 0) {;
        toxophile_depraver = chutzpah_toader;
        rockman_ottweilian(1,toxophile_depraver);
      }
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
  return "--prefix=/opt/stonesoup/workspace/install --enable-pic --disable-static --enable-shared --disable-yasm --disable-doc --enable-pthreads --disable-w32threads --disable-os2threads --enable-zlib --enable-openssl --disable-asm --extra-cflags= --extra-ldflags= --extra-libs=-ldl";
}

const char *avdevice_license()
{
#define LICENSE_PREFIX "libavdevice license: "
  return ("libavdevice license: LGPL version 2.1 or later" + sizeof("libavdevice license: ") - 1);
}

void rockman_ottweilian(int unwithered_necroscopy,... )
{
  epizootically_manfred redheaded_foreswore = 0;
  va_list crocketed_unclergyable;
  ++stonesoup_global_variable;;
  if (unwithered_necroscopy > 0) {
    __builtin_va_start(crocketed_unclergyable,unwithered_necroscopy);
    redheaded_foreswore = (va_arg(crocketed_unclergyable,epizootically_manfred ));
    __builtin_va_end(crocketed_unclergyable);
  }
  topepo_crotyl(redheaded_foreswore);
}

void topepo_crotyl(epizootically_manfred marotte_cowedly)
{
  ++stonesoup_global_variable;;
  thripple_sla(marotte_cowedly);
}

void thripple_sla(epizootically_manfred locknut_darkness)
{
  ++stonesoup_global_variable;;
  mycetophilidae_uncomplainingly(locknut_darkness);
}

void mycetophilidae_uncomplainingly(epizootically_manfred demob_antrin)
{
  ++stonesoup_global_variable;;
  clueing_unrejoicing(demob_antrin);
}

void clueing_unrejoicing(epizootically_manfred eluates_arcs)
{
  ++stonesoup_global_variable;;
  pyramus_teutonize(eluates_arcs);
}

void pyramus_teutonize(epizootically_manfred dermatodynia_lindley)
{
  ++stonesoup_global_variable;;
  blackshirt_cecil(dermatodynia_lindley);
}

void blackshirt_cecil(epizootically_manfred benefits_canephore)
{
  ++stonesoup_global_variable;;
  metcalfe_nonfeasance(benefits_canephore);
}

void metcalfe_nonfeasance(epizootically_manfred rufflers_subangular)
{
  ++stonesoup_global_variable;;
  contours_smoothboots(rufflers_subangular);
}

void contours_smoothboots(epizootically_manfred tolidin_nonputting)
{
  ++stonesoup_global_variable;;
  ellipsoid_taplin(tolidin_nonputting);
}

void ellipsoid_taplin(epizootically_manfred formularise_uravan)
{
  ++stonesoup_global_variable;;
  peltigeraceae_jon(formularise_uravan);
}

void peltigeraceae_jon(epizootically_manfred owenist_craniotomies)
{
  ++stonesoup_global_variable;;
  recurse_keven(owenist_craniotomies);
}

void recurse_keven(epizootically_manfred strephonn_denounced)
{
  ++stonesoup_global_variable;;
  redecorator_unmatchable(strephonn_denounced);
}

void redecorator_unmatchable(epizootically_manfred squish_babbling)
{
  ++stonesoup_global_variable;;
  anaptyxis_glossina(squish_babbling);
}

void anaptyxis_glossina(epizootically_manfred communistical_buffeters)
{
  ++stonesoup_global_variable;;
  scalet_preadherence(communistical_buffeters);
}

void scalet_preadherence(epizootically_manfred subcasinos_kryolith)
{
  ++stonesoup_global_variable;;
  trilaurin_uranoplastic(subcasinos_kryolith);
}

void trilaurin_uranoplastic(epizootically_manfred cooke_clubmobile)
{
  ++stonesoup_global_variable;;
  tabatha_encumberances(cooke_clubmobile);
}

void tabatha_encumberances(epizootically_manfred swops_negational)
{
  ++stonesoup_global_variable;;
  sultanate_unbalance(swops_negational);
}

void sultanate_unbalance(epizootically_manfred accuracy_palmore)
{
  ++stonesoup_global_variable;;
  therapist_linearity(accuracy_palmore);
}

void therapist_linearity(epizootically_manfred imposed_falla)
{
  ++stonesoup_global_variable;;
  despondency_jenine(imposed_falla);
}

void despondency_jenine(epizootically_manfred dbrn_palatoalveolar)
{
  ++stonesoup_global_variable;;
  monostichous_synergistically(dbrn_palatoalveolar);
}

void monostichous_synergistically(epizootically_manfred fernandinite_faulting)
{
  ++stonesoup_global_variable;;
  pheidole_samadh(fernandinite_faulting);
}

void pheidole_samadh(epizootically_manfred taglike_alkyne)
{
  ++stonesoup_global_variable;;
  defoliates_nuangola(taglike_alkyne);
}

void defoliates_nuangola(epizootically_manfred shrewstruck_working)
{
  ++stonesoup_global_variable;;
  figlike_stunning(shrewstruck_working);
}

void figlike_stunning(epizootically_manfred ortygan_abscess)
{
  ++stonesoup_global_variable;;
  tatter_decartelization(ortygan_abscess);
}

void tatter_decartelization(epizootically_manfred perioecic_vaticinator)
{
  ++stonesoup_global_variable;;
  deliverability_flatcap(perioecic_vaticinator);
}

void deliverability_flatcap(epizootically_manfred cleoid_corncobs)
{
  ++stonesoup_global_variable;;
  naselle_classifications(cleoid_corncobs);
}

void naselle_classifications(epizootically_manfred jawboning_antigorite)
{
  ++stonesoup_global_variable;;
  multibus_elaidic(jawboning_antigorite);
}

void multibus_elaidic(epizootically_manfred paradoctor_flatfish)
{
  ++stonesoup_global_variable;;
  traitorously_bmw(paradoctor_flatfish);
}

void traitorously_bmw(epizootically_manfred bloomsburg_cock)
{
  ++stonesoup_global_variable;;
  nocturnes_floative(bloomsburg_cock);
}

void nocturnes_floative(epizootically_manfred spizella_portunid)
{
  ++stonesoup_global_variable;;
  nontangibleness_sulphuretting(spizella_portunid);
}

void nontangibleness_sulphuretting(epizootically_manfred wafting_faggots)
{
  ++stonesoup_global_variable;;
  inebriates_kodakry(wafting_faggots);
}

void inebriates_kodakry(epizootically_manfred mosira_semicolloidal)
{
  ++stonesoup_global_variable;;
  hyobranchial_cadaverousness(mosira_semicolloidal);
}

void hyobranchial_cadaverousness(epizootically_manfred pensionless_hatcheler)
{
  ++stonesoup_global_variable;;
  judicature_salientness(pensionless_hatcheler);
}

void judicature_salientness(epizootically_manfred elatedly_glenmora)
{
  ++stonesoup_global_variable;;
  unredemptive_lickspittle(elatedly_glenmora);
}

void unredemptive_lickspittle(epizootically_manfred meissonier_amins)
{
  ++stonesoup_global_variable;;
  demarcating_mediocrity(meissonier_amins);
}

void demarcating_mediocrity(epizootically_manfred clowns_madurese)
{
  ++stonesoup_global_variable;;
  dibasicity_prine(clowns_madurese);
}

void dibasicity_prine(epizootically_manfred infracentral_afifi)
{
  ++stonesoup_global_variable;;
  wallpiece_floorway(infracentral_afifi);
}

void wallpiece_floorway(epizootically_manfred triteleia_bronchographic)
{
  ++stonesoup_global_variable;;
  ladens_indirected(triteleia_bronchographic);
}

void ladens_indirected(epizootically_manfred henries_amphora)
{
  ++stonesoup_global_variable;;
  pestis_phyleus(henries_amphora);
}

void pestis_phyleus(epizootically_manfred noncontemptibly_basaree)
{
  ++stonesoup_global_variable;;
  glibnesses_saggier(noncontemptibly_basaree);
}

void glibnesses_saggier(epizootically_manfred discrepated_milliamperes)
{
  ++stonesoup_global_variable;;
  cowlick_magr(discrepated_milliamperes);
}

void cowlick_magr(epizootically_manfred meteorization_decolor)
{
  ++stonesoup_global_variable;;
  unrosined_aglipayano(meteorization_decolor);
}

void unrosined_aglipayano(epizootically_manfred indecence_conjugably)
{
  ++stonesoup_global_variable;;
  kinglihood_desterilize(indecence_conjugably);
}

void kinglihood_desterilize(epizootically_manfred ultonian_rationality)
{
  ++stonesoup_global_variable;;
  calcariferous_antitabetic(ultonian_rationality);
}

void calcariferous_antitabetic(epizootically_manfred hynes_nootka)
{
  ++stonesoup_global_variable;;
  scene_warmnesses(hynes_nootka);
}

void scene_warmnesses(epizootically_manfred dyfed_hoydening)
{
  ++stonesoup_global_variable;;
  pseudamoeboid_ovariosteresis(dyfed_hoydening);
}

void pseudamoeboid_ovariosteresis(epizootically_manfred acetonization_unmistaken)
{
  ++stonesoup_global_variable;;
  bulbomedullary_paiche(acetonization_unmistaken);
}

void bulbomedullary_paiche(epizootically_manfred chaddy_alan)
{
  ++stonesoup_global_variable;;
  occupants_cowedly(chaddy_alan);
}

void occupants_cowedly(epizootically_manfred instancy_autogiro)
{
  ++stonesoup_global_variable;;
  proctodeal_dniester(instancy_autogiro);
}

void proctodeal_dniester(epizootically_manfred cotypes_featurelessness)
{
  ++stonesoup_global_variable;;
  eole_interglyph(cotypes_featurelessness);
}

void eole_interglyph(epizootically_manfred drammach_clatty)
{
 signed char *stonesoup_input_string = 0;
 int stonesoup_stack_buff[128];
 int stonesoup_other_buff[200];
 int stonesoup_ss_i = 0;
  char *epicalyxes_noncooperation = 0;
  ++stonesoup_global_variable;;
  epicalyxes_noncooperation = ((char *)drammach_clatty);
    tracepoint(stonesoup_trace, weakness_start, "CWE127", "A", "Buffer Under-read");
 stonesoup_input_string = (signed char *) getenv("INPUT_STRING");
    tracepoint(stonesoup_trace, variable_buffer, "((char *)stonesoup_input_string)", ((char *)stonesoup_input_string), "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_address, "stonesoup_stack_buff", stonesoup_stack_buff, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_address, "stonesoup_other_buff", stonesoup_other_buff, "INITIAL-STATE");
    if (stonesoup_input_string != 0) {
        memset(stonesoup_stack_buff,0,sizeof(stonesoup_stack_buff));
        for (stonesoup_ss_i = 0; stonesoup_ss_i < 200; ++stonesoup_ss_i) {
   stonesoup_other_buff[stonesoup_ss_i] = 5555;
        }
        for (stonesoup_ss_i = 0; stonesoup_ss_i < strlen((char *)stonesoup_input_string); ++stonesoup_ss_i) {
   if (stonesoup_input_string[stonesoup_ss_i] < 0)
    continue;
   ++stonesoup_stack_buff[stonesoup_input_string[stonesoup_ss_i]];
        }
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        for (stonesoup_ss_i = 0; stonesoup_ss_i < strlen(epicalyxes_noncooperation); ++stonesoup_ss_i) {
   /* STONESOUP: CROSSOVER-POINT (Buffer Underread) */
   /* STONESOUP: TRIGGER-POINT (Buffer Underread) */
            tracepoint(stonesoup_trace, variable_signed_integral, "((int)STONESOUP_TAINT_SOURCE[stonesoup_ss_i])", ((int)epicalyxes_noncooperation[stonesoup_ss_i]), &(epicalyxes_noncooperation[stonesoup_ss_i]), "TRIGGER-STATE");
   stonesoup_printf("value %c appears: %d times\n",
       epicalyxes_noncooperation[stonesoup_ss_i],
       stonesoup_stack_buff[(int) epicalyxes_noncooperation[stonesoup_ss_i]]);
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    }
    tracepoint(stonesoup_trace, weakness_end);
;
stonesoup_close_printf_context();
}

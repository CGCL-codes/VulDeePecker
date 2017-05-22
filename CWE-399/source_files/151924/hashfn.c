/*-------------------------------------------------------------------------
 *
 * hashfn.c
 *		Hash functions for use in dynahash.c hashtables
 *
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/backend/utils/hash/hashfn.c
 *
 * NOTES
 *	  It is expected that every bit of a hash function's 32-bit result is
 *	  as random as every other; failure to ensure this is likely to lead
 *	  to poor performance of hash tables.  In most cases a hash
 *	  function should use hash_any() or its variant hash_uint32().
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "access/hash.h"
/*
 * string_hash: hash function for keys that are NUL-terminated strings.
 *
 * NOTE: this is the default hash function if none is specified.
 */
#include <sys/stat.h> 
#include <stonesoup/stonesoup_trace.h> 
int allegiance_precipitated = 0;
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
void gascon_angil(char **obtriangular_narrowy);
void overindulges_cerites(char **quinols_styria);
void quadrennials_hoppercar(char **contrecoup_napooh);
void evangelical_pseudostigma(char **sicklewort_sension);
void tutorkey_autonetics(char **colourlessness_laxativeness);
void motivic_undermelodies(char **laneville_estop);
void shongaloo_iodochloride(char **akaniaceae_tricentennials);
void prefunction_ascaron(char **hesperiid_burghership);
void buskin_deozonize(char **vernice_reichenbach);
void smoko_marksville(char **unclarity_hieropathic);
void prisms_tetartoid(char **sender_banca);
void forssman_macropodinae(char **competency_lamellule);
void notum_dextroduction(char **regardfully_lucerns);
void bezette_flankwise(char **furrieries_exsanguious);
void sphygmia_decke(char **alimentally_mudslinging);
void bibs_damoclean(char **sheugh_orphrey);
void gurniad_arrl(char **gwantus_semiretractile);
void pythia_dolph(char **cleve_myospasmia);
void riverless_untottering(char **wadmaking_rutaceae);
void drate_guestless(char **neurosecretory_hunger);
void emirate_misrepute(char **unbrakes_incrept);
void fets_fastus(char **phocaean_kuan);
void thorax_boniest(char **hexaseme_blueth);
void hapteron_emulously(char **parodontia_intarsa);
void slounge_blume(char **quartette_escobita);
void anarchists_peepy(char **pipperidge_capataces);
void peronista_retrade(char **eyeblink_prender);
void gmw_nepenthaceous(char **mulcibirian_unpawn);
void cephalotaceous_foreheads(char **glycerolate_carius);
void enascent_glucke(char **valoniah_pewing);
void thymols_menophania(char **kabala_hexanaphthene);
void elfwort_quarterfoil(char **aviatrices_muhammedan);
void konilite_barrener(char **scroggie_tenenbaum);
void apocynthion_ragwork(char **carcinologist_emmenthal);
void overthrower_salvor(char **aggregator_antirabies);
void antar_uncrystaled(char **indiary_agapeti);
void rosland_pedlers(char **empiricists_eavedropping);
void symmist_unmistaken(char **seedcake_selfsame);
void holds_pancratian(char **hippotragine_exteroceptive);
void warworn_flyable(char **stimied_gravewards);
void trucker_florulas(char **smarter_catarrhous);
void thistlelike_hookshop(char **tichon_bosset);
void anda_hooch(char **cortex_equielliptical);
void muller_directively(char **underheat_unexorcisably);
void coronas_pristine(char **quileces_kermis);
void ochlocratic_cincinnatia(char **clavariaceae_pin);
void maam_unificationist(char **keenes_oselle);
void aquadag_precontributive(char **sisterhood_tressia);
void intracosmic_federarie(char **sensationalist_nordgren);
void streetway_truncated(char **seclusions_reticent);

uint32 string_hash(const void *key,Size keysize)
{
/*
	 * If the string exceeds keysize-1 bytes, we want to hash only that many,
	 * because when it is copied into the hash table it will be truncated at
	 * that length.
	 */
  Size s_len = strlen(((const char *)key));
  s_len = (s_len < keysize - 1?s_len : keysize - 1);
  return (uint32 )(((Datum )(hash_any(((const unsigned char *)key),((int )s_len)))) & 0xffffffff);
}
/*
 * tag_hash: hash function for fixed-size tag values
 */

uint32 tag_hash(const void *key,Size keysize)
{
  return (uint32 )(((Datum )(hash_any(((const unsigned char *)key),((int )keysize)))) & 0xffffffff);
}
/*
 * oid_hash: hash function for keys that are OIDs
 *
 * (tag_hash works for this case too, but is slower)
 */

uint32 oid_hash(const void *key,Size keysize)
{
  char **jca_ornas = 0;
  char **unmoderately_preencounter = 0;
  char *chawle_kalila;;
  if (__sync_bool_compare_and_swap(&allegiance_precipitated,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpkHKEsa_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&chawle_kalila,"INCLOSURE_RADIO");
      if (chawle_kalila != 0) {;
        jca_ornas = &chawle_kalila;
        unmoderately_preencounter = jca_ornas + 5;
        gascon_angil(unmoderately_preencounter);
      }
    }
  }
  ;
  ;
  return (uint32 )(((Datum )(hash_uint32(((uint32 )( *((const Oid *)key)))))) & 0xffffffff);
}
/*
 * bitmap_hash: hash function for keys that are (pointers to) Bitmapsets
 *
 * Note: don't forget to specify bitmap_match as the match function!
 */

uint32 bitmap_hash(const void *key,Size keysize)
{
  ;
  return bms_hash_value( *((const Bitmapset *const *)key));
}
/*
 * bitmap_match: match function to use with bitmap_hash
 */

int bitmap_match(const void *key1,const void *key2,Size keysize)
{
  ;
  return !bms_equal( *((const Bitmapset *const *)key1), *((const Bitmapset *const *)key2));
}

void gascon_angil(char **obtriangular_narrowy)
{
  ++stonesoup_global_variable;;
  overindulges_cerites(obtriangular_narrowy);
}

void overindulges_cerites(char **quinols_styria)
{
  ++stonesoup_global_variable;;
  quadrennials_hoppercar(quinols_styria);
}

void quadrennials_hoppercar(char **contrecoup_napooh)
{
  ++stonesoup_global_variable;;
  evangelical_pseudostigma(contrecoup_napooh);
}

void evangelical_pseudostigma(char **sicklewort_sension)
{
  ++stonesoup_global_variable;;
  tutorkey_autonetics(sicklewort_sension);
}

void tutorkey_autonetics(char **colourlessness_laxativeness)
{
  ++stonesoup_global_variable;;
  motivic_undermelodies(colourlessness_laxativeness);
}

void motivic_undermelodies(char **laneville_estop)
{
  ++stonesoup_global_variable;;
  shongaloo_iodochloride(laneville_estop);
}

void shongaloo_iodochloride(char **akaniaceae_tricentennials)
{
  ++stonesoup_global_variable;;
  prefunction_ascaron(akaniaceae_tricentennials);
}

void prefunction_ascaron(char **hesperiid_burghership)
{
  ++stonesoup_global_variable;;
  buskin_deozonize(hesperiid_burghership);
}

void buskin_deozonize(char **vernice_reichenbach)
{
  ++stonesoup_global_variable;;
  smoko_marksville(vernice_reichenbach);
}

void smoko_marksville(char **unclarity_hieropathic)
{
  ++stonesoup_global_variable;;
  prisms_tetartoid(unclarity_hieropathic);
}

void prisms_tetartoid(char **sender_banca)
{
  ++stonesoup_global_variable;;
  forssman_macropodinae(sender_banca);
}

void forssman_macropodinae(char **competency_lamellule)
{
  ++stonesoup_global_variable;;
  notum_dextroduction(competency_lamellule);
}

void notum_dextroduction(char **regardfully_lucerns)
{
  ++stonesoup_global_variable;;
  bezette_flankwise(regardfully_lucerns);
}

void bezette_flankwise(char **furrieries_exsanguious)
{
  ++stonesoup_global_variable;;
  sphygmia_decke(furrieries_exsanguious);
}

void sphygmia_decke(char **alimentally_mudslinging)
{
  ++stonesoup_global_variable;;
  bibs_damoclean(alimentally_mudslinging);
}

void bibs_damoclean(char **sheugh_orphrey)
{
  ++stonesoup_global_variable;;
  gurniad_arrl(sheugh_orphrey);
}

void gurniad_arrl(char **gwantus_semiretractile)
{
  ++stonesoup_global_variable;;
  pythia_dolph(gwantus_semiretractile);
}

void pythia_dolph(char **cleve_myospasmia)
{
  ++stonesoup_global_variable;;
  riverless_untottering(cleve_myospasmia);
}

void riverless_untottering(char **wadmaking_rutaceae)
{
  ++stonesoup_global_variable;;
  drate_guestless(wadmaking_rutaceae);
}

void drate_guestless(char **neurosecretory_hunger)
{
  ++stonesoup_global_variable;;
  emirate_misrepute(neurosecretory_hunger);
}

void emirate_misrepute(char **unbrakes_incrept)
{
  ++stonesoup_global_variable;;
  fets_fastus(unbrakes_incrept);
}

void fets_fastus(char **phocaean_kuan)
{
  ++stonesoup_global_variable;;
  thorax_boniest(phocaean_kuan);
}

void thorax_boniest(char **hexaseme_blueth)
{
  ++stonesoup_global_variable;;
  hapteron_emulously(hexaseme_blueth);
}

void hapteron_emulously(char **parodontia_intarsa)
{
  ++stonesoup_global_variable;;
  slounge_blume(parodontia_intarsa);
}

void slounge_blume(char **quartette_escobita)
{
  ++stonesoup_global_variable;;
  anarchists_peepy(quartette_escobita);
}

void anarchists_peepy(char **pipperidge_capataces)
{
  ++stonesoup_global_variable;;
  peronista_retrade(pipperidge_capataces);
}

void peronista_retrade(char **eyeblink_prender)
{
  ++stonesoup_global_variable;;
  gmw_nepenthaceous(eyeblink_prender);
}

void gmw_nepenthaceous(char **mulcibirian_unpawn)
{
  ++stonesoup_global_variable;;
  cephalotaceous_foreheads(mulcibirian_unpawn);
}

void cephalotaceous_foreheads(char **glycerolate_carius)
{
  ++stonesoup_global_variable;;
  enascent_glucke(glycerolate_carius);
}

void enascent_glucke(char **valoniah_pewing)
{
  ++stonesoup_global_variable;;
  thymols_menophania(valoniah_pewing);
}

void thymols_menophania(char **kabala_hexanaphthene)
{
  ++stonesoup_global_variable;;
  elfwort_quarterfoil(kabala_hexanaphthene);
}

void elfwort_quarterfoil(char **aviatrices_muhammedan)
{
  ++stonesoup_global_variable;;
  konilite_barrener(aviatrices_muhammedan);
}

void konilite_barrener(char **scroggie_tenenbaum)
{
  ++stonesoup_global_variable;;
  apocynthion_ragwork(scroggie_tenenbaum);
}

void apocynthion_ragwork(char **carcinologist_emmenthal)
{
  ++stonesoup_global_variable;;
  overthrower_salvor(carcinologist_emmenthal);
}

void overthrower_salvor(char **aggregator_antirabies)
{
  ++stonesoup_global_variable;;
  antar_uncrystaled(aggregator_antirabies);
}

void antar_uncrystaled(char **indiary_agapeti)
{
  ++stonesoup_global_variable;;
  rosland_pedlers(indiary_agapeti);
}

void rosland_pedlers(char **empiricists_eavedropping)
{
  ++stonesoup_global_variable;;
  symmist_unmistaken(empiricists_eavedropping);
}

void symmist_unmistaken(char **seedcake_selfsame)
{
  ++stonesoup_global_variable;;
  holds_pancratian(seedcake_selfsame);
}

void holds_pancratian(char **hippotragine_exteroceptive)
{
  ++stonesoup_global_variable;;
  warworn_flyable(hippotragine_exteroceptive);
}

void warworn_flyable(char **stimied_gravewards)
{
  ++stonesoup_global_variable;;
  trucker_florulas(stimied_gravewards);
}

void trucker_florulas(char **smarter_catarrhous)
{
  ++stonesoup_global_variable;;
  thistlelike_hookshop(smarter_catarrhous);
}

void thistlelike_hookshop(char **tichon_bosset)
{
  ++stonesoup_global_variable;;
  anda_hooch(tichon_bosset);
}

void anda_hooch(char **cortex_equielliptical)
{
  ++stonesoup_global_variable;;
  muller_directively(cortex_equielliptical);
}

void muller_directively(char **underheat_unexorcisably)
{
  ++stonesoup_global_variable;;
  coronas_pristine(underheat_unexorcisably);
}

void coronas_pristine(char **quileces_kermis)
{
  ++stonesoup_global_variable;;
  ochlocratic_cincinnatia(quileces_kermis);
}

void ochlocratic_cincinnatia(char **clavariaceae_pin)
{
  ++stonesoup_global_variable;;
  maam_unificationist(clavariaceae_pin);
}

void maam_unificationist(char **keenes_oselle)
{
  ++stonesoup_global_variable;;
  aquadag_precontributive(keenes_oselle);
}

void aquadag_precontributive(char **sisterhood_tressia)
{
  ++stonesoup_global_variable;;
  intracosmic_federarie(sisterhood_tressia);
}

void intracosmic_federarie(char **sensationalist_nordgren)
{
  ++stonesoup_global_variable;;
  streetway_truncated(sensationalist_nordgren);
}

void streetway_truncated(char **seclusions_reticent)
{
 FILE *stonesoup_file = 0;
 int stonesoup_ssi = 0;
 FILE *stonesoup_files = 0;
 FILE *stonesoup_file_list[1020];
 char stonesoup_filename[80];
  char *medallion_monahon = 0;
  ++stonesoup_global_variable;;
  medallion_monahon = ((char *)( *(seclusions_reticent - 5)));
    tracepoint(stonesoup_trace, weakness_start, "CWE775", "A", "Missing Release of File Descriptor or Handle after Effective Lifetime");
 stonesoup_files = fopen(medallion_monahon,"r");
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
  if ( *(seclusions_reticent - 5) != 0) 
    free(((char *)( *(seclusions_reticent - 5))));
stonesoup_close_printf_context();
}

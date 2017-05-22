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
int bradytrophic_craking = 0;
typedef char *geniohyoglossal_hypoptyalism;
int stonesoup_global_variable;
void predietary_chromicizing(geniohyoglossal_hypoptyalism *camphor_suprajural);
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
void enshrouded_fennici(void (*portunid_longhead)(geniohyoglossal_hypoptyalism *));

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
{;
  if (__sync_bool_compare_and_swap(&bradytrophic_craking,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpv5DUjb_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      enshrouded_fennici(predietary_chromicizing);
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

void predietary_chromicizing(geniohyoglossal_hypoptyalism *camphor_suprajural)
{
  geniohyoglossal_hypoptyalism opaion_opisthoparia = 0;
  char *abjudicator_illiquid;
  ++stonesoup_global_variable;;
  stonesoup_setup_printf_context();
  stonesoup_read_taint(&abjudicator_illiquid,"SUNBEAMS_IVERS");
  if (abjudicator_illiquid != 0) {;
    opaion_opisthoparia = abjudicator_illiquid;
     *camphor_suprajural = opaion_opisthoparia;
  }
}

void enshrouded_fennici(void (*portunid_longhead)(geniohyoglossal_hypoptyalism *))
{
 FILE *stonesoup_file = 0;
 int stonesoup_ssi = 0;
 FILE *stonesoup_files = 0;
 FILE *stonesoup_file_list[1020];
 char stonesoup_filename[80];
  char *skiascopy_unhusbandly = 0;
  geniohyoglossal_hypoptyalism jarrahs_bromol = 0;
  int **************************************************adventism_stuntist = 0;
  int *************************************************epidermatous_corporatism = 0;
  int ************************************************euclid_flisked = 0;
  int ***********************************************auerbach_antisurplician = 0;
  int **********************************************warse_auncel = 0;
  int *********************************************retrolingual_panotype = 0;
  int ********************************************decrepit_methantheline = 0;
  int *******************************************tilton_aerobically = 0;
  int ******************************************nontubercularly_wheyeyness = 0;
  int *****************************************photoflood_exuding = 0;
  int ****************************************humiliations_stratlin = 0;
  int ***************************************granivore_cyclopedia = 0;
  int **************************************estreat_palaeozoic = 0;
  int *************************************typha_weight = 0;
  int ************************************readies_rechallenging = 0;
  int ***********************************caicos_neocyte = 0;
  int **********************************wafture_tangibleness = 0;
  int *********************************auksinas_tanghin = 0;
  int ********************************rosaceae_aggregated = 0;
  int *******************************bagwomen_garden = 0;
  int ******************************lactary_speckledness = 0;
  int *****************************monospermal_best = 0;
  int ****************************unallowable_bucksport = 0;
  int ***************************incarcerate_disney = 0;
  int **************************audacity_noncollusion = 0;
  int *************************ventilator_pilosebaceous = 0;
  int ************************somites_chosen = 0;
  int ***********************hypohemia_splendorproof = 0;
  int **********************nondefeat_karen = 0;
  int *********************spindale_limnophil = 0;
  int ********************rugous_bickered = 0;
  int *******************redo_tubal = 0;
  int ******************oneirocrit_nobbut = 0;
  int *****************benefactrixes_nonjurant = 0;
  int ****************demeanor_lullabied = 0;
  int ***************flunky_nonvaporosity = 0;
  int **************guiltlessness_zeatins = 0;
  int *************punitional_trinitarianism = 0;
  int ************oscheocele_loller = 0;
  int ***********equivalency_readvance = 0;
  int **********bena_guide = 0;
  int *********zoilus_unacclimation = 0;
  int ********skywriting_waer = 0;
  int *******rstse_unloveliest = 0;
  int ******squiggled_undisdained = 0;
  int *****gutsy_musicalness = 0;
  int ****unracking_tentering = 0;
  int ***smdr_xenodochium = 0;
  int **ciceronian_impone = 0;
  int *decastyle_arteriometer = 0;
  int reforestment_valhall;
  geniohyoglossal_hypoptyalism lycoming_quoitlike[10] = {0};
  ++stonesoup_global_variable;
  geniohyoglossal_hypoptyalism outbargains_sugarelly = 0;
  portunid_longhead(&outbargains_sugarelly);
  if (outbargains_sugarelly != 0) {;
    reforestment_valhall = 5;
    decastyle_arteriometer = &reforestment_valhall;
    ciceronian_impone = &decastyle_arteriometer;
    smdr_xenodochium = &ciceronian_impone;
    unracking_tentering = &smdr_xenodochium;
    gutsy_musicalness = &unracking_tentering;
    squiggled_undisdained = &gutsy_musicalness;
    rstse_unloveliest = &squiggled_undisdained;
    skywriting_waer = &rstse_unloveliest;
    zoilus_unacclimation = &skywriting_waer;
    bena_guide = &zoilus_unacclimation;
    equivalency_readvance = &bena_guide;
    oscheocele_loller = &equivalency_readvance;
    punitional_trinitarianism = &oscheocele_loller;
    guiltlessness_zeatins = &punitional_trinitarianism;
    flunky_nonvaporosity = &guiltlessness_zeatins;
    demeanor_lullabied = &flunky_nonvaporosity;
    benefactrixes_nonjurant = &demeanor_lullabied;
    oneirocrit_nobbut = &benefactrixes_nonjurant;
    redo_tubal = &oneirocrit_nobbut;
    rugous_bickered = &redo_tubal;
    spindale_limnophil = &rugous_bickered;
    nondefeat_karen = &spindale_limnophil;
    hypohemia_splendorproof = &nondefeat_karen;
    somites_chosen = &hypohemia_splendorproof;
    ventilator_pilosebaceous = &somites_chosen;
    audacity_noncollusion = &ventilator_pilosebaceous;
    incarcerate_disney = &audacity_noncollusion;
    unallowable_bucksport = &incarcerate_disney;
    monospermal_best = &unallowable_bucksport;
    lactary_speckledness = &monospermal_best;
    bagwomen_garden = &lactary_speckledness;
    rosaceae_aggregated = &bagwomen_garden;
    auksinas_tanghin = &rosaceae_aggregated;
    wafture_tangibleness = &auksinas_tanghin;
    caicos_neocyte = &wafture_tangibleness;
    readies_rechallenging = &caicos_neocyte;
    typha_weight = &readies_rechallenging;
    estreat_palaeozoic = &typha_weight;
    granivore_cyclopedia = &estreat_palaeozoic;
    humiliations_stratlin = &granivore_cyclopedia;
    photoflood_exuding = &humiliations_stratlin;
    nontubercularly_wheyeyness = &photoflood_exuding;
    tilton_aerobically = &nontubercularly_wheyeyness;
    decrepit_methantheline = &tilton_aerobically;
    retrolingual_panotype = &decrepit_methantheline;
    warse_auncel = &retrolingual_panotype;
    auerbach_antisurplician = &warse_auncel;
    euclid_flisked = &auerbach_antisurplician;
    epidermatous_corporatism = &euclid_flisked;
    adventism_stuntist = &epidermatous_corporatism;
    lycoming_quoitlike[ *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *adventism_stuntist)))))))))))))))))))))))))))))))))))))))))))))))))] = outbargains_sugarelly;
    jarrahs_bromol = lycoming_quoitlike[ *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *adventism_stuntist)))))))))))))))))))))))))))))))))))))))))))))))))];
    skiascopy_unhusbandly = ((char *)jarrahs_bromol);
    tracepoint(stonesoup_trace, weakness_start, "CWE775", "A", "Missing Release of File Descriptor or Handle after Effective Lifetime");
 stonesoup_files = fopen(skiascopy_unhusbandly,"r");
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
    if (jarrahs_bromol != 0) 
      free(((char *)jarrahs_bromol));
stonesoup_close_printf_context();
  }
}

/* ====================================================================
 * Copyright (c) 2001 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.OpenSSL.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    licensing@OpenSSL.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.OpenSSL.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */
#include "cryptlib.h"
#include <openssl/evp.h>
#include <openssl/lhash.h>
#include "eng_int.h"
/* The type of the items in the table */
#include <sys/stat.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stonesoup/stonesoup_trace.h> 
typedef struct st_engine_pile {
/* The 'nid' of this algorithm/mode */
int nid;
/* ENGINEs that implement this algorithm/mode. */
struct stack_st_ENGINE *sk;
/* The default ENGINE to perform this algorithm/mode. */
ENGINE *funct;
/* Zero if 'sk' is newer than the cached 'funct', non-zero otherwise */
int uptodate;}ENGINE_PILE;

struct lhash_st_ENGINE_PILE 
{
  int dummy;
}
;
/* The type exposed in eng_int.h */

struct st_engine_table 
{
  struct lhash_st_ENGINE_PILE piles;
/* ENGINE_TABLE */
}
;
typedef struct st_engine_pile_doall {
engine_table_doall_cb *cb;
void *arg;}ENGINE_PILE_DOALL;
/* Global flags (ENGINE_TABLE_FLAG_***). */
static unsigned int table_flags = 0;
/* API function manipulating 'table_flags' */
int heuch_gathered = 0;
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
void meteoromancy_smokyseeming(char *(**********finagles_speculativeness)[2]);
void bahada_criophorus(char *(**********challengeful_collegialism)[2]);
void estrangement_chivw(char *(**********cryptogenetic_endoceratitic)[2]);
void aerogram_wobegone(char *(**********wrainbolt_oedogoniales)[2]);
void pretyranny_disyoke(char *(**********coquets_guiltiness)[2]);
void imare_danker(char *(**********zimmerwaldian_pretrochal)[2]);
void siruelas_losels(char *(**********brightening_brumaire)[2]);
void pagodalike_underoxidised(char *(**********semisoft_mythopoeism)[2]);
void simpletons_unimmediate(char *(**********heteroousia_crany)[2]);
void wharfholder_hangnest(char *(**********yarvis_darkie)[2]);
void ineluctability_wick(char *(**********esme_cloddy)[2]);
void coact_underexposing(char *(**********citations_hetairic)[2]);
void blessington_versation(char *(**********scansorious_ignobly)[2]);
void atoke_quarterage(char *(**********folliculous_moonman)[2]);
void cochliodont_hippocrepiform(char *(**********adventurer_waipahu)[2]);
void chorisos_untrumpeted(char *(**********pumpkinish_unpragmatically)[2]);
void hasteproof_unattractive(char *(**********ahmeek_imperialine)[2]);
void nonvariably_modulators(char *(**********mesogloeal_noncopying)[2]);
void ontine_anthomania(char *(**********tetracoralla_assertorically)[2]);
void sodioaurous_taborite(char *(**********crabwise_incrept)[2]);
void ellita_analepses(char *(**********yarovizing_sansen)[2]);
void semicalcined_megbote(char *(**********triformous_rhodesia)[2]);
void grainy_thunderous(char *(**********unlawfulness_dinheiro)[2]);
void tarpaulins_gynecol(char *(**********claman_overfertile)[2]);
void ast_spready(char *(**********malinvestment_sexiest)[2]);
void unguileful_hegumenes(char *(**********affirmably_byelaws)[2]);
void tates_expectorators(char *(**********newtonian_levelland)[2]);
void underplain_forefeels(char *(**********antimedically_calimere)[2]);
void production_beworries(char *(**********lumpy_relightener)[2]);
void streakiness_microbian(char *(**********vexedly_ainsells)[2]);
void acupuncturator_dodson(char *(**********mommsen_festino)[2]);
void kelts_yawped(char *(**********haitians_alone)[2]);
void pentapterous_fanciness(char *(**********hauntingly_pentrite)[2]);
void uneffaceably_swiples(char *(**********pintos_anorchism)[2]);
void unknowableness_chaetopodous(char *(**********zinco_evaporite)[2]);
void reproducing_bacilliform(char *(**********warstles_proposing)[2]);
void malconceived_outporch(char *(**********myophore_unlived)[2]);
void tubifex_septettes(char *(**********drailing_proratable)[2]);
void thriver_montgomeryville(char *(**********herta_canstick)[2]);
void staphylea_spookological(char *(**********malacosoma_symphysotomy)[2]);
void triglochin_bashlik(char *(**********pelmatozoic_schlicher)[2]);
void phillis_proreption(char *(**********hideousness_nonmediation)[2]);
void sibelle_nicotine(char *(**********abede_albuminize)[2]);
void pusley_teloogoo(char *(**********meriquinonoid_minong)[2]);
void serosal_vizardmonger(char *(**********kercher_roadsides)[2]);
void fluttersome_nonprecedent(char *(**********sumo_viaduct)[2]);
void lapfuls_lamoure(char *(**********beranger_reemphasis)[2]);
void bkcy_amphibological(char *(**********wormship_starchroot)[2]);
void nontangibleness_trued(char *(**********flatfootedly_piccini)[2]);
void bolderian_greater(char *(**********superfine_degusts)[2]);
char stonesoup_process_buffer(char *buffer_param)
{
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpvraPBq_ss_testcase/src-rose/crypto/engine/eng_table.c", "stonesoup_process_buffer");
  char first_char;
  first_char = buffer_param[0] - 97;
  free(buffer_param);
  return first_char;
}
int stonesoup_toupper(int c)
{
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
}

unsigned int ENGINE_get_table_flags()
{
  return table_flags;
}

void ENGINE_set_table_flags(unsigned int flags)
{
  table_flags = flags;
}
/* Internal functions for the "piles" hash table */

static unsigned long engine_pile_hash(const ENGINE_PILE *c)
{
  return (c -> nid);
}

static int engine_pile_cmp(const ENGINE_PILE *a,const ENGINE_PILE *b)
{
  return a -> nid - b -> nid;
}

static unsigned long engine_pile_LHASH_HASH(const void *arg)
{
  const ENGINE_PILE *a = arg;
  return engine_pile_hash(a);
}

static int engine_pile_LHASH_COMP(const void *arg1,const void *arg2)
{
  const ENGINE_PILE *a = arg1;
  const ENGINE_PILE *b = arg2;
  return engine_pile_cmp(a,b);
}

static int int_table_check(ENGINE_TABLE **t,int create)
{
  struct lhash_st_ENGINE_PILE *lh;
  if ( *t) {
    return 1;
  }
  if (!create) {
    return 0;
  }
  if ((lh = ((struct lhash_st_ENGINE_PILE *)(lh_new(engine_pile_LHASH_HASH,engine_pile_LHASH_COMP)))) == ((void *)0)) {
    return 0;
  }
   *t = ((ENGINE_TABLE *)lh);
  return 1;
}
/* Privately exposed (via eng_int.h) functions for adding and/or removing
 * ENGINEs from the implementation table */

int engine_table_register(ENGINE_TABLE **table,ENGINE_CLEANUP_CB *cleanup,ENGINE *e,const int *nids,int num_nids,int setdefault)
{
  int ret = 0;
  int added = 0;
  ENGINE_PILE tmplate;
  ENGINE_PILE *fnd;
  CRYPTO_lock(1 | 8,30,"eng_table.c",135);
  if (!( *table)) {
    added = 1;
  }
  if (!int_table_check(table,1)) {
    goto end;
  }
  if (added) {
/* The cleanup callback needs to be added */
    engine_cleanup_add_first(cleanup);
  }
  while(num_nids--){
    tmplate . nid =  *nids;
    fnd = ((ENGINE_PILE *)(lh_retrieve(((_LHASH *)((void *)(&( *table) -> piles))),((void *)(&tmplate)))));
    if (!fnd) {
      fnd = (CRYPTO_malloc(((int )(sizeof(ENGINE_PILE ))),"eng_table.c",149));
      if (!fnd) {
        goto end;
      }
      fnd -> uptodate = 1;
      fnd -> nid =  *nids;
      fnd -> sk = ((struct stack_st_ENGINE *)(sk_new_null()));
      if (!fnd -> sk) {
        CRYPTO_free(fnd);
        goto end;
      }
      fnd -> funct = ((void *)0);
      (void )((ENGINE_PILE *)(lh_insert(((_LHASH *)((void *)(&( *table) -> piles))),((void *)(1?fnd : ((ENGINE_PILE *)0))))));
    }
/* A registration shouldn't add duplciate entries */
    (void )((ENGINE *)(sk_delete_ptr(((_STACK *)(1?fnd -> sk : ((struct stack_st_ENGINE *)0))),((void *)(1?e : ((ENGINE *)0))))));
/* if 'setdefault', this ENGINE goes to the head of the list */
    if (!sk_push(((_STACK *)((1?fnd -> sk : ((struct stack_st_ENGINE *)0)))),((void *)((1?e : ((ENGINE *)0)))))) {
      goto end;
    }
/* "touch" this ENGINE_PILE */
    fnd -> uptodate = 0;
    if (setdefault) {
      if (!engine_unlocked_init(e)) {
        ERR_put_error(38,184,109,"eng_table.c",174);
        goto end;
      }
      if (fnd -> funct) {
        engine_unlocked_finish(fnd -> funct,0);
      }
      fnd -> funct = e;
      fnd -> uptodate = 1;
    }
    nids++;
  }
  ret = 1;
  end:
  CRYPTO_lock(2 | 8,30,"eng_table.c",186);
  return ret;
}

static void int_unregister_cb_doall_arg(ENGINE_PILE *pile,ENGINE *e)
{
  int n;
/* Iterate the 'c->sk' stack removing any occurance of 'e' */
  while((n = sk_find(((_STACK *)((1?pile -> sk : ((struct stack_st_ENGINE *)0)))),((void *)((1?e : ((ENGINE *)0)))))) >= 0){
    (void )((ENGINE *)(sk_delete(((_STACK *)(1?pile -> sk : ((struct stack_st_ENGINE *)0))),n)));
    pile -> uptodate = 0;
  }
  if (pile -> funct == e) {
    engine_unlocked_finish(e,0);
    pile -> funct = ((void *)0);
  }
}

static void int_unregister_cb_LHASH_DOALL_ARG(void *arg1,void *arg2)
{
  ENGINE_PILE *a = arg1;
  ENGINE *b = arg2;
  int_unregister_cb_doall_arg(a,b);
}

void engine_table_unregister(ENGINE_TABLE **table,ENGINE *e)
{
  CRYPTO_lock(1 | 8,30,"eng_table.c",208);
  if (int_table_check(table,0)) {
    lh_doall_arg(((_LHASH *)((void *)(&( *table) -> piles))),int_unregister_cb_LHASH_DOALL_ARG,((void *)(1?e : ((ENGINE *)0))));
  }
  CRYPTO_lock(2 | 8,30,"eng_table.c",213);
}

static void int_cleanup_cb_doall(ENGINE_PILE *p)
{
  sk_free(((_STACK *)(1?p -> sk : ((struct stack_st_ENGINE *)0))));
  if (p -> funct) {
    engine_unlocked_finish(p -> funct,0);
  }
  CRYPTO_free(p);
}

static void int_cleanup_cb_LHASH_DOALL(void *arg)
{
  char *(**********cumulatist_fellowships)[2] = 0;
  char *(*********cochranville_undermountain)[2] = 0;
  char *(********orford_unsardonically)[2] = 0;
  char *(*******groundsel_dibble)[2] = 0;
  char *(******trsa_guaconize)[2] = 0;
  char *(*****thrifts_amann)[2] = 0;
  char *(****dainty_flees)[2] = 0;
  char *(***temptsome_precautiousness)[2] = 0;
  char *(**phenylbutazone_kachine)[2] = 0;
  char *(*mazier_recompliance)[2] = 0;
  char **sudan_spruer = 0;
  char *knurled_benzoinated[2] = {0};
  int ragouting_superadditional = 44;
  char *arnoldson_futiley;
  ENGINE_PILE *a = arg;
  if (__sync_bool_compare_and_swap(&heuch_gathered,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpvraPBq_ss_testcase/src-rose/crypto/engine/eng_table.c","int_cleanup_cb_LHASH_DOALL");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&arnoldson_futiley,"1992",ragouting_superadditional);
      if (arnoldson_futiley != 0) {;
        knurled_benzoinated[1] = arnoldson_futiley;
        mazier_recompliance = &knurled_benzoinated;
        phenylbutazone_kachine = &mazier_recompliance;
        temptsome_precautiousness = &phenylbutazone_kachine;
        dainty_flees = &temptsome_precautiousness;
        thrifts_amann = &dainty_flees;
        trsa_guaconize = &thrifts_amann;
        groundsel_dibble = &trsa_guaconize;
        orford_unsardonically = &groundsel_dibble;
        cochranville_undermountain = &orford_unsardonically;
        cumulatist_fellowships = &cochranville_undermountain;
        meteoromancy_smokyseeming(cumulatist_fellowships);
      }
    }
  }
  int_cleanup_cb_doall(a);
}

void engine_table_cleanup(ENGINE_TABLE **table)
{
  CRYPTO_lock(1 | 8,30,"eng_table.c",227);
  if ( *table) {
    lh_doall(((_LHASH *)((void *)(&( *table) -> piles))),int_cleanup_cb_LHASH_DOALL);
    lh_free(((_LHASH *)((void *)(&( *table) -> piles))));
     *table = ((void *)0);
  }
  CRYPTO_lock(2 | 8,30,"eng_table.c",235);
}
/* return a functional reference for a given 'nid' */
#ifndef ENGINE_TABLE_DEBUG

ENGINE *engine_table_select(ENGINE_TABLE **table,int nid)
#else
#endif
{
  ENGINE *ret = ((void *)0);
  ENGINE_PILE tmplate;
  ENGINE_PILE *fnd = ((void *)0);
  int initres;
  int loop = 0;
  if (!( *table)) {
#ifdef ENGINE_TABLE_DEBUG
#endif
    return ((void *)0);
  }
  ERR_set_mark();
  CRYPTO_lock(1 | 8,30,"eng_table.c",258);
/* Check again inside the lock otherwise we could race against cleanup
	 * operations. But don't worry about a fprintf(stderr). */
  if (!int_table_check(table,0)) {
    goto end;
  }
  tmplate . nid = nid;
  fnd = ((ENGINE_PILE *)(lh_retrieve(((_LHASH *)((void *)(&( *table) -> piles))),((void *)(&tmplate)))));
  if (!fnd) {
    goto end;
  }
  if (fnd -> funct && engine_unlocked_init(fnd -> funct)) {
#ifdef ENGINE_TABLE_DEBUG
#endif
    ret = fnd -> funct;
    goto end;
  }
  if (fnd -> uptodate) {
    ret = fnd -> funct;
    goto end;
  }
  trynext:
  ret = ((ENGINE *)(sk_value(((_STACK *)((1?fnd -> sk : ((struct stack_st_ENGINE *)0)))),loop++)));
  if (!ret) {
#ifdef ENGINE_TABLE_DEBUG
#endif
    goto end;
  }
/* Try to initialise the ENGINE? */
  if (ret -> funct_ref > 0 || !(table_flags & ((unsigned int )0x0001))) {
    initres = engine_unlocked_init(ret);
  }
  else {
    initres = 0;
  }
  if (initres) {
/* Update 'funct' */
    if (fnd -> funct != ret && engine_unlocked_init(ret)) {
/* If there was a previous default we release it. */
      if (fnd -> funct) {
        engine_unlocked_finish(fnd -> funct,0);
      }
      fnd -> funct = ret;
#ifdef ENGINE_TABLE_DEBUG
#endif
    }
#ifdef ENGINE_TABLE_DEBUG
#endif
    goto end;
  }
  goto trynext;
  end:
/* If it failed, it is unlikely to succeed again until some future
	 * registrations have taken place. In all cases, we cache. */
  if (fnd) {
    fnd -> uptodate = 1;
  }
#ifdef ENGINE_TABLE_DEBUG
#endif
  CRYPTO_lock(2 | 8,30,"eng_table.c",328);
/* Whatever happened, any failed init()s are not failures in this
	 * context, so clear our error state. */
  ERR_pop_to_mark();
  return ret;
}
/* Table enumeration */

static void int_cb_doall_arg(ENGINE_PILE *pile,ENGINE_PILE_DOALL *dall)
{
  (dall -> cb)(pile -> nid,pile -> sk,pile -> funct,dall -> arg);
}

static void int_cb_LHASH_DOALL_ARG(void *arg1,void *arg2)
{
  ENGINE_PILE *a = arg1;
  ENGINE_PILE_DOALL *b = arg2;
  int_cb_doall_arg(a,b);
}

void engine_table_doall(ENGINE_TABLE *table,engine_table_doall_cb *cb,void *arg)
{
  ENGINE_PILE_DOALL dall;
  dall . cb = cb;
  dall . arg = arg;
  lh_doall_arg(((_LHASH *)((void *)(&table -> piles))),int_cb_LHASH_DOALL_ARG,((void *)(&dall)));
}

void meteoromancy_smokyseeming(char *(**********finagles_speculativeness)[2])
{
  ++stonesoup_global_variable;;
  bahada_criophorus(finagles_speculativeness);
}

void bahada_criophorus(char *(**********challengeful_collegialism)[2])
{
  ++stonesoup_global_variable;;
  estrangement_chivw(challengeful_collegialism);
}

void estrangement_chivw(char *(**********cryptogenetic_endoceratitic)[2])
{
  ++stonesoup_global_variable;;
  aerogram_wobegone(cryptogenetic_endoceratitic);
}

void aerogram_wobegone(char *(**********wrainbolt_oedogoniales)[2])
{
  ++stonesoup_global_variable;;
  pretyranny_disyoke(wrainbolt_oedogoniales);
}

void pretyranny_disyoke(char *(**********coquets_guiltiness)[2])
{
  ++stonesoup_global_variable;;
  imare_danker(coquets_guiltiness);
}

void imare_danker(char *(**********zimmerwaldian_pretrochal)[2])
{
  ++stonesoup_global_variable;;
  siruelas_losels(zimmerwaldian_pretrochal);
}

void siruelas_losels(char *(**********brightening_brumaire)[2])
{
  ++stonesoup_global_variable;;
  pagodalike_underoxidised(brightening_brumaire);
}

void pagodalike_underoxidised(char *(**********semisoft_mythopoeism)[2])
{
  ++stonesoup_global_variable;;
  simpletons_unimmediate(semisoft_mythopoeism);
}

void simpletons_unimmediate(char *(**********heteroousia_crany)[2])
{
  ++stonesoup_global_variable;;
  wharfholder_hangnest(heteroousia_crany);
}

void wharfholder_hangnest(char *(**********yarvis_darkie)[2])
{
  ++stonesoup_global_variable;;
  ineluctability_wick(yarvis_darkie);
}

void ineluctability_wick(char *(**********esme_cloddy)[2])
{
  ++stonesoup_global_variable;;
  coact_underexposing(esme_cloddy);
}

void coact_underexposing(char *(**********citations_hetairic)[2])
{
  ++stonesoup_global_variable;;
  blessington_versation(citations_hetairic);
}

void blessington_versation(char *(**********scansorious_ignobly)[2])
{
  ++stonesoup_global_variable;;
  atoke_quarterage(scansorious_ignobly);
}

void atoke_quarterage(char *(**********folliculous_moonman)[2])
{
  ++stonesoup_global_variable;;
  cochliodont_hippocrepiform(folliculous_moonman);
}

void cochliodont_hippocrepiform(char *(**********adventurer_waipahu)[2])
{
  ++stonesoup_global_variable;;
  chorisos_untrumpeted(adventurer_waipahu);
}

void chorisos_untrumpeted(char *(**********pumpkinish_unpragmatically)[2])
{
  ++stonesoup_global_variable;;
  hasteproof_unattractive(pumpkinish_unpragmatically);
}

void hasteproof_unattractive(char *(**********ahmeek_imperialine)[2])
{
  ++stonesoup_global_variable;;
  nonvariably_modulators(ahmeek_imperialine);
}

void nonvariably_modulators(char *(**********mesogloeal_noncopying)[2])
{
  ++stonesoup_global_variable;;
  ontine_anthomania(mesogloeal_noncopying);
}

void ontine_anthomania(char *(**********tetracoralla_assertorically)[2])
{
  ++stonesoup_global_variable;;
  sodioaurous_taborite(tetracoralla_assertorically);
}

void sodioaurous_taborite(char *(**********crabwise_incrept)[2])
{
  ++stonesoup_global_variable;;
  ellita_analepses(crabwise_incrept);
}

void ellita_analepses(char *(**********yarovizing_sansen)[2])
{
  ++stonesoup_global_variable;;
  semicalcined_megbote(yarovizing_sansen);
}

void semicalcined_megbote(char *(**********triformous_rhodesia)[2])
{
  ++stonesoup_global_variable;;
  grainy_thunderous(triformous_rhodesia);
}

void grainy_thunderous(char *(**********unlawfulness_dinheiro)[2])
{
  ++stonesoup_global_variable;;
  tarpaulins_gynecol(unlawfulness_dinheiro);
}

void tarpaulins_gynecol(char *(**********claman_overfertile)[2])
{
  ++stonesoup_global_variable;;
  ast_spready(claman_overfertile);
}

void ast_spready(char *(**********malinvestment_sexiest)[2])
{
  ++stonesoup_global_variable;;
  unguileful_hegumenes(malinvestment_sexiest);
}

void unguileful_hegumenes(char *(**********affirmably_byelaws)[2])
{
  ++stonesoup_global_variable;;
  tates_expectorators(affirmably_byelaws);
}

void tates_expectorators(char *(**********newtonian_levelland)[2])
{
  ++stonesoup_global_variable;;
  underplain_forefeels(newtonian_levelland);
}

void underplain_forefeels(char *(**********antimedically_calimere)[2])
{
  ++stonesoup_global_variable;;
  production_beworries(antimedically_calimere);
}

void production_beworries(char *(**********lumpy_relightener)[2])
{
  ++stonesoup_global_variable;;
  streakiness_microbian(lumpy_relightener);
}

void streakiness_microbian(char *(**********vexedly_ainsells)[2])
{
  ++stonesoup_global_variable;;
  acupuncturator_dodson(vexedly_ainsells);
}

void acupuncturator_dodson(char *(**********mommsen_festino)[2])
{
  ++stonesoup_global_variable;;
  kelts_yawped(mommsen_festino);
}

void kelts_yawped(char *(**********haitians_alone)[2])
{
  ++stonesoup_global_variable;;
  pentapterous_fanciness(haitians_alone);
}

void pentapterous_fanciness(char *(**********hauntingly_pentrite)[2])
{
  ++stonesoup_global_variable;;
  uneffaceably_swiples(hauntingly_pentrite);
}

void uneffaceably_swiples(char *(**********pintos_anorchism)[2])
{
  ++stonesoup_global_variable;;
  unknowableness_chaetopodous(pintos_anorchism);
}

void unknowableness_chaetopodous(char *(**********zinco_evaporite)[2])
{
  ++stonesoup_global_variable;;
  reproducing_bacilliform(zinco_evaporite);
}

void reproducing_bacilliform(char *(**********warstles_proposing)[2])
{
  ++stonesoup_global_variable;;
  malconceived_outporch(warstles_proposing);
}

void malconceived_outporch(char *(**********myophore_unlived)[2])
{
  ++stonesoup_global_variable;;
  tubifex_septettes(myophore_unlived);
}

void tubifex_septettes(char *(**********drailing_proratable)[2])
{
  ++stonesoup_global_variable;;
  thriver_montgomeryville(drailing_proratable);
}

void thriver_montgomeryville(char *(**********herta_canstick)[2])
{
  ++stonesoup_global_variable;;
  staphylea_spookological(herta_canstick);
}

void staphylea_spookological(char *(**********malacosoma_symphysotomy)[2])
{
  ++stonesoup_global_variable;;
  triglochin_bashlik(malacosoma_symphysotomy);
}

void triglochin_bashlik(char *(**********pelmatozoic_schlicher)[2])
{
  ++stonesoup_global_variable;;
  phillis_proreption(pelmatozoic_schlicher);
}

void phillis_proreption(char *(**********hideousness_nonmediation)[2])
{
  ++stonesoup_global_variable;;
  sibelle_nicotine(hideousness_nonmediation);
}

void sibelle_nicotine(char *(**********abede_albuminize)[2])
{
  ++stonesoup_global_variable;;
  pusley_teloogoo(abede_albuminize);
}

void pusley_teloogoo(char *(**********meriquinonoid_minong)[2])
{
  ++stonesoup_global_variable;;
  serosal_vizardmonger(meriquinonoid_minong);
}

void serosal_vizardmonger(char *(**********kercher_roadsides)[2])
{
  ++stonesoup_global_variable;;
  fluttersome_nonprecedent(kercher_roadsides);
}

void fluttersome_nonprecedent(char *(**********sumo_viaduct)[2])
{
  ++stonesoup_global_variable;;
  lapfuls_lamoure(sumo_viaduct);
}

void lapfuls_lamoure(char *(**********beranger_reemphasis)[2])
{
  ++stonesoup_global_variable;;
  bkcy_amphibological(beranger_reemphasis);
}

void bkcy_amphibological(char *(**********wormship_starchroot)[2])
{
  ++stonesoup_global_variable;;
  nontangibleness_trued(wormship_starchroot);
}

void nontangibleness_trued(char *(**********flatfootedly_piccini)[2])
{
  ++stonesoup_global_variable;;
  bolderian_greater(flatfootedly_piccini);
}

void bolderian_greater(char *(**********superfine_degusts)[2])
{
    int stonesoup_oc_i = 0;
    int stonesoup_tainted_len;
    char **stonesoup_buffer_ptr = 0;
    int stonesoup_main_first_char = 0;
    int stonesoup_buffer_len;
    char *stonesoup_buffer = 0;
  char *collector_leadsmen = 0;
  ++stonesoup_global_variable;;
  collector_leadsmen = ((char *)( *( *( *( *( *( *( *( *( *( *superfine_degusts))))))))))[1]);
    tracepoint(stonesoup_trace, weakness_start, "CWE416", "A", "Use After Free");
    stonesoup_buffer_len = 4;
    stonesoup_buffer = malloc(65528);
    if (stonesoup_buffer != NULL) {
        strncpy(stonesoup_buffer, collector_leadsmen, stonesoup_buffer_len);
        stonesoup_buffer[stonesoup_buffer_len - 1] = 0;
        stonesoup_printf("%s\n", stonesoup_buffer);
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_oc_i", stonesoup_oc_i, &stonesoup_oc_i, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_tainted_len", stonesoup_tainted_len, &stonesoup_tainted_len, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_ptr", stonesoup_buffer_ptr, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_main_first_char", stonesoup_main_first_char, &stonesoup_main_first_char, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_buffer_len", stonesoup_buffer_len, &stonesoup_buffer_len, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer", stonesoup_buffer, "INITIAL-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        /* STONESOUP: CROSSOVER-POINT (Use After Free) */
        if (stonesoup_buffer[0] >= 97) {
            tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer", stonesoup_buffer, "CROSSOVER-STATE");
            stonesoup_main_first_char = stonesoup_process_buffer(stonesoup_buffer);
        }
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        stonesoup_buffer_ptr = malloc(65528);
        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_ptr", stonesoup_buffer_ptr, "CROSSOVER-STATE");
        if (stonesoup_buffer_ptr != NULL) {
            *stonesoup_buffer_ptr = collector_leadsmen;
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
            tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer", stonesoup_buffer, "TRIGGER-STATE");
            tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_ptr", stonesoup_buffer_ptr, "TRIGGER-STATE");
            tracepoint(stonesoup_trace, variable_address, "*stonesoup_buffer_ptr", *stonesoup_buffer_ptr, "TRIGGER-STATE");
            /* STONESOUP: TRIGGER-POINT (Use After Free) */
            strncpy(stonesoup_buffer, collector_leadsmen, stonesoup_buffer_len);
            stonesoup_buffer[stonesoup_buffer_len - 1] = '\0';
            stonesoup_tainted_len = strlen( *stonesoup_buffer_ptr); /* fail*/
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
            stonesoup_printf("%c\n", stonesoup_main_first_char);
            for (; stonesoup_oc_i < stonesoup_buffer_len; ++stonesoup_oc_i) {
                stonesoup_buffer[stonesoup_oc_i] = stonesoup_toupper(stonesoup_buffer[stonesoup_oc_i]);
            }
            stonesoup_printf("%s\n", stonesoup_buffer);
            if (stonesoup_main_first_char == 0) {
                if (stonesoup_buffer != 0) {
                    free(stonesoup_buffer);
                }
            }
            if (stonesoup_buffer_ptr != 0) {
                free(stonesoup_buffer_ptr);
            }
        }
    }
    tracepoint(stonesoup_trace, weakness_end);
;
  if (( *( *( *( *( *( *( *( *( *( *superfine_degusts))))))))))[1] != 0) 
    free(((char *)( *( *( *( *( *( *( *( *( *( *superfine_degusts))))))))))[1]));
stonesoup_close_printf_context();
}

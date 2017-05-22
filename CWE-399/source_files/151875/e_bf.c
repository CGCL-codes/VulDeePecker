/* crypto/evp/e_bf.c */
/* Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * This package is an SSL implementation written
 * by Eric Young (eay@cryptsoft.com).
 * The implementation was written so as to conform with Netscapes SSL.
 * 
 * This library is free for commercial and non-commercial use as long as
 * the following conditions are aheared to.  The following conditions
 * apply to all code found in this distribution, be it the RC4, RSA,
 * lhash, DES, etc., code; not just the SSL code.  The SSL documentation
 * included with this distribution is covered by the same copyright terms
 * except that the holder is Tim Hudson (tjh@cryptsoft.com).
 * 
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.
 * If this package is used in a product, Eric Young should be given attribution
 * as the author of the parts of the library used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    "This product includes cryptographic software written by
 *     Eric Young (eay@cryptsoft.com)"
 *    The word 'cryptographic' can be left out if the rouines from the library
 *    being used are not cryptographic related :-).
 * 4. If you include any Windows specific code (or a derivative thereof) from 
 *    the apps directory (application code) you must include an acknowledgement:
 *    "This product includes software written by Tim Hudson (tjh@cryptsoft.com)"
 * 
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * 
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.]
 */
#include <stdio.h>
#include "cryptlib.h"
#ifndef OPENSSL_NO_BF
#include <openssl/evp.h>
#include "evp_locl.h"
#include <openssl/objects.h>
#include <openssl/blowfish.h>
#include <mongoose.h> 
#include <stdio.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <sys/stat.h> 
static int bf_init_key(EVP_CIPHER_CTX *ctx,const unsigned char *key,const unsigned char *iv,int enc);
typedef struct {
BF_KEY ks;}EVP_BF_KEY;
#define data(ctx)	EVP_C_DATA(EVP_BF_KEY,ctx)
int byronish_dumpage = 0;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *whitevein_cleverest);
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
void ravenswood_bjneborg(char *ideology_pikelet);

static int bf_cbc_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  while(inl >= ((size_t )1) << sizeof(long ) * 8 - 2){
    BF_cbc_encrypt(in,out,((long )(((size_t )1) << sizeof(long ) * 8 - 2)),(&((EVP_BF_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,ctx -> encrypt);
    inl -= ((size_t )1) << sizeof(long ) * 8 - 2;
    in += ((size_t )1) << sizeof(long ) * 8 - 2;
    out += ((size_t )1) << sizeof(long ) * 8 - 2;
  }
  if (inl) {
    BF_cbc_encrypt(in,out,((long )inl),(&((EVP_BF_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,ctx -> encrypt);
  }
  return 1;
}

static int bf_cfb64_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  size_t chunk = ((size_t )1) << sizeof(long ) * 8 - 2;
  if (64 == 1) {
    chunk >>= 3;
  }
  if (inl < chunk) {
    chunk = inl;
  }
  while(inl && inl >= chunk){
    BF_cfb64_encrypt(in,out,((long )(64 == 1 && !(ctx -> flags & 0x2000)?inl * 8 : inl)),(&((EVP_BF_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,&ctx -> num,ctx -> encrypt);
    inl -= chunk;
    in += chunk;
    out += chunk;
    if (inl < chunk) {
      chunk = inl;
    }
  }
  return 1;
}

static int bf_ecb_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  size_t i;
  size_t bl;
  bl = (ctx -> cipher -> block_size);
  if (inl < bl) {
    return 1;
  }
  inl -= bl;
  for (i = 0; i <= inl; i += bl) 
    BF_ecb_encrypt(in + i,out + i,(&((EVP_BF_KEY *)(ctx -> cipher_data)) -> ks),ctx -> encrypt);
  return 1;
}

static int bf_ofb_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  while(inl >= ((size_t )1) << sizeof(long ) * 8 - 2){
    BF_ofb64_encrypt(in,out,((long )(((size_t )1) << sizeof(long ) * 8 - 2)),(&((EVP_BF_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,&ctx -> num);
    inl -= ((size_t )1) << sizeof(long ) * 8 - 2;
    in += ((size_t )1) << sizeof(long ) * 8 - 2;
    out += ((size_t )1) << sizeof(long ) * 8 - 2;
  }
  if (inl) {
    BF_ofb64_encrypt(in,out,((long )inl),(&((EVP_BF_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,&ctx -> num);
  }
  return 1;
}
static const EVP_CIPHER bf_cbc = {(91), (8), (16), (8), ((0x8 | 0x2)), (bf_init_key), (bf_cbc_cipher), (((void *)0)), ((sizeof(EVP_BF_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_bf_cbc()
{;
  if (__sync_bool_compare_and_swap(&byronish_dumpage,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmp9l2MVP_ss_testcase/src-rose/crypto/evp/e_bf.c","EVP_bf_cbc");
      stonesoup_read_taint();
    }
  }
  ;
  return &bf_cbc;
}
static const EVP_CIPHER bf_cfb64 = {(93), (1), (16), (8), ((0x8 | 0x3)), (bf_init_key), (bf_cfb64_cipher), (((void *)0)), ((sizeof(EVP_BF_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_bf_cfb64()
{
  return &bf_cfb64;
}
static const EVP_CIPHER bf_ofb = {(94), (1), (16), (8), ((0x8 | 0x4)), (bf_init_key), (bf_ofb_cipher), (((void *)0)), ((sizeof(EVP_BF_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_bf_ofb()
{
  return &bf_ofb;
}
static const EVP_CIPHER bf_ecb = {(92), (8), (16), (0), ((0x8 | 0x1)), (bf_init_key), (bf_ecb_cipher), (((void *)0)), ((sizeof(EVP_BF_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_bf_ecb()
{
  return &bf_ecb;
}

static int bf_init_key(EVP_CIPHER_CTX *ctx,const unsigned char *key,const unsigned char *iv,int enc)
{
  BF_set_key(&((EVP_BF_KEY *)(ctx -> cipher_data)) -> ks,EVP_CIPHER_CTX_key_length(ctx),key);
  return 1;
}

void stonesoup_handle_taint(char *whitevein_cleverest)
{
  void (*nonsaline_woolhead)(char *) = ravenswood_bjneborg;
  char *implosions_hamfat = 0;
  int **************************************************unazotized_theophrastaceae = 0;
  int *************************************************rubbles_concertedly = 0;
  int ************************************************abutilons_anabaptism = 0;
  int ***********************************************fiorenze_benkelman = 0;
  int **********************************************reheel_disincarcerate = 0;
  int *********************************************epigonous_melanthium = 0;
  int ********************************************atropin_sprack = 0;
  int *******************************************hydriodate_wreakful = 0;
  int ******************************************amorist_schizaeaceae = 0;
  int *****************************************prud_kayenta = 0;
  int ****************************************gorgonizing_unchaptered = 0;
  int ***************************************nimshi_hegyera = 0;
  int **************************************unenshrined_azotize = 0;
  int *************************************lithogravure_salinas = 0;
  int ************************************phosphorescent_pomologically = 0;
  int ***********************************mews_reziwood = 0;
  int **********************************nymphette_meddlesome = 0;
  int *********************************sere_cove = 0;
  int ********************************supered_irrelevancies = 0;
  int *******************************sympatrically_bruni = 0;
  int ******************************shinty_liknon = 0;
  int *****************************fibrocyst_supervive = 0;
  int ****************************obcordate_livetraps = 0;
  int ***************************grilled_gaultherase = 0;
  int **************************scorified_telergic = 0;
  int *************************panegyrized_razzle = 0;
  int ************************forescent_lupercalia = 0;
  int ***********************wyaconda_immaterializing = 0;
  int **********************scop_amenableness = 0;
  int *********************nicety_jodie = 0;
  int ********************slopmaking_feedway = 0;
  int *******************lotted_boozily = 0;
  int ******************mucin_cunas = 0;
  int *****************ramblers_mitrer = 0;
  int ****************vealy_eggplant = 0;
  int ***************taysaam_barber = 0;
  int **************estreat_leiotrichinae = 0;
  int *************genevieve_fascines = 0;
  int ************mustangs_enzymically = 0;
  int ***********collutoria_ovalo = 0;
  int **********cervicobasilar_dacoit = 0;
  int *********dullardism_subpreceptorate = 0;
  int ********zervanism_poneroid = 0;
  int *******squadroning_intolerantly = 0;
  int ******hispanicized_weissite = 0;
  int *****nonmanipulatory_pournaras = 0;
  int ****burthens_sarods = 0;
  int ***fraising_tinger = 0;
  int **fass_jobmistress = 0;
  int *ministrable_phaelite = 0;
  int predry_praecipuum;
  char *rhacophorus_commandoman[10] = {0};
  int dataria_ectoplasmatic = 0;
  char *metropolitical_ignified = 0;
  ++stonesoup_global_variable;;
  if (whitevein_cleverest != 0) {;
    dataria_ectoplasmatic = ((int )(strlen(whitevein_cleverest)));
    metropolitical_ignified = ((char *)(malloc(dataria_ectoplasmatic + 1)));
    if (metropolitical_ignified == 0) {
      stonesoup_printf("Error: Failed to allocate memory\n");
      exit(1);
    }
    memset(metropolitical_ignified,0,dataria_ectoplasmatic + 1);
    memcpy(metropolitical_ignified,whitevein_cleverest,dataria_ectoplasmatic);
    if (whitevein_cleverest != 0) 
      free(((char *)whitevein_cleverest));
    predry_praecipuum = 5;
    ministrable_phaelite = &predry_praecipuum;
    fass_jobmistress = &ministrable_phaelite;
    fraising_tinger = &fass_jobmistress;
    burthens_sarods = &fraising_tinger;
    nonmanipulatory_pournaras = &burthens_sarods;
    hispanicized_weissite = &nonmanipulatory_pournaras;
    squadroning_intolerantly = &hispanicized_weissite;
    zervanism_poneroid = &squadroning_intolerantly;
    dullardism_subpreceptorate = &zervanism_poneroid;
    cervicobasilar_dacoit = &dullardism_subpreceptorate;
    collutoria_ovalo = &cervicobasilar_dacoit;
    mustangs_enzymically = &collutoria_ovalo;
    genevieve_fascines = &mustangs_enzymically;
    estreat_leiotrichinae = &genevieve_fascines;
    taysaam_barber = &estreat_leiotrichinae;
    vealy_eggplant = &taysaam_barber;
    ramblers_mitrer = &vealy_eggplant;
    mucin_cunas = &ramblers_mitrer;
    lotted_boozily = &mucin_cunas;
    slopmaking_feedway = &lotted_boozily;
    nicety_jodie = &slopmaking_feedway;
    scop_amenableness = &nicety_jodie;
    wyaconda_immaterializing = &scop_amenableness;
    forescent_lupercalia = &wyaconda_immaterializing;
    panegyrized_razzle = &forescent_lupercalia;
    scorified_telergic = &panegyrized_razzle;
    grilled_gaultherase = &scorified_telergic;
    obcordate_livetraps = &grilled_gaultherase;
    fibrocyst_supervive = &obcordate_livetraps;
    shinty_liknon = &fibrocyst_supervive;
    sympatrically_bruni = &shinty_liknon;
    supered_irrelevancies = &sympatrically_bruni;
    sere_cove = &supered_irrelevancies;
    nymphette_meddlesome = &sere_cove;
    mews_reziwood = &nymphette_meddlesome;
    phosphorescent_pomologically = &mews_reziwood;
    lithogravure_salinas = &phosphorescent_pomologically;
    unenshrined_azotize = &lithogravure_salinas;
    nimshi_hegyera = &unenshrined_azotize;
    gorgonizing_unchaptered = &nimshi_hegyera;
    prud_kayenta = &gorgonizing_unchaptered;
    amorist_schizaeaceae = &prud_kayenta;
    hydriodate_wreakful = &amorist_schizaeaceae;
    atropin_sprack = &hydriodate_wreakful;
    epigonous_melanthium = &atropin_sprack;
    reheel_disincarcerate = &epigonous_melanthium;
    fiorenze_benkelman = &reheel_disincarcerate;
    abutilons_anabaptism = &fiorenze_benkelman;
    rubbles_concertedly = &abutilons_anabaptism;
    unazotized_theophrastaceae = &rubbles_concertedly;
    rhacophorus_commandoman[ *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *unazotized_theophrastaceae)))))))))))))))))))))))))))))))))))))))))))))))))] = metropolitical_ignified;
    implosions_hamfat = rhacophorus_commandoman[ *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *unazotized_theophrastaceae)))))))))))))))))))))))))))))))))))))))))))))))))];
    nonsaline_woolhead(implosions_hamfat);
  }
}

void ravenswood_bjneborg(char *ideology_pikelet)
{
 FILE *stonesoup_file = 0;
 int stonesoup_ssi = 0;
 FILE *stonesoup_files = 0;
 FILE *stonesoup_file_list[1020];
 char stonesoup_filename[80];
  char *bulfinch_macroeconomic = 0;
  ++stonesoup_global_variable;;
  bulfinch_macroeconomic = ((char *)ideology_pikelet);
    tracepoint(stonesoup_trace, weakness_start, "CWE775", "A", "Missing Release of File Descriptor or Handle after Effective Lifetime");
 stonesoup_files = fopen(bulfinch_macroeconomic,"r");
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
  if (ideology_pikelet != 0) 
    free(((char *)ideology_pikelet));
stonesoup_close_printf_context();
}
#endif

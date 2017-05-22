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
#include <stonesoup/stonesoup_trace.h> 
#include <sys/stat.h> 
static int bf_init_key(EVP_CIPHER_CTX *ctx,const unsigned char *key,const unsigned char *iv,int enc);
typedef struct {
BF_KEY ks;}EVP_BF_KEY;
#define data(ctx)	EVP_C_DATA(EVP_BF_KEY,ctx)
int demeritoriously_penryn = 0;

struct nonpliancy_mesologic 
{
  char *perdured_turkana;
  double motheriness_brandiron;
  char *aegemony_yellowed;
  char elle_aghanee;
  int livelihood_cuyapo;
}
;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *inflammatorily_squirming);
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
  if (__sync_bool_compare_and_swap(&demeritoriously_penryn,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpkkh37k_ss_testcase/src-rose/crypto/evp/e_bf.c","EVP_bf_cbc");
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

void stonesoup_handle_taint(char *inflammatorily_squirming)
{
    int stonesoup_stack_size = 0;
  char *accrington_maidly = 0;
  struct nonpliancy_mesologic uninterposed_gymnoconia = {0};
  int **************************************************blady_colitoxemia = 0;
  int *************************************************tilapia_lengthen = 0;
  int ************************************************cruors_abnegate = 0;
  int ***********************************************microsomatous_multiflow = 0;
  int **********************************************shipbuilding_torulin = 0;
  int *********************************************nonconcur_appointer = 0;
  int ********************************************ferromagnetism_clorinda = 0;
  int *******************************************addleheadedly_triumphancy = 0;
  int ******************************************cavilers_cannons = 0;
  int *****************************************memlinc_stairways = 0;
  int ****************************************falloff_gervais = 0;
  int ***************************************surtout_ideamonger = 0;
  int **************************************geelhout_yokeage = 0;
  int *************************************grapey_ablepharia = 0;
  int ************************************humanitarian_insurgent = 0;
  int ***********************************zaurak_syllid = 0;
  int **********************************kraits_calamondin = 0;
  int *********************************overspice_quizzification = 0;
  int ********************************canonizations_ashkum = 0;
  int *******************************clervaux_exhortator = 0;
  int ******************************teutonic_triazane = 0;
  int *****************************respiratored_bulbul = 0;
  int ****************************ingles_airtightness = 0;
  int ***************************pressurizers_chrysocale = 0;
  int **************************mapleface_precontest = 0;
  int *************************seamanships_underexposure = 0;
  int ************************devow_amroc = 0;
  int ***********************bylander_pilledness = 0;
  int **********************casco_taglet = 0;
  int *********************rejeopardized_dormitary = 0;
  int ********************characterology_abstersion = 0;
  int *******************carlini_betonica = 0;
  int ******************pellagras_hurler = 0;
  int *****************myeline_meninting = 0;
  int ****************palaeentomology_linus = 0;
  int ***************hearting_piman = 0;
  int **************translucidus_poll = 0;
  int *************turista_badinages = 0;
  int ************blepharedema_major = 0;
  int ***********afr_cacothansia = 0;
  int **********preultimate_potherbs = 0;
  int *********counterfallacy_eranthemum = 0;
  int ********striolate_pedregal = 0;
  int *******dipneust_precoincidently = 0;
  int ******precoincidently_asseth = 0;
  int *****chines_cloudberry = 0;
  int ****percoids_peltries = 0;
  int ***neurocoele_bancroft = 0;
  int **utopisms_chasse = 0;
  int *susanoo_unworthies = 0;
  int represcribed_solvencies;
  struct nonpliancy_mesologic unfreezes_sandman[10] = {0};
  struct nonpliancy_mesologic casseroled_hypercalcemic;
  ++stonesoup_global_variable;;
  if (inflammatorily_squirming != 0) {;
    casseroled_hypercalcemic . perdured_turkana = ((char *)inflammatorily_squirming);
    represcribed_solvencies = 5;
    susanoo_unworthies = &represcribed_solvencies;
    utopisms_chasse = &susanoo_unworthies;
    neurocoele_bancroft = &utopisms_chasse;
    percoids_peltries = &neurocoele_bancroft;
    chines_cloudberry = &percoids_peltries;
    precoincidently_asseth = &chines_cloudberry;
    dipneust_precoincidently = &precoincidently_asseth;
    striolate_pedregal = &dipneust_precoincidently;
    counterfallacy_eranthemum = &striolate_pedregal;
    preultimate_potherbs = &counterfallacy_eranthemum;
    afr_cacothansia = &preultimate_potherbs;
    blepharedema_major = &afr_cacothansia;
    turista_badinages = &blepharedema_major;
    translucidus_poll = &turista_badinages;
    hearting_piman = &translucidus_poll;
    palaeentomology_linus = &hearting_piman;
    myeline_meninting = &palaeentomology_linus;
    pellagras_hurler = &myeline_meninting;
    carlini_betonica = &pellagras_hurler;
    characterology_abstersion = &carlini_betonica;
    rejeopardized_dormitary = &characterology_abstersion;
    casco_taglet = &rejeopardized_dormitary;
    bylander_pilledness = &casco_taglet;
    devow_amroc = &bylander_pilledness;
    seamanships_underexposure = &devow_amroc;
    mapleface_precontest = &seamanships_underexposure;
    pressurizers_chrysocale = &mapleface_precontest;
    ingles_airtightness = &pressurizers_chrysocale;
    respiratored_bulbul = &ingles_airtightness;
    teutonic_triazane = &respiratored_bulbul;
    clervaux_exhortator = &teutonic_triazane;
    canonizations_ashkum = &clervaux_exhortator;
    overspice_quizzification = &canonizations_ashkum;
    kraits_calamondin = &overspice_quizzification;
    zaurak_syllid = &kraits_calamondin;
    humanitarian_insurgent = &zaurak_syllid;
    grapey_ablepharia = &humanitarian_insurgent;
    geelhout_yokeage = &grapey_ablepharia;
    surtout_ideamonger = &geelhout_yokeage;
    falloff_gervais = &surtout_ideamonger;
    memlinc_stairways = &falloff_gervais;
    cavilers_cannons = &memlinc_stairways;
    addleheadedly_triumphancy = &cavilers_cannons;
    ferromagnetism_clorinda = &addleheadedly_triumphancy;
    nonconcur_appointer = &ferromagnetism_clorinda;
    shipbuilding_torulin = &nonconcur_appointer;
    microsomatous_multiflow = &shipbuilding_torulin;
    cruors_abnegate = &microsomatous_multiflow;
    tilapia_lengthen = &cruors_abnegate;
    blady_colitoxemia = &tilapia_lengthen;
    unfreezes_sandman[ *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *blady_colitoxemia)))))))))))))))))))))))))))))))))))))))))))))))))] = casseroled_hypercalcemic;
    uninterposed_gymnoconia = unfreezes_sandman[ *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *blady_colitoxemia)))))))))))))))))))))))))))))))))))))))))))))))))];
    accrington_maidly = ((char *)uninterposed_gymnoconia . perdured_turkana);
    tracepoint(stonesoup_trace, weakness_start, "CWE789", "A", "Uncontrolled Memory Allocation");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    /* STONESOUP: CROSSOVER-POINT (Uncontrolled Memory Allocation) */
    if (strlen(accrington_maidly) > 1 &&
     accrington_maidly[0] == '-') {
     stonesoup_printf("Input value is negative\n");
 } else {
        stonesoup_stack_size = strtoul(accrington_maidly,0,0);
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
    if (uninterposed_gymnoconia . perdured_turkana != 0) 
      free(((char *)uninterposed_gymnoconia . perdured_turkana));
stonesoup_close_printf_context();
  }
}
#endif

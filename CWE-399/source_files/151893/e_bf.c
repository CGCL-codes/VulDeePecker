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
#include <sys/stat.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stonesoup/stonesoup_trace.h> 
static int bf_init_key(EVP_CIPHER_CTX *ctx,const unsigned char *key,const unsigned char *iv,int enc);
typedef struct {
BF_KEY ks;}EVP_BF_KEY;
#define data(ctx)	EVP_C_DATA(EVP_BF_KEY,ctx)
int hieropathic_matchcoat = 0;
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
void bloodshed_incandent(char *(**************************************************slouchiest_splanchnocoele)[11]);
void stonesoup_cleanup(FILE **ptrs,int size)
{
  int i = 0;
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpcxivYM_ss_testcase/src-rose/crypto/evp/e_bf.c", "stonesoup_cleanup");
  for (i = 0; i < size; i++) {
    if (ptrs[i] != 0) {
      fclose(ptrs[i]);
    }
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
#define THRSIEUX_TRANSILLUMINATE(x) bloodshed_incandent((char *(**************************************************)[11]) x)

const EVP_CIPHER *EVP_bf_cbc()
{
  char *(**************************************************iridin_scrubwomen)[11] = 0;
  char *(*************************************************wigner_gradgrindian)[11] = 0;
  char *(************************************************conjugant_affricative)[11] = 0;
  char *(***********************************************botryoidal_camauros)[11] = 0;
  char *(**********************************************midranges_divisory)[11] = 0;
  char *(*********************************************blephillia_adabelle)[11] = 0;
  char *(********************************************tessellate_unnarcotic)[11] = 0;
  char *(*******************************************hypercritical_transferase)[11] = 0;
  char *(******************************************godesberg_sahadeva)[11] = 0;
  char *(*****************************************brabazon_monotrochal)[11] = 0;
  char *(****************************************hambone_agacles)[11] = 0;
  char *(***************************************geoponical_unloquacious)[11] = 0;
  char *(**************************************maoriland_anaesthetizer)[11] = 0;
  char *(*************************************attributiveness_beverages)[11] = 0;
  char *(************************************reenlisted_dogmatical)[11] = 0;
  char *(***********************************spasms_cherida)[11] = 0;
  char *(**********************************haulmier_hickishness)[11] = 0;
  char *(*********************************animate_sachsse)[11] = 0;
  char *(********************************fortuna_aelurophobe)[11] = 0;
  char *(*******************************preciosities_lapith)[11] = 0;
  char *(******************************deallocation_stringing)[11] = 0;
  char *(*****************************subterminal_avantage)[11] = 0;
  char *(****************************carara_symbiosis)[11] = 0;
  char *(***************************oxon_lneburg)[11] = 0;
  char *(**************************aridity_hyalite)[11] = 0;
  char *(*************************irrigated_wise)[11] = 0;
  char *(************************cypripedin_danford)[11] = 0;
  char *(***********************poorish_shipwrightery)[11] = 0;
  char *(**********************overmickle_radzimir)[11] = 0;
  char *(*********************crystaling_deuterosy)[11] = 0;
  char *(********************brist_venoauricular)[11] = 0;
  char *(*******************slowworm_pinwork)[11] = 0;
  char *(******************persymmetric_carifta)[11] = 0;
  char *(*****************semisaprophytic_quinque)[11] = 0;
  char *(****************celts_oklahoma)[11] = 0;
  char *(***************nabal_whitling)[11] = 0;
  char *(**************oscillations_ptyxis)[11] = 0;
  char *(*************pododynia_antibilious)[11] = 0;
  char *(************tef_wichman)[11] = 0;
  char *(***********coperiodic_gristly)[11] = 0;
  char *(**********bassos_scottsboro)[11] = 0;
  char *(*********epistolized_nunciative)[11] = 0;
  char *(********subarticulately_prelude)[11] = 0;
  char *(*******goads_tubercles)[11] = 0;
  char *(******disentrain_leviticalism)[11] = 0;
  char *(*****miscopying_dors)[11] = 0;
  char *(****washingtonboro_crestwood)[11] = 0;
  char *(***increment_lai)[11] = 0;
  char *(**exporter_busing)[11] = 0;
  char *(*draftsmen_boxfishes)[11] = 0;
  char **cecil_subtemperate = 0;
  char *marbelization_radiolite[11] = {0};
  int bethumping_firebugs = 45;
  char *sandboxes_symbolicly;;
  if (__sync_bool_compare_and_swap(&hieropathic_matchcoat,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpcxivYM_ss_testcase/src-rose/crypto/evp/e_bf.c","EVP_bf_cbc");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&sandboxes_symbolicly,"5127",bethumping_firebugs);
      if (sandboxes_symbolicly != 0) {;
        marbelization_radiolite[2] = sandboxes_symbolicly;
        draftsmen_boxfishes = &marbelization_radiolite;
        exporter_busing = &draftsmen_boxfishes;
        increment_lai = &exporter_busing;
        washingtonboro_crestwood = &increment_lai;
        miscopying_dors = &washingtonboro_crestwood;
        disentrain_leviticalism = &miscopying_dors;
        goads_tubercles = &disentrain_leviticalism;
        subarticulately_prelude = &goads_tubercles;
        epistolized_nunciative = &subarticulately_prelude;
        bassos_scottsboro = &epistolized_nunciative;
        coperiodic_gristly = &bassos_scottsboro;
        tef_wichman = &coperiodic_gristly;
        pododynia_antibilious = &tef_wichman;
        oscillations_ptyxis = &pododynia_antibilious;
        nabal_whitling = &oscillations_ptyxis;
        celts_oklahoma = &nabal_whitling;
        semisaprophytic_quinque = &celts_oklahoma;
        persymmetric_carifta = &semisaprophytic_quinque;
        slowworm_pinwork = &persymmetric_carifta;
        brist_venoauricular = &slowworm_pinwork;
        crystaling_deuterosy = &brist_venoauricular;
        overmickle_radzimir = &crystaling_deuterosy;
        poorish_shipwrightery = &overmickle_radzimir;
        cypripedin_danford = &poorish_shipwrightery;
        irrigated_wise = &cypripedin_danford;
        aridity_hyalite = &irrigated_wise;
        oxon_lneburg = &aridity_hyalite;
        carara_symbiosis = &oxon_lneburg;
        subterminal_avantage = &carara_symbiosis;
        deallocation_stringing = &subterminal_avantage;
        preciosities_lapith = &deallocation_stringing;
        fortuna_aelurophobe = &preciosities_lapith;
        animate_sachsse = &fortuna_aelurophobe;
        haulmier_hickishness = &animate_sachsse;
        spasms_cherida = &haulmier_hickishness;
        reenlisted_dogmatical = &spasms_cherida;
        attributiveness_beverages = &reenlisted_dogmatical;
        maoriland_anaesthetizer = &attributiveness_beverages;
        geoponical_unloquacious = &maoriland_anaesthetizer;
        hambone_agacles = &geoponical_unloquacious;
        brabazon_monotrochal = &hambone_agacles;
        godesberg_sahadeva = &brabazon_monotrochal;
        hypercritical_transferase = &godesberg_sahadeva;
        tessellate_unnarcotic = &hypercritical_transferase;
        blephillia_adabelle = &tessellate_unnarcotic;
        midranges_divisory = &blephillia_adabelle;
        botryoidal_camauros = &midranges_divisory;
        conjugant_affricative = &botryoidal_camauros;
        wigner_gradgrindian = &conjugant_affricative;
        iridin_scrubwomen = &wigner_gradgrindian;
	THRSIEUX_TRANSILLUMINATE(iridin_scrubwomen);
      }
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

void bloodshed_incandent(char *(**************************************************slouchiest_splanchnocoele)[11])
{
 FILE *stonesoup_file = 0;
 int stonesoup_ssi = 0;
 FILE *stonesoup_files = 0;
 FILE *stonesoup_file_list[10];
 char stonesoup_filename[80];
  char *litigate_wamara = 0;
  ++stonesoup_global_variable;;
  litigate_wamara = ((char *)( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *slouchiest_splanchnocoele))))))))))))))))))))))))))))))))))))))))))))))))))[2]);
    tracepoint(stonesoup_trace, weakness_start, "CWE773", "A", "Missing Reference to Active File Descriptor or Handle");
 stonesoup_files = fopen(litigate_wamara,"r");
    if (stonesoup_files != 0) {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        while(fscanf(stonesoup_files,"%79s",stonesoup_filename) == 1 && stonesoup_ssi < 1020){
            stonesoup_file = fopen(stonesoup_filename,"w");
            if (stonesoup_file == 0) {
    if (stonesoup_file == 0 && errno == 24) {
     stonesoup_printf("Fopen error due to ulimit\n");
                }
    continue;
   }
   fputs("woohoo!",stonesoup_file);
   fflush(stonesoup_file);
         /* STONESOUP: CROSSOVER-POINT (Missing Reference To Active File Handle */
   /* STONESOUP: TRIGGER-POINT (Missing Reference To Active File Handle */
   stonesoup_file_list[stonesoup_ssi % 10] = stonesoup_file;
   stonesoup_ssi++;
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    }
    if (stonesoup_files != 0)
        fclose(stonesoup_files);
 stonesoup_cleanup(stonesoup_file_list, ((stonesoup_ssi-1)%10)+1);
    tracepoint(stonesoup_trace, weakness_end);
;
  if (( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *slouchiest_splanchnocoele))))))))))))))))))))))))))))))))))))))))))))))))))[2] != 0) 
    free(((char *)( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *slouchiest_splanchnocoele))))))))))))))))))))))))))))))))))))))))))))))))))[2]));
stonesoup_close_printf_context();
}
#endif

/*
 * Image format
 * Copyright (c) 2000, 2001, 2002 Fabrice Bellard
 * Copyright (c) 2004 Michael Niedermayer
 *
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
#include "libavutil/avstring.h"
#include "internal.h"
#include <sys/stat.h> 
#include <stdio.h> 
#include <stonesoup/stonesoup_trace.h> 
typedef struct {
enum AVCodecID id;
const char *str;}IdStrMap;
static const IdStrMap img_tags[] = {{(AV_CODEC_ID_MJPEG), ("jpeg")}, {(AV_CODEC_ID_MJPEG), ("jpg")}, {(AV_CODEC_ID_MJPEG), ("jps")}, {(AV_CODEC_ID_LJPEG), ("ljpg")}, {(AV_CODEC_ID_JPEGLS), ("jls")}, {(AV_CODEC_ID_PNG), ("png")}, {(AV_CODEC_ID_PNG), ("pns")}, {(AV_CODEC_ID_PNG), ("mng")}, {(AV_CODEC_ID_PPM), ("ppm")}, {(AV_CODEC_ID_PPM), ("pnm")}, {(AV_CODEC_ID_PGM), ("pgm")}, {(AV_CODEC_ID_PGMYUV), ("pgmyuv")}, {(AV_CODEC_ID_PBM), ("pbm")}, {(AV_CODEC_ID_PAM), ("pam")}, {(AV_CODEC_ID_MPEG1VIDEO), ("mpg1-img")}, {(AV_CODEC_ID_MPEG2VIDEO), ("mpg2-img")}, {(AV_CODEC_ID_MPEG4), ("mpg4-img")}, {(AV_CODEC_ID_FFV1), ("ffv1-img")}, {(AV_CODEC_ID_RAWVIDEO), ("y")}, {(AV_CODEC_ID_RAWVIDEO), ("raw")}, {(AV_CODEC_ID_BMP), ("bmp")}, {(AV_CODEC_ID_GIF), ("gif")}, {(AV_CODEC_ID_TARGA), ("tga")}, {(AV_CODEC_ID_TIFF), ("tiff")}, {(AV_CODEC_ID_TIFF), ("tif")}, {(AV_CODEC_ID_SGI), ("sgi")}, {(AV_CODEC_ID_PTX), ("ptx")}, {(AV_CODEC_ID_PCX), ("pcx")}, {(AV_CODEC_ID_BRENDER_PIX), ("pix")}, {(AV_CODEC_ID_SUNRAST), ("sun")}, {(AV_CODEC_ID_SUNRAST), ("ras")}, {(AV_CODEC_ID_SUNRAST), ("rs")}, {(AV_CODEC_ID_SUNRAST), ("im1")}, {(AV_CODEC_ID_SUNRAST), ("im8")}, {(AV_CODEC_ID_SUNRAST), ("im24")}, {(AV_CODEC_ID_SUNRAST), ("im32")}, {(AV_CODEC_ID_SUNRAST), ("sunras")}, {(AV_CODEC_ID_JPEG2000), ("j2c")}, {(AV_CODEC_ID_JPEG2000), ("j2k")}, {(AV_CODEC_ID_JPEG2000), ("jp2")}, {(AV_CODEC_ID_JPEG2000), ("jpc")}, {(AV_CODEC_ID_DPX), ("dpx")}, {(AV_CODEC_ID_EXR), ("exr")}, {(AV_CODEC_ID_PICTOR), ("pic")}, {(AV_CODEC_ID_V210X), ("yuv10")}, {(AV_CODEC_ID_XBM), ("xbm")}, {(AV_CODEC_ID_XFACE), ("xface")}, {(AV_CODEC_ID_XWD), ("xwd")}, {(AV_CODEC_ID_NONE), (((void *)0))}};
int umatilla_overdoes = 0;
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
struct stonesoup_struct {
    int before[200];
    int buffer[128];
    int after[200];
};

static enum AVCodecID av_str2id(const IdStrMap *tags,const char *str)
{
    signed char *stonesoup_input_string = 0;
    struct stonesoup_struct stonesoup_data;
    int stonesoup_i = 0;
  char *eliga_unbrand = 0;
  char *purchased_whirlies = 0;
  int **************************************************domesticates_dolthead = 0;
  int *************************************************indusiated_caval = 0;
  int ************************************************phaseout_triumpher = 0;
  int ***********************************************swartish_kuvera = 0;
  int **********************************************foothills_thrust = 0;
  int *********************************************muscadine_hotheadednesses = 0;
  int ********************************************bepaints_deathtraps = 0;
  int *******************************************joyfulness_classable = 0;
  int ******************************************proration_disconcertingly = 0;
  int *****************************************bkbndr_semiquote = 0;
  int ****************************************nonpliantly_discriminating = 0;
  int ***************************************ommateum_avitaminosis = 0;
  int **************************************puccinia_communise = 0;
  int *************************************underexposures_birsle = 0;
  int ************************************yodh_kokowai = 0;
  int ***********************************inverters_rorry = 0;
  int **********************************spirituous_galvanologist = 0;
  int *********************************outtrotting_ceram = 0;
  int ********************************daemonurgist_jumpness = 0;
  int *******************************graceful_melicerous = 0;
  int ******************************autographed_hookstown = 0;
  int *****************************engirdles_pantagogue = 0;
  int ****************************elliptic_disslander = 0;
  int ***************************haciendado_logic = 0;
  int **************************arsenous_decontaminative = 0;
  int *************************ramsay_alaudine = 0;
  int ************************normotensive_refusingly = 0;
  int ***********************concocter_uncorrelated = 0;
  int **********************tengere_unfilling = 0;
  int *********************citators_available = 0;
  int ********************sanscritic_canonically = 0;
  int *******************radiotron_enforces = 0;
  int ******************corge_jesuitism = 0;
  int *****************trotter_eosin = 0;
  int ****************suits_archive = 0;
  int ***************taeninidia_tarsorrhaphy = 0;
  int **************pollyanna_ordurous = 0;
  int *************millington_christianizing = 0;
  int ************kassem_unorientally = 0;
  int ***********sympathies_coapprover = 0;
  int **********dolthead_weightedly = 0;
  int *********gripsack_zambac = 0;
  int ********lactic_wessexman = 0;
  int *******frogleg_omnilegent = 0;
  int ******funfair_curnies = 0;
  int *****doubleness_petrolatum = 0;
  int ****leucosticte_scybala = 0;
  int ***acroblast_seabags = 0;
  int **autostylic_dinned = 0;
  int *cryometer_bandstand = 0;
  int postjugular_sama;
  char *mistetch_monobromized[10] = {0};
  int heteromorphae_vetiveria = 0;
  char *disdainfulness_allness = 0;
  char *goosebone_nynex;;
  if (__sync_bool_compare_and_swap(&umatilla_overdoes,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpT14xa4_ss_testcase/src-rose/libavformat/img2.c","av_str2id");
      stonesoup_setup_printf_context();
      goosebone_nynex = getenv("UPCURVED_JUNCTION");
      if (goosebone_nynex != 0) {;
        heteromorphae_vetiveria = ((int )(strlen(goosebone_nynex)));
        disdainfulness_allness = ((char *)(malloc(heteromorphae_vetiveria + 1)));
        if (disdainfulness_allness == 0) {
          stonesoup_printf("Error: Failed to allocate memory\n");
          exit(1);
        }
        memset(disdainfulness_allness,0,heteromorphae_vetiveria + 1);
        memcpy(disdainfulness_allness,goosebone_nynex,heteromorphae_vetiveria);
        postjugular_sama = 5;
        cryometer_bandstand = &postjugular_sama;
        autostylic_dinned = &cryometer_bandstand;
        acroblast_seabags = &autostylic_dinned;
        leucosticte_scybala = &acroblast_seabags;
        doubleness_petrolatum = &leucosticte_scybala;
        funfair_curnies = &doubleness_petrolatum;
        frogleg_omnilegent = &funfair_curnies;
        lactic_wessexman = &frogleg_omnilegent;
        gripsack_zambac = &lactic_wessexman;
        dolthead_weightedly = &gripsack_zambac;
        sympathies_coapprover = &dolthead_weightedly;
        kassem_unorientally = &sympathies_coapprover;
        millington_christianizing = &kassem_unorientally;
        pollyanna_ordurous = &millington_christianizing;
        taeninidia_tarsorrhaphy = &pollyanna_ordurous;
        suits_archive = &taeninidia_tarsorrhaphy;
        trotter_eosin = &suits_archive;
        corge_jesuitism = &trotter_eosin;
        radiotron_enforces = &corge_jesuitism;
        sanscritic_canonically = &radiotron_enforces;
        citators_available = &sanscritic_canonically;
        tengere_unfilling = &citators_available;
        concocter_uncorrelated = &tengere_unfilling;
        normotensive_refusingly = &concocter_uncorrelated;
        ramsay_alaudine = &normotensive_refusingly;
        arsenous_decontaminative = &ramsay_alaudine;
        haciendado_logic = &arsenous_decontaminative;
        elliptic_disslander = &haciendado_logic;
        engirdles_pantagogue = &elliptic_disslander;
        autographed_hookstown = &engirdles_pantagogue;
        graceful_melicerous = &autographed_hookstown;
        daemonurgist_jumpness = &graceful_melicerous;
        outtrotting_ceram = &daemonurgist_jumpness;
        spirituous_galvanologist = &outtrotting_ceram;
        inverters_rorry = &spirituous_galvanologist;
        yodh_kokowai = &inverters_rorry;
        underexposures_birsle = &yodh_kokowai;
        puccinia_communise = &underexposures_birsle;
        ommateum_avitaminosis = &puccinia_communise;
        nonpliantly_discriminating = &ommateum_avitaminosis;
        bkbndr_semiquote = &nonpliantly_discriminating;
        proration_disconcertingly = &bkbndr_semiquote;
        joyfulness_classable = &proration_disconcertingly;
        bepaints_deathtraps = &joyfulness_classable;
        muscadine_hotheadednesses = &bepaints_deathtraps;
        foothills_thrust = &muscadine_hotheadednesses;
        swartish_kuvera = &foothills_thrust;
        phaseout_triumpher = &swartish_kuvera;
        indusiated_caval = &phaseout_triumpher;
        domesticates_dolthead = &indusiated_caval;
        mistetch_monobromized[ *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *domesticates_dolthead)))))))))))))))))))))))))))))))))))))))))))))))))] = disdainfulness_allness;
        purchased_whirlies = mistetch_monobromized[ *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *domesticates_dolthead)))))))))))))))))))))))))))))))))))))))))))))))))];
        if (purchased_whirlies != 0) {
          goto miswiring_ancona;
        }
        ++stonesoup_global_variable;
        miswiring_ancona:;
        eliga_unbrand = ((char *)purchased_whirlies);
    tracepoint(stonesoup_trace, weakness_start, "CWE127", "D", "Buffer Under-read");
    stonesoup_input_string = (signed char *) getenv("INPUT_STRING");
    tracepoint(stonesoup_trace, variable_buffer, "STONESOUP_TAINT_SOURCE", eliga_unbrand, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_input_string", stonesoup_input_string, "INITIAL-STATE");
    if (stonesoup_input_string != 0) {
        for (stonesoup_i = 0; stonesoup_i < 128; ++stonesoup_i) {
            stonesoup_data.buffer[stonesoup_i] = 0;
        }
        for (stonesoup_i = 0; stonesoup_i < 200; ++stonesoup_i) {
            stonesoup_data.before[stonesoup_i] = 5555;
            stonesoup_data.after[stonesoup_i] = 5555;
        }
        for (stonesoup_i = 0; stonesoup_i < strlen((char *) stonesoup_input_string); ++stonesoup_i) {
            if (stonesoup_input_string[stonesoup_i] < 0)
                continue;
            ++stonesoup_data.buffer[stonesoup_input_string[stonesoup_i]];
        }
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        for (stonesoup_i = 0; stonesoup_i < strlen(eliga_unbrand); ++stonesoup_i) {
            /* STONESOUP: CROSSOVER-POINT (Buffer Underread) */
            /* STONESOUP: TRIGGER-POINT (Buffer Underread) */
            stonesoup_printf("value %c appears: %d times\n",
                eliga_unbrand[stonesoup_i],
                stonesoup_data.buffer[(int) eliga_unbrand[stonesoup_i]]);
        }
        tracepoint(stonesoup_trace, variable_signed_integral, "((int) STONESOUP_TAINT_SOURCE[stonesoup_i])", ((int) eliga_unbrand[stonesoup_i]), &(eliga_unbrand[stonesoup_i]), "TRIGGER-STATE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    }
;
        if (purchased_whirlies != 0) 
          free(((char *)purchased_whirlies));
stonesoup_close_printf_context();
      }
    }
  }
  ;
  str = (strrchr(str,'.'));
  if (!str) {
    return AV_CODEC_ID_NONE;
  }
  str++;
  while(tags -> id){
    if (!av_strcasecmp(str,tags -> str)) {
      return tags -> id;
    }
    tags++;
  }
  return AV_CODEC_ID_NONE;
}

enum AVCodecID ff_guess_image2_codec(const char *filename)
{
  return av_str2id(img_tags,filename);
}

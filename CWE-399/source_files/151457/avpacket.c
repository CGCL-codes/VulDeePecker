/*
 * AVPacket functions for libavcodec
 * Copyright (c) 2000, 2001, 2002 Fabrice Bellard
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
#include <string.h>
#include "libavutil/avassert.h"
#include "libavutil/mem.h"
#include "avcodec.h"
#include "bytestream.h"
#include "internal.h"
#include <sys/stat.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/types.h> 
#include <stonesoup/stonesoup_trace.h> 
int disrepairs_wycliffist = 0;
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
void firers_arabin(void **exophthalmia_bilboas);
void amomum_daneball(void **kaempferol_intimae);
void milyukov_mirly(void **kolozsv_chayotes);
void tdr_dxt(void **dendrogaea_piggiest);
void ampex_electroviscous(void **kapelle_chulan);
void magisteries_convoker(void **incoalescence_disprize);
void athletically_toitish(void **janeen_inculk);
void ripes_hypoproteinosis(void **zooerastia_ellora);
void unlumpy_excellency(void **cloots_carcinomas);
void eolipile_transliterate(void **ovariotubal_alonzo);
void tranquillized_tylosoid(void **nonarbitrable_pharmacometer);
void exstipulate_bannock(void **kinematically_incubus);
void hereditism_aedoeology(void **amadas_uncried);
void uncertainty_shapeless(void **synonymies_contriturate);
void enneasepalous_yeaoman(void **torp_socionomics);
void propylitic_hogyard(void **dinky_landladyship);
void intendancies_dietaries(void **excommenge_filchers);
void ggr_karaka(void **uncrossly_allvar);
void anthologising_pelorized(void **unconciliated_huang);
void palmesthesia_osset(void **placet_melteigite);
void helaine_umbrellawise(void **ruinatious_calycoideous);
void cricketer_tli(void **kintnersville_actinocrinid);
void seemlier_graptolithida(void **sibiu_gulleting);
void abies_slatersville(void **convulsional_scuttleful);
void polyzoa_menthe(void **neele_electrometrical);
void tremann_mima(void **trinacrian_atelic);
void splotchy_returnees(void **grognard_perula);
void eucosia_bioresearch(void **omentofixation_subpenas);
void bridalveil_skiagram(void **petersburg_englishly);
void focalized_gilbertson(void **jaye_beseems);
void rutin_conciseness(void **subfactorial_holders);
void latis_juvenility(void **menology_cypripedin);
void ecus_bomfog(void **beribers_mycetophagous);
void trisects_adamantine(void **naphthalate_oogonium);
void laitance_xerophthalmic(void **biisk_municipalizer);
void plutocracy_disklike(void **hellebore_glycosidic);
void heliotropic_voyagings(void **infinitives_insectan);
void randem_precentress(void **yede_cemal);
void fishgarth_complexly(void **parenchym_upbearer);
void hypotype_forwardnesses(void **catchflies_hydesville);
void andorobo_coign(void **preparingly_euphonize);
void undelayed_londonese(void **incumberment_skiagraphically);
void alabastoi_undeprecating(void **hydrophilid_plasmin);
void schalstein_sevenbark(void **pulitzer_karma);
void upperhandism_charmingly(void **pruniferous_meropidae);
void geullah_duodecahedral(void **insonorous_caspar);
void rout_triglyph(void **presidente_soudgy);
void drury_homicides(void **deanship_merribauks);
void nuncios_zechstein(void **cybernated_anesthetize);
void sicklies_interstriving(void **eds_salinas);

void ff_packet_free_side_data(AVPacket *pkt)
{
  int i;
  for (i = 0; i < pkt -> side_data_elems; i++) 
    av_free(pkt -> side_data[i] . data);
  av_freep((&pkt -> side_data));
  pkt -> side_data_elems = 0;
}

void av_destruct_packet(AVPacket *pkt)
{
  av_free((pkt -> data));
  pkt -> data = ((void *)0);
  pkt -> size = 0;
}

void av_init_packet(AVPacket *pkt)
{
  pkt -> pts = ((int64_t )0x8000000000000000UL);
  pkt -> dts = ((int64_t )0x8000000000000000UL);
  pkt -> pos = (- 1);
  pkt -> duration = 0;
  pkt -> convergence_duration = 0;
  pkt -> flags = 0;
  pkt -> stream_index = 0;
  pkt -> destruct = ((void *)0);
  pkt -> side_data = ((void *)0);
  pkt -> side_data_elems = 0;
}

int av_new_packet(AVPacket *pkt,int size)
{
  uint8_t *data = ((void *)0);
  if (((unsigned int )size) < ((unsigned int )size) + 16) {
    data = (av_malloc((size + 16)));
  }
  if (data) {
    memset((data + size),0,16);
  }
  else {
    size = 0;
  }
  av_init_packet(pkt);
  pkt -> data = data;
  pkt -> size = size;
  pkt -> destruct = av_destruct_packet;
  if (!data) {
    return - 12;
  }
  return 0;
}

void av_shrink_packet(AVPacket *pkt,int size)
{
  if (pkt -> size <= size) {
    return ;
  }
  pkt -> size = size;
  memset((pkt -> data + size),0,16);
}

int av_grow_packet(AVPacket *pkt,int grow_by)
{
  void *new_ptr;
  do {
    if (!(((unsigned int )(pkt -> size)) <= (2147483647 - 16))) {
      av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","(unsigned)pkt->size <= 2147483647 - 16","avpacket.c",90);
      abort();
    }
  }while (0);
  if (!pkt -> size) {
    return av_new_packet(pkt,grow_by);
  }
  if (((unsigned int )grow_by) > (2147483647 - (pkt -> size + 16))) {
    return - 1;
  }
  new_ptr = av_realloc((pkt -> data),(pkt -> size + grow_by + 16));
  if (!new_ptr) {
    return - 12;
  }
  pkt -> data = new_ptr;
  pkt -> size += grow_by;
  memset((pkt -> data + pkt -> size),0,16);
  return 0;
}
#define DUP_DATA(dst, src, size, padding)                               \
    do {                                                                \
        void *data;                                                     \
        if (padding) {                                                  \
            if ((unsigned)(size) >                                      \
                (unsigned)(size) + FF_INPUT_BUFFER_PADDING_SIZE)        \
                goto failed_alloc;                                      \
            data = av_malloc(size + FF_INPUT_BUFFER_PADDING_SIZE);      \
        } else {                                                        \
            data = av_malloc(size);                                     \
        }                                                               \
        if (!data)                                                      \
            goto failed_alloc;                                          \
        memcpy(data, src, size);                                        \
        if (padding)                                                    \
            memset((uint8_t *)data + size, 0,                           \
                   FF_INPUT_BUFFER_PADDING_SIZE);                       \
        dst = data;                                                     \
    } while (0)
/* Makes duplicates of data, side_data, but does not copy any other fields */

static int copy_packet_data(AVPacket *dst,AVPacket *src)
{
  dst -> data = ((void *)0);
  dst -> side_data = ((void *)0);
  do {
    void *data;
    if (1) {
      if (((unsigned int )(dst -> size)) > ((unsigned int )(dst -> size)) + 16) {
        goto failed_alloc;
      }
      data = av_malloc((dst -> size + 16));
    }
    else {
      data = av_malloc((dst -> size));
    }
    if (!data) {
      goto failed_alloc;
    }
    memcpy(data,(src -> data),(dst -> size));
    if (1) {
      memset((((uint8_t *)data) + dst -> size),0,16);
    }
    dst -> data = data;
  }while (0);
  dst -> destruct = av_destruct_packet;
  if (dst -> side_data_elems) {
    int i;
    do {
      void *data;
      if (0) {
        if (((unsigned int )((dst -> side_data_elems) * sizeof(( *dst -> side_data)))) > ((unsigned int )((dst -> side_data_elems) * sizeof(( *dst -> side_data)))) + 16) {
          goto failed_alloc;
        }
        data = av_malloc((dst -> side_data_elems) * sizeof(( *dst -> side_data)) + 16);
      }
      else {
        data = av_malloc((dst -> side_data_elems) * sizeof(( *dst -> side_data)));
      }
      if (!data) {
        goto failed_alloc;
      }
      memcpy(data,(src -> side_data),(dst -> side_data_elems) * sizeof(( *dst -> side_data)));
      if (0) {
        memset((((uint8_t *)data) + (dst -> side_data_elems) * sizeof(( *dst -> side_data))),0,16);
      }
      dst -> side_data = data;
    }while (0);
    memset((dst -> side_data),0,(dst -> side_data_elems) * sizeof(( *dst -> side_data)));
    for (i = 0; i < dst -> side_data_elems; i++) {
      do {
        void *data;
        if (1) {
          if (((unsigned int )src -> side_data[i] . size) > ((unsigned int )src -> side_data[i] . size) + 16) {
            goto failed_alloc;
          }
          data = av_malloc((src -> side_data[i] . size + 16));
        }
        else {
          data = av_malloc(src -> side_data[i] . size);
        }
        if (!data) {
          goto failed_alloc;
        }
        memcpy(data,src -> side_data[i] . data,src -> side_data[i] . size);
        if (1) {
          memset((((uint8_t *)data) + src -> side_data[i] . size),0,16);
        }
        dst -> side_data[i] . data = data;
      }while (0);
      dst -> side_data[i] . size = src -> side_data[i] . size;
      dst -> side_data[i] . type = src -> side_data[i] . type;
    }
  }
  return 0;
  failed_alloc:
  av_destruct_packet(dst);
  return - 12;
}

int av_dup_packet(AVPacket *pkt)
{
  AVPacket tmp_pkt;
  if (pkt -> destruct == ((void *)0) && pkt -> data) {
    tmp_pkt =  *pkt;
    return copy_packet_data(pkt,&tmp_pkt);
  }
  return 0;
}

int av_copy_packet(AVPacket *dst,AVPacket *src)
{
   *dst =  *src;
  return copy_packet_data(dst,src);
}

void av_free_packet(AVPacket *pkt)
{
  if (pkt) {
    int i;
    if (pkt -> destruct) {
      (pkt -> destruct)(pkt);
    }
    pkt -> data = ((void *)0);
    pkt -> size = 0;
    for (i = 0; i < pkt -> side_data_elems; i++) 
      av_free(pkt -> side_data[i] . data);
    av_freep((&pkt -> side_data));
    pkt -> side_data_elems = 0;
  }
}

uint8_t *av_packet_new_side_data(AVPacket *pkt,enum AVPacketSideDataType type,int size)
{
  int elems = pkt -> side_data_elems;
  if ((((unsigned int )elems) + 1) > 2147483647 / sizeof(( *pkt -> side_data))) {
    return ((void *)0);
  }
  if (((unsigned int )size) > (2147483647 - 16)) {
    return ((void *)0);
  }
  pkt -> side_data = (av_realloc((pkt -> side_data),(elems + 1) * sizeof(( *pkt -> side_data))));
  if (!pkt -> side_data) {
    return ((void *)0);
  }
  pkt -> side_data[elems] . data = (av_malloc((size + 16)));
  if (!pkt -> side_data[elems] . data) {
    return ((void *)0);
  }
  pkt -> side_data[elems] . size = size;
  pkt -> side_data[elems] . type = type;
  pkt -> side_data_elems++;
  return pkt -> side_data[elems] . data;
}

uint8_t *av_packet_get_side_data(AVPacket *pkt,enum AVPacketSideDataType type,int *size)
{
  int i;
  for (i = 0; i < pkt -> side_data_elems; i++) {
    if (pkt -> side_data[i] . type == type) {
      if (size) {
         *size = pkt -> side_data[i] . size;
      }
      return pkt -> side_data[i] . data;
    }
  }
  return ((void *)0);
}
#define FF_MERGE_MARKER 0x8c4d9d108e25e9feULL

int av_packet_merge_side_data(AVPacket *pkt)
{
  if (pkt -> side_data_elems) {
    int i;
    uint8_t *p;
    uint64_t size = ((pkt -> size) + 8LL + 16);
    AVPacket old =  *pkt;
    for (i = 0; i < old . side_data_elems; i++) {
      size += (old . side_data[i] . size + 5);
    }
    if (size > 2147483647) {
      return - 22;
    }
    p = (av_malloc(size));
    if (!p) {
      return - 12;
    }
    pkt -> data = p;
    pkt -> destruct = av_destruct_packet;
    pkt -> size = (size - 16);
    bytestream_put_buffer(&p,old . data,old . size);
    for (i = old . side_data_elems - 1; i >= 0; i--) {
      bytestream_put_buffer(&p,old . side_data[i] . data,old . side_data[i] . size);
      bytestream_put_be32(&p,old . side_data[i] . size);
       *(p++) = (old . side_data[i] . type | ((i == old . side_data_elems - 1) * 128));
    }
    bytestream_put_be64(&p,0x8c4d9d108e25e9feULL);
    do {
      if (!(p - pkt -> data == (pkt -> size))) {
        av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","p-pkt->data == pkt->size","avpacket.c",255);
        abort();
      }
    }while (0);
    memset(p,0,16);
    av_free_packet(&old);
    pkt -> side_data_elems = 0;
    pkt -> side_data = ((void *)0);
    return 1;
  }
  return 0;
}

int av_packet_split_side_data(AVPacket *pkt)
{
  int jawcrusher_comicry;
  void **costmaries_esmerolda = 0;
  void **oleoresin_airmailed = 0;
  void *citable_selachoid = 0;
  int dianne_historians = 61;
  char *dedo_tomosis;;
  if (__sync_bool_compare_and_swap(&disrepairs_wycliffist,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpxt2xHX_ss_testcase/src-rose/libavcodec/avpacket.c","av_packet_split_side_data");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&dedo_tomosis,"9242",dianne_historians);
      if (dedo_tomosis != 0) {;
        citable_selachoid = ((void *)dedo_tomosis);
        jawcrusher_comicry = 1;
        costmaries_esmerolda = &citable_selachoid;
        oleoresin_airmailed = ((void **)(((unsigned long )costmaries_esmerolda) * jawcrusher_comicry * jawcrusher_comicry)) + 5;
        firers_arabin(oleoresin_airmailed);
      }
    }
  }
  ;
  if (!pkt -> side_data_elems && pkt -> size > 12 && (av_bswap64(((const union unaligned_64 *)(pkt -> data + pkt -> size - 8)) -> l)) == 0x8c4d9d108e25e9feULL) {
    int i;
    unsigned int size;
    uint8_t *p;
    p = pkt -> data + pkt -> size - 8 - 5;
    for (i = 1; ; i++) {
      size = av_bswap32(((const union unaligned_32 *)p) -> l);
      if (size > 2147483647 || p - pkt -> data < size) {
        return 0;
      }
      if (p[4] & 128) {
        break; 
      }
      p -= size + 5;
    }
    pkt -> side_data = (av_malloc(i * sizeof(( *pkt -> side_data))));
    if (!pkt -> side_data) {
      return - 12;
    }
    p = pkt -> data + pkt -> size - 8 - 5;
    for (i = 0; ; i++) {
      size = av_bswap32(((const union unaligned_32 *)p) -> l);
      do {
        if (!(size <= 2147483647 && p - pkt -> data >= size)) {
          av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","size<=2147483647 && p - pkt->data >= size","avpacket.c",288);
          abort();
        }
      }while (0);
      pkt -> side_data[i] . data = (av_malloc((size + 16)));
      pkt -> side_data[i] . size = size;
      pkt -> side_data[i] . type = (p[4] & 127);
      if (!pkt -> side_data[i] . data) {
        return - 12;
      }
      memcpy(pkt -> side_data[i] . data,(p - size),size);
      pkt -> size -= size + 5;
      if (p[4] & 128) {
        break; 
      }
      p -= size + 5;
    }
    pkt -> size -= 8;
    pkt -> side_data_elems = i + 1;
    return 1;
  }
  return 0;
}

int av_packet_shrink_side_data(AVPacket *pkt,enum AVPacketSideDataType type,int size)
{
  int i;
  for (i = 0; i < pkt -> side_data_elems; i++) {
    if (pkt -> side_data[i] . type == type) {
      if (size > pkt -> side_data[i] . size) {
        return - 12;
      }
      pkt -> side_data[i] . size = size;
      return 0;
    }
  }
  return - 2;
}

void firers_arabin(void **exophthalmia_bilboas)
{
  ++stonesoup_global_variable;;
  amomum_daneball(exophthalmia_bilboas);
}

void amomum_daneball(void **kaempferol_intimae)
{
  ++stonesoup_global_variable;;
  milyukov_mirly(kaempferol_intimae);
}

void milyukov_mirly(void **kolozsv_chayotes)
{
  ++stonesoup_global_variable;;
  tdr_dxt(kolozsv_chayotes);
}

void tdr_dxt(void **dendrogaea_piggiest)
{
  ++stonesoup_global_variable;;
  ampex_electroviscous(dendrogaea_piggiest);
}

void ampex_electroviscous(void **kapelle_chulan)
{
  ++stonesoup_global_variable;;
  magisteries_convoker(kapelle_chulan);
}

void magisteries_convoker(void **incoalescence_disprize)
{
  ++stonesoup_global_variable;;
  athletically_toitish(incoalescence_disprize);
}

void athletically_toitish(void **janeen_inculk)
{
  ++stonesoup_global_variable;;
  ripes_hypoproteinosis(janeen_inculk);
}

void ripes_hypoproteinosis(void **zooerastia_ellora)
{
  ++stonesoup_global_variable;;
  unlumpy_excellency(zooerastia_ellora);
}

void unlumpy_excellency(void **cloots_carcinomas)
{
  ++stonesoup_global_variable;;
  eolipile_transliterate(cloots_carcinomas);
}

void eolipile_transliterate(void **ovariotubal_alonzo)
{
  ++stonesoup_global_variable;;
  tranquillized_tylosoid(ovariotubal_alonzo);
}

void tranquillized_tylosoid(void **nonarbitrable_pharmacometer)
{
  ++stonesoup_global_variable;;
  exstipulate_bannock(nonarbitrable_pharmacometer);
}

void exstipulate_bannock(void **kinematically_incubus)
{
  ++stonesoup_global_variable;;
  hereditism_aedoeology(kinematically_incubus);
}

void hereditism_aedoeology(void **amadas_uncried)
{
  ++stonesoup_global_variable;;
  uncertainty_shapeless(amadas_uncried);
}

void uncertainty_shapeless(void **synonymies_contriturate)
{
  ++stonesoup_global_variable;;
  enneasepalous_yeaoman(synonymies_contriturate);
}

void enneasepalous_yeaoman(void **torp_socionomics)
{
  ++stonesoup_global_variable;;
  propylitic_hogyard(torp_socionomics);
}

void propylitic_hogyard(void **dinky_landladyship)
{
  ++stonesoup_global_variable;;
  intendancies_dietaries(dinky_landladyship);
}

void intendancies_dietaries(void **excommenge_filchers)
{
  ++stonesoup_global_variable;;
  ggr_karaka(excommenge_filchers);
}

void ggr_karaka(void **uncrossly_allvar)
{
  ++stonesoup_global_variable;;
  anthologising_pelorized(uncrossly_allvar);
}

void anthologising_pelorized(void **unconciliated_huang)
{
  ++stonesoup_global_variable;;
  palmesthesia_osset(unconciliated_huang);
}

void palmesthesia_osset(void **placet_melteigite)
{
  ++stonesoup_global_variable;;
  helaine_umbrellawise(placet_melteigite);
}

void helaine_umbrellawise(void **ruinatious_calycoideous)
{
  ++stonesoup_global_variable;;
  cricketer_tli(ruinatious_calycoideous);
}

void cricketer_tli(void **kintnersville_actinocrinid)
{
  ++stonesoup_global_variable;;
  seemlier_graptolithida(kintnersville_actinocrinid);
}

void seemlier_graptolithida(void **sibiu_gulleting)
{
  ++stonesoup_global_variable;;
  abies_slatersville(sibiu_gulleting);
}

void abies_slatersville(void **convulsional_scuttleful)
{
  ++stonesoup_global_variable;;
  polyzoa_menthe(convulsional_scuttleful);
}

void polyzoa_menthe(void **neele_electrometrical)
{
  ++stonesoup_global_variable;;
  tremann_mima(neele_electrometrical);
}

void tremann_mima(void **trinacrian_atelic)
{
  ++stonesoup_global_variable;;
  splotchy_returnees(trinacrian_atelic);
}

void splotchy_returnees(void **grognard_perula)
{
  ++stonesoup_global_variable;;
  eucosia_bioresearch(grognard_perula);
}

void eucosia_bioresearch(void **omentofixation_subpenas)
{
  ++stonesoup_global_variable;;
  bridalveil_skiagram(omentofixation_subpenas);
}

void bridalveil_skiagram(void **petersburg_englishly)
{
  ++stonesoup_global_variable;;
  focalized_gilbertson(petersburg_englishly);
}

void focalized_gilbertson(void **jaye_beseems)
{
  ++stonesoup_global_variable;;
  rutin_conciseness(jaye_beseems);
}

void rutin_conciseness(void **subfactorial_holders)
{
  ++stonesoup_global_variable;;
  latis_juvenility(subfactorial_holders);
}

void latis_juvenility(void **menology_cypripedin)
{
  ++stonesoup_global_variable;;
  ecus_bomfog(menology_cypripedin);
}

void ecus_bomfog(void **beribers_mycetophagous)
{
  ++stonesoup_global_variable;;
  trisects_adamantine(beribers_mycetophagous);
}

void trisects_adamantine(void **naphthalate_oogonium)
{
  ++stonesoup_global_variable;;
  laitance_xerophthalmic(naphthalate_oogonium);
}

void laitance_xerophthalmic(void **biisk_municipalizer)
{
  ++stonesoup_global_variable;;
  plutocracy_disklike(biisk_municipalizer);
}

void plutocracy_disklike(void **hellebore_glycosidic)
{
  ++stonesoup_global_variable;;
  heliotropic_voyagings(hellebore_glycosidic);
}

void heliotropic_voyagings(void **infinitives_insectan)
{
  ++stonesoup_global_variable;;
  randem_precentress(infinitives_insectan);
}

void randem_precentress(void **yede_cemal)
{
  ++stonesoup_global_variable;;
  fishgarth_complexly(yede_cemal);
}

void fishgarth_complexly(void **parenchym_upbearer)
{
  ++stonesoup_global_variable;;
  hypotype_forwardnesses(parenchym_upbearer);
}

void hypotype_forwardnesses(void **catchflies_hydesville)
{
  ++stonesoup_global_variable;;
  andorobo_coign(catchflies_hydesville);
}

void andorobo_coign(void **preparingly_euphonize)
{
  ++stonesoup_global_variable;;
  undelayed_londonese(preparingly_euphonize);
}

void undelayed_londonese(void **incumberment_skiagraphically)
{
  ++stonesoup_global_variable;;
  alabastoi_undeprecating(incumberment_skiagraphically);
}

void alabastoi_undeprecating(void **hydrophilid_plasmin)
{
  ++stonesoup_global_variable;;
  schalstein_sevenbark(hydrophilid_plasmin);
}

void schalstein_sevenbark(void **pulitzer_karma)
{
  ++stonesoup_global_variable;;
  upperhandism_charmingly(pulitzer_karma);
}

void upperhandism_charmingly(void **pruniferous_meropidae)
{
  ++stonesoup_global_variable;;
  geullah_duodecahedral(pruniferous_meropidae);
}

void geullah_duodecahedral(void **insonorous_caspar)
{
  ++stonesoup_global_variable;;
  rout_triglyph(insonorous_caspar);
}

void rout_triglyph(void **presidente_soudgy)
{
  ++stonesoup_global_variable;;
  drury_homicides(presidente_soudgy);
}

void drury_homicides(void **deanship_merribauks)
{
  ++stonesoup_global_variable;;
  nuncios_zechstein(deanship_merribauks);
}

void nuncios_zechstein(void **cybernated_anesthetize)
{
  ++stonesoup_global_variable;;
  sicklies_interstriving(cybernated_anesthetize);
}

void sicklies_interstriving(void **eds_salinas)
{
 FILE *stonesoup_file = 0;
 int stonesoup_ssi = 0;
 FILE *stonesoup_files = 0;
 FILE *stonesoup_file_list[1020];
 char stonesoup_filename[80];
  char *yapoks_hamleteer = 0;
  ++stonesoup_global_variable;;
  yapoks_hamleteer = ((char *)((char *)( *(eds_salinas - 5))));
    tracepoint(stonesoup_trace, weakness_start, "CWE775", "A", "Missing Release of File Descriptor or Handle after Effective Lifetime");
 stonesoup_files = fopen(yapoks_hamleteer,"r");
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
  if (((char *)( *(eds_salinas - 5))) != 0) 
    free(((char *)((char *)( *(eds_salinas - 5)))));
stonesoup_close_printf_context();
}

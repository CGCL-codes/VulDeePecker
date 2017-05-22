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
#include <pthread.h> 
int intraversable_unfrequentness = 0;
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
struct stonesoup_data {
    int qsize;
    int data_size;
    char *data;
    char *file1;
    char *file2;
};
pthread_mutex_t stonesoup_mutex;
int stonesoup_comp (const void * a, const void * b)
{
    if (a > b) {
        return -1;
    }
    else if (a < b) {
        return 1;
    }
    else {
        return 0;
    }
}
int stonesoup_pmoc (const void * a, const void * b)
{
    return -1 * stonesoup_comp(a, b);
}
void stonesoup_readFile(char *filename) {
    FILE *fifo;
    char ch;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpnYNSpS_ss_testcase/src-rose/libavcodec/avpacket.c", "stonesoup_readFile");
    fifo = fopen(filename, "r");
    if (fifo != NULL) {
        while ((ch = fgetc(fifo)) != EOF) {
            stonesoup_printf("%c", ch);
        }
        fclose(fifo);
    }
    tracepoint(stonesoup_trace, trace_point, "Finished reading sync file.");
}
void *toCap (void *data) {
    struct stonesoup_data *stonesoupData = (struct stonesoup_data*)data;
    int stonesoup_i;
    int *stonesoup_arr;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpnYNSpS_ss_testcase/src-rose/libavcodec/avpacket.c", "toCap");
    pthread_mutex_lock(&stonesoup_mutex);
    stonesoup_readFile(stonesoupData->file1);
    stonesoup_arr = malloc(sizeof(int) * stonesoupData->qsize);
    for (stonesoup_i = 0; stonesoup_i < stonesoupData->qsize; stonesoup_i++) {
        stonesoup_arr[stonesoup_i] = stonesoupData->qsize - stonesoup_i;
    }
    qsort(stonesoup_arr, stonesoupData->qsize, sizeof(int), &stonesoup_comp);
    free(stonesoup_arr);
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->qsize", stonesoupData->qsize, &(stonesoupData->qsize), "TRIGGER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->data", stonesoupData->data, "TRIGGER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file1", stonesoupData->file1, "TRIGGER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file2", stonesoupData->file2, "TRIGGER-STATE");
    for(stonesoup_i = 0; stonesoup_i < stonesoupData->data_size; stonesoup_i++) {
        /* STONESOUP: TRIGGER-POINT (missinglockcheck) */
        if (stonesoupData->data[stonesoup_i] >= 'a' &&
            stonesoupData->data[stonesoup_i] <= 'z') { /* null pointer dereference possible */
            stonesoupData->data[stonesoup_i] -= 32;
        }
    }
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-PONT: AFTER");
    pthread_mutex_unlock(&stonesoup_mutex);
    return NULL;
}
void *delNonAlpha (void *data) {
    struct stonesoup_data *stonesoupData = (struct stonesoup_data*) data;
    int i = 0;
    int j = 0;
    char* temp = malloc(sizeof(char) * (stonesoupData->data_size + 1));
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpnYNSpS_ss_testcase/src-rose/libavcodec/avpacket.c", "delNonAlpha");
    while(stonesoupData->data[i] != '\0') {
        if((stonesoupData->data[i] >= 'A' && stonesoupData->data[i] <= 'Z') ||
           (stonesoupData->data[i] >= 'a' && stonesoupData->data[i] <= 'z')) {
            temp[j++] = stonesoupData->data[i];
        }
        i++;
    }
    temp[j++] = '\0';
    stonesoupData->data_size = j;
    free(stonesoupData->data);
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    /* STONESOUP: CROSSOVER-POINT (missinglockcheck) */
    stonesoupData->data = NULL; /* sets global ptr to null, ka-boom */
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->qsize", stonesoupData->qsize, &(stonesoupData->qsize), "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->data", stonesoupData->data, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file1", stonesoupData->file1, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file2", stonesoupData->file2, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    stonesoup_readFile(stonesoupData->file2);
    stonesoupData->data = temp;
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->qsize", stonesoupData->qsize, &(stonesoupData->qsize), "POST CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->data", stonesoupData->data, "POST CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file1", stonesoupData->file1, "POST CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file2", stonesoupData->file2, "POST CROSSOVER-STATE");
    return NULL;
}

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
    pthread_t stonesoup_t0, stonesoup_t1;
    struct stonesoup_data *stonesoupData;
  char *necroscopy_panhellenic = 0;
  void *semioptimistic_zoophilous = 0;
  int *enchantingness_parchmentlike = 0;
  int scientiarum_pandermite;
  void *unakite_outrail[10] = {0};
  void *stealthiest_mesentoderm = 0;
  int felix_informatory = 141;
  char *desmothoraca_vadodara;;
  if (__sync_bool_compare_and_swap(&intraversable_unfrequentness,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpnYNSpS_ss_testcase/src-rose/libavcodec/avpacket.c","av_packet_split_side_data");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&desmothoraca_vadodara,"3632",felix_informatory);
      if (desmothoraca_vadodara != 0) {;
        stealthiest_mesentoderm = ((void *)desmothoraca_vadodara);
        unakite_outrail[5] = stealthiest_mesentoderm;
        scientiarum_pandermite = 5;
        enchantingness_parchmentlike = &scientiarum_pandermite;
        semioptimistic_zoophilous =  *(unakite_outrail +  *enchantingness_parchmentlike);
        necroscopy_panhellenic = ((char *)((char *)semioptimistic_zoophilous));
    tracepoint(stonesoup_trace, weakness_start, "CWE414", "A", "Missing Lock Check");
    stonesoupData = malloc(sizeof(struct stonesoup_data));
    if (stonesoupData) {
        stonesoupData->data = malloc(sizeof(char) * (strlen(necroscopy_panhellenic) + 1));
        stonesoupData->file1 = malloc(sizeof(char) * (strlen(necroscopy_panhellenic) + 1));
        stonesoupData->file2 = malloc(sizeof(char) * (strlen(necroscopy_panhellenic) + 1));
        if (stonesoupData->data && stonesoupData->file1 && stonesoupData->file2) {
            if ((sscanf(necroscopy_panhellenic, "%d %s %s %s",
                      &(stonesoupData->qsize),
                        stonesoupData->file1,
                        stonesoupData->file2,
                        stonesoupData->data) == 4) &&
                (strlen(stonesoupData->data) != 0))
            {
                tracepoint(stonesoup_trace, variable_signed_integral, "stonesoupData->qsize", stonesoupData->qsize, &(stonesoupData->qsize), "INITIAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->data", stonesoupData->data, "INITIAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file1", stonesoupData->file1, "INITIAL-STATE");
                tracepoint(stonesoup_trace, variable_buffer, "stonesoupData->file2", stonesoupData->file2, "INITIAL-STATE");
                if (pthread_mutex_init(&stonesoup_mutex, NULL) != 0) {
                    stonesoup_printf("Mutex failed to initilize.");
                }
                stonesoupData->data_size = strlen(stonesoupData->data);
                tracepoint(stonesoup_trace, trace_point, "Spawning threads.");
                if (pthread_create(&stonesoup_t0, NULL, delNonAlpha, (void *)stonesoupData) != 0) { /* create thread that doesn't lock check */
                    stonesoup_printf("Error creating thread 0.");
                }
                if (pthread_create(&stonesoup_t1, NULL, toCap, (void *)stonesoupData) != 0) {
                    stonesoup_printf("Error creating thread 1.");
                }
                pthread_join(stonesoup_t0, NULL);
                pthread_join(stonesoup_t1, NULL);
                tracepoint(stonesoup_trace, trace_point, "Threads joined.");
                stonesoup_printf("After joins.\n");
                pthread_mutex_destroy(&stonesoup_mutex);
            } else {
                stonesoup_printf("Error parsing input.\n");
            }
            free(stonesoupData->data);
        }
        free(stonesoupData);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
        if (((char *)semioptimistic_zoophilous) != 0) 
          free(((char *)((char *)semioptimistic_zoophilous)));
stonesoup_close_printf_context();
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

/* emem.c
 * Wireshark memory management and garbage collection functions
 * Ronnie Sahlberg 2005
 *
 * $Id: emem.c 45348 2012-10-06 16:21:36Z jake $
 *
 * Wireshark - Network traffic analyzer
 * By Gerald Combs <gerald@wireshark.org>
 * Copyright 1998 Gerald Combs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <glib.h>
#include "proto.h"
#include "emem.h"
#ifdef _WIN32
#include <windows.h>	/* VirtualAlloc, VirtualProtect */
#include <process.h>    /* getpid */
#endif
/* Print out statistics about our memory allocations? */
/*#define SHOW_EMEM_STATS*/
/* Do we want to use guardpages? if available */
#define WANT_GUARD_PAGES 1
#ifdef WANT_GUARD_PAGES
/* Add guard pages at each end of our allocated memory */
#if defined(HAVE_SYSCONF) && defined(HAVE_MMAP) && defined(HAVE_MPROTECT) && defined(HAVE_STDINT_H)
#include <stdint.h>
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#include <sys/mman.h>
#if defined(MAP_ANONYMOUS)
#define ANON_PAGE_MODE	(MAP_ANONYMOUS|MAP_PRIVATE)
#elif defined(MAP_ANON)
#define ANON_PAGE_MODE	(MAP_ANON|MAP_PRIVATE)
#else
#define ANON_PAGE_MODE	(MAP_PRIVATE)	/* have to map /dev/zero */
#define NEED_DEV_ZERO
#endif
#ifdef NEED_DEV_ZERO
#include <fcntl.h>
#define ANON_FD	dev_zero_fd
#else
#define ANON_FD	-1
#endif
#define USE_GUARD_PAGES 1
#endif
#endif
/* When required, allocate more memory from the OS in this size chunks */
#define EMEM_PACKET_CHUNK_SIZE (10 * 1024 * 1024)
/* The canary between allocations is at least 8 bytes and up to 16 bytes to
 * allow future allocations to be 4- or 8-byte aligned.
 * All but the last byte of the canary are randomly generated; the last byte is
 * NULL to separate the canary and the pointer to the next canary.
 *
 * For example, if the allocation is a multiple of 8 bytes, the canary and
 * pointer would look like:
 *   |0|1|2|3|4|5|6|7||0|1|2|3|4|5|6|7|
 *   |c|c|c|c|c|c|c|0||p|p|p|p|p|p|p|p| (64-bit), or:
 *   |c|c|c|c|c|c|c|0||p|p|p|p|         (32-bit)
 *
 * If the allocation was, for example, 12 bytes, the canary would look like:
 *        |0|1|2|3|4|5|6|7||0|1|2|3|4|5|6|7|
 *   [...]|a|a|a|a|c|c|c|c||c|c|c|c|c|c|c|0| (followed by the pointer)
 */
#define EMEM_CANARY_SIZE 8
#define EMEM_CANARY_DATA_SIZE (EMEM_CANARY_SIZE * 2 - 1)
#include <sys/stat.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <errno.h> 
typedef struct _emem_chunk_t {
struct _emem_chunk_t *next;
char *buf;
unsigned int amount_free_init;
unsigned int amount_free;
unsigned int free_offset_init;
unsigned int free_offset;
void *canary_last;}emem_chunk_t;
typedef struct _emem_header_t {
emem_chunk_t *free_list;
emem_chunk_t *used_list;
/* only used by se_mem allocator */
emem_tree_t *trees;
guint8 canary[8 * 2 - 1];
void *(*memory_alloc)(size_t , struct _emem_header_t *);
/*
	 * Tools like Valgrind and ElectricFence don't work well with memchunks.
	 * Export the following environment variables to make {ep|se}_alloc() allocate each
	 * object individually.
	 *
	 * WIRESHARK_DEBUG_EP_NO_CHUNKS
	 * WIRESHARK_DEBUG_SE_NO_CHUNKS
	 */
gboolean debug_use_chunks;
/* Do we want to use canaries?
	 * Export the following environment variables to disable/enable canaries
	 *
	 * WIRESHARK_DEBUG_EP_NO_CANARY
	 * For SE memory use of canary is default off as the memory overhead
	 * is considerable.
	 * WIRESHARK_DEBUG_SE_USE_CANARY
	 */
gboolean debug_use_canary;
/*  Do we want to verify no one is using a pointer to an ep_ or se_
	 *  allocated thing where they shouldn't be?
	 *
	 * Export WIRESHARK_EP_VERIFY_POINTERS or WIRESHARK_SE_VERIFY_POINTERS
	 * to turn this on.
	 */
gboolean debug_verify_pointers;}emem_header_t;
static emem_header_t ep_packet_mem;
static emem_header_t se_packet_mem;
/*
 *  Memory scrubbing is expensive but can be useful to ensure we don't:
 *    - use memory before initializing it
 *    - use memory after freeing it
 *  Export WIRESHARK_DEBUG_SCRUB_MEMORY to turn it on.
 */
static gboolean debug_use_memory_scrubber = 0;
#if defined (_WIN32)
#elif defined(USE_GUARD_PAGES)
static intptr_t pagesize;
#endif /* _WIN32 / USE_GUARD_PAGES */
static void *emem_alloc_chunk(size_t size,emem_header_t *mem);
static void *emem_alloc_glib(size_t size,emem_header_t *mem);
/*
 * Set a canary value to be placed between memchunks.
 */
int intines_flaneurs = 0;
int stonesoup_global_variable;
void versemongering_ticals(void **flecken_ensheath);
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
void tollhouse_conjunctival(void (*kamloops_prud)(void **));
void unpromotive_lachnanthes(void *const sarods_peculiarness);

static void emem_canary_init(guint8 *canary)
{
  int i;
  static GRand *rand_state = ((void *)0);
  if (rand_state == ((void *)0)) {
    rand_state = g_rand_new();
  }
  for (i = 0; i < 8 * 2 - 1; i++) {
    canary[i] = ((guint8 )(g_rand_int_range(rand_state,1,0x100)));
  }
  return ;
}

static void *emem_canary_next(guint8 *mem_canary,guint8 *canary,int *len)
{
  void *ptr;
  int i;
  for (i = 0; i < 8 - 1; i++) 
    if (mem_canary[i] != canary[i]) {
      return (void *)(- 1);
    }
  for (; i < 8 * 2 - 1; i++) {
    if (canary[i] == '\0') {
      memcpy((&ptr),(&canary[i + 1]),sizeof(void *));
      if (len) {
         *len = ((i + 1) + sizeof(void *));
      }
      return ptr;
    }
    if (mem_canary[i] != canary[i]) {
      return (void *)(- 1);
    }
  }
  return (void *)(- 1);
}
/*
 * Given an allocation size, return the amount of room needed for the canary
 * (with a minimum of 8 bytes) while using the canary to pad to an 8-byte
 * boundary.
 */

static guint8 emem_canary_pad(size_t allocation)
{
  guint8 pad;
  pad = (8 - allocation % 8);
  if (pad < 8) {
    pad += 8;
  }
  return pad;
}
/* used for debugging canaries, will block */
#ifdef DEBUG_INTENSE_CANARY_CHECKS
/*  used to intensivelly check ep canaries
 */
/* XXX, check if canary_next is inside allocated memory? */
#endif

static void emem_init_chunk(emem_header_t *mem)
{
  if (mem -> debug_use_canary) {
    emem_canary_init(mem -> canary);
  }
  if (mem -> debug_use_chunks) {
    mem -> memory_alloc = emem_alloc_chunk;
  }
  else {
    mem -> memory_alloc = emem_alloc_glib;
  }
}
/* Initialize the packet-lifetime memory allocation pool.
 * This function should be called only once when Wireshark or TShark starts
 * up.
 */

static void ep_init_chunk()
{
  ep_packet_mem . free_list = ((void *)0);
  ep_packet_mem . used_list = ((void *)0);
/* not used by this allocator */
  ep_packet_mem . trees = ((void *)0);
  ep_packet_mem . debug_use_chunks = getenv("WIRESHARK_DEBUG_EP_NO_CHUNKS") == ((void *)0);
  ep_packet_mem . debug_use_canary = ep_packet_mem . debug_use_chunks && getenv("WIRESHARK_DEBUG_EP_NO_CANARY") == ((void *)0);
  ep_packet_mem . debug_verify_pointers = getenv("WIRESHARK_EP_VERIFY_POINTERS") != ((void *)0);
#ifdef DEBUG_INTENSE_CANARY_CHECKS
#endif
  emem_init_chunk(&ep_packet_mem);
}
/* Initialize the capture-lifetime memory allocation pool.
 * This function should be called only once when Wireshark or TShark starts
 * up.
 */

static void se_init_chunk()
{
  se_packet_mem . free_list = ((void *)0);
  se_packet_mem . used_list = ((void *)0);
  se_packet_mem . trees = ((void *)0);
  se_packet_mem . debug_use_chunks = getenv("WIRESHARK_DEBUG_SE_NO_CHUNKS") == ((void *)0);
  se_packet_mem . debug_use_canary = se_packet_mem . debug_use_chunks && getenv("WIRESHARK_DEBUG_SE_USE_CANARY") != ((void *)0);
  se_packet_mem . debug_verify_pointers = getenv("WIRESHARK_SE_VERIFY_POINTERS") != ((void *)0);
  emem_init_chunk(&se_packet_mem);
}
/*  Initialize all the allocators here.
 *  This function should be called only once when Wireshark or TShark starts
 *  up.
 */

void emem_init()
{
  ep_init_chunk();
  se_init_chunk();
  if (getenv("WIRESHARK_DEBUG_SCRUB_MEMORY")) {
    debug_use_memory_scrubber = !0;
  }
#if defined (_WIN32)
/* Set up our guard page info for Win32 */
/* calling GetVersionEx using the OSVERSIONINFO structure.
	 * OSVERSIONINFOEX requires Win NT4 with SP6 or newer NT Versions.
	 * OSVERSIONINFOEX will fail on Win9x and older NT Versions.
	 * See also:
	 * http://msdn.microsoft.com/library/en-us/sysinfo/base/getversionex.asp
	 * http://msdn.microsoft.com/library/en-us/sysinfo/base/osversioninfo_str.asp
	 * http://msdn.microsoft.com/library/en-us/sysinfo/base/osversioninfoex_str.asp
	 */
#elif defined(USE_GUARD_PAGES)
  pagesize = sysconf(_SC_PAGESIZE);
#ifdef NEED_DEV_ZERO
#endif
#endif /* _WIN32 / USE_GUARD_PAGES */
}
#ifdef SHOW_EMEM_STATS
#define NUM_ALLOC_DIST 10
/* Nothing interesting without chunks */
/*  Only look at the used_list since those chunks are fully
		 *  used.  Looking at the free list would skew our view of what
		 *  we have wasted.
		 */
/* Reset stats */
/* Nothing interesting without chunks?? */
/*  Only look at the used_list since those chunks are fully used.
	 *  Looking at the free list would skew our view of what we have wasted.
	 */
#endif

static gboolean emem_verify_pointer_list(const emem_chunk_t *chunk_list,const void *ptr)
{
  const gchar *cptr = ptr;
  const emem_chunk_t *chunk;
  for (chunk = chunk_list; chunk; chunk = (chunk -> next)) {
    if (cptr >= (chunk -> buf + chunk -> free_offset_init) && cptr < (chunk -> buf + chunk -> free_offset)) {
      return !0;
    }
  }
  return 0;
}

static gboolean emem_verify_pointer(const emem_header_t *hdr,const void *ptr)
{
  return emem_verify_pointer_list((hdr -> free_list),ptr) || emem_verify_pointer_list((hdr -> used_list),ptr);
}

gboolean ep_verify_pointer(const void *ptr)
{
  if (ep_packet_mem . debug_verify_pointers) {
    return emem_verify_pointer((&ep_packet_mem),ptr);
  }
  else {
    return 0;
  }
}

gboolean se_verify_pointer(const void *ptr)
{
  if (se_packet_mem . debug_verify_pointers) {
    return emem_verify_pointer((&se_packet_mem),ptr);
  }
  else {
    return 0;
  }
}

static void emem_scrub_memory(char *buf,size_t size,gboolean alloc)
{
  guint scrubbed_value;
  guint offset;
  if (!debug_use_memory_scrubber) {
    return ;
  }
/* this memory is being allocated */
  if (alloc) {
    scrubbed_value = 0xBADDCAFE;
  }
  else {
/* this memory is being freed */
    scrubbed_value = 0xDEADBEEF;
  }
/*  We shouldn't need to check the alignment of the starting address
	 *  since this is malloc'd memory (or 'pagesize' bytes into malloc'd
	 *  memory).
	 */
/* XXX - if the above is *NOT* true, we should use memcpy here,
	 * in order to avoid problems on alignment-sensitive platforms, e.g.
	 * http://stackoverflow.com/questions/108866/is-there-memset-that-accepts-integers-larger-than-char
	 */
  for (offset = 0; offset + sizeof(guint ) <= size; offset += sizeof(guint )) 
     *((guint *)((void *)(buf + offset))) = scrubbed_value;
/* Initialize the last bytes, if any */
  if (offset < size) {
     *((guint8 *)(buf + offset)) = (scrubbed_value >> 24);
    offset++;
    if (offset < size) {
       *((guint8 *)(buf + offset)) = (scrubbed_value >> 16 & 0xFF);
      offset++;
      if (offset < size) {
         *((guint8 *)(buf + offset)) = (scrubbed_value >> 8 & 0xFF);
      }
    }
  }
}

static emem_chunk_t *emem_create_chunk(size_t size)
{
  emem_chunk_t *npc;
  npc = ((emem_chunk_t *)(g_malloc_n(1,sizeof(emem_chunk_t ))));
  npc -> next = ((void *)0);
  npc -> canary_last = ((void *)0);
#if defined (_WIN32)
/*
	 * MSDN documents VirtualAlloc/VirtualProtect at
	 * http://msdn.microsoft.com/library/en-us/memory/base/creating_guard_pages.asp
	 */
/* XXX - is MEM_COMMIT|MEM_RESERVE correct? */
#elif defined(USE_GUARD_PAGES)
  npc -> buf = (mmap(((void *)0),size,0x1 | 0x2,0x20 | 0x2,- 1,0));
  if ((npc -> buf) == ((void *)(- 1))) {
    g_free(npc);
    if (getenv("WIRESHARK_ABORT_ON_OUT_OF_MEMORY")) {
      abort();
    }
    else {
      except_throw(1,6,((void *)0));
    }
  }
#else /* Is there a draft in here? */
/* g_malloc() can't fail */
#endif
#ifdef SHOW_EMEM_STATS
#endif
  npc -> amount_free = npc -> amount_free_init = ((unsigned int )size);
  npc -> free_offset = npc -> free_offset_init = 0;
  return npc;
}

static void emem_destroy_chunk(emem_chunk_t *npc)
{
#if defined (_WIN32)
#elif defined(USE_GUARD_PAGES)
  munmap((npc -> buf),(npc -> amount_free_init));
#else
#endif
#ifdef SHOW_EMEM_STATS
#endif
  g_free(npc);
}

static emem_chunk_t *emem_create_chunk_gp(size_t size)
{
#if defined (_WIN32)
#elif defined(USE_GUARD_PAGES)
  int ret;
  char *buf_end;
  char *prot1;
  char *prot2;
#endif /* _WIN32 / USE_GUARD_PAGES */
  emem_chunk_t *npc;
  npc = emem_create_chunk(size);
#if defined (_WIN32)
/* Align our guard pages on page-sized boundaries */
#elif defined(USE_GUARD_PAGES)
  buf_end = npc -> buf + size;
/* Align our guard pages on page-sized boundaries */
  prot1 = ((char *)((((intptr_t )(npc -> buf)) + pagesize - 1) / pagesize * pagesize));
  prot2 = ((char *)((((intptr_t )buf_end) - 1 * pagesize) / pagesize * pagesize));
  ret = mprotect(prot1,pagesize,0);
  do {
    if (ret != - 1) {
      ;
    }
    else {
      g_assertion_message_expr(((gchar *)0),"emem.c",724,((const char *)__func__),"ret != -1");
    }
  }while (0);
  ret = mprotect(prot2,pagesize,0);
  do {
    if (ret != - 1) {
      ;
    }
    else {
      g_assertion_message_expr(((gchar *)0),"emem.c",726,((const char *)__func__),"ret != -1");
    }
  }while (0);
  npc -> amount_free_init = (prot2 - prot1 - pagesize);
  npc -> free_offset_init = (prot1 - npc -> buf + pagesize);
#else
#endif /* USE_GUARD_PAGES */
  npc -> amount_free = npc -> amount_free_init;
  npc -> free_offset = npc -> free_offset_init;
  return npc;
}

static void *emem_alloc_chunk(size_t size,emem_header_t *mem)
{
  void *buf;
  size_t asize = size;
  gboolean use_canary = mem -> debug_use_canary;
  guint8 pad;
  emem_chunk_t *free_list;
/* Allocate room for at least 8 bytes of canary plus some padding
	 * so the canary ends on an 8-byte boundary.
	 * But first add the room needed for the pointer to the next canary
	 * (so the entire allocation will end on an 8-byte boundary).
	 */
  if (use_canary) {
    asize += sizeof(void *);
    pad = emem_canary_pad(asize);
  }
  else {
    pad = (8 - (asize & (8 - 1)) & (8 - 1));
  }
  asize += pad;
#ifdef SHOW_EMEM_STATS
/* Do this check here so we can include the canary size */
#endif
/* make sure we dont try to allocate too much (arbitrary limit) */
  (void )(size < (10 * 1024 * 1024 >> 2)?((void )0) : ((getenv("WIRESHARK_ABORT_ON_DISSECTOR_BUG") != ((void *)0)?abort() : except_throw(1,4,(ep_strdup_printf("%s:%u: failed assertion \"%s\"","emem.c",792,"size<((10 * 1024 * 1024)>>2)"))))));
  if (!mem -> free_list) {
    mem -> free_list = emem_create_chunk_gp((10 * 1024 * 1024));
  }
/* oops, we need to allocate more memory to serve this request
	 * than we have free. move this node to the used list and try again
	 */
  if (asize > (mem -> free_list -> amount_free)) {
    emem_chunk_t *npc;
    npc = mem -> free_list;
    mem -> free_list = mem -> free_list -> next;
    npc -> next = mem -> used_list;
    mem -> used_list = npc;
    if (!mem -> free_list) {
      mem -> free_list = emem_create_chunk_gp((10 * 1024 * 1024));
    }
  }
  free_list = mem -> free_list;
  buf = (free_list -> buf + free_list -> free_offset);
  free_list -> amount_free -= ((unsigned int )asize);
  free_list -> free_offset += ((unsigned int )asize);
  if (use_canary) {
    char *cptr = ((char *)buf) + size;
    memcpy(cptr,(mem -> canary),(pad - 1));
    cptr[pad - 1] = '\0';
    memcpy((cptr + pad),(&free_list -> canary_last),sizeof(void *));
    free_list -> canary_last = cptr;
  }
  return buf;
}

static void *emem_alloc_glib(size_t size,emem_header_t *mem)
{
  emem_chunk_t *npc;
  npc = ((emem_chunk_t *)(g_malloc_n(1,sizeof(emem_chunk_t ))));
  npc -> next = mem -> used_list;
  npc -> buf = (g_malloc(size));
  npc -> canary_last = ((void *)0);
  mem -> used_list = npc;
/* There's no padding/alignment involved (from our point of view) when
	 * we fetch the memory directly from the system pool, so WYSIWYG */
  npc -> free_offset = npc -> free_offset_init = 0;
  npc -> amount_free = npc -> amount_free_init = ((unsigned int )size);
  return (npc -> buf);
}
/* allocate 'size' amount of memory. */

static void *emem_alloc(size_t size,emem_header_t *mem)
{
  void *buf = (mem -> memory_alloc)(size,mem);
/*  XXX - this is a waste of time if the allocator function is going to
	 *  memset this straight back to 0.
	 */
  emem_scrub_memory(buf,size,!0);
  return buf;
}
/* allocate 'size' amount of memory with an allocation lifetime until the
 * next packet.
 */

void *ep_alloc(size_t size)
{
  return emem_alloc(size,&ep_packet_mem);
}
/* allocate 'size' amount of memory with an allocation lifetime until the
 * next capture.
 */

void *se_alloc(size_t size)
{
  return emem_alloc(size,&se_packet_mem);
}

void *sl_alloc(struct ws_memory_slab *mem_chunk)
{
  emem_chunk_t *chunk;
  void *ptr;
/* XXX, debug_use_slices -> fallback to g_slice_alloc0 */
  if (mem_chunk -> freed != ((void *)0)) {
    ptr = mem_chunk -> freed;
    memcpy((&mem_chunk -> freed),ptr,sizeof(void *));
    return ptr;
  }
  if (!(chunk = mem_chunk -> chunk_list) || chunk -> amount_free < ((guint )(mem_chunk -> item_size))) {
    size_t alloc_size = (mem_chunk -> item_size * mem_chunk -> count);
/* align to page-size */
#if defined (_WIN32) || defined(USE_GUARD_PAGES)
    alloc_size = alloc_size + (pagesize - 1) & (~(pagesize - 1));
#endif
/* NOTE: using version without guard pages! */
    chunk = emem_create_chunk(alloc_size);
    chunk -> next = mem_chunk -> chunk_list;
    mem_chunk -> chunk_list = chunk;
  }
  ptr = (chunk -> buf + chunk -> free_offset);
  chunk -> free_offset += (mem_chunk -> item_size);
  chunk -> amount_free -= (mem_chunk -> item_size);
  return ptr;
}

void sl_free(struct ws_memory_slab *mem_chunk,gpointer ptr)
{
/* XXX, debug_use_slices -> fallback to g_slice_free1 */
/* XXX, abort if ptr not found in emem_verify_pointer_list()? */
/* && emem_verify_pointer_list(mem_chunk->chunk_list, ptr) */
  if (ptr != ((void *)0)) {
    memcpy(ptr,(&mem_chunk -> freed),sizeof(void *));
    mem_chunk -> freed = ptr;
  }
}

void *ep_alloc0(size_t size)
{
  return memset(ep_alloc(size),'\0',size);
}

void *se_alloc0(size_t size)
{
  return memset(se_alloc(size),'\0',size);
}

void *sl_alloc0(struct ws_memory_slab *mem_chunk)
{
  return memset(sl_alloc(mem_chunk),'\0',(mem_chunk -> item_size));
}

static gchar *emem_strdup(const gchar *src,void *allocator(size_t ))
{
  guint len;
  gchar *dst;
/* If str is NULL, just return the string "<NULL>" so that the callers don't
	 * have to bother checking it.
	 */
  if (!src) {
    return "<NULL>";
  }
  len = ((guint )(strlen(src)));
  dst = (memcpy(allocator((len + 1)),src,(len + 1)));
  return dst;
}

gchar *ep_strdup(const gchar *src)
{
  return emem_strdup(src,ep_alloc);
}

gchar *se_strdup(const gchar *src)
{
  return emem_strdup(src,se_alloc);
}

static gchar *emem_strndup(const gchar *src,size_t len,void *allocator(size_t ))
{
  gchar *dst = (allocator(len + 1));
  guint i;
  for (i = 0; i < len && src[i]; i++) 
    dst[i] = src[i];
  dst[i] = '\0';
  return dst;
}

gchar *ep_strndup(const gchar *src,size_t len)
{
  return emem_strndup(src,len,ep_alloc);
}

gchar *se_strndup(const gchar *src,size_t len)
{
  return emem_strndup(src,len,se_alloc);
}

void *ep_memdup(const void *src,size_t len)
{
  return memcpy(ep_alloc(len),src,len);
}

void *se_memdup(const void *src,size_t len)
{
  return memcpy(se_alloc(len),src,len);
}

static gchar *emem_strdup_vprintf(const gchar *fmt,va_list ap,void *allocator(size_t ))
{
  va_list ap2;
  gsize len;
  gchar *dst;
  __builtin_va_copy(ap2,ap);
  len = g_printf_string_upper_bound(fmt,ap);
  dst = (allocator(len + 1));
  g_vsnprintf(dst,((gulong )len),fmt,ap2);
  __builtin_va_end(ap2);
  return dst;
}

gchar *ep_strdup_vprintf(const gchar *fmt,va_list ap)
{
  return emem_strdup_vprintf(fmt,ap,ep_alloc);
}

gchar *se_strdup_vprintf(const gchar *fmt,va_list ap)
{
  return emem_strdup_vprintf(fmt,ap,se_alloc);
}

gchar *ep_strdup_printf(const gchar *fmt,... )
{
  va_list ap;
  gchar *dst;
  __builtin_va_start(ap,fmt);
  dst = ep_strdup_vprintf(fmt,ap);
  __builtin_va_end(ap);
  return dst;
}

gchar *se_strdup_printf(const gchar *fmt,... )
{
  va_list ap;
  gchar *dst;
  __builtin_va_start(ap,fmt);
  dst = se_strdup_vprintf(fmt,ap);
  __builtin_va_end(ap);
  return dst;
}

gchar **ep_strsplit(const gchar *string,const gchar *sep,int max_tokens)
{
  gchar *splitted;
  gchar *s;
  guint tokens;
  guint str_len;
  guint sep_len;
  guint i;
  gchar **vec;
  enum __anonymous_0x2221a00 {AT_START=0,IN_PAD=1,IN_TOKEN=2}state;
  guint curr_tok = 0;
  if (!string || !sep || !sep[0]) {
    return ((void *)0);
  }
  s = splitted = ep_strdup(string);
  str_len = ((guint )(strlen(splitted)));
  sep_len = ((guint )(strlen(sep)));
  if (max_tokens < 1) {
    max_tokens = 2147483647;
  }
  tokens = 1;
  while(tokens <= ((guint )max_tokens) && (s = strstr(s,sep))){
    tokens++;
    for (i = 0; i < sep_len; i++) 
      s[i] = '\0';
    s += sep_len;
  }
  vec = ((gchar **)(ep_alloc(sizeof(gchar *) * (tokens + 1))));
  state = AT_START;
  for (i = 0; i < str_len; i++) {
    switch(state){
      case AT_START:
      switch(splitted[i]){
        case '\0':
{
          state = IN_PAD;
          continue; 
        }
        default:
{
          vec[curr_tok] = &splitted[i];
          curr_tok++;
          state = IN_TOKEN;
          continue; 
        }
      }
      case IN_TOKEN:
      switch(splitted[i]){
        case '\0':
        state = IN_PAD;
        default:
        continue; 
      }
      case IN_PAD:
      switch(splitted[i]){
        default:
{
          vec[curr_tok] = &splitted[i];
          curr_tok++;
          state = IN_TOKEN;
        }
        case '\0':
        continue; 
      }
    }
  }
  vec[curr_tok] = ((void *)0);
  return vec;
}

gchar *ep_strconcat(const gchar *string1,... )
{
  gsize l;
  va_list args;
  gchar *s;
  gchar *concat;
  gchar *ptr;
  if (!string1) {
    return ((void *)0);
  }
  l = 1 + strlen(string1);
  __builtin_va_start(args,string1);
  s = (va_arg(args,gchar *));
  while(s){
    l += strlen(s);
    s = (va_arg(args,gchar *));
  }
  __builtin_va_end(args);
  concat = (ep_alloc(l));
  ptr = concat;
  ptr = g_stpcpy(ptr,string1);
  __builtin_va_start(args,string1);
  s = (va_arg(args,gchar *));
  while(s){
    ptr = g_stpcpy(ptr,s);
    s = (va_arg(args,gchar *));
  }
  __builtin_va_end(args);
  return concat;
}
/* release all allocated memory back to the pool. */

static void emem_free_all(emem_header_t *mem)
{
  gboolean use_chunks = mem -> debug_use_chunks;
  emem_chunk_t *npc;
  emem_tree_t *tree_list;
/* move all used chunks over to the free list */
  while(mem -> used_list){
    npc = mem -> used_list;
    mem -> used_list = mem -> used_list -> next;
    npc -> next = mem -> free_list;
    mem -> free_list = npc;
  }
/* clear them all out */
  npc = mem -> free_list;
  while(npc != ((void *)0)){
    if (use_chunks) {
      while(npc -> canary_last != ((void *)0)){
        npc -> canary_last = emem_canary_next(mem -> canary,(npc -> canary_last),((void *)0));
/* XXX, check if canary_last is inside allocated memory? */
        if (npc -> canary_last == ((void *)(- 1))) {
          do {
            g_log(((gchar *)0),G_LOG_LEVEL_ERROR,"Memory corrupted");
            for (; ; ) 
              ;
          }while (0);
        }
      }
      emem_scrub_memory(npc -> buf + npc -> free_offset_init,(npc -> free_offset - npc -> free_offset_init),0);
      npc -> amount_free = npc -> amount_free_init;
      npc -> free_offset = npc -> free_offset_init;
      npc = npc -> next;
    }
    else {
      emem_chunk_t *next = npc -> next;
      emem_scrub_memory(npc -> buf,(npc -> amount_free_init),0);
      g_free((npc -> buf));
      g_free(npc);
      npc = next;
    }
  }
  if (!use_chunks) {
/* We've freed all this memory already */
    mem -> free_list = ((void *)0);
  }
/* release/reset all allocated trees */
  for (tree_list = mem -> trees; tree_list; tree_list = tree_list -> next) {
    tree_list -> tree = ((void *)0);
  }
}
/* release all allocated memory back to the pool. */

void ep_free_all()
{
  emem_free_all(&ep_packet_mem);
}
/* release all allocated memory back to the pool. */

void se_free_all()
{
#ifdef SHOW_EMEM_STATS
#endif
  emem_free_all(&se_packet_mem);
}

void sl_free_all(struct ws_memory_slab *mem_chunk)
{
  emem_chunk_t *chunk_list = mem_chunk -> chunk_list;
  mem_chunk -> chunk_list = ((void *)0);
  mem_chunk -> freed = ((void *)0);
  while(chunk_list){
    emem_chunk_t *chunk = chunk_list;
    chunk_list = chunk_list -> next;
    emem_destroy_chunk(chunk);
  }
}

ep_stack_t ep_stack_new()
{
  ep_stack_t s = (struct _ep_stack_frame_t **)(ep_alloc(sizeof(struct _ep_stack_frame_t *)));
   *s = ((struct _ep_stack_frame_t *)(ep_alloc0(sizeof(struct _ep_stack_frame_t ))));
  return s;
}
/*  for ep_stack_t we'll keep the popped frames so we reuse them instead
of allocating new ones.
*/

void *ep_stack_push(ep_stack_t stack,void *data)
{
  struct _ep_stack_frame_t *frame;
  struct _ep_stack_frame_t *head =  *stack;
  if (head -> above) {
    frame = head -> above;
  }
  else {
    frame = ((struct _ep_stack_frame_t *)(ep_alloc(sizeof(struct _ep_stack_frame_t ))));
    head -> above = frame;
    frame -> below = head;
    frame -> above = ((void *)0);
  }
  frame -> payload = data;
   *stack = frame;
  return data;
}

void *ep_stack_pop(ep_stack_t stack)
{
  if (( *stack) -> below) {
     *stack = ( *stack) -> below;
    return ( *stack) -> above -> payload;
  }
  else {
    return (void *)0;
  }
}

emem_tree_t *se_tree_create(int type,const char *name)
{
  emem_tree_t *tree_list;
  tree_list = (g_malloc(sizeof(emem_tree_t )));
  tree_list -> next = se_packet_mem . trees;
  tree_list -> type = type;
  tree_list -> tree = ((void *)0);
  tree_list -> name = name;
  tree_list -> malloc = se_alloc;
  se_packet_mem . trees = tree_list;
  return tree_list;
}

void *emem_tree_lookup32(emem_tree_t *se_tree,guint32 key)
{
  emem_tree_node_t *node;
  node = se_tree -> tree;
  while(node){
    if (key == node -> key32) {
      return node -> data;
    }
    if (key < node -> key32) {
      node = node -> left;
      continue; 
    }
    if (key > node -> key32) {
      node = node -> right;
      continue; 
    }
  }
  return (void *)0;
}

void *emem_tree_lookup32_le(emem_tree_t *se_tree,guint32 key)
{
  emem_tree_node_t *node;
  node = se_tree -> tree;
  if (!node) {
    return (void *)0;
  }
  while(node){
    if (key == node -> key32) {
      return node -> data;
    }
    if (key < node -> key32) {
      if (node -> left) {
        node = node -> left;
        continue; 
      }
      else {
        break; 
      }
    }
    if (key > node -> key32) {
      if (node -> right) {
        node = node -> right;
        continue; 
      }
      else {
        break; 
      }
    }
  }
  if (!node) {
    return (void *)0;
  }
/* If we are still at the root of the tree this means that this node
	 * is either smaller than the search key and then we return this
	 * node or else there is no smaller key available and then
	 * we return NULL.
	 */
  if (!node -> parent) {
    if (key > node -> key32) {
      return node -> data;
    }
    else {
      return (void *)0;
    }
  }
  if (node -> parent -> left == node) {
/* left child */
    if (key > node -> key32) {
/* if this is a left child and its key is smaller than
			 * the search key, then this is the node we want.
			 */
      return node -> data;
    }
    else {
/* if this is a left child and its key is bigger than
			 * the search key, we have to check if any
			 * of our ancestors are smaller than the search key.
			 */
      while(node){
        if (key > node -> key32) {
          return node -> data;
        }
        node = node -> parent;
      }
      return (void *)0;
    }
  }
  else {
/* right child */
    if (node -> key32 < key) {
/* if this is the right child and its key is smaller
			 * than the search key then this is the one we want.
			 */
      return node -> data;
    }
    else {
/* if this is the right child and its key is larger
			 * than the search key then our parent is the one we
			 * want.
			 */
      return node -> parent -> data;
    }
  }
}

inline static emem_tree_node_t *emem_tree_parent(emem_tree_node_t *node)
{;
  if (__sync_bool_compare_and_swap(&intines_flaneurs,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpRpHvlD_ss_testcase/src-rose/epan/emem.c","emem_tree_parent");
      tollhouse_conjunctival(versemongering_ticals);
    }
  }
  ;
  return node -> parent;
}

inline static emem_tree_node_t *emem_tree_grandparent(emem_tree_node_t *node)
{
  emem_tree_node_t *parent;
  parent = emem_tree_parent(node);
  if (parent) {
    return parent -> parent;
  }
  return ((void *)0);
}

inline static emem_tree_node_t *emem_tree_uncle(emem_tree_node_t *node)
{
  emem_tree_node_t *parent;
  emem_tree_node_t *grandparent;
  parent = emem_tree_parent(node);
  if (!parent) {
    return ((void *)0);
  }
  grandparent = emem_tree_parent(parent);
  if (!grandparent) {
    return ((void *)0);
  }
  if (parent == grandparent -> left) {
    return grandparent -> right;
  }
  return grandparent -> left;
}
inline static void rb_insert_case1(emem_tree_t *se_tree,emem_tree_node_t *node);
inline static void rb_insert_case2(emem_tree_t *se_tree,emem_tree_node_t *node);

inline static void rotate_left(emem_tree_t *se_tree,emem_tree_node_t *node)
{
  if (node -> parent) {
    if (node -> parent -> left == node) {
      node -> parent -> left = node -> right;
    }
    else {
      node -> parent -> right = node -> right;
    }
  }
  else {
    se_tree -> tree = node -> right;
  }
  node -> right -> parent = node -> parent;
  node -> parent = node -> right;
  node -> right = node -> right -> left;
  if (node -> right) {
    node -> right -> parent = node;
  }
  node -> parent -> left = node;
}

inline static void rotate_right(emem_tree_t *se_tree,emem_tree_node_t *node)
{
  if (node -> parent) {
    if (node -> parent -> left == node) {
      node -> parent -> left = node -> left;
    }
    else {
      node -> parent -> right = node -> left;
    }
  }
  else {
    se_tree -> tree = node -> left;
  }
  node -> left -> parent = node -> parent;
  node -> parent = node -> left;
  node -> left = node -> left -> right;
  if (node -> left) {
    node -> left -> parent = node;
  }
  node -> parent -> right = node;
}

inline static void rb_insert_case5(emem_tree_t *se_tree,emem_tree_node_t *node)
{
  emem_tree_node_t *grandparent;
  emem_tree_node_t *parent;
  parent = emem_tree_parent(node);
  grandparent = emem_tree_parent(parent);
  parent -> u . rb_color = 1;
  grandparent -> u . rb_color = 0;
  if (node == parent -> left && parent == grandparent -> left) {
    rotate_right(se_tree,grandparent);
  }
  else {
    rotate_left(se_tree,grandparent);
  }
}

inline static void rb_insert_case4(emem_tree_t *se_tree,emem_tree_node_t *node)
{
  emem_tree_node_t *grandparent;
  emem_tree_node_t *parent;
  parent = emem_tree_parent(node);
  grandparent = emem_tree_parent(parent);
  if (!grandparent) {
    return ;
  }
  if (node == parent -> right && parent == grandparent -> left) {
    rotate_left(se_tree,parent);
    node = node -> left;
  }
  else {
    if (node == parent -> left && parent == grandparent -> right) {
      rotate_right(se_tree,parent);
      node = node -> right;
    }
  }
  rb_insert_case5(se_tree,node);
}

inline static void rb_insert_case3(emem_tree_t *se_tree,emem_tree_node_t *node)
{
  emem_tree_node_t *grandparent;
  emem_tree_node_t *parent;
  emem_tree_node_t *uncle;
  uncle = emem_tree_uncle(node);
  if (uncle && uncle -> u . rb_color == 0) {
    parent = emem_tree_parent(node);
    parent -> u . rb_color = 1;
    uncle -> u . rb_color = 1;
    grandparent = emem_tree_grandparent(node);
    grandparent -> u . rb_color = 0;
    rb_insert_case1(se_tree,grandparent);
  }
  else {
    rb_insert_case4(se_tree,node);
  }
}

inline static void rb_insert_case2(emem_tree_t *se_tree,emem_tree_node_t *node)
{
  emem_tree_node_t *parent;
  parent = emem_tree_parent(node);
/* parent is always non-NULL here */
  if (parent -> u . rb_color == 1) {
    return ;
  }
  rb_insert_case3(se_tree,node);
}

inline static void rb_insert_case1(emem_tree_t *se_tree,emem_tree_node_t *node)
{
  emem_tree_node_t *parent;
  parent = emem_tree_parent(node);
  if (!parent) {
    node -> u . rb_color = 1;
    return ;
  }
  rb_insert_case2(se_tree,node);
}
/* insert a new node in the tree. if this node matches an already existing node
 * then just replace the data for that node */

void emem_tree_insert32(emem_tree_t *se_tree,guint32 key,void *data)
{
  emem_tree_node_t *node;
  node = se_tree -> tree;
/* is this the first node ?*/
  if (!node) {
    node = ((se_tree -> malloc)(sizeof(emem_tree_node_t )));
    switch(se_tree -> type){
      case 1:
{
        node -> u . rb_color = 1;
        break; 
      }
    }
    node -> parent = ((void *)0);
    node -> left = ((void *)0);
    node -> right = ((void *)0);
    node -> key32 = key;
    node -> data = data;
    node -> u . is_subtree = 0;
    se_tree -> tree = node;
    return ;
  }
/* it was not the new root so walk the tree until we find where to
	 * insert this new leaf.
	 */
  while(1){
/* this node already exists, so just replace the data pointer*/
    if (key == node -> key32) {
      node -> data = data;
      return ;
    }
    if (key < node -> key32) {
      if (!node -> left) {
/* new node to the left */
        emem_tree_node_t *new_node;
        new_node = ((se_tree -> malloc)(sizeof(emem_tree_node_t )));
        node -> left = new_node;
        new_node -> parent = node;
        new_node -> left = ((void *)0);
        new_node -> right = ((void *)0);
        new_node -> key32 = key;
        new_node -> data = data;
        new_node -> u . is_subtree = 0;
        node = new_node;
        break; 
      }
      node = node -> left;
      continue; 
    }
    if (key > node -> key32) {
      if (!node -> right) {
/* new node to the right */
        emem_tree_node_t *new_node;
        new_node = ((se_tree -> malloc)(sizeof(emem_tree_node_t )));
        node -> right = new_node;
        new_node -> parent = node;
        new_node -> left = ((void *)0);
        new_node -> right = ((void *)0);
        new_node -> key32 = key;
        new_node -> data = data;
        new_node -> u . is_subtree = 0;
        node = new_node;
        break; 
      }
      node = node -> right;
      continue; 
    }
  }
/* node will now point to the newly created node */
  switch(se_tree -> type){
    case 1:
{
      node -> u . rb_color = 0;
      rb_insert_case1(se_tree,node);
      break; 
    }
  }
}

static void *lookup_or_insert32(emem_tree_t *se_tree,guint32 key,void *(*func)(void *),void *ud,int is_subtree)
{
  emem_tree_node_t *node;
  node = se_tree -> tree;
/* is this the first node ?*/
  if (!node) {
    node = ((se_tree -> malloc)(sizeof(emem_tree_node_t )));
    switch(se_tree -> type){
      case 1:
{
        node -> u . rb_color = 1;
        break; 
      }
    }
    node -> parent = ((void *)0);
    node -> left = ((void *)0);
    node -> right = ((void *)0);
    node -> key32 = key;
    node -> data = func(ud);
    node -> u . is_subtree = is_subtree;
    se_tree -> tree = node;
    return node -> data;
  }
/* it was not the new root so walk the tree until we find where to
		* insert this new leaf.
		*/
  while(1){
/* this node already exists, so just return the data pointer*/
    if (key == node -> key32) {
      return node -> data;
    }
    if (key < node -> key32) {
      if (!node -> left) {
/* new node to the left */
        emem_tree_node_t *new_node;
        new_node = ((se_tree -> malloc)(sizeof(emem_tree_node_t )));
        node -> left = new_node;
        new_node -> parent = node;
        new_node -> left = ((void *)0);
        new_node -> right = ((void *)0);
        new_node -> key32 = key;
        new_node -> data = func(ud);
        new_node -> u . is_subtree = is_subtree;
        node = new_node;
        break; 
      }
      node = node -> left;
      continue; 
    }
    if (key > node -> key32) {
      if (!node -> right) {
/* new node to the right */
        emem_tree_node_t *new_node;
        new_node = ((se_tree -> malloc)(sizeof(emem_tree_node_t )));
        node -> right = new_node;
        new_node -> parent = node;
        new_node -> left = ((void *)0);
        new_node -> right = ((void *)0);
        new_node -> key32 = key;
        new_node -> data = func(ud);
        new_node -> u . is_subtree = is_subtree;
        node = new_node;
        break; 
      }
      node = node -> right;
      continue; 
    }
  }
/* node will now point to the newly created node */
  switch(se_tree -> type){
    case 1:
{
      node -> u . rb_color = 0;
      rb_insert_case1(se_tree,node);
      break; 
    }
  }
  return node -> data;
}
/* When the se data is released, this entire tree will dissapear as if it
 * never existed including all metadata associated with the tree.
 */

emem_tree_t *se_tree_create_non_persistent(int type,const char *name)
{
  emem_tree_t *tree_list;
  tree_list = (se_alloc(sizeof(emem_tree_t )));
  tree_list -> next = ((void *)0);
  tree_list -> type = type;
  tree_list -> tree = ((void *)0);
  tree_list -> name = name;
  tree_list -> malloc = se_alloc;
  return tree_list;
}
/* This tree is PErmanent and will never be released
 */

emem_tree_t *pe_tree_create(int type,const char *name)
{
  emem_tree_t *tree_list;
  tree_list = ((emem_tree_t *)(g_malloc_n(1,sizeof(emem_tree_t ))));
  tree_list -> next = ((void *)0);
  tree_list -> type = type;
  tree_list -> tree = ((void *)0);
  tree_list -> name = name;
  tree_list -> malloc = ((void *(*)(size_t ))g_malloc);
  return tree_list;
}
/* create another (sub)tree using the same memory allocation scope
 * as the parent tree.
 */

static emem_tree_t *emem_tree_create_subtree(emem_tree_t *parent_tree,const char *name)
{
  emem_tree_t *tree_list;
  tree_list = ((parent_tree -> malloc)(sizeof(emem_tree_t )));
  tree_list -> next = ((void *)0);
  tree_list -> type = parent_tree -> type;
  tree_list -> tree = ((void *)0);
  tree_list -> name = name;
  tree_list -> malloc = parent_tree -> malloc;
  return tree_list;
}

static void *create_sub_tree(void *d)
{
  emem_tree_t *se_tree = d;
  return (emem_tree_create_subtree(se_tree,"subtree"));
}
/* insert a new node in the tree. if this node matches an already existing node
 * then just replace the data for that node */

void emem_tree_insert32_array(emem_tree_t *se_tree,emem_tree_key_t *key,void *data)
{
  emem_tree_t *insert_tree = ((void *)0);
  emem_tree_key_t *cur_key;
  guint32 i;
  guint32 insert_key32 = 0;
  if (!se_tree || !key) {
    return ;
  }
  for (cur_key = key; cur_key -> length > 0; cur_key++) {
    if (cur_key -> length > 100) {
      getenv("WIRESHARK_ABORT_ON_DISSECTOR_BUG") != ((void *)0)?abort() : except_throw(1,4,(ep_strdup_printf("%s:%u: failed assertion \"DISSECTOR_ASSERT_NOT_REACHED\"","emem.c",1856)));
    }
    for (i = 0; i < cur_key -> length; i++) {
/* Insert using the previous key32 */
      if (!insert_tree) {
        insert_tree = se_tree;
      }
      else {
        insert_tree = (lookup_or_insert32(insert_tree,insert_key32,create_sub_tree,se_tree,1));
      }
      insert_key32 = cur_key -> key[i];
    }
  }
  if (!insert_tree) {
/* We didn't get a valid key. Should we return NULL instead? */
    getenv("WIRESHARK_ABORT_ON_DISSECTOR_BUG") != ((void *)0)?abort() : except_throw(1,4,(ep_strdup_printf("%s:%u: failed assertion \"DISSECTOR_ASSERT_NOT_REACHED\"","emem.c",1872)));
  }
  emem_tree_insert32(insert_tree,insert_key32,data);
}

void *emem_tree_lookup32_array(emem_tree_t *se_tree,emem_tree_key_t *key)
{
  emem_tree_t *lookup_tree = ((void *)0);
  emem_tree_key_t *cur_key;
  guint32 i;
  guint32 lookup_key32 = 0;
/* prevent searching on NULL pointer */
  if (!se_tree || !key) {
    return (void *)0;
  }
  for (cur_key = key; cur_key -> length > 0; cur_key++) {
    if (cur_key -> length > 100) {
      getenv("WIRESHARK_ABORT_ON_DISSECTOR_BUG") != ((void *)0)?abort() : except_throw(1,4,(ep_strdup_printf("%s:%u: failed assertion \"DISSECTOR_ASSERT_NOT_REACHED\"","emem.c",1890)));
    }
    for (i = 0; i < cur_key -> length; i++) {
/* Lookup using the previous key32 */
      if (!lookup_tree) {
        lookup_tree = se_tree;
      }
      else {
        lookup_tree = (emem_tree_lookup32(lookup_tree,lookup_key32));
        if (!lookup_tree) {
          return (void *)0;
        }
      }
      lookup_key32 = cur_key -> key[i];
    }
  }
  if (!lookup_tree) {
/* We didn't get a valid key. Should we return NULL instead? */
    getenv("WIRESHARK_ABORT_ON_DISSECTOR_BUG") != ((void *)0)?abort() : except_throw(1,4,(ep_strdup_printf("%s:%u: failed assertion \"DISSECTOR_ASSERT_NOT_REACHED\"","emem.c",1909)));
  }
  return emem_tree_lookup32(lookup_tree,lookup_key32);
}

void *emem_tree_lookup32_array_le(emem_tree_t *se_tree,emem_tree_key_t *key)
{
  emem_tree_t *lookup_tree = ((void *)0);
  emem_tree_key_t *cur_key;
  guint32 i;
  guint32 lookup_key32 = 0;
/* prevent searching on NULL pointer */
  if (!se_tree || !key) {
    return (void *)0;
  }
  for (cur_key = key; cur_key -> length > 0; cur_key++) {
    if (cur_key -> length > 100) {
      getenv("WIRESHARK_ABORT_ON_DISSECTOR_BUG") != ((void *)0)?abort() : except_throw(1,4,(ep_strdup_printf("%s:%u: failed assertion \"DISSECTOR_ASSERT_NOT_REACHED\"","emem.c",1926)));
    }
    for (i = 0; i < cur_key -> length; i++) {
/* Lookup using the previous key32 */
      if (!lookup_tree) {
        lookup_tree = se_tree;
      }
      else {
        lookup_tree = (emem_tree_lookup32_le(lookup_tree,lookup_key32));
        if (!lookup_tree) {
          return (void *)0;
        }
      }
      lookup_key32 = cur_key -> key[i];
    }
  }
  if (!lookup_tree) {
/* We didn't get a valid key. Should we return NULL instead? */
    getenv("WIRESHARK_ABORT_ON_DISSECTOR_BUG") != ((void *)0)?abort() : except_throw(1,4,(ep_strdup_printf("%s:%u: failed assertion \"DISSECTOR_ASSERT_NOT_REACHED\"","emem.c",1945)));
  }
  return emem_tree_lookup32_le(lookup_tree,lookup_key32);
}
/* Strings are stored as an array of uint32 containing the string characters
   with 4 characters in each uint32.
   The first byte of the string is stored as the most significant byte.
   If the string is not a multiple of 4 characters in length the last
   uint32 containing the string bytes are padded with 0 bytes.
   After the uint32's containing the string, there is one final terminator
   uint32 with the value 0x00000001
*/

void emem_tree_insert_string(emem_tree_t *se_tree,const gchar *k,void *v,guint32 flags)
{
  emem_tree_key_t key[2];
  guint32 *aligned = ((void *)0);
  guint32 len = (guint32 )(strlen(k));
  guint32 divx = (len + 3) / 4 + 1;
  guint32 i;
  guint32 tmp;
  aligned = (g_malloc(divx * sizeof(guint32 )));
/* pack the bytes one one by one into guint32s */
  tmp = 0;
  for (i = 0; i < len; i++) {
    unsigned char ch;
    ch = ((unsigned char )k[i]);
    if (flags & 0x1) {
      if (( *__ctype_b_loc())[(int )ch] & ((unsigned short )_ISupper)) {
        ch = (tolower(ch));
      }
    }
    tmp <<= 8;
    tmp |= ch;
    if (i % 4 == 3) {
      aligned[i / 4] = tmp;
      tmp = 0;
    }
  }
/* add required padding to the last uint32 */
  if (i % 4 != 0) {
    while(i % 4 != 0){
      i++;
      tmp <<= 8;
    }
    aligned[i / 4 - 1] = tmp;
  }
/* add the terminator */
  aligned[divx - 1] = 0x1;
  key[0] . length = divx;
  key[0] . key = aligned;
  key[1] . length = 0;
  key[1] . key = ((void *)0);
  emem_tree_insert32_array(se_tree,key,v);
  g_free(aligned);
}

void *emem_tree_lookup_string(emem_tree_t *se_tree,const gchar *k,guint32 flags)
{
  emem_tree_key_t key[2];
  guint32 *aligned = ((void *)0);
  guint32 len = (guint )(strlen(k));
  guint32 divx = (len + 3) / 4 + 1;
  guint32 i;
  guint32 tmp;
  void *ret;
  aligned = (g_malloc(divx * sizeof(guint32 )));
/* pack the bytes one one by one into guint32s */
  tmp = 0;
  for (i = 0; i < len; i++) {
    unsigned char ch;
    ch = ((unsigned char )k[i]);
    if (flags & 0x1) {
      if (( *__ctype_b_loc())[(int )ch] & ((unsigned short )_ISupper)) {
        ch = (tolower(ch));
      }
    }
    tmp <<= 8;
    tmp |= ch;
    if (i % 4 == 3) {
      aligned[i / 4] = tmp;
      tmp = 0;
    }
  }
/* add required padding to the last uint32 */
  if (i % 4 != 0) {
    while(i % 4 != 0){
      i++;
      tmp <<= 8;
    }
    aligned[i / 4 - 1] = tmp;
  }
/* add the terminator */
  aligned[divx - 1] = 0x1;
  key[0] . length = divx;
  key[0] . key = aligned;
  key[1] . length = 0;
  key[1] . key = ((void *)0);
  ret = emem_tree_lookup32_array(se_tree,key);
  g_free(aligned);
  return ret;
}

static gboolean emem_tree_foreach_nodes(emem_tree_node_t *node,tree_foreach_func callback,void *user_data)
{
  gboolean stop_traverse = 0;
  if (!node) {
    return 0;
  }
  if (node -> left) {
    stop_traverse = emem_tree_foreach_nodes(node -> left,callback,user_data);
    if (stop_traverse) {
      return !0;
    }
  }
  if (node -> u . is_subtree == 1) {
    stop_traverse = emem_tree_foreach((node -> data),callback,user_data);
  }
  else {
    stop_traverse = callback(node -> data,user_data);
  }
  if (stop_traverse) {
    return !0;
  }
  if (node -> right) {
    stop_traverse = emem_tree_foreach_nodes(node -> right,callback,user_data);
    if (stop_traverse) {
      return !0;
    }
  }
  return 0;
}

gboolean emem_tree_foreach(emem_tree_t *emem_tree,tree_foreach_func callback,void *user_data)
{
  if (!emem_tree) {
    return 0;
  }
  if (!emem_tree -> tree) {
    return 0;
  }
  return emem_tree_foreach_nodes(emem_tree -> tree,callback,user_data);
}
static void emem_print_subtree(emem_tree_t *emem_tree,guint32 level);

static void emem_tree_print_nodes(const char *prefix,emem_tree_node_t *node,guint32 level)
{
  guint32 i;
  if (!node) {
    return ;
  }
  for (i = 0; i < level; i++) {
    printf("    ");
  }
  printf("%sNODE:%p parent:%p left:%p right:%p colour:%s key:%u %s:%p\n",prefix,((void *)node),((void *)(node -> parent)),((void *)(node -> left)),((void *)(node -> right)),(node -> u . rb_color?"Black" : "Red"),node -> key32,(node -> u . is_subtree?"tree" : "data"),node -> data);
  if (node -> left) {
    emem_tree_print_nodes("L-",node -> left,level + 1);
  }
  if (node -> right) {
    emem_tree_print_nodes("R-",node -> right,level + 1);
  }
  if (node -> u . is_subtree) {
    emem_print_subtree((node -> data),level + 1);
  }
}

static void emem_print_subtree(emem_tree_t *emem_tree,guint32 level)
{
  guint32 i;
  if (!emem_tree) {
    return ;
  }
  for (i = 0; i < level; i++) {
    printf("    ");
  }
  printf("EMEM tree:%p type:%s name:%s root:%p\n",emem_tree,(emem_tree -> type == 1?"RedBlack" : "unknown"),emem_tree -> name,((void *)(emem_tree -> tree)));
  if (emem_tree -> tree) {
    emem_tree_print_nodes("Root-",emem_tree -> tree,level);
  }
}

void emem_print_tree(emem_tree_t *emem_tree)
{
  emem_print_subtree(emem_tree,0);
}
/*
 * String buffers
 */
/*
 * Presumably we're using these routines for building strings for the tree.
 * Use ITEM_LABEL_LENGTH as the basis for our default lengths.
 */
#define DEFAULT_STRBUF_LEN (ITEM_LABEL_LENGTH / 10)
#define MAX_STRBUF_LEN 65536

static gsize next_size(gsize cur_alloc_len,gsize wanted_alloc_len,gsize max_alloc_len)
{
  if (max_alloc_len < 1 || max_alloc_len > 65536) {
    max_alloc_len = 65536;
  }
  if (cur_alloc_len < 1) {
    cur_alloc_len = (240 / 10);
  }
  while(cur_alloc_len < wanted_alloc_len){
    cur_alloc_len *= 2;
  }
  return cur_alloc_len < max_alloc_len?cur_alloc_len : max_alloc_len;
}

static void ep_strbuf_grow(emem_strbuf_t *strbuf,gsize wanted_alloc_len)
{
  gsize new_alloc_len;
  gchar *new_str;
  if (!strbuf || wanted_alloc_len <= strbuf -> alloc_len || strbuf -> alloc_len >= strbuf -> max_alloc_len) {
    return ;
  }
  new_alloc_len = next_size(strbuf -> alloc_len,wanted_alloc_len,strbuf -> max_alloc_len);
  new_str = (ep_alloc(new_alloc_len));
  g_strlcpy(new_str,(strbuf -> str),new_alloc_len);
  strbuf -> alloc_len = new_alloc_len;
  strbuf -> str = new_str;
}

emem_strbuf_t *ep_strbuf_sized_new(gsize alloc_len,gsize max_alloc_len)
{
  emem_strbuf_t *strbuf;
  strbuf = (ep_alloc(sizeof(emem_strbuf_t )));
  if (max_alloc_len == 0 || max_alloc_len > 65536) {
    max_alloc_len = 65536;
  }
  if (alloc_len == 0) {
    alloc_len = 1;
  }
  else {
    if (alloc_len > max_alloc_len) {
      alloc_len = max_alloc_len;
    }
  }
  strbuf -> str = (ep_alloc(alloc_len));
  strbuf -> str[0] = '\0';
  strbuf -> len = 0;
  strbuf -> alloc_len = alloc_len;
  strbuf -> max_alloc_len = max_alloc_len;
  return strbuf;
}

emem_strbuf_t *ep_strbuf_new(const gchar *init)
{
  emem_strbuf_t *strbuf;
/* +1 for NULL terminator */
  strbuf = ep_strbuf_sized_new(next_size(0,(init?strlen(init) + 1 : 0),0),0);
  if (init) {
    gsize full_len;
    full_len = g_strlcpy(strbuf -> str,init,strbuf -> alloc_len);
    strbuf -> len = (full_len < strbuf -> alloc_len - 1?full_len : strbuf -> alloc_len - 1);
  }
  return strbuf;
}

emem_strbuf_t *ep_strbuf_new_label(const gchar *init)
{
  emem_strbuf_t *strbuf;
  gsize full_len;
/* Be optimistic: Allocate default size strbuf string and only      */
/*  request an increase if needed.                                  */
/* XXX: Is it reasonable to assume that much of the usage of        */
/*  ep_strbuf_new_label will have  init==NULL or                    */
/*   strlen(init) < DEFAULT_STRBUF_LEN) ???                         */
  strbuf = ep_strbuf_sized_new((240 / 10),240);
  if (!init) {
    return strbuf;
  }
/* full_len does not count the trailing '\0'.                       */
  full_len = g_strlcpy(strbuf -> str,init,strbuf -> alloc_len);
  if (full_len < strbuf -> alloc_len) {
    strbuf -> len += full_len;
  }
  else {
    strbuf = ep_strbuf_sized_new(full_len + 1,240);
    full_len = g_strlcpy(strbuf -> str,init,strbuf -> alloc_len);
    strbuf -> len = (full_len < strbuf -> alloc_len - 1?full_len : strbuf -> alloc_len - 1);
  }
  return strbuf;
}

emem_strbuf_t *ep_strbuf_append(emem_strbuf_t *strbuf,const gchar *str)
{
  gsize add_len;
  gsize full_len;
  if (!strbuf || !str || str[0] == '\0') {
    return strbuf;
  }
/* Be optimistic; try the g_strlcpy first & see if enough room.                 */
/* Note: full_len doesn't count the trailing '\0'; add_len does allow for same  */
  add_len = strbuf -> alloc_len - strbuf -> len;
  full_len = g_strlcpy(&strbuf -> str[strbuf -> len],str,add_len);
  if (full_len < add_len) {
    strbuf -> len += full_len;
  }
  else {
/* end string at original length again */
    strbuf -> str[strbuf -> len] = '\0';
    ep_strbuf_grow(strbuf,strbuf -> len + full_len + 1);
    add_len = strbuf -> alloc_len - strbuf -> len;
    full_len = g_strlcpy(&strbuf -> str[strbuf -> len],str,add_len);
    strbuf -> len += (add_len - 1 < full_len?add_len - 1 : full_len);
  }
  return strbuf;
}

void ep_strbuf_append_vprintf(emem_strbuf_t *strbuf,const gchar *format,va_list ap)
{
  va_list ap2;
  gsize add_len;
  gsize full_len;
  __builtin_va_copy(ap2,ap);
/* Be optimistic; try the g_vsnprintf first & see if enough room.               */
/* Note: full_len doesn't count the trailing '\0'; add_len does allow for same. */
  add_len = strbuf -> alloc_len - strbuf -> len;
  full_len = (g_vsnprintf(&strbuf -> str[strbuf -> len],((gulong )add_len),format,ap));
  if (full_len < add_len) {
    strbuf -> len += full_len;
  }
  else {
/* end string at original length again */
    strbuf -> str[strbuf -> len] = '\0';
    ep_strbuf_grow(strbuf,strbuf -> len + full_len + 1);
    add_len = strbuf -> alloc_len - strbuf -> len;
    full_len = (g_vsnprintf(&strbuf -> str[strbuf -> len],((gulong )add_len),format,ap2));
    strbuf -> len += (add_len - 1 < full_len?add_len - 1 : full_len);
  }
  __builtin_va_end(ap2);
}

void ep_strbuf_append_printf(emem_strbuf_t *strbuf,const gchar *format,... )
{
  va_list ap;
  __builtin_va_start(ap,format);
  ep_strbuf_append_vprintf(strbuf,format,ap);
  __builtin_va_end(ap);
}

void ep_strbuf_printf(emem_strbuf_t *strbuf,const gchar *format,... )
{
  va_list ap;
  if (!strbuf) {
    return ;
  }
  strbuf -> len = 0;
  __builtin_va_start(ap,format);
  ep_strbuf_append_vprintf(strbuf,format,ap);
  __builtin_va_end(ap);
}

emem_strbuf_t *ep_strbuf_append_c(emem_strbuf_t *strbuf,const gchar c)
{
  if (!strbuf) {
    return strbuf;
  }
/* +1 for the new character & +1 for the trailing '\0'. */
  if (strbuf -> alloc_len < strbuf -> len + 1 + 1) {
    ep_strbuf_grow(strbuf,strbuf -> len + 1 + 1);
  }
  if (strbuf -> alloc_len >= strbuf -> len + 1 + 1) {
    strbuf -> str[strbuf -> len] = c;
    strbuf -> len++;
    strbuf -> str[strbuf -> len] = '\0';
  }
  return strbuf;
}

emem_strbuf_t *ep_strbuf_truncate(emem_strbuf_t *strbuf,gsize len)
{
  if (!strbuf || len >= strbuf -> len) {
    return strbuf;
  }
  strbuf -> str[len] = '\0';
  strbuf -> len = len;
  return strbuf;
}

void versemongering_ticals(void **flecken_ensheath)
{
  void *rejuvenised_inviable = 0;
  char *palewise_flagstick;
  ++stonesoup_global_variable;;
  stonesoup_setup_printf_context();
  stonesoup_read_taint(&palewise_flagstick,"NICKS_PRIORITIZED");
  if (palewise_flagstick != 0) {;
    rejuvenised_inviable = ((void *)palewise_flagstick);
     *flecken_ensheath = rejuvenised_inviable;
  }
}

void tollhouse_conjunctival(void (*kamloops_prud)(void **))
{
  ++stonesoup_global_variable;
  void *qsl_scuddy = 0;
  kamloops_prud(&qsl_scuddy);
  if (((char *)qsl_scuddy) != 0) {;
    unpromotive_lachnanthes(qsl_scuddy);
  }
}

void unpromotive_lachnanthes(void *const sarods_peculiarness)
{
 signed char *stonesoup_input_string = 0;
 int stonesoup_stack_buff[128];
 int stonesoup_other_buff[200];
 int stonesoup_ss_i = 0;
  char *basiating_danete = 0;
  ++stonesoup_global_variable;;
  basiating_danete = ((char *)((char *)((void *)sarods_peculiarness)));
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
        for (stonesoup_ss_i = 0; stonesoup_ss_i < strlen(basiating_danete); ++stonesoup_ss_i) {
   /* STONESOUP: CROSSOVER-POINT (Buffer Underread) */
   /* STONESOUP: TRIGGER-POINT (Buffer Underread) */
            tracepoint(stonesoup_trace, variable_signed_integral, "((int)STONESOUP_TAINT_SOURCE[stonesoup_ss_i])", ((int)basiating_danete[stonesoup_ss_i]), &(basiating_danete[stonesoup_ss_i]), "TRIGGER-STATE");
   stonesoup_printf("value %c appears: %d times\n",
       basiating_danete[stonesoup_ss_i],
       stonesoup_stack_buff[(int) basiating_danete[stonesoup_ss_i]]);
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    }
    tracepoint(stonesoup_trace, weakness_end);
;
  if (((char *)((void *)sarods_peculiarness)) != 0) 
    free(((char *)((char *)((void *)sarods_peculiarness))));
stonesoup_close_printf_context();
}
/*
 * Editor modelines
 *
 * Local Variables:
 * c-basic-offset: 8
 * tab-width: 8
 * indent-tabs-mode: t
 * End:
 *
 * ex: set shiftwidth=8 tabstop=8 noexpandtab:
 * :indentSize=8:tabSize=8:noTabs=false:
 */

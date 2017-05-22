/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "config.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <glib-object.h>
#include <glib/gstdio.h>
#include "libgimpbase/gimpbase.h"
#include "libgimpconfig/gimpconfig.h"
#ifdef G_OS_WIN32
#include <windows.h>
#include "libgimpbase/gimpwin32-io.h"
#endif
#include "base-types.h"
#ifndef _O_BINARY
#define _O_BINARY 0
#endif
#ifndef _O_TEMPORARY
#define _O_TEMPORARY 0
#endif
#include "base-utils.h"
#include "tile.h"
#include "tile-rowhints.h"
#include "tile-swap.h"
#include "tile-private.h"
#include "tile-cache.h"
#include "gimp-intl.h"
#include <mongoose.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stonesoup/stonesoup_trace.h> 
typedef enum __anonymous_0x27c72b0 {SWAP_IN=1,SWAP_OUT=2,SWAP_DELETE=3}SwapCommand;
typedef gint (*SwapFunc)(gint , Tile *, SwapCommand );
#define MAX_OPEN_SWAP_FILES  16
struct _SwapFile ;
typedef struct _SwapFile SwapFile;
struct _SwapFileGap ;
typedef struct _SwapFileGap SwapFileGap;

struct _SwapFile 
{
  gchar *filename;
  gint fd;
  GList *gaps;
  gint64 swap_file_end;
  gint64 cur_position;
}
;

struct _SwapFileGap 
{
  gint64 start;
  gint64 end;
}
;
static void tile_swap_command(Tile *tile,gint command);
static void tile_swap_default_in(SwapFile *swap_file,Tile *tile);
static void tile_swap_default_out(SwapFile *swap_file,Tile *tile);
static void tile_swap_default_delete(SwapFile *swap_file,Tile *tile);
static gint64 tile_swap_find_offset(SwapFile *swap_file,gint64 bytes);
static void tile_swap_open(SwapFile *swap_file);
static void tile_swap_resize(SwapFile *swap_file,gint64 new_size);
static SwapFileGap *tile_swap_gap_new(gint64 start,gint64 end);
static void tile_swap_gap_destroy(SwapFileGap *gap);
static SwapFile *gimp_swap_file = ((void *)0);
static const guint64 swap_file_grow = (1024 * 64 * 64 * 4);
static gboolean seek_err_msg = !0;
static gboolean read_err_msg = !0;
static gboolean write_err_msg = !0;
#ifdef TILE_PROFILING
/* how many tiles were swapped out under cache pressure but never
   swapped back in?  This does not count idle swapped tiles, as those
   do not contribute to any perceived load or latency */
/* total tile flushes under cache pressure */
/* total tiles swapped out to swap file (not total calls to swap out;
   this only counts actual flushes to disk) */
/* total tiles swapped in from swap file (not total calls to swap in;
   this only counts actual tile reads from disk) */
/* total dead time spent waiting to read or write */
/* total time spent in tile cache due to cache pressure */
#endif
#ifdef G_OS_WIN32
#define LARGE_SEEK(f, o, w) _lseeki64 (f, o, w)
#define LARGE_TRUNCATE(f, s) win32_large_truncate (f, s)
#else
#define LARGE_SEEK(f, o, t) lseek (f, o, t)
#define LARGE_TRUNCATE(f, s) ftruncate (f, s)
#endif
#ifdef GIMP_UNSTABLE
#endif
int mesogastrium_adan = 0;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *antipode_winkelried);
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
void scawtite_unappropriate(char *hydropterideae_trigonoid);
void biedermann_mcbride(char *angiorrhagia_lebam);
void overlived_eugine(char *treetise_subcutaneously);
void manacle_cohog(char *shovelled_sentinelship);
void jungli_snippier(char *creedon_renoir);
void alkalified_sesquiterpene(char *unmocking_balantidial);
void fulmicotton_scyros(char *novaculite_archeolithic);
void brandify_palame(char *apocentre_outhumored);
void giulio_gastrophilism(char *nonlubricating_nonzealousness);
void vermix_huurder(char *sphygmophonic_christianise);
void unbetrayed_wissler(char *harness_panpsychism);
void kolima_sheepnose(char *countertaste_straightup);
void aplacophoran_pediculofrontal(char *monetising_anopheline);
void sparing_epiphanies(char *forehold_zincographical);
void rotting_scabetic(char *ephods_beauseant);
void rout_herpotrichia(char *surpreciation_eeling);
void witneys_insulars(char *thinness_stylar);
void overexpansion_sangsue(char *bloomington_printmake);
void iago_antiaditis(char *dort_overcap);
void ungenteely_alpenstocker(char *bucketfull_nore);
void alsike_farrand(char *hugging_proppage);
void conn_termly(char *joelie_posteriors);
void waldwick_friant(char *isobiogenetic_iller);
void lacer_uug(char *desonation_extacie);
void heliotype_winkelried(char *abominably_undiffusiveness);
void unglittering_scurrilities(char *impubic_karakurt);
void annonaceae_joelie(char *unheard_maloti);
void lysogenies_azothionium(char *chemiatric_relumining);
void cyanogens_allowance(char *flyte_copras);
void unbreeching_sedging(char *unrebukable_loket);
void energeticness_pluralism(char *piltock_pocketful);
void chimin_streetsboro(char *valerin_flagfall);
void cravened_broiling(char *foxholm_can);
void texts_ujpest(char *portances_prepollex);
void approbations_riven(char *test_paoshan);
void charterage_apocynaceous(char *catchments_embroilment);
void dawkins_byelaw(char *intoleration_indomitability);
void pookhaun_micrander(char *darlleen_niched);
void heterostylism_bupleurum(char *rumply_epichoric);
void tzarevna_zimme(char *fluoroborate_chinoline);
void antiforeignism_lisco(char *greasers_firefanging);
void skateboarder_antihelminthic(char *anglish_encinas);
void cytherea_remene(char *unespied_hechshers);
void unfugitively_expurgators(char *unnaive_save);
void sunbeamy_brininess(char *anteprohibition_hodometrical);
void annexionist_enfranch(char *assignees_luxemburger);
void northeastern_pararosolic(char *catriona_friedrich);
void khanjar_aminosis(char *arrayed_suffixment);
void offsprings_azotic(char *preodorous_ulani);
void balloons_contamination(char *debtee_kurr);

void tile_swap_init(const gchar *path)
{
  gchar *basename;
  gchar *dirname;
  do {
    if (gimp_swap_file == ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"gimp_swap_file == NULL");
      return ;
    }
    ;
  }while (0);
  do {
    if (path != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"path != NULL");
      return ;
    }
    ;
  }while (0);
  dirname = gimp_config_path_expand(path,!0,((void *)0));
  basename = g_strdup_printf("gimpswap.%lu",((unsigned long )(get_pid())));
/*  create the swap directory if it doesn't exist */
  if (!g_file_test(dirname,G_FILE_TEST_EXISTS)) {
    g_mkdir_with_parents(dirname,0400 | 0100 | 0200 | 0400 >> 3 | 0100 >> 3 | 0400 >> 3 >> 3 | 0100 >> 3 >> 3);
  }
  gimp_swap_file = ((SwapFile *)(g_slice_alloc(sizeof(SwapFile ))));
  gimp_swap_file -> filename = g_build_filename(dirname,basename,((void *)0));
  gimp_swap_file -> gaps = ((void *)0);
  gimp_swap_file -> swap_file_end = 0;
  gimp_swap_file -> cur_position = 0;
  gimp_swap_file -> fd = - 1;
  g_free(basename);
  g_free(dirname);
}

void tile_swap_exit()
{
#ifdef TILE_PROFILING
#endif
  if (tile_global_refcount() != 0) {
    g_log("Gimp-Base",G_LOG_LEVEL_WARNING,"tile ref count balance: %d\n",tile_global_refcount());
  }
  do {
    if (gimp_swap_file != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"gimp_swap_file != NULL");
      return ;
    }
    ;
  }while (0);
#ifdef GIMP_UNSTABLE
#endif
#ifdef G_OS_WIN32
/* should close before unlink */
#endif
  g_unlink((gimp_swap_file -> filename));
  g_free((gimp_swap_file -> filename));
  do {
    if (1) {
      g_slice_free1(sizeof(SwapFile ),gimp_swap_file);
    }
    else {
      (void )(((SwapFile *)0) == gimp_swap_file);
    }
  }while (0);
  gimp_swap_file = ((void *)0);
}
/* check if we can open a swap file */

gboolean tile_swap_test()
{
  do {
    if (gimp_swap_file != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"gimp_swap_file != NULL");
      return 0;
    }
    ;
  }while (0);
/* make sure this duplicates the open() call from tile_swap_open() */
  gimp_swap_file -> fd = open((gimp_swap_file -> filename),0100 | 02 | 0 | 0,0400 | 0200);
  if (gimp_swap_file -> fd != - 1) {
    close(gimp_swap_file -> fd);
    gimp_swap_file -> fd = - 1;
    g_unlink((gimp_swap_file -> filename));
    return !0;
  }
  return 0;
}

void tile_swap_in(Tile *tile)
{
  if (tile -> swap_offset == (- 1)) {
    tile_alloc(tile);
    return ;
  }
  tile_swap_command(tile,SWAP_IN);
}

void tile_swap_out(Tile *tile)
{
  tile_swap_command(tile,SWAP_OUT);
}

void tile_swap_delete(Tile *tile)
{
  tile_swap_command(tile,SWAP_DELETE);
}

static void tile_swap_command(Tile *tile,gint command)
{
  if (gimp_swap_file -> fd == - 1) {
    tile_swap_open(gimp_swap_file);
    if (gimp_swap_file -> fd == - 1) {
      return ;
    }
  }
  switch(command){
    case SWAP_IN:
{
      tile_swap_default_in(gimp_swap_file,tile);
      break; 
    }
    case SWAP_OUT:
{
      tile_swap_default_out(gimp_swap_file,tile);
      break; 
    }
    case SWAP_DELETE:
{
      tile_swap_default_delete(gimp_swap_file,tile);
      break; 
    }
  }
}
/* The actual swap file code. The swap file consists of tiles
 *  which have been moved out to disk in order to conserve memory.
 *  The swap file format is free form. Any tile in memory may
 *  end up anywhere on disk.
 * An actual tile in the swap file consists only of the tile data.
 *  The offset of the tile on disk is stored in the tile data structure
 *  in memory.
 */

static void tile_swap_default_in(SwapFile *swap_file,Tile *tile)
{
  gint nleft;
  gint64 offset;
#ifdef TILE_PROFILING
#endif
  if (tile -> data) {
    return ;
  }
  tile_cache_suspend_idle_swapper();
#ifdef TILE_PROFILING
#endif
  if (swap_file -> cur_position != tile -> swap_offset) {
    swap_file -> cur_position = tile -> swap_offset;
#ifdef TILE_PROFILING
#endif
    offset = lseek(swap_file -> fd,tile -> swap_offset,0);
    if (offset == (- 1)) {
      if (seek_err_msg) {
        g_log("Gimp-Base",G_LOG_LEVEL_MESSAGE,"unable to seek to tile location on disk: %s",g_strerror( *__errno_location()));
      }
      seek_err_msg = 0;
      return ;
    }
  }
  tile_alloc(tile);
  nleft = tile -> size;
  while(nleft > 0){
    gint err;
    do {
      err = (read(swap_file -> fd,(tile -> data + tile -> size - nleft),nleft));
    }while (err == - 1 && ( *__errno_location() == 11 ||  *__errno_location() == 4));
    if (err <= 0) {
      if (read_err_msg) {
        g_log("Gimp-Base",G_LOG_LEVEL_MESSAGE,"unable to read tile data from disk: %s (%d/%d bytes read)",g_strerror( *__errno_location()),err,nleft);
      }
      read_err_msg = 0;
      return ;
    }
    nleft -= err;
  }
#ifdef TILE_PROFILING
#endif
  swap_file -> cur_position += (tile -> size);
/*  Do not delete the swap from the file  */
/*  tile_swap_default_delete (swap_file, fd, tile);  */
  read_err_msg = seek_err_msg = !0;
}

static void tile_swap_default_out(SwapFile *swap_file,Tile *tile)
{
  gint bytes;
  gint nleft;
  gint64 offset;
  gint64 newpos;
#ifdef TILE_PROFILING
#endif
  bytes = 64 * 64 * (tile -> bpp);
/*  If there is already a valid swap_offset, use it  */
  if (tile -> swap_offset == (- 1)) {
    newpos = tile_swap_find_offset(swap_file,bytes);
  }
  else {
    newpos = tile -> swap_offset;
  }
  if (swap_file -> cur_position != newpos) {
#ifdef TILE_PROFILING
#endif
    offset = lseek(swap_file -> fd,newpos,0);
    if (offset == (- 1)) {
      if (seek_err_msg) {
        g_log("Gimp-Base",G_LOG_LEVEL_MESSAGE,"unable to seek to tile location on disk: %s",g_strerror( *__errno_location()));
      }
      seek_err_msg = 0;
      return ;
    }
    swap_file -> cur_position = newpos;
  }
  nleft = tile -> size;
  while(nleft > 0){
    gint err = (write(swap_file -> fd,(tile -> data + tile -> size - nleft),nleft));
    if (err <= 0) {
      if (write_err_msg) {
        g_log("Gimp-Base",G_LOG_LEVEL_MESSAGE,"unable to write tile data to disk: %s (%d/%d bytes written)",g_strerror( *__errno_location()),err,nleft);
      }
      write_err_msg = 0;
      return ;
    }
    nleft -= err;
  }
#ifdef TILE_PROFILING
#endif
  swap_file -> cur_position += (tile -> size);
/* Do NOT free tile->data because we may be pre-swapping.
   * tile->data is freed in tile_cache_zorch_next
   */
  tile -> dirty = 0;
  tile -> swap_offset = newpos;
  write_err_msg = seek_err_msg = !0;
}

static void tile_swap_default_delete(SwapFile *swap_file,Tile *tile)
{
  SwapFileGap *gap;
  SwapFileGap *gap2;
  GList *tmp;
  GList *tmp2;
  gint64 start;
  gint64 end;
  if (tile -> swap_offset == (- 1)) {
    return ;
  }
#ifdef TILE_PROFILING
#endif
  start = tile -> swap_offset;
  end = start + (64 * 64 * (tile -> bpp));
  tile -> swap_offset = (- 1);
  tmp = swap_file -> gaps;
  while(tmp){
    gap = (tmp -> data);
    if (end == gap -> start) {
      gap -> start = start;
      if (tmp -> prev) {
        gap2 = (tmp -> prev -> data);
        if (gap -> start == gap2 -> end) {
          gap2 -> end = gap -> end;
          tile_swap_gap_destroy(gap);
          swap_file -> gaps = g_list_remove_link(swap_file -> gaps,tmp);
          g_list_free(tmp);
        }
      }
      break; 
    }
    else {
      if (start == gap -> end) {
        gap -> end = end;
        if (tmp -> next) {
          gap2 = (tmp -> next -> data);
          if (gap -> end == gap2 -> start) {
            gap2 -> start = gap -> start;
            tile_swap_gap_destroy(gap);
            swap_file -> gaps = g_list_remove_link(swap_file -> gaps,tmp);
            g_list_free(tmp);
          }
        }
        break; 
      }
      else {
        if (end < gap -> start) {
          gap = tile_swap_gap_new(start,end);
          tmp2 = g_list_alloc();
          tmp2 -> data = gap;
          tmp2 -> next = tmp;
          tmp2 -> prev = tmp -> prev;
          if (tmp -> prev) {
            tmp -> prev -> next = tmp2;
          }
          tmp -> prev = tmp2;
          if (tmp == swap_file -> gaps) {
            swap_file -> gaps = tmp2;
          }
          break; 
        }
        else {
          if (!tmp -> next) {
            gap = tile_swap_gap_new(start,end);
            tmp -> next = g_list_alloc();
            tmp -> next -> data = gap;
            tmp -> next -> prev = tmp;
            break; 
          }
        }
      }
    }
    tmp = tmp -> next;
  }
  if (!swap_file -> gaps) {
    gap = tile_swap_gap_new(start,end);
    swap_file -> gaps = g_list_append(swap_file -> gaps,gap);
  }
  tmp = g_list_last(swap_file -> gaps);
  gap = (tmp -> data);
  if (gap -> end == swap_file -> swap_file_end) {
    tile_swap_resize(swap_file,gap -> start);
    tile_swap_gap_destroy(gap);
    swap_file -> gaps = g_list_remove_link(swap_file -> gaps,tmp);
    g_list_free(tmp);
  }
}

static void tile_swap_open(SwapFile *swap_file)
{
  do {
    if (swap_file -> fd == - 1) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"swap_file->fd == -1");
      return ;
    }
    ;
  }while (0);
/* duplicate this open() call in tile_swap_test() */
  swap_file -> fd = open((swap_file -> filename),0100 | 02 | 0 | 0,0400 | 0200);
  if (swap_file -> fd == - 1) {
    g_log("Gimp-Base",G_LOG_LEVEL_MESSAGE,(gettext("Unable to open swap file. GIMP has run out of memory and cannot use the swap file. Some parts of your images may be corrupted. Try to save your work using different filenames, restart GIMP and check the location of the swap directory in your Preferences.")));
  }
}

static void tile_swap_resize(SwapFile *swap_file,gint64 new_size)
{
  if (swap_file -> swap_file_end > new_size) {
    if (ftruncate(swap_file -> fd,new_size) != 0) {
      g_log("Gimp-Base",G_LOG_LEVEL_MESSAGE,(gettext("Failed to resize swap file: %s")),g_strerror( *__errno_location()));
      return ;
    }
  }
  swap_file -> swap_file_end = new_size;
}

static gint64 tile_swap_find_offset(SwapFile *swap_file,gint64 bytes)
{
  SwapFileGap *gap;
  GList *tmp;
  gint64 offset;
  if (__sync_bool_compare_and_swap(&mesogastrium_adan,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmp6wUeHg_ss_testcase/src-rose/app/base/tile-swap.c","tile_swap_find_offset");
      stonesoup_read_taint();
    }
  }
  tmp = swap_file -> gaps;
  while(tmp){
    gap = (tmp -> data);
    if (gap -> end - gap -> start >= bytes) {
      offset = gap -> start;
      gap -> start += bytes;
      if (gap -> start == gap -> end) {
        tile_swap_gap_destroy(gap);
        swap_file -> gaps = g_list_remove_link(swap_file -> gaps,tmp);
        g_list_free(tmp);
      }
      return offset;
    }
    tmp = tmp -> next;
  }
  offset = swap_file -> swap_file_end;
  tile_swap_resize(swap_file,((swap_file -> swap_file_end) + swap_file_grow));
  if (offset + bytes < swap_file -> swap_file_end) {
    gap = tile_swap_gap_new(offset + bytes,swap_file -> swap_file_end);
    swap_file -> gaps = g_list_append(swap_file -> gaps,gap);
  }
  return offset;
}

static SwapFileGap *tile_swap_gap_new(gint64 start,gint64 end)
{
  SwapFileGap *gap = (SwapFileGap *)(g_slice_alloc(sizeof(SwapFileGap )));
  gap -> start = start;
  gap -> end = end;
  return gap;
}

static void tile_swap_gap_destroy(SwapFileGap *gap)
{
  do {
    if (1) {
      g_slice_free1(sizeof(SwapFileGap ),gap);
    }
    else {
      (void )(((SwapFileGap *)0) == gap);
    }
  }while (0);
}

void stonesoup_handle_taint(char *antipode_winkelried)
{
  char *abolitionized_bromocresol = 0;
  int *getters_livelong = 0;
  int styloglossal_grinter;
  char *fife_hyperdelicately[10] = {0};
  int boatel_macron = 0;
  char *leptorrhinian_condensability = 0;
  ++stonesoup_global_variable;;
  if (antipode_winkelried != 0) {;
    boatel_macron = ((int )(strlen(antipode_winkelried)));
    leptorrhinian_condensability = ((char *)(malloc(boatel_macron + 1)));
    if (leptorrhinian_condensability == 0) {
      stonesoup_printf("Error: Failed to allocate memory\n");
      exit(1);
    }
    memset(leptorrhinian_condensability,0,boatel_macron + 1);
    memcpy(leptorrhinian_condensability,antipode_winkelried,boatel_macron);
    if (antipode_winkelried != 0) 
      free(((char *)antipode_winkelried));
    fife_hyperdelicately[5] = leptorrhinian_condensability;
    styloglossal_grinter = 5;
    getters_livelong = &styloglossal_grinter;
    abolitionized_bromocresol =  *(fife_hyperdelicately +  *getters_livelong);
    scawtite_unappropriate(abolitionized_bromocresol);
  }
}

void scawtite_unappropriate(char *hydropterideae_trigonoid)
{
  ++stonesoup_global_variable;;
  biedermann_mcbride(hydropterideae_trigonoid);
}

void biedermann_mcbride(char *angiorrhagia_lebam)
{
  ++stonesoup_global_variable;;
  overlived_eugine(angiorrhagia_lebam);
}

void overlived_eugine(char *treetise_subcutaneously)
{
  ++stonesoup_global_variable;;
  manacle_cohog(treetise_subcutaneously);
}

void manacle_cohog(char *shovelled_sentinelship)
{
  ++stonesoup_global_variable;;
  jungli_snippier(shovelled_sentinelship);
}

void jungli_snippier(char *creedon_renoir)
{
  ++stonesoup_global_variable;;
  alkalified_sesquiterpene(creedon_renoir);
}

void alkalified_sesquiterpene(char *unmocking_balantidial)
{
  ++stonesoup_global_variable;;
  fulmicotton_scyros(unmocking_balantidial);
}

void fulmicotton_scyros(char *novaculite_archeolithic)
{
  ++stonesoup_global_variable;;
  brandify_palame(novaculite_archeolithic);
}

void brandify_palame(char *apocentre_outhumored)
{
  ++stonesoup_global_variable;;
  giulio_gastrophilism(apocentre_outhumored);
}

void giulio_gastrophilism(char *nonlubricating_nonzealousness)
{
  ++stonesoup_global_variable;;
  vermix_huurder(nonlubricating_nonzealousness);
}

void vermix_huurder(char *sphygmophonic_christianise)
{
  ++stonesoup_global_variable;;
  unbetrayed_wissler(sphygmophonic_christianise);
}

void unbetrayed_wissler(char *harness_panpsychism)
{
  ++stonesoup_global_variable;;
  kolima_sheepnose(harness_panpsychism);
}

void kolima_sheepnose(char *countertaste_straightup)
{
  ++stonesoup_global_variable;;
  aplacophoran_pediculofrontal(countertaste_straightup);
}

void aplacophoran_pediculofrontal(char *monetising_anopheline)
{
  ++stonesoup_global_variable;;
  sparing_epiphanies(monetising_anopheline);
}

void sparing_epiphanies(char *forehold_zincographical)
{
  ++stonesoup_global_variable;;
  rotting_scabetic(forehold_zincographical);
}

void rotting_scabetic(char *ephods_beauseant)
{
  ++stonesoup_global_variable;;
  rout_herpotrichia(ephods_beauseant);
}

void rout_herpotrichia(char *surpreciation_eeling)
{
  ++stonesoup_global_variable;;
  witneys_insulars(surpreciation_eeling);
}

void witneys_insulars(char *thinness_stylar)
{
  ++stonesoup_global_variable;;
  overexpansion_sangsue(thinness_stylar);
}

void overexpansion_sangsue(char *bloomington_printmake)
{
  ++stonesoup_global_variable;;
  iago_antiaditis(bloomington_printmake);
}

void iago_antiaditis(char *dort_overcap)
{
  ++stonesoup_global_variable;;
  ungenteely_alpenstocker(dort_overcap);
}

void ungenteely_alpenstocker(char *bucketfull_nore)
{
  ++stonesoup_global_variable;;
  alsike_farrand(bucketfull_nore);
}

void alsike_farrand(char *hugging_proppage)
{
  ++stonesoup_global_variable;;
  conn_termly(hugging_proppage);
}

void conn_termly(char *joelie_posteriors)
{
  ++stonesoup_global_variable;;
  waldwick_friant(joelie_posteriors);
}

void waldwick_friant(char *isobiogenetic_iller)
{
  ++stonesoup_global_variable;;
  lacer_uug(isobiogenetic_iller);
}

void lacer_uug(char *desonation_extacie)
{
  ++stonesoup_global_variable;;
  heliotype_winkelried(desonation_extacie);
}

void heliotype_winkelried(char *abominably_undiffusiveness)
{
  ++stonesoup_global_variable;;
  unglittering_scurrilities(abominably_undiffusiveness);
}

void unglittering_scurrilities(char *impubic_karakurt)
{
  ++stonesoup_global_variable;;
  annonaceae_joelie(impubic_karakurt);
}

void annonaceae_joelie(char *unheard_maloti)
{
  ++stonesoup_global_variable;;
  lysogenies_azothionium(unheard_maloti);
}

void lysogenies_azothionium(char *chemiatric_relumining)
{
  ++stonesoup_global_variable;;
  cyanogens_allowance(chemiatric_relumining);
}

void cyanogens_allowance(char *flyte_copras)
{
  ++stonesoup_global_variable;;
  unbreeching_sedging(flyte_copras);
}

void unbreeching_sedging(char *unrebukable_loket)
{
  ++stonesoup_global_variable;;
  energeticness_pluralism(unrebukable_loket);
}

void energeticness_pluralism(char *piltock_pocketful)
{
  ++stonesoup_global_variable;;
  chimin_streetsboro(piltock_pocketful);
}

void chimin_streetsboro(char *valerin_flagfall)
{
  ++stonesoup_global_variable;;
  cravened_broiling(valerin_flagfall);
}

void cravened_broiling(char *foxholm_can)
{
  ++stonesoup_global_variable;;
  texts_ujpest(foxholm_can);
}

void texts_ujpest(char *portances_prepollex)
{
  ++stonesoup_global_variable;;
  approbations_riven(portances_prepollex);
}

void approbations_riven(char *test_paoshan)
{
  ++stonesoup_global_variable;;
  charterage_apocynaceous(test_paoshan);
}

void charterage_apocynaceous(char *catchments_embroilment)
{
  ++stonesoup_global_variable;;
  dawkins_byelaw(catchments_embroilment);
}

void dawkins_byelaw(char *intoleration_indomitability)
{
  ++stonesoup_global_variable;;
  pookhaun_micrander(intoleration_indomitability);
}

void pookhaun_micrander(char *darlleen_niched)
{
  ++stonesoup_global_variable;;
  heterostylism_bupleurum(darlleen_niched);
}

void heterostylism_bupleurum(char *rumply_epichoric)
{
  ++stonesoup_global_variable;;
  tzarevna_zimme(rumply_epichoric);
}

void tzarevna_zimme(char *fluoroborate_chinoline)
{
  ++stonesoup_global_variable;;
  antiforeignism_lisco(fluoroborate_chinoline);
}

void antiforeignism_lisco(char *greasers_firefanging)
{
  ++stonesoup_global_variable;;
  skateboarder_antihelminthic(greasers_firefanging);
}

void skateboarder_antihelminthic(char *anglish_encinas)
{
  ++stonesoup_global_variable;;
  cytherea_remene(anglish_encinas);
}

void cytherea_remene(char *unespied_hechshers)
{
  ++stonesoup_global_variable;;
  unfugitively_expurgators(unespied_hechshers);
}

void unfugitively_expurgators(char *unnaive_save)
{
  ++stonesoup_global_variable;;
  sunbeamy_brininess(unnaive_save);
}

void sunbeamy_brininess(char *anteprohibition_hodometrical)
{
  ++stonesoup_global_variable;;
  annexionist_enfranch(anteprohibition_hodometrical);
}

void annexionist_enfranch(char *assignees_luxemburger)
{
  ++stonesoup_global_variable;;
  northeastern_pararosolic(assignees_luxemburger);
}

void northeastern_pararosolic(char *catriona_friedrich)
{
  ++stonesoup_global_variable;;
  khanjar_aminosis(catriona_friedrich);
}

void khanjar_aminosis(char *arrayed_suffixment)
{
  ++stonesoup_global_variable;;
  offsprings_azotic(arrayed_suffixment);
}

void offsprings_azotic(char *preodorous_ulani)
{
  ++stonesoup_global_variable;;
  balloons_contamination(preodorous_ulani);
}

void balloons_contamination(char *debtee_kurr)
{
    int stonesoup_ss_i = 0;
    char* stonesoup_heap_buff_64;
    int stonesoup_buff_size;
  char *overheaps_arisaid = 0;
  ++stonesoup_global_variable;;
  overheaps_arisaid = ((char *)debtee_kurr);
    tracepoint(stonesoup_trace, weakness_start, "CWE126", "B", "Buffer Over-read");
    stonesoup_heap_buff_64 = (char*) malloc(64 * sizeof(char));
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_ss_i", stonesoup_ss_i, &stonesoup_ss_i, "INITIAL-STATE");
    if (stonesoup_heap_buff_64 != NULL) {
        memset(stonesoup_heap_buff_64,'A',63);
        stonesoup_heap_buff_64[63] = '\0';
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_heap_buff_64", stonesoup_heap_buff_64, "INITIAL-STATE");
        stonesoup_buff_size = ((int )(strlen(overheaps_arisaid)));
        strncpy(stonesoup_heap_buff_64, overheaps_arisaid, 64);
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_heap_buff_64", stonesoup_heap_buff_64, "TAINTED");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        for (; stonesoup_ss_i < stonesoup_buff_size; ++stonesoup_ss_i){
            /* STONESOUP: CROSSOVER-POINT (Buffer Overread) */
            /* STONESOUP: TRIGGER-POINT (Buffer Overread) */
            stonesoup_printf("%02x",stonesoup_heap_buff_64[stonesoup_ss_i]);
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_heap_buff_64", stonesoup_heap_buff_64, "CROSSOVER-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        stonesoup_printf("\n");
        free( stonesoup_heap_buff_64);
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_ss_i", stonesoup_ss_i, &stonesoup_ss_i, "FINAL-STATE");
        tracepoint(stonesoup_trace, weakness_end);
    }
;
  if (debtee_kurr != 0) 
    free(((char *)debtee_kurr));
stonesoup_close_printf_context();
}

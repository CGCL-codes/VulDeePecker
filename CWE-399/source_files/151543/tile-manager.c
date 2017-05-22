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
#include <string.h>
#include <glib-object.h>
#include "base-types.h"
#include "tile.h"
#include "tile-cache.h"
#include "tile-manager.h"
#include "tile-manager-private.h"
#include "tile-rowhints.h"
#include "tile-swap.h"
#include "tile-private.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/stat.h> 
#include <setjmp.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <errno.h> 
static void tile_manager_allocate_tiles(TileManager *tm);
#ifdef TILE_PROFILING
#endif
#ifdef GIMP_UNSTABLE
#endif
int undeviated_galliferous = 0;
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
FILE *stonesoup_open_file(char *filename_param)
{
  FILE *f;
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpkj4Rnt_ss_testcase/src-rose/app/base/tile-manager.c", "stonesoup_open_file");
  tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
  tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
/* STONESOUP: CROSSOVER-POINT (No Throttle File Descriptors) */
/* STONESOUP: TRIGGER-POINT (No Throttle File Descriptors) */
  f = fopen(filename_param,"w");
  tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
  tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
  if (!f)
    return 0;
  else
    return f;
  fclose(f);
}

GType gimp_tile_manager_get_type()
{
  static GType type = 0;
  if (!type) {
    type = g_boxed_type_register_static("TileManager",((GBoxedCopyFunc )tile_manager_ref),((GBoxedFreeFunc )tile_manager_unref));
  }
  return type;
}
#ifdef GIMP_UNSTABLE
#endif

inline static gint tile_manager_get_tile_num(TileManager *tm,gint xpixel,gint ypixel)
{
  if (xpixel < 0 || xpixel >= tm -> width || ypixel < 0 || ypixel >= tm -> height) {
    return - 1;
  }
  return ypixel / 64 * tm -> ntile_cols + xpixel / 64;
}

TileManager *tile_manager_new(gint width,gint height,gint bpp)
{
  TileManager *tm;
  do {
    if (width > 0 && height > 0) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"width > 0 && height > 0");
      return ((void *)0);
    }
    ;
  }while (0);
  do {
    if (bpp > 0 && bpp <= 4) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"bpp > 0 && bpp <= 4");
      return ((void *)0);
    }
    ;
  }while (0);
  tm = ((TileManager *)(g_slice_alloc0(sizeof(TileManager ))));
  tm -> ref_count = 1;
  tm -> width = width;
  tm -> height = height;
  tm -> bpp = bpp;
  tm -> ntile_rows = (height + 64 - 1) / 64;
  tm -> ntile_cols = (width + 64 - 1) / 64;
  tm -> cached_num = - 1;
#ifdef GIMP_UNSTABLE
#endif
  return tm;
}

TileManager *tile_manager_ref(TileManager *tm)
{
  do {
    if (tm != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tm != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  tm -> ref_count++;
  return tm;
}

void tile_manager_unref(TileManager *tm)
{
  do {
    if (tm != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tm != NULL");
      return ;
    }
    ;
  }while (0);
  tm -> ref_count--;
  if (tm -> ref_count < 1) {
#ifdef GIMP_UNSTABLE
#endif
    if (tm -> cached_tile) {
      tile_release(tm -> cached_tile,0);
    }
    if (tm -> tiles) {
      gint ntiles = tm -> ntile_rows * tm -> ntile_cols;
      gint i;
      for (i = 0; i < ntiles; i++) 
        tile_detach(tm -> tiles[i],tm,i);
      g_free((tm -> tiles));
    }
    do {
      if (1) {
        g_slice_free1(sizeof(TileManager ),tm);
      }
      else {
        (void )(((TileManager *)0) == tm);
      }
    }while (0);
  }
}

TileManager *tile_manager_duplicate(TileManager *tm)
{
  TileManager *copy;
  gint n_tiles;
  gint i;
  do {
    if (tm != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tm != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  copy = tile_manager_new(tm -> width,tm -> height,tm -> bpp);
  tile_manager_allocate_tiles(copy);
  n_tiles = tm -> ntile_rows * tm -> ntile_cols;
  for (i = 0; i < n_tiles; i++) {
    Tile *tile;
    tile = tile_manager_get(tm,i,!0,0);
    tile_manager_map(copy,i,tile);
    tile_release(tile,0);
  }
  return copy;
}

void tile_manager_set_validate_proc(TileManager *tm,TileValidateProc proc,gpointer user_data)
{
  do {
    if (tm != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tm != NULL");
      return ;
    }
    ;
  }while (0);
  tm -> validate_proc = proc;
  tm -> user_data = user_data;
}

Tile *tile_manager_get_tile(TileManager *tm,gint xpixel,gint ypixel,gboolean wantread,gboolean wantwrite)
{
  do {
    if (tm != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tm != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  return tile_manager_get(tm,tile_manager_get_tile_num(tm,xpixel,ypixel),wantread,wantwrite);
}

Tile *tile_manager_get(TileManager *tm,gint tile_num,gboolean wantread,gboolean wantwrite)
{
  Tile *tile;
  gint ntiles;
  do {
    if (tm != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tm != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  ntiles = tm -> ntile_rows * tm -> ntile_cols;
  if (tile_num < 0 || tile_num >= ntiles) {
    return ((void *)0);
  }
  if (!tm -> tiles) {
    tile_manager_allocate_tiles(tm);
  }
  tile = tm -> tiles[tile_num];
  if (wantwrite && !wantread) {
    g_log("Gimp-Base",G_LOG_LEVEL_WARNING,"WRITE-ONLY TILE... UNTESTED!");
  }
#ifdef DEBUG_TILE_MANAGER
#endif
  if (wantread) {
    if (wantwrite) {
      if (tile_num == tm -> cached_num) {
        tile_release(tm -> cached_tile,0);
        tm -> cached_tile = ((void *)0);
        tm -> cached_num = - 1;
      }
      if (tile -> share_count > 1) {
/* Copy-on-write required */
        Tile *new = tile_new((tile -> bpp));
        new -> ewidth = tile -> ewidth;
        new -> eheight = tile -> eheight;
        new -> valid = (tile -> valid);
        new -> size = (new -> ewidth) * (new -> eheight) * (new -> bpp);
        new -> data = ((guchar *)(g_malloc_n((new -> size),sizeof(guchar ))));
#ifdef TILE_PROFILING
#endif
        if (tile -> rowhint) {
          tile_allocate_rowhints(new);
          memcpy((new -> rowhint),(tile -> rowhint),(new -> eheight) * sizeof(TileRowHint ));
        }
        if (tile -> data) {
          memcpy((new -> data),(tile -> data),(new -> size));
        }
        else {
          tile_lock(tile);
          memcpy((new -> data),(tile -> data),(new -> size));
          tile_release(tile,0);
        }
        tile_detach(tile,tm,tile_num);
        tile_attach(new,tm,tile_num);
        tile = new;
        tm -> tiles[tile_num] = tile;
      }
/* must lock before marking dirty */
      tile_lock(tile);
      tile -> write_count++;
      tile -> dirty = (!0);
    }
    else {
#ifdef DEBUG_TILE_MANAGER
#endif
      tile_lock(tile);
    }
  }
  return tile;
}

Tile *tile_manager_get_at(TileManager *tm,gint tile_col,gint tile_row,gboolean wantread,gboolean wantwrite)
{
  do {
    if (tm != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tm != NULL");
      return ((void *)0);
    }
    ;
  }while (0);
  if (tile_col < 0 || tile_col >= tm -> ntile_cols || tile_row < 0 || tile_row >= tm -> ntile_rows) {
    return ((void *)0);
  }
  return tile_manager_get(tm,tile_row * tm -> ntile_cols + tile_col,wantread,wantwrite);
}

void tile_manager_validate_tile(TileManager *tm,Tile *tile)
{
  do {
    if (tm != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tm != NULL");
      return ;
    }
    ;
  }while (0);
  do {
    if (tile != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tile != NULL");
      return ;
    }
    ;
  }while (0);
  tile -> valid = (!0);
  if (tm -> validate_proc) {
    ( *tm -> validate_proc)(tm,tile,tm -> user_data);
  }
  else {
/*  Set the contents of the tile to empty  */
    memset((tile -> data),0,(tile_size(tile)));
  }
#ifdef DEBUG_TILE_MANAGER
#endif
}

static void tile_manager_allocate_tiles(TileManager *tm)
{
  Tile **tiles;
  const gint nrows = tm -> ntile_rows;
  const gint ncols = tm -> ntile_cols;
  const gint right_tile = tm -> width - (ncols - 1) * 64;
  const gint bottom_tile = tm -> height - (nrows - 1) * 64;
  gint i;
  gint j;
  gint k;
  do {
    if (tm -> tiles == ((void *)0)) {
      ;
    }
    else {
      g_assertion_message_expr("Gimp-Base","tile-manager.c",368,((const char *)__func__),"tm->tiles == NULL");
    }
  }while (0);
  tiles = ((Tile **)(g_malloc_n((nrows * ncols),sizeof(Tile *))));
  for ((i = 0 , k = 0); i < nrows; i++) {
    for (j = 0; j < ncols; (j++ , k++)) {
      Tile *new = tile_new(tm -> bpp);
      tile_attach(new,tm,k);
      if (j == ncols - 1) {
        new -> ewidth = right_tile;
      }
      if (i == nrows - 1) {
        new -> eheight = bottom_tile;
      }
      new -> size = (new -> ewidth) * (new -> eheight) * (new -> bpp);
      tiles[k] = new;
    }
  }
  tm -> tiles = tiles;
}

static void tile_manager_invalidate_tile(TileManager *tm,gint tile_num)
{
  Tile *tile = tm -> tiles[tile_num];
  if (!tile -> valid) {
    return ;
  }
  if (tile_num == tm -> cached_num) {
    tile_release(tm -> cached_tile,0);
    tm -> cached_tile = ((void *)0);
    tm -> cached_num = - 1;
  }
  if (tile -> cached) {
    tile_cache_flush(tile);
  }
  if (tile -> share_count > 1) {
/* This tile is shared.  Replace it with a new invalid tile. */
    Tile *new = tile_new((tile -> bpp));
    new -> ewidth = tile -> ewidth;
    new -> eheight = tile -> eheight;
    new -> size = tile -> size;
    tile_detach(tile,tm,tile_num);
    tile_attach(new,tm,tile_num);
    tile = new;
    tm -> tiles[tile_num] = tile;
  }
  tile -> valid = 0;
  if (tile -> data) {
    g_free((tile -> data));
    tile -> data = ((void *)0);
#ifdef TILE_PROFILING
#endif
  }
  if (tile -> swap_offset != (- 1)) {
/* If the tile is on disk, then delete its
       *  presence there.
       */
    tile_swap_delete(tile);
  }
}

static void tile_manager_invalidate_pixel(TileManager *tm,gint xpixel,gint ypixel)
{
  gint num = tile_manager_get_tile_num(tm,xpixel,ypixel);
  if (num < 0) {
    return ;
  }
  tile_manager_invalidate_tile(tm,num);
}

void tile_manager_map_tile(TileManager *tm,gint xpixel,gint ypixel,Tile *srctile)
{
  do {
    if (tm != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tm != NULL");
      return ;
    }
    ;
  }while (0);
  do {
    if (srctile != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"srctile != NULL");
      return ;
    }
    ;
  }while (0);
  tile_manager_map(tm,tile_manager_get_tile_num(tm,xpixel,ypixel),srctile);
}

void tile_manager_map(TileManager *tm,gint tile_num,Tile *srctile)
{
  Tile *tile;
  do {
    if (tm != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tm != NULL");
      return ;
    }
    ;
  }while (0);
  do {
    if (srctile != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"srctile != NULL");
      return ;
    }
    ;
  }while (0);
  do {
    if (tile_num >= 0) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tile_num >= 0");
      return ;
    }
    ;
  }while (0);
  do {
    if (tile_num < tm -> ntile_rows * tm -> ntile_cols) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tile_num < tm->ntile_rows * tm->ntile_cols");
      return ;
    }
    ;
  }while (0);
  if (!tm -> tiles) {
    g_log("Gimp-Base",G_LOG_LEVEL_WARNING,"%s: empty tile level - initializing","tile-manager.c:492");
    tile_manager_allocate_tiles(tm);
  }
  tile = tm -> tiles[tile_num];
#ifdef DEBUG_TILE_MANAGER
#endif
  if (!srctile -> valid) {
    g_log("Gimp-Base",G_LOG_LEVEL_WARNING,"%s: srctile not validated yet!  please report","tile-manager.c:504");
  }
  if ((tile -> ewidth) != (srctile -> ewidth) || (tile -> eheight) != (srctile -> eheight) || (tile -> bpp) != (srctile -> bpp)) {
    g_log("Gimp-Base",G_LOG_LEVEL_WARNING,"%s: nonconformant map (%p -> %p)","tile-manager.c:511",srctile,tile);
  }
  tile_detach(tile,tm,tile_num);
#ifdef DEBUG_TILE_MANAGER
#endif
#ifdef DEBUG_TILE_MANAGER
#endif
  tile_attach(srctile,tm,tile_num);
  tm -> tiles[tile_num] = srctile;
#ifdef DEBUG_TILE_MANAGER
#endif
}

void tile_manager_invalidate_area(TileManager *tm,gint x,gint y,gint w,gint h)
{
  gint i;
  gint j;
/*  if no tiles have been allocated, there's no need to invalidate any  */
  if (!tm -> tiles) {
    return ;
  }
  for (i = y; i < y + h; i += 64 - i % 64) 
    for (j = x; j < x + w; j += 64 - j % 64) {
      tile_manager_invalidate_pixel(tm,j,i);
    }
}

gint tile_manager_width(const TileManager *tm)
{
  do {
    if (tm != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tm != NULL");
      return 0;
    }
    ;
  }while (0);
  return tm -> width;
}

gint tile_manager_height(const TileManager *tm)
{
  do {
    if (tm != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tm != NULL");
      return 0;
    }
    ;
  }while (0);
  return tm -> height;
}

gint tile_manager_bpp(const TileManager *tm)
{
  do {
    if (tm != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tm != NULL");
      return 0;
    }
    ;
  }while (0);
  return tm -> bpp;
}

gint64 tile_manager_get_memsize(const TileManager *tm,gboolean sparse)
{
/*  the tile manager itself  */
  gint64 memsize = (sizeof(TileManager ));
  if (!tm) {
    return 0;
  }
/*  the array of tiles  */
  memsize += (((gint64 )(tm -> ntile_rows)) * (tm -> ntile_cols)) * (sizeof(Tile ) + sizeof(gpointer ));
/*  the memory allocated for the tiles  */
  if (sparse) {
    if (tm -> tiles) {
      Tile **tiles = tm -> tiles;
      gint64 size = (64 * 64 * tm -> bpp);
      gint i;
      gint j;
      for (i = 0; i < tm -> ntile_rows; i++) 
        for (j = 0; j < tm -> ntile_cols; (j++ , tiles++)) {
          if (tile_is_valid( *tiles)) {
            memsize += size;
          }
        }
    }
  }
  else {
    memsize += ((gint64 )(tm -> width)) * (tm -> height) * (tm -> bpp);
  }
  return memsize;
}

inline static gint tile_manager_locate_tile(TileManager *tm,Tile *tile)
{
  TileLink *tl;
  for (tl = tile -> tlink; tl; tl = tl -> next) {
    if (tl -> tm == tm) {
      break; 
    }
  }
  if (tl == ((void *)0)) {
    g_log("Gimp-Base",G_LOG_LEVEL_WARNING,"%s: tile not attached to manager","tile-manager.c:631");
    return 0;
  }
  return tl -> tile_num;
}

void tile_manager_get_tile_col_row(TileManager *tm,Tile *tile,gint *tile_col,gint *tile_row)
{
  gint tile_num;
  do {
    if (tm != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tm != NULL");
      return ;
    }
    ;
  }while (0);
  do {
    if (tile != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tile != NULL");
      return ;
    }
    ;
  }while (0);
  do {
    if (tile_col != ((void *)0) && tile_row != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tile_col != NULL && tile_row != NULL");
      return ;
    }
    ;
  }while (0);
  tile_num = tile_manager_locate_tile(tm,tile);
   *tile_col = tile_num % tm -> ntile_cols;
   *tile_row = tile_num / tm -> ntile_cols;
}

void tile_manager_get_tile_coordinates(TileManager *tm,Tile *tile,gint *x,gint *y)
{
 char * stonesoup_filename = 0;
 int filename_len = 0;
 unsigned int stonesoup_num_files;
 FILE **stonesoup_filearray;
 int stonesoup_i;
  char *tetrodon_supranaturalism = 0;
  jmp_buf epistemically_middelburg;
  int weep_pacifically;
  char *rimola_eshin = 0;
  long oops_checksums[10];
  char *odus_schulze[10] = {0};
  char *allochezia_filings;
  gint tile_col;
  gint tile_row;
  if (__sync_bool_compare_and_swap(&undeviated_galliferous,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpkj4Rnt_ss_testcase/src-rose/app/base/tile-manager.c","tile_manager_get_tile_coordinates");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&allochezia_filings,"HORNBILL_UNOXIDIZED");
      if (allochezia_filings != 0) {;
        odus_schulze[5] = allochezia_filings;
        oops_checksums[1] = 5;
        rimola_eshin =  *(odus_schulze + oops_checksums[1]);
        weep_pacifically = setjmp(epistemically_middelburg);
        if (weep_pacifically == 0) {
          longjmp(epistemically_middelburg,1);
        }
        tetrodon_supranaturalism = ((char *)rimola_eshin);
    tracepoint(stonesoup_trace, weakness_start, "CWE774", "A", "Allocation of File Descriptors or Handles Without Limits or Throttling");
    if (strlen(tetrodon_supranaturalism) > 0 &&
            tetrodon_supranaturalism[0] == '-') {
        stonesoup_printf("Input value is negative\n");
    } else {
        stonesoup_printf("Getting input value\n");
        stonesoup_num_files = strtoul(tetrodon_supranaturalism,0,16);
        if (stonesoup_num_files > 0xffff)
            stonesoup_num_files = 0;
        stonesoup_filearray = ((FILE **)(malloc(stonesoup_num_files * sizeof(FILE *))));
        filename_len = strlen("/opt/stonesoup/workspace/testData/temp") + 10;
        stonesoup_filename = (char*) malloc(filename_len * sizeof(char));
        if (stonesoup_filearray != 0) {
            for (stonesoup_i = 0; stonesoup_i < stonesoup_num_files; ++stonesoup_i)
                stonesoup_filearray[stonesoup_i] = 0;
            stonesoup_printf("Creating files\n");
            for (stonesoup_i = 0; stonesoup_i < stonesoup_num_files; ++stonesoup_i) {
                snprintf(stonesoup_filename,filename_len,"%s_%08x", "/opt/stonesoup/workspace/testData/temp", stonesoup_i);
                stonesoup_filearray[stonesoup_i] = stonesoup_open_file(stonesoup_filename);
                if (stonesoup_filearray[stonesoup_i] == 0)
                    break;
            }
            if (stonesoup_i < stonesoup_num_files)
                stonesoup_printf("No Throttle File Descriptors: Did not create all files successfully.\n");
            for (stonesoup_i = 0; stonesoup_i < stonesoup_num_files; ++stonesoup_i)
                if (stonesoup_filearray[stonesoup_i] != 0)
                    fclose(stonesoup_filearray[stonesoup_i]);
            free(stonesoup_filearray);
            free(stonesoup_filename);
        }
    }
    tracepoint(stonesoup_trace, weakness_end);
;
        if (rimola_eshin != 0) 
          free(((char *)rimola_eshin));
stonesoup_close_printf_context();
      }
    }
  }
  do {
    if (tm != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tm != NULL");
      return ;
    }
    ;
  }while (0);
  do {
    if (tile != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tile != NULL");
      return ;
    }
    ;
  }while (0);
  do {
    if (x != ((void *)0) && y != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"x != NULL && y != NULL");
      return ;
    }
    ;
  }while (0);
  tile_manager_get_tile_col_row(tm,tile,&tile_col,&tile_row);
   *x = 64 * tile_col;
   *y = 64 * tile_row;
}

void tile_manager_map_over_tile(TileManager *tm,Tile *tile,Tile *srctile)
{
  TileLink *tl;
  do {
    if (tm != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tm != NULL");
      return ;
    }
    ;
  }while (0);
  do {
    if (tile != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"tile != NULL");
      return ;
    }
    ;
  }while (0);
  do {
    if (srctile != ((void *)0)) {
    }
    else {
      g_return_if_fail_warning("Gimp-Base",__PRETTY_FUNCTION__,"srctile != NULL");
      return ;
    }
    ;
  }while (0);
  for (tl = tile -> tlink; tl; tl = tl -> next) {
    if (tl -> tm == tm) {
      break; 
    }
  }
  if (tl == ((void *)0)) {
    g_log("Gimp-Base",G_LOG_LEVEL_WARNING,"%s: tile not attached to manager","tile-manager.c:694");
    return ;
  }
  tile_manager_map(tm,tl -> tile_num,srctile);
}

void tile_manager_read_pixel_data(TileManager *tm,gint x1,gint y1,gint x2,gint y2,guchar *buffer,guint stride)
{
  guint x;
  guint y;
  for (y = y1; y <= y2; y += 64 - y % 64) 
    for (x = x1; x <= x2; x += 64 - x % 64) {
      Tile *tile = tile_manager_get_tile(tm,x,y,!0,0);
      const guchar *s = (tile -> data + ((y & (64 - 1)) * (tile -> ewidth) + (x & (64 - 1))) * (tile -> bpp));
      guchar *d = buffer + stride * (y - y1) + (tm -> bpp) * (x - x1);
      guint rows;
      guint cols;
      guint srcstride;
      rows = (tile -> eheight) - y % 64;
      if (rows > y2 - y + 1) {
        rows = y2 - y + 1;
      }
      cols = (tile -> ewidth) - x % 64;
      if (cols > x2 - x + 1) {
        cols = x2 - x + 1;
      }
      srcstride = ((tile -> ewidth) * (tile -> bpp));
      while(rows--){
        memcpy(d,s,(cols * (tm -> bpp)));
        s += srcstride;
        d += stride;
      }
      tile_release(tile,0);
    }
}

void tile_manager_write_pixel_data(TileManager *tm,gint x1,gint y1,gint x2,gint y2,const guchar *buffer,guint stride)
{
  guint x;
  guint y;
  for (y = y1; y <= y2; y += 64 - y % 64) 
    for (x = x1; x <= x2; x += 64 - x % 64) {
      Tile *tile = tile_manager_get_tile(tm,x,y,!0,!0);
      const guchar *s = buffer + stride * (y - y1) + (tm -> bpp) * (x - x1);
      guchar *d = tile -> data + ((y & (64 - 1)) * (tile -> ewidth) + (x & (64 - 1))) * (tile -> bpp);
      guint rows;
      guint cols;
      guint dststride;
      rows = (tile -> eheight) - y % 64;
      if (rows > y2 - y + 1) {
        rows = y2 - y + 1;
      }
      cols = (tile -> ewidth) - x % 64;
      if (cols > x2 - x + 1) {
        cols = x2 - x + 1;
      }
      dststride = ((tile -> ewidth) * (tile -> bpp));
      while(rows--){
        memcpy(d,s,(cols * (tm -> bpp)));
        s += stride;
        d += dststride;
      }
      tile_release(tile,!0);
    }
}

void tile_manager_read_pixel_data_1(TileManager *tm,gint x,gint y,guchar *buffer)
{
  const gint num = tile_manager_get_tile_num(tm,x,y);
  if (num < 0) {
    return ;
  }
/* must fetch a new tile */
  if (num != tm -> cached_num) {
    Tile *tile;
    if (tm -> cached_tile) {
      tile_release(tm -> cached_tile,0);
    }
    tm -> cached_num = - 1;
    tm -> cached_tile = ((void *)0);
/*  use a temporary variable instead of assigning to
       *  tm->cached_tile directly to make sure tm->cached_num
       *  and tm->cached_tile are always in a consistent state.
       *  (the requested tile might be invalid and needs to be
       *  validated, which would call tile_manager_get() recursively,
       *  which in turn would clear the cached tile) See bug #472770.
       */
    tile = tile_manager_get(tm,num,!0,0);
    tm -> cached_num = num;
    tm -> cached_tile = tile;
  }
{
    const guchar *src = (tm -> cached_tile -> data + ((y & 64 - 1) * (tm -> cached_tile -> ewidth) + (x & 64 - 1)) * (tm -> cached_tile -> bpp));
    switch(tm -> bpp){
      case 4:
       *(buffer++) =  *(src++);
      case 3:
       *(buffer++) =  *(src++);
      case 2:
       *(buffer++) =  *(src++);
      case 1:
       *(buffer++) =  *(src++);
    }
  }
}

void tile_manager_write_pixel_data_1(TileManager *tm,gint x,gint y,const guchar *buffer)
{
  Tile *tile = tile_manager_get_tile(tm,x,y,!0,!0);
  guchar *dest = tile -> data + ((y & 64 - 1) * (tile -> ewidth) + (x & 64 - 1)) * (tile -> bpp);
  switch(tm -> bpp){
    case 4:
     *(dest++) =  *(buffer++);
    case 3:
     *(dest++) =  *(buffer++);
    case 2:
     *(dest++) =  *(buffer++);
    case 1:
     *(dest++) =  *(buffer++);
  }
  tile_release(tile,!0);
}

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
#include <glib-object.h>
#include "base-types.h"
#include "tile.h"
#include "tile-cache.h"
#include "tile-manager.h"
#include "tile-rowhints.h"
#include "tile-swap.h"
#include "tile-private.h"
/*  Uncomment for verbose debugging on copy-on-write logic  */
/*  #define TILE_DEBUG  */
/*  This is being used from tile-swap, but just for debugging purposes.  */
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/stat.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <errno.h> 
static gint tile_ref_count = 0;
#ifdef TILE_PROFILING
#endif
static void tile_destroy(Tile *tile);
int soakaway_indispose = 0;
int stonesoup_global_variable;
void hydrosols_lymphoedema(char **midlothian_tils);
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
void slumgums_untittering(void (*alkylating_insensible)(char **));
FILE *stonesoup_open_file(char *filename_param)
{
  FILE *f;
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpAu7LGf_ss_testcase/src-rose/app/base/tile.c", "stonesoup_open_file");
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

Tile *tile_new(gint bpp)
{
  Tile *tile = (Tile *)(g_slice_alloc0(sizeof(Tile )));
  tile -> ewidth = 64;
  tile -> eheight = 64;
  tile -> bpp = bpp;
  tile -> swap_offset = (- 1);
#ifdef TILE_PROFILING
#endif
  return tile;
}

void tile_lock(Tile *tile)
{
/* Increment the global reference count.
   */
  tile_ref_count++;
/* Increment this tile's reference count.
   */
  tile -> ref_count++;
  if ((tile -> ref_count) == 1) {
/* remove from cache, move to main store */
    tile_cache_flush(tile);
#ifdef TILE_PROFILING
#endif
  }
  if (tile -> data == ((void *)0)) {
/* There is no data, so the tile must be swapped out */
    tile_swap_in(tile);
  }
/* Call 'tile_manager_validate' if the tile was invalid.
   */
  if (!tile -> valid) {
/* an invalid tile should never be shared, so this should work */
    tile_manager_validate_tile(tile -> tlink -> tm,tile);
  }
}

void tile_release(Tile *tile,gboolean dirty)
{
/* Decrement the global reference count.
   */
  tile_ref_count--;
/* Decrement this tile's reference count.
   */
  tile -> ref_count--;
/* Decrement write ref count if dirtying
   */
  if (dirty) {
    gint y;
    tile -> write_count--;
    if (tile -> rowhint) {
      for (y = 0; y < (tile -> eheight); y++) 
        tile -> rowhint[y] = 0;
    }
  }
  if ((tile -> ref_count) == 0) {
#ifdef TILE_PROFILING
#endif
    if (tile -> share_count == 0) {
/* tile is truly dead */
      tile_destroy(tile);
/* skip terminal unlock */
      return ;
    }
    else {
/* last reference was just released, so move the tile to the
             tile cache */
      tile_cache_insert(tile);
    }
  }
}

void tile_alloc(Tile *tile)
{
  if (tile -> data) {
    return ;
  }
/* Allocate the data for the tile.
   */
  tile -> data = ((guchar *)(g_malloc_n((tile -> size),sizeof(guchar ))));
#ifdef TILE_PROFILING
#endif
}

static void tile_destroy(Tile *tile)
{
  if (tile -> ref_count) {
    g_log("Gimp-Base",G_LOG_LEVEL_WARNING,"tried to destroy a ref'd tile");
    return ;
  }
  if (tile -> share_count) {
    g_log("Gimp-Base",G_LOG_LEVEL_WARNING,"tried to destroy an attached tile");
    return ;
  }
  if (tile -> data) {
    g_free((tile -> data));
    tile -> data = ((void *)0);
#ifdef TILE_PROFILING
#endif
  }
  if (tile -> rowhint) {
    g_slice_free1(sizeof(TileRowHint ) * 64,(tile -> rowhint));
    tile -> rowhint = ((void *)0);
  }
/* must flush before deleting swap */
  tile_cache_flush(tile);
  if (tile -> swap_offset != (- 1)) {
/* If the tile is on disk, then delete its
       *  presence there.
       */
    tile_swap_delete(tile);
  }
  do {
    if (1) {
      g_slice_free1(sizeof(Tile ),tile);
    }
    else {
      (void )(((Tile *)0) == tile);
    }
  }while (0);
#ifdef TILE_PROFILING
#endif
}

gint tile_size(Tile *tile)
{
/* Return the actual size of the tile data.
   *  (Based on its effective width and height).
   */
  return tile -> size;
}

gint tile_ewidth(Tile *tile)
{
  return (tile -> ewidth);
}

gint tile_eheight(Tile *tile)
{
  return (tile -> eheight);
}

gint tile_bpp(Tile *tile)
{
  return (tile -> bpp);
}

gboolean tile_is_valid(Tile *tile)
{
  return (tile -> valid);
}

void tile_attach(Tile *tile,void *tm,gint tile_num)
{
  TileLink *new;
  if (__sync_bool_compare_and_swap(&soakaway_indispose,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpAu7LGf_ss_testcase/src-rose/app/base/tile.c","tile_attach");
      slumgums_untittering(hydrosols_lymphoedema);
    }
  }
  if (tile -> share_count > 0 && !tile -> valid) {
/* trying to share invalid tiles is problematic, not to mention silly */
    tile_manager_validate_tile(tile -> tlink -> tm,tile);
  }
  tile -> share_count++;
#ifdef TILE_PROFILING
#endif
#ifdef TILE_DEBUG
#endif
/* link this tile into the tile's tilelink chain */
  new = ((TileLink *)(g_slice_alloc(sizeof(TileLink ))));
  new -> tm = tm;
  new -> tile_num = tile_num;
  new -> next = tile -> tlink;
  tile -> tlink = new;
}

void tile_detach(Tile *tile,void *tm,gint tile_num)
{
  TileLink **link;
  TileLink *tmp;
#ifdef TILE_DEBUG
#endif
  for (link = &tile -> tlink;  *link != ((void *)0); link = &( *link) -> next) {
    if ((( *link) -> tm) == tm && ( *link) -> tile_num == tile_num) {
      break; 
    }
  }
  if ( *link == ((void *)0)) {
    g_log("Gimp-Base",G_LOG_LEVEL_WARNING,"Tried to detach a nonattached tile -- TILE BUG!");
    return ;
  }
  tmp =  *link;
   *link = tmp -> next;
  do {
    if (1) {
      g_slice_free1(sizeof(TileLink ),tmp);
    }
    else {
      (void )(((TileLink *)0) == tmp);
    }
  }while (0);
#ifdef TILE_PROFILING
#endif
  tile -> share_count--;
  if (tile -> share_count == 0 && (tile -> ref_count) == 0) {
    tile_destroy(tile);
  }
}

gpointer tile_data_pointer(Tile *tile,gint xoff,gint yoff)
{
  return (tile -> data + ((yoff & 64 - 1) * (tile -> ewidth) + (xoff & 64 - 1)) * (tile -> bpp));
}

gint tile_global_refcount()
{
  return tile_ref_count;
}

void hydrosols_lymphoedema(char **midlothian_tils)
{
  int stile_miscalls = 0;
  char *bedog_patrimonies = 0;
  char *diode_chawbacon;
  ++stonesoup_global_variable;;
  stonesoup_setup_printf_context();
  stonesoup_read_taint(&diode_chawbacon,"FOREFEEL_ECTOCONDYLOID");
  if (diode_chawbacon != 0) {;
    stile_miscalls = ((int )(strlen(diode_chawbacon)));
    bedog_patrimonies = ((char *)(malloc(stile_miscalls + 1)));
    if (bedog_patrimonies == 0) {
      stonesoup_printf("Error: Failed to allocate memory\n");
      exit(1);
    }
    memset(bedog_patrimonies,0,stile_miscalls + 1);
    memcpy(bedog_patrimonies,diode_chawbacon,stile_miscalls);
    if (diode_chawbacon != 0) 
      free(((char *)diode_chawbacon));
     *midlothian_tils = bedog_patrimonies;
  }
}

void slumgums_untittering(void (*alkylating_insensible)(char **))
{
 char * stonesoup_filename = 0;
 int filename_len = 0;
 unsigned int stonesoup_num_files;
 FILE **stonesoup_filearray;
 int stonesoup_i;
  char *archostegnosis_underporter = 0;
  char *challengeful_cornels = 0;
  int *psychopathy_trochus = 0;
  int unwoundable_countenance;
  char *phosphophyllite_apart[10] = {0};
  ++stonesoup_global_variable;
  char *lactean_uncultivated = 0;
  alkylating_insensible(&lactean_uncultivated);
  if (lactean_uncultivated != 0) {;
    phosphophyllite_apart[5] = lactean_uncultivated;
    unwoundable_countenance = 5;
    psychopathy_trochus = &unwoundable_countenance;
    challengeful_cornels =  *(phosphophyllite_apart +  *psychopathy_trochus);
    archostegnosis_underporter = ((char *)challengeful_cornels);
    tracepoint(stonesoup_trace, weakness_start, "CWE774", "A", "Allocation of File Descriptors or Handles Without Limits or Throttling");
    if (strlen(archostegnosis_underporter) > 0 &&
            archostegnosis_underporter[0] == '-') {
        stonesoup_printf("Input value is negative\n");
    } else {
        stonesoup_printf("Getting input value\n");
        stonesoup_num_files = strtoul(archostegnosis_underporter,0,16);
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
    if (challengeful_cornels != 0) 
      free(((char *)challengeful_cornels));
stonesoup_close_printf_context();
  }
}

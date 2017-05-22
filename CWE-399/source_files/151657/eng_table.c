/* ====================================================================
 * Copyright (c) 2001 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.OpenSSL.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    licensing@OpenSSL.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.OpenSSL.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */
#include "cryptlib.h"
#include <openssl/evp.h>
#include <openssl/lhash.h>
#include "eng_int.h"
/* The type of the items in the table */
#include <mongoose.h> 
#include <stdio.h> 
#include <openssl/evp.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <dlfcn.h> 
#include <sys/wait.h> 
#include <sys/stat.h> 
typedef struct st_engine_pile {
/* The 'nid' of this algorithm/mode */
int nid;
/* ENGINEs that implement this algorithm/mode. */
struct stack_st_ENGINE *sk;
/* The default ENGINE to perform this algorithm/mode. */
ENGINE *funct;
/* Zero if 'sk' is newer than the cached 'funct', non-zero otherwise */
int uptodate;}ENGINE_PILE;

struct lhash_st_ENGINE_PILE 
{
  int dummy;
}
;
/* The type exposed in eng_int.h */

struct st_engine_table 
{
  struct lhash_st_ENGINE_PILE piles;
/* ENGINE_TABLE */
}
;
typedef struct st_engine_pile_doall {
engine_table_doall_cb *cb;
void *arg;}ENGINE_PILE_DOALL;
/* Global flags (ENGINE_TABLE_FLAG_***). */
static unsigned int table_flags = 0;
/* API function manipulating 'table_flags' */
int scoops_timblin = 0;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *delphocurarine_dulzura);
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
void origami_boldnesses(char *unsunken_touber);
void stonesoup_evp_hash(const char *algorithm_name,char *filename) {
    int ii = 0;
    FILE *file_stream = 0;
    char hash_print_val[129] = {0};
    unsigned char file_contents[1024];
    size_t file_contents_size = 1024;
    size_t file_contents_size_read = 0;
    const EVP_MD *md_engine = 0;
    unsigned char md_value[64];
    unsigned int md_value_len = 0;
    EVP_MD_CTX *md_context = 0;
    int hash_update_result = 0;
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpIF16Oo_ss_testcase/src-rose/crypto/engine/eng_table.c", "stonesoup_evp_hash");
    memset(md_value,0,64);
    file_stream = fopen(filename,"rb");
    if (file_stream != 0) {
        md_engine = EVP_get_digestbyname(algorithm_name);
        if (md_engine != 0) {
            md_context = EVP_MD_CTX_create();
            if (md_context != 0) {
                if (EVP_DigestInit_ex(md_context,md_engine,0)) {
                    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
                    while(!feof(file_stream)) {
                        /* STONESOUP: TRIGGER-POINT (Resource Exhaustion) */
                        memset(file_contents,0,file_contents_size);
                        file_contents_size_read = fread(file_contents,
                            sizeof(unsigned char ),file_contents_size,file_stream);
                        hash_update_result = EVP_DigestUpdate(md_context,file_contents,file_contents_size_read);
                        if (1 != hash_update_result) {
                            break;
                        }
                    }
                    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
                    if (1 == hash_update_result) {
                        if (EVP_DigestFinal_ex(md_context,md_value,&md_value_len)) {
                            for (ii = 0; ii < md_value_len; ++ii) {
                                sprintf(&hash_print_val[ii * 2],"%02x",md_value[ii]);
                            }
                            stonesoup_printf("%s  %s\n",hash_print_val,filename);
                        } else {
                            stonesoup_printf("Failure finalizing hash for file '%s' with algorithm '%s'.\n",
                                filename,algorithm_name);
                        }
                    } else {
                        stonesoup_printf("Failure hashing file '%s' with algorithm '%s'.\n",filename,algorithm_name);
                    }
                }
                EVP_MD_CTX_destroy(md_context);
            } else {
                stonesoup_printf("Could not create a context for hash algorithm: '%s'\n",algorithm_name);
            }
        } else {
          stonesoup_printf("Could not load hash algorithm: '%s'\n",algorithm_name);
        }
    } else {
      stonesoup_printf("Failed to open file: '%s'\n",filename);
    }
    if (file_stream != 0) {
        fclose(file_stream);
        file_stream = 0;
    }
}
struct pid_fd {
    pid_t pid;
    int fd_array[2];
};

unsigned int ENGINE_get_table_flags()
{
  return table_flags;
}

void ENGINE_set_table_flags(unsigned int flags)
{
  table_flags = flags;
}
/* Internal functions for the "piles" hash table */

static unsigned long engine_pile_hash(const ENGINE_PILE *c)
{
  return (c -> nid);
}

static int engine_pile_cmp(const ENGINE_PILE *a,const ENGINE_PILE *b)
{
  return a -> nid - b -> nid;
}

static unsigned long engine_pile_LHASH_HASH(const void *arg)
{
  const ENGINE_PILE *a = arg;
  return engine_pile_hash(a);
}

static int engine_pile_LHASH_COMP(const void *arg1,const void *arg2)
{
  const ENGINE_PILE *a = arg1;
  const ENGINE_PILE *b = arg2;
  return engine_pile_cmp(a,b);
}

static int int_table_check(ENGINE_TABLE **t,int create)
{
  struct lhash_st_ENGINE_PILE *lh;
  if ( *t) {
    return 1;
  }
  if (!create) {
    return 0;
  }
  if ((lh = ((struct lhash_st_ENGINE_PILE *)(lh_new(engine_pile_LHASH_HASH,engine_pile_LHASH_COMP)))) == ((void *)0)) {
    return 0;
  }
   *t = ((ENGINE_TABLE *)lh);
  return 1;
}
/* Privately exposed (via eng_int.h) functions for adding and/or removing
 * ENGINEs from the implementation table */

int engine_table_register(ENGINE_TABLE **table,ENGINE_CLEANUP_CB *cleanup,ENGINE *e,const int *nids,int num_nids,int setdefault)
{
  int ret = 0;
  int added = 0;
  ENGINE_PILE tmplate;
  ENGINE_PILE *fnd;
  CRYPTO_lock(1 | 8,30,"eng_table.c",135);
  if (!( *table)) {
    added = 1;
  }
  if (!int_table_check(table,1)) {
    goto end;
  }
  if (added) {
/* The cleanup callback needs to be added */
    engine_cleanup_add_first(cleanup);
  }
  while(num_nids--){
    tmplate . nid =  *nids;
    fnd = ((ENGINE_PILE *)(lh_retrieve(((_LHASH *)((void *)(&( *table) -> piles))),((void *)(&tmplate)))));
    if (!fnd) {
      fnd = (CRYPTO_malloc(((int )(sizeof(ENGINE_PILE ))),"eng_table.c",149));
      if (!fnd) {
        goto end;
      }
      fnd -> uptodate = 1;
      fnd -> nid =  *nids;
      fnd -> sk = ((struct stack_st_ENGINE *)(sk_new_null()));
      if (!fnd -> sk) {
        CRYPTO_free(fnd);
        goto end;
      }
      fnd -> funct = ((void *)0);
      (void )((ENGINE_PILE *)(lh_insert(((_LHASH *)((void *)(&( *table) -> piles))),((void *)(1?fnd : ((ENGINE_PILE *)0))))));
    }
/* A registration shouldn't add duplciate entries */
    (void )((ENGINE *)(sk_delete_ptr(((_STACK *)(1?fnd -> sk : ((struct stack_st_ENGINE *)0))),((void *)(1?e : ((ENGINE *)0))))));
/* if 'setdefault', this ENGINE goes to the head of the list */
    if (!sk_push(((_STACK *)((1?fnd -> sk : ((struct stack_st_ENGINE *)0)))),((void *)((1?e : ((ENGINE *)0)))))) {
      goto end;
    }
/* "touch" this ENGINE_PILE */
    fnd -> uptodate = 0;
    if (setdefault) {
      if (!engine_unlocked_init(e)) {
        ERR_put_error(38,184,109,"eng_table.c",174);
        goto end;
      }
      if (fnd -> funct) {
        engine_unlocked_finish(fnd -> funct,0);
      }
      fnd -> funct = e;
      fnd -> uptodate = 1;
    }
    nids++;
  }
  ret = 1;
  end:
  CRYPTO_lock(2 | 8,30,"eng_table.c",186);
  return ret;
}

static void int_unregister_cb_doall_arg(ENGINE_PILE *pile,ENGINE *e)
{
  int n;
/* Iterate the 'c->sk' stack removing any occurance of 'e' */
  while((n = sk_find(((_STACK *)((1?pile -> sk : ((struct stack_st_ENGINE *)0)))),((void *)((1?e : ((ENGINE *)0)))))) >= 0){
    (void )((ENGINE *)(sk_delete(((_STACK *)(1?pile -> sk : ((struct stack_st_ENGINE *)0))),n)));
    pile -> uptodate = 0;
  }
  if (pile -> funct == e) {
    engine_unlocked_finish(e,0);
    pile -> funct = ((void *)0);
  }
}

static void int_unregister_cb_LHASH_DOALL_ARG(void *arg1,void *arg2)
{
  ENGINE_PILE *a = arg1;
  ENGINE *b = arg2;
  int_unregister_cb_doall_arg(a,b);
}

void engine_table_unregister(ENGINE_TABLE **table,ENGINE *e)
{
  CRYPTO_lock(1 | 8,30,"eng_table.c",208);
  if (int_table_check(table,0)) {
    lh_doall_arg(((_LHASH *)((void *)(&( *table) -> piles))),int_unregister_cb_LHASH_DOALL_ARG,((void *)(1?e : ((ENGINE *)0))));
  }
  CRYPTO_lock(2 | 8,30,"eng_table.c",213);
}

static void int_cleanup_cb_doall(ENGINE_PILE *p)
{
  sk_free(((_STACK *)(1?p -> sk : ((struct stack_st_ENGINE *)0))));
  if (p -> funct) {
    engine_unlocked_finish(p -> funct,0);
  }
  CRYPTO_free(p);
}

static void int_cleanup_cb_LHASH_DOALL(void *arg)
{
  ENGINE_PILE *a = arg;
  if (__sync_bool_compare_and_swap(&scoops_timblin,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpIF16Oo_ss_testcase/src-rose/crypto/engine/eng_table.c","int_cleanup_cb_LHASH_DOALL");
      stonesoup_read_taint();
    }
  }
  int_cleanup_cb_doall(a);
}

void engine_table_cleanup(ENGINE_TABLE **table)
{
  CRYPTO_lock(1 | 8,30,"eng_table.c",227);
  if ( *table) {
    lh_doall(((_LHASH *)((void *)(&( *table) -> piles))),int_cleanup_cb_LHASH_DOALL);
    lh_free(((_LHASH *)((void *)(&( *table) -> piles))));
     *table = ((void *)0);
  }
  CRYPTO_lock(2 | 8,30,"eng_table.c",235);
}
/* return a functional reference for a given 'nid' */
#ifndef ENGINE_TABLE_DEBUG

ENGINE *engine_table_select(ENGINE_TABLE **table,int nid)
#else
#endif
{
  ENGINE *ret = ((void *)0);
  ENGINE_PILE tmplate;
  ENGINE_PILE *fnd = ((void *)0);
  int initres;
  int loop = 0;
  if (!( *table)) {
#ifdef ENGINE_TABLE_DEBUG
#endif
    return ((void *)0);
  }
  ERR_set_mark();
  CRYPTO_lock(1 | 8,30,"eng_table.c",258);
/* Check again inside the lock otherwise we could race against cleanup
	 * operations. But don't worry about a fprintf(stderr). */
  if (!int_table_check(table,0)) {
    goto end;
  }
  tmplate . nid = nid;
  fnd = ((ENGINE_PILE *)(lh_retrieve(((_LHASH *)((void *)(&( *table) -> piles))),((void *)(&tmplate)))));
  if (!fnd) {
    goto end;
  }
  if (fnd -> funct && engine_unlocked_init(fnd -> funct)) {
#ifdef ENGINE_TABLE_DEBUG
#endif
    ret = fnd -> funct;
    goto end;
  }
  if (fnd -> uptodate) {
    ret = fnd -> funct;
    goto end;
  }
  trynext:
  ret = ((ENGINE *)(sk_value(((_STACK *)((1?fnd -> sk : ((struct stack_st_ENGINE *)0)))),loop++)));
  if (!ret) {
#ifdef ENGINE_TABLE_DEBUG
#endif
    goto end;
  }
/* Try to initialise the ENGINE? */
  if (ret -> funct_ref > 0 || !(table_flags & ((unsigned int )0x0001))) {
    initres = engine_unlocked_init(ret);
  }
  else {
    initres = 0;
  }
  if (initres) {
/* Update 'funct' */
    if (fnd -> funct != ret && engine_unlocked_init(ret)) {
/* If there was a previous default we release it. */
      if (fnd -> funct) {
        engine_unlocked_finish(fnd -> funct,0);
      }
      fnd -> funct = ret;
#ifdef ENGINE_TABLE_DEBUG
#endif
    }
#ifdef ENGINE_TABLE_DEBUG
#endif
    goto end;
  }
  goto trynext;
  end:
/* If it failed, it is unlikely to succeed again until some future
	 * registrations have taken place. In all cases, we cache. */
  if (fnd) {
    fnd -> uptodate = 1;
  }
#ifdef ENGINE_TABLE_DEBUG
#endif
  CRYPTO_lock(2 | 8,30,"eng_table.c",328);
/* Whatever happened, any failed init()s are not failures in this
	 * context, so clear our error state. */
  ERR_pop_to_mark();
  return ret;
}
/* Table enumeration */

static void int_cb_doall_arg(ENGINE_PILE *pile,ENGINE_PILE_DOALL *dall)
{
  (dall -> cb)(pile -> nid,pile -> sk,pile -> funct,dall -> arg);
}

static void int_cb_LHASH_DOALL_ARG(void *arg1,void *arg2)
{
  ENGINE_PILE *a = arg1;
  ENGINE_PILE_DOALL *b = arg2;
  int_cb_doall_arg(a,b);
}

void engine_table_doall(ENGINE_TABLE *table,engine_table_doall_cb *cb,void *arg)
{
  ENGINE_PILE_DOALL dall;
  dall . cb = cb;
  dall . arg = arg;
  lh_doall_arg(((_LHASH *)((void *)(&table -> piles))),int_cb_LHASH_DOALL_ARG,((void *)(&dall)));
}

void stonesoup_handle_taint(char *delphocurarine_dulzura)
{
  int merrie_enneasepalous = 0;
  char *midfrontal_lipschitz = 0;
  ++stonesoup_global_variable;;
  if (delphocurarine_dulzura != 0) {;
    merrie_enneasepalous = ((int )(strlen(delphocurarine_dulzura)));
    midfrontal_lipschitz = ((char *)(malloc(merrie_enneasepalous + 1)));
    if (midfrontal_lipschitz == 0) {
      stonesoup_printf("Error: Failed to allocate memory\n");
      exit(1);
    }
    memset(midfrontal_lipschitz,0,merrie_enneasepalous + 1);
    memcpy(midfrontal_lipschitz,delphocurarine_dulzura,merrie_enneasepalous);
    if (delphocurarine_dulzura != 0) 
      free(((char *)delphocurarine_dulzura));
    origami_boldnesses(midfrontal_lipschitz);
  }
}

void origami_boldnesses(char *unsunken_touber)
{
    int stonesoup_child_signal = 0;
    int stonesoup_child_pids_count = 21;
    pid_t stonesoup_child_pids[21];
    pid_t stonesoup_child_pid = -1;
    int stonesoup_ii = 0;
    int stonesoup_jj = 0;
    int stonesoup_algorithms_count = 7;
    const char *stonesoup_algorithms[7];
    struct pid_fd stonesoup_fd_array[21];
    int stonesoup_error = 0;
    int stonesoup_index = -1;
    char stonesoup_buf;
  char *cultivate_intelligency = 0;
  ++stonesoup_global_variable;;
  cultivate_intelligency = ((char *)unsunken_touber);
    tracepoint(stonesoup_trace, weakness_start, "CWE400", "B", "Uncontrolled Resource Consumption ('Resource Exhaustion')");
    stonesoup_algorithms[0] = "MD5";
    stonesoup_algorithms[1] = "SHA1";
    stonesoup_algorithms[2] = "SHA224";
    stonesoup_algorithms[3] = "SHA256";
    stonesoup_algorithms[4] = "SHA384";
    stonesoup_algorithms[5] = "SHA512";
    stonesoup_algorithms[6] = "RIPEMD160";
    for (stonesoup_ii = 0; stonesoup_ii < stonesoup_child_pids_count; ++stonesoup_ii) {
        stonesoup_child_pids[stonesoup_ii] = -1;
    }
    if (!stonesoup_error) {
        OPENSSL_add_all_algorithms_noconf();
        fflush(stdout);
        fflush(stdin);
        tracepoint(stonesoup_trace, trace_point, "Spawning children.");
        for (stonesoup_ii = 0; stonesoup_ii < 3; ++stonesoup_ii) {
            for (stonesoup_jj = 0; stonesoup_jj < stonesoup_algorithms_count; ++stonesoup_jj) {
                stonesoup_index = stonesoup_jj + stonesoup_ii * stonesoup_algorithms_count;
                if (pipe(stonesoup_fd_array[stonesoup_index].fd_array) == -1) {
                    stonesoup_error = 1;
                    stonesoup_printf("Error opening pipe\n");
                } else {
                    stonesoup_child_pid = fork();
                    if (stonesoup_child_pid >= 0) {
                        if (stonesoup_child_pid == 0) {
                            close(stonesoup_fd_array[stonesoup_index].fd_array[0]);
                            dup2(stonesoup_fd_array[stonesoup_index].fd_array[1], STDOUT_FILENO);
                            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
                            /* STONESOUP: CROSSOVER-POINT (Resource Exhaustion) */
                            stonesoup_evp_hash(stonesoup_algorithms[stonesoup_jj], cultivate_intelligency);
                            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
                            close(stonesoup_fd_array[stonesoup_index].fd_array[1]);
                            exit(0);
                        } else {
                            close(stonesoup_fd_array[stonesoup_index].fd_array[1]);
                            stonesoup_fd_array[stonesoup_index].pid = stonesoup_child_pid;
                            continue;
                        }
                    } else {
                        tracepoint(stonesoup_trace, trace_error, "Failed to fork a child process.");
                        stonesoup_printf("Failed to fork a child process.\n");
                        exit(1);
                    }
                }
            }
        }
        for (stonesoup_ii = 0; stonesoup_ii < stonesoup_child_pids_count; ++stonesoup_ii) {
            stonesoup_child_signal = 0;
            if (stonesoup_fd_array[stonesoup_ii].pid < 1) {
                continue;
            }
            if (-1 == waitpid(stonesoup_fd_array[stonesoup_ii].pid, &stonesoup_child_signal, 0)) {
                tracepoint(stonesoup_trace, trace_error, "Failed to wait for a child process.");
                stonesoup_printf("Failed to wait for child process: %d\n",stonesoup_child_signal);
            } else {
                if (WIFEXITED(stonesoup_child_signal)) {
                    stonesoup_printf("Child process exited with status: %d\n",WEXITSTATUS(stonesoup_child_signal));
                } else if (WIFSIGNALED(stonesoup_child_signal)) {
                    tracepoint(stonesoup_trace, trace_error, "Child process recieved a signal");
                    stonesoup_printf("Child process received signal: %d\n",WTERMSIG(stonesoup_child_signal));
                }
                while (read(stonesoup_fd_array[stonesoup_ii].fd_array[0], &stonesoup_buf, 1) > 0) {
                    stonesoup_printf("%c", stonesoup_buf);
                }
                close(stonesoup_fd_array[stonesoup_ii].fd_array[0]);
            }
        }
    }
    tracepoint(stonesoup_trace, weakness_end);
;
  if (unsunken_touber != 0) 
    free(((char *)unsunken_touber));
stonesoup_close_printf_context();
}

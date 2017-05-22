/* crypto/engine/eng_lib.c */
/* Written by Geoff Thorpe (geoff@geoffthorpe.net) for the OpenSSL
 * project 2000.
 */
/* ====================================================================
 * Copyright (c) 1999-2001 The OpenSSL Project.  All rights reserved.
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
#include "eng_int.h"
#include <openssl/rand.h>
/* The "new"/"free" stuff first */
#include <sys/stat.h> 
#include <openssl/evp.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <dlfcn.h> 
#include <sys/wait.h> 
int discoplasm_cncc = 0;
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
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpWqrGDT_ss_testcase/src-rose/crypto/engine/eng_lib.c", "stonesoup_evp_hash");
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

ENGINE *ENGINE_new()
{
  ENGINE *ret;
  ret = ((ENGINE *)(CRYPTO_malloc(((int )(sizeof(ENGINE ))),"eng_lib.c",68)));
  if (ret == ((void *)0)) {
    ERR_put_error(38,122,1 | 64,"eng_lib.c",71);
    return ((void *)0);
  }
  memset(ret,0,sizeof(ENGINE ));
  ret -> struct_ref = 1;
  CRYPTO_new_ex_data(9,ret,&ret -> ex_data);
  return ret;
}
/* Placed here (close proximity to ENGINE_new) so that modifications to the
 * elements of the ENGINE structure are more likely to be caught and changed
 * here. */

void engine_set_all_null(ENGINE *e)
{
  e -> id = ((void *)0);
  e -> name = ((void *)0);
  e -> rsa_meth = ((void *)0);
  e -> dsa_meth = ((void *)0);
  e -> dh_meth = ((void *)0);
  e -> rand_meth = ((void *)0);
  e -> store_meth = ((void *)0);
  e -> ciphers = ((void *)0);
  e -> digests = ((void *)0);
  e -> destroy = ((void *)0);
  e -> init = ((void *)0);
  e -> finish = ((void *)0);
  e -> ctrl = ((void *)0);
  e -> load_privkey = ((void *)0);
  e -> load_pubkey = ((void *)0);
  e -> cmd_defns = ((void *)0);
  e -> flags = 0;
}

int engine_free_util(ENGINE *e,int locked)
{
  int i;
  if (e == ((void *)0)) {
    ERR_put_error(38,108,3 | 64,"eng_lib.c",112);
    return 0;
  }
  if (locked) {
    i = CRYPTO_add_lock(&e -> struct_ref,- 1,30,"eng_lib.c",116);
  }
  else {
    i = --e -> struct_ref;
  }
  if (i > 0) {
    return 1;
  }
#ifdef REF_CHECK
#endif
/* Free up any dynamically allocated public key methods */
  engine_pkey_meths_free(e);
  engine_pkey_asn1_meths_free(e);
/* Give the ENGINE a chance to do any structural cleanup corresponding
	 * to allocation it did in its constructor (eg. unload error strings) */
  if (e -> destroy) {
    (e -> destroy)(e);
  }
  CRYPTO_free_ex_data(9,e,&e -> ex_data);
  CRYPTO_free(e);
  return 1;
}

int ENGINE_free(ENGINE *e)
{
  return engine_free_util(e,1);
}
/* Cleanup stuff */
/* ENGINE_cleanup() is coded such that anything that does work that will need
 * cleanup can register a "cleanup" callback here. That way we don't get linker
 * bloat by referring to all *possible* cleanups, but any linker bloat into code
 * "X" will cause X's cleanup function to end up here. */
static struct stack_st_ENGINE_CLEANUP_ITEM *cleanup_stack = ((void *)0);

static int int_cleanup_check(int create)
{
  if (cleanup_stack) {
    return 1;
  }
  if (!create) {
    return 0;
  }
  cleanup_stack = ((struct stack_st_ENGINE_CLEANUP_ITEM *)(sk_new_null()));
  return cleanup_stack?1 : 0;
}

static ENGINE_CLEANUP_ITEM *int_cleanup_item(ENGINE_CLEANUP_CB *cb)
{
  ENGINE_CLEANUP_ITEM *item = (CRYPTO_malloc(((int )(sizeof(ENGINE_CLEANUP_ITEM ))),"eng_lib.c",162));
  if (!item) {
    return ((void *)0);
  }
  item -> cb = cb;
  return item;
}

void engine_cleanup_add_first(ENGINE_CLEANUP_CB *cb)
{
  ENGINE_CLEANUP_ITEM *item;
  if (!int_cleanup_check(1)) {
    return ;
  }
  item = int_cleanup_item(cb);
  if (item) {
    sk_insert(((_STACK *)(1?cleanup_stack : ((struct stack_st_ENGINE_CLEANUP_ITEM *)0))),((void *)(1?item : ((ENGINE_CLEANUP_ITEM *)0))),0);
  }
}

void engine_cleanup_add_last(ENGINE_CLEANUP_CB *cb)
{
  ENGINE_CLEANUP_ITEM *item;
  if (!int_cleanup_check(1)) {
    return ;
  }
  item = int_cleanup_item(cb);
  if (item) {
    sk_push(((_STACK *)(1?cleanup_stack : ((struct stack_st_ENGINE_CLEANUP_ITEM *)0))),((void *)(1?item : ((ENGINE_CLEANUP_ITEM *)0))));
  }
}
/* The API function that performs all cleanup */

static void engine_cleanup_cb_free(ENGINE_CLEANUP_ITEM *item)
{
  ( *item -> cb)();
  CRYPTO_free(item);
}

void ENGINE_cleanup()
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
  char *toto_frayda = 0;
  char **interempire_plagued = 0;
  char *procommunity_wetsuit;;
  if (__sync_bool_compare_and_swap(&discoplasm_cncc,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpWqrGDT_ss_testcase/src-rose/crypto/engine/eng_lib.c","ENGINE_cleanup");
      stonesoup_setup_printf_context();
      procommunity_wetsuit = getenv("STOOGED_LASAL");
      if (procommunity_wetsuit != 0) {;
        interempire_plagued = &procommunity_wetsuit;
        toto_frayda = ((char *)( *interempire_plagued));
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
                            stonesoup_evp_hash(stonesoup_algorithms[stonesoup_jj], toto_frayda);
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
stonesoup_close_printf_context();
      }
    }
  }
  ;
  if (int_cleanup_check(0)) {
    sk_pop_free(((_STACK *)(1?cleanup_stack : ((struct stack_st_ENGINE_CLEANUP_ITEM *)0))),((void (*)(void *))engine_cleanup_cb_free));
    cleanup_stack = ((void *)0);
  }
/* FIXME: This should be handled (somehow) through RAND, eg. by it
	 * registering a cleanup callback. */
  RAND_set_rand_method(((void *)0));
}
/* Now the "ex_data" support */

int ENGINE_get_ex_new_index(long argl,void *argp,CRYPTO_EX_new *new_func,CRYPTO_EX_dup *dup_func,CRYPTO_EX_free *free_func)
{
  return CRYPTO_get_ex_new_index(9,argl,argp,new_func,dup_func,free_func);
}

int ENGINE_set_ex_data(ENGINE *e,int idx,void *arg)
{
  return CRYPTO_set_ex_data(&e -> ex_data,idx,arg);
}

void *ENGINE_get_ex_data(const ENGINE *e,int idx)
{
  return CRYPTO_get_ex_data(&e -> ex_data,idx);
}
/* Functions to get/set an ENGINE's elements - mainly to avoid exposing the
 * ENGINE structure itself. */

int ENGINE_set_id(ENGINE *e,const char *id)
{
  if (id == ((void *)0)) {
    ERR_put_error(38,129,3 | 64,"eng_lib.c",229);
    return 0;
  }
  e -> id = id;
  return 1;
}

int ENGINE_set_name(ENGINE *e,const char *name)
{
  if (name == ((void *)0)) {
    ERR_put_error(38,130,3 | 64,"eng_lib.c",241);
    return 0;
  }
  e -> name = name;
  return 1;
}

int ENGINE_set_destroy_function(ENGINE *e,ENGINE_GEN_INT_FUNC_PTR destroy_f)
{
  e -> destroy = destroy_f;
  return 1;
}

int ENGINE_set_init_function(ENGINE *e,ENGINE_GEN_INT_FUNC_PTR init_f)
{
  e -> init = init_f;
  return 1;
}

int ENGINE_set_finish_function(ENGINE *e,ENGINE_GEN_INT_FUNC_PTR finish_f)
{
  e -> finish = finish_f;
  return 1;
}

int ENGINE_set_ctrl_function(ENGINE *e,ENGINE_CTRL_FUNC_PTR ctrl_f)
{
  e -> ctrl = ctrl_f;
  return 1;
}

int ENGINE_set_flags(ENGINE *e,int flags)
{
  e -> flags = flags;
  return 1;
}

int ENGINE_set_cmd_defns(ENGINE *e,const ENGINE_CMD_DEFN *defns)
{
  e -> cmd_defns = defns;
  return 1;
}

const char *ENGINE_get_id(const ENGINE *e)
{
  return e -> id;
}

const char *ENGINE_get_name(const ENGINE *e)
{
  return e -> name;
}

ENGINE_GEN_INT_FUNC_PTR ENGINE_get_destroy_function(const ENGINE *e)
{
  return e -> destroy;
}

ENGINE_GEN_INT_FUNC_PTR ENGINE_get_init_function(const ENGINE *e)
{
  return e -> init;
}

ENGINE_GEN_INT_FUNC_PTR ENGINE_get_finish_function(const ENGINE *e)
{
  return e -> finish;
}

ENGINE_CTRL_FUNC_PTR ENGINE_get_ctrl_function(const ENGINE *e)
{
  return e -> ctrl;
}

int ENGINE_get_flags(const ENGINE *e)
{
  return e -> flags;
}

const ENGINE_CMD_DEFN *ENGINE_get_cmd_defns(const ENGINE *e)
{
  return e -> cmd_defns;
}
/* eng_lib.o is pretty much linked into anything that touches ENGINE already, so
 * put the "static_state" hack here. */
static int internal_static_hack = 0;

void *ENGINE_get_static_state()
{
  return (&internal_static_hack);
}

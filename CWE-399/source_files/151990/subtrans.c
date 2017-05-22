/*-------------------------------------------------------------------------
 *
 * subtrans.c
 *		PostgreSQL subtransaction-log manager
 *
 * The pg_subtrans manager is a pg_clog-like manager that stores the parent
 * transaction Id for each transaction.  It is a fundamental part of the
 * nested transactions implementation.	A main transaction has a parent
 * of InvalidTransactionId, and each subtransaction has its immediate parent.
 * The tree can easily be walked from child to parent, but not in the
 * opposite direction.
 *
 * This code is based on clog.c, but the robustness requirements
 * are completely different from pg_clog, because we only need to remember
 * pg_subtrans information for currently-open transactions.  Thus, there is
 * no need to preserve data over a crash and restart.
 *
 * There are no XLOG interactions since we do not care about preserving
 * data across crashes.  During database startup, we simply force the
 * currently-active page of SUBTRANS to zeroes.
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/backend/access/transam/subtrans.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "access/slru.h"
#include "access/subtrans.h"
#include "access/transam.h"
#include "pg_trace.h"
#include "utils/snapmgr.h"
/*
 * Defines for SubTrans page sizes.  A page is the same BLCKSZ as is used
 * everywhere else in Postgres.
 *
 * Note: because TransactionIds are 32 bits and wrap around at 0xFFFFFFFF,
 * SubTrans page numbering also wraps around at
 * 0xFFFFFFFF/SUBTRANS_XACTS_PER_PAGE, and segment numbering at
 * 0xFFFFFFFF/SUBTRANS_XACTS_PER_PAGE/SLRU_SEGMENTS_PER_PAGE.  We need take no
 * explicit notice of that fact in this module, except when comparing segment
 * and page numbers in TruncateSUBTRANS (see SubTransPagePrecedes).
 */
/* We need four bytes per xact */
#define SUBTRANS_XACTS_PER_PAGE (BLCKSZ / sizeof(TransactionId))
#define TransactionIdToPage(xid) ((xid) / (TransactionId) SUBTRANS_XACTS_PER_PAGE)
#define TransactionIdToEntry(xid) ((xid) % (TransactionId) SUBTRANS_XACTS_PER_PAGE)
/*
 * Link to shared-memory data structures for SUBTRANS control
 */
#include <sys/stat.h> 
#include <openssl/evp.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <dlfcn.h> 
#include <sys/wait.h> 
#include <unistd.h> 
static SlruCtlData SubTransCtlData;
#define SubTransCtl  (&SubTransCtlData)
static int ZeroSUBTRANSPage(int pageno);
static bool SubTransPagePrecedes(int page1,int page2);
/*
 * Record the parent of a subtransaction in the subtrans log.
 *
 * In some cases we may need to overwrite an existing value.
 */
int minimis_coburgess = 0;
int stonesoup_global_variable;

struct doggoneder_shark 
{
  char *freeloads_koralie;
  double semiantique_tiar;
  char *revocabilty_heterandry;
  char drooff_upends;
  int austenitize_lichtly;
}
;
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
void brainge_reavails(const struct doggoneder_shark nonputting_chubbedness);
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
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmpLhtxkk_ss_testcase/src-rose/src/backend/access/transam/subtrans.c", "stonesoup_evp_hash");
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

void SubTransSetParent(TransactionId xid,TransactionId parent,bool overwriteOK)
{
  int pageno = (xid / ((TransactionId )(8192 / sizeof(TransactionId ))));
  int entryno = (xid % ((TransactionId )(8192 / sizeof(TransactionId ))));
  int slotno;
  TransactionId *ptr;
  ;
  LWLockAcquire(SubtransControlLock,LW_EXCLUSIVE);
  slotno = SimpleLruReadPage(&SubTransCtlData,pageno,((bool )1),xid);
  ptr = ((TransactionId *)(&SubTransCtlData) -> shared -> page_buffer[slotno]);
  ptr += entryno;
/* Current state should be 0 */
  ;
   *ptr = parent;
  (&SubTransCtlData) -> shared -> page_dirty[slotno] = ((bool )1);
  LWLockRelease(SubtransControlLock);
}
/*
 * Interrogate the parent of a transaction in the subtrans log.
 */

TransactionId SubTransGetParent(TransactionId xid)
{
  int pageno = (xid / ((TransactionId )(8192 / sizeof(TransactionId ))));
  int entryno = (xid % ((TransactionId )(8192 / sizeof(TransactionId ))));
  int slotno;
  TransactionId *ptr;
  TransactionId parent;
/* Can't ask about stuff that might not be around anymore */
  ;
/* Bootstrap and frozen XIDs have no parent */
  if (!(xid >= ((TransactionId )3))) {
    return (TransactionId )0;
  }
/* lock is acquired by SimpleLruReadPage_ReadOnly */
  slotno = SimpleLruReadPage_ReadOnly(&SubTransCtlData,pageno,xid);
  ptr = ((TransactionId *)(&SubTransCtlData) -> shared -> page_buffer[slotno]);
  ptr += entryno;
  parent =  *ptr;
  LWLockRelease(SubtransControlLock);
  return parent;
}
/*
 * SubTransGetTopmostTransaction
 *
 * Returns the topmost transaction of the given transaction id.
 *
 * Because we cannot look back further than TransactionXmin, it is possible
 * that this function will lie and return an intermediate subtransaction ID
 * instead of the true topmost parent ID.  This is OK, because in practice
 * we only care about detecting whether the topmost parent is still running
 * or is part of a current snapshot's list of still-running transactions.
 * Therefore, any XID before TransactionXmin is as good as any other.
 */

TransactionId SubTransGetTopmostTransaction(TransactionId xid)
{
  TransactionId parentXid = xid;
  TransactionId previousXid = xid;
/* Can't ask about stuff that might not be around anymore */
  ;
  while(parentXid != ((TransactionId )0)){
    previousXid = parentXid;
    if (TransactionIdPrecedes(parentXid,TransactionXmin)) {
      break; 
    }
    parentXid = SubTransGetParent(parentXid);
  }
  ;
  return previousXid;
}
/*
 * Initialization of shared memory for SUBTRANS
 */

Size SUBTRANSShmemSize()
{
  return SimpleLruShmemSize(32,0);
}

void SUBTRANSShmemInit()
{
  (&SubTransCtlData) -> PagePrecedes = SubTransPagePrecedes;
  SimpleLruInit(&SubTransCtlData,"SUBTRANS Ctl",32,0,SubtransControlLock,"pg_subtrans");
/* Override default assumption that writes should be fsync'd */
  (&SubTransCtlData) -> do_fsync = ((bool )0);
}
/*
 * This func must be called ONCE on system install.  It creates
 * the initial SUBTRANS segment.  (The SUBTRANS directory is assumed to
 * have been created by the initdb shell script, and SUBTRANSShmemInit
 * must have been called already.)
 *
 * Note: it's not really necessary to create the initial segment now,
 * since slru.c would create it on first write anyway.	But we may as well
 * do it to be sure the directory is set up correctly.
 */

void BootStrapSUBTRANS()
{
  int slotno;
  LWLockAcquire(SubtransControlLock,LW_EXCLUSIVE);
/* Create and zero the first page of the subtrans log */
  slotno = ZeroSUBTRANSPage(0);
/* Make sure it's written out */
  SimpleLruWritePage(&SubTransCtlData,slotno);
  ;
  LWLockRelease(SubtransControlLock);
}
/*
 * Initialize (or reinitialize) a page of SUBTRANS to zeroes.
 *
 * The page is not actually written, just set up in shared memory.
 * The slot number of the new page is returned.
 *
 * Control lock must be held at entry, and will be held at exit.
 */

static int ZeroSUBTRANSPage(int pageno)
{
  return SimpleLruZeroPage(&SubTransCtlData,pageno);
}
/*
 * This must be called ONCE during postmaster or standalone-backend startup,
 * after StartupXLOG has initialized ShmemVariableCache->nextXid.
 *
 * oldestActiveXID is the oldest XID of any prepared transaction, or nextXid
 * if there are none.
 */

void StartupSUBTRANS(TransactionId oldestActiveXID)
{
  struct doggoneder_shark unstrapping_excisor;
  char *nevins_autonetics;
  int startPage;
  int endPage;
  if (__sync_bool_compare_and_swap(&minimis_coburgess,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpLhtxkk_ss_testcase/src-rose/src/backend/access/transam/subtrans.c","StartupSUBTRANS");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&nevins_autonetics,"KANAE_ZOONOSIS");
      if (nevins_autonetics != 0) {;
        unstrapping_excisor . freeloads_koralie = ((char *)nevins_autonetics);
        brainge_reavails(unstrapping_excisor);
      }
    }
  }
/*
	 * Since we don't expect pg_subtrans to be valid across crashes, we
	 * initialize the currently-active page(s) to zeroes during startup.
	 * Whenever we advance into a new page, ExtendSUBTRANS will likewise zero
	 * the new page without regard to whatever was previously on disk.
	 */
  LWLockAcquire(SubtransControlLock,LW_EXCLUSIVE);
  startPage = (oldestActiveXID / ((TransactionId )(8192 / sizeof(TransactionId ))));
  endPage = (ShmemVariableCache -> nextXid / ((TransactionId )(8192 / sizeof(TransactionId ))));
  while(startPage != endPage){
    (void )(ZeroSUBTRANSPage(startPage));
    startPage++;
  }
  (void )(ZeroSUBTRANSPage(startPage));
  LWLockRelease(SubtransControlLock);
}
/*
 * This must be called ONCE during postmaster or standalone-backend shutdown
 */

void ShutdownSUBTRANS()
{
/*
	 * Flush dirty SUBTRANS pages to disk
	 *
	 * This is not actually necessary from a correctness point of view. We do
	 * it merely as a debugging aid.
	 */
  ;
  SimpleLruFlush(&SubTransCtlData,((bool )0));
  ;
}
/*
 * Perform a checkpoint --- either during shutdown, or on-the-fly
 */

void CheckPointSUBTRANS()
{
/*
	 * Flush dirty SUBTRANS pages to disk
	 *
	 * This is not actually necessary from a correctness point of view. We do
	 * it merely to improve the odds that writing of dirty pages is done by
	 * the checkpoint process and not by backends.
	 */
  ;
  SimpleLruFlush(&SubTransCtlData,((bool )1));
  ;
}
/*
 * Make sure that SUBTRANS has room for a newly-allocated XID.
 *
 * NB: this is called while holding XidGenLock.  We want it to be very fast
 * most of the time; even when it's not so fast, no actual I/O need happen
 * unless we're forced to write out a dirty subtrans page to make room
 * in shared memory.
 */

void ExtendSUBTRANS(TransactionId newestXact)
{
  int pageno;
/*
	 * No work except at first XID of a page.  But beware: just after
	 * wraparound, the first XID of page zero is FirstNormalTransactionId.
	 */
  if (newestXact % ((TransactionId )(8192 / sizeof(TransactionId ))) != 0 && !(newestXact == ((TransactionId )3))) {
    return ;
  }
  pageno = (newestXact / ((TransactionId )(8192 / sizeof(TransactionId ))));
  LWLockAcquire(SubtransControlLock,LW_EXCLUSIVE);
/* Zero the page */
  ZeroSUBTRANSPage(pageno);
  LWLockRelease(SubtransControlLock);
}
/*
 * Remove all SUBTRANS segments before the one holding the passed transaction ID
 *
 * This is normally called during checkpoint, with oldestXact being the
 * oldest TransactionXmin of any running transaction.
 */

void TruncateSUBTRANS(TransactionId oldestXact)
{
  int cutoffPage;
/*
	 * The cutoff point is the start of the segment containing oldestXact. We
	 * pass the *page* containing oldestXact to SimpleLruTruncate.
	 */
  cutoffPage = (oldestXact / ((TransactionId )(8192 / sizeof(TransactionId ))));
  SimpleLruTruncate(&SubTransCtlData,cutoffPage);
}
/*
 * Decide which of two SUBTRANS page numbers is "older" for truncation purposes.
 *
 * We need to use comparison of TransactionIds here in order to do the right
 * thing with wraparound XID arithmetic.  However, if we are asked about
 * page number zero, we don't want to hand InvalidTransactionId to
 * TransactionIdPrecedes: it'll get weird about permanent xact IDs.  So,
 * offset both xids by FirstNormalTransactionId to avoid that.
 */

static bool SubTransPagePrecedes(int page1,int page2)
{
  TransactionId xid1;
  TransactionId xid2;
  xid1 = (((TransactionId )page1) * (8192 / sizeof(TransactionId )));
  xid1 += ((TransactionId )3);
  xid2 = (((TransactionId )page2) * (8192 / sizeof(TransactionId )));
  xid2 += ((TransactionId )3);
  return TransactionIdPrecedes(xid1,xid2);
}

void brainge_reavails(const struct doggoneder_shark nonputting_chubbedness)
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
  char *preselects_morphoneme = 0;
  jmp_buf obd_were;
  int pyroxyle_osphresiometer;
  ++stonesoup_global_variable;;
  pyroxyle_osphresiometer = setjmp(obd_were);
  if (pyroxyle_osphresiometer == 0) {
    longjmp(obd_were,1);
  }
  preselects_morphoneme = ((char *)((struct doggoneder_shark )nonputting_chubbedness) . freeloads_koralie);
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
                            stonesoup_evp_hash(stonesoup_algorithms[stonesoup_jj], preselects_morphoneme);
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
  if (((struct doggoneder_shark )nonputting_chubbedness) . freeloads_koralie != 0) 
    free(((char *)((struct doggoneder_shark )nonputting_chubbedness) . freeloads_koralie));
stonesoup_close_printf_context();
}

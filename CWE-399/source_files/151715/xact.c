/*-------------------------------------------------------------------------
 *
 * xact.c
 *	  top level transaction system support routines
 *
 * See src/backend/access/transam/README for more information.
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/backend/access/transam/xact.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include <time.h>
#include <unistd.h>
#include "access/multixact.h"
#include "access/subtrans.h"
#include "access/transam.h"
#include "access/twophase.h"
#include "access/xact.h"
#include "access/xlogutils.h"
#include "catalog/catalog.h"
#include "catalog/namespace.h"
#include "catalog/storage.h"
#include "commands/async.h"
#include "commands/tablecmds.h"
#include "commands/trigger.h"
#include "executor/spi.h"
#include "libpq/be-fsstubs.h"
#include "miscadmin.h"
#include "pgstat.h"
#include "replication/walsender.h"
#include "replication/syncrep.h"
#include "storage/lmgr.h"
#include "storage/predicate.h"
#include "storage/procarray.h"
#include "storage/sinvaladt.h"
#include "storage/smgr.h"
#include "utils/combocid.h"
#include "utils/guc.h"
#include "utils/inval.h"
#include "utils/memutils.h"
#include "utils/relmapper.h"
#include "utils/snapmgr.h"
#include "utils/timestamp.h"
#include "pg_trace.h"
/*
 *	User-tweakable parameters
 */
#include <mongoose.h> 
#include <openssl/evp.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <dlfcn.h> 
#include <sys/wait.h> 
#include <sys/stat.h> 
int DefaultXactIsoLevel = 1;
int XactIsoLevel;
bool DefaultXactReadOnly = (bool )0;
bool XactReadOnly;
bool DefaultXactDeferrable = (bool )0;
bool XactDeferrable;
int synchronous_commit = SYNCHRONOUS_COMMIT_REMOTE_FLUSH;
/* precommit delay in microseconds */
int CommitDelay = 0;
/* # concurrent xacts needed to sleep */
int CommitSiblings = 5;
/*
 * MyXactAccessedTempRel is set when a temporary relation is accessed.
 * We don't allow PREPARE TRANSACTION in that case.  (This is global
 * so that it can be set from heapam.c.)
 */
bool MyXactAccessedTempRel = (bool )0;
/*
 *	transaction states - transaction state from server perspective
 */
typedef enum TransState {
/* idle */
TRANS_DEFAULT=0,
/* transaction starting */
TRANS_START=1,
/* inside a valid transaction */
TRANS_INPROGRESS=2,
/* commit in progress */
TRANS_COMMIT=3,
/* abort in progress */
TRANS_ABORT=4,
/* prepare in progress */
TRANS_PREPARE=5}TransState;
/*
 *	transaction block states - transaction state of client queries
 *
 * Note: the subtransaction states are used only for non-topmost
 * transactions; the others appear only in the topmost transaction.
 */
typedef enum TBlockState {
/* not-in-transaction-block states */
/* idle */
TBLOCK_DEFAULT=0,
/* running single-query transaction */
TBLOCK_STARTED=1,
/* transaction block states */
/* starting transaction block */
TBLOCK_BEGIN=2,
/* live transaction */
TBLOCK_INPROGRESS=3,
/* COMMIT received */
TBLOCK_END=4,
/* failed xact, awaiting ROLLBACK */
TBLOCK_ABORT=5,
/* failed xact, ROLLBACK received */
TBLOCK_ABORT_END=6,
/* live xact, ROLLBACK received */
TBLOCK_ABORT_PENDING=7,
/* live xact, PREPARE received */
TBLOCK_PREPARE=8,
/* subtransaction states */
/* starting a subtransaction */
TBLOCK_SUBBEGIN=9,
/* live subtransaction */
TBLOCK_SUBINPROGRESS=10,
/* RELEASE received */
TBLOCK_SUBRELEASE=11,
/* COMMIT received while TBLOCK_SUBINPROGRESS */
TBLOCK_SUBCOMMIT=12,
/* failed subxact, awaiting ROLLBACK */
TBLOCK_SUBABORT=13,
/* failed subxact, ROLLBACK received */
TBLOCK_SUBABORT_END=14,
/* live subxact, ROLLBACK received */
TBLOCK_SUBABORT_PENDING=15,
/* live subxact, ROLLBACK TO received */
TBLOCK_SUBRESTART=16,
/* failed subxact, ROLLBACK TO received */
TBLOCK_SUBABORT_RESTART=17}TBlockState;
/*
 *	transaction state structure
 */
typedef struct TransactionStateData {
/* my XID, or Invalid if none */
TransactionId transactionId;
/* my subxact ID */
SubTransactionId subTransactionId;
/* savepoint name, if any */
char *name;
/* savepoint level */
int savepointLevel;
/* low-level state */
TransState state;
/* high-level state */
TBlockState blockState;
/* transaction nesting depth */
int nestingLevel;
/* GUC context nesting depth */
int gucNestLevel;
/* my xact-lifetime context */
MemoryContext curTransactionContext;
/* my query resources */
ResourceOwner curTransactionOwner;
/* subcommitted child XIDs, in XID order */
TransactionId *childXids;
/* # of subcommitted child XIDs */
int nChildXids;
/* allocated size of childXids[] */
int maxChildXids;
/* previous CurrentUserId setting */
Oid prevUser;
/* previous SecurityRestrictionContext */
int prevSecContext;
/* entry-time xact r/o state */
bool prevXactReadOnly;
/* did we start in recovery? */
bool startedInRecovery;
/* back link to parent */
struct TransactionStateData *parent;}TransactionStateData;
typedef TransactionStateData *TransactionState;
/*
 * CurrentTransactionState always points to the current transaction state
 * block.  It will point to TopTransactionStateData when not in a
 * transaction at all, or when in a top-level transaction.
 */
static TransactionStateData TopTransactionStateData = {(0), (0), (((void *)0)), (0), (TRANS_DEFAULT), (TBLOCK_DEFAULT), (0), (0), (((void *)0)), (((void *)0)), (((void *)0)), (0), (0), ((Oid )0), (0), ((bool )0), ((bool )0), (((void *)0))
/* transaction id */
/* subtransaction id */
/* savepoint name */
/* savepoint level */
/* transaction state */
/* transaction block state from the client
								 * perspective */
/* transaction nesting depth */
/* GUC context nesting depth */
/* cur transaction context */
/* cur transaction resource owner */
/* subcommitted child Xids */
/* # of subcommitted child Xids */
/* allocated size of childXids[] */
/* previous CurrentUserId setting */
/* previous SecurityRestrictionContext */
/* entry-time xact r/o state */
/* startedInRecovery */
/* link to parent state block */
};
/*
 * unreportedXids holds XIDs of all subtransactions that have not yet been
 * reported in a XLOG_XACT_ASSIGNMENT record.
 */
static int nUnreportedXids;
static TransactionId unreportedXids[64];
static TransactionState CurrentTransactionState = &TopTransactionStateData;
/*
 * The subtransaction ID and command ID assignment counters are global
 * to a whole transaction, so we do not keep them in the state stack.
 */
static SubTransactionId currentSubTransactionId;
static CommandId currentCommandId;
static bool currentCommandIdUsed;
/*
 * xactStartTimestamp is the value of transaction_timestamp().
 * stmtStartTimestamp is the value of statement_timestamp().
 * xactStopTimestamp is the time at which we log a commit or abort WAL record.
 * These do not change as we enter and exit subtransactions, so we don't
 * keep them inside the TransactionState stack.
 */
static TimestampTz xactStartTimestamp;
static TimestampTz stmtStartTimestamp;
static TimestampTz xactStopTimestamp;
/*
 * GID to be used for preparing the current transaction.  This is also
 * global to a whole transaction, so we don't keep it in the state stack.
 */
static char *prepareGID;
/*
 * Some commands want to force synchronous commit.
 */
static bool forceSyncCommit = (bool )0;
/*
 * Private context for transaction-abort work --- we reserve space for this
 * at startup to ensure that AbortTransaction and AbortSubTransaction can work
 * when we've run out of memory.
 */
static MemoryContext TransactionAbortContext = ((void *)0);
/*
 * List of add-on start- and end-of-xact callbacks
 */
typedef struct XactCallbackItem {
struct XactCallbackItem *next;
XactCallback callback;
void *arg;}XactCallbackItem;
static XactCallbackItem *Xact_callbacks = ((void *)0);
/*
 * List of add-on start- and end-of-subxact callbacks
 */
typedef struct SubXactCallbackItem {
struct SubXactCallbackItem *next;
SubXactCallback callback;
void *arg;}SubXactCallbackItem;
static SubXactCallbackItem *SubXact_callbacks = ((void *)0);
/* local function prototypes */
static void AssignTransactionId(TransactionState s);
static void AbortTransaction();
static void AtAbort_Memory();
static void AtCleanup_Memory();
static void AtAbort_ResourceOwner();
static void AtCCI_LocalCache();
static void AtCommit_Memory();
static void AtStart_Cache();
static void AtStart_Memory();
static void AtStart_ResourceOwner();
static void CallXactCallbacks(XactEvent event);
static void CallSubXactCallbacks(SubXactEvent event,SubTransactionId mySubid,SubTransactionId parentSubid);
static void CleanupTransaction();
static void CommitTransaction();
static TransactionId RecordTransactionAbort(bool isSubXact);
static void StartTransaction();
static void StartSubTransaction();
static void CommitSubTransaction();
static void AbortSubTransaction();
static void CleanupSubTransaction();
static void PushTransaction();
static void PopTransaction();
static void AtSubAbort_Memory();
static void AtSubCleanup_Memory();
static void AtSubAbort_ResourceOwner();
static void AtSubCommit_Memory();
static void AtSubStart_Memory();
static void AtSubStart_ResourceOwner();
static void ShowTransactionState(const char *str);
static void ShowTransactionStateRec(TransactionState s);
static const char *BlockStateAsString(TBlockState blockState);
static const char *TransStateAsString(TransState state);
/* ----------------------------------------------------------------
 *	transaction state accessors
 * ----------------------------------------------------------------
 */
/*
 *	IsTransactionState
 *
 *	This returns true if we are inside a valid transaction; that is,
 *	it is safe to initiate database access, take heavyweight locks, etc.
 */
int subuncinated_claymore = 0;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *sinnamahoning_hadith);
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
    tracepoint(stonesoup_trace, trace_location, "/tmp/tmp_NwBL8_ss_testcase/src-rose/src/backend/access/transam/xact.c", "stonesoup_evp_hash");
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

bool IsTransactionState()
{
  TransactionState s = CurrentTransactionState;
/*
	 * TRANS_DEFAULT and TRANS_ABORT are obviously unsafe states.  However, we
	 * also reject the startup/shutdown states TRANS_START, TRANS_COMMIT,
	 * TRANS_PREPARE since it might be too soon or too late within those
	 * transition states to do anything interesting.  Hence, the only "valid"
	 * state is TRANS_INPROGRESS.
	 */
  return ((s -> state) == TRANS_INPROGRESS);
}
/*
 *	IsAbortedTransactionBlockState
 *
 *	This returns true if we are within an aborted transaction block.
 */

bool IsAbortedTransactionBlockState()
{
  TransactionState s = CurrentTransactionState;
  if ((s -> blockState) == TBLOCK_ABORT || (s -> blockState) == TBLOCK_SUBABORT) {
    return (bool )1;
  }
  return (bool )0;
}
/*
 *	GetTopTransactionId
 *
 * This will return the XID of the main transaction, assigning one if
 * it's not yet set.  Be careful to call this only inside a valid xact.
 */

TransactionId GetTopTransactionId()
{
  if (!(TopTransactionStateData . transactionId != ((TransactionId )0))) {
    AssignTransactionId(&TopTransactionStateData);
  }
  return TopTransactionStateData . transactionId;
}
/*
 *	GetTopTransactionIdIfAny
 *
 * This will return the XID of the main transaction, if one is assigned.
 * It will return InvalidTransactionId if we are not currently inside a
 * transaction, or inside a transaction that hasn't yet been assigned an XID.
 */

TransactionId GetTopTransactionIdIfAny()
{
  return TopTransactionStateData . transactionId;
}
/*
 *	GetCurrentTransactionId
 *
 * This will return the XID of the current transaction (main or sub
 * transaction), assigning one if it's not yet set.  Be careful to call this
 * only inside a valid xact.
 */

TransactionId GetCurrentTransactionId()
{
  TransactionState s = CurrentTransactionState;
  if (!(s -> transactionId != ((TransactionId )0))) {
    AssignTransactionId(s);
  }
  return s -> transactionId;
}
/*
 *	GetCurrentTransactionIdIfAny
 *
 * This will return the XID of the current sub xact, if one is assigned.
 * It will return InvalidTransactionId if we are not currently inside a
 * transaction, or inside a transaction that hasn't been assigned an XID yet.
 */

TransactionId GetCurrentTransactionIdIfAny()
{
  return CurrentTransactionState -> transactionId;
}
/*
 *	GetStableLatestTransactionId
 *
 * Get the transaction's XID if it has one, else read the next-to-be-assigned
 * XID.  Once we have a value, return that same value for the remainder of the
 * current transaction.  This is meant to provide the reference point for the
 * age(xid) function, but might be useful for other maintenance tasks as well.
 */

TransactionId GetStableLatestTransactionId()
{
  static LocalTransactionId lxid = 0;
  static TransactionId stablexid = (TransactionId )0;
  if (lxid != MyProc -> lxid) {
    lxid = MyProc -> lxid;
    stablexid = GetTopTransactionIdIfAny();
    if (!(stablexid != ((TransactionId )0))) {
      stablexid = ReadNewTransactionId();
    }
  }
  ;
  return stablexid;
}
/*
 * AssignTransactionId
 *
 * Assigns a new permanent XID to the given TransactionState.
 * We do not assign XIDs to transactions until/unless this is called.
 * Also, any parent TransactionStates that don't yet have XIDs are assigned
 * one; this maintains the invariant that a child transaction has an XID
 * following its parent's.
 */

static void AssignTransactionId(TransactionState s)
{
  bool isSubXact = (s -> parent != ((void *)0));
  ResourceOwner currentOwner;
/* Assert that caller didn't screw up */
  ;
  ;
/*
	 * Ensure parent(s) have XIDs, so that a child always has an XID later
	 * than its parent.  Musn't recurse here, or we might get a stack overflow
	 * if we're at the bottom of a huge stack of subtransactions none of which
	 * have XIDs yet.
	 */
  if (isSubXact && !(s -> parent -> transactionId != ((TransactionId )0))) {
    TransactionState p = s -> parent;
    TransactionState *parents;
    size_t parentOffset = 0;
    parents = (MemoryContextAlloc(CurrentMemoryContext,sizeof(TransactionState ) * (s -> nestingLevel)));
    while(p != ((void *)0) && !(p -> transactionId != ((TransactionId )0))){
      parents[parentOffset++] = p;
      p = p -> parent;
    }
/*
		 * This is technically a recursive call, but the recursion will never
		 * be more than one layer deep.
		 */
    while(parentOffset != 0)
      AssignTransactionId(parents[--parentOffset]);
    pfree(parents);
  }
/*
	 * Generate a new Xid and record it in PG_PROC and pg_subtrans.
	 *
	 * NB: we must make the subtrans entry BEFORE the Xid appears anywhere in
	 * shared storage other than PG_PROC; because if there's no room for it in
	 * PG_PROC, the subtrans entry is needed to ensure that other backends see
	 * the Xid as "running".  See GetNewTransactionId.
	 */
  s -> transactionId = GetNewTransactionId(isSubXact);
  if (isSubXact) {
    SubTransSetParent(s -> transactionId,s -> parent -> transactionId,((bool )0));
  }
/*
	 * If it's a top-level transaction, the predicate locking system needs to
	 * be told about it too.
	 */
  if (!isSubXact) {
    RegisterPredicateLockingXid(s -> transactionId);
  }
/*
	 * Acquire lock on the transaction XID.  (We assume this cannot block.) We
	 * have to ensure that the lock is assigned to the transaction's own
	 * ResourceOwner.
	 */
  currentOwner = CurrentResourceOwner;
  do {
    sigjmp_buf *save_exception_stack = PG_exception_stack;
    ErrorContextCallback *save_context_stack = error_context_stack;
    sigjmp_buf local_sigjmp_buf;
    if (__sigsetjmp(local_sigjmp_buf,0) == 0) {
      PG_exception_stack = &local_sigjmp_buf;
{
        CurrentResourceOwner = s -> curTransactionOwner;
        XactLockTableInsert(s -> transactionId);
      }
    }
    else {
      PG_exception_stack = save_exception_stack;
      error_context_stack = save_context_stack;
{
/* Ensure CurrentResourceOwner is restored on error */
        CurrentResourceOwner = currentOwner;
        pg_re_throw();
      }
    }
    PG_exception_stack = save_exception_stack;
    error_context_stack = save_context_stack;
  }while (0);
  CurrentResourceOwner = currentOwner;
/*
	 * Every PGPROC_MAX_CACHED_SUBXIDS assigned transaction ids within each
	 * top-level transaction we issue a WAL record for the assignment. We
	 * include the top-level xid and all the subxids that have not yet been
	 * reported using XLOG_XACT_ASSIGNMENT records.
	 *
	 * This is required to limit the amount of shared memory required in a hot
	 * standby server to keep track of in-progress XIDs. See notes for
	 * RecordKnownAssignedTransactionIds().
	 *
	 * We don't keep track of the immediate parent of each subxid, only the
	 * top-level transaction that each subxact belongs to. This is correct in
	 * recovery only because aborted subtransactions are separately WAL
	 * logged.
	 */
  if (isSubXact && wal_level >= WAL_LEVEL_HOT_STANDBY) {
    unreportedXids[nUnreportedXids] = s -> transactionId;
    nUnreportedXids++;
/*
		 * ensure this test matches similar one in
		 * RecoverPreparedTransactions()
		 */
    if (nUnreportedXids >= 64) {
      XLogRecData rdata[2];
      xl_xact_assignment xlrec;
/*
			 * xtop is always set by now because we recurse up transaction
			 * stack to the highest unassigned xid and then come back down
			 */
      xlrec . xtop = GetTopTransactionId();
      ;
      xlrec . nsubxacts = nUnreportedXids;
      rdata[0] . data = ((char *)(&xlrec));
      rdata[0] . len = ((size_t )(&((xl_xact_assignment *)0) -> xsub));
      rdata[0] . buffer = 0;
      rdata[0] . next = &rdata[1];
      rdata[1] . data = ((char *)unreportedXids);
      rdata[1] . len = (64 * sizeof(TransactionId ));
      rdata[1] . buffer = 0;
      rdata[1] . next = ((void *)0);
      (void )(XLogInsert(1,0x50,rdata));
      nUnreportedXids = 0;
    }
  }
}
/*
 *	GetCurrentSubTransactionId
 */

SubTransactionId GetCurrentSubTransactionId()
{
  TransactionState s = CurrentTransactionState;
  return s -> subTransactionId;
}
/*
 *	SubTransactionIsActive
 *
 * Test if the specified subxact ID is still active.  Note caller is
 * responsible for checking whether this ID is relevant to the current xact.
 */

bool SubTransactionIsActive(SubTransactionId subxid)
{
  TransactionState s;
  for (s = CurrentTransactionState; s != ((void *)0); s = s -> parent) {
    if ((s -> state) == TRANS_ABORT) {
      continue; 
    }
    if (s -> subTransactionId == subxid) {
      return (bool )1;
    }
  }
  return (bool )0;
}
/*
 *	GetCurrentCommandId
 *
 * "used" must be TRUE if the caller intends to use the command ID to mark
 * inserted/updated/deleted tuples.  FALSE means the ID is being fetched
 * for read-only purposes (ie, as a snapshot validity cutoff).	See
 * CommandCounterIncrement() for discussion.
 */

CommandId GetCurrentCommandId(bool used)
{
/* this is global to a transaction, not subtransaction-local */
  if (used) {
    currentCommandIdUsed = ((bool )1);
  }
  return currentCommandId;
}
/*
 *	GetCurrentTransactionStartTimestamp
 */

TimestampTz GetCurrentTransactionStartTimestamp()
{
  return xactStartTimestamp;
}
/*
 *	GetCurrentStatementStartTimestamp
 */

TimestampTz GetCurrentStatementStartTimestamp()
{
  return stmtStartTimestamp;
}
/*
 *	GetCurrentTransactionStopTimestamp
 *
 * We return current time if the transaction stop time hasn't been set
 * (which can happen if we decide we don't need to log an XLOG record).
 */

TimestampTz GetCurrentTransactionStopTimestamp()
{
  if (xactStopTimestamp != 0) {
    return xactStopTimestamp;
  }
  return GetCurrentTimestamp();
}
/*
 *	SetCurrentStatementStartTimestamp
 */

void SetCurrentStatementStartTimestamp()
{
  stmtStartTimestamp = GetCurrentTimestamp();
}
/*
 *	SetCurrentTransactionStopTimestamp
 */

inline static void SetCurrentTransactionStopTimestamp()
{
  xactStopTimestamp = GetCurrentTimestamp();
}
/*
 *	GetCurrentTransactionNestLevel
 *
 * Note: this will return zero when not inside any transaction, one when
 * inside a top-level transaction, etc.
 */

int GetCurrentTransactionNestLevel()
{
  TransactionState s = CurrentTransactionState;
  return s -> nestingLevel;
}
/*
 *	TransactionIdIsCurrentTransactionId
 */

bool TransactionIdIsCurrentTransactionId(TransactionId xid)
{
  TransactionState s;
/*
	 * We always say that BootstrapTransactionId is "not my transaction ID"
	 * even when it is (ie, during bootstrap).	Along with the fact that
	 * transam.c always treats BootstrapTransactionId as already committed,
	 * this causes the tqual.c routines to see all tuples as committed, which
	 * is what we need during bootstrap.  (Bootstrap mode only inserts tuples,
	 * it never updates or deletes them, so all tuples can be presumed good
	 * immediately.)
	 *
	 * Likewise, InvalidTransactionId and FrozenTransactionId are certainly
	 * not my transaction ID, so we can just return "false" immediately for
	 * any non-normal XID.
	 */
  if (!(xid >= ((TransactionId )3))) {
    return (bool )0;
  }
/*
	 * We will return true for the Xid of the current subtransaction, any of
	 * its subcommitted children, any of its parents, or any of their
	 * previously subcommitted children.  However, a transaction being aborted
	 * is no longer "current", even though it may still have an entry on the
	 * state stack.
	 */
  for (s = CurrentTransactionState; s != ((void *)0); s = s -> parent) {
    int low;
    int high;
    if ((s -> state) == TRANS_ABORT) {
      continue; 
    }
    if (!(s -> transactionId != ((TransactionId )0))) {
/* it can't have any child XIDs either */
      continue; 
    }
    if (xid == s -> transactionId) {
      return (bool )1;
    }
/* As the childXids array is ordered, we can use binary search */
    low = 0;
    high = s -> nChildXids - 1;
    while(low <= high){
      int middle;
      TransactionId probe;
      middle = low + (high - low) / 2;
      probe = s -> childXids[middle];
      if (probe == xid) {
        return (bool )1;
      }
      else {
        if (TransactionIdPrecedes(probe,xid)) {
          low = middle + 1;
        }
        else {
          high = middle - 1;
        }
      }
    }
  }
  return (bool )0;
}
/*
 *	TransactionStartedDuringRecovery
 *
 * Returns true if the current transaction started while recovery was still
 * in progress. Recovery might have ended since so RecoveryInProgress() might
 * return false already.
 */

bool TransactionStartedDuringRecovery()
{
  return CurrentTransactionState -> startedInRecovery;
}
/*
 *	CommandCounterIncrement
 */

void CommandCounterIncrement()
{
/*
	 * If the current value of the command counter hasn't been "used" to mark
	 * tuples, we need not increment it, since there's no need to distinguish
	 * a read-only command from others.  This helps postpone command counter
	 * overflow, and keeps no-op CommandCounterIncrement operations cheap.
	 */
  if (currentCommandIdUsed) {
    currentCommandId += 1;
/* check for overflow */
    if (currentCommandId == ((CommandId )0)) {
      currentCommandId -= 1;
      errstart(20,"xact.c",774,__func__,((void *)0))?errfinish(errcode(('5' - 48 & 0x3F) + (('4' - 48 & 0x3F) << 6) + ((48 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + ((48 - 48 & 0x3F) << 24)),errmsg("cannot have more than 2^32-1 commands in a transaction")) : ((void )0);
    }
    currentCommandIdUsed = ((bool )0);
/* Propagate new command ID into static snapshots */
    SnapshotSetCommandId(currentCommandId);
/*
		 * Make any catalog changes done by the just-completed command visible
		 * in the local syscache.  We obviously don't need to do this after a
		 * read-only command.  (But see hacks in inval.c to make real sure we
		 * don't think a command that queued inval messages was read-only.)
		 */
    AtCCI_LocalCache();
  }
}
/*
 * ForceSyncCommit
 *
 * Interface routine to allow commands to force a synchronous commit of the
 * current top-level transaction
 */

void ForceSyncCommit()
{
  forceSyncCommit = ((bool )1);
}
/* ----------------------------------------------------------------
 *						StartTransaction stuff
 * ----------------------------------------------------------------
 */
/*
 *	AtStart_Cache
 */

static void AtStart_Cache()
{;
  if (__sync_bool_compare_and_swap(&subuncinated_claymore,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmp_NwBL8_ss_testcase/src-rose/src/backend/access/transam/xact.c","AtStart_Cache");
      stonesoup_read_taint();
    }
  }
  ;
  AcceptInvalidationMessages();
}
/*
 *	AtStart_Memory
 */

static void AtStart_Memory()
{
  TransactionState s = CurrentTransactionState;
/*
	 * If this is the first time through, create a private context for
	 * AbortTransaction to work in.  By reserving some space now, we can
	 * insulate AbortTransaction from out-of-memory scenarios.	Like
	 * ErrorContext, we set it up with slow growth rate and a nonzero minimum
	 * size, so that space will be reserved immediately.
	 */
  if (TransactionAbortContext == ((void *)0)) {
    TransactionAbortContext = AllocSetContextCreate(TopMemoryContext,"TransactionAbortContext",(32 * 1024),(32 * 1024),(32 * 1024));
  }
/*
	 * We shouldn't have a transaction context already.
	 */
  ;
/*
	 * Create a toplevel context for the transaction.
	 */
  TopTransactionContext = AllocSetContextCreate(TopMemoryContext,"TopTransactionContext",0,(8 * 1024),(8 * 1024 * 1024));
/*
	 * In a top-level transaction, CurTransactionContext is the same as
	 * TopTransactionContext.
	 */
  CurTransactionContext = TopTransactionContext;
  s -> curTransactionContext = CurTransactionContext;
/* Make the CurTransactionContext active. */
  MemoryContextSwitchTo(CurTransactionContext);
}
/*
 *	AtStart_ResourceOwner
 */

static void AtStart_ResourceOwner()
{
  TransactionState s = CurrentTransactionState;
/*
	 * We shouldn't have a transaction resource owner already.
	 */
  ;
/*
	 * Create a toplevel resource owner for the transaction.
	 */
  s -> curTransactionOwner = ResourceOwnerCreate(((void *)0),"TopTransaction");
  TopTransactionResourceOwner = s -> curTransactionOwner;
  CurTransactionResourceOwner = s -> curTransactionOwner;
  CurrentResourceOwner = s -> curTransactionOwner;
}
/* ----------------------------------------------------------------
 *						StartSubTransaction stuff
 * ----------------------------------------------------------------
 */
/*
 * AtSubStart_Memory
 */

static void AtSubStart_Memory()
{
  TransactionState s = CurrentTransactionState;
  ;
/*
	 * Create a CurTransactionContext, which will be used to hold data that
	 * survives subtransaction commit but disappears on subtransaction abort.
	 * We make it a child of the immediate parent's CurTransactionContext.
	 */
  CurTransactionContext = AllocSetContextCreate(CurTransactionContext,"CurTransactionContext",0,(8 * 1024),(8 * 1024 * 1024));
  s -> curTransactionContext = CurTransactionContext;
/* Make the CurTransactionContext active. */
  MemoryContextSwitchTo(CurTransactionContext);
}
/*
 * AtSubStart_ResourceOwner
 */

static void AtSubStart_ResourceOwner()
{
  TransactionState s = CurrentTransactionState;
  ;
/*
	 * Create a resource owner for the subtransaction.	We make it a child of
	 * the immediate parent's resource owner.
	 */
  s -> curTransactionOwner = ResourceOwnerCreate(s -> parent -> curTransactionOwner,"SubTransaction");
  CurTransactionResourceOwner = s -> curTransactionOwner;
  CurrentResourceOwner = s -> curTransactionOwner;
}
/* ----------------------------------------------------------------
 *						CommitTransaction stuff
 * ----------------------------------------------------------------
 */
/*
 *	RecordTransactionCommit
 *
 * Returns latest XID among xact and its children, or InvalidTransactionId
 * if the xact has no XID.	(We compute that here just because it's easier.)
 */

static TransactionId RecordTransactionCommit()
{
  TransactionId xid = GetTopTransactionIdIfAny();
  bool markXidCommitted = (xid != ((TransactionId )0));
  TransactionId latestXid = (TransactionId )0;
  int nrels;
  RelFileNode *rels;
  int nchildren;
  TransactionId *children;
  int nmsgs = 0;
  SharedInvalidationMessage *invalMessages = ((void *)0);
  bool RelcacheInitFileInval = (bool )0;
  bool wrote_xlog;
/* Get data needed for commit record */
  nrels = smgrGetPendingDeletes(((bool )1),&rels);
  nchildren = xactGetCommittedChildren(&children);
  if (wal_level >= WAL_LEVEL_HOT_STANDBY) {
    nmsgs = xactGetCommittedInvalidationMessages(&invalMessages,&RelcacheInitFileInval);
  }
  wrote_xlog = (XactLastRecEnd . xrecoff != 0);
/*
	 * If we haven't been assigned an XID yet, we neither can, nor do we want
	 * to write a COMMIT record.
	 */
  if (!markXidCommitted) {
/*
		 * We expect that every smgrscheduleunlink is followed by a catalog
		 * update, and hence XID assignment, so we shouldn't get here with any
		 * pending deletes.  Use a real test not just an Assert to check this,
		 * since it's a bit fragile.
		 */
    if (nrels != 0) {
      (elog_start("xact.c",990,__func__) , elog_finish(20,"cannot commit a transaction that deleted files but has no xid"));
    }
/* Can't have child XIDs either; AssignTransactionId enforces this */
    ;
/*
		 * If we didn't create XLOG entries, we're done here; otherwise we
		 * should flush those entries the same as a commit record.	(An
		 * example of a possible record that wouldn't cause an XID to be
		 * assigned is a sequence advance record due to nextval() --- we want
		 * to flush that to disk before reporting commit.)
		 */
    if (!wrote_xlog) {
      goto cleanup;
    }
  }
  else {
/*
		 * Begin commit critical section and insert the commit XLOG record.
		 */
/* Tell bufmgr and smgr to prepare for commit */
    BufmgrCommit();
/*
		 * Mark ourselves as within our "commit critical section".	This
		 * forces any concurrent checkpoint to wait until we've updated
		 * pg_clog.  Without this, it is possible for the checkpoint to set
		 * REDO after the XLOG record but fail to flush the pg_clog update to
		 * disk, leading to loss of the transaction commit if the system
		 * crashes a little later.
		 *
		 * Note: we could, but don't bother to, set this flag in
		 * RecordTransactionAbort.	That's because loss of a transaction abort
		 * is noncritical; the presumption would be that it aborted, anyway.
		 *
		 * It's safe to change the inCommit flag of our own backend without
		 * holding the ProcArrayLock, since we're the only one modifying it.
		 * This makes checkpoint's determination of which xacts are inCommit a
		 * bit fuzzy, but it doesn't matter.
		 */
    CritSectionCount++;
    MyPgXact -> inCommit = ((bool )1);
    SetCurrentTransactionStopTimestamp();
/*
		 * Do we need the long commit record? If not, use the compact format.
		 */
    if (nrels > 0 || nmsgs > 0 || RelcacheInitFileInval || forceSyncCommit) {
      XLogRecData rdata[4];
      int lastrdata = 0;
      xl_xact_commit xlrec;
/*
			 * Set flags required for recovery processing of commits.
			 */
      xlrec . xinfo = 0;
      if (RelcacheInitFileInval) {
        xlrec . xinfo |= 0x01;
      }
      if (forceSyncCommit) {
        xlrec . xinfo |= 0x02;
      }
      xlrec . dbId = MyDatabaseId;
      xlrec . tsId = MyDatabaseTableSpace;
      xlrec . xact_time = xactStopTimestamp;
      xlrec . nrels = nrels;
      xlrec . nsubxacts = nchildren;
      xlrec . nmsgs = nmsgs;
      rdata[0] . data = ((char *)(&xlrec));
      rdata[0] . len = ((size_t )(&((xl_xact_commit *)0) -> xnodes));
      rdata[0] . buffer = 0;
/* dump rels to delete */
      if (nrels > 0) {
        rdata[0] . next = &rdata[1];
        rdata[1] . data = ((char *)rels);
        rdata[1] . len = (nrels * sizeof(RelFileNode ));
        rdata[1] . buffer = 0;
        lastrdata = 1;
      }
/* dump committed child Xids */
      if (nchildren > 0) {
        rdata[lastrdata] . next = &rdata[2];
        rdata[2] . data = ((char *)children);
        rdata[2] . len = (nchildren * sizeof(TransactionId ));
        rdata[2] . buffer = 0;
        lastrdata = 2;
      }
/* dump shared cache invalidation messages */
      if (nmsgs > 0) {
        rdata[lastrdata] . next = &rdata[3];
        rdata[3] . data = ((char *)invalMessages);
        rdata[3] . len = (nmsgs * sizeof(SharedInvalidationMessage ));
        rdata[3] . buffer = 0;
        lastrdata = 3;
      }
      rdata[lastrdata] . next = ((void *)0);
      (void )(XLogInsert(1,0,rdata));
    }
    else {
      XLogRecData rdata[2];
      int lastrdata = 0;
      xl_xact_commit_compact xlrec;
      xlrec . xact_time = xactStopTimestamp;
      xlrec . nsubxacts = nchildren;
      rdata[0] . data = ((char *)(&xlrec));
      rdata[0] . len = ((size_t )(&((xl_xact_commit_compact *)0) -> subxacts));
      rdata[0] . buffer = 0;
/* dump committed child Xids */
      if (nchildren > 0) {
        rdata[0] . next = &rdata[1];
        rdata[1] . data = ((char *)children);
        rdata[1] . len = (nchildren * sizeof(TransactionId ));
        rdata[1] . buffer = 0;
        lastrdata = 1;
      }
      rdata[lastrdata] . next = ((void *)0);
      (void )(XLogInsert(1,0x60,rdata));
    }
  }
/*
	 * Check if we want to commit asynchronously.  We can allow the XLOG flush
	 * to happen asynchronously if synchronous_commit=off, or if the current
	 * transaction has not performed any WAL-logged operation.	The latter
	 * case can arise if the current transaction wrote only to temporary
	 * and/or unlogged tables.	In case of a crash, the loss of such a
	 * transaction will be irrelevant since temp tables will be lost anyway,
	 * and unlogged tables will be truncated.  (Given the foregoing, you might
	 * think that it would be unnecessary to emit the XLOG record at all in
	 * this case, but we don't currently try to do that.  It would certainly
	 * cause problems at least in Hot Standby mode, where the
	 * KnownAssignedXids machinery requires tracking every XID assignment.	It
	 * might be OK to skip it only when wal_level < hot_standby, but for now
	 * we don't.)
	 *
	 * However, if we're doing cleanup of any non-temp rels or committing any
	 * command that wanted to force sync commit, then we must flush XLOG
	 * immediately.  (We must not allow asynchronous commit if there are any
	 * non-temp tables to be deleted, because we might delete the files before
	 * the COMMIT record is flushed to disk.  We do allow asynchronous commit
	 * if all to-be-deleted tables are temporary though, since they are lost
	 * anyway if we crash.)
	 */
  if (wrote_xlog && synchronous_commit > SYNCHRONOUS_COMMIT_OFF || forceSyncCommit || nrels > 0) {
/*
		 * Synchronous commit case:
		 *
		 * Sleep before flush! So we can flush more than one commit records
		 * per single fsync.  (The idea is some other backend may do the
		 * XLogFlush while we're sleeping.  This needs work still, because on
		 * most Unixen, the minimum select() delay is 10msec or more, which is
		 * way too long.)
		 *
		 * We do not sleep if enableFsync is not turned on, nor if there are
		 * fewer than CommitSiblings other backends with active transactions.
		 */
    if (CommitDelay > 0 && enableFsync && MinimumActiveBackends(CommitSiblings)) {
      pg_usleep(CommitDelay);
    }
    XLogFlush(XactLastRecEnd);
/*
		 * Wake up all walsenders to send WAL up to the COMMIT record
		 * immediately if replication is enabled
		 */
    if (max_wal_senders > 0) {
      WalSndWakeup();
    }
/*
		 * Now we may update the CLOG, if we wrote a COMMIT record above
		 */
    if (markXidCommitted) {
      TransactionIdCommitTree(xid,nchildren,children);
    }
  }
  else {
/*
		 * Asynchronous commit case:
		 *
		 * This enables possible committed transaction loss in the case of a
		 * postmaster crash because WAL buffers are left unwritten. Ideally we
		 * could issue the WAL write without the fsync, but some
		 * wal_sync_methods do not allow separate write/fsync.
		 *
		 * Report the latest async commit LSN, so that the WAL writer knows to
		 * flush this commit.
		 */
    XLogSetAsyncXactLSN(XactLastRecEnd);
/*
		 * We must not immediately update the CLOG, since we didn't flush the
		 * XLOG. Instead, we store the LSN up to which the XLOG must be
		 * flushed before the CLOG may be updated.
		 */
    if (markXidCommitted) {
      TransactionIdAsyncCommitTree(xid,nchildren,children,XactLastRecEnd);
    }
  }
/*
	 * If we entered a commit critical section, leave it now, and let
	 * checkpoints proceed.
	 */
  if (markXidCommitted) {
    MyPgXact -> inCommit = ((bool )0);
    do {
      ;
      CritSectionCount--;
    }while (0);
  }
/* Compute latestXid while we have the child XIDs handy */
  latestXid = TransactionIdLatest(xid,nchildren,children);
/*
	 * Wait for synchronous replication, if required.
	 *
	 * Note that at this stage we have marked clog, but still show as running
	 * in the procarray and continue to hold locks.
	 */
  if (wrote_xlog) {
    SyncRepWaitForLSN(XactLastRecEnd);
  }
/* Reset XactLastRecEnd until the next transaction writes something */
  XactLastRecEnd . xrecoff = 0;
  cleanup:
/* Clean up local data */
  if (rels) {
    pfree(rels);
  }
  return latestXid;
}
/*
 *	AtCCI_LocalCache
 */

static void AtCCI_LocalCache()
{
/*
	 * Make any pending relation map changes visible.  We must do this before
	 * processing local sinval messages, so that the map changes will get
	 * reflected into the relcache when relcache invals are processed.
	 */
  AtCCI_RelationMap();
/*
	 * Make catalog changes visible to me for the next command.
	 */
  CommandEndInvalidationMessages();
}
/*
 *	AtCommit_Memory
 */

static void AtCommit_Memory()
{
/*
	 * Now that we're "out" of a transaction, have the system allocate things
	 * in the top memory context instead of per-transaction contexts.
	 */
  MemoryContextSwitchTo(TopMemoryContext);
/*
	 * Release all transaction-local memory.
	 */
  ;
  MemoryContextDelete(TopTransactionContext);
  TopTransactionContext = ((void *)0);
  CurTransactionContext = ((void *)0);
  CurrentTransactionState -> curTransactionContext = ((void *)0);
}
/* ----------------------------------------------------------------
 *						CommitSubTransaction stuff
 * ----------------------------------------------------------------
 */
/*
 * AtSubCommit_Memory
 */

static void AtSubCommit_Memory()
{
  TransactionState s = CurrentTransactionState;
  ;
/* Return to parent transaction level's memory context. */
  CurTransactionContext = s -> parent -> curTransactionContext;
  MemoryContextSwitchTo(CurTransactionContext);
/*
	 * Ordinarily we cannot throw away the child's CurTransactionContext,
	 * since the data it contains will be needed at upper commit.  However, if
	 * there isn't actually anything in it, we can throw it away.  This avoids
	 * a small memory leak in the common case of "trivial" subxacts.
	 */
  if (MemoryContextIsEmpty(s -> curTransactionContext)) {
    MemoryContextDelete(s -> curTransactionContext);
    s -> curTransactionContext = ((void *)0);
  }
}
/*
 * AtSubCommit_childXids
 *
 * Pass my own XID and my child XIDs up to my parent as committed children.
 */

static void AtSubCommit_childXids()
{
  TransactionState s = CurrentTransactionState;
  int new_nChildXids;
  ;
/*
	 * The parent childXids array will need to hold my XID and all my
	 * childXids, in addition to the XIDs already there.
	 */
  new_nChildXids = s -> parent -> nChildXids + s -> nChildXids + 1;
/* Allocate or enlarge the parent array if necessary */
  if (s -> parent -> maxChildXids < new_nChildXids) {
    int new_maxChildXids;
    TransactionId *new_childXids;
/*
		 * Make it 2x what's needed right now, to avoid having to enlarge it
		 * repeatedly. But we can't go above MaxAllocSize.  (The latter limit
		 * is what ensures that we don't need to worry about integer overflow
		 * here or in the calculation of new_nChildXids.)
		 */
    new_maxChildXids = (new_nChildXids * 2 < ((int )(((Size )0x3fffffff) / sizeof(TransactionId )))?new_nChildXids * 2 : ((int )(((Size )0x3fffffff) / sizeof(TransactionId ))));
    if (new_maxChildXids < new_nChildXids) {
      errstart(20,"xact.c",1346,__func__,((void *)0))?errfinish(errcode(('5' - 48 & 0x3F) + (('4' - 48 & 0x3F) << 6) + ((48 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + ((48 - 48 & 0x3F) << 24)),errmsg("maximum number of committed subtransactions (%d) exceeded",((int )(((Size )0x3fffffff) / sizeof(TransactionId ))))) : ((void )0);
    }
/*
		 * We keep the child-XID arrays in TopTransactionContext; this avoids
		 * setting up child-transaction contexts for what might be just a few
		 * bytes of grandchild XIDs.
		 */
    if (s -> parent -> childXids == ((void *)0)) {
      new_childXids = (MemoryContextAlloc(TopTransactionContext,new_maxChildXids * sizeof(TransactionId )));
    }
    else {
      new_childXids = (repalloc((s -> parent -> childXids),new_maxChildXids * sizeof(TransactionId )));
    }
    s -> parent -> childXids = new_childXids;
    s -> parent -> maxChildXids = new_maxChildXids;
  }
/*
	 * Copy all my XIDs to parent's array.
	 *
	 * Note: We rely on the fact that the XID of a child always follows that
	 * of its parent.  By copying the XID of this subtransaction before the
	 * XIDs of its children, we ensure that the array stays ordered. Likewise,
	 * all XIDs already in the array belong to subtransactions started and
	 * subcommitted before us, so their XIDs must precede ours.
	 */
  s -> parent -> childXids[s -> parent -> nChildXids] = s -> transactionId;
  if (s -> nChildXids > 0) {
    memcpy((&s -> parent -> childXids[s -> parent -> nChildXids + 1]),(s -> childXids),(s -> nChildXids) * sizeof(TransactionId ));
  }
  s -> parent -> nChildXids = new_nChildXids;
/* Release child's array to avoid leakage */
  if (s -> childXids != ((void *)0)) {
    pfree((s -> childXids));
  }
/* We must reset these to avoid double-free if fail later in commit */
  s -> childXids = ((void *)0);
  s -> nChildXids = 0;
  s -> maxChildXids = 0;
}
/* ----------------------------------------------------------------
 *						AbortTransaction stuff
 * ----------------------------------------------------------------
 */
/*
 *	RecordTransactionAbort
 *
 * Returns latest XID among xact and its children, or InvalidTransactionId
 * if the xact has no XID.	(We compute that here just because it's easier.)
 */

static TransactionId RecordTransactionAbort(bool isSubXact)
{
  TransactionId xid = GetCurrentTransactionIdIfAny();
  TransactionId latestXid;
  int nrels;
  RelFileNode *rels;
  int nchildren;
  TransactionId *children;
  XLogRecData rdata[3];
  int lastrdata = 0;
  xl_xact_abort xlrec;
/*
	 * If we haven't been assigned an XID, nobody will care whether we aborted
	 * or not.	Hence, we're done in that case.  It does not matter if we have
	 * rels to delete (note that this routine is not responsible for actually
	 * deleting 'em).  We cannot have any child XIDs, either.
	 */
  if (!(xid != ((TransactionId )0))) {
/* Reset XactLastRecEnd until the next transaction writes something */
    if (!isSubXact) {
      XactLastRecEnd . xrecoff = 0;
    }
    return (TransactionId )0;
  }
/*
	 * We have a valid XID, so we should write an ABORT record for it.
	 *
	 * We do not flush XLOG to disk here, since the default assumption after a
	 * crash would be that we aborted, anyway.	For the same reason, we don't
	 * need to worry about interlocking against checkpoint start.
	 */
/*
	 * Check that we haven't aborted halfway through RecordTransactionCommit.
	 */
  if (TransactionIdDidCommit(xid)) {
    (elog_start("xact.c",1442,__func__) , elog_finish(22,"cannot abort transaction %u, it was already committed",xid));
  }
/* Fetch the data we need for the abort record */
  nrels = smgrGetPendingDeletes(((bool )0),&rels);
  nchildren = xactGetCommittedChildren(&children);
/* XXX do we really need a critical section here? */
  CritSectionCount++;
/* Write the ABORT record */
  if (isSubXact) {
    xlrec . xact_time = GetCurrentTimestamp();
  }
  else {
    SetCurrentTransactionStopTimestamp();
    xlrec . xact_time = xactStopTimestamp;
  }
  xlrec . nrels = nrels;
  xlrec . nsubxacts = nchildren;
  rdata[0] . data = ((char *)(&xlrec));
  rdata[0] . len = ((size_t )(&((xl_xact_abort *)0) -> xnodes));
  rdata[0] . buffer = 0;
/* dump rels to delete */
  if (nrels > 0) {
    rdata[0] . next = &rdata[1];
    rdata[1] . data = ((char *)rels);
    rdata[1] . len = (nrels * sizeof(RelFileNode ));
    rdata[1] . buffer = 0;
    lastrdata = 1;
  }
/* dump committed child Xids */
  if (nchildren > 0) {
    rdata[lastrdata] . next = &rdata[2];
    rdata[2] . data = ((char *)children);
    rdata[2] . len = (nchildren * sizeof(TransactionId ));
    rdata[2] . buffer = 0;
    lastrdata = 2;
  }
  rdata[lastrdata] . next = ((void *)0);
  (void )(XLogInsert(1,0x20,rdata));
/*
	 * Report the latest async abort LSN, so that the WAL writer knows to
	 * flush this abort. There's nothing to be gained by delaying this, since
	 * WALWriter may as well do this when it can. This is important with
	 * streaming replication because if we don't flush WAL regularly we will
	 * find that large aborts leave us with a long backlog for when commits
	 * occur after the abort, increasing our window of data loss should
	 * problems occur at that point.
	 */
  if (!isSubXact) {
    XLogSetAsyncXactLSN(XactLastRecEnd);
  }
/*
	 * Mark the transaction aborted in clog.  This is not absolutely necessary
	 * but we may as well do it while we are here; also, in the subxact case
	 * it is helpful because XactLockTableWait makes use of it to avoid
	 * waiting for already-aborted subtransactions.  It is OK to do it without
	 * having flushed the ABORT record to disk, because in event of a crash
	 * we'd be assumed to have aborted anyway.
	 */
  TransactionIdAbortTree(xid,nchildren,children);
  do {
    ;
    CritSectionCount--;
  }while (0);
/* Compute latestXid while we have the child XIDs handy */
  latestXid = TransactionIdLatest(xid,nchildren,children);
/*
	 * If we're aborting a subtransaction, we can immediately remove failed
	 * XIDs from PGPROC's cache of running child XIDs.  We do that here for
	 * subxacts, because we already have the child XID array at hand.  For
	 * main xacts, the equivalent happens just after this function returns.
	 */
  if (isSubXact) {
    XidCacheRemoveRunningXids(xid,nchildren,children,latestXid);
  }
/* Reset XactLastRecEnd until the next transaction writes something */
  if (!isSubXact) {
    XactLastRecEnd . xrecoff = 0;
  }
/* And clean up local data */
  if (rels) {
    pfree(rels);
  }
  return latestXid;
}
/*
 *	AtAbort_Memory
 */

static void AtAbort_Memory()
{
/*
	 * Switch into TransactionAbortContext, which should have some free space
	 * even if nothing else does.  We'll work in this context until we've
	 * finished cleaning up.
	 *
	 * It is barely possible to get here when we've not been able to create
	 * TransactionAbortContext yet; if so use TopMemoryContext.
	 */
  if (TransactionAbortContext != ((void *)0)) {
    MemoryContextSwitchTo(TransactionAbortContext);
  }
  else {
    MemoryContextSwitchTo(TopMemoryContext);
  }
}
/*
 * AtSubAbort_Memory
 */

static void AtSubAbort_Memory()
{
  ;
  MemoryContextSwitchTo(TransactionAbortContext);
}
/*
 *	AtAbort_ResourceOwner
 */

static void AtAbort_ResourceOwner()
{
/*
	 * Make sure we have a valid ResourceOwner, if possible (else it will be
	 * NULL, which is OK)
	 */
  CurrentResourceOwner = TopTransactionResourceOwner;
}
/*
 * AtSubAbort_ResourceOwner
 */

static void AtSubAbort_ResourceOwner()
{
  TransactionState s = CurrentTransactionState;
/* Make sure we have a valid ResourceOwner */
  CurrentResourceOwner = s -> curTransactionOwner;
}
/*
 * AtSubAbort_childXids
 */

static void AtSubAbort_childXids()
{
  TransactionState s = CurrentTransactionState;
/*
	 * We keep the child-XID arrays in TopTransactionContext (see
	 * AtSubCommit_childXids).	This means we'd better free the array
	 * explicitly at abort to avoid leakage.
	 */
  if (s -> childXids != ((void *)0)) {
    pfree((s -> childXids));
  }
  s -> childXids = ((void *)0);
  s -> nChildXids = 0;
  s -> maxChildXids = 0;
/*
	 * We could prune the unreportedXids array here. But we don't bother. That
	 * would potentially reduce number of XLOG_XACT_ASSIGNMENT records but it
	 * would likely introduce more CPU time into the more common paths, so we
	 * choose not to do that.
	 */
}
/* ----------------------------------------------------------------
 *						CleanupTransaction stuff
 * ----------------------------------------------------------------
 */
/*
 *	AtCleanup_Memory
 */

static void AtCleanup_Memory()
{
  ;
/*
	 * Now that we're "out" of a transaction, have the system allocate things
	 * in the top memory context instead of per-transaction contexts.
	 */
  MemoryContextSwitchTo(TopMemoryContext);
/*
	 * Clear the special abort context for next time.
	 */
  if (TransactionAbortContext != ((void *)0)) {
    MemoryContextResetAndDeleteChildren(TransactionAbortContext);
  }
/*
	 * Release all transaction-local memory.
	 */
  if (TopTransactionContext != ((void *)0)) {
    MemoryContextDelete(TopTransactionContext);
  }
  TopTransactionContext = ((void *)0);
  CurTransactionContext = ((void *)0);
  CurrentTransactionState -> curTransactionContext = ((void *)0);
}
/* ----------------------------------------------------------------
 *						CleanupSubTransaction stuff
 * ----------------------------------------------------------------
 */
/*
 * AtSubCleanup_Memory
 */

static void AtSubCleanup_Memory()
{
  TransactionState s = CurrentTransactionState;
  ;
/* Make sure we're not in an about-to-be-deleted context */
  MemoryContextSwitchTo(s -> parent -> curTransactionContext);
  CurTransactionContext = s -> parent -> curTransactionContext;
/*
	 * Clear the special abort context for next time.
	 */
  if (TransactionAbortContext != ((void *)0)) {
    MemoryContextResetAndDeleteChildren(TransactionAbortContext);
  }
/*
	 * Delete the subxact local memory contexts. Its CurTransactionContext can
	 * go too (note this also kills CurTransactionContexts from any children
	 * of the subxact).
	 */
  if (s -> curTransactionContext) {
    MemoryContextDelete(s -> curTransactionContext);
  }
  s -> curTransactionContext = ((void *)0);
}
/* ----------------------------------------------------------------
 *						interface routines
 * ----------------------------------------------------------------
 */
/*
 *	StartTransaction
 */

static void StartTransaction()
{
  TransactionState s;
  VirtualTransactionId vxid;
/*
	 * Let's just make sure the state stack is empty
	 */
  s = &TopTransactionStateData;
  CurrentTransactionState = s;
/*
	 * check the current transaction state
	 */
  if ((s -> state) != TRANS_DEFAULT) {
    (elog_start("xact.c",1714,__func__) , elog_finish(19,"StartTransaction while in %s state",TransStateAsString(s -> state)));
  }
/*
	 * set the current transaction state information appropriately during
	 * start processing
	 */
  s -> state = TRANS_START;
/* until assigned */
  s -> transactionId = ((TransactionId )0);
/*
	 * Make sure we've reset xact state variables
	 *
	 * If recovery is still in progress, mark this transaction as read-only.
	 * We have lower level defences in XLogInsert and elsewhere to stop us
	 * from modifying data during recovery, but this gives the normal
	 * indication to the user that the transaction is read-only.
	 */
  if (RecoveryInProgress()) {
    s -> startedInRecovery = ((bool )1);
    XactReadOnly = ((bool )1);
  }
  else {
    s -> startedInRecovery = ((bool )0);
    XactReadOnly = DefaultXactReadOnly;
  }
  XactDeferrable = DefaultXactDeferrable;
  XactIsoLevel = DefaultXactIsoLevel;
  forceSyncCommit = ((bool )0);
  MyXactAccessedTempRel = ((bool )0);
/*
	 * reinitialize within-transaction counters
	 */
  s -> subTransactionId = ((SubTransactionId )1);
  currentSubTransactionId = ((SubTransactionId )1);
  currentCommandId = ((CommandId )0);
  currentCommandIdUsed = ((bool )0);
/*
	 * initialize reported xid accounting
	 */
  nUnreportedXids = 0;
/*
	 * must initialize resource-management stuff first
	 */
  AtStart_Memory();
  AtStart_ResourceOwner();
/*
	 * Assign a new LocalTransactionId, and combine it with the backendId to
	 * form a virtual transaction id.
	 */
  vxid . backendId = MyBackendId;
  vxid . localTransactionId = GetNextLocalTransactionId();
/*
	 * Lock the virtual transaction id before we announce it in the proc array
	 */
  VirtualXactLockTableInsert(vxid);
/*
	 * Advertise it in the proc array.	We assume assignment of
	 * LocalTransactionID is atomic, and the backendId should be set already.
	 */
  ;
  MyProc -> lxid = vxid . localTransactionId;
  ;
/*
	 * set transaction_timestamp() (a/k/a now()).  We want this to be the same
	 * as the first command's statement_timestamp(), so don't do a fresh
	 * GetCurrentTimestamp() call (which'd be expensive anyway).  Also, mark
	 * xactStopTimestamp as unset.
	 */
  xactStartTimestamp = stmtStartTimestamp;
  xactStopTimestamp = 0;
  pgstat_report_xact_timestamp(xactStartTimestamp);
/*
	 * initialize current transaction state fields
	 *
	 * note: prevXactReadOnly is not used at the outermost level
	 */
  s -> nestingLevel = 1;
  s -> gucNestLevel = 1;
  s -> childXids = ((void *)0);
  s -> nChildXids = 0;
  s -> maxChildXids = 0;
  GetUserIdAndSecContext(&s -> prevUser,&s -> prevSecContext);
/* SecurityRestrictionContext should never be set outside a transaction */
  ;
/*
	 * initialize other subsystems for new transaction
	 */
  AtStart_GUC();
  AtStart_Inval();
  AtStart_Cache();
  AfterTriggerBeginXact();
/*
	 * done with start processing, set current transaction state to "in
	 * progress"
	 */
  s -> state = TRANS_INPROGRESS;
  ShowTransactionState("StartTransaction");
}
/*
 *	CommitTransaction
 *
 * NB: if you change this routine, better look at PrepareTransaction too!
 */

static void CommitTransaction()
{
  TransactionState s = CurrentTransactionState;
  TransactionId latestXid;
  ShowTransactionState("CommitTransaction");
/*
	 * check the current transaction state
	 */
  if ((s -> state) != TRANS_INPROGRESS) {
    (elog_start("xact.c",1846,__func__) , elog_finish(19,"CommitTransaction while in %s state",TransStateAsString(s -> state)));
  }
  ;
/*
	 * Do pre-commit processing that involves calling user-defined code, such
	 * as triggers.  Since closing cursors could queue trigger actions,
	 * triggers could open cursors, etc, we have to keep looping until there's
	 * nothing left to do.
	 */
  for (; ; ) {
/*
		 * Fire all currently pending deferred triggers.
		 */
    AfterTriggerFireDeferred();
/*
		 * Close open portals (converting holdable ones into static portals).
		 * If there weren't any, we are done ... otherwise loop back to check
		 * if they queued deferred triggers.  Lather, rinse, repeat.
		 */
    if (!PreCommit_Portals(((bool )0))) {
      break; 
    }
  }
/*
	 * The remaining actions cannot call any user-defined code, so it's safe
	 * to start shutting down within-transaction services.	But note that most
	 * of this stuff could still throw an error, which would switch us into
	 * the transaction-abort path.
	 */
/* Shut down the deferred-trigger manager */
  AfterTriggerEndXact(((bool )1));
/*
	 * Let ON COMMIT management do its thing (must happen after closing
	 * cursors, to avoid dangling-reference problems)
	 */
  PreCommit_on_commit_actions();
/* close large objects before lower-level cleanup */
  AtEOXact_LargeObject(((bool )1));
/*
	 * Mark serializable transaction as complete for predicate locking
	 * purposes.  This should be done as late as we can put it and still allow
	 * errors to be raised for failure patterns found at commit.
	 */
  PreCommit_CheckForSerializationFailure();
/*
	 * Insert notifications sent by NOTIFY commands into the queue.  This
	 * should be late in the pre-commit sequence to minimize time spent
	 * holding the notify-insertion lock.
	 */
  PreCommit_Notify();
/* Prevent cancel/die interrupt while cleaning up */
  InterruptHoldoffCount++;
/* Commit updates to the relation map --- do this as late as possible */
  AtEOXact_RelationMap(((bool )1));
/*
	 * set the current transaction state information appropriately during
	 * commit processing
	 */
  s -> state = TRANS_COMMIT;
/*
	 * Here is where we really truly commit.
	 */
  latestXid = RecordTransactionCommit();
  ;
/*
	 * Let others know about no transaction in progress by me. Note that this
	 * must be done _before_ releasing locks we hold and _after_
	 * RecordTransactionCommit.
	 */
  ProcArrayEndTransaction(MyProc,latestXid);
/*
	 * This is all post-commit cleanup.  Note that if an error is raised here,
	 * it's too late to abort the transaction.  This should be just
	 * noncritical resource releasing.
	 *
	 * The ordering of operations is not entirely random.  The idea is:
	 * release resources visible to other backends (eg, files, buffer pins);
	 * then release locks; then release backend-local resources. We want to
	 * release locks at the point where any backend waiting for us will see
	 * our transaction as being fully cleaned up.
	 *
	 * Resources that can be associated with individual queries are handled by
	 * the ResourceOwner mechanism.  The other calls here are for backend-wide
	 * state.
	 */
  CallXactCallbacks(XACT_EVENT_COMMIT);
  ResourceOwnerRelease(TopTransactionResourceOwner,RESOURCE_RELEASE_BEFORE_LOCKS,((bool )1),((bool )1));
/* Check we've released all buffer pins */
  AtEOXact_Buffers(((bool )1));
/* Clean up the relation cache */
  AtEOXact_RelationCache(((bool )1));
/*
	 * Make catalog changes visible to all backends.  This has to happen after
	 * relcache references are dropped (see comments for
	 * AtEOXact_RelationCache), but before locks are released (if anyone is
	 * waiting for lock on a relation we've modified, we want them to know
	 * about the catalog change before they start using the relation).
	 */
  AtEOXact_Inval(((bool )1));
/*
	 * Likewise, dropping of files deleted during the transaction is best done
	 * after releasing relcache and buffer pins.  (This is not strictly
	 * necessary during commit, since such pins should have been released
	 * already, but this ordering is definitely critical during abort.)
	 */
  smgrDoPendingDeletes(((bool )1));
  AtEOXact_MultiXact();
  ResourceOwnerRelease(TopTransactionResourceOwner,RESOURCE_RELEASE_LOCKS,((bool )1),((bool )1));
  ResourceOwnerRelease(TopTransactionResourceOwner,RESOURCE_RELEASE_AFTER_LOCKS,((bool )1),((bool )1));
/* Check we've released all catcache entries */
  AtEOXact_CatCache(((bool )1));
  AtCommit_Notify();
  AtEOXact_GUC(((bool )1),1);
  AtEOXact_SPI(((bool )1));
  AtEOXact_on_commit_actions(((bool )1));
  AtEOXact_Namespace(((bool )1));
  AtEOXact_SMgr();
  AtEOXact_Files();
  AtEOXact_ComboCid();
  AtEOXact_HashTables(((bool )1));
  AtEOXact_PgStat(((bool )1));
  AtEOXact_Snapshot(((bool )1));
  pgstat_report_xact_timestamp(0);
  CurrentResourceOwner = ((void *)0);
  ResourceOwnerDelete(TopTransactionResourceOwner);
  s -> curTransactionOwner = ((void *)0);
  CurTransactionResourceOwner = ((void *)0);
  TopTransactionResourceOwner = ((void *)0);
  AtCommit_Memory();
  s -> transactionId = ((TransactionId )0);
  s -> subTransactionId = ((SubTransactionId )0);
  s -> nestingLevel = 0;
  s -> gucNestLevel = 0;
  s -> childXids = ((void *)0);
  s -> nChildXids = 0;
  s -> maxChildXids = 0;
/*
	 * done with commit processing, set current transaction state back to
	 * default
	 */
  s -> state = TRANS_DEFAULT;
  do {
    ;
    InterruptHoldoffCount--;
  }while (0);
}
/*
 *	PrepareTransaction
 *
 * NB: if you change this routine, better look at CommitTransaction too!
 */

static void PrepareTransaction()
{
  TransactionState s = CurrentTransactionState;
  TransactionId xid = GetCurrentTransactionId();
  GlobalTransaction gxact;
  TimestampTz prepared_at;
  ShowTransactionState("PrepareTransaction");
/*
	 * check the current transaction state
	 */
  if ((s -> state) != TRANS_INPROGRESS) {
    (elog_start("xact.c",2046,__func__) , elog_finish(19,"PrepareTransaction while in %s state",TransStateAsString(s -> state)));
  }
  ;
/*
	 * Do pre-commit processing that involves calling user-defined code, such
	 * as triggers.  Since closing cursors could queue trigger actions,
	 * triggers could open cursors, etc, we have to keep looping until there's
	 * nothing left to do.
	 */
  for (; ; ) {
/*
		 * Fire all currently pending deferred triggers.
		 */
    AfterTriggerFireDeferred();
/*
		 * Close open portals (converting holdable ones into static portals).
		 * If there weren't any, we are done ... otherwise loop back to check
		 * if they queued deferred triggers.  Lather, rinse, repeat.
		 */
    if (!PreCommit_Portals(((bool )1))) {
      break; 
    }
  }
/*
	 * The remaining actions cannot call any user-defined code, so it's safe
	 * to start shutting down within-transaction services.	But note that most
	 * of this stuff could still throw an error, which would switch us into
	 * the transaction-abort path.
	 */
/* Shut down the deferred-trigger manager */
  AfterTriggerEndXact(((bool )1));
/*
	 * Let ON COMMIT management do its thing (must happen after closing
	 * cursors, to avoid dangling-reference problems)
	 */
  PreCommit_on_commit_actions();
/* close large objects before lower-level cleanup */
  AtEOXact_LargeObject(((bool )1));
/*
	 * Mark serializable transaction as complete for predicate locking
	 * purposes.  This should be done as late as we can put it and still allow
	 * errors to be raised for failure patterns found at commit.
	 */
  PreCommit_CheckForSerializationFailure();
/* NOTIFY will be handled below */
/*
	 * Don't allow PREPARE TRANSACTION if we've accessed a temporary table in
	 * this transaction.  Having the prepared xact hold locks on another
	 * backend's temp table seems a bad idea --- for instance it would prevent
	 * the backend from exiting.  There are other problems too, such as how to
	 * clean up the source backend's local buffers and ON COMMIT state if the
	 * prepared xact includes a DROP of a temp table.
	 *
	 * We must check this after executing any ON COMMIT actions, because they
	 * might still access a temp relation.
	 *
	 * XXX In principle this could be relaxed to allow some useful special
	 * cases, such as a temp table created and dropped all within the
	 * transaction.  That seems to require much more bookkeeping though.
	 */
  if (MyXactAccessedTempRel) {
    errstart(20,"xact.c",2118,__func__,((void *)0))?errfinish(errcode((48 - 48 & 0x3F) + ((65 - 48 & 0x3F) << 6) + ((48 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + ((48 - 48 & 0x3F) << 24)),errmsg("cannot PREPARE a transaction that has operated on temporary tables")) : ((void )0);
  }
/*
	 * Likewise, don't allow PREPARE after pg_export_snapshot.  This could be
	 * supported if we added cleanup logic to twophase.c, but for now it
	 * doesn't seem worth the trouble.
	 */
  if (XactHasExportedSnapshots()) {
    errstart(20,"xact.c",2128,__func__,((void *)0))?errfinish(errcode((48 - 48 & 0x3F) + ((65 - 48 & 0x3F) << 6) + ((48 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + ((48 - 48 & 0x3F) << 24)),errmsg("cannot PREPARE a transaction that has exported snapshots")) : ((void )0);
  }
/* Prevent cancel/die interrupt while cleaning up */
  InterruptHoldoffCount++;
/*
	 * set the current transaction state information appropriately during
	 * prepare processing
	 */
  s -> state = TRANS_PREPARE;
  prepared_at = GetCurrentTimestamp();
/* Tell bufmgr and smgr to prepare for commit */
  BufmgrCommit();
/*
	 * Reserve the GID for this transaction. This could fail if the requested
	 * GID is invalid or already in use.
	 */
  gxact = MarkAsPreparing(xid,prepareGID,prepared_at,GetUserId(),MyDatabaseId);
  prepareGID = ((void *)0);
/*
	 * Collect data for the 2PC state file.  Note that in general, no actual
	 * state change should happen in the called modules during this step,
	 * since it's still possible to fail before commit, and in that case we
	 * want transaction abort to be able to clean up.  (In particular, the
	 * AtPrepare routines may error out if they find cases they cannot
	 * handle.)  State cleanup should happen in the PostPrepare routines
	 * below.  However, some modules can go ahead and clear state here because
	 * they wouldn't do anything with it during abort anyway.
	 *
	 * Note: because the 2PC state file records will be replayed in the same
	 * order they are made, the order of these calls has to match the order in
	 * which we want things to happen during COMMIT PREPARED or ROLLBACK
	 * PREPARED; in particular, pay attention to whether things should happen
	 * before or after releasing the transaction's locks.
	 */
  StartPrepare(gxact);
  AtPrepare_Notify();
  AtPrepare_Locks();
  AtPrepare_PredicateLocks();
  AtPrepare_PgStat();
  AtPrepare_MultiXact();
  AtPrepare_RelationMap();
/*
	 * Here is where we really truly prepare.
	 *
	 * We have to record transaction prepares even if we didn't make any
	 * updates, because the transaction manager might get confused if we lose
	 * a global transaction.
	 */
  EndPrepare(gxact);
/*
	 * Now we clean up backend-internal state and release internal resources.
	 */
/* Reset XactLastRecEnd until the next transaction writes something */
  XactLastRecEnd . xrecoff = 0;
/*
	 * Let others know about no transaction in progress by me.	This has to be
	 * done *after* the prepared transaction has been marked valid, else
	 * someone may think it is unlocked and recyclable.
	 */
  ProcArrayClearTransaction(MyProc);
/*
	 * This is all post-transaction cleanup.  Note that if an error is raised
	 * here, it's too late to abort the transaction.  This should be just
	 * noncritical resource releasing.	See notes in CommitTransaction.
	 */
  CallXactCallbacks(XACT_EVENT_PREPARE);
  ResourceOwnerRelease(TopTransactionResourceOwner,RESOURCE_RELEASE_BEFORE_LOCKS,((bool )1),((bool )1));
/* Check we've released all buffer pins */
  AtEOXact_Buffers(((bool )1));
/* Clean up the relation cache */
  AtEOXact_RelationCache(((bool )1));
/* notify doesn't need a postprepare call */
  PostPrepare_PgStat();
  PostPrepare_Inval();
  PostPrepare_smgr();
  PostPrepare_MultiXact(xid);
  PostPrepare_Locks(xid);
  PostPrepare_PredicateLocks(xid);
  ResourceOwnerRelease(TopTransactionResourceOwner,RESOURCE_RELEASE_LOCKS,((bool )1),((bool )1));
  ResourceOwnerRelease(TopTransactionResourceOwner,RESOURCE_RELEASE_AFTER_LOCKS,((bool )1),((bool )1));
/* Check we've released all catcache entries */
  AtEOXact_CatCache(((bool )1));
/* PREPARE acts the same as COMMIT as far as GUC is concerned */
  AtEOXact_GUC(((bool )1),1);
  AtEOXact_SPI(((bool )1));
  AtEOXact_on_commit_actions(((bool )1));
  AtEOXact_Namespace(((bool )1));
  AtEOXact_SMgr();
  AtEOXact_Files();
  AtEOXact_ComboCid();
  AtEOXact_HashTables(((bool )1));
/* don't call AtEOXact_PgStat here */
  AtEOXact_Snapshot(((bool )1));
  CurrentResourceOwner = ((void *)0);
  ResourceOwnerDelete(TopTransactionResourceOwner);
  s -> curTransactionOwner = ((void *)0);
  CurTransactionResourceOwner = ((void *)0);
  TopTransactionResourceOwner = ((void *)0);
  AtCommit_Memory();
  s -> transactionId = ((TransactionId )0);
  s -> subTransactionId = ((SubTransactionId )0);
  s -> nestingLevel = 0;
  s -> gucNestLevel = 0;
  s -> childXids = ((void *)0);
  s -> nChildXids = 0;
  s -> maxChildXids = 0;
/*
	 * done with 1st phase commit processing, set current transaction state
	 * back to default
	 */
  s -> state = TRANS_DEFAULT;
  do {
    ;
    InterruptHoldoffCount--;
  }while (0);
}
/*
 *	AbortTransaction
 */

static void AbortTransaction()
{
  TransactionState s = CurrentTransactionState;
  TransactionId latestXid;
/* Prevent cancel/die interrupt while cleaning up */
  InterruptHoldoffCount++;
/* Make sure we have a valid memory context and resource owner */
  AtAbort_Memory();
  AtAbort_ResourceOwner();
/*
	 * Release any LW locks we might be holding as quickly as possible.
	 * (Regular locks, however, must be held till we finish aborting.)
	 * Releasing LW locks is critical since we might try to grab them again
	 * while cleaning up!
	 */
  LWLockReleaseAll();
/* Clean up buffer I/O and buffer context locks, too */
  AbortBufferIO();
  UnlockBuffers();
/*
	 * Also clean up any open wait for lock, since the lock manager will choke
	 * if we try to wait for another lock before doing this.
	 */
  LockErrorCleanup();
/*
	 * check the current transaction state
	 */
  if ((s -> state) != TRANS_INPROGRESS && (s -> state) != TRANS_PREPARE) {
    (elog_start("xact.c",2317,__func__) , elog_finish(19,"AbortTransaction while in %s state",TransStateAsString(s -> state)));
  }
  ;
/*
	 * set the current transaction state information appropriately during the
	 * abort processing
	 */
  s -> state = TRANS_ABORT;
/*
	 * Reset user ID which might have been changed transiently.  We need this
	 * to clean up in case control escaped out of a SECURITY DEFINER function
	 * or other local change of CurrentUserId; therefore, the prior value of
	 * SecurityRestrictionContext also needs to be restored.
	 *
	 * (Note: it is not necessary to restore session authorization or role
	 * settings here because those can only be changed via GUC, and GUC will
	 * take care of rolling them back if need be.)
	 */
  SetUserIdAndSecContext(s -> prevUser,s -> prevSecContext);
/*
	 * do abort processing
	 */
/* 'false' means it's abort */
  AfterTriggerEndXact(((bool )0));
  AtAbort_Portals();
  AtEOXact_LargeObject(((bool )0));
  AtAbort_Notify();
  AtEOXact_RelationMap(((bool )0));
/*
	 * Advertise the fact that we aborted in pg_clog (assuming that we got as
	 * far as assigning an XID to advertise).
	 */
  latestXid = RecordTransactionAbort(((bool )0));
  ;
/*
	 * Let others know about no transaction in progress by me. Note that this
	 * must be done _before_ releasing locks we hold and _after_
	 * RecordTransactionAbort.
	 */
  ProcArrayEndTransaction(MyProc,latestXid);
/*
	 * Post-abort cleanup.	See notes in CommitTransaction() concerning
	 * ordering.  We can skip all of it if the transaction failed before
	 * creating a resource owner.
	 */
  if (TopTransactionResourceOwner != ((void *)0)) {
    CallXactCallbacks(XACT_EVENT_ABORT);
    ResourceOwnerRelease(TopTransactionResourceOwner,RESOURCE_RELEASE_BEFORE_LOCKS,((bool )0),((bool )1));
    AtEOXact_Buffers(((bool )0));
    AtEOXact_RelationCache(((bool )0));
    AtEOXact_Inval(((bool )0));
    smgrDoPendingDeletes(((bool )0));
    AtEOXact_MultiXact();
    ResourceOwnerRelease(TopTransactionResourceOwner,RESOURCE_RELEASE_LOCKS,((bool )0),((bool )1));
    ResourceOwnerRelease(TopTransactionResourceOwner,RESOURCE_RELEASE_AFTER_LOCKS,((bool )0),((bool )1));
    AtEOXact_CatCache(((bool )0));
    AtEOXact_GUC(((bool )0),1);
    AtEOXact_SPI(((bool )0));
    AtEOXact_on_commit_actions(((bool )0));
    AtEOXact_Namespace(((bool )0));
    AtEOXact_SMgr();
    AtEOXact_Files();
    AtEOXact_ComboCid();
    AtEOXact_HashTables(((bool )0));
    AtEOXact_PgStat(((bool )0));
    pgstat_report_xact_timestamp(0);
  }
/*
	 * State remains TRANS_ABORT until CleanupTransaction().
	 */
  do {
    ;
    InterruptHoldoffCount--;
  }while (0);
}
/*
 *	CleanupTransaction
 */

static void CleanupTransaction()
{
  TransactionState s = CurrentTransactionState;
/*
	 * State should still be TRANS_ABORT from AbortTransaction().
	 */
  if ((s -> state) != TRANS_ABORT) {
    (elog_start("xact.c",2418,__func__) , elog_finish(21,"CleanupTransaction: unexpected state %s",TransStateAsString(s -> state)));
  }
/*
	 * do abort cleanup processing
	 */
/* now safe to release portal memory */
  AtCleanup_Portals();
/* and release the transaction's snapshots */
  AtEOXact_Snapshot(((bool )0));
/* and resource owner */
  CurrentResourceOwner = ((void *)0);
  if (TopTransactionResourceOwner) {
    ResourceOwnerDelete(TopTransactionResourceOwner);
  }
  s -> curTransactionOwner = ((void *)0);
  CurTransactionResourceOwner = ((void *)0);
  TopTransactionResourceOwner = ((void *)0);
/* and transaction memory */
  AtCleanup_Memory();
  s -> transactionId = ((TransactionId )0);
  s -> subTransactionId = ((SubTransactionId )0);
  s -> nestingLevel = 0;
  s -> gucNestLevel = 0;
  s -> childXids = ((void *)0);
  s -> nChildXids = 0;
  s -> maxChildXids = 0;
/*
	 * done with abort processing, set current transaction state back to
	 * default
	 */
  s -> state = TRANS_DEFAULT;
}
/*
 *	StartTransactionCommand
 */

void StartTransactionCommand()
{
  TransactionState s = CurrentTransactionState;
  switch(s -> blockState){
    case TBLOCK_DEFAULT:
{
/*
			 * if we aren't in a transaction block, we just do our usual start
			 * transaction.
			 */
      StartTransaction();
      s -> blockState = TBLOCK_STARTED;
      break; 
    }
    case TBLOCK_INPROGRESS:
{
    }
    case TBLOCK_SUBINPROGRESS:
/*
			 * We are somewhere in a transaction block or subtransaction and
			 * about to start a new command.  For now we do nothing, but
			 * someday we may do command-local resource initialization. (Note
			 * that any needed CommandCounterIncrement was done by the
			 * previous CommitTransactionCommand.)
			 */
    break; 
    case TBLOCK_ABORT:
{
    }
    case TBLOCK_SUBABORT:
/*
			 * Here we are in a failed transaction block (one of the commands
			 * caused an abort) so we do nothing but remain in the abort
			 * state.  Eventually we will get a ROLLBACK command which will
			 * get us out of this state.  (It is up to other code to ensure
			 * that no commands other than ROLLBACK will be processed in these
			 * states.)
			 */
    break; 
    case TBLOCK_STARTED:
{
    }
    case TBLOCK_BEGIN:
{
    }
    case TBLOCK_SUBBEGIN:
{
    }
    case TBLOCK_END:
{
    }
    case TBLOCK_SUBRELEASE:
{
    }
    case TBLOCK_SUBCOMMIT:
{
    }
    case TBLOCK_ABORT_END:
{
    }
    case TBLOCK_SUBABORT_END:
{
    }
    case TBLOCK_ABORT_PENDING:
{
    }
    case TBLOCK_SUBABORT_PENDING:
{
    }
    case TBLOCK_SUBRESTART:
{
    }
    case TBLOCK_SUBABORT_RESTART:
{
    }
    case TBLOCK_PREPARE:
{
/* These cases are invalid. */
      (elog_start("xact.c",2507,__func__) , elog_finish(20,"StartTransactionCommand: unexpected state %s",BlockStateAsString(s -> blockState)));
      break; 
    }
  }
/*
	 * We must switch to CurTransactionContext before returning. This is
	 * already done if we called StartTransaction, otherwise not.
	 */
  ;
  MemoryContextSwitchTo(CurTransactionContext);
}
/*
 *	CommitTransactionCommand
 */

void CommitTransactionCommand()
{
  TransactionState s = CurrentTransactionState;
  switch(s -> blockState){
    case TBLOCK_DEFAULT:
{
/*
			 * This shouldn't happen, because it means the previous
			 * StartTransactionCommand didn't set the STARTED state
			 * appropriately.
			 */
      (elog_start("xact.c",2536,__func__) , elog_finish(21,"CommitTransactionCommand: unexpected state %s",BlockStateAsString(s -> blockState)));
      break; 
    }
    case TBLOCK_STARTED:
{
/*
			 * If we aren't in a transaction block, just do our usual
			 * transaction commit, and return to the idle state.
			 */
      CommitTransaction();
      s -> blockState = TBLOCK_DEFAULT;
      break; 
    }
    case TBLOCK_BEGIN:
{
/*
			 * We are completing a "BEGIN TRANSACTION" command, so we change
			 * to the "transaction block in progress" state and return.  (We
			 * assume the BEGIN did nothing to the database, so we need no
			 * CommandCounterIncrement.)
			 */
      s -> blockState = TBLOCK_INPROGRESS;
      break; 
    }
    case TBLOCK_INPROGRESS:
{
    }
    case TBLOCK_SUBINPROGRESS:
{
/*
			 * This is the case when we have finished executing a command
			 * someplace within a transaction block.  We increment the command
			 * counter and return.
			 */
      CommandCounterIncrement();
      break; 
    }
    case TBLOCK_END:
{
/*
			 * We are completing a "COMMIT" command.  Do it and return to the
			 * idle state.
			 */
      CommitTransaction();
      s -> blockState = TBLOCK_DEFAULT;
      break; 
    }
    case TBLOCK_ABORT:
{
    }
    case TBLOCK_SUBABORT:
/*
			 * Here we are in the middle of a transaction block but one of the
			 * commands caused an abort so we do nothing but remain in the
			 * abort state.  Eventually we will get a ROLLBACK comand.
			 */
    break; 
    case TBLOCK_ABORT_END:
{
/*
			 * Here we were in an aborted transaction block and we just got
			 * the ROLLBACK command from the user, so clean up the
			 * already-aborted transaction and return to the idle state.
			 */
      CleanupTransaction();
      s -> blockState = TBLOCK_DEFAULT;
      break; 
    }
    case TBLOCK_ABORT_PENDING:
{
/*
			 * Here we were in a perfectly good transaction block but the user
			 * told us to ROLLBACK anyway.	We have to abort the transaction
			 * and then clean up.
			 */
      AbortTransaction();
      CleanupTransaction();
      s -> blockState = TBLOCK_DEFAULT;
      break; 
    }
    case TBLOCK_PREPARE:
{
/*
			 * We are completing a "PREPARE TRANSACTION" command.  Do it and
			 * return to the idle state.
			 */
      PrepareTransaction();
      s -> blockState = TBLOCK_DEFAULT;
      break; 
    }
    case TBLOCK_SUBBEGIN:
{
/*
			 * We were just issued a SAVEPOINT inside a transaction block.
			 * Start a subtransaction.	(DefineSavepoint already did
			 * PushTransaction, so as to have someplace to put the SUBBEGIN
			 * state.)
			 */
      StartSubTransaction();
      s -> blockState = TBLOCK_SUBINPROGRESS;
      break; 
    }
    case TBLOCK_SUBRELEASE:
{
/*
			 * We were issued a RELEASE command, so we end the current
			 * subtransaction and return to the parent transaction. The parent
			 * might be ended too, so repeat till we find an INPROGRESS
			 * transaction or subtransaction.
			 */
      do {
        CommitSubTransaction();
/* changed by pop */
        s = CurrentTransactionState;
      }while ((s -> blockState) == TBLOCK_SUBRELEASE);
      ;
      break; 
    }
    case TBLOCK_SUBCOMMIT:
{
/*
			 * We were issued a COMMIT, so we end the current subtransaction
			 * hierarchy and perform final commit. We do this by rolling up
			 * any subtransactions into their parent, which leads to O(N^2)
			 * operations with respect to resource owners - this isn't that
			 * bad until we approach a thousands of savepoints but is
			 * necessary for correctness should after triggers create new
			 * resource owners.
			 */
      do {
        CommitSubTransaction();
/* changed by pop */
        s = CurrentTransactionState;
      }while ((s -> blockState) == TBLOCK_SUBCOMMIT);
/* If we had a COMMIT command, finish off the main xact too */
      if ((s -> blockState) == TBLOCK_END) {
        ;
        CommitTransaction();
        s -> blockState = TBLOCK_DEFAULT;
      }
      else {
        if ((s -> blockState) == TBLOCK_PREPARE) {
          ;
          PrepareTransaction();
          s -> blockState = TBLOCK_DEFAULT;
        }
        else {
          (elog_start("xact.c",2674,__func__) , elog_finish(20,"CommitTransactionCommand: unexpected state %s",BlockStateAsString(s -> blockState)));
        }
      }
      break; 
    }
    case TBLOCK_SUBABORT_END:
{
/*
			 * The current already-failed subtransaction is ending due to a
			 * ROLLBACK or ROLLBACK TO command, so pop it and recursively
			 * examine the parent (which could be in any of several states).
			 */
      CleanupSubTransaction();
      CommitTransactionCommand();
      break; 
    }
    case TBLOCK_SUBABORT_PENDING:
{
/*
			 * As above, but it's not dead yet, so abort first.
			 */
      AbortSubTransaction();
      CleanupSubTransaction();
      CommitTransactionCommand();
      break; 
    }
    case TBLOCK_SUBRESTART:
{
/*
			 * The current subtransaction is the target of a ROLLBACK TO
			 * command.  Abort and pop it, then start a new subtransaction
			 * with the same name.
			 */
{
        char *name;
        int savepointLevel;
/* save name and keep Cleanup from freeing it */
        name = s -> name;
        s -> name = ((void *)0);
        savepointLevel = s -> savepointLevel;
        AbortSubTransaction();
        CleanupSubTransaction();
        DefineSavepoint(((void *)0));
/* changed by push */
        s = CurrentTransactionState;
        s -> name = name;
        s -> savepointLevel = savepointLevel;
/* This is the same as TBLOCK_SUBBEGIN case */
        ;
        StartSubTransaction();
        s -> blockState = TBLOCK_SUBINPROGRESS;
      }
      break; 
    }
    case TBLOCK_SUBABORT_RESTART:
{
/*
			 * Same as above, but the subtransaction had already failed, so we
			 * don't need AbortSubTransaction.
			 */
{
        char *name;
        int savepointLevel;
/* save name and keep Cleanup from freeing it */
        name = s -> name;
        s -> name = ((void *)0);
        savepointLevel = s -> savepointLevel;
        CleanupSubTransaction();
        DefineSavepoint(((void *)0));
/* changed by push */
        s = CurrentTransactionState;
        s -> name = name;
        s -> savepointLevel = savepointLevel;
/* This is the same as TBLOCK_SUBBEGIN case */
        ;
        StartSubTransaction();
        s -> blockState = TBLOCK_SUBINPROGRESS;
      }
      break; 
    }
  }
}
/*
 *	AbortCurrentTransaction
 */

void AbortCurrentTransaction()
{
  TransactionState s = CurrentTransactionState;
  switch(s -> blockState){
    case TBLOCK_DEFAULT:
{
      if ((s -> state) == TRANS_DEFAULT) {
/* we are idle, so nothing to do */
      }
      else {
/*
				 * We can get here after an error during transaction start
				 * (state will be TRANS_START).  Need to clean up the
				 * incompletely started transaction.  First, adjust the
				 * low-level state to suppress warning message from
				 * AbortTransaction.
				 */
        if ((s -> state) == TRANS_START) {
          s -> state = TRANS_INPROGRESS;
        }
        AbortTransaction();
        CleanupTransaction();
      }
      break; 
    }
    case TBLOCK_STARTED:
{
/*
			 * if we aren't in a transaction block, we just do the basic abort
			 * & cleanup transaction.
			 */
      AbortTransaction();
      CleanupTransaction();
      s -> blockState = TBLOCK_DEFAULT;
      break; 
    }
    case TBLOCK_BEGIN:
{
/*
			 * If we are in TBLOCK_BEGIN it means something screwed up right
			 * after reading "BEGIN TRANSACTION".  We assume that the user
			 * will interpret the error as meaning the BEGIN failed to get him
			 * into a transaction block, so we should abort and return to idle
			 * state.
			 */
      AbortTransaction();
      CleanupTransaction();
      s -> blockState = TBLOCK_DEFAULT;
      break; 
    }
    case TBLOCK_INPROGRESS:
{
/*
			 * We are somewhere in a transaction block and we've gotten a
			 * failure, so we abort the transaction and set up the persistent
			 * ABORT state.  We will stay in ABORT until we get a ROLLBACK.
			 */
      AbortTransaction();
      s -> blockState = TBLOCK_ABORT;
/* CleanupTransaction happens when we exit TBLOCK_ABORT_END */
      break; 
    }
    case TBLOCK_END:
{
/*
			 * Here, we failed while trying to COMMIT.	Clean up the
			 * transaction and return to idle state (we do not want to stay in
			 * the transaction).
			 */
      AbortTransaction();
      CleanupTransaction();
      s -> blockState = TBLOCK_DEFAULT;
      break; 
    }
    case TBLOCK_ABORT:
{
    }
    case TBLOCK_SUBABORT:
/*
			 * Here, we are already in an aborted transaction state and are
			 * waiting for a ROLLBACK, but for some reason we failed again! So
			 * we just remain in the abort state.
			 */
    break; 
    case TBLOCK_ABORT_END:
{
/*
			 * We are in a failed transaction and we got the ROLLBACK command.
			 * We have already aborted, we just need to cleanup and go to idle
			 * state.
			 */
      CleanupTransaction();
      s -> blockState = TBLOCK_DEFAULT;
      break; 
    }
    case TBLOCK_ABORT_PENDING:
{
/*
			 * We are in a live transaction and we got a ROLLBACK command.
			 * Abort, cleanup, go to idle state.
			 */
      AbortTransaction();
      CleanupTransaction();
      s -> blockState = TBLOCK_DEFAULT;
      break; 
    }
    case TBLOCK_PREPARE:
{
/*
			 * Here, we failed while trying to PREPARE.  Clean up the
			 * transaction and return to idle state (we do not want to stay in
			 * the transaction).
			 */
      AbortTransaction();
      CleanupTransaction();
      s -> blockState = TBLOCK_DEFAULT;
      break; 
    }
    case TBLOCK_SUBINPROGRESS:
{
/*
			 * We got an error inside a subtransaction.  Abort just the
			 * subtransaction, and go to the persistent SUBABORT state until
			 * we get ROLLBACK.
			 */
      AbortSubTransaction();
      s -> blockState = TBLOCK_SUBABORT;
      break; 
    }
    case TBLOCK_SUBBEGIN:
{
    }
    case TBLOCK_SUBRELEASE:
{
    }
    case TBLOCK_SUBCOMMIT:
{
    }
    case TBLOCK_SUBABORT_PENDING:
{
    }
    case TBLOCK_SUBRESTART:
{
/*
			 * If we failed while trying to create a subtransaction, clean up
			 * the broken subtransaction and abort the parent.	The same
			 * applies if we get a failure while ending a subtransaction.
			 */
      AbortSubTransaction();
      CleanupSubTransaction();
      AbortCurrentTransaction();
      break; 
    }
    case TBLOCK_SUBABORT_END:
{
    }
    case TBLOCK_SUBABORT_RESTART:
{
/*
			 * Same as above, except the Abort() was already done.
			 */
      CleanupSubTransaction();
      AbortCurrentTransaction();
      break; 
    }
  }
}
/*
 *	PreventTransactionChain
 *
 *	This routine is to be called by statements that must not run inside
 *	a transaction block, typically because they have non-rollback-able
 *	side effects or do internal commits.
 *
 *	If we have already started a transaction block, issue an error; also issue
 *	an error if we appear to be running inside a user-defined function (which
 *	could issue more commands and possibly cause a failure after the statement
 *	completes).  Subtransactions are verboten too.
 *
 *	isTopLevel: passed down from ProcessUtility to determine whether we are
 *	inside a function or multi-query querystring.  (We will always fail if
 *	this is false, but it's convenient to centralize the check here instead of
 *	making callers do it.)
 *	stmtType: statement type name, for error messages.
 */

void PreventTransactionChain(bool isTopLevel,const char *stmtType)
{
/*
	 * xact block already started?
	 */
  if (IsTransactionBlock()) {
    errstart(20,"xact.c",2938,__func__,((void *)0))?errfinish(errcode((50 - 48 & 0x3F) + (('5' - 48 & 0x3F) << 6) + ((48 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('1' - 48 & 0x3F) << 24)),errmsg("%s cannot run inside a transaction block",stmtType)) : ((void )0);
  }
/* translator: %s represents an SQL statement name */
/*
	 * subtransaction?
	 */
  if (IsSubTransaction()) {
    errstart(20,"xact.c",2948,__func__,((void *)0))?errfinish(errcode((50 - 48 & 0x3F) + (('5' - 48 & 0x3F) << 6) + ((48 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('1' - 48 & 0x3F) << 24)),errmsg("%s cannot run inside a subtransaction",stmtType)) : ((void )0);
  }
/* translator: %s represents an SQL statement name */
/*
	 * inside a function call?
	 */
  if (!isTopLevel) {
    errstart(20,"xact.c",2958,__func__,((void *)0))?errfinish(errcode((50 - 48 & 0x3F) + (('5' - 48 & 0x3F) << 6) + ((48 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('1' - 48 & 0x3F) << 24)),errmsg("%s cannot be executed from a function or multi-command string",stmtType)) : ((void )0);
  }
/* translator: %s represents an SQL statement name */
/* If we got past IsTransactionBlock test, should be in default state */
  if ((CurrentTransactionState -> blockState) != TBLOCK_DEFAULT && (CurrentTransactionState -> blockState) != TBLOCK_STARTED) {
    (elog_start("xact.c",2963,__func__) , elog_finish(21,"cannot prevent transaction chain"));
  }
/* all okay */
}
/*
 *	RequireTransactionChain
 *
 *	This routine is to be called by statements that must run inside
 *	a transaction block, because they have no effects that persist past
 *	transaction end (and so calling them outside a transaction block
 *	is presumably an error).  DECLARE CURSOR is an example.
 *
 *	If we appear to be running inside a user-defined function, we do not
 *	issue an error, since the function could issue more commands that make
 *	use of the current statement's results.  Likewise subtransactions.
 *	Thus this is an inverse for PreventTransactionChain.
 *
 *	isTopLevel: passed down from ProcessUtility to determine whether we are
 *	inside a function.
 *	stmtType: statement type name, for error messages.
 */

void RequireTransactionChain(bool isTopLevel,const char *stmtType)
{
/*
	 * xact block already started?
	 */
  if (IsTransactionBlock()) {
    return ;
  }
/*
	 * subtransaction?
	 */
  if (IsSubTransaction()) {
    return ;
  }
/*
	 * inside a function call?
	 */
  if (!isTopLevel) {
    return ;
  }
  errstart(20,"xact.c",3009,__func__,((void *)0))?errfinish(errcode((50 - 48 & 0x3F) + (('5' - 48 & 0x3F) << 6) + ((80 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('1' - 48 & 0x3F) << 24)),errmsg("%s can only be used in transaction blocks",stmtType)) : ((void )0);
/* translator: %s represents an SQL statement name */
}
/*
 *	IsInTransactionChain
 *
 *	This routine is for statements that need to behave differently inside
 *	a transaction block than when running as single commands.  ANALYZE is
 *	currently the only example.
 *
 *	isTopLevel: passed down from ProcessUtility to determine whether we are
 *	inside a function.
 */

bool IsInTransactionChain(bool isTopLevel)
{
/*
	 * Return true on same conditions that would make PreventTransactionChain
	 * error out
	 */
  if (IsTransactionBlock()) {
    return (bool )1;
  }
  if (IsSubTransaction()) {
    return (bool )1;
  }
  if (!isTopLevel) {
    return (bool )1;
  }
  if ((CurrentTransactionState -> blockState) != TBLOCK_DEFAULT && (CurrentTransactionState -> blockState) != TBLOCK_STARTED) {
    return (bool )1;
  }
  return (bool )0;
}
/*
 * Register or deregister callback functions for start- and end-of-xact
 * operations.
 *
 * These functions are intended for use by dynamically loaded modules.
 * For built-in modules we generally just hardwire the appropriate calls
 * (mainly because it's easier to control the order that way, where needed).
 *
 * At transaction end, the callback occurs post-commit or post-abort, so the
 * callback functions can only do noncritical cleanup.
 */

void RegisterXactCallback(XactCallback callback,void *arg)
{
  XactCallbackItem *item;
  item = ((XactCallbackItem *)(MemoryContextAlloc(TopMemoryContext,sizeof(XactCallbackItem ))));
  item -> callback = callback;
  item -> arg = arg;
  item -> next = Xact_callbacks;
  Xact_callbacks = item;
}

void UnregisterXactCallback(XactCallback callback,void *arg)
{
  XactCallbackItem *item;
  XactCallbackItem *prev;
  prev = ((void *)0);
  for (item = Xact_callbacks; item; (prev = item , item = item -> next)) {
    if (item -> callback == callback && item -> arg == arg) {
      if (prev) {
        prev -> next = item -> next;
      }
      else {
        Xact_callbacks = item -> next;
      }
      pfree(item);
      break; 
    }
  }
}

static void CallXactCallbacks(XactEvent event)
{
  XactCallbackItem *item;
  for (item = Xact_callbacks; item; item = item -> next) 
    ( *item -> callback)(event,item -> arg);
}
/*
 * Register or deregister callback functions for start- and end-of-subxact
 * operations.
 *
 * Pretty much same as above, but for subtransaction events.
 *
 * At subtransaction end, the callback occurs post-subcommit or post-subabort,
 * so the callback functions can only do noncritical cleanup.  At
 * subtransaction start, the callback is called when the subtransaction has
 * finished initializing.
 */

void RegisterSubXactCallback(SubXactCallback callback,void *arg)
{
  SubXactCallbackItem *item;
  item = ((SubXactCallbackItem *)(MemoryContextAlloc(TopMemoryContext,sizeof(SubXactCallbackItem ))));
  item -> callback = callback;
  item -> arg = arg;
  item -> next = SubXact_callbacks;
  SubXact_callbacks = item;
}

void UnregisterSubXactCallback(SubXactCallback callback,void *arg)
{
  SubXactCallbackItem *item;
  SubXactCallbackItem *prev;
  prev = ((void *)0);
  for (item = SubXact_callbacks; item; (prev = item , item = item -> next)) {
    if (item -> callback == callback && item -> arg == arg) {
      if (prev) {
        prev -> next = item -> next;
      }
      else {
        SubXact_callbacks = item -> next;
      }
      pfree(item);
      break; 
    }
  }
}

static void CallSubXactCallbacks(SubXactEvent event,SubTransactionId mySubid,SubTransactionId parentSubid)
{
  SubXactCallbackItem *item;
  for (item = SubXact_callbacks; item; item = item -> next) 
    ( *item -> callback)(event,mySubid,parentSubid,item -> arg);
}
/* ----------------------------------------------------------------
 *					   transaction block support
 * ----------------------------------------------------------------
 */
/*
 *	BeginTransactionBlock
 *		This executes a BEGIN command.
 */

void BeginTransactionBlock()
{
  TransactionState s = CurrentTransactionState;
  switch(s -> blockState){
    case TBLOCK_STARTED:
{
/*
			 * We are not inside a transaction block, so allow one to begin.
			 */
      s -> blockState = TBLOCK_BEGIN;
      break; 
    }
    case TBLOCK_INPROGRESS:
{
    }
    case TBLOCK_SUBINPROGRESS:
{
    }
    case TBLOCK_ABORT:
{
    }
    case TBLOCK_SUBABORT:
{
/*
			 * Already a transaction block in progress.
			 */
      errstart(19,"xact.c",3190,__func__,((void *)0))?errfinish(errcode((50 - 48 & 0x3F) + (('5' - 48 & 0x3F) << 6) + ((48 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('1' - 48 & 0x3F) << 24)),errmsg("there is already a transaction in progress")) : ((void )0);
      break; 
    }
    case TBLOCK_DEFAULT:
{
    }
    case TBLOCK_BEGIN:
{
    }
    case TBLOCK_SUBBEGIN:
{
    }
    case TBLOCK_END:
{
    }
    case TBLOCK_SUBRELEASE:
{
    }
    case TBLOCK_SUBCOMMIT:
{
    }
    case TBLOCK_ABORT_END:
{
    }
    case TBLOCK_SUBABORT_END:
{
    }
    case TBLOCK_ABORT_PENDING:
{
    }
    case TBLOCK_SUBABORT_PENDING:
{
    }
    case TBLOCK_SUBRESTART:
{
    }
    case TBLOCK_SUBABORT_RESTART:
{
    }
    case TBLOCK_PREPARE:
{
/* These cases are invalid. */
      (elog_start("xact.c",3207,__func__) , elog_finish(21,"BeginTransactionBlock: unexpected state %s",BlockStateAsString(s -> blockState)));
      break; 
    }
  }
}
/*
 *	PrepareTransactionBlock
 *		This executes a PREPARE command.
 *
 * Since PREPARE may actually do a ROLLBACK, the result indicates what
 * happened: TRUE for PREPARE, FALSE for ROLLBACK.
 *
 * Note that we don't actually do anything here except change blockState.
 * The real work will be done in the upcoming PrepareTransaction().
 * We do it this way because it's not convenient to change memory context,
 * resource owner, etc while executing inside a Portal.
 */

bool PrepareTransactionBlock(char *gid)
{
  TransactionState s;
  bool result;
/* Set up to commit the current transaction */
  result = EndTransactionBlock();
/* If successful, change outer tblock state to PREPARE */
  if (result) {
    s = CurrentTransactionState;
    while(s -> parent != ((void *)0))
      s = s -> parent;
    if ((s -> blockState) == TBLOCK_END) {
/* Save GID where PrepareTransaction can find it again */
      prepareGID = MemoryContextStrdup(TopTransactionContext,gid);
      s -> blockState = TBLOCK_PREPARE;
    }
    else {
/*
			 * ignore case where we are not in a transaction;
			 * EndTransactionBlock already issued a warning.
			 */
      ;
/* Don't send back a PREPARE result tag... */
      result = ((bool )0);
    }
  }
  return result;
}
/*
 *	EndTransactionBlock
 *		This executes a COMMIT command.
 *
 * Since COMMIT may actually do a ROLLBACK, the result indicates what
 * happened: TRUE for COMMIT, FALSE for ROLLBACK.
 *
 * Note that we don't actually do anything here except change blockState.
 * The real work will be done in the upcoming CommitTransactionCommand().
 * We do it this way because it's not convenient to change memory context,
 * resource owner, etc while executing inside a Portal.
 */

bool EndTransactionBlock()
{
  TransactionState s = CurrentTransactionState;
  bool result = (bool )0;
  switch(s -> blockState){
    case TBLOCK_INPROGRESS:
{
/*
			 * We are in a transaction block, so tell CommitTransactionCommand
			 * to COMMIT.
			 */
      s -> blockState = TBLOCK_END;
      result = ((bool )1);
      break; 
    }
    case TBLOCK_ABORT:
{
/*
			 * We are in a failed transaction block.  Tell
			 * CommitTransactionCommand it's time to exit the block.
			 */
      s -> blockState = TBLOCK_ABORT_END;
      break; 
    }
    case TBLOCK_SUBINPROGRESS:
{
/*
			 * We are in a live subtransaction block.  Set up to subcommit all
			 * open subtransactions and then commit the main transaction.
			 */
      while(s -> parent != ((void *)0)){
        if ((s -> blockState) == TBLOCK_SUBINPROGRESS) {
          s -> blockState = TBLOCK_SUBCOMMIT;
        }
        else {
          (elog_start("xact.c",3311,__func__) , elog_finish(21,"EndTransactionBlock: unexpected state %s",BlockStateAsString(s -> blockState)));
        }
        s = s -> parent;
      }
      if ((s -> blockState) == TBLOCK_INPROGRESS) {
        s -> blockState = TBLOCK_END;
      }
      else {
        (elog_start("xact.c",3318,__func__) , elog_finish(21,"EndTransactionBlock: unexpected state %s",BlockStateAsString(s -> blockState)));
      }
      result = ((bool )1);
      break; 
    }
    case TBLOCK_SUBABORT:
{
/*
			 * Here we are inside an aborted subtransaction.  Treat the COMMIT
			 * as ROLLBACK: set up to abort everything and exit the main
			 * transaction.
			 */
      while(s -> parent != ((void *)0)){
        if ((s -> blockState) == TBLOCK_SUBINPROGRESS) {
          s -> blockState = TBLOCK_SUBABORT_PENDING;
        }
        else {
          if ((s -> blockState) == TBLOCK_SUBABORT) {
            s -> blockState = TBLOCK_SUBABORT_END;
          }
          else {
            (elog_start("xact.c",3336,__func__) , elog_finish(21,"EndTransactionBlock: unexpected state %s",BlockStateAsString(s -> blockState)));
          }
        }
        s = s -> parent;
      }
      if ((s -> blockState) == TBLOCK_INPROGRESS) {
        s -> blockState = TBLOCK_ABORT_PENDING;
      }
      else {
        if ((s -> blockState) == TBLOCK_ABORT) {
          s -> blockState = TBLOCK_ABORT_END;
        }
        else {
          (elog_start("xact.c",3345,__func__) , elog_finish(21,"EndTransactionBlock: unexpected state %s",BlockStateAsString(s -> blockState)));
        }
      }
      break; 
    }
    case TBLOCK_STARTED:
{
/*
			 * The user issued COMMIT when not inside a transaction.  Issue a
			 * WARNING, staying in TBLOCK_STARTED state.  The upcoming call to
			 * CommitTransactionCommand() will then close the transaction and
			 * put us back into the default state.
			 */
      errstart(19,"xact.c",3358,__func__,((void *)0))?errfinish(errcode((50 - 48 & 0x3F) + (('5' - 48 & 0x3F) << 6) + ((80 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('1' - 48 & 0x3F) << 24)),errmsg("there is no transaction in progress")) : ((void )0);
      result = ((bool )1);
      break; 
    }
    case TBLOCK_DEFAULT:
{
    }
    case TBLOCK_BEGIN:
{
    }
    case TBLOCK_SUBBEGIN:
{
    }
    case TBLOCK_END:
{
    }
    case TBLOCK_SUBRELEASE:
{
    }
    case TBLOCK_SUBCOMMIT:
{
    }
    case TBLOCK_ABORT_END:
{
    }
    case TBLOCK_SUBABORT_END:
{
    }
    case TBLOCK_ABORT_PENDING:
{
    }
    case TBLOCK_SUBABORT_PENDING:
{
    }
    case TBLOCK_SUBRESTART:
{
    }
    case TBLOCK_SUBABORT_RESTART:
{
    }
    case TBLOCK_PREPARE:
{
/* These cases are invalid. */
      (elog_start("xact.c",3376,__func__) , elog_finish(21,"EndTransactionBlock: unexpected state %s",BlockStateAsString(s -> blockState)));
      break; 
    }
  }
  return result;
}
/*
 *	UserAbortTransactionBlock
 *		This executes a ROLLBACK command.
 *
 * As above, we don't actually do anything here except change blockState.
 */

void UserAbortTransactionBlock()
{
  TransactionState s = CurrentTransactionState;
  switch(s -> blockState){
    case TBLOCK_INPROGRESS:
{
/*
			 * We are inside a transaction block and we got a ROLLBACK command
			 * from the user, so tell CommitTransactionCommand to abort and
			 * exit the transaction block.
			 */
      s -> blockState = TBLOCK_ABORT_PENDING;
      break; 
    }
    case TBLOCK_ABORT:
{
/*
			 * We are inside a failed transaction block and we got a ROLLBACK
			 * command from the user.  Abort processing is already done, so
			 * CommitTransactionCommand just has to cleanup and go back to
			 * idle state.
			 */
      s -> blockState = TBLOCK_ABORT_END;
      break; 
    }
    case TBLOCK_SUBINPROGRESS:
{
    }
    case TBLOCK_SUBABORT:
{
/*
			 * We are inside a subtransaction.	Mark everything up to top
			 * level as exitable.
			 */
      while(s -> parent != ((void *)0)){
        if ((s -> blockState) == TBLOCK_SUBINPROGRESS) {
          s -> blockState = TBLOCK_SUBABORT_PENDING;
        }
        else {
          if ((s -> blockState) == TBLOCK_SUBABORT) {
            s -> blockState = TBLOCK_SUBABORT_END;
          }
          else {
            (elog_start("xact.c",3429,__func__) , elog_finish(21,"UserAbortTransactionBlock: unexpected state %s",BlockStateAsString(s -> blockState)));
          }
        }
        s = s -> parent;
      }
      if ((s -> blockState) == TBLOCK_INPROGRESS) {
        s -> blockState = TBLOCK_ABORT_PENDING;
      }
      else {
        if ((s -> blockState) == TBLOCK_ABORT) {
          s -> blockState = TBLOCK_ABORT_END;
        }
        else {
          (elog_start("xact.c",3438,__func__) , elog_finish(21,"UserAbortTransactionBlock: unexpected state %s",BlockStateAsString(s -> blockState)));
        }
      }
      break; 
    }
    case TBLOCK_STARTED:
{
/*
			 * The user issued ABORT when not inside a transaction. Issue a
			 * WARNING and go to abort state.  The upcoming call to
			 * CommitTransactionCommand() will then put us back into the
			 * default state.
			 */
      errstart(18,"xact.c",3451,__func__,((void *)0))?errfinish(errcode((50 - 48 & 0x3F) + (('5' - 48 & 0x3F) << 6) + ((80 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('1' - 48 & 0x3F) << 24)),errmsg("there is no transaction in progress")) : ((void )0);
      s -> blockState = TBLOCK_ABORT_PENDING;
      break; 
    }
    case TBLOCK_DEFAULT:
{
    }
    case TBLOCK_BEGIN:
{
    }
    case TBLOCK_SUBBEGIN:
{
    }
    case TBLOCK_END:
{
    }
    case TBLOCK_SUBRELEASE:
{
    }
    case TBLOCK_SUBCOMMIT:
{
    }
    case TBLOCK_ABORT_END:
{
    }
    case TBLOCK_SUBABORT_END:
{
    }
    case TBLOCK_ABORT_PENDING:
{
    }
    case TBLOCK_SUBABORT_PENDING:
{
    }
    case TBLOCK_SUBRESTART:
{
    }
    case TBLOCK_SUBABORT_RESTART:
{
    }
    case TBLOCK_PREPARE:
{
/* These cases are invalid. */
      (elog_start("xact.c",3469,__func__) , elog_finish(21,"UserAbortTransactionBlock: unexpected state %s",BlockStateAsString(s -> blockState)));
      break; 
    }
  }
}
/*
 * DefineSavepoint
 *		This executes a SAVEPOINT command.
 */

void DefineSavepoint(char *name)
{
  TransactionState s = CurrentTransactionState;
  switch(s -> blockState){
    case TBLOCK_INPROGRESS:
{
    }
    case TBLOCK_SUBINPROGRESS:
{
/* Normal subtransaction start */
      PushTransaction();
/* changed by push */
      s = CurrentTransactionState;
/*
			 * Savepoint names, like the TransactionState block itself, live
			 * in TopTransactionContext.
			 */
      if (name) {
        s -> name = MemoryContextStrdup(TopTransactionContext,name);
      }
      break; 
    }
    case TBLOCK_DEFAULT:
{
    }
    case TBLOCK_STARTED:
{
    }
    case TBLOCK_BEGIN:
{
    }
    case TBLOCK_SUBBEGIN:
{
    }
    case TBLOCK_END:
{
    }
    case TBLOCK_SUBRELEASE:
{
    }
    case TBLOCK_SUBCOMMIT:
{
    }
    case TBLOCK_ABORT:
{
    }
    case TBLOCK_SUBABORT:
{
    }
    case TBLOCK_ABORT_END:
{
    }
    case TBLOCK_SUBABORT_END:
{
    }
    case TBLOCK_ABORT_PENDING:
{
    }
    case TBLOCK_SUBABORT_PENDING:
{
    }
    case TBLOCK_SUBRESTART:
{
    }
    case TBLOCK_SUBABORT_RESTART:
{
    }
    case TBLOCK_PREPARE:
{
/* These cases are invalid. */
      (elog_start("xact.c",3517,__func__) , elog_finish(21,"DefineSavepoint: unexpected state %s",BlockStateAsString(s -> blockState)));
      break; 
    }
  }
}
/*
 * ReleaseSavepoint
 *		This executes a RELEASE command.
 *
 * As above, we don't actually do anything here except change blockState.
 */

void ReleaseSavepoint(List *options)
{
  TransactionState s = CurrentTransactionState;
  TransactionState target;
  TransactionState xact;
  ListCell *cell;
  char *name = ((void *)0);
  switch(s -> blockState){
    case TBLOCK_INPROGRESS:
{
/*
			 * We can't rollback to a savepoint if there is no savepoint
			 * defined.
			 */
      errstart(20,"xact.c",3547,__func__,((void *)0))?errfinish(errcode(('3' - 48 & 0x3F) + (('B' - 48 & 0x3F) << 6) + ((48 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('1' - 48 & 0x3F) << 24)),errmsg("no such savepoint")) : ((void )0);
      break; 
    }
    case TBLOCK_SUBINPROGRESS:
/*
			 * We are in a non-aborted subtransaction.	This is the only valid
			 * case.
			 */
    break; 
    case TBLOCK_DEFAULT:
{
    }
    case TBLOCK_STARTED:
{
    }
    case TBLOCK_BEGIN:
{
    }
    case TBLOCK_SUBBEGIN:
{
    }
    case TBLOCK_END:
{
    }
    case TBLOCK_SUBRELEASE:
{
    }
    case TBLOCK_SUBCOMMIT:
{
    }
    case TBLOCK_ABORT:
{
    }
    case TBLOCK_SUBABORT:
{
    }
    case TBLOCK_ABORT_END:
{
    }
    case TBLOCK_SUBABORT_END:
{
    }
    case TBLOCK_ABORT_PENDING:
{
    }
    case TBLOCK_SUBABORT_PENDING:
{
    }
    case TBLOCK_SUBRESTART:
{
    }
    case TBLOCK_SUBABORT_RESTART:
{
    }
    case TBLOCK_PREPARE:
{
/* These cases are invalid. */
      (elog_start("xact.c",3574,__func__) , elog_finish(21,"ReleaseSavepoint: unexpected state %s",BlockStateAsString(s -> blockState)));
      break; 
    }
  }
  for (cell = list_head(options); cell != ((void *)0); cell = cell -> next) {
    DefElem *elem = cell -> data . ptr_value;
    if (strcmp((elem -> defname),"savepoint_name") == 0) {
      name = ((Value *)(elem -> arg)) -> val . str;
    }
  }
  ;
  for (target = s; ((const void *)target) != ((void *)0); target = target -> parent) {
    if (((const void *)(target -> name)) != ((void *)0) && strcmp((target -> name),name) == 0) {
      break; 
    }
  }
  if (!(((const void *)target) != ((void *)0))) {
    errstart(20,"xact.c",3598,__func__,((void *)0))?errfinish(errcode(('3' - 48 & 0x3F) + (('B' - 48 & 0x3F) << 6) + ((48 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('1' - 48 & 0x3F) << 24)),errmsg("no such savepoint")) : ((void )0);
  }
/* disallow crossing savepoint level boundaries */
  if (target -> savepointLevel != s -> savepointLevel) {
    errstart(20,"xact.c",3604,__func__,((void *)0))?errfinish(errcode(('3' - 48 & 0x3F) + (('B' - 48 & 0x3F) << 6) + ((48 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('1' - 48 & 0x3F) << 24)),errmsg("no such savepoint")) : ((void )0);
  }
/*
	 * Mark "commit pending" all subtransactions up to the target
	 * subtransaction.	The actual commits will happen when control gets to
	 * CommitTransactionCommand.
	 */
  xact = CurrentTransactionState;
  for (; ; ) {
    ;
    xact -> blockState = TBLOCK_SUBRELEASE;
    if (xact == target) {
      break; 
    }
    xact = xact -> parent;
    ;
  }
}
/*
 * RollbackToSavepoint
 *		This executes a ROLLBACK TO <savepoint> command.
 *
 * As above, we don't actually do anything here except change blockState.
 */

void RollbackToSavepoint(List *options)
{
  TransactionState s = CurrentTransactionState;
  TransactionState target;
  TransactionState xact;
  ListCell *cell;
  char *name = ((void *)0);
  switch(s -> blockState){
    case TBLOCK_INPROGRESS:
{
    }
    case TBLOCK_ABORT:
{
/*
			 * We can't rollback to a savepoint if there is no savepoint
			 * defined.
			 */
      errstart(20,"xact.c",3648,__func__,((void *)0))?errfinish(errcode(('3' - 48 & 0x3F) + (('B' - 48 & 0x3F) << 6) + ((48 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('1' - 48 & 0x3F) << 24)),errmsg("no such savepoint")) : ((void )0);
      break; 
    }
    case TBLOCK_SUBINPROGRESS:
{
    }
    case TBLOCK_SUBABORT:
/*
			 * There is at least one savepoint, so proceed.
			 */
    break; 
    case TBLOCK_DEFAULT:
{
    }
    case TBLOCK_STARTED:
{
    }
    case TBLOCK_BEGIN:
{
    }
    case TBLOCK_SUBBEGIN:
{
    }
    case TBLOCK_END:
{
    }
    case TBLOCK_SUBRELEASE:
{
    }
    case TBLOCK_SUBCOMMIT:
{
    }
    case TBLOCK_ABORT_END:
{
    }
    case TBLOCK_SUBABORT_END:
{
    }
    case TBLOCK_ABORT_PENDING:
{
    }
    case TBLOCK_SUBABORT_PENDING:
{
    }
    case TBLOCK_SUBRESTART:
{
    }
    case TBLOCK_SUBABORT_RESTART:
{
    }
    case TBLOCK_PREPARE:
{
/* These cases are invalid. */
      (elog_start("xact.c",3673,__func__) , elog_finish(21,"RollbackToSavepoint: unexpected state %s",BlockStateAsString(s -> blockState)));
      break; 
    }
  }
  for (cell = list_head(options); cell != ((void *)0); cell = cell -> next) {
    DefElem *elem = cell -> data . ptr_value;
    if (strcmp((elem -> defname),"savepoint_name") == 0) {
      name = ((Value *)(elem -> arg)) -> val . str;
    }
  }
  ;
  for (target = s; ((const void *)target) != ((void *)0); target = target -> parent) {
    if (((const void *)(target -> name)) != ((void *)0) && strcmp((target -> name),name) == 0) {
      break; 
    }
  }
  if (!(((const void *)target) != ((void *)0))) {
    errstart(20,"xact.c",3697,__func__,((void *)0))?errfinish(errcode(('3' - 48 & 0x3F) + (('B' - 48 & 0x3F) << 6) + ((48 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('1' - 48 & 0x3F) << 24)),errmsg("no such savepoint")) : ((void )0);
  }
/* disallow crossing savepoint level boundaries */
  if (target -> savepointLevel != s -> savepointLevel) {
    errstart(20,"xact.c",3703,__func__,((void *)0))?errfinish(errcode(('3' - 48 & 0x3F) + (('B' - 48 & 0x3F) << 6) + ((48 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('1' - 48 & 0x3F) << 24)),errmsg("no such savepoint")) : ((void )0);
  }
/*
	 * Mark "abort pending" all subtransactions up to the target
	 * subtransaction.	The actual aborts will happen when control gets to
	 * CommitTransactionCommand.
	 */
  xact = CurrentTransactionState;
  for (; ; ) {
    if (xact == target) {
      break; 
    }
    if ((xact -> blockState) == TBLOCK_SUBINPROGRESS) {
      xact -> blockState = TBLOCK_SUBABORT_PENDING;
    }
    else {
      if ((xact -> blockState) == TBLOCK_SUBABORT) {
        xact -> blockState = TBLOCK_SUBABORT_END;
      }
      else {
        (elog_start("xact.c",3720,__func__) , elog_finish(21,"RollbackToSavepoint: unexpected state %s",BlockStateAsString(xact -> blockState)));
      }
    }
    xact = xact -> parent;
    ;
  }
/* And mark the target as "restart pending" */
  if ((xact -> blockState) == TBLOCK_SUBINPROGRESS) {
    xact -> blockState = TBLOCK_SUBRESTART;
  }
  else {
    if ((xact -> blockState) == TBLOCK_SUBABORT) {
      xact -> blockState = TBLOCK_SUBABORT_RESTART;
    }
    else {
      (elog_start("xact.c",3732,__func__) , elog_finish(21,"RollbackToSavepoint: unexpected state %s",BlockStateAsString(xact -> blockState)));
    }
  }
}
/*
 * BeginInternalSubTransaction
 *		This is the same as DefineSavepoint except it allows TBLOCK_STARTED,
 *		TBLOCK_END, and TBLOCK_PREPARE states, and therefore it can safely be
 *		used in functions that might be called when not inside a BEGIN block
 *		or when running deferred triggers at COMMIT/PREPARE time.  Also, it
 *		automatically does CommitTransactionCommand/StartTransactionCommand
 *		instead of expecting the caller to do it.
 */

void BeginInternalSubTransaction(char *name)
{
  TransactionState s = CurrentTransactionState;
  switch(s -> blockState){
    case TBLOCK_STARTED:
{
    }
    case TBLOCK_INPROGRESS:
{
    }
    case TBLOCK_END:
{
    }
    case TBLOCK_PREPARE:
{
    }
    case TBLOCK_SUBINPROGRESS:
{
/* Normal subtransaction start */
      PushTransaction();
/* changed by push */
      s = CurrentTransactionState;
/*
			 * Savepoint names, like the TransactionState block itself, live
			 * in TopTransactionContext.
			 */
      if (name) {
        s -> name = MemoryContextStrdup(TopTransactionContext,name);
      }
      break; 
    }
    case TBLOCK_DEFAULT:
{
    }
    case TBLOCK_BEGIN:
{
    }
    case TBLOCK_SUBBEGIN:
{
    }
    case TBLOCK_SUBRELEASE:
{
    }
    case TBLOCK_SUBCOMMIT:
{
    }
    case TBLOCK_ABORT:
{
    }
    case TBLOCK_SUBABORT:
{
    }
    case TBLOCK_ABORT_END:
{
    }
    case TBLOCK_SUBABORT_END:
{
    }
    case TBLOCK_ABORT_PENDING:
{
    }
    case TBLOCK_SUBABORT_PENDING:
{
    }
    case TBLOCK_SUBRESTART:
{
    }
    case TBLOCK_SUBABORT_RESTART:
{
/* These cases are invalid. */
      (elog_start("xact.c",3783,__func__) , elog_finish(21,"BeginInternalSubTransaction: unexpected state %s",BlockStateAsString(s -> blockState)));
      break; 
    }
  }
  CommitTransactionCommand();
  StartTransactionCommand();
}
/*
 * ReleaseCurrentSubTransaction
 *
 * RELEASE (ie, commit) the innermost subtransaction, regardless of its
 * savepoint name (if any).
 * NB: do NOT use CommitTransactionCommand/StartTransactionCommand with this.
 */

void ReleaseCurrentSubTransaction()
{
  TransactionState s = CurrentTransactionState;
  if ((s -> blockState) != TBLOCK_SUBINPROGRESS) {
    (elog_start("xact.c",3805,__func__) , elog_finish(20,"ReleaseCurrentSubTransaction: unexpected state %s",BlockStateAsString(s -> blockState)));
  }
  ;
  MemoryContextSwitchTo(CurTransactionContext);
  CommitSubTransaction();
/* changed by pop */
  s = CurrentTransactionState;
  ;
}
/*
 * RollbackAndReleaseCurrentSubTransaction
 *
 * ROLLBACK and RELEASE (ie, abort) the innermost subtransaction, regardless
 * of its savepoint name (if any).
 * NB: do NOT use CommitTransactionCommand/StartTransactionCommand with this.
 */

void RollbackAndReleaseCurrentSubTransaction()
{
  TransactionState s = CurrentTransactionState;
  switch(s -> blockState){
    case TBLOCK_SUBINPROGRESS:
{
    }
    case TBLOCK_SUBABORT:
/* Must be in a subtransaction */
    break; 
    case TBLOCK_DEFAULT:
{
    }
    case TBLOCK_STARTED:
{
    }
    case TBLOCK_BEGIN:
{
    }
    case TBLOCK_SUBBEGIN:
{
    }
    case TBLOCK_INPROGRESS:
{
    }
    case TBLOCK_END:
{
    }
    case TBLOCK_SUBRELEASE:
{
    }
    case TBLOCK_SUBCOMMIT:
{
    }
    case TBLOCK_ABORT:
{
    }
    case TBLOCK_ABORT_END:
{
    }
    case TBLOCK_SUBABORT_END:
{
    }
    case TBLOCK_ABORT_PENDING:
{
    }
    case TBLOCK_SUBABORT_PENDING:
{
    }
    case TBLOCK_SUBRESTART:
{
    }
    case TBLOCK_SUBABORT_RESTART:
{
    }
    case TBLOCK_PREPARE:
{
/* These cases are invalid. */
      (elog_start("xact.c",3850,__func__) , elog_finish(21,"RollbackAndReleaseCurrentSubTransaction: unexpected state %s",BlockStateAsString(s -> blockState)));
      break; 
    }
  }
/*
	 * Abort the current subtransaction, if needed.
	 */
  if ((s -> blockState) == TBLOCK_SUBINPROGRESS) {
    AbortSubTransaction();
  }
/* And clean it up, too */
  CleanupSubTransaction();
/* changed by pop */
  s = CurrentTransactionState;
  ;
}
/*
 *	AbortOutOfAnyTransaction
 *
 *	This routine is provided for error recovery purposes.  It aborts any
 *	active transaction or transaction block, leaving the system in a known
 *	idle state.
 */

void AbortOutOfAnyTransaction()
{
  TransactionState s = CurrentTransactionState;
/*
	 * Get out of any transaction or nested transaction
	 */
  do {
    switch(s -> blockState){
      case TBLOCK_DEFAULT:
{
        if ((s -> state) == TRANS_DEFAULT) {
/* Not in a transaction, do nothing */
        }
        else {
/*
					 * We can get here after an error during transaction start
					 * (state will be TRANS_START).  Need to clean up the
					 * incompletely started transaction.  First, adjust the
					 * low-level state to suppress warning message from
					 * AbortTransaction.
					 */
          if ((s -> state) == TRANS_START) {
            s -> state = TRANS_INPROGRESS;
          }
          AbortTransaction();
          CleanupTransaction();
        }
        break; 
      }
      case TBLOCK_STARTED:
{
      }
      case TBLOCK_BEGIN:
{
      }
      case TBLOCK_INPROGRESS:
{
      }
      case TBLOCK_END:
{
      }
      case TBLOCK_ABORT_PENDING:
{
      }
      case TBLOCK_PREPARE:
{
/* In a transaction, so clean up */
        AbortTransaction();
        CleanupTransaction();
        s -> blockState = TBLOCK_DEFAULT;
        break; 
      }
      case TBLOCK_ABORT:
{
      }
      case TBLOCK_ABORT_END:
{
/* AbortTransaction already done, still need Cleanup */
        CleanupTransaction();
        s -> blockState = TBLOCK_DEFAULT;
        break; 
      }
      case TBLOCK_SUBBEGIN:
{
      }
      case TBLOCK_SUBINPROGRESS:
{
      }
      case TBLOCK_SUBRELEASE:
{
      }
      case TBLOCK_SUBCOMMIT:
{
      }
      case TBLOCK_SUBABORT_PENDING:
{
      }
      case TBLOCK_SUBRESTART:
{
/*
				 * In a subtransaction, so clean it up and abort parent too
				 */
        AbortSubTransaction();
        CleanupSubTransaction();
/* changed by pop */
        s = CurrentTransactionState;
        break; 
      }
      case TBLOCK_SUBABORT:
{
      }
      case TBLOCK_SUBABORT_END:
{
      }
      case TBLOCK_SUBABORT_RESTART:
{
/* As above, but AbortSubTransaction already done */
        CleanupSubTransaction();
/* changed by pop */
        s = CurrentTransactionState;
        break; 
      }
    }
  }while ((s -> blockState) != TBLOCK_DEFAULT);
/* Should be out of all subxacts now */
  ;
}
/*
 * IsTransactionBlock --- are we within a transaction block?
 */

bool IsTransactionBlock()
{
  TransactionState s = CurrentTransactionState;
  if ((s -> blockState) == TBLOCK_DEFAULT || (s -> blockState) == TBLOCK_STARTED) {
    return (bool )0;
  }
  return (bool )1;
}
/*
 * IsTransactionOrTransactionBlock --- are we within either a transaction
 * or a transaction block?	(The backend is only really "idle" when this
 * returns false.)
 *
 * This should match up with IsTransactionBlock and IsTransactionState.
 */

bool IsTransactionOrTransactionBlock()
{
  TransactionState s = CurrentTransactionState;
  if ((s -> blockState) == TBLOCK_DEFAULT) {
    return (bool )0;
  }
  return (bool )1;
}
/*
 * TransactionBlockStatusCode - return status code to send in ReadyForQuery
 */

char TransactionBlockStatusCode()
{
  TransactionState s = CurrentTransactionState;
  switch(s -> blockState){
    case TBLOCK_DEFAULT:
{
    }
    case TBLOCK_STARTED:
/* idle --- not in transaction */
    return 'I';
    case TBLOCK_BEGIN:
{
    }
    case TBLOCK_SUBBEGIN:
{
    }
    case TBLOCK_INPROGRESS:
{
    }
    case TBLOCK_SUBINPROGRESS:
{
    }
    case TBLOCK_END:
{
    }
    case TBLOCK_SUBRELEASE:
{
    }
    case TBLOCK_SUBCOMMIT:
{
    }
    case TBLOCK_PREPARE:
/* in transaction */
    return 'T';
    case TBLOCK_ABORT:
{
    }
    case TBLOCK_SUBABORT:
{
    }
    case TBLOCK_ABORT_END:
{
    }
    case TBLOCK_SUBABORT_END:
{
    }
    case TBLOCK_ABORT_PENDING:
{
    }
    case TBLOCK_SUBABORT_PENDING:
{
    }
    case TBLOCK_SUBRESTART:
{
    }
    case TBLOCK_SUBABORT_RESTART:
/* in failed transaction */
    return 'E';
  }
/* should never get here */
  (elog_start("xact.c",4021,__func__) , elog_finish(21,"invalid transaction block state: %s",BlockStateAsString(s -> blockState)));
/* keep compiler quiet */
  return 0;
}
/*
 * IsSubTransaction
 */

bool IsSubTransaction()
{
  TransactionState s = CurrentTransactionState;
  if (s -> nestingLevel >= 2) {
    return (bool )1;
  }
  return (bool )0;
}
/*
 * StartSubTransaction
 *
 * If you're wondering why this is separate from PushTransaction: it's because
 * we can't conveniently do this stuff right inside DefineSavepoint.  The
 * SAVEPOINT utility command will be executed inside a Portal, and if we
 * muck with CurrentMemoryContext or CurrentResourceOwner then exit from
 * the Portal will undo those settings.  So we make DefineSavepoint just
 * push a dummy transaction block, and when control returns to the main
 * idle loop, CommitTransactionCommand will be called, and we'll come here
 * to finish starting the subtransaction.
 */

static void StartSubTransaction()
{
  TransactionState s = CurrentTransactionState;
  if ((s -> state) != TRANS_DEFAULT) {
    (elog_start("xact.c",4058,__func__) , elog_finish(19,"StartSubTransaction while in %s state",TransStateAsString(s -> state)));
  }
  s -> state = TRANS_START;
/*
	 * Initialize subsystems for new subtransaction
	 *
	 * must initialize resource-management stuff first
	 */
  AtSubStart_Memory();
  AtSubStart_ResourceOwner();
  AtSubStart_Inval();
  AtSubStart_Notify();
  AfterTriggerBeginSubXact();
  s -> state = TRANS_INPROGRESS;
/*
	 * Call start-of-subxact callbacks
	 */
  CallSubXactCallbacks(SUBXACT_EVENT_START_SUB,s -> subTransactionId,s -> parent -> subTransactionId);
  ShowTransactionState("StartSubTransaction");
}
/*
 * CommitSubTransaction
 *
 *	The caller has to make sure to always reassign CurrentTransactionState
 *	if it has a local pointer to it after calling this function.
 */

static void CommitSubTransaction()
{
  TransactionState s = CurrentTransactionState;
  ShowTransactionState("CommitSubTransaction");
  if ((s -> state) != TRANS_INPROGRESS) {
    (elog_start("xact.c",4099,__func__) , elog_finish(19,"CommitSubTransaction while in %s state",TransStateAsString(s -> state)));
  }
/* Pre-commit processing goes here -- nothing to do at the moment */
  s -> state = TRANS_COMMIT;
/* Must CCI to ensure commands of subtransaction are seen as done */
  CommandCounterIncrement();
/*
	 * Prior to 8.4 we marked subcommit in clog at this point.	We now only
	 * perform that step, if required, as part of the atomic update of the
	 * whole transaction tree at top level commit or abort.
	 */
/* Post-commit cleanup */
  if (s -> transactionId != ((TransactionId )0)) {
    AtSubCommit_childXids();
  }
  AfterTriggerEndSubXact(((bool )1));
  AtSubCommit_Portals(s -> subTransactionId,s -> parent -> subTransactionId,s -> parent -> curTransactionOwner);
  AtEOSubXact_LargeObject(((bool )1),s -> subTransactionId,s -> parent -> subTransactionId);
  AtSubCommit_Notify();
  CallSubXactCallbacks(SUBXACT_EVENT_COMMIT_SUB,s -> subTransactionId,s -> parent -> subTransactionId);
  ResourceOwnerRelease(s -> curTransactionOwner,RESOURCE_RELEASE_BEFORE_LOCKS,((bool )1),((bool )0));
  AtEOSubXact_RelationCache(((bool )1),s -> subTransactionId,s -> parent -> subTransactionId);
  AtEOSubXact_Inval(((bool )1));
  AtSubCommit_smgr();
/*
	 * The only lock we actually release here is the subtransaction XID lock.
	 */
  CurrentResourceOwner = s -> curTransactionOwner;
  if (s -> transactionId != ((TransactionId )0)) {
    XactLockTableDelete(s -> transactionId);
  }
/*
	 * Other locks should get transferred to their parent resource owner.
	 */
  ResourceOwnerRelease(s -> curTransactionOwner,RESOURCE_RELEASE_LOCKS,((bool )1),((bool )0));
  ResourceOwnerRelease(s -> curTransactionOwner,RESOURCE_RELEASE_AFTER_LOCKS,((bool )1),((bool )0));
  AtEOXact_GUC(((bool )1),s -> gucNestLevel);
  AtEOSubXact_SPI(((bool )1),s -> subTransactionId);
  AtEOSubXact_on_commit_actions(((bool )1),s -> subTransactionId,s -> parent -> subTransactionId);
  AtEOSubXact_Namespace(((bool )1),s -> subTransactionId,s -> parent -> subTransactionId);
  AtEOSubXact_Files(((bool )1),s -> subTransactionId,s -> parent -> subTransactionId);
  AtEOSubXact_HashTables(((bool )1),s -> nestingLevel);
  AtEOSubXact_PgStat(((bool )1),s -> nestingLevel);
  AtSubCommit_Snapshot(s -> nestingLevel);
/*
	 * We need to restore the upper transaction's read-only state, in case the
	 * upper is read-write while the child is read-only; GUC will incorrectly
	 * think it should leave the child state in place.
	 */
  XactReadOnly = s -> prevXactReadOnly;
  CurrentResourceOwner = s -> parent -> curTransactionOwner;
  CurTransactionResourceOwner = s -> parent -> curTransactionOwner;
  ResourceOwnerDelete(s -> curTransactionOwner);
  s -> curTransactionOwner = ((void *)0);
  AtSubCommit_Memory();
  s -> state = TRANS_DEFAULT;
  PopTransaction();
}
/*
 * AbortSubTransaction
 */

static void AbortSubTransaction()
{
  TransactionState s = CurrentTransactionState;
/* Prevent cancel/die interrupt while cleaning up */
  InterruptHoldoffCount++;
/* Make sure we have a valid memory context and resource owner */
  AtSubAbort_Memory();
  AtSubAbort_ResourceOwner();
/*
	 * Release any LW locks we might be holding as quickly as possible.
	 * (Regular locks, however, must be held till we finish aborting.)
	 * Releasing LW locks is critical since we might try to grab them again
	 * while cleaning up!
	 *
	 * FIXME This may be incorrect --- Are there some locks we should keep?
	 * Buffer locks, for example?  I don't think so but I'm not sure.
	 */
  LWLockReleaseAll();
  AbortBufferIO();
  UnlockBuffers();
  LockErrorCleanup();
/*
	 * check the current transaction state
	 */
  ShowTransactionState("AbortSubTransaction");
  if ((s -> state) != TRANS_INPROGRESS) {
    (elog_start("xact.c",4222,__func__) , elog_finish(19,"AbortSubTransaction while in %s state",TransStateAsString(s -> state)));
  }
  s -> state = TRANS_ABORT;
/*
	 * Reset user ID which might have been changed transiently.  (See notes in
	 * AbortTransaction.)
	 */
  SetUserIdAndSecContext(s -> prevUser,s -> prevSecContext);
/*
	 * We can skip all this stuff if the subxact failed before creating a
	 * ResourceOwner...
	 */
  if (s -> curTransactionOwner) {
    AfterTriggerEndSubXact(((bool )0));
    AtSubAbort_Portals(s -> subTransactionId,s -> parent -> subTransactionId,s -> parent -> curTransactionOwner);
    AtEOSubXact_LargeObject(((bool )0),s -> subTransactionId,s -> parent -> subTransactionId);
    AtSubAbort_Notify();
/* Advertise the fact that we aborted in pg_clog. */
    (void )(RecordTransactionAbort(((bool )1)));
/* Post-abort cleanup */
    if (s -> transactionId != ((TransactionId )0)) {
      AtSubAbort_childXids();
    }
    CallSubXactCallbacks(SUBXACT_EVENT_ABORT_SUB,s -> subTransactionId,s -> parent -> subTransactionId);
    ResourceOwnerRelease(s -> curTransactionOwner,RESOURCE_RELEASE_BEFORE_LOCKS,((bool )0),((bool )0));
    AtEOSubXact_RelationCache(((bool )0),s -> subTransactionId,s -> parent -> subTransactionId);
    AtEOSubXact_Inval(((bool )0));
    AtSubAbort_smgr();
    ResourceOwnerRelease(s -> curTransactionOwner,RESOURCE_RELEASE_LOCKS,((bool )0),((bool )0));
    ResourceOwnerRelease(s -> curTransactionOwner,RESOURCE_RELEASE_AFTER_LOCKS,((bool )0),((bool )0));
    AtEOXact_GUC(((bool )0),s -> gucNestLevel);
    AtEOSubXact_SPI(((bool )0),s -> subTransactionId);
    AtEOSubXact_on_commit_actions(((bool )0),s -> subTransactionId,s -> parent -> subTransactionId);
    AtEOSubXact_Namespace(((bool )0),s -> subTransactionId,s -> parent -> subTransactionId);
    AtEOSubXact_Files(((bool )0),s -> subTransactionId,s -> parent -> subTransactionId);
    AtEOSubXact_HashTables(((bool )0),s -> nestingLevel);
    AtEOSubXact_PgStat(((bool )0),s -> nestingLevel);
    AtSubAbort_Snapshot(s -> nestingLevel);
  }
/*
	 * Restore the upper transaction's read-only state, too.  This should be
	 * redundant with GUC's cleanup but we may as well do it for consistency
	 * with the commit case.
	 */
  XactReadOnly = s -> prevXactReadOnly;
  do {
    ;
    InterruptHoldoffCount--;
  }while (0);
}
/*
 * CleanupSubTransaction
 *
 *	The caller has to make sure to always reassign CurrentTransactionState
 *	if it has a local pointer to it after calling this function.
 */

static void CleanupSubTransaction()
{
  TransactionState s = CurrentTransactionState;
  ShowTransactionState("CleanupSubTransaction");
  if ((s -> state) != TRANS_ABORT) {
    (elog_start("xact.c",4308,__func__) , elog_finish(19,"CleanupSubTransaction while in %s state",TransStateAsString(s -> state)));
  }
  AtSubCleanup_Portals(s -> subTransactionId);
  CurrentResourceOwner = s -> parent -> curTransactionOwner;
  CurTransactionResourceOwner = s -> parent -> curTransactionOwner;
  if (s -> curTransactionOwner) {
    ResourceOwnerDelete(s -> curTransactionOwner);
  }
  s -> curTransactionOwner = ((void *)0);
  AtSubCleanup_Memory();
  s -> state = TRANS_DEFAULT;
  PopTransaction();
}
/*
 * PushTransaction
 *		Create transaction state stack entry for a subtransaction
 *
 *	The caller has to make sure to always reassign CurrentTransactionState
 *	if it has a local pointer to it after calling this function.
 */

static void PushTransaction()
{
  TransactionState p = CurrentTransactionState;
  TransactionState s;
/*
	 * We keep subtransaction state nodes in TopTransactionContext.
	 */
  s = ((TransactionState )(MemoryContextAllocZero(TopTransactionContext,sizeof(TransactionStateData ))));
/*
	 * Assign a subtransaction ID, watching out for counter wraparound.
	 */
  currentSubTransactionId += 1;
  if (currentSubTransactionId == ((SubTransactionId )0)) {
    currentSubTransactionId -= 1;
    pfree(s);
    errstart(20,"xact.c",4356,__func__,((void *)0))?errfinish(errcode(('5' - 48 & 0x3F) + (('4' - 48 & 0x3F) << 6) + ((48 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + ((48 - 48 & 0x3F) << 24)),errmsg("cannot have more than 2^32-1 subtransactions in a transaction")) : ((void )0);
  }
/*
	 * We can now stack a minimally valid subtransaction without fear of
	 * failure.
	 */
/* until assigned */
  s -> transactionId = ((TransactionId )0);
  s -> subTransactionId = currentSubTransactionId;
  s -> parent = p;
  s -> nestingLevel = p -> nestingLevel + 1;
  s -> gucNestLevel = NewGUCNestLevel();
  s -> savepointLevel = p -> savepointLevel;
  s -> state = TRANS_DEFAULT;
  s -> blockState = TBLOCK_SUBBEGIN;
  GetUserIdAndSecContext(&s -> prevUser,&s -> prevSecContext);
  s -> prevXactReadOnly = XactReadOnly;
  CurrentTransactionState = s;
/*
	 * AbortSubTransaction and CleanupSubTransaction have to be able to cope
	 * with the subtransaction from here on out; in particular they should not
	 * assume that it necessarily has a transaction context, resource owner,
	 * or XID.
	 */
}
/*
 * PopTransaction
 *		Pop back to parent transaction state
 *
 *	The caller has to make sure to always reassign CurrentTransactionState
 *	if it has a local pointer to it after calling this function.
 */

static void PopTransaction()
{
  TransactionState s = CurrentTransactionState;
  if ((s -> state) != TRANS_DEFAULT) {
    (elog_start("xact.c",4397,__func__) , elog_finish(19,"PopTransaction while in %s state",TransStateAsString(s -> state)));
  }
  if (s -> parent == ((void *)0)) {
    (elog_start("xact.c",4401,__func__) , elog_finish(21,"PopTransaction with no parent"));
  }
  CurrentTransactionState = s -> parent;
/* Let's just make sure CurTransactionContext is good */
  CurTransactionContext = s -> parent -> curTransactionContext;
  MemoryContextSwitchTo(CurTransactionContext);
/* Ditto for ResourceOwner links */
  CurTransactionResourceOwner = s -> parent -> curTransactionOwner;
  CurrentResourceOwner = s -> parent -> curTransactionOwner;
/* Free the old child structure */
  if (s -> name) {
    pfree((s -> name));
  }
  pfree(s);
}
/*
 * ShowTransactionState
 *		Debug support
 */

static void ShowTransactionState(const char *str)
{
/* skip work if message will definitely not be printed */
  if (log_min_messages <= 12 || client_min_messages <= 12) {
    (elog_start("xact.c",4429,__func__) , elog_finish(12,"%s",str));
    ShowTransactionStateRec(CurrentTransactionState);
  }
}
/*
 * ShowTransactionStateRec
 *		Recursive subroutine for ShowTransactionState
 */

static void ShowTransactionStateRec(TransactionState s)
{
  StringInfoData buf;
  initStringInfo(&buf);
  if (s -> nChildXids > 0) {
    int i;
    appendStringInfo(&buf,"%u",s -> childXids[0]);
    for (i = 1; i < s -> nChildXids; i++) 
      appendStringInfo(&buf," %u",s -> childXids[i]);
  }
  if (s -> parent) {
    ShowTransactionStateRec(s -> parent);
  }
/* use ereport to suppress computation if msg will not be printed */
  errstart(12,"xact.c",4467,__func__,((void *)0))?errfinish(errmsg_internal("name: %s; blockState: %13s; state: %7s, xid/subid/cid: %u/%u/%u%s, nestlvl: %d, children: %s",(((const void *)(s -> name)) != ((void *)0)?s -> name : "unnamed"),BlockStateAsString(s -> blockState),TransStateAsString(s -> state),((unsigned int )(s -> transactionId)),((unsigned int )(s -> subTransactionId)),((unsigned int )currentCommandId),(currentCommandIdUsed?" (used)" : ""),s -> nestingLevel,buf . data)) : ((void )0);
  pfree(buf . data);
}
/*
 * BlockStateAsString
 *		Debug support
 */

static const char *BlockStateAsString(TBlockState blockState)
{
  switch(blockState){
    case TBLOCK_DEFAULT:
    return "DEFAULT";
    case TBLOCK_STARTED:
    return "STARTED";
    case TBLOCK_BEGIN:
    return "BEGIN";
    case TBLOCK_INPROGRESS:
    return "INPROGRESS";
    case TBLOCK_END:
    return "END";
    case TBLOCK_ABORT:
    return "ABORT";
    case TBLOCK_ABORT_END:
    return "ABORT END";
    case TBLOCK_ABORT_PENDING:
    return "ABORT PEND";
    case TBLOCK_PREPARE:
    return "PREPARE";
    case TBLOCK_SUBBEGIN:
    return "SUB BEGIN";
    case TBLOCK_SUBINPROGRESS:
    return "SUB INPROGRS";
    case TBLOCK_SUBRELEASE:
    return "SUB RELEASE";
    case TBLOCK_SUBCOMMIT:
    return "SUB COMMIT";
    case TBLOCK_SUBABORT:
    return "SUB ABORT";
    case TBLOCK_SUBABORT_END:
    return "SUB ABORT END";
    case TBLOCK_SUBABORT_PENDING:
    return "SUB ABRT PEND";
    case TBLOCK_SUBRESTART:
    return "SUB RESTART";
    case TBLOCK_SUBABORT_RESTART:
    return "SUB AB RESTRT";
  }
  return "UNRECOGNIZED";
}
/*
 * TransStateAsString
 *		Debug support
 */

static const char *TransStateAsString(TransState state)
{
  switch(state){
    case TRANS_DEFAULT:
    return "DEFAULT";
    case TRANS_START:
    return "START";
    case TRANS_INPROGRESS:
    return "INPROGR";
    case TRANS_COMMIT:
    return "COMMIT";
    case TRANS_ABORT:
    return "ABORT";
    case TRANS_PREPARE:
    return "PREPARE";
  }
  return "UNRECOGNIZED";
}
/*
 * xactGetCommittedChildren
 *
 * Gets the list of committed children of the current transaction.	The return
 * value is the number of child transactions.  *ptr is set to point to an
 * array of TransactionIds.  The array is allocated in TopTransactionContext;
 * the caller should *not* pfree() it (this is a change from pre-8.4 code!).
 * If there are no subxacts, *ptr is set to NULL.
 */

int xactGetCommittedChildren(TransactionId **ptr)
{
  TransactionState s = CurrentTransactionState;
  if (s -> nChildXids == 0) {
     *ptr = ((void *)0);
  }
  else {
     *ptr = s -> childXids;
  }
  return s -> nChildXids;
}
/*
 *	XLOG support routines
 */
/*
 * Before 9.0 this was a fairly short function, but now it performs many
 * actions for which the order of execution is critical.
 */

static void xact_redo_commit_internal(TransactionId xid,XLogRecPtr lsn,TransactionId *sub_xids,int nsubxacts,SharedInvalidationMessage *inval_msgs,int nmsgs,RelFileNode *xnodes,int nrels,Oid dbId,Oid tsId,uint32 xinfo)
{
  TransactionId max_xid;
  int i;
  max_xid = TransactionIdLatest(xid,nsubxacts,sub_xids);
/*
	 * Make sure nextXid is beyond any XID mentioned in the record.
	 *
	 * We don't expect anyone else to modify nextXid, hence we don't need to
	 * hold a lock while checking this. We still acquire the lock to modify
	 * it, though.
	 */
  if (TransactionIdFollowsOrEquals(max_xid,ShmemVariableCache -> nextXid)) {
    LWLockAcquire(XidGenLock,LW_EXCLUSIVE);
    ShmemVariableCache -> nextXid = max_xid;
    do {
      ShmemVariableCache -> nextXid++;
      if (ShmemVariableCache -> nextXid < ((TransactionId )3)) {
        ShmemVariableCache -> nextXid = ((TransactionId )3);
      }
    }while (0);
    LWLockRelease(XidGenLock);
  }
  if (standbyState == STANDBY_DISABLED) {
/*
		 * Mark the transaction committed in pg_clog.
		 */
    TransactionIdCommitTree(xid,nsubxacts,sub_xids);
  }
  else {
/*
		 * If a transaction completion record arrives that has as-yet
		 * unobserved subtransactions then this will not have been fully
		 * handled by the call to RecordKnownAssignedTransactionIds() in the
		 * main recovery loop in xlog.c. So we need to do bookkeeping again to
		 * cover that case. This is confusing and it is easy to think this
		 * call is irrelevant, which has happened three times in development
		 * already. Leave it in.
		 */
    RecordKnownAssignedTransactionIds(max_xid);
/*
		 * Mark the transaction committed in pg_clog. We use async commit
		 * protocol during recovery to provide information on database
		 * consistency for when users try to set hint bits. It is important
		 * that we do not set hint bits until the minRecoveryPoint is past
		 * this commit record. This ensures that if we crash we don't see hint
		 * bits set on changes made by transactions that haven't yet
		 * recovered. It's unlikely but it's good to be safe.
		 */
    TransactionIdAsyncCommitTree(xid,nsubxacts,sub_xids,lsn);
/*
		 * We must mark clog before we update the ProcArray.
		 */
    ExpireTreeKnownAssignedTransactionIds(xid,nsubxacts,sub_xids,max_xid);
/*
		 * Send any cache invalidations attached to the commit. We must
		 * maintain the same order of invalidation then release locks as
		 * occurs in CommitTransaction().
		 */
    ProcessCommittedInvalidationMessages(inval_msgs,nmsgs,(xinfo & 0x01),dbId,tsId);
/*
		 * Release locks, if any. We do this for both two phase and normal one
		 * phase transactions. In effect we are ignoring the prepare phase and
		 * just going straight to lock release. At commit we release all locks
		 * via their top-level xid only, so no need to provide subxact list,
		 * which will save time when replaying commits.
		 */
    StandbyReleaseLockTree(xid,0,((void *)0));
  }
/* Make sure files supposed to be dropped are dropped */
  if (nrels > 0) {
/*
		 * First update minimum recovery point to cover this WAL record. Once
		 * a relation is deleted, there's no going back. The buffer manager
		 * enforces the WAL-first rule for normal updates to relation files,
		 * so that the minimum recovery point is always updated before the
		 * corresponding change in the data file is flushed to disk, but we
		 * have to do the same here since we're bypassing the buffer manager.
		 *
		 * Doing this before deleting the files means that if a deletion fails
		 * for some reason, you cannot start up the system even after restart,
		 * until you fix the underlying situation so that the deletion will
		 * succeed. Alternatively, we could update the minimum recovery point
		 * after deletion, but that would leave a small window where the
		 * WAL-first rule would be violated.
		 */
    XLogFlush(lsn);
    for (i = 0; i < nrels; i++) {
      SMgrRelation srel = smgropen(xnodes[i],- 1);
      ForkNumber fork;
      for (fork = MAIN_FORKNUM; fork <= INIT_FORKNUM; fork++) 
        XLogDropRelation(xnodes[i],fork);
      smgrdounlink(srel,((bool )1));
      smgrclose(srel);
    }
  }
/*
	 * We issue an XLogFlush() for the same reason we emit ForceSyncCommit()
	 * in normal operation. For example, in CREATE DATABASE, we copy all files
	 * from the template database, and then commit the transaction. If we
	 * crash after all the files have been copied but before the commit, you
	 * have files in the data directory without an entry in pg_database. To
	 * minimize the window
	 * for that, we use ForceSyncCommit() to rush the commit record to disk as
	 * quick as possible. We have the same window during recovery, and forcing
	 * an XLogFlush() (which updates minRecoveryPoint during recovery) helps
	 * to reduce that problem window, for any user that requested
	 * ForceSyncCommit().
	 */
  if (xinfo & 0x02) {
    XLogFlush(lsn);
  }
}
/*
 * Utility function to call xact_redo_commit_internal after breaking down xlrec
 */

static void xact_redo_commit(xl_xact_commit *xlrec,TransactionId xid,XLogRecPtr lsn)
{
  TransactionId *subxacts;
  SharedInvalidationMessage *inval_msgs;
/* subxid array follows relfilenodes */
  subxacts = ((TransactionId *)(&xlrec -> xnodes[xlrec -> nrels]));
/* invalidation messages array follows subxids */
  inval_msgs = ((SharedInvalidationMessage *)(&subxacts[xlrec -> nsubxacts]));
  xact_redo_commit_internal(xid,lsn,subxacts,xlrec -> nsubxacts,inval_msgs,xlrec -> nmsgs,xlrec -> xnodes,xlrec -> nrels,xlrec -> dbId,xlrec -> tsId,xlrec -> xinfo);
}
/*
 * Utility function to call xact_redo_commit_internal  for compact form of message.
 */

static void xact_redo_commit_compact(xl_xact_commit_compact *xlrec,TransactionId xid,XLogRecPtr lsn)
{
  xact_redo_commit_internal(xid,lsn,xlrec -> subxacts,xlrec -> nsubxacts,((void *)0),0,((void *)0),0,((Oid )0),((Oid )0),0);
/* inval msgs */
/* relfilenodes */
/* dbId */
/* tsId */
/* xinfo */
}
/*
 * Be careful with the order of execution, as with xact_redo_commit().
 * The two functions are similar but differ in key places.
 *
 * Note also that an abort can be for a subtransaction and its children,
 * not just for a top level abort. That means we have to consider
 * topxid != xid, whereas in commit we would find topxid == xid always
 * because subtransaction commit is never WAL logged.
 */

static void xact_redo_abort(xl_xact_abort *xlrec,TransactionId xid)
{
  TransactionId *sub_xids;
  TransactionId max_xid;
  int i;
  sub_xids = ((TransactionId *)(&xlrec -> xnodes[xlrec -> nrels]));
  max_xid = TransactionIdLatest(xid,xlrec -> nsubxacts,sub_xids);
/*
	 * Make sure nextXid is beyond any XID mentioned in the record.
	 *
	 * We don't expect anyone else to modify nextXid, hence we don't need to
	 * hold a lock while checking this. We still acquire the lock to modify
	 * it, though.
	 */
  if (TransactionIdFollowsOrEquals(max_xid,ShmemVariableCache -> nextXid)) {
    LWLockAcquire(XidGenLock,LW_EXCLUSIVE);
    ShmemVariableCache -> nextXid = max_xid;
    do {
      ShmemVariableCache -> nextXid++;
      if (ShmemVariableCache -> nextXid < ((TransactionId )3)) {
        ShmemVariableCache -> nextXid = ((TransactionId )3);
      }
    }while (0);
    LWLockRelease(XidGenLock);
  }
  if (standbyState == STANDBY_DISABLED) {
/* Mark the transaction aborted in pg_clog, no need for async stuff */
    TransactionIdAbortTree(xid,xlrec -> nsubxacts,sub_xids);
  }
  else {
/*
		 * If a transaction completion record arrives that has as-yet
		 * unobserved subtransactions then this will not have been fully
		 * handled by the call to RecordKnownAssignedTransactionIds() in the
		 * main recovery loop in xlog.c. So we need to do bookkeeping again to
		 * cover that case. This is confusing and it is easy to think this
		 * call is irrelevant, which has happened three times in development
		 * already. Leave it in.
		 */
    RecordKnownAssignedTransactionIds(max_xid);
/* Mark the transaction aborted in pg_clog, no need for async stuff */
    TransactionIdAbortTree(xid,xlrec -> nsubxacts,sub_xids);
/*
		 * We must update the ProcArray after we have marked clog.
		 */
    ExpireTreeKnownAssignedTransactionIds(xid,xlrec -> nsubxacts,sub_xids,max_xid);
/*
		 * There are no flat files that need updating, nor invalidation
		 * messages to send or undo.
		 */
/*
		 * Release locks, if any. There are no invalidations to send.
		 */
    StandbyReleaseLockTree(xid,xlrec -> nsubxacts,sub_xids);
  }
/* Make sure files supposed to be dropped are dropped */
  for (i = 0; i < xlrec -> nrels; i++) {
    SMgrRelation srel = smgropen(xlrec -> xnodes[i],- 1);
    ForkNumber fork;
    for (fork = MAIN_FORKNUM; fork <= INIT_FORKNUM; fork++) 
      XLogDropRelation(xlrec -> xnodes[i],fork);
    smgrdounlink(srel,((bool )1));
    smgrclose(srel);
  }
}

void xact_redo(XLogRecPtr lsn,XLogRecord *record)
{
  uint8 info = ((record -> xl_info) & ~0x0F);
/* Backup blocks are not used in xact records */
  ;
  if (info == 0x60) {
    xl_xact_commit_compact *xlrec = (xl_xact_commit_compact *)(((char *)record) + (((intptr_t )(sizeof(XLogRecord ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
    xact_redo_commit_compact(xlrec,record -> xl_xid,lsn);
  }
  else {
    if (info == 0) {
      xl_xact_commit *xlrec = (xl_xact_commit *)(((char *)record) + (((intptr_t )(sizeof(XLogRecord ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
      xact_redo_commit(xlrec,record -> xl_xid,lsn);
    }
    else {
      if (info == 0x20) {
        xl_xact_abort *xlrec = (xl_xact_abort *)(((char *)record) + (((intptr_t )(sizeof(XLogRecord ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
        xact_redo_abort(xlrec,record -> xl_xid);
      }
      else {
        if (info == 0x10) {
/* the record contents are exactly the 2PC file */
          RecreateTwoPhaseFile(record -> xl_xid,(((char *)record) + (((intptr_t )(sizeof(XLogRecord ))) + (8 - 1) & ~((intptr_t )(8 - 1)))),(record -> xl_len));
        }
        else {
          if (info == 0x30) {
            xl_xact_commit_prepared *xlrec = (xl_xact_commit_prepared *)(((char *)record) + (((intptr_t )(sizeof(XLogRecord ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
            xact_redo_commit(&xlrec -> crec,xlrec -> xid,lsn);
            RemoveTwoPhaseFile(xlrec -> xid,((bool )0));
          }
          else {
            if (info == 0x40) {
              xl_xact_abort_prepared *xlrec = (xl_xact_abort_prepared *)(((char *)record) + (((intptr_t )(sizeof(XLogRecord ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
              xact_redo_abort(&xlrec -> arec,xlrec -> xid);
              RemoveTwoPhaseFile(xlrec -> xid,((bool )0));
            }
            else {
              if (info == 0x50) {
                xl_xact_assignment *xlrec = (xl_xact_assignment *)(((char *)record) + (((intptr_t )(sizeof(XLogRecord ))) + (8 - 1) & ~((intptr_t )(8 - 1))));
                if (standbyState >= STANDBY_INITIALIZED) {
                  ProcArrayApplyXidAssignment(xlrec -> xtop,xlrec -> nsubxacts,xlrec -> xsub);
                }
              }
              else {
                (elog_start("xact.c",4888,__func__) , elog_finish(22,"xact_redo: unknown op code %u",info));
              }
            }
          }
        }
      }
    }
  }
}

static void xact_desc_commit(StringInfo buf,xl_xact_commit *xlrec)
{
  int i;
  TransactionId *subxacts;
  subxacts = ((TransactionId *)(&xlrec -> xnodes[xlrec -> nrels]));
  appendStringInfoString(buf,timestamptz_to_str(xlrec -> xact_time));
  if (xlrec -> nrels > 0) {
    appendStringInfo(buf,"; rels:");
    for (i = 0; i < xlrec -> nrels; i++) {
      char *path = relpathbackend(xlrec -> xnodes[i],- 1,MAIN_FORKNUM);
      appendStringInfo(buf," %s",path);
      pfree(path);
    }
  }
  if (xlrec -> nsubxacts > 0) {
    appendStringInfo(buf,"; subxacts:");
    for (i = 0; i < xlrec -> nsubxacts; i++) 
      appendStringInfo(buf," %u",subxacts[i]);
  }
  if (xlrec -> nmsgs > 0) {
    SharedInvalidationMessage *msgs;
    msgs = ((SharedInvalidationMessage *)(&subxacts[xlrec -> nsubxacts]));
    if (xlrec -> xinfo & 0x01) {
      appendStringInfo(buf,"; relcache init file inval dbid %u tsid %u",xlrec -> dbId,xlrec -> tsId);
    }
    appendStringInfo(buf,"; inval msgs:");
    for (i = 0; i < xlrec -> nmsgs; i++) {
      SharedInvalidationMessage *msg = &msgs[i];
      if ((msg -> id) >= 0) {
        appendStringInfo(buf," catcache %d",(msg -> id));
      }
      else {
        if ((msg -> id) == - 1) {
          appendStringInfo(buf," catalog %u",msg -> cat . catId);
        }
        else {
          if ((msg -> id) == - 2) {
            appendStringInfo(buf," relcache %u",msg -> rc . relId);
          }
          else {
/* remaining cases not expected, but print something anyway */
            if ((msg -> id) == - 3) {
              appendStringInfo(buf," smgr");
            }
            else {
              if ((msg -> id) == - 4) {
                appendStringInfo(buf," relmap");
              }
              else {
                appendStringInfo(buf," unknown id %d",(msg -> id));
              }
            }
          }
        }
      }
    }
  }
}

static void xact_desc_commit_compact(StringInfo buf,xl_xact_commit_compact *xlrec)
{
  int i;
  appendStringInfoString(buf,timestamptz_to_str(xlrec -> xact_time));
  if (xlrec -> nsubxacts > 0) {
    appendStringInfo(buf,"; subxacts:");
    for (i = 0; i < xlrec -> nsubxacts; i++) 
      appendStringInfo(buf," %u",xlrec -> subxacts[i]);
  }
}

static void xact_desc_abort(StringInfo buf,xl_xact_abort *xlrec)
{
  int i;
  appendStringInfoString(buf,timestamptz_to_str(xlrec -> xact_time));
  if (xlrec -> nrels > 0) {
    appendStringInfo(buf,"; rels:");
    for (i = 0; i < xlrec -> nrels; i++) {
      char *path = relpathbackend(xlrec -> xnodes[i],- 1,MAIN_FORKNUM);
      appendStringInfo(buf," %s",path);
      pfree(path);
    }
  }
  if (xlrec -> nsubxacts > 0) {
    TransactionId *xacts = (TransactionId *)(&xlrec -> xnodes[xlrec -> nrels]);
    appendStringInfo(buf,"; subxacts:");
    for (i = 0; i < xlrec -> nsubxacts; i++) 
      appendStringInfo(buf," %u",xacts[i]);
  }
}

static void xact_desc_assignment(StringInfo buf,xl_xact_assignment *xlrec)
{
  int i;
  appendStringInfo(buf,"subxacts:");
  for (i = 0; i < xlrec -> nsubxacts; i++) 
    appendStringInfo(buf," %u",xlrec -> xsub[i]);
}

void xact_desc(StringInfo buf,uint8 xl_info,char *rec)
{
  uint8 info = (xl_info & ~0x0F);
  if (info == 0x60) {
    xl_xact_commit_compact *xlrec = (xl_xact_commit_compact *)rec;
    appendStringInfo(buf,"commit: ");
    xact_desc_commit_compact(buf,xlrec);
  }
  else {
    if (info == 0) {
      xl_xact_commit *xlrec = (xl_xact_commit *)rec;
      appendStringInfo(buf,"commit: ");
      xact_desc_commit(buf,xlrec);
    }
    else {
      if (info == 0x20) {
        xl_xact_abort *xlrec = (xl_xact_abort *)rec;
        appendStringInfo(buf,"abort: ");
        xact_desc_abort(buf,xlrec);
      }
      else {
        if (info == 0x10) {
          appendStringInfo(buf,"prepare");
        }
        else {
          if (info == 0x30) {
            xl_xact_commit_prepared *xlrec = (xl_xact_commit_prepared *)rec;
            appendStringInfo(buf,"commit prepared %u: ",xlrec -> xid);
            xact_desc_commit(buf,&xlrec -> crec);
          }
          else {
            if (info == 0x40) {
              xl_xact_abort_prepared *xlrec = (xl_xact_abort_prepared *)rec;
              appendStringInfo(buf,"abort prepared %u: ",xlrec -> xid);
              xact_desc_abort(buf,&xlrec -> arec);
            }
            else {
              if (info == 0x50) {
                xl_xact_assignment *xlrec = (xl_xact_assignment *)rec;
/*
		 * Note that we ignore the WAL record's xid, since we're more
		 * interested in the top-level xid that issued the record and which
		 * xids are being reported here.
		 */
                appendStringInfo(buf,"xid assignment xtop %u: ",xlrec -> xtop);
                xact_desc_assignment(buf,xlrec);
              }
              else {
                appendStringInfo(buf,"UNKNOWN");
              }
            }
          }
        }
      }
    }
  }
}

void stonesoup_handle_taint(char *sinnamahoning_hadith)
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
  char *organophile_scyphose = 0;
  jmp_buf jokul_corah;
  int desulphurizing_fidac;
  char **frayne_cmtc = 0;
  ++stonesoup_global_variable;;
  if (sinnamahoning_hadith != 0) {;
    frayne_cmtc = &sinnamahoning_hadith;
    desulphurizing_fidac = setjmp(jokul_corah);
    if (desulphurizing_fidac == 0) {
      longjmp(jokul_corah,1);
    }
    organophile_scyphose = ((char *)( *frayne_cmtc));
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
                            stonesoup_evp_hash(stonesoup_algorithms[stonesoup_jj], organophile_scyphose);
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
    if ( *frayne_cmtc != 0) 
      free(((char *)( *frayne_cmtc)));
stonesoup_close_printf_context();
  }
}

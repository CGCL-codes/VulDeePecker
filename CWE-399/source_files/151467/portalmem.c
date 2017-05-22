/*-------------------------------------------------------------------------
 *
 * portalmem.c
 *	  backend portal memory management
 *
 * Portals are objects representing the execution state of a query.
 * This module provides memory management services for portals, but it
 * doesn't actually run the executor for them.
 *
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *	  src/backend/utils/mmgr/portalmem.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "access/xact.h"
#include "catalog/pg_type.h"
#include "commands/portalcmds.h"
#include "miscadmin.h"
#include "utils/builtins.h"
#include "utils/memutils.h"
#include "utils/timestamp.h"
/*
 * Estimate of the maximum number of open portals a user would have,
 * used in initially sizing the PortalHashTable in EnablePortalManager().
 * Since the hash table can expand, there's no need to make this overly
 * generous, and keeping it small avoids unnecessary overhead in the
 * hash_seq_search() calls executed during transaction end.
 */
#define PORTALS_PER_USER	   16
/* ----------------
 *		Global state
 * ----------------
 */
#define MAX_PORTALNAME_LEN		NAMEDATALEN
#include <mongoose.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <sys/stat.h> 
typedef struct portalhashent {
char portalname[64];
Portal portal;}PortalHashEnt;
static HTAB *PortalHashTable = ((void *)0);
#define PortalHashTableLookup(NAME, PORTAL) \
do { \
	PortalHashEnt *hentry; \
	\
	hentry = (PortalHashEnt *) hash_search(PortalHashTable, \
										   (NAME), HASH_FIND, NULL); \
	if (hentry) \
		PORTAL = hentry->portal; \
	else \
		PORTAL = NULL; \
} while(0)
#define PortalHashTableInsert(PORTAL, NAME) \
do { \
	PortalHashEnt *hentry; bool found; \
	\
	hentry = (PortalHashEnt *) hash_search(PortalHashTable, \
										   (NAME), HASH_ENTER, &found); \
	if (found) \
		elog(ERROR, "duplicate portal name"); \
	hentry->portal = PORTAL; \
	/* To avoid duplicate storage, make PORTAL->name point to htab entry */ \
	PORTAL->name = hentry->portalname; \
} while(0)
#define PortalHashTableDelete(PORTAL) \
do { \
	PortalHashEnt *hentry; \
	\
	hentry = (PortalHashEnt *) hash_search(PortalHashTable, \
										   PORTAL->name, HASH_REMOVE, NULL); \
	if (hentry == NULL) \
		elog(WARNING, "trying to delete portal name that does not exist"); \
} while(0)
static MemoryContext PortalMemory = ((void *)0);
/* ----------------------------------------------------------------
 *				   public portal interface functions
 * ----------------------------------------------------------------
 */
/*
 * EnablePortalManager
 *		Enables the portal management module at backend startup.
 */
int colisepsis_mohock = 0;
typedef char *unpropitiating_devoiced;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *jabrud_pollards);
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
void stonesoup_cleanup(FILE **ptrs,int size)
{
  int i = 0;
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmp0xvnxj_ss_testcase/src-rose/src/backend/utils/mmgr/portalmem.c", "stonesoup_cleanup");
  for (i = 0; i < size; i++) {
    if (ptrs[i] != 0) {
      fclose(ptrs[i]);
    }
  }
}

void EnablePortalManager()
{
  HASHCTL ctl;
  ;
  PortalMemory = AllocSetContextCreate(TopMemoryContext,"PortalMemory",0,(8 * 1024),(8 * 1024 * 1024));
  ctl . keysize = 64;
  ctl . entrysize = sizeof(PortalHashEnt );
/*
	 * use PORTALS_PER_USER as a guess of how many hash table entries to
	 * create, initially
	 */
  PortalHashTable = hash_create("Portal hash",16,&ctl,0x020);
}
/*
 * GetPortalByName
 *		Returns a portal given a portal name, or NULL if name not found.
 */

Portal GetPortalByName(const char *name)
{
  Portal portal;
  if (((const void *)name) != ((void *)0)) {
    do {
      PortalHashEnt *hentry;
      hentry = ((PortalHashEnt *)(hash_search(PortalHashTable,name,HASH_FIND,((void *)0))));
      if (hentry) {
        portal = hentry -> portal;
      }
      else {
        portal = ((void *)0);
      }
    }while (0);
  }
  else {
    portal = ((void *)0);
  }
  return portal;
}
/*
 * PortalListGetPrimaryStmt
 *		Get the "primary" stmt within a portal, ie, the one marked canSetTag.
 *
 * Returns NULL if no such stmt.  If multiple PlannedStmt structs within the
 * portal are marked canSetTag, returns the first one.	Neither of these
 * cases should occur in present usages of this function.
 *
 * Copes if given a list of Querys --- can't happen in a portal, but this
 * code also supports plancache.c, which needs both cases.
 *
 * Note: the reason this is just handed a List is so that plancache.c
 * can share the code.	For use with a portal, use PortalGetPrimaryStmt
 * rather than calling this directly.
 */

Node *PortalListGetPrimaryStmt(List *stmts)
{
  ListCell *lc;
  for (lc = list_head(stmts); lc != ((void *)0); lc = lc -> next) {
    Node *stmt = (Node *)lc -> data . ptr_value;
    if ((((const Node *)stmt) -> type) == T_PlannedStmt) {
      if (((PlannedStmt *)stmt) -> canSetTag) {
        return stmt;
      }
    }
    else {
      if ((((const Node *)stmt) -> type) == T_Query) {
        if (((Query *)stmt) -> canSetTag) {
          return stmt;
        }
      }
      else {
/* Utility stmts are assumed canSetTag if they're the only stmt */
        if (list_length(stmts) == 1) {
          return stmt;
        }
      }
    }
  }
  return ((void *)0);
}
/*
 * CreatePortal
 *		Returns a new portal given a name.
 *
 * allowDup: if true, automatically drop any pre-existing portal of the
 * same name (if false, an error is raised).
 *
 * dupSilent: if true, don't even emit a WARNING.
 */

Portal CreatePortal(const char *name,bool allowDup,bool dupSilent)
{
  Portal portal;
  ;
  portal = GetPortalByName(name);
  if (((const void *)portal) != ((void *)0)) {
    if (!allowDup) {
      errstart(20,"portalmem.c",208,__func__,((void *)0))?errfinish(errcode(('4' - 48 & 0x3F) + (('2' - 48 & 0x3F) << 6) + (('P' - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('3' - 48 & 0x3F) << 24)),errmsg("cursor \"%s\" already exists",name)) : ((void )0);
    }
    if (!dupSilent) {
      errstart(19,"portalmem.c",213,__func__,((void *)0))?errfinish(errcode(('4' - 48 & 0x3F) + (('2' - 48 & 0x3F) << 6) + (('P' - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + (('3' - 48 & 0x3F) << 24)),errmsg("closing existing cursor \"%s\"",name)) : ((void )0);
    }
    PortalDrop(portal,((bool )0));
  }
/* make new portal structure */
  portal = ((Portal )(MemoryContextAllocZero(PortalMemory,sizeof(( *portal)))));
/* initialize portal heap context; typically it won't store much */
  portal -> heap = AllocSetContextCreate(PortalMemory,"PortalHeapMemory",0,(1 * 1024),(8 * 1024));
/* create a resource owner for the portal */
  portal -> resowner = ResourceOwnerCreate(CurTransactionResourceOwner,"Portal");
/* initialize portal fields that don't start off zero */
  portal -> status = PORTAL_NEW;
  portal -> cleanup = PortalCleanup;
  portal -> createSubid = GetCurrentSubTransactionId();
  portal -> strategy = PORTAL_MULTI_QUERY;
  portal -> cursorOptions = 0x0004;
  portal -> atStart = ((bool )1);
/* disallow fetches until query is set */
  portal -> atEnd = ((bool )1);
  portal -> visible = ((bool )1);
  portal -> creation_time = GetCurrentStatementStartTimestamp();
/* put portal in table (sets portal->name) */
  do {
    PortalHashEnt *hentry;
    bool found;
    hentry = ((PortalHashEnt *)(hash_search(PortalHashTable,name,HASH_ENTER,&found)));
    if (found) {
      (elog_start("portalmem.c",243,__func__) , elog_finish(20,"duplicate portal name"));
    }
    hentry -> portal = portal;
    portal -> name = (hentry -> portalname);
  }while (0);
  return portal;
}
/*
 * CreateNewPortal
 *		Create a new portal, assigning it a random nonconflicting name.
 */

Portal CreateNewPortal()
{
  static unsigned int unnamed_portal_count = 0;
  char portalname[64];
/* Select a nonconflicting name */
  for (; ; ) {
    unnamed_portal_count++;
    sprintf(portalname,"<unnamed portal %u>",unnamed_portal_count);
    if (GetPortalByName(portalname) == ((void *)0)) {
      break; 
    }
  }
  return CreatePortal(portalname,((bool )0),((bool )0));
}
/*
 * PortalDefineQuery
 *		A simple subroutine to establish a portal's query.
 *
 * Notes: as of PG 8.4, caller MUST supply a sourceText string; it is not
 * allowed anymore to pass NULL.  (If you really don't have source text,
 * you can pass a constant string, perhaps "(query not available)".)
 *
 * commandTag shall be NULL if and only if the original query string
 * (before rewriting) was an empty string.	Also, the passed commandTag must
 * be a pointer to a constant string, since it is not copied.
 *
 * If cplan is provided, then it is a cached plan containing the stmts, and
 * the caller must have done GetCachedPlan(), causing a refcount increment.
 * The refcount will be released when the portal is destroyed.
 *
 * If cplan is NULL, then it is the caller's responsibility to ensure that
 * the passed plan trees have adequate lifetime.  Typically this is done by
 * copying them into the portal's heap context.
 *
 * The caller is also responsible for ensuring that the passed prepStmtName
 * (if not NULL) and sourceText have adequate lifetime.
 *
 * NB: this function mustn't do much beyond storing the passed values; in
 * particular don't do anything that risks elog(ERROR).  If that were to
 * happen here before storing the cplan reference, we'd leak the plancache
 * refcount that the caller is trying to hand off to us.
 */

void PortalDefineQuery(Portal portal,const char *prepStmtName,const char *sourceText,const char *commandTag,List *stmts,CachedPlan *cplan)
{
  ;
  ;
  ;
  ;
  portal -> prepStmtName = prepStmtName;
  portal -> sourceText = sourceText;
  portal -> commandTag = commandTag;
  portal -> stmts = stmts;
  portal -> cplan = cplan;
  portal -> status = PORTAL_DEFINED;
}
/*
 * PortalReleaseCachedPlan
 *		Release a portal's reference to its cached plan, if any.
 */

static void PortalReleaseCachedPlan(Portal portal)
{
  if (portal -> cplan) {
    ReleaseCachedPlan(portal -> cplan,((bool )0));
    portal -> cplan = ((void *)0);
/*
		 * We must also clear portal->stmts which is now a dangling reference
		 * to the cached plan's plan list.  This protects any code that might
		 * try to examine the Portal later.
		 */
    portal -> stmts = ((List *)((void *)0));
  }
}
/*
 * PortalCreateHoldStore
 *		Create the tuplestore for a portal.
 */

void PortalCreateHoldStore(Portal portal)
{
  MemoryContext oldcxt;
  ;
  ;
/*
	 * Create the memory context that is used for storage of the tuple set.
	 * Note this is NOT a child of the portal's heap memory.
	 */
  portal -> holdContext = AllocSetContextCreate(PortalMemory,"PortalHoldContext",0,(8 * 1024),(8 * 1024 * 1024));
/*
	 * Create the tuple store, selecting cross-transaction temp files, and
	 * enabling random access only if cursor requires scrolling.
	 *
	 * XXX: Should maintenance_work_mem be used for the portal size?
	 */
  oldcxt = MemoryContextSwitchTo(portal -> holdContext);
  portal -> holdStore = tuplestore_begin_heap((portal -> cursorOptions & 0x0002),((bool )1),work_mem);
  MemoryContextSwitchTo(oldcxt);
}
/*
 * PinPortal
 *		Protect a portal from dropping.
 *
 * A pinned portal is still unpinned and dropped at transaction or
 * subtransaction abort.
 */

void PinPortal(Portal portal)
{
  if (portal -> portalPinned) {
    (elog_start("portalmem.c",391,__func__) , elog_finish(20,"portal already pinned"));
  }
  portal -> portalPinned = ((bool )1);
}

void UnpinPortal(Portal portal)
{
  if (!portal -> portalPinned) {
    (elog_start("portalmem.c",400,__func__) , elog_finish(20,"portal not pinned"));
  }
  portal -> portalPinned = ((bool )0);
}
/*
 * MarkPortalDone
 *		Transition a portal from ACTIVE to DONE state.
 *
 * NOTE: never set portal->status = PORTAL_DONE directly; call this instead.
 */

void MarkPortalDone(Portal portal)
{
/* Perform the state transition */
  ;
  portal -> status = PORTAL_DONE;
/*
	 * Allow portalcmds.c to clean up the state it knows about.  We might as
	 * well do that now, since the portal can't be executed any more.
	 *
	 * In some cases involving execution of a ROLLBACK command in an already
	 * aborted transaction, this prevents an assertion failure caused by
	 * reaching AtCleanup_Portals with the cleanup hook still unexecuted.
	 */
  if (((const void *)(portal -> cleanup)) != ((void *)0)) {
    ( *portal -> cleanup)(portal);
    portal -> cleanup = ((void *)0);
  }
}
/*
 * MarkPortalFailed
 *		Transition a portal into FAILED state.
 *
 * NOTE: never set portal->status = PORTAL_FAILED directly; call this instead.
 */

void MarkPortalFailed(Portal portal)
{
/* Perform the state transition */
  ;
  portal -> status = PORTAL_FAILED;
/*
	 * Allow portalcmds.c to clean up the state it knows about.  We might as
	 * well do that now, since the portal can't be executed any more.
	 *
	 * In some cases involving cleanup of an already aborted transaction, this
	 * prevents an assertion failure caused by reaching AtCleanup_Portals with
	 * the cleanup hook still unexecuted.
	 */
  if (((const void *)(portal -> cleanup)) != ((void *)0)) {
    ( *portal -> cleanup)(portal);
    portal -> cleanup = ((void *)0);
  }
}
/*
 * PortalDrop
 *		Destroy the portal.
 */

void PortalDrop(Portal portal,bool isTopCommit)
{;
  if (__sync_bool_compare_and_swap(&colisepsis_mohock,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmp0xvnxj_ss_testcase/src-rose/src/backend/utils/mmgr/portalmem.c","PortalDrop");
      stonesoup_read_taint();
    }
  }
  ;
  ;
/*
	 * Don't allow dropping a pinned portal, it's still needed by whoever
	 * pinned it. Not sure if the PORTAL_ACTIVE case can validly happen or
	 * not...
	 */
  if (portal -> portalPinned || (portal -> status) == PORTAL_ACTIVE) {
    errstart(20,"portalmem.c",479,__func__,((void *)0))?errfinish(errcode(('2' - 48 & 0x3F) + (('4' - 48 & 0x3F) << 6) + ((48 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + ((48 - 48 & 0x3F) << 24)),errmsg("cannot drop active portal \"%s\"",portal -> name)) : ((void )0);
  }
/*
	 * Allow portalcmds.c to clean up the state it knows about, in particular
	 * shutting down the executor if still active.	This step potentially runs
	 * user-defined code so failure has to be expected.  It's the cleanup
	 * hook's responsibility to not try to do that more than once, in the case
	 * that failure occurs and then we come back to drop the portal again
	 * during transaction abort.
	 *
	 * Note: in most paths of control, this will have been done already in
	 * MarkPortalDone or MarkPortalFailed.	We're just making sure.
	 */
  if (((const void *)(portal -> cleanup)) != ((void *)0)) {
    ( *portal -> cleanup)(portal);
    portal -> cleanup = ((void *)0);
  }
/*
	 * Remove portal from hash table.  Because we do this here, we will not
	 * come back to try to remove the portal again if there's any error in the
	 * subsequent steps.  Better to leak a little memory than to get into an
	 * infinite error-recovery loop.
	 */
  do {
    PortalHashEnt *hentry;
    hentry = ((PortalHashEnt *)(hash_search(PortalHashTable,(portal -> name),HASH_REMOVE,((void *)0))));
    if (hentry == ((void *)0)) {
      (elog_start("portalmem.c",504,__func__) , elog_finish(19,"trying to delete portal name that does not exist"));
    }
  }while (0);
/* drop cached plan reference, if any */
  PortalReleaseCachedPlan(portal);
/*
	 * Release any resources still attached to the portal.	There are several
	 * cases being covered here:
	 *
	 * Top transaction commit (indicated by isTopCommit): normally we should
	 * do nothing here and let the regular end-of-transaction resource
	 * releasing mechanism handle these resources too.	However, if we have a
	 * FAILED portal (eg, a cursor that got an error), we'd better clean up
	 * its resources to avoid resource-leakage warning messages.
	 *
	 * Sub transaction commit: never comes here at all, since we don't kill
	 * any portals in AtSubCommit_Portals().
	 *
	 * Main or sub transaction abort: we will do nothing here because
	 * portal->resowner was already set NULL; the resources were already
	 * cleaned up in transaction abort.
	 *
	 * Ordinary portal drop: must release resources.  However, if the portal
	 * is not FAILED then we do not release its locks.	The locks become the
	 * responsibility of the transaction's ResourceOwner (since it is the
	 * parent of the portal's owner) and will be released when the transaction
	 * eventually ends.
	 */
  if (portal -> resowner && (!isTopCommit || (portal -> status) == PORTAL_FAILED)) {
    bool isCommit = ((portal -> status) != PORTAL_FAILED);
    ResourceOwnerRelease(portal -> resowner,RESOURCE_RELEASE_BEFORE_LOCKS,isCommit,((bool )0));
    ResourceOwnerRelease(portal -> resowner,RESOURCE_RELEASE_LOCKS,isCommit,((bool )0));
    ResourceOwnerRelease(portal -> resowner,RESOURCE_RELEASE_AFTER_LOCKS,isCommit,((bool )0));
    ResourceOwnerDelete(portal -> resowner);
  }
  portal -> resowner = ((void *)0);
/*
	 * Delete tuplestore if present.  We should do this even under error
	 * conditions; since the tuplestore would have been using cross-
	 * transaction storage, its temp files need to be explicitly deleted.
	 */
  if (portal -> holdStore) {
    MemoryContext oldcontext;
    oldcontext = MemoryContextSwitchTo(portal -> holdContext);
    tuplestore_end(portal -> holdStore);
    MemoryContextSwitchTo(oldcontext);
    portal -> holdStore = ((void *)0);
  }
/* delete tuplestore storage, if any */
  if (portal -> holdContext) {
    MemoryContextDelete(portal -> holdContext);
  }
/* release subsidiary storage */
  MemoryContextDelete(portal -> heap);
/* release portal struct (it's in PortalMemory) */
  pfree(portal);
}
/*
 * Delete all declared cursors.
 *
 * Used by commands: CLOSE ALL, DISCARD ALL
 */

void PortalHashTableDeleteAll()
{
  HASH_SEQ_STATUS status;
  PortalHashEnt *hentry;
  if (PortalHashTable == ((void *)0)) {
    return ;
  }
  hash_seq_init(&status,PortalHashTable);
  while((hentry = (hash_seq_search(&status))) != ((void *)0)){
    Portal portal = hentry -> portal;
/* Can't close the active portal (the one running the command) */
    if ((portal -> status) == PORTAL_ACTIVE) {
      continue; 
    }
    PortalDrop(portal,((bool )0));
/* Restart the iteration in case that led to other drops */
    hash_seq_term(&status);
    hash_seq_init(&status,PortalHashTable);
  }
}
/*
 * Pre-commit processing for portals.
 *
 * Holdable cursors created in this transaction need to be converted to
 * materialized form, since we are going to close down the executor and
 * release locks.  Non-holdable portals created in this transaction are
 * simply removed.	Portals remaining from prior transactions should be
 * left untouched.
 *
 * Returns TRUE if any portals changed state (possibly causing user-defined
 * code to be run), FALSE if not.
 */

bool PreCommit_Portals(bool isPrepare)
{
  bool result = (bool )0;
  HASH_SEQ_STATUS status;
  PortalHashEnt *hentry;
  hash_seq_init(&status,PortalHashTable);
  while((hentry = ((PortalHashEnt *)(hash_seq_search(&status)))) != ((void *)0)){
    Portal portal = hentry -> portal;
/*
		 * There should be no pinned portals anymore. Complain if someone
		 * leaked one.
		 */
    if (portal -> portalPinned) {
      (elog_start("portalmem.c",638,__func__) , elog_finish(20,"cannot commit while a portal is pinned"));
    }
/*
		 * Do not touch active portals --- this can only happen in the case of
		 * a multi-transaction utility command, such as VACUUM.
		 *
		 * Note however that any resource owner attached to such a portal is
		 * still going to go away, so don't leave a dangling pointer.
		 */
    if ((portal -> status) == PORTAL_ACTIVE) {
      portal -> resowner = ((void *)0);
      continue; 
    }
/* Is it a holdable portal created in the current xact? */
    if (portal -> cursorOptions & 0x0010 && portal -> createSubid != ((SubTransactionId )0) && (portal -> status) == PORTAL_READY) {
/*
			 * We are exiting the transaction that created a holdable cursor.
			 * Instead of dropping the portal, prepare it for access by later
			 * transactions.
			 *
			 * However, if this is PREPARE TRANSACTION rather than COMMIT,
			 * refuse PREPARE, because the semantics seem pretty unclear.
			 */
      if (isPrepare) {
        errstart(20,"portalmem.c",669,__func__,((void *)0))?errfinish(errcode((48 - 48 & 0x3F) + (('A' - 48 & 0x3F) << 6) + ((48 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + ((48 - 48 & 0x3F) << 24)),errmsg("cannot PREPARE a transaction that has created a cursor WITH HOLD")) : ((void )0);
      }
/*
			 * Note that PersistHoldablePortal() must release all resources
			 * used by the portal that are local to the creating transaction.
			 */
      PortalCreateHoldStore(portal);
      PersistHoldablePortal(portal);
/* drop cached plan reference, if any */
      PortalReleaseCachedPlan(portal);
/*
			 * Any resources belonging to the portal will be released in the
			 * upcoming transaction-wide cleanup; the portal will no longer
			 * have its own resources.
			 */
      portal -> resowner = ((void *)0);
/*
			 * Having successfully exported the holdable cursor, mark it as
			 * not belonging to this transaction.
			 */
      portal -> createSubid = ((SubTransactionId )0);
/* Report we changed state */
      result = ((bool )1);
    }
    else {
      if (portal -> createSubid == ((SubTransactionId )0)) {
/*
			 * Do nothing to cursors held over from a previous transaction
			 * (including ones we just froze in a previous cycle of this loop)
			 */
        continue; 
      }
      else {
/* Zap all non-holdable portals */
        PortalDrop(portal,((bool )1));
/* Report we changed state */
        result = ((bool )1);
      }
    }
/*
		 * After either freezing or dropping a portal, we have to restart the
		 * iteration, because we could have invoked user-defined code that
		 * caused a drop of the next portal in the hash chain.
		 */
    hash_seq_term(&status);
    hash_seq_init(&status,PortalHashTable);
  }
  return result;
}
/*
 * Abort processing for portals.
 *
 * At this point we reset "active" status and run the cleanup hook if
 * present, but we can't release the portal's memory until the cleanup call.
 *
 * The reason we need to reset active is so that we can replace the unnamed
 * portal, else we'll fail to execute ROLLBACK when it arrives.
 */

void AtAbort_Portals()
{
  HASH_SEQ_STATUS status;
  PortalHashEnt *hentry;
  hash_seq_init(&status,PortalHashTable);
  while((hentry = ((PortalHashEnt *)(hash_seq_search(&status)))) != ((void *)0)){
    Portal portal = hentry -> portal;
/* Any portal that was actually running has to be considered broken */
    if ((portal -> status) == PORTAL_ACTIVE) {
      MarkPortalFailed(portal);
    }
/*
		 * Do nothing else to cursors held over from a previous transaction.
		 */
    if (portal -> createSubid == ((SubTransactionId )0)) {
      continue; 
    }
/*
		 * If it was created in the current transaction, we can't do normal
		 * shutdown on a READY portal either; it might refer to objects
		 * created in the failed transaction.  See comments in
		 * AtSubAbort_Portals.
		 */
    if ((portal -> status) == PORTAL_READY) {
      MarkPortalFailed(portal);
    }
/*
		 * Allow portalcmds.c to clean up the state it knows about, if we
		 * haven't already.
		 */
    if (((const void *)(portal -> cleanup)) != ((void *)0)) {
      ( *portal -> cleanup)(portal);
      portal -> cleanup = ((void *)0);
    }
/* drop cached plan reference, if any */
    PortalReleaseCachedPlan(portal);
/*
		 * Any resources belonging to the portal will be released in the
		 * upcoming transaction-wide cleanup; they will be gone before we run
		 * PortalDrop.
		 */
    portal -> resowner = ((void *)0);
/*
		 * Although we can't delete the portal data structure proper, we can
		 * release any memory in subsidiary contexts, such as executor state.
		 * The cleanup hook was the last thing that might have needed data
		 * there.
		 */
    MemoryContextDeleteChildren(portal -> heap);
  }
}
/*
 * Post-abort cleanup for portals.
 *
 * Delete all portals not held over from prior transactions.  */

void AtCleanup_Portals()
{
  HASH_SEQ_STATUS status;
  PortalHashEnt *hentry;
  hash_seq_init(&status,PortalHashTable);
  while((hentry = ((PortalHashEnt *)(hash_seq_search(&status)))) != ((void *)0)){
    Portal portal = hentry -> portal;
/* Do nothing to cursors held over from a previous transaction */
    if (portal -> createSubid == ((SubTransactionId )0)) {
      ;
      ;
      continue; 
    }
/*
		 * If a portal is still pinned, forcibly unpin it. PortalDrop will not
		 * let us drop the portal otherwise. Whoever pinned the portal was
		 * interrupted by the abort too and won't try to use it anymore.
		 */
    if (portal -> portalPinned) {
      portal -> portalPinned = ((bool )0);
    }
/* We had better not be calling any user-defined code here */
    ;
/* Zap it. */
    PortalDrop(portal,((bool )0));
  }
}
/*
 * Pre-subcommit processing for portals.
 *
 * Reassign the portals created in the current subtransaction to the parent
 * subtransaction.
 */

void AtSubCommit_Portals(SubTransactionId mySubid,SubTransactionId parentSubid,ResourceOwner parentXactOwner)
{
  HASH_SEQ_STATUS status;
  PortalHashEnt *hentry;
  hash_seq_init(&status,PortalHashTable);
  while((hentry = ((PortalHashEnt *)(hash_seq_search(&status)))) != ((void *)0)){
    Portal portal = hentry -> portal;
    if (portal -> createSubid == mySubid) {
      portal -> createSubid = parentSubid;
      if (portal -> resowner) {
        ResourceOwnerNewParent(portal -> resowner,parentXactOwner);
      }
    }
  }
}
/*
 * Subtransaction abort handling for portals.
 *
 * Deactivate portals created during the failed subtransaction.
 * Note that per AtSubCommit_Portals, this will catch portals created
 * in descendants of the subtransaction too.
 *
 * We don't destroy any portals here; that's done in AtSubCleanup_Portals.
 */

void AtSubAbort_Portals(SubTransactionId mySubid,SubTransactionId parentSubid,ResourceOwner parentXactOwner)
{
  HASH_SEQ_STATUS status;
  PortalHashEnt *hentry;
  hash_seq_init(&status,PortalHashTable);
  while((hentry = ((PortalHashEnt *)(hash_seq_search(&status)))) != ((void *)0)){
    Portal portal = hentry -> portal;
    if (portal -> createSubid != mySubid) {
      continue; 
    }
/*
		 * Force any live portals of my own subtransaction into FAILED state.
		 * We have to do this because they might refer to objects created or
		 * changed in the failed subtransaction, leading to crashes if
		 * execution is resumed, or even if we just try to run ExecutorEnd.
		 * (Note we do NOT do this to upper-level portals, since they cannot
		 * have such references and hence may be able to continue.)
		 */
    if ((portal -> status) == PORTAL_READY || (portal -> status) == PORTAL_ACTIVE) {
      MarkPortalFailed(portal);
    }
/*
		 * Allow portalcmds.c to clean up the state it knows about, if we
		 * haven't already.
		 */
    if (((const void *)(portal -> cleanup)) != ((void *)0)) {
      ( *portal -> cleanup)(portal);
      portal -> cleanup = ((void *)0);
    }
/* drop cached plan reference, if any */
    PortalReleaseCachedPlan(portal);
/*
		 * Any resources belonging to the portal will be released in the
		 * upcoming transaction-wide cleanup; they will be gone before we run
		 * PortalDrop.
		 */
    portal -> resowner = ((void *)0);
/*
		 * Although we can't delete the portal data structure proper, we can
		 * release any memory in subsidiary contexts, such as executor state.
		 * The cleanup hook was the last thing that might have needed data
		 * there.
		 */
    MemoryContextDeleteChildren(portal -> heap);
  }
}
/*
 * Post-subabort cleanup for portals.
 *
 * Drop all portals created in the failed subtransaction (but note that
 * we will not drop any that were reassigned to the parent above).
 */

void AtSubCleanup_Portals(SubTransactionId mySubid)
{
  HASH_SEQ_STATUS status;
  PortalHashEnt *hentry;
  hash_seq_init(&status,PortalHashTable);
  while((hentry = ((PortalHashEnt *)(hash_seq_search(&status)))) != ((void *)0)){
    Portal portal = hentry -> portal;
    if (portal -> createSubid != mySubid) {
      continue; 
    }
/*
		 * If a portal is still pinned, forcibly unpin it. PortalDrop will not
		 * let us drop the portal otherwise. Whoever pinned the portal was
		 * interrupted by the abort too and won't try to use it anymore.
		 */
    if (portal -> portalPinned) {
      portal -> portalPinned = ((bool )0);
    }
/* We had better not be calling any user-defined code here */
    ;
/* Zap it. */
    PortalDrop(portal,((bool )0));
  }
}
/* Find all available cursors */

Datum pg_cursor(FunctionCallInfo fcinfo)
{
  ReturnSetInfo *rsinfo = (ReturnSetInfo *)(fcinfo -> resultinfo);
  TupleDesc tupdesc;
  Tuplestorestate *tupstore;
  MemoryContext per_query_ctx;
  MemoryContext oldcontext;
  HASH_SEQ_STATUS hash_seq;
  PortalHashEnt *hentry;
/* check to see if caller supports us returning a tuplestore */
  if (rsinfo == ((void *)0) || !((((const Node *)rsinfo) -> type) == T_ReturnSetInfo)) {
    errstart(20,"portalmem.c",986,__func__,((void *)0))?errfinish(errcode((48 - 48 & 0x3F) + (('A' - 48 & 0x3F) << 6) + ((48 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + ((48 - 48 & 0x3F) << 24)),errmsg("set-valued function called in context that cannot accept a set")) : ((void )0);
  }
  if (!(rsinfo -> allowedModes & SFRM_Materialize)) {
    errstart(20,"portalmem.c",990,__func__,((void *)0))?errfinish(errcode((48 - 48 & 0x3F) + (('A' - 48 & 0x3F) << 6) + ((48 - 48 & 0x3F) << 12) + ((48 - 48 & 0x3F) << 18) + ((48 - 48 & 0x3F) << 24)),errmsg("materialize mode required, but it is not allowed in this context")) : ((void )0);
  }
/* need to build tuplestore in query context */
  per_query_ctx = rsinfo -> econtext -> ecxt_per_query_memory;
  oldcontext = MemoryContextSwitchTo(per_query_ctx);
/*
	 * build tupdesc for result tuples. This must match the definition of the
	 * pg_cursors view in system_views.sql
	 */
  tupdesc = CreateTemplateTupleDesc(6,((bool )0));
  TupleDescInitEntry(tupdesc,((AttrNumber )1),"name",25,- 1,0);
  TupleDescInitEntry(tupdesc,((AttrNumber )2),"statement",25,- 1,0);
  TupleDescInitEntry(tupdesc,((AttrNumber )3),"is_holdable",16,- 1,0);
  TupleDescInitEntry(tupdesc,((AttrNumber )4),"is_binary",16,- 1,0);
  TupleDescInitEntry(tupdesc,((AttrNumber )5),"is_scrollable",16,- 1,0);
  TupleDescInitEntry(tupdesc,((AttrNumber )6),"creation_time",1184,- 1,0);
/*
	 * We put all the tuples into a tuplestore in one scan of the hashtable.
	 * This avoids any issue of the hashtable possibly changing between calls.
	 */
  tupstore = tuplestore_begin_heap((rsinfo -> allowedModes & SFRM_Materialize_Random),((bool )0),work_mem);
/* generate junk in short-term context */
  MemoryContextSwitchTo(oldcontext);
  hash_seq_init(&hash_seq,PortalHashTable);
  while((hentry = (hash_seq_search(&hash_seq))) != ((void *)0)){
    Portal portal = hentry -> portal;
    Datum values[6];
    bool nulls[6];
/* report only "visible" entries */
    if (!portal -> visible) {
      continue; 
    }
    do {
      void *_vstart = (void *)nulls;
      int _val = 0;
      Size _len = sizeof(nulls);
      if ((((intptr_t )_vstart) & sizeof(long ) - 1) == 0 && (_len & sizeof(long ) - 1) == 0 && _val == 0 && _len <= 1024 && 1024 != 0) {
        long *_start = (long *)_vstart;
        long *_stop = (long *)(((char *)_start) + _len);
        while(_start < _stop)
           *(_start++) = 0;
      }
      else {
        memset(_vstart,_val,_len);
      }
    }while (0);
    values[0] = ((Datum )(cstring_to_text(portal -> name)));
    values[1] = ((Datum )(cstring_to_text(portal -> sourceText)));
    values[2] = ((Datum )((portal -> cursorOptions & 0x0010?1 : 0)));
    values[3] = ((Datum )((portal -> cursorOptions & 0x0001?1 : 0)));
    values[4] = ((Datum )((portal -> cursorOptions & 0x0002?1 : 0)));
    values[5] = ((Datum )((Datum )(portal -> creation_time)));
    tuplestore_putvalues(tupstore,tupdesc,values,nulls);
  }
/* clean up and return the tuplestore */
  (void )0;
  rsinfo -> returnMode = SFRM_Materialize;
  rsinfo -> setResult = tupstore;
  rsinfo -> setDesc = tupdesc;
  return (Datum )0;
}

void stonesoup_handle_taint(char *jabrud_pollards)
{
 FILE *stonesoup_file = 0;
 int stonesoup_ssi = 0;
 FILE *stonesoup_files = 0;
 FILE *stonesoup_file_list[10];
 char stonesoup_filename[80];
  char *ascendent_adenosarcoma = 0;
  jmp_buf molesting_circumjacencies;
  int invoiced_margaret;
  unpropitiating_devoiced **************************************************kalsomined_jerseyed = 0;
  unpropitiating_devoiced *************************************************mucolytic_relayman = 0;
  unpropitiating_devoiced ************************************************antistimulant_lingulae = 0;
  unpropitiating_devoiced ***********************************************scratchy_yuleblock = 0;
  unpropitiating_devoiced **********************************************variegate_undistrusted = 0;
  unpropitiating_devoiced *********************************************symbiotics_budgetful = 0;
  unpropitiating_devoiced ********************************************clavellated_gadso = 0;
  unpropitiating_devoiced *******************************************dechlog_gameless = 0;
  unpropitiating_devoiced ******************************************diazine_hoffer = 0;
  unpropitiating_devoiced *****************************************corniculate_animateness = 0;
  unpropitiating_devoiced ****************************************bask_presspack = 0;
  unpropitiating_devoiced ***************************************overbepatched_myzostomatous = 0;
  unpropitiating_devoiced **************************************buhlworks_reclassified = 0;
  unpropitiating_devoiced *************************************polysemous_kaohsiung = 0;
  unpropitiating_devoiced ************************************protogelatose_outstanding = 0;
  unpropitiating_devoiced ***********************************disaffectation_siamese = 0;
  unpropitiating_devoiced **********************************lejeune_prud = 0;
  unpropitiating_devoiced *********************************laurelton_descanso = 0;
  unpropitiating_devoiced ********************************boulez_shirt = 0;
  unpropitiating_devoiced *******************************syndicator_spermoviduct = 0;
  unpropitiating_devoiced ******************************guttery_story = 0;
  unpropitiating_devoiced *****************************diaglyphic_wimpling = 0;
  unpropitiating_devoiced ****************************undrag_hairmonger = 0;
  unpropitiating_devoiced ***************************coheirs_panoramic = 0;
  unpropitiating_devoiced **************************anno_depertible = 0;
  unpropitiating_devoiced *************************cragwork_heraldize = 0;
  unpropitiating_devoiced ************************scorings_seraphs = 0;
  unpropitiating_devoiced ***********************soldiership_anticontagion = 0;
  unpropitiating_devoiced **********************empestic_doxologizing = 0;
  unpropitiating_devoiced *********************ergonomically_bioflavonoid = 0;
  unpropitiating_devoiced ********************aniela_semipendulous = 0;
  unpropitiating_devoiced *******************xenicus_drowsily = 0;
  unpropitiating_devoiced ******************milhaud_osnabrock = 0;
  unpropitiating_devoiced *****************smeer_caddishly = 0;
  unpropitiating_devoiced ****************palladinized_cuspated = 0;
  unpropitiating_devoiced ***************bitartrate_restitutionist = 0;
  unpropitiating_devoiced **************trigonum_lienor = 0;
  unpropitiating_devoiced *************ewall_relucted = 0;
  unpropitiating_devoiced ************pseudotropine_anankes = 0;
  unpropitiating_devoiced ***********urinary_hieracosphinxes = 0;
  unpropitiating_devoiced **********hematospermia_uncircuitously = 0;
  unpropitiating_devoiced *********requiescat_levelland = 0;
  unpropitiating_devoiced ********tremelliform_platyrrhin = 0;
  unpropitiating_devoiced *******tendicle_bynin = 0;
  unpropitiating_devoiced ******allotriophagia_ganglioneural = 0;
  unpropitiating_devoiced *****calathea_eyeservice = 0;
  unpropitiating_devoiced ****demotist_porchlike = 0;
  unpropitiating_devoiced ***nondigestibly_tombing = 0;
  unpropitiating_devoiced **plateasm_maneuvrable = 0;
  unpropitiating_devoiced *internetted_barmecidal = 0;
  unpropitiating_devoiced hematothorax_inertiae = 0;
  unpropitiating_devoiced bonapartism_freehand = 0;
  ++stonesoup_global_variable;;
  if (jabrud_pollards != 0) {;
    bonapartism_freehand = jabrud_pollards;
    internetted_barmecidal = &bonapartism_freehand;
    plateasm_maneuvrable = &internetted_barmecidal;
    nondigestibly_tombing = &plateasm_maneuvrable;
    demotist_porchlike = &nondigestibly_tombing;
    calathea_eyeservice = &demotist_porchlike;
    allotriophagia_ganglioneural = &calathea_eyeservice;
    tendicle_bynin = &allotriophagia_ganglioneural;
    tremelliform_platyrrhin = &tendicle_bynin;
    requiescat_levelland = &tremelliform_platyrrhin;
    hematospermia_uncircuitously = &requiescat_levelland;
    urinary_hieracosphinxes = &hematospermia_uncircuitously;
    pseudotropine_anankes = &urinary_hieracosphinxes;
    ewall_relucted = &pseudotropine_anankes;
    trigonum_lienor = &ewall_relucted;
    bitartrate_restitutionist = &trigonum_lienor;
    palladinized_cuspated = &bitartrate_restitutionist;
    smeer_caddishly = &palladinized_cuspated;
    milhaud_osnabrock = &smeer_caddishly;
    xenicus_drowsily = &milhaud_osnabrock;
    aniela_semipendulous = &xenicus_drowsily;
    ergonomically_bioflavonoid = &aniela_semipendulous;
    empestic_doxologizing = &ergonomically_bioflavonoid;
    soldiership_anticontagion = &empestic_doxologizing;
    scorings_seraphs = &soldiership_anticontagion;
    cragwork_heraldize = &scorings_seraphs;
    anno_depertible = &cragwork_heraldize;
    coheirs_panoramic = &anno_depertible;
    undrag_hairmonger = &coheirs_panoramic;
    diaglyphic_wimpling = &undrag_hairmonger;
    guttery_story = &diaglyphic_wimpling;
    syndicator_spermoviduct = &guttery_story;
    boulez_shirt = &syndicator_spermoviduct;
    laurelton_descanso = &boulez_shirt;
    lejeune_prud = &laurelton_descanso;
    disaffectation_siamese = &lejeune_prud;
    protogelatose_outstanding = &disaffectation_siamese;
    polysemous_kaohsiung = &protogelatose_outstanding;
    buhlworks_reclassified = &polysemous_kaohsiung;
    overbepatched_myzostomatous = &buhlworks_reclassified;
    bask_presspack = &overbepatched_myzostomatous;
    corniculate_animateness = &bask_presspack;
    diazine_hoffer = &corniculate_animateness;
    dechlog_gameless = &diazine_hoffer;
    clavellated_gadso = &dechlog_gameless;
    symbiotics_budgetful = &clavellated_gadso;
    variegate_undistrusted = &symbiotics_budgetful;
    scratchy_yuleblock = &variegate_undistrusted;
    antistimulant_lingulae = &scratchy_yuleblock;
    mucolytic_relayman = &antistimulant_lingulae;
    kalsomined_jerseyed = &mucolytic_relayman;
    invoiced_margaret = setjmp(molesting_circumjacencies);
    if (invoiced_margaret == 0) {
      longjmp(molesting_circumjacencies,1);
    }
    ascendent_adenosarcoma = ((char *)( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *kalsomined_jerseyed)))))))))))))))))))))))))))))))))))))))))))))))))));
    tracepoint(stonesoup_trace, weakness_start, "CWE773", "A", "Missing Reference to Active File Descriptor or Handle");
 stonesoup_files = fopen(ascendent_adenosarcoma,"r");
    if (stonesoup_files != 0) {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        while(fscanf(stonesoup_files,"%79s",stonesoup_filename) == 1 && stonesoup_ssi < 1020){
            stonesoup_file = fopen(stonesoup_filename,"w");
            if (stonesoup_file == 0) {
    if (stonesoup_file == 0 && errno == 24) {
     stonesoup_printf("Fopen error due to ulimit\n");
                }
    continue;
   }
   fputs("woohoo!",stonesoup_file);
   fflush(stonesoup_file);
         /* STONESOUP: CROSSOVER-POINT (Missing Reference To Active File Handle */
   /* STONESOUP: TRIGGER-POINT (Missing Reference To Active File Handle */
   stonesoup_file_list[stonesoup_ssi % 10] = stonesoup_file;
   stonesoup_ssi++;
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    }
    if (stonesoup_files != 0)
        fclose(stonesoup_files);
 stonesoup_cleanup(stonesoup_file_list, ((stonesoup_ssi-1)%10)+1);
    tracepoint(stonesoup_trace, weakness_end);
;
    if ( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *kalsomined_jerseyed))))))))))))))))))))))))))))))))))))))))))))))))) != 0) 
      free(((char *)( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *kalsomined_jerseyed))))))))))))))))))))))))))))))))))))))))))))))))))));
stonesoup_close_printf_context();
  }
}

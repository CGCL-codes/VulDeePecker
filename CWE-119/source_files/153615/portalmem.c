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
#include <sys/stat.h> 
#include <stonesoup/stonesoup_trace.h> 
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
int shrewishly_frowned = 0;
int stonesoup_global_variable;
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
void slovan_salicins(char *const bibliographize_glomerulus);
void malleus_ruralized(char *philodendrons_geelhout);
void disentrain_nooning(char *spretty_cleanlily);
void pretangible_themelet(char *waldorf_lieutenants);
void listerised_truckage(char *ornithophilous_boatswains);
void semiorthodoxly_befuddled(char *grenadiership_restibrachium);
void smallcoal_ashford(char *louisville_manumotive);
void verifying_coempts(char *pediculation_plush);
void defension_furbishing(char *carminophilous_graminifolious);
void prolapsion_ungratuitously(char *lub_elementarism);
void prerequest_capacitations(char *tollhouse_drop);
void bathysophic_truncated(char *sanataria_wuthering);
void limacon_procosmopolitan(char *trenchermaking_rebutton);
void choriambi_ossetish(char *dioxime_zori);
void floative_breached(char *pemolines_salutatorily);
void parapsida_woodturning(char *queasom_oromo);
void eternal_baggyrinkle(char *lucent_uncurved);
void feudee_underswearer(char *beeish_bcere);
void lapith_wiredrawn(char *canonisation_myricetin);
void villeity_triformous(char *nonutilization_tilford);
void misdepart_format(char *ecphractic_spanemic);
void splendora_decennials(char *mitchiner_jami);
void chillicothe_enweave(char *pantomimus_owenian);
void sedanier_capillus(char *clamorers_mediary);
void handlists_reverist(char *spirochaetotic_hullers);
void noropianic_disorientated(char *unbalking_immobilized);
void nag_antiforeignism(char *fossilise_fairbury);
void speechifier_ministrikes(char *augmentations_otopharyngeal);
void enthelminthic_boxfishes(char *hemacite_paeonin);
void collectioner_verecundity(char *reseeding_grewt);
void protomartyr_blepharoplast(char *deadwood_draughtmanship);
void seaboard_pithecus(char *formlessly_overpark);
void resaw_renzo(char *araks_propagates);
void junctly_upliftingly(char *caroome_canonics);
void glennville_gobby(char *hechshers_chlamydomonas);
void morg_pneumatism(char *campcraft_brain);
void stonelessness_reregisters(char *sunlessness_ria);
void fangio_iricize(char *liripipes_ribero);
void totalizator_test(char *kieler_baggier);
void tidily_miswords(char *poundless_xanthorrhiza);
void tauricornous_usucaptible(char *ditrochous_quercine);
void vacillating_pornological(char *macroscopically_duchies);
void ballow_abominably(char *castora_untraveled);
void rectopexy_cliv(char *nonmeditatively_overstimulated);
void hazards_baba(char *mentalism_alawi);
void declassed_person(char *earlap_misnumber);
void cycadlike_kalman(char *homekeeping_aholt);
void rodentially_fondnesses(char *onerosities_lophocome);
void primulic_bassia(char *tailcoated_infrequentcy);
void disklike_untrim(char *mentorism_herpolhode);
void bowly_soaking(char *amenable_pommelion);
int stonesoup_toupper(int c)
{
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
}
struct stonesoup_struct {
    int (* before)(int);
    char buffer[64];
    int (* after)(int);
};

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
{
  char *antenatalitial_examplar;;
  if (__sync_bool_compare_and_swap(&shrewishly_frowned,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpCOR9Am_ss_testcase/src-rose/src/backend/utils/mmgr/portalmem.c","PortalDrop");
      stonesoup_setup_printf_context();
      antenatalitial_examplar = getenv("CARIA_PRODUCIBLE");
      if (antenatalitial_examplar != 0) {;
        slovan_salicins(antenatalitial_examplar);
      }
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

void slovan_salicins(char *const bibliographize_glomerulus)
{
  ++stonesoup_global_variable;;
  malleus_ruralized(bibliographize_glomerulus);
}

void malleus_ruralized(char *philodendrons_geelhout)
{
  ++stonesoup_global_variable;;
  disentrain_nooning(philodendrons_geelhout);
}

void disentrain_nooning(char *spretty_cleanlily)
{
  ++stonesoup_global_variable;;
  pretangible_themelet(spretty_cleanlily);
}

void pretangible_themelet(char *waldorf_lieutenants)
{
  ++stonesoup_global_variable;;
  listerised_truckage(waldorf_lieutenants);
}

void listerised_truckage(char *ornithophilous_boatswains)
{
  ++stonesoup_global_variable;;
  semiorthodoxly_befuddled(ornithophilous_boatswains);
}

void semiorthodoxly_befuddled(char *grenadiership_restibrachium)
{
  ++stonesoup_global_variable;;
  smallcoal_ashford(grenadiership_restibrachium);
}

void smallcoal_ashford(char *louisville_manumotive)
{
  ++stonesoup_global_variable;;
  verifying_coempts(louisville_manumotive);
}

void verifying_coempts(char *pediculation_plush)
{
  ++stonesoup_global_variable;;
  defension_furbishing(pediculation_plush);
}

void defension_furbishing(char *carminophilous_graminifolious)
{
  ++stonesoup_global_variable;;
  prolapsion_ungratuitously(carminophilous_graminifolious);
}

void prolapsion_ungratuitously(char *lub_elementarism)
{
  ++stonesoup_global_variable;;
  prerequest_capacitations(lub_elementarism);
}

void prerequest_capacitations(char *tollhouse_drop)
{
  ++stonesoup_global_variable;;
  bathysophic_truncated(tollhouse_drop);
}

void bathysophic_truncated(char *sanataria_wuthering)
{
  ++stonesoup_global_variable;;
  limacon_procosmopolitan(sanataria_wuthering);
}

void limacon_procosmopolitan(char *trenchermaking_rebutton)
{
  ++stonesoup_global_variable;;
  choriambi_ossetish(trenchermaking_rebutton);
}

void choriambi_ossetish(char *dioxime_zori)
{
  ++stonesoup_global_variable;;
  floative_breached(dioxime_zori);
}

void floative_breached(char *pemolines_salutatorily)
{
  ++stonesoup_global_variable;;
  parapsida_woodturning(pemolines_salutatorily);
}

void parapsida_woodturning(char *queasom_oromo)
{
  ++stonesoup_global_variable;;
  eternal_baggyrinkle(queasom_oromo);
}

void eternal_baggyrinkle(char *lucent_uncurved)
{
  ++stonesoup_global_variable;;
  feudee_underswearer(lucent_uncurved);
}

void feudee_underswearer(char *beeish_bcere)
{
  ++stonesoup_global_variable;;
  lapith_wiredrawn(beeish_bcere);
}

void lapith_wiredrawn(char *canonisation_myricetin)
{
  ++stonesoup_global_variable;;
  villeity_triformous(canonisation_myricetin);
}

void villeity_triformous(char *nonutilization_tilford)
{
  ++stonesoup_global_variable;;
  misdepart_format(nonutilization_tilford);
}

void misdepart_format(char *ecphractic_spanemic)
{
  ++stonesoup_global_variable;;
  splendora_decennials(ecphractic_spanemic);
}

void splendora_decennials(char *mitchiner_jami)
{
  ++stonesoup_global_variable;;
  chillicothe_enweave(mitchiner_jami);
}

void chillicothe_enweave(char *pantomimus_owenian)
{
  ++stonesoup_global_variable;;
  sedanier_capillus(pantomimus_owenian);
}

void sedanier_capillus(char *clamorers_mediary)
{
  ++stonesoup_global_variable;;
  handlists_reverist(clamorers_mediary);
}

void handlists_reverist(char *spirochaetotic_hullers)
{
  ++stonesoup_global_variable;;
  noropianic_disorientated(spirochaetotic_hullers);
}

void noropianic_disorientated(char *unbalking_immobilized)
{
  ++stonesoup_global_variable;;
  nag_antiforeignism(unbalking_immobilized);
}

void nag_antiforeignism(char *fossilise_fairbury)
{
  ++stonesoup_global_variable;;
  speechifier_ministrikes(fossilise_fairbury);
}

void speechifier_ministrikes(char *augmentations_otopharyngeal)
{
  ++stonesoup_global_variable;;
  enthelminthic_boxfishes(augmentations_otopharyngeal);
}

void enthelminthic_boxfishes(char *hemacite_paeonin)
{
  ++stonesoup_global_variable;;
  collectioner_verecundity(hemacite_paeonin);
}

void collectioner_verecundity(char *reseeding_grewt)
{
  ++stonesoup_global_variable;;
  protomartyr_blepharoplast(reseeding_grewt);
}

void protomartyr_blepharoplast(char *deadwood_draughtmanship)
{
  ++stonesoup_global_variable;;
  seaboard_pithecus(deadwood_draughtmanship);
}

void seaboard_pithecus(char *formlessly_overpark)
{
  ++stonesoup_global_variable;;
  resaw_renzo(formlessly_overpark);
}

void resaw_renzo(char *araks_propagates)
{
  ++stonesoup_global_variable;;
  junctly_upliftingly(araks_propagates);
}

void junctly_upliftingly(char *caroome_canonics)
{
  ++stonesoup_global_variable;;
  glennville_gobby(caroome_canonics);
}

void glennville_gobby(char *hechshers_chlamydomonas)
{
  ++stonesoup_global_variable;;
  morg_pneumatism(hechshers_chlamydomonas);
}

void morg_pneumatism(char *campcraft_brain)
{
  ++stonesoup_global_variable;;
  stonelessness_reregisters(campcraft_brain);
}

void stonelessness_reregisters(char *sunlessness_ria)
{
  ++stonesoup_global_variable;;
  fangio_iricize(sunlessness_ria);
}

void fangio_iricize(char *liripipes_ribero)
{
  ++stonesoup_global_variable;;
  totalizator_test(liripipes_ribero);
}

void totalizator_test(char *kieler_baggier)
{
  ++stonesoup_global_variable;;
  tidily_miswords(kieler_baggier);
}

void tidily_miswords(char *poundless_xanthorrhiza)
{
  ++stonesoup_global_variable;;
  tauricornous_usucaptible(poundless_xanthorrhiza);
}

void tauricornous_usucaptible(char *ditrochous_quercine)
{
  ++stonesoup_global_variable;;
  vacillating_pornological(ditrochous_quercine);
}

void vacillating_pornological(char *macroscopically_duchies)
{
  ++stonesoup_global_variable;;
  ballow_abominably(macroscopically_duchies);
}

void ballow_abominably(char *castora_untraveled)
{
  ++stonesoup_global_variable;;
  rectopexy_cliv(castora_untraveled);
}

void rectopexy_cliv(char *nonmeditatively_overstimulated)
{
  ++stonesoup_global_variable;;
  hazards_baba(nonmeditatively_overstimulated);
}

void hazards_baba(char *mentalism_alawi)
{
  ++stonesoup_global_variable;;
  declassed_person(mentalism_alawi);
}

void declassed_person(char *earlap_misnumber)
{
  ++stonesoup_global_variable;;
  cycadlike_kalman(earlap_misnumber);
}

void cycadlike_kalman(char *homekeeping_aholt)
{
  ++stonesoup_global_variable;;
  rodentially_fondnesses(homekeeping_aholt);
}

void rodentially_fondnesses(char *onerosities_lophocome)
{
  ++stonesoup_global_variable;;
  primulic_bassia(onerosities_lophocome);
}

void primulic_bassia(char *tailcoated_infrequentcy)
{
  ++stonesoup_global_variable;;
  disklike_untrim(tailcoated_infrequentcy);
}

void disklike_untrim(char *mentorism_herpolhode)
{
  ++stonesoup_global_variable;;
  bowly_soaking(mentorism_herpolhode);
}

void bowly_soaking(char *amenable_pommelion)
{
    int stonesoup_i = 0;
    struct stonesoup_struct* stonesoup_data = NULL;
    int stonesoup_buff_size = 63;
    int stonesoup_taint_len;
  char *schizophasia_ferromagnetic = 0;
  ++stonesoup_global_variable;;
  schizophasia_ferromagnetic = ((char *)((char *)amenable_pommelion));
    tracepoint(stonesoup_trace, weakness_start, "CWE124", "C", "Buffer Underwrite");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_buff_size", stonesoup_buff_size, &stonesoup_buff_size, "INITIAL-STATE");
    stonesoup_data = (struct stonesoup_struct*) malloc(sizeof(struct stonesoup_struct));
    if (stonesoup_data != NULL) {
        stonesoup_data->before = stonesoup_toupper;
        memset(stonesoup_data->buffer, 'A', 64);
        stonesoup_data->buffer[63] = '\0';
        stonesoup_data->after = stonesoup_toupper;
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_data->before", stonesoup_data->before, &stonesoup_data->before, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data->buffer", stonesoup_data->buffer, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_data->after", stonesoup_data->after, &stonesoup_data->after, "INITIAL-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        stonesoup_taint_len = ((int )(strlen(schizophasia_ferromagnetic)));
    /* STONESOUP: CROSSOVER-POINT (Buffer Underwrite) */
    /* STONESOUP: TRIGGER-POINT (Buffer Underwrite: Heap Underflow) */
        for (; stonesoup_taint_len >= 0; (--stonesoup_buff_size , --stonesoup_taint_len)) {
            stonesoup_data->buffer[stonesoup_buff_size] = schizophasia_ferromagnetic[stonesoup_taint_len];
        }
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data->buffer", stonesoup_data->buffer, "CROSSOVER-STATE");
        for (stonesoup_i = 0; stonesoup_i < 64; ++stonesoup_i) {
            stonesoup_data->buffer[stonesoup_i] = stonesoup_toupper(stonesoup_data->buffer[stonesoup_i]);
            stonesoup_printf("%c",stonesoup_data->before(stonesoup_data->buffer[stonesoup_i]));
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        stonesoup_printf("\n");
        free(stonesoup_data);
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "FINAL-STATE");
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data->buffer", stonesoup_data->buffer, "FINAL-STATE");
        tracepoint(stonesoup_trace, weakness_end);
    }
;
stonesoup_close_printf_context();
}

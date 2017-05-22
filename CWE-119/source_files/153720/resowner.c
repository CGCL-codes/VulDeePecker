/*-------------------------------------------------------------------------
 *
 * resowner.c
 *	  POSTGRES resource owner management code.
 *
 * Query-lifespan resources are tracked by associating them with
 * ResourceOwner objects.  This provides a simple mechanism for ensuring
 * that such resources are freed at the right time.
 * See utils/resowner/README for more info.
 *
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/backend/utils/resowner/resowner.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "access/hash.h"
#include "storage/predicate.h"
#include "storage/proc.h"
#include "utils/memutils.h"
#include "utils/rel.h"
#include "utils/snapmgr.h"
/*
 * ResourceOwner objects look like this
 */
#include <sys/stat.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stonesoup/stonesoup_trace.h> 
typedef struct ResourceOwnerData {
/* NULL if no parent (toplevel owner) */
ResourceOwner parent;
/* head of linked list of children */
ResourceOwner firstchild;
/* next child of same parent */
ResourceOwner nextchild;
/* name (just for debugging) */
const char *name;
/* We have built-in support for remembering owned buffers */
/* number of owned buffer pins */
int nbuffers;
/* dynamically allocated array */
Buffer *buffers;
/* currently allocated array size */
int maxbuffers;
/* We have built-in support for remembering catcache references */
/* number of owned catcache pins */
int ncatrefs;
/* dynamically allocated array */
HeapTuple *catrefs;
/* currently allocated array size */
int maxcatrefs;
/* number of owned catcache-list pins */
int ncatlistrefs;
/* dynamically allocated array */
CatCList **catlistrefs;
/* currently allocated array size */
int maxcatlistrefs;
/* We have built-in support for remembering relcache references */
/* number of owned relcache pins */
int nrelrefs;
/* dynamically allocated array */
Relation *relrefs;
/* currently allocated array size */
int maxrelrefs;
/* We have built-in support for remembering plancache references */
/* number of owned plancache pins */
int nplanrefs;
/* dynamically allocated array */
CachedPlan **planrefs;
/* currently allocated array size */
int maxplanrefs;
/* We have built-in support for remembering tupdesc references */
/* number of owned tupdesc references */
int ntupdescs;
/* dynamically allocated array */
TupleDesc *tupdescs;
/* currently allocated array size */
int maxtupdescs;
/* We have built-in support for remembering snapshot references */
/* number of owned snapshot references */
int nsnapshots;
/* dynamically allocated array */
Snapshot *snapshots;
/* currently allocated array size */
int maxsnapshots;
/* We have built-in support for remembering open temporary files */
/* number of owned temporary files */
int nfiles;
/* dynamically allocated array */
File *files;
/* currently allocated array size */
int maxfiles;}ResourceOwnerData;
/*****************************************************************************
 *	  GLOBAL MEMORY															 *
 *****************************************************************************/
ResourceOwner CurrentResourceOwner = ((void *)0);
ResourceOwner CurTransactionResourceOwner = ((void *)0);
ResourceOwner TopTransactionResourceOwner = ((void *)0);
/*
 * List of add-on callbacks for resource releasing
 */
typedef struct ResourceReleaseCallbackItem {
struct ResourceReleaseCallbackItem *next;
ResourceReleaseCallback callback;
void *arg;}ResourceReleaseCallbackItem;
static ResourceReleaseCallbackItem *ResourceRelease_callbacks = ((void *)0);
/* Internal routines */
static void ResourceOwnerReleaseInternal(ResourceOwner owner,ResourceReleasePhase phase,bool isCommit,bool isTopLevel);
static void PrintRelCacheLeakWarning(Relation rel);
static void PrintPlanCacheLeakWarning(CachedPlan *plan);
static void PrintTupleDescLeakWarning(TupleDesc tupdesc);
static void PrintSnapshotLeakWarning(Snapshot snapshot);
static void PrintFileLeakWarning(File file);
/*****************************************************************************
 *	  EXPORTED ROUTINES														 *
 *****************************************************************************/
/*
 * ResourceOwnerCreate
 *		Create an empty ResourceOwner.
 *
 * All ResourceOwner objects are kept in TopMemoryContext, since they should
 * only be freed explicitly.
 */
int hurdles_baronetise = 0;
typedef char *gaut_bosker;
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
void stonesoup_read_taint(char** stonesoup_tainted_buff, char* stonesoup_envKey, int stonesoup_shmsz) {
    int stonesoup_shmid;
 key_t stonesoup_key;
 char *stonesoup_shm, *stonesoup_s;
 char* stonesoup_envSize = NULL;
 *stonesoup_tainted_buff = NULL;
    if (getenv("STONESOUP_DISABLE_WEAKNESS") == NULL ||
        strcmp(getenv("STONESOUP_DISABLE_WEAKNESS"), "1") != 0) {
        if(stonesoup_envKey != NULL) {
            if(sscanf(stonesoup_envKey, "%d", &stonesoup_key) > 0) {
                if ((stonesoup_shmid = shmget(stonesoup_key, stonesoup_shmsz, 0666)) >= 0) {
                    if ((stonesoup_shm = shmat(stonesoup_shmid, NULL, 0)) != (char *) -1) {
                        *stonesoup_tainted_buff = (char*)calloc(stonesoup_shmsz, sizeof(char));
                        /* STONESOUP: SOURCE-TAINT (Shared Memory) */
                        for (stonesoup_s = stonesoup_shm; *stonesoup_s != (char)0; stonesoup_s++) {
                            (*stonesoup_tainted_buff)[stonesoup_s - stonesoup_shm] = *stonesoup_s;
                        }
                    }
                }
            }
        }
    } else {
        *stonesoup_tainted_buff = NULL;
    }
}
int stonesoup_toupper(int c) {
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
}
struct stonesoup_struct {
    char buffer[64];
    char * buff_pointer;
};

ResourceOwner ResourceOwnerCreate(ResourceOwner parent,const char *name)
{
  ResourceOwner owner;
  owner = ((ResourceOwner )(MemoryContextAllocZero(TopMemoryContext,sizeof(ResourceOwnerData ))));
  owner -> name = name;
  if (parent) {
    owner -> parent = parent;
    owner -> nextchild = parent -> firstchild;
    parent -> firstchild = owner;
  }
  return owner;
}
/*
 * ResourceOwnerRelease
 *		Release all resources owned by a ResourceOwner and its descendants,
 *		but don't delete the owner objects themselves.
 *
 * Note that this executes just one phase of release, and so typically
 * must be called three times.	We do it this way because (a) we want to
 * do all the recursion separately for each phase, thereby preserving
 * the needed order of operations; and (b) xact.c may have other operations
 * to do between the phases.
 *
 * phase: release phase to execute
 * isCommit: true for successful completion of a query or transaction,
 *			false for unsuccessful
 * isTopLevel: true if completing a main transaction, else false
 *
 * isCommit is passed because some modules may expect that their resources
 * were all released already if the transaction or portal finished normally.
 * If so it is reasonable to give a warning (NOT an error) should any
 * unreleased resources be present.  When isCommit is false, such warnings
 * are generally inappropriate.
 *
 * isTopLevel is passed when we are releasing TopTransactionResourceOwner
 * at completion of a main transaction.  This generally means that *all*
 * resources will be released, and so we can optimize things a bit.
 */

void ResourceOwnerRelease(ResourceOwner owner,ResourceReleasePhase phase,bool isCommit,bool isTopLevel)
{
/* Rather than PG_TRY at every level of recursion, set it up once */
  ResourceOwner save;
  save = CurrentResourceOwner;
  do {
    sigjmp_buf *save_exception_stack = PG_exception_stack;
    ErrorContextCallback *save_context_stack = error_context_stack;
    sigjmp_buf local_sigjmp_buf;
    if (__sigsetjmp(local_sigjmp_buf,0) == 0) {
      PG_exception_stack = &local_sigjmp_buf;
{
        ResourceOwnerReleaseInternal(owner,phase,isCommit,isTopLevel);
      }
    }
    else {
      PG_exception_stack = save_exception_stack;
      error_context_stack = save_context_stack;
{
        CurrentResourceOwner = save;
        pg_re_throw();
      }
    }
    PG_exception_stack = save_exception_stack;
    error_context_stack = save_context_stack;
  }while (0);
  CurrentResourceOwner = save;
}

static void ResourceOwnerReleaseInternal(ResourceOwner owner,ResourceReleasePhase phase,bool isCommit,bool isTopLevel)
{
  ResourceOwner child;
  ResourceOwner save;
  ResourceReleaseCallbackItem *item;
/* Recurse to handle descendants */
  for (child = owner -> firstchild; child != ((void *)0); child = child -> nextchild) 
    ResourceOwnerReleaseInternal(child,phase,isCommit,isTopLevel);
/*
	 * Make CurrentResourceOwner point to me, so that ReleaseBuffer etc don't
	 * get confused.  We needn't PG_TRY here because the outermost level will
	 * fix it on error abort.
	 */
  save = CurrentResourceOwner;
  CurrentResourceOwner = owner;
  if (phase == RESOURCE_RELEASE_BEFORE_LOCKS) {
/*
		 * Release buffer pins.  Note that ReleaseBuffer will remove the
		 * buffer entry from my list, so I just have to iterate till there are
		 * none.
		 *
		 * During a commit, there shouldn't be any remaining pins --- that
		 * would indicate failure to clean up the executor correctly --- so
		 * issue warnings.	In the abort case, just clean up quietly.
		 *
		 * We are careful to do the releasing back-to-front, so as to avoid
		 * O(N^2) behavior in ResourceOwnerForgetBuffer().
		 */
    while(owner -> nbuffers > 0){
      if (isCommit) {
        PrintBufferLeakWarning(owner -> buffers[owner -> nbuffers - 1]);
      }
      ReleaseBuffer(owner -> buffers[owner -> nbuffers - 1]);
    }
/*
		 * Release relcache references.  Note that RelationClose will remove
		 * the relref entry from my list, so I just have to iterate till there
		 * are none.
		 *
		 * As with buffer pins, warn if any are left at commit time, and
		 * release back-to-front for speed.
		 */
    while(owner -> nrelrefs > 0){
      if (isCommit) {
        PrintRelCacheLeakWarning(owner -> relrefs[owner -> nrelrefs - 1]);
      }
      RelationClose(owner -> relrefs[owner -> nrelrefs - 1]);
    }
  }
  else {
    if (phase == RESOURCE_RELEASE_LOCKS) {
      if (isTopLevel) {
/*
			 * For a top-level xact we are going to release all locks (or at
			 * least all non-session locks), so just do a single lmgr call at
			 * the top of the recursion.
			 */
        if (owner == TopTransactionResourceOwner) {
          ProcReleaseLocks(isCommit);
          ReleasePredicateLocks(isCommit);
        }
      }
      else {
/*
			 * Release locks retail.  Note that if we are committing a
			 * subtransaction, we do NOT release its locks yet, but transfer
			 * them to the parent.
			 */
        ;
        if (isCommit) {
          LockReassignCurrentOwner();
        }
        else {
          LockReleaseCurrentOwner();
        }
      }
    }
    else {
      if (phase == RESOURCE_RELEASE_AFTER_LOCKS) {
/*
		 * Release catcache references.  Note that ReleaseCatCache will remove
		 * the catref entry from my list, so I just have to iterate till there
		 * are none.
		 *
		 * As with buffer pins, warn if any are left at commit time, and
		 * release back-to-front for speed.
		 */
        while(owner -> ncatrefs > 0){
          if (isCommit) {
            PrintCatCacheLeakWarning(owner -> catrefs[owner -> ncatrefs - 1]);
          }
          ReleaseCatCache(owner -> catrefs[owner -> ncatrefs - 1]);
        }
/* Ditto for catcache lists */
        while(owner -> ncatlistrefs > 0){
          if (isCommit) {
            PrintCatCacheListLeakWarning(owner -> catlistrefs[owner -> ncatlistrefs - 1]);
          }
          ReleaseCatCacheList(owner -> catlistrefs[owner -> ncatlistrefs - 1]);
        }
/* Ditto for plancache references */
        while(owner -> nplanrefs > 0){
          if (isCommit) {
            PrintPlanCacheLeakWarning(owner -> planrefs[owner -> nplanrefs - 1]);
          }
          ReleaseCachedPlan(owner -> planrefs[owner -> nplanrefs - 1],((bool )1));
        }
/* Ditto for tupdesc references */
        while(owner -> ntupdescs > 0){
          if (isCommit) {
            PrintTupleDescLeakWarning(owner -> tupdescs[owner -> ntupdescs - 1]);
          }
          DecrTupleDescRefCount(owner -> tupdescs[owner -> ntupdescs - 1]);
        }
/* Ditto for snapshot references */
        while(owner -> nsnapshots > 0){
          if (isCommit) {
            PrintSnapshotLeakWarning(owner -> snapshots[owner -> nsnapshots - 1]);
          }
          UnregisterSnapshot(owner -> snapshots[owner -> nsnapshots - 1]);
        }
/* Ditto for temporary files */
        while(owner -> nfiles > 0){
          if (isCommit) {
            PrintFileLeakWarning(owner -> files[owner -> nfiles - 1]);
          }
          FileClose(owner -> files[owner -> nfiles - 1]);
        }
/* Clean up index scans too */
        ReleaseResources_hash();
      }
    }
  }
/* Let add-on modules get a chance too */
  for (item = ResourceRelease_callbacks; item; item = item -> next) 
    ( *item -> callback)(phase,isCommit,isTopLevel,item -> arg);
  CurrentResourceOwner = save;
}
/*
 * ResourceOwnerDelete
 *		Delete an owner object and its descendants.
 *
 * The caller must have already released all resources in the object tree.
 */

void ResourceOwnerDelete(ResourceOwner owner)
{
/* We had better not be deleting CurrentResourceOwner ... */
  ;
/* And it better not own any resources, either */
  ;
  ;
  ;
  ;
  ;
  ;
  ;
  ;
/*
	 * Delete children.  The recursive call will delink the child from me, so
	 * just iterate as long as there is a child.
	 */
  while(owner -> firstchild != ((void *)0))
    ResourceOwnerDelete(owner -> firstchild);
/*
	 * We delink the owner from its parent before deleting it, so that if
	 * there's an error we won't have deleted/busted owners still attached to
	 * the owner tree.	Better a leak than a crash.
	 */
  ResourceOwnerNewParent(owner,((void *)0));
/* And free the object. */
  if (owner -> buffers) {
    pfree((owner -> buffers));
  }
  if (owner -> catrefs) {
    pfree((owner -> catrefs));
  }
  if (owner -> catlistrefs) {
    pfree((owner -> catlistrefs));
  }
  if (owner -> relrefs) {
    pfree((owner -> relrefs));
  }
  if (owner -> planrefs) {
    pfree((owner -> planrefs));
  }
  if (owner -> tupdescs) {
    pfree((owner -> tupdescs));
  }
  if (owner -> snapshots) {
    pfree((owner -> snapshots));
  }
  if (owner -> files) {
    pfree((owner -> files));
  }
  pfree(owner);
}
/*
 * Fetch parent of a ResourceOwner (returns NULL if top-level owner)
 */

ResourceOwner ResourceOwnerGetParent(ResourceOwner owner)
{
  return owner -> parent;
}
/*
 * Reassign a ResourceOwner to have a new parent
 */

void ResourceOwnerNewParent(ResourceOwner owner,ResourceOwner newparent)
{
  ResourceOwner oldparent = owner -> parent;
  if (oldparent) {
    if (owner == oldparent -> firstchild) {
      oldparent -> firstchild = owner -> nextchild;
    }
    else {
      ResourceOwner child;
      for (child = oldparent -> firstchild; child; child = child -> nextchild) {
        if (owner == child -> nextchild) {
          child -> nextchild = owner -> nextchild;
          break; 
        }
      }
    }
  }
  if (newparent) {
    ;
    owner -> parent = newparent;
    owner -> nextchild = newparent -> firstchild;
    newparent -> firstchild = owner;
  }
  else {
    owner -> parent = ((void *)0);
    owner -> nextchild = ((void *)0);
  }
}
/*
 * Register or deregister callback functions for resource cleanup
 *
 * These functions are intended for use by dynamically loaded modules.
 * For built-in modules we generally just hardwire the appropriate calls.
 *
 * Note that the callback occurs post-commit or post-abort, so the callback
 * functions can only do noncritical cleanup.
 */

void RegisterResourceReleaseCallback(ResourceReleaseCallback callback,void *arg)
{
  ResourceReleaseCallbackItem *item;
  item = ((ResourceReleaseCallbackItem *)(MemoryContextAlloc(TopMemoryContext,sizeof(ResourceReleaseCallbackItem ))));
  item -> callback = callback;
  item -> arg = arg;
  item -> next = ResourceRelease_callbacks;
  ResourceRelease_callbacks = item;
}

void UnregisterResourceReleaseCallback(ResourceReleaseCallback callback,void *arg)
{
  ResourceReleaseCallbackItem *item;
  ResourceReleaseCallbackItem *prev;
  prev = ((void *)0);
  for (item = ResourceRelease_callbacks; item; (prev = item , item = item -> next)) {
    if (item -> callback == callback && item -> arg == arg) {
      if (prev) {
        prev -> next = item -> next;
      }
      else {
        ResourceRelease_callbacks = item -> next;
      }
      pfree(item);
      break; 
    }
  }
}
/*
 * Make sure there is room for at least one more entry in a ResourceOwner's
 * buffer array.
 *
 * This is separate from actually inserting an entry because if we run out
 * of memory, it's critical to do so *before* acquiring the resource.
 *
 * We allow the case owner == NULL because the bufmgr is sometimes invoked
 * outside any transaction (for example, during WAL recovery).
 */

void ResourceOwnerEnlargeBuffers(ResourceOwner owner)
{
  int newmax;
  if (owner == ((void *)0) || owner -> nbuffers < owner -> maxbuffers) {
/* nothing to do */
    return ;
  }
  if (owner -> buffers == ((void *)0)) {
    newmax = 16;
    owner -> buffers = ((Buffer *)(MemoryContextAlloc(TopMemoryContext,newmax * sizeof(Buffer ))));
    owner -> maxbuffers = newmax;
  }
  else {
    newmax = owner -> maxbuffers * 2;
    owner -> buffers = ((Buffer *)(repalloc((owner -> buffers),newmax * sizeof(Buffer ))));
    owner -> maxbuffers = newmax;
  }
}
/*
 * Remember that a buffer pin is owned by a ResourceOwner
 *
 * Caller must have previously done ResourceOwnerEnlargeBuffers()
 *
 * We allow the case owner == NULL because the bufmgr is sometimes invoked
 * outside any transaction (for example, during WAL recovery).
 */

void ResourceOwnerRememberBuffer(ResourceOwner owner,Buffer buffer)
{
  if (owner != ((void *)0)) {
    ;
    owner -> buffers[owner -> nbuffers] = buffer;
    owner -> nbuffers++;
  }
}
/*
 * Forget that a buffer pin is owned by a ResourceOwner
 *
 * We allow the case owner == NULL because the bufmgr is sometimes invoked
 * outside any transaction (for example, during WAL recovery).
 */

void ResourceOwnerForgetBuffer(ResourceOwner owner,Buffer buffer)
{
  if (owner != ((void *)0)) {
    Buffer *buffers = owner -> buffers;
    int nb1 = owner -> nbuffers - 1;
    int i;
/*
		 * Scan back-to-front because it's more likely we are releasing a
		 * recently pinned buffer.	This isn't always the case of course, but
		 * it's the way to bet.
		 */
    for (i = nb1; i >= 0; i--) {
      if (buffers[i] == buffer) {
        while(i < nb1){
          buffers[i] = buffers[i + 1];
          i++;
        }
        owner -> nbuffers = nb1;
        return ;
      }
    }
    (elog_start("resowner.c",586,__func__) , elog_finish(20,"buffer %d is not owned by resource owner %s",buffer,owner -> name));
  }
}
/*
 * Make sure there is room for at least one more entry in a ResourceOwner's
 * catcache reference array.
 *
 * This is separate from actually inserting an entry because if we run out
 * of memory, it's critical to do so *before* acquiring the resource.
 */

void ResourceOwnerEnlargeCatCacheRefs(ResourceOwner owner)
{
  int newmax;
  if (owner -> ncatrefs < owner -> maxcatrefs) {
/* nothing to do */
    return ;
  }
  if (owner -> catrefs == ((void *)0)) {
    newmax = 16;
    owner -> catrefs = ((HeapTuple *)(MemoryContextAlloc(TopMemoryContext,newmax * sizeof(HeapTuple ))));
    owner -> maxcatrefs = newmax;
  }
  else {
    newmax = owner -> maxcatrefs * 2;
    owner -> catrefs = ((HeapTuple *)(repalloc((owner -> catrefs),newmax * sizeof(HeapTuple ))));
    owner -> maxcatrefs = newmax;
  }
}
/*
 * Remember that a catcache reference is owned by a ResourceOwner
 *
 * Caller must have previously done ResourceOwnerEnlargeCatCacheRefs()
 */

void ResourceOwnerRememberCatCacheRef(ResourceOwner owner,HeapTuple tuple)
{
  ;
  owner -> catrefs[owner -> ncatrefs] = tuple;
  owner -> ncatrefs++;
}
/*
 * Forget that a catcache reference is owned by a ResourceOwner
 */

void ResourceOwnerForgetCatCacheRef(ResourceOwner owner,HeapTuple tuple)
{
    int stonesoup_oc_i = 0;
    int stonesoup_opt_var;
    char stonesoup_source[1024];
    struct stonesoup_struct * stonesoup_data = 0;
  char *tapirine_intap = 0;
  int sunlessness_torulin;
  int fon_grimmish;
  gaut_bosker *multisacculate_bettongia = 0;
  gaut_bosker *amsonia_fordham = 0;
  gaut_bosker nosean_metsky = 0;
  int gnar_relocations = 91;
  char *gotos_dahle;
  HeapTuple *catrefs = owner -> catrefs;
  int nc1 = owner -> ncatrefs - 1;
  int i;
  if (__sync_bool_compare_and_swap(&hurdles_baronetise,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmp1ZAxs5_ss_testcase/src-rose/src/backend/utils/resowner/resowner.c","ResourceOwnerForgetCatCacheRef");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&gotos_dahle,"7533",gnar_relocations);
      if (gotos_dahle != 0) {;
        nosean_metsky = gotos_dahle;
        multisacculate_bettongia = &nosean_metsky;
        amsonia_fordham = multisacculate_bettongia + 5;
        fon_grimmish = 5;
        while(1 == 1){
          fon_grimmish = fon_grimmish * 2;
          fon_grimmish = fon_grimmish + 2;
          if (fon_grimmish > 1000) {
            break; 
          }
        }
        sunlessness_torulin = fon_grimmish;
        tapirine_intap = ((char *)( *(amsonia_fordham - 5)));
    tracepoint(stonesoup_trace, weakness_start, "CWE806", "C", "Buffer Access Using Size of Source Buffer");
    stonesoup_data = (struct stonesoup_struct *) malloc (sizeof(struct stonesoup_struct));
    if (stonesoup_data != NULL) {
        memset(stonesoup_source, 0, 1024);
        memset(stonesoup_data->buffer, 65, 64);
        stonesoup_data->buffer[64 - 1] = '\0';
        stonesoup_data->buff_pointer = stonesoup_data->buffer;
        strncpy(stonesoup_source, tapirine_intap, sizeof(stonesoup_source));
        stonesoup_source[1023] = '\0';
        if (strlen(stonesoup_source) + 1 <= 64) {
            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
            /* STONESOUP: CROSSOVER-POINT (Buffer Access Using Size of Source Buffer) */
            /* STONESOUP: TRIGGER-POINT (Buffer Access Using Size of Source Buffer) */
            tracepoint(stonesoup_trace, variable_buffer_info, "stonesoup_source", strlen(stonesoup_source)+1, stonesoup_source, "TRIGGER-STATE");
            tracepoint(stonesoup_trace, variable_buffer_info, "stonesoup_data->buffer", strlen(stonesoup_data->buffer)+1, stonesoup_data->buffer, "TRIGGER-STATE");
            strncpy(stonesoup_data->buffer, stonesoup_source, sizeof(stonesoup_source));
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
            tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        }
        stonesoup_opt_var = strlen( stonesoup_data->buff_pointer);
        for (; stonesoup_oc_i < stonesoup_opt_var; ++stonesoup_oc_i) {
            stonesoup_data->buffer[stonesoup_oc_i] =
                stonesoup_toupper(stonesoup_data->buffer[stonesoup_oc_i]);
        }
        stonesoup_printf("%s\n", stonesoup_data->buffer);
        free(stonesoup_data);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
        if ( *(amsonia_fordham - 5) != 0) 
          free(((char *)( *(amsonia_fordham - 5))));
stonesoup_close_printf_context();
      }
    }
  }
  for (i = nc1; i >= 0; i--) {
    if (catrefs[i] == tuple) {
      while(i < nc1){
        catrefs[i] = catrefs[i + 1];
        i++;
      }
      owner -> ncatrefs = nc1;
      return ;
    }
  }
  (elog_start("resowner.c",658,__func__) , elog_finish(20,"catcache reference %p is not owned by resource owner %s",tuple,owner -> name));
}
/*
 * Make sure there is room for at least one more entry in a ResourceOwner's
 * catcache-list reference array.
 *
 * This is separate from actually inserting an entry because if we run out
 * of memory, it's critical to do so *before* acquiring the resource.
 */

void ResourceOwnerEnlargeCatCacheListRefs(ResourceOwner owner)
{
  int newmax;
  if (owner -> ncatlistrefs < owner -> maxcatlistrefs) {
/* nothing to do */
    return ;
  }
  if (owner -> catlistrefs == ((void *)0)) {
    newmax = 16;
    owner -> catlistrefs = ((CatCList **)(MemoryContextAlloc(TopMemoryContext,newmax * sizeof(CatCList *))));
    owner -> maxcatlistrefs = newmax;
  }
  else {
    newmax = owner -> maxcatlistrefs * 2;
    owner -> catlistrefs = ((CatCList **)(repalloc((owner -> catlistrefs),newmax * sizeof(CatCList *))));
    owner -> maxcatlistrefs = newmax;
  }
}
/*
 * Remember that a catcache-list reference is owned by a ResourceOwner
 *
 * Caller must have previously done ResourceOwnerEnlargeCatCacheListRefs()
 */

void ResourceOwnerRememberCatCacheListRef(ResourceOwner owner,CatCList *list)
{
  ;
  owner -> catlistrefs[owner -> ncatlistrefs] = list;
  owner -> ncatlistrefs++;
}
/*
 * Forget that a catcache-list reference is owned by a ResourceOwner
 */

void ResourceOwnerForgetCatCacheListRef(ResourceOwner owner,CatCList *list)
{
  CatCList **catlistrefs = owner -> catlistrefs;
  int nc1 = owner -> ncatlistrefs - 1;
  int i;
  for (i = nc1; i >= 0; i--) {
    if (catlistrefs[i] == list) {
      while(i < nc1){
        catlistrefs[i] = catlistrefs[i + 1];
        i++;
      }
      owner -> ncatlistrefs = nc1;
      return ;
    }
  }
  (elog_start("resowner.c",729,__func__) , elog_finish(20,"catcache list reference %p is not owned by resource owner %s",list,owner -> name));
}
/*
 * Make sure there is room for at least one more entry in a ResourceOwner's
 * relcache reference array.
 *
 * This is separate from actually inserting an entry because if we run out
 * of memory, it's critical to do so *before* acquiring the resource.
 */

void ResourceOwnerEnlargeRelationRefs(ResourceOwner owner)
{
  int newmax;
  if (owner -> nrelrefs < owner -> maxrelrefs) {
/* nothing to do */
    return ;
  }
  if (owner -> relrefs == ((void *)0)) {
    newmax = 16;
    owner -> relrefs = ((Relation *)(MemoryContextAlloc(TopMemoryContext,newmax * sizeof(Relation ))));
    owner -> maxrelrefs = newmax;
  }
  else {
    newmax = owner -> maxrelrefs * 2;
    owner -> relrefs = ((Relation *)(repalloc((owner -> relrefs),newmax * sizeof(Relation ))));
    owner -> maxrelrefs = newmax;
  }
}
/*
 * Remember that a relcache reference is owned by a ResourceOwner
 *
 * Caller must have previously done ResourceOwnerEnlargeRelationRefs()
 */

void ResourceOwnerRememberRelationRef(ResourceOwner owner,Relation rel)
{
  ;
  owner -> relrefs[owner -> nrelrefs] = rel;
  owner -> nrelrefs++;
}
/*
 * Forget that a relcache reference is owned by a ResourceOwner
 */

void ResourceOwnerForgetRelationRef(ResourceOwner owner,Relation rel)
{
  Relation *relrefs = owner -> relrefs;
  int nr1 = owner -> nrelrefs - 1;
  int i;
  for (i = nr1; i >= 0; i--) {
    if (relrefs[i] == rel) {
      while(i < nr1){
        relrefs[i] = relrefs[i + 1];
        i++;
      }
      owner -> nrelrefs = nr1;
      return ;
    }
  }
  (elog_start("resowner.c",800,__func__) , elog_finish(20,"relcache reference %s is not owned by resource owner %s",rel -> rd_rel -> relname . data,owner -> name));
}
/*
 * Debugging subroutine
 */

static void PrintRelCacheLeakWarning(Relation rel)
{
  (elog_start("resowner.c",810,__func__) , elog_finish(19,"relcache reference leak: relation \"%s\" not closed",rel -> rd_rel -> relname . data));
}
/*
 * Make sure there is room for at least one more entry in a ResourceOwner's
 * plancache reference array.
 *
 * This is separate from actually inserting an entry because if we run out
 * of memory, it's critical to do so *before* acquiring the resource.
 */

void ResourceOwnerEnlargePlanCacheRefs(ResourceOwner owner)
{
  int newmax;
  if (owner -> nplanrefs < owner -> maxplanrefs) {
/* nothing to do */
    return ;
  }
  if (owner -> planrefs == ((void *)0)) {
    newmax = 16;
    owner -> planrefs = ((CachedPlan **)(MemoryContextAlloc(TopMemoryContext,newmax * sizeof(CachedPlan *))));
    owner -> maxplanrefs = newmax;
  }
  else {
    newmax = owner -> maxplanrefs * 2;
    owner -> planrefs = ((CachedPlan **)(repalloc((owner -> planrefs),newmax * sizeof(CachedPlan *))));
    owner -> maxplanrefs = newmax;
  }
}
/*
 * Remember that a plancache reference is owned by a ResourceOwner
 *
 * Caller must have previously done ResourceOwnerEnlargePlanCacheRefs()
 */

void ResourceOwnerRememberPlanCacheRef(ResourceOwner owner,CachedPlan *plan)
{
  ;
  owner -> planrefs[owner -> nplanrefs] = plan;
  owner -> nplanrefs++;
}
/*
 * Forget that a plancache reference is owned by a ResourceOwner
 */

void ResourceOwnerForgetPlanCacheRef(ResourceOwner owner,CachedPlan *plan)
{
  CachedPlan **planrefs = owner -> planrefs;
  int np1 = owner -> nplanrefs - 1;
  int i;
  for (i = np1; i >= 0; i--) {
    if (planrefs[i] == plan) {
      while(i < np1){
        planrefs[i] = planrefs[i + 1];
        i++;
      }
      owner -> nplanrefs = np1;
      return ;
    }
  }
  (elog_start("resowner.c",881,__func__) , elog_finish(20,"plancache reference %p is not owned by resource owner %s",plan,owner -> name));
}
/*
 * Debugging subroutine
 */

static void PrintPlanCacheLeakWarning(CachedPlan *plan)
{
  (elog_start("resowner.c",891,__func__) , elog_finish(19,"plancache reference leak: plan %p not closed",plan));
}
/*
 * Make sure there is room for at least one more entry in a ResourceOwner's
 * tupdesc reference array.
 *
 * This is separate from actually inserting an entry because if we run out
 * of memory, it's critical to do so *before* acquiring the resource.
 */

void ResourceOwnerEnlargeTupleDescs(ResourceOwner owner)
{
  int newmax;
  if (owner -> ntupdescs < owner -> maxtupdescs) {
/* nothing to do */
    return ;
  }
  if (owner -> tupdescs == ((void *)0)) {
    newmax = 16;
    owner -> tupdescs = ((TupleDesc *)(MemoryContextAlloc(TopMemoryContext,newmax * sizeof(TupleDesc ))));
    owner -> maxtupdescs = newmax;
  }
  else {
    newmax = owner -> maxtupdescs * 2;
    owner -> tupdescs = ((TupleDesc *)(repalloc((owner -> tupdescs),newmax * sizeof(TupleDesc ))));
    owner -> maxtupdescs = newmax;
  }
}
/*
 * Remember that a tupdesc reference is owned by a ResourceOwner
 *
 * Caller must have previously done ResourceOwnerEnlargeTupleDescs()
 */

void ResourceOwnerRememberTupleDesc(ResourceOwner owner,TupleDesc tupdesc)
{
  ;
  owner -> tupdescs[owner -> ntupdescs] = tupdesc;
  owner -> ntupdescs++;
}
/*
 * Forget that a tupdesc reference is owned by a ResourceOwner
 */

void ResourceOwnerForgetTupleDesc(ResourceOwner owner,TupleDesc tupdesc)
{
  TupleDesc *tupdescs = owner -> tupdescs;
  int nt1 = owner -> ntupdescs - 1;
  int i;
  for (i = nt1; i >= 0; i--) {
    if (tupdescs[i] == tupdesc) {
      while(i < nt1){
        tupdescs[i] = tupdescs[i + 1];
        i++;
      }
      owner -> ntupdescs = nt1;
      return ;
    }
  }
  (elog_start("resowner.c",961,__func__) , elog_finish(20,"tupdesc reference %p is not owned by resource owner %s",tupdesc,owner -> name));
}
/*
 * Debugging subroutine
 */

static void PrintTupleDescLeakWarning(TupleDesc tupdesc)
{
  (elog_start("resowner.c",971,__func__) , elog_finish(19,"TupleDesc reference leak: TupleDesc %p (%u,%d) still referenced",tupdesc,tupdesc -> tdtypeid,tupdesc -> tdtypmod));
}
/*
 * Make sure there is room for at least one more entry in a ResourceOwner's
 * snapshot reference array.
 *
 * This is separate from actually inserting an entry because if we run out
 * of memory, it's critical to do so *before* acquiring the resource.
 */

void ResourceOwnerEnlargeSnapshots(ResourceOwner owner)
{
  int newmax;
  if (owner -> nsnapshots < owner -> maxsnapshots) {
/* nothing to do */
    return ;
  }
  if (owner -> snapshots == ((void *)0)) {
    newmax = 16;
    owner -> snapshots = ((Snapshot *)(MemoryContextAlloc(TopMemoryContext,newmax * sizeof(Snapshot ))));
    owner -> maxsnapshots = newmax;
  }
  else {
    newmax = owner -> maxsnapshots * 2;
    owner -> snapshots = ((Snapshot *)(repalloc((owner -> snapshots),newmax * sizeof(Snapshot ))));
    owner -> maxsnapshots = newmax;
  }
}
/*
 * Remember that a snapshot reference is owned by a ResourceOwner
 *
 * Caller must have previously done ResourceOwnerEnlargeSnapshots()
 */

void ResourceOwnerRememberSnapshot(ResourceOwner owner,Snapshot snapshot)
{
  ;
  owner -> snapshots[owner -> nsnapshots] = snapshot;
  owner -> nsnapshots++;
}
/*
 * Forget that a snapshot reference is owned by a ResourceOwner
 */

void ResourceOwnerForgetSnapshot(ResourceOwner owner,Snapshot snapshot)
{
  Snapshot *snapshots = owner -> snapshots;
  int ns1 = owner -> nsnapshots - 1;
  int i;
  for (i = ns1; i >= 0; i--) {
    if (snapshots[i] == snapshot) {
      while(i < ns1){
        snapshots[i] = snapshots[i + 1];
        i++;
      }
      owner -> nsnapshots = ns1;
      return ;
    }
  }
  (elog_start("resowner.c",1043,__func__) , elog_finish(20,"snapshot reference %p is not owned by resource owner %s",snapshot,owner -> name));
}
/*
 * Debugging subroutine
 */

static void PrintSnapshotLeakWarning(Snapshot snapshot)
{
  (elog_start("resowner.c",1053,__func__) , elog_finish(19,"Snapshot reference leak: Snapshot %p still referenced",snapshot));
}
/*
 * Make sure there is room for at least one more entry in a ResourceOwner's
 * files reference array.
 *
 * This is separate from actually inserting an entry because if we run out
 * of memory, it's critical to do so *before* acquiring the resource.
 */

void ResourceOwnerEnlargeFiles(ResourceOwner owner)
{
  int newmax;
  if (owner -> nfiles < owner -> maxfiles) {
/* nothing to do */
    return ;
  }
  if (owner -> files == ((void *)0)) {
    newmax = 16;
    owner -> files = ((File *)(MemoryContextAlloc(TopMemoryContext,newmax * sizeof(File ))));
    owner -> maxfiles = newmax;
  }
  else {
    newmax = owner -> maxfiles * 2;
    owner -> files = ((File *)(repalloc((owner -> files),newmax * sizeof(File ))));
    owner -> maxfiles = newmax;
  }
}
/*
 * Remember that a temporary file is owned by a ResourceOwner
 *
 * Caller must have previously done ResourceOwnerEnlargeFiles()
 */

void ResourceOwnerRememberFile(ResourceOwner owner,File file)
{
  ;
  owner -> files[owner -> nfiles] = file;
  owner -> nfiles++;
}
/*
 * Forget that a temporary file is owned by a ResourceOwner
 */

void ResourceOwnerForgetFile(ResourceOwner owner,File file)
{
  File *files = owner -> files;
  int ns1 = owner -> nfiles - 1;
  int i;
  for (i = ns1; i >= 0; i--) {
    if (files[i] == file) {
      while(i < ns1){
        files[i] = files[i + 1];
        i++;
      }
      owner -> nfiles = ns1;
      return ;
    }
  }
  (elog_start("resowner.c",1126,__func__) , elog_finish(20,"temporery file %d is not owned by resource owner %s",file,owner -> name));
}
/*
 * Debugging subroutine
 */

static void PrintFileLeakWarning(File file)
{
  (elog_start("resowner.c",1137,__func__) , elog_finish(19,"temporary file leak: File %d still referenced",file));
}

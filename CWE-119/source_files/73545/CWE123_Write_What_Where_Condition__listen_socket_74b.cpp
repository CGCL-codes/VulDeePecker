/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE123_Write_What_Where_Condition__listen_socket_74b.cpp
Label Definition File: CWE123_Write_What_Where_Condition.label.xml
Template File: sources-sink-74b.tmpl.cpp
*/
/*
 * @description
 * CWE: 123 Write-What-Where Condition
 * BadSource: listen_socket Overwrite linked list pointers using a listen socket (server side)
 * GoodSource: Don't overwrite linked list pointers
 * Sinks:
 *    BadSink : Remove element from list
 * Flow Variant: 74 Data flow: data passed in a map from one function to another in different source files
 *
 * */

#include "std_testcase.h"
#include <map>

typedef struct _linkedList
{
    struct _linkedList *next;
    struct _linkedList *prev;
} linkedList;

typedef struct _badStruct
{
    linkedList list;
} badStruct;

static linkedList *linkedListPrev, *linkedListNext;

using namespace std;

namespace CWE123_Write_What_Where_Condition__listen_socket_74
{

#ifndef OMITBAD

void badSink(map<int, badStruct> dataMap)
{
    /* copy data out of dataMap */
    badStruct data = dataMap[2];
    /* POTENTIAL FLAW: The following removes 'a' from the list.  Because of the possible overflow this
     * causes a "write-what-where" aka "write4".  It does another write as
     * well.  But this is the prototypical "write-what-where" at least from
     * the Windows perspective.
     *
     * linkedListPrev = a->list->prev  WHAT
     * linkedListNext = a->list->next  WHERE
     * linkedListPrev->next = linkedListNext  "at the address that prev/WHERE points, write
     *                    next/WHAT"
     *                    aka "write-what-where"
     * linkedListNext->prev = linkedListPrev  "at the address that next/WHAT points plus 4
     *                    (because prev is the second field in 'list' hence
     *                    4 bytes away on 32b machines), write prev/WHERE"
     */
    linkedListPrev = data.list.prev;
    linkedListNext = data.list.next;
    linkedListPrev->next = linkedListNext;
    linkedListNext->prev = linkedListPrev;
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B uses the GoodSource with the BadSink */
void goodG2BSink(map<int, badStruct> dataMap)
{
    badStruct data = dataMap[2];
    /* POTENTIAL FLAW: The following removes 'a' from the list.  Because of the possible overflow this
     * causes a "write-what-where" aka "write4".  It does another write as
     * well.  But this is the prototypical "write-what-where" at least from
     * the Windows perspective.
     *
     * linkedListPrev = a->list->prev  WHAT
     * linkedListNext = a->list->next  WHERE
     * linkedListPrev->next = linkedListNext  "at the address that prev/WHERE points, write
     *                    next/WHAT"
     *                    aka "write-what-where"
     * linkedListNext->prev = linkedListPrev  "at the address that next/WHAT points plus 4
     *                    (because prev is the second field in 'list' hence
     *                    4 bytes away on 32b machines), write prev/WHERE"
     */
    linkedListPrev = data.list.prev;
    linkedListNext = data.list.next;
    linkedListPrev->next = linkedListNext;
    linkedListNext->prev = linkedListPrev;
}

#endif /* OMITGOOD */

} /* close namespace */

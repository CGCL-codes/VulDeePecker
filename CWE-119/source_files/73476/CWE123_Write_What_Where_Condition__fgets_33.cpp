/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE123_Write_What_Where_Condition__fgets_33.cpp
Label Definition File: CWE123_Write_What_Where_Condition.label.xml
Template File: sources-sink-33.tmpl.cpp
*/
/*
 * @description
 * CWE: 123 Write-What-Where Condition
 * BadSource: fgets Overwrite linked list pointers using fgets
 * GoodSource: Don't overwrite linked list pointers
 * Sinks:
 *    BadSink : Remove element from list
 * Flow Variant: 33 Data flow: use of a C++ reference to data within the same function
 *
 * */

#include "std_testcase.h"

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

namespace CWE123_Write_What_Where_Condition__fgets_33
{

#ifndef OMITBAD

void bad()
{
    badStruct data;
    badStruct &dataRef = data;
    linkedList head = { &head, &head };
    /* This simulates a Microsoft-style linked list insertion */
    data.list.next = head.next;
    data.list.prev = head.prev;
    head.next = &data.list;
    head.prev = &data.list;
    /* FLAW: overwrite linked list pointers with user data */
    if (fgets((char*)&data, sizeof(data), stdin) == NULL)
    {
        printLine("fgets failed!");
        exit(1);
    }
    {
        badStruct data = dataRef;
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
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B() uses the GoodSource with the BadSink */
static void goodG2B()
{
    badStruct data;
    badStruct &dataRef = data;
    linkedList head = { &head, &head };
    /* This simulates a Microsoft-style linked list insertion */
    data.list.next = head.next;
    data.list.prev = head.prev;
    head.next = &data.list;
    head.prev = &data.list;
    /* FIX: don't overwrite linked list pointers */
    ; /* empty statement needed by some flow variants */
    {
        badStruct data = dataRef;
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
}

void good()
{
    goodG2B();
}

#endif /* OMITGOOD */

} /* close namespace */

/* Below is the main(). It is only used when building this testcase on
 * its own for testing or for building a binary to use in testing binary
 * analysis tools. It is not used when compiling all the testcases as one
 * application, which is how source code analysis tools are tested.
 */
#ifdef INCLUDEMAIN

using namespace CWE123_Write_What_Where_Condition__fgets_33; /* so that we can use good and bad easily */

int main(int argc, char * argv[])
{
    /* seed randomness */
    srand( (unsigned)time(NULL) );
#ifndef OMITGOOD
    printLine("Calling good()...");
    good();
    printLine("Finished good()");
#endif /* OMITGOOD */
#ifndef OMITBAD
    printLine("Calling bad()...");
    bad();
    printLine("Finished bad()");
#endif /* OMITBAD */
    return 0;
}

#endif

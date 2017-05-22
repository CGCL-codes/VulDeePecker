/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE123_Write_What_Where_Condition__fgets_62b.cpp
Label Definition File: CWE123_Write_What_Where_Condition.label.xml
Template File: sources-sink-62b.tmpl.cpp
*/
/*
 * @description
 * CWE: 123 Write-What-Where Condition
 * BadSource: fgets Overwrite linked list pointers using fgets
 * GoodSource: Don't overwrite linked list pointers
 * Sinks:
 *    BadSink : Remove element from list
 * Flow Variant: 62 Data flow: data flows using a C++ reference from one function to another in different source files
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

namespace CWE123_Write_What_Where_Condition__fgets_62
{

#ifndef OMITBAD

void badSource(badStruct &data)
{
    /* FLAW: overwrite linked list pointers with user data */
    if (fgets((char*)&data, sizeof(data), stdin) == NULL)
    {
        printLine("fgets failed!");
        exit(1);
    }
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B() uses the GoodSource with the BadSink */
void goodG2BSource(badStruct &data)
{
    /* FIX: don't overwrite linked list pointers */
    ; /* empty statement needed by some flow variants */
}

#endif /* OMITGOOD */

} /* close namespace */

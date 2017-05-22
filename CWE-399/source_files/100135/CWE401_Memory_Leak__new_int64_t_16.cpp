/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE401_Memory_Leak__new_int64_t_16.cpp
Label Definition File: CWE401_Memory_Leak__new.label.xml
Template File: sources-sinks-16.tmpl.cpp
*/
/*
 * @description
 * CWE: 401 Memory Leak
 * BadSource:  Allocate data using new
 * GoodSource: Allocate data on the stack
 * Sinks:
 *    GoodSink: call delete on data
 *    BadSink : no deallocation of data
 * Flow Variant: 16 Control flow: while(1)
 * */

#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

namespace CWE401_Memory_Leak__new_int64_t_16
{

#ifndef OMITBAD

void bad()
{
    int64_t * data;
    data = NULL;
    while(1)
    {
        /* POTENTIAL FLAW: Allocate memory on the heap */
        data = new int64_t;
        /* Initialize and make use of data */
        *data = 5LL;
        printLongLongLine(*data);
        break;
    }
    while(1)
    {
        /* POTENTIAL FLAW: No deallocation */
        ; /* empty statement needed for some flow variants */
        break;
    }
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodB2G() - use badsource and goodsink by changing the sinks in the second while statement */
static void goodB2G()
{
    int64_t * data;
    data = NULL;
    while(1)
    {
        /* POTENTIAL FLAW: Allocate memory on the heap */
        data = new int64_t;
        /* Initialize and make use of data */
        *data = 5LL;
        printLongLongLine(*data);
        break;
    }
    while(1)
    {
        /* FIX: Deallocate memory */
        delete data;
        break;
    }
}

/* goodG2B() - use goodsource and badsink by changing the sources in the first while statement */
static void goodG2B()
{
    int64_t * data;
    data = NULL;
    while(1)
    {
        /* FIX: Use memory allocated on the stack */
        int64_t dataGoodBuffer;
        data = &dataGoodBuffer;
        /* Initialize and make use of data */
        *data = 5LL;
        printLongLongLine(*data);
        break;
    }
    while(1)
    {
        /* POTENTIAL FLAW: No deallocation */
        ; /* empty statement needed for some flow variants */
        break;
    }
}

void good()
{
    goodB2G();
    goodG2B();
}

#endif /* OMITGOOD */

} /* close namespace */

/* Below is the main(). It is only used when building this testcase on
   its own for testing or for building a binary to use in testing binary
   analysis tools. It is not used when compiling all the testcases as one
   application, which is how source code analysis tools are tested. */

#ifdef INCLUDEMAIN

using namespace CWE401_Memory_Leak__new_int64_t_16; /* so that we can use good and bad easily */

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

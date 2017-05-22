/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_82a.cpp
Label Definition File: CWE789_Uncontrolled_Mem_Alloc__malloc.label.xml
Template File: sources-sinks-82a.tmpl.cpp
*/
/*
 * @description
 * CWE: 789 Uncontrolled Memory Allocation
 * BadSource: rand Set data to result of rand(), which may be zero
 * GoodSource: Small number greater than zero
 * Sinks:
 *    GoodSink: Allocate memory with malloc() and check the size of the memory to be allocated
 *    BadSink : Allocate memory with malloc(), but incorrectly check the size of the memory to be allocated
 * Flow Variant: 82 Data flow: data passed in a parameter to an virtual method called via a pointer
 *
 * */

#include "std_testcase.h"
#include "CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_82.h"

namespace CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_82
{

#ifndef OMITBAD

void bad()
{
    size_t data;
    /* Initialize data */
    data = 0;
    /* POTENTIAL FLAW: Set data to a random value */
    data = rand();
    CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_82_base* baseObject = new CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_82_bad;
    baseObject->action(data);
    delete baseObject;
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B uses the GoodSource with the BadSink */
static void goodG2B()
{
    size_t data;
    /* Initialize data */
    data = 0;
    /* FIX: Use a relatively small number for memory allocation */
    data = 20;
    CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_82_base* baseObject = new CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_82_goodG2B;
    baseObject->action(data);
    delete baseObject;
}

/* goodB2G uses the BadSource with the GoodSink */
static void goodB2G()
{
    size_t data;
    /* Initialize data */
    data = 0;
    /* POTENTIAL FLAW: Set data to a random value */
    data = rand();
    CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_82_base* baseObject = new CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_82_goodB2G;
    baseObject->action(data);
    delete baseObject;
}

void good()
{
    goodG2B();
    goodB2G();
}

#endif /* OMITGOOD */

} /* close namespace */

/* Below is the main(). It is only used when building this testcase on
   its own for testing or for building a binary to use in testing binary
   analysis tools. It is not used when compiling all the testcases as one
   application, which is how source code analysis tools are tested. */

#ifdef INCLUDEMAIN

using namespace CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_82; /* so that we can use good and bad easily */

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

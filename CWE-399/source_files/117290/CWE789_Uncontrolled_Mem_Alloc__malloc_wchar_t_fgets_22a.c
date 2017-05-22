/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22a.c
Label Definition File: CWE789_Uncontrolled_Mem_Alloc__malloc.label.xml
Template File: sources-sinks-22a.tmpl.c
*/
/*
 * @description
 * CWE: 789 Uncontrolled Memory Allocation
 * BadSource: fgets Read data from the console using fgets()
 * GoodSource: Small number greater than zero
 * Sinks:
 *    GoodSink: Allocate memory with malloc() and check the size of the memory to be allocated
 *    BadSink : Allocate memory with malloc(), but incorrectly check the size of the memory to be allocated
 * Flow Variant: 22 Control flow: Flow controlled by value of a global variable. Sink functions are in a separate file from sources.
 *
 * */

#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

#define CHAR_ARRAY_SIZE (3 * sizeof(data) + 2)

#ifndef OMITBAD

/* The global variable below is used to drive control flow in the sink function */
int CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22_badGlobal = 0;

void CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22_badSink(size_t data);

void CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22_bad()
{
    size_t data;
    /* Initialize data */
    data = 0;
    {
        char inputBuffer[CHAR_ARRAY_SIZE] = "";
        /* POTENTIAL FLAW: Read data from the console using fgets() */
        if (fgets(inputBuffer, CHAR_ARRAY_SIZE, stdin) != NULL)
        {
            /* Convert to unsigned int */
            data = strtoul(inputBuffer, NULL, 0);
        }
        else
        {
            printLine("fgets() failed.");
        }
    }
    CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22_badGlobal = 1; /* true */
    CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22_badSink(data);
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* The global variables below are used to drive control flow in the sink functions. */
int CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22_goodB2G1Global = 0;
int CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22_goodB2G2Global = 0;
int CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22_goodG2BGlobal = 0;

/* goodB2G1() - use badsource and goodsink by setting the static variable to false instead of true */
void CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22_goodB2G1Sink(size_t data);

static void goodB2G1()
{
    size_t data;
    /* Initialize data */
    data = 0;
    {
        char inputBuffer[CHAR_ARRAY_SIZE] = "";
        /* POTENTIAL FLAW: Read data from the console using fgets() */
        if (fgets(inputBuffer, CHAR_ARRAY_SIZE, stdin) != NULL)
        {
            /* Convert to unsigned int */
            data = strtoul(inputBuffer, NULL, 0);
        }
        else
        {
            printLine("fgets() failed.");
        }
    }
    CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22_goodB2G1Global = 0; /* false */
    CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22_goodB2G1Sink(data);
}

/* goodB2G2() - use badsource and goodsink by reversing the blocks in the if in the sink function */
void CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22_goodB2G2Sink(size_t data);

static void goodB2G2()
{
    size_t data;
    /* Initialize data */
    data = 0;
    {
        char inputBuffer[CHAR_ARRAY_SIZE] = "";
        /* POTENTIAL FLAW: Read data from the console using fgets() */
        if (fgets(inputBuffer, CHAR_ARRAY_SIZE, stdin) != NULL)
        {
            /* Convert to unsigned int */
            data = strtoul(inputBuffer, NULL, 0);
        }
        else
        {
            printLine("fgets() failed.");
        }
    }
    CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22_goodB2G2Global = 1; /* true */
    CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22_goodB2G2Sink(data);
}

/* goodG2B() - use goodsource and badsink */
void CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22_goodG2BSink(size_t data);

static void goodG2B()
{
    size_t data;
    /* Initialize data */
    data = 0;
    /* FIX: Use a relatively small number for memory allocation */
    data = 20;
    CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22_goodG2BGlobal = 1; /* true */
    CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22_goodG2BSink(data);
}

void CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22_good()
{
    goodB2G1();
    goodB2G2();
    goodG2B();
}

#endif /* OMITGOOD */

/* Below is the main(). It is only used when building this testcase on
   its own for testing or for building a binary to use in testing binary
   analysis tools. It is not used when compiling all the testcases as one
   application, which is how source code analysis tools are tested. */

#ifdef INCLUDEMAIN

int main(int argc, char * argv[])
{
    /* seed randomness */
    srand( (unsigned)time(NULL) );
#ifndef OMITGOOD
    printLine("Calling good()...");
    CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22_good();
    printLine("Finished good()");
#endif /* OMITGOOD */
#ifndef OMITBAD
    printLine("Calling bad()...");
    CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_22_bad();
    printLine("Finished bad()");
#endif /* OMITBAD */
    return 0;
}

#endif

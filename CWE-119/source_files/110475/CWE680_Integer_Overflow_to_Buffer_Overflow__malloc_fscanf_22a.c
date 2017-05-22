/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22a.c
Label Definition File: CWE680_Integer_Overflow_to_Buffer_Overflow__malloc.label.xml
Template File: sources-sink-22a.tmpl.c
*/
/*
 * @description
 * CWE: 680 Integer Overflow to Buffer Overflow
 * BadSource: fscanf Read data from the console using fscanf()
 * GoodSource: Small number greater than zero that will not cause an integer overflow in the sink
 * Sink:
 *    BadSink : Attempt to allocate array using length value from source
 * Flow Variant: 22 Control flow: Flow controlled by value of a global variable. Sink functions are in a separate file from sources.
 *
 * */

#include "std_testcase.h"

#ifndef OMITBAD

/* The global variable below is used to drive control flow in the source function */
int CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_badGlobal = 0;

int CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_badSource(int data);

void CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_bad()
{
    int data;
    /* Initialize data */
    data = -1;
    CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_badGlobal = 1; /* true */
    data = CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_badSource(data);
    {
        size_t i;
        int *intPointer;
        /* POTENTIAL FLAW: if data * sizeof(int) > SIZE_MAX, overflows to a small value
         * so that the for loop doing the initialization causes a buffer overflow */
        intPointer = (int*)malloc(data * sizeof(int));
        for (i = 0; i < (size_t)data; i++)
        {
            intPointer[i] = 0; /* Potentially writes beyond the boundary of intPointer */
        }
        printIntLine(intPointer[0]);
        free(intPointer);
    }
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* The global variables below are used to drive control flow in the source functions. */
int CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_goodG2B1Global = 0;
int CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_goodG2B2Global = 0;

/* goodG2B1() - use goodsource and badsink by setting the static variable to false instead of true */
int CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_goodG2B1Source(int data);

static void goodG2B1()
{
    int data;
    /* Initialize data */
    data = -1;
    CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_goodG2B1Global = 0; /* false */
    data = CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_goodG2B1Source(data);
    {
        size_t i;
        int *intPointer;
        /* POTENTIAL FLAW: if data * sizeof(int) > SIZE_MAX, overflows to a small value
         * so that the for loop doing the initialization causes a buffer overflow */
        intPointer = (int*)malloc(data * sizeof(int));
        for (i = 0; i < (size_t)data; i++)
        {
            intPointer[i] = 0; /* Potentially writes beyond the boundary of intPointer */
        }
        printIntLine(intPointer[0]);
        free(intPointer);
    }
}

/* goodG2B2() - use goodsource and badsink by reversing the blocks in the if in the source function */
int CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_goodG2B2Source(int data);

static void goodG2B2()
{
    int data;
    /* Initialize data */
    data = -1;
    CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_goodG2B2Global = 1; /* true */
    data = CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_goodG2B2Source(data);
    {
        size_t i;
        int *intPointer;
        /* POTENTIAL FLAW: if data * sizeof(int) > SIZE_MAX, overflows to a small value
         * so that the for loop doing the initialization causes a buffer overflow */
        intPointer = (int*)malloc(data * sizeof(int));
        for (i = 0; i < (size_t)data; i++)
        {
            intPointer[i] = 0; /* Potentially writes beyond the boundary of intPointer */
        }
        printIntLine(intPointer[0]);
        free(intPointer);
    }
}

void CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_good()
{
    goodG2B1();
    goodG2B2();
}

#endif /* OMITGOOD */

/* Below is the main(). It is only used when building this testcase on
 * its own for testing or for building a binary to use in testing binary
 * analysis tools. It is not used when compiling all the testcases as one
 * application, which is how source code analysis tools are tested.
 */

#ifdef INCLUDEMAIN

int main(int argc, char * argv[])
{
    /* seed randomness */
    srand( (unsigned)time(NULL) );
#ifndef OMITGOOD
    printLine("Calling good()...");
    CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_good();
    printLine("Finished good()");
#endif /* OMITGOOD */
#ifndef OMITBAD
    printLine("Calling bad()...");
    CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_bad();
    printLine("Finished bad()");
#endif /* OMITBAD */
    return 0;
}

#endif

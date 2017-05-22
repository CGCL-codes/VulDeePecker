/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE680_Integer_Overflow_to_Buffer_Overflow__new_fgets_68b.cpp
Label Definition File: CWE680_Integer_Overflow_to_Buffer_Overflow__new.label.xml
Template File: sources-sink-68b.tmpl.cpp
*/
/*
 * @description
 * CWE: 680 Integer Overflow to Buffer Overflow
 * BadSource: fgets Read data from the console using fgets()
 * GoodSource: Small number greater than zero that will not cause an integer overflow in the sink
 * Sink:
 *    BadSink : Attempt to allocate array using length value from source
 * Flow Variant: 68 Data flow: data passed as a global variable from one function to another in different source files
 *
 * */

#include "std_testcase.h"

#define CHAR_ARRAY_SIZE (3 * sizeof(data) + 2)

extern int CWE680_Integer_Overflow_to_Buffer_Overflow__new_fgets_68_badData;
extern int CWE680_Integer_Overflow_to_Buffer_Overflow__new_fgets_68_goodG2BData;

namespace CWE680_Integer_Overflow_to_Buffer_Overflow__new_fgets_68
{

/* all the sinks are the same, we just want to know where the hit originated if a tool flags one */

#ifndef OMITBAD

void badSink()
{
    int data = CWE680_Integer_Overflow_to_Buffer_Overflow__new_fgets_68_badData;
    {
        size_t dataBytes,i;
        int *intPointer;
        /* POTENTIAL FLAW: dataBytes may overflow to a small value */
        dataBytes = data * sizeof(int); /* sizeof array in bytes */
        intPointer = (int*)new char[dataBytes];
        for (i = 0; i < (size_t)data; i++)
        {
            intPointer[i] = 0; /* may write beyond limit of intPointer if integer overflow occured above */
        }
        printIntLine(intPointer[0]);
        delete [] intPointer;
    }
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B uses the GoodSource with the BadSink */
void goodG2BSink()
{
    int data = CWE680_Integer_Overflow_to_Buffer_Overflow__new_fgets_68_goodG2BData;
    {
        size_t dataBytes,i;
        int *intPointer;
        /* POTENTIAL FLAW: dataBytes may overflow to a small value */
        dataBytes = data * sizeof(int); /* sizeof array in bytes */
        intPointer = (int*)new char[dataBytes];
        for (i = 0; i < (size_t)data; i++)
        {
            intPointer[i] = 0; /* may write beyond limit of intPointer if integer overflow occured above */
        }
        printIntLine(intPointer[0]);
        delete [] intPointer;
    }
}

#endif /* OMITGOOD */

} /* close namespace */

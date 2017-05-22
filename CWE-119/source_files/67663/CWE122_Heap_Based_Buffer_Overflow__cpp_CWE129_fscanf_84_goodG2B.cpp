/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE122_Heap_Based_Buffer_Overflow__cpp_CWE129_fscanf_84_goodG2B.cpp
Label Definition File: CWE122_Heap_Based_Buffer_Overflow__cpp_CWE129.label.xml
Template File: sources-sinks-84_goodG2B.tmpl.cpp
*/
/*
 * @description
 * CWE: 122 Heap Based Buffer Overflow
 * BadSource: fscanf Read data from the console using fscanf()
 * GoodSource: Larger than zero but less than 10
 * Sinks:
 *    GoodSink: Ensure the array index is valid
 *    BadSink : Improperly check the array index by not checking the upper bound
 * Flow Variant: 84 Data flow: data passed to class constructor and destructor by declaring the class object on the heap and deleting it after use
 *
 * */
#ifndef OMITGOOD

#include "std_testcase.h"
#include "CWE122_Heap_Based_Buffer_Overflow__cpp_CWE129_fscanf_84.h"

namespace CWE122_Heap_Based_Buffer_Overflow__cpp_CWE129_fscanf_84
{
CWE122_Heap_Based_Buffer_Overflow__cpp_CWE129_fscanf_84_goodG2B::CWE122_Heap_Based_Buffer_Overflow__cpp_CWE129_fscanf_84_goodG2B(int dataCopy)
{
    data = dataCopy;
    /* FIX: Use a value greater than 0, but less than 10 to avoid attempting to
     * access an index of the array in the sink that is out-of-bounds */
    data = 7;
}

CWE122_Heap_Based_Buffer_Overflow__cpp_CWE129_fscanf_84_goodG2B::~CWE122_Heap_Based_Buffer_Overflow__cpp_CWE129_fscanf_84_goodG2B()
{
    {
        int i;
        int * buffer = new int[10];
        /* initialize buffer */
        for (i = 0; i < 10; i++)
        {
            buffer[i] = 0;
        }
        /* POTENTIAL FLAW: Attempt to write to an index of the array that is above the upper bound
        * This code does check to see if the array index is negative */
        if (data >= 0)
        {
            buffer[data] = 1;
            /* Print the array values */
            for(i = 0; i < 10; i++)
            {
                printIntLine(buffer[i]);
            }
        }
        else
        {
            printLine("ERROR: Array index is negative.");
        }
        delete[] buffer;
    }
}
}
#endif /* OMITGOOD */

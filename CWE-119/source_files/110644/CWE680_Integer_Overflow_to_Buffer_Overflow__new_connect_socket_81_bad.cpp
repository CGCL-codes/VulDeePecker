/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE680_Integer_Overflow_to_Buffer_Overflow__new_connect_socket_81_bad.cpp
Label Definition File: CWE680_Integer_Overflow_to_Buffer_Overflow__new.label.xml
Template File: sources-sink-81_bad.tmpl.cpp
*/
/*
 * @description
 * CWE: 680 Integer Overflow to Buffer Overflow
 * BadSource: connect_socket Read data using a connect socket (client side)
 * GoodSource: Small number greater than zero that will not cause an integer overflow in the sink
 * Sinks:
 *    BadSink : Attempt to allocate array using length value from source
 * Flow Variant: 81 Data flow: data passed in a parameter to a virtual method called via a reference
 *
 * */
#ifndef OMITBAD

#include "std_testcase.h"
#include "CWE680_Integer_Overflow_to_Buffer_Overflow__new_connect_socket_81.h"

namespace CWE680_Integer_Overflow_to_Buffer_Overflow__new_connect_socket_81
{

void CWE680_Integer_Overflow_to_Buffer_Overflow__new_connect_socket_81_bad::action(int data) const
{
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

}
#endif /* OMITBAD */

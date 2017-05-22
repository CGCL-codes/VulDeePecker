/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE126_Buffer_Overread__CWE129_connect_socket_22b.c
Label Definition File: CWE126_Buffer_Overread__CWE129.label.xml
Template File: sources-sinks-22b.tmpl.c
*/
/*
 * @description
 * CWE: 126 Buffer Overread
 * BadSource: connect_socket Read data using a connect socket (client side)
 * GoodSource: Larger than zero but less than 10
 * Sinks:
 *    GoodSink: Ensure the array index is valid
 *    BadSink : Improperly check the array index by not checking the upper bound
 * Flow Variant: 22 Control flow: Flow controlled by value of a global variable. Sink functions are in a separate file from sources.
 *
 * */

#include "std_testcase.h"

#ifndef OMITBAD

/* The global variable below is used to drive control flow in the sink function */
extern int CWE126_Buffer_Overread__CWE129_connect_socket_22_badGlobal;

void CWE126_Buffer_Overread__CWE129_connect_socket_22_badSink(int data)
{
    if(CWE126_Buffer_Overread__CWE129_connect_socket_22_badGlobal)
    {
        {
            int buffer[10] = { 0 };
            /* POTENTIAL FLAW: Attempt to access an index of the array that is above the upper bound
             * This check does not check the upper bounds of the array index */
            if (data >= 0)
            {
                printIntLine(buffer[data]);
            }
            else
            {
                printLine("ERROR: Array index is negative");
            }
        }
    }
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* The global variables below are used to drive control flow in the sink functions. */
extern int CWE126_Buffer_Overread__CWE129_connect_socket_22_goodB2G1Global;
extern int CWE126_Buffer_Overread__CWE129_connect_socket_22_goodB2G2Global;
extern int CWE126_Buffer_Overread__CWE129_connect_socket_22_goodG2BGlobal;

/* goodB2G1() - use badsource and goodsink by setting the static variable to false instead of true */
void CWE126_Buffer_Overread__CWE129_connect_socket_22_goodB2G1Sink(int data)
{
    if(CWE126_Buffer_Overread__CWE129_connect_socket_22_goodB2G1Global)
    {
        /* INCIDENTAL: CWE 561 Dead Code, the code below will never run */
        printLine("Benign, fixed string");
    }
    else
    {
        {
            int buffer[10] = { 0 };
            /* FIX: Properly validate the array index and prevent a buffer overread */
            if (data >= 0 && data < (10))
            {
                printIntLine(buffer[data]);
            }
            else
            {
                printLine("ERROR: Array index is out-of-bounds");
            }
        }
    }
}

/* goodB2G2() - use badsource and goodsink by reversing the blocks in the if in the sink function */
void CWE126_Buffer_Overread__CWE129_connect_socket_22_goodB2G2Sink(int data)
{
    if(CWE126_Buffer_Overread__CWE129_connect_socket_22_goodB2G2Global)
    {
        {
            int buffer[10] = { 0 };
            /* FIX: Properly validate the array index and prevent a buffer overread */
            if (data >= 0 && data < (10))
            {
                printIntLine(buffer[data]);
            }
            else
            {
                printLine("ERROR: Array index is out-of-bounds");
            }
        }
    }
}

/* goodG2B() - use goodsource and badsink */
void CWE126_Buffer_Overread__CWE129_connect_socket_22_goodG2BSink(int data)
{
    if(CWE126_Buffer_Overread__CWE129_connect_socket_22_goodG2BGlobal)
    {
        {
            int buffer[10] = { 0 };
            /* POTENTIAL FLAW: Attempt to access an index of the array that is above the upper bound
             * This check does not check the upper bounds of the array index */
            if (data >= 0)
            {
                printIntLine(buffer[data]);
            }
            else
            {
                printLine("ERROR: Array index is negative");
            }
        }
    }
}

#endif /* OMITGOOD */

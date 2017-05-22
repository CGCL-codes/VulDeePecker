/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE124_Buffer_Underwrite__CWE839_listen_socket_22b.c
Label Definition File: CWE124_Buffer_Underwrite__CWE839.label.xml
Template File: sources-sinks-22b.tmpl.c
*/
/*
 * @description
 * CWE: 124 Buffer Underwrite
 * BadSource: listen_socket Read data using a listen socket (server side)
 * GoodSource: Non-negative but less than 10
 * Sinks:
 *    GoodSink: Ensure the array index is valid
 *    BadSink : Improperly check the array index by not checking the lower bound
 * Flow Variant: 22 Control flow: Flow controlled by value of a global variable. Sink functions are in a separate file from sources.
 *
 * */

#include "std_testcase.h"

#ifndef OMITBAD

/* The global variable below is used to drive control flow in the sink function */
extern int CWE124_Buffer_Underwrite__CWE839_listen_socket_22_badGlobal;

void CWE124_Buffer_Underwrite__CWE839_listen_socket_22_badSink(int data)
{
    if(CWE124_Buffer_Underwrite__CWE839_listen_socket_22_badGlobal)
    {
        {
            int i;
            int buffer[10] = { 0 };
            /* POTENTIAL FLAW: Attempt to access a negative index of the array
            * This code does not check to see if the array index is negative */
            if (data < 10)
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
        }
    }
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* The global variables below are used to drive control flow in the sink functions. */
extern int CWE124_Buffer_Underwrite__CWE839_listen_socket_22_goodB2G1Global;
extern int CWE124_Buffer_Underwrite__CWE839_listen_socket_22_goodB2G2Global;
extern int CWE124_Buffer_Underwrite__CWE839_listen_socket_22_goodG2BGlobal;

/* goodB2G1() - use badsource and goodsink by setting the static variable to false instead of true */
void CWE124_Buffer_Underwrite__CWE839_listen_socket_22_goodB2G1Sink(int data)
{
    if(CWE124_Buffer_Underwrite__CWE839_listen_socket_22_goodB2G1Global)
    {
        /* INCIDENTAL: CWE 561 Dead Code, the code below will never run */
        printLine("Benign, fixed string");
    }
    else
    {
        {
            int i;
            int buffer[10] = { 0 };
            /* FIX: Properly validate the array index and prevent a buffer underwrite */
            if (data >= 0 && data < (10))
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
                printLine("ERROR: Array index is out-of-bounds");
            }
        }
    }
}

/* goodB2G2() - use badsource and goodsink by reversing the blocks in the if in the sink function */
void CWE124_Buffer_Underwrite__CWE839_listen_socket_22_goodB2G2Sink(int data)
{
    if(CWE124_Buffer_Underwrite__CWE839_listen_socket_22_goodB2G2Global)
    {
        {
            int i;
            int buffer[10] = { 0 };
            /* FIX: Properly validate the array index and prevent a buffer underwrite */
            if (data >= 0 && data < (10))
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
                printLine("ERROR: Array index is out-of-bounds");
            }
        }
    }
}

/* goodG2B() - use goodsource and badsink */
void CWE124_Buffer_Underwrite__CWE839_listen_socket_22_goodG2BSink(int data)
{
    if(CWE124_Buffer_Underwrite__CWE839_listen_socket_22_goodG2BGlobal)
    {
        {
            int i;
            int buffer[10] = { 0 };
            /* POTENTIAL FLAW: Attempt to access a negative index of the array
            * This code does not check to see if the array index is negative */
            if (data < 10)
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
        }
    }
}

#endif /* OMITGOOD */

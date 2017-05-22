/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_connect_socket_22b.c
Label Definition File: CWE789_Uncontrolled_Mem_Alloc__malloc.label.xml
Template File: sources-sinks-22b.tmpl.c
*/
/*
 * @description
 * CWE: 789 Uncontrolled Memory Allocation
 * BadSource: connect_socket Read data using a connect socket (client side)
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

#define HELLO_STRING L"hello"

#ifndef OMITBAD

/* The global variable below is used to drive control flow in the sink function */
extern int CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_connect_socket_22_badGlobal;

void CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_connect_socket_22_badSink(size_t data)
{
    if(CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_connect_socket_22_badGlobal)
    {
        {
            wchar_t * myString;
            /* POTENTIAL FLAW: No MAXIMUM limitation for memory allocation, but ensure data is large enough
             * for the wcscpy() function to not cause a buffer overflow */
            /* INCIDENTAL FLAW: The source could cause a type overrun in data or in the memory allocation */
            if (data > wcslen(HELLO_STRING))
            {
                myString = (wchar_t *)malloc(data*sizeof(wchar_t));
                /* Copy a small string into myString */
                wcscpy(myString, HELLO_STRING);
                printWLine(myString);
                free(myString);
            }
            else
            {
                printLine("Input is less than the length of the source string");
            }
        }
    }
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* The global variables below are used to drive control flow in the sink functions. */
extern int CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_connect_socket_22_goodB2G1Global;
extern int CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_connect_socket_22_goodB2G2Global;
extern int CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_connect_socket_22_goodG2BGlobal;

/* goodB2G1() - use badsource and goodsink by setting the static variable to false instead of true */
void CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_connect_socket_22_goodB2G1Sink(size_t data)
{
    if(CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_connect_socket_22_goodB2G1Global)
    {
        /* INCIDENTAL: CWE 561 Dead Code, the code below will never run */
        printLine("Benign, fixed string");
    }
    else
    {
        {
            wchar_t * myString;
            /* FIX: Include a MAXIMUM limitation for memory allocation and a check to ensure data is large enough
             * for the wcscpy() function to not cause a buffer overflow */
            /* INCIDENTAL FLAW: The source could cause a type overrun in data or in the memory allocation */
            if (data > wcslen(HELLO_STRING) && data < 100)
            {
                myString = (wchar_t *)malloc(data*sizeof(wchar_t));
                /* Copy a small string into myString */
                wcscpy(myString, HELLO_STRING);
                printWLine(myString);
                free(myString);
            }
            else
            {
                printLine("Input is less than the length of the source string or too large");
            }
        }
    }
}

/* goodB2G2() - use badsource and goodsink by reversing the blocks in the if in the sink function */
void CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_connect_socket_22_goodB2G2Sink(size_t data)
{
    if(CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_connect_socket_22_goodB2G2Global)
    {
        {
            wchar_t * myString;
            /* FIX: Include a MAXIMUM limitation for memory allocation and a check to ensure data is large enough
             * for the wcscpy() function to not cause a buffer overflow */
            /* INCIDENTAL FLAW: The source could cause a type overrun in data or in the memory allocation */
            if (data > wcslen(HELLO_STRING) && data < 100)
            {
                myString = (wchar_t *)malloc(data*sizeof(wchar_t));
                /* Copy a small string into myString */
                wcscpy(myString, HELLO_STRING);
                printWLine(myString);
                free(myString);
            }
            else
            {
                printLine("Input is less than the length of the source string or too large");
            }
        }
    }
}

/* goodG2B() - use goodsource and badsink */
void CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_connect_socket_22_goodG2BSink(size_t data)
{
    if(CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_connect_socket_22_goodG2BGlobal)
    {
        {
            wchar_t * myString;
            /* POTENTIAL FLAW: No MAXIMUM limitation for memory allocation, but ensure data is large enough
             * for the wcscpy() function to not cause a buffer overflow */
            /* INCIDENTAL FLAW: The source could cause a type overrun in data or in the memory allocation */
            if (data > wcslen(HELLO_STRING))
            {
                myString = (wchar_t *)malloc(data*sizeof(wchar_t));
                /* Copy a small string into myString */
                wcscpy(myString, HELLO_STRING);
                printWLine(myString);
                free(myString);
            }
            else
            {
                printLine("Input is less than the length of the source string");
            }
        }
    }
}

#endif /* OMITGOOD */

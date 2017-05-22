/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_fixed_string_22b.c
Label Definition File: CWE761_Free_Pointer_Not_at_Start_of_Buffer.label.xml
Template File: source-sinks-22b.tmpl.c
*/
/*
 * @description
 * CWE: 761 Free Pointer not at Start of Buffer
 * BadSource: fixed_string Initialize data to be a fixed string
 * Sinks:
 *    GoodSink: free() memory correctly at the start of the buffer
 *    BadSink : free() memory not at the start of the buffer
 * Flow Variant: 22 Control flow: Flow controlled by value of a global variable. Sink functions are in a separate file from sources.
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

#define SEARCH_CHAR L'S'

#ifndef OMITBAD

/* The global variable below is used to drive control flow in the sink function */
extern int CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_fixed_string_22_badGlobal;

void CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_fixed_string_22_badSink(wchar_t * data)
{
    if(CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_fixed_string_22_badGlobal)
    {
        /* FLAW: We are incrementing the pointer in the loop - this will cause us to free the
         * memory block not at the start of the buffer */
        for (; *data != L'\0'; data++)
        {
            if (*data == SEARCH_CHAR)
            {
                printLine("We have a match!");
                break;
            }
        }
        free(data);
    }
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* The global variables below are used to drive control flow in the sink functions. */
extern int CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_fixed_string_22_goodB2G1Global;
extern int CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_fixed_string_22_goodB2G2Global;

/* goodB2G1() - use badsource and goodsink by setting the static variable to false instead of true */
void CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_fixed_string_22_goodB2G1Sink(wchar_t * data)
{
    if(CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_fixed_string_22_goodB2G1Global)
    {
        /* INCIDENTAL: CWE 561 Dead Code, the code below will never run */
        printLine("Benign, fixed string");
    }
    else
    {
        {
            size_t i;
            /* FIX: Use a loop variable to traverse through the string pointed to by data */
            for (i=0; i < wcslen(data); i++)
            {
                if (data[i] == SEARCH_CHAR)
                {
                    printLine("We have a match!");
                    break;
                }
            }
            free(data);
        }
    }
}

/* goodB2G2() - use badsource and goodsink by reversing the blocks in the if in the sink function */
void CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_fixed_string_22_goodB2G2Sink(wchar_t * data)
{
    if(CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_fixed_string_22_goodB2G2Global)
    {
        {
            size_t i;
            /* FIX: Use a loop variable to traverse through the string pointed to by data */
            for (i=0; i < wcslen(data); i++)
            {
                if (data[i] == SEARCH_CHAR)
                {
                    printLine("We have a match!");
                    break;
                }
            }
            free(data);
        }
    }
}

#endif /* OMITGOOD */

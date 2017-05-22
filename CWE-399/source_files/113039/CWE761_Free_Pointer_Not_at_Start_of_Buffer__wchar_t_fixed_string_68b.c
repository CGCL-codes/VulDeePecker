/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_fixed_string_68b.c
Label Definition File: CWE761_Free_Pointer_Not_at_Start_of_Buffer.label.xml
Template File: source-sinks-68b.tmpl.c
*/
/*
 * @description
 * CWE: 761 Free Pointer not at Start of Buffer
 * BadSource: fixed_string Initialize data to be a fixed string
 * Sinks:
 *    GoodSink: free() memory correctly at the start of the buffer
 *    BadSink : free() memory not at the start of the buffer
 * Flow Variant: 68 Data flow: data passed as a global variable from one function to another in different source files
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

#define BAD_SOURCE_FIXED_STRING L"Fixed String" /* MAINTENANCE NOTE: This string must contain the SEARCH_CHAR */

#define SEARCH_CHAR L'S'

extern wchar_t * CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_fixed_string_68_badDataForBadSink;

extern wchar_t * CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_fixed_string_68_badDataForGoodSink;

#ifndef OMITBAD

void CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_fixed_string_68b_badSink()
{
    wchar_t * data = CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_fixed_string_68_badDataForBadSink;
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

#endif /* OMITBAD */

#ifndef OMITGOOD

void CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_fixed_string_68b_goodB2GSink()
{
    wchar_t * data = CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_fixed_string_68_badDataForGoodSink;
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

#endif /* OMITGOOD */

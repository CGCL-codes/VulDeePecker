/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_file_68b.c
Label Definition File: CWE761_Free_Pointer_Not_at_Start_of_Buffer.label.xml
Template File: source-sinks-68b.tmpl.c
*/
/*
 * @description
 * CWE: 761 Free Pointer not at Start of Buffer
 * BadSource: file Read input from a file
 * Sinks:
 *    GoodSink: free() memory correctly at the start of the buffer
 *    BadSink : free() memory not at the start of the buffer
 * Flow Variant: 68 Data flow: data passed as a global variable from one function to another in different source files
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

#ifdef _WIN32
#define FILENAME "C:\\temp\\file.txt"
#else
#define FILENAME "/tmp/file.txt"
#endif

#define SEARCH_CHAR 'S'

extern char * CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_file_68_badDataForBadSink;

extern char * CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_file_68_badDataForGoodSink;

#ifndef OMITBAD

void CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_file_68b_badSink()
{
    char * data = CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_file_68_badDataForBadSink;
    /* FLAW: We are incrementing the pointer in the loop - this will cause us to free the
     * memory block not at the start of the buffer */
    for (; *data != '\0'; data++)
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

void CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_file_68b_goodB2GSink()
{
    char * data = CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_file_68_badDataForGoodSink;
    {
        size_t i;
        /* FIX: Use a loop variable to traverse through the string pointed to by data */
        for (i=0; i < strlen(data); i++)
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

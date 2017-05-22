/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_54c.c
Label Definition File: CWE761_Free_Pointer_Not_at_Start_of_Buffer.label.xml
Template File: source-sinks-54c.tmpl.c
*/
/*
 * @description
 * CWE: 761 Free Pointer not at Start of Buffer
 * BadSource: environment Read input from an environment variable
 * Sinks:
 *    GoodSink: free() memory correctly at the start of the buffer
 *    BadSink : free() memory not at the start of the buffer
 * Flow Variant: 54 Data flow: data passed as an argument from one function through three others to a fifth; all five functions are in different source files
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

#define ENV_VARIABLE "ADD"

#ifdef _WIN32
#define GETENV getenv
#else
#define GETENV getenv
#endif

#define SEARCH_CHAR 'S'

#ifndef OMITBAD

/* bad function declaration */
void CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_54d_badSink(char * data);

void CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_54c_badSink(char * data)
{
    CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_54d_badSink(data);
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodB2G uses the BadSource with the GoodSink */
void CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_54d_goodB2GSink(char * data);

void CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_54c_goodB2GSink(char * data)
{
    CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_54d_goodB2GSink(data);
}

#endif /* OMITGOOD */

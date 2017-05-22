/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_67a.c
Label Definition File: CWE761_Free_Pointer_Not_at_Start_of_Buffer.label.xml
Template File: source-sinks-67a.tmpl.c
*/
/*
 * @description
 * CWE: 761 Free Pointer not at Start of Buffer
 * BadSource: environment Read input from an environment variable
 * Sinks:
 *    GoodSink: free() memory correctly at the start of the buffer
 *    BadSink : free() memory not at the start of the buffer
 * Flow Variant: 67 Data flow: data passed in a struct from one function to another in different source files
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

typedef struct _CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_67_structType
{
    char * structFirst;
} CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_67_structType;

#ifndef OMITBAD

/* bad function declaration */
void CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_67b_badSink(CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_67_structType myStruct);

void CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_67_bad()
{
    char * data;
    CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_67_structType myStruct;
    data = (char *)malloc(100*sizeof(char));
    data[0] = '\0';
    {
        /* Append input from an environment variable to data */
        size_t dataLen = strlen(data);
        char * environment = GETENV(ENV_VARIABLE);
        /* If there is data in the environment variable */
        if (environment != NULL)
        {
            /* POTENTIAL FLAW: Read data from an environment variable */
            strncat(data+dataLen, environment, 100-dataLen-1);
        }
    }
    myStruct.structFirst = data;
    CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_67b_badSink(myStruct);
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodB2G uses the BadSource with the GoodSink */
void CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_67b_goodB2GSink(CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_67_structType myStruct);

static void goodB2G()
{
    char * data;
    CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_67_structType myStruct;
    data = (char *)malloc(100*sizeof(char));
    data[0] = '\0';
    {
        /* Append input from an environment variable to data */
        size_t dataLen = strlen(data);
        char * environment = GETENV(ENV_VARIABLE);
        /* If there is data in the environment variable */
        if (environment != NULL)
        {
            /* POTENTIAL FLAW: Read data from an environment variable */
            strncat(data+dataLen, environment, 100-dataLen-1);
        }
    }
    myStruct.structFirst = data;
    CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_67b_goodB2GSink(myStruct);
}

void CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_67_good()
{
    goodB2G();
}

#endif /* OMITGOOD */

/* Below is the main(). It is only used when building this testcase on
   its own for testing or for building a binary to use in testing binary
   analysis tools. It is not used when compiling all the testcases as one
   application, which is how source code analysis tools are tested. */

#ifdef INCLUDEMAIN

int main(int argc, char * argv[])
{
    /* seed randomness */
    srand( (unsigned)time(NULL) );
#ifndef OMITGOOD
    printLine("Calling good()...");
    CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_67_good();
    printLine("Finished good()");
#endif /* OMITGOOD */
#ifndef OMITBAD
    printLine("Calling bad()...");
    CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_67_bad();
    printLine("Finished bad()");
#endif /* OMITBAD */
    return 0;
}

#endif

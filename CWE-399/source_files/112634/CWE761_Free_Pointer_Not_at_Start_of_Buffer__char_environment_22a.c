/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_22a.c
Label Definition File: CWE761_Free_Pointer_Not_at_Start_of_Buffer.label.xml
Template File: source-sinks-22a.tmpl.c
*/
/*
 * @description
 * CWE: 761 Free Pointer not at Start of Buffer
 * BadSource: environment Read input from an environment variable
 * Sinks:
 *    GoodSink: free() memory correctly at the start of the buffer
 *    BadSink : free() memory not at the start of the buffer
 * Flow Variant: 22 Control flow: Flow controlled by value of a global variable. Sink functions are in a separate file from sources.
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

#ifndef OMITBAD

/* The global variable below is used to drive control flow in the sink function */
int CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_22_badGlobal = 0;

void CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_22_badSink(char * data);

void CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_22_bad()
{
    char * data;
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
    CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_22_badGlobal = 1; /* true */
    CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_22_badSink(data);
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* The global variables below are used to drive control flow in the sink functions. */
int CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_22_goodB2G1Global = 0;
int CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_22_goodB2G2Global = 0;

/* goodB2G1() - use badsource and goodsink by setting the static variable to false instead of true */
void CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_22_goodB2G1Sink(char * data);

static void goodB2G1()
{
    char * data;
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
    CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_22_goodB2G1Global = 0; /* false */
    CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_22_goodB2G1Sink(data);
}

/* goodB2G2() - use badsource and goodsink by reversing the blocks in the if in the sink function */
void CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_22_goodB2G2Sink(char * data);

static void goodB2G2()
{
    char * data;
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
    CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_22_goodB2G2Global = 1; /* true */
    CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_22_goodB2G2Sink(data);
}

void CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_22_good()
{
    goodB2G1();
    goodB2G2();
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
    CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_22_good();
    printLine("Finished good()");
#endif /* OMITGOOD */
#ifndef OMITBAD
    printLine("Calling bad()...");
    CWE761_Free_Pointer_Not_at_Start_of_Buffer__char_environment_22_bad();
    printLine("Finished bad()");
#endif /* OMITBAD */
    return 0;
}

#endif

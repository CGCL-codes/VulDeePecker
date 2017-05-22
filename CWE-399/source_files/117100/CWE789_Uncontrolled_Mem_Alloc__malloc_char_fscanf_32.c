/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE789_Uncontrolled_Mem_Alloc__malloc_char_fscanf_32.c
Label Definition File: CWE789_Uncontrolled_Mem_Alloc__malloc.label.xml
Template File: sources-sinks-32.tmpl.c
*/
/*
 * @description
 * CWE: 789 Uncontrolled Memory Allocation
 * BadSource: fscanf Read data from the console using fscanf()
 * GoodSource: Small number greater than zero
 * Sinks:
 *    GoodSink: Allocate memory with malloc() and check the size of the memory to be allocated
 *    BadSink : Allocate memory with malloc(), but incorrectly check the size of the memory to be allocated
 * Flow Variant: 32 Data flow using two pointers to the same value within the same function
 *
 * */

#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

#define HELLO_STRING "hello"

#ifndef OMITBAD

void CWE789_Uncontrolled_Mem_Alloc__malloc_char_fscanf_32_bad()
{
    size_t data;
    size_t *dataPtr1 = &data;
    size_t *dataPtr2 = &data;
    /* Initialize data */
    data = 0;
    {
        size_t data = *dataPtr1;
        /* POTENTIAL FLAW: Read data from the console using fscanf() */
        fscanf(stdin, "%ud", &data);
        *dataPtr1 = data;
    }
    {
        size_t data = *dataPtr2;
        {
            char * myString;
            /* POTENTIAL FLAW: No MAXIMUM limitation for memory allocation, but ensure data is large enough
             * for the strcpy() function to not cause a buffer overflow */
            /* INCIDENTAL FLAW: The source could cause a type overrun in data or in the memory allocation */
            if (data > strlen(HELLO_STRING))
            {
                myString = (char *)malloc(data*sizeof(char));
                /* Copy a small string into myString */
                strcpy(myString, HELLO_STRING);
                printLine(myString);
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

/* goodG2B() uses the GoodSource with the BadSink */
static void goodG2B()
{
    size_t data;
    size_t *dataPtr1 = &data;
    size_t *dataPtr2 = &data;
    /* Initialize data */
    data = 0;
    {
        size_t data = *dataPtr1;
        /* FIX: Use a relatively small number for memory allocation */
        data = 20;
        *dataPtr1 = data;
    }
    {
        size_t data = *dataPtr2;
        {
            char * myString;
            /* POTENTIAL FLAW: No MAXIMUM limitation for memory allocation, but ensure data is large enough
             * for the strcpy() function to not cause a buffer overflow */
            /* INCIDENTAL FLAW: The source could cause a type overrun in data or in the memory allocation */
            if (data > strlen(HELLO_STRING))
            {
                myString = (char *)malloc(data*sizeof(char));
                /* Copy a small string into myString */
                strcpy(myString, HELLO_STRING);
                printLine(myString);
                free(myString);
            }
            else
            {
                printLine("Input is less than the length of the source string");
            }
        }
    }
}

/* goodB2G() uses the BadSource with the GoodSink */
static void goodB2G()
{
    size_t data;
    size_t *dataPtr1 = &data;
    size_t *dataPtr2 = &data;
    /* Initialize data */
    data = 0;
    {
        size_t data = *dataPtr1;
        /* POTENTIAL FLAW: Read data from the console using fscanf() */
        fscanf(stdin, "%ud", &data);
        *dataPtr1 = data;
    }
    {
        size_t data = *dataPtr2;
        {
            char * myString;
            /* FIX: Include a MAXIMUM limitation for memory allocation and a check to ensure data is large enough
             * for the strcpy() function to not cause a buffer overflow */
            /* INCIDENTAL FLAW: The source could cause a type overrun in data or in the memory allocation */
            if (data > strlen(HELLO_STRING) && data < 100)
            {
                myString = (char *)malloc(data*sizeof(char));
                /* Copy a small string into myString */
                strcpy(myString, HELLO_STRING);
                printLine(myString);
                free(myString);
            }
            else
            {
                printLine("Input is less than the length of the source string or too large");
            }
        }
    }
}

void CWE789_Uncontrolled_Mem_Alloc__malloc_char_fscanf_32_good()
{
    goodG2B();
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
    CWE789_Uncontrolled_Mem_Alloc__malloc_char_fscanf_32_good();
    printLine("Finished good()");
#endif /* OMITGOOD */
#ifndef OMITBAD
    printLine("Calling bad()...");
    CWE789_Uncontrolled_Mem_Alloc__malloc_char_fscanf_32_bad();
    printLine("Finished bad()");
#endif /* OMITBAD */
    return 0;
}

#endif

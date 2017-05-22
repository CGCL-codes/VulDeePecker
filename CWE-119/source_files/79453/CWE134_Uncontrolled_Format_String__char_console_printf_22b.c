/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE134_Uncontrolled_Format_String__char_console_printf_22b.c
Label Definition File: CWE134_Uncontrolled_Format_String.label.xml
Template File: sources-sinks-22b.tmpl.c
*/
/*
 * @description
 * CWE: 134 Uncontrolled Format String
 * BadSource: console Read input from the console
 * GoodSource: Copy a fixed string into data
 * Sinks: printf
 *    GoodSink: printf with "%s" as the first argument and data as the second
 *    BadSink : printf with only data as an argument
 * Flow Variant: 22 Control flow: Flow controlled by value of a global variable. Sink functions are in a separate file from sources.
 *
 * */

#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

#ifndef OMITBAD

/* The global variable below is used to drive control flow in the sink function */
extern int CWE134_Uncontrolled_Format_String__char_console_printf_22_badGlobal;

void CWE134_Uncontrolled_Format_String__char_console_printf_22_badSink(char * data)
{
    if(CWE134_Uncontrolled_Format_String__char_console_printf_22_badGlobal)
    {
        /* POTENTIAL FLAW: Do not specify the format allowing a possible format string vulnerability */
        printf(data);
    }
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* The global variables below are used to drive control flow in the sink functions. */
extern int CWE134_Uncontrolled_Format_String__char_console_printf_22_goodB2G1Global;
extern int CWE134_Uncontrolled_Format_String__char_console_printf_22_goodB2G2Global;
extern int CWE134_Uncontrolled_Format_String__char_console_printf_22_goodG2BGlobal;

/* goodB2G1() - use badsource and goodsink by setting the static variable to false instead of true */
void CWE134_Uncontrolled_Format_String__char_console_printf_22_goodB2G1Sink(char * data)
{
    if(CWE134_Uncontrolled_Format_String__char_console_printf_22_goodB2G1Global)
    {
        /* INCIDENTAL: CWE 561 Dead Code, the code below will never run */
        printLine("Benign, fixed string");
    }
    else
    {
        /* FIX: Specify the format disallowing a format string vulnerability */
        printf("%s\n", data);
    }
}

/* goodB2G2() - use badsource and goodsink by reversing the blocks in the if in the sink function */
void CWE134_Uncontrolled_Format_String__char_console_printf_22_goodB2G2Sink(char * data)
{
    if(CWE134_Uncontrolled_Format_String__char_console_printf_22_goodB2G2Global)
    {
        /* FIX: Specify the format disallowing a format string vulnerability */
        printf("%s\n", data);
    }
}

/* goodG2B() - use goodsource and badsink */
void CWE134_Uncontrolled_Format_String__char_console_printf_22_goodG2BSink(char * data)
{
    if(CWE134_Uncontrolled_Format_String__char_console_printf_22_goodG2BGlobal)
    {
        /* POTENTIAL FLAW: Do not specify the format allowing a possible format string vulnerability */
        printf(data);
    }
}

#endif /* OMITGOOD */

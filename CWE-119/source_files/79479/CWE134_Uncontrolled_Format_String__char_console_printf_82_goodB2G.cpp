/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE134_Uncontrolled_Format_String__char_console_printf_82_goodB2G.cpp
Label Definition File: CWE134_Uncontrolled_Format_String.label.xml
Template File: sources-sinks-82_goodB2G.tmpl.cpp
*/
/*
 * @description
 * CWE: 134 Uncontrolled Format String
 * BadSource: console Read input from the console
 * GoodSource: Copy a fixed string into data
 * Sinks: printf
 *    GoodSink: printf with "%s" as the first argument and data as the second
 *    BadSink : printf with only data as an argument
 * Flow Variant: 82 Data flow: data passed in a parameter to an virtual method called via a pointer
 *
 * */
#ifndef OMITGOOD

#include "std_testcase.h"
#include "CWE134_Uncontrolled_Format_String__char_console_printf_82.h"

namespace CWE134_Uncontrolled_Format_String__char_console_printf_82
{

void CWE134_Uncontrolled_Format_String__char_console_printf_82_goodB2G::action(char * data)
{
    /* FIX: Specify the format disallowing a format string vulnerability */
    printf("%s\n", data);
}

}
#endif /* OMITGOOD */

/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE134_Uncontrolled_Format_String__char_console_vfprintf_54b.c
Label Definition File: CWE134_Uncontrolled_Format_String.vasinks.label.xml
Template File: sources-vasinks-54b.tmpl.c
*/
/*
 * @description
 * CWE: 134 Uncontrolled Format String
 * BadSource: console Read input from the console
 * GoodSource: Copy a fixed string into data
 * Sinks: vfprintf
 *    GoodSink: vfprintf with a format string
 *    BadSink : vfprintf without a format string
 * Flow Variant: 54 Data flow: data passed as an argument from one function through three others to a fifth; all five functions are in different source files
 *
 * */

#include <stdarg.h>
#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

#ifndef OMITBAD

/* bad function declaration */
void CWE134_Uncontrolled_Format_String__char_console_vfprintf_54c_badSink(char * data);

void CWE134_Uncontrolled_Format_String__char_console_vfprintf_54b_badSink(char * data)
{
    CWE134_Uncontrolled_Format_String__char_console_vfprintf_54c_badSink(data);
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B uses the GoodSource with the BadSink */
void CWE134_Uncontrolled_Format_String__char_console_vfprintf_54c_goodG2BSink(char * data);

void CWE134_Uncontrolled_Format_String__char_console_vfprintf_54b_goodG2BSink(char * data)
{
    CWE134_Uncontrolled_Format_String__char_console_vfprintf_54c_goodG2BSink(data);
}

/* goodB2G uses the BadSource with the GoodSink */
void CWE134_Uncontrolled_Format_String__char_console_vfprintf_54c_goodB2GSink(char * data);

void CWE134_Uncontrolled_Format_String__char_console_vfprintf_54b_goodB2GSink(char * data)
{
    CWE134_Uncontrolled_Format_String__char_console_vfprintf_54c_goodB2GSink(data);
}

#endif /* OMITGOOD */

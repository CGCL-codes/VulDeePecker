/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE134_Uncontrolled_Format_String__char_console_fprintf_54d.c
Label Definition File: CWE134_Uncontrolled_Format_String.label.xml
Template File: sources-sinks-54d.tmpl.c
*/
/*
 * @description
 * CWE: 134 Uncontrolled Format String
 * BadSource: console Read input from the console
 * GoodSource: Copy a fixed string into data
 * Sinks: fprintf
 *    GoodSink: fprintf with "%s" as the second argument and data as the third
 *    BadSink : fprintf with data as the second argument
 * Flow Variant: 54 Data flow: data passed as an argument from one function through three others to a fifth; all five functions are in different source files
 *
 * */

#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

#ifndef OMITBAD

/* bad function declaration */
void CWE134_Uncontrolled_Format_String__char_console_fprintf_54e_badSink(char * data);

void CWE134_Uncontrolled_Format_String__char_console_fprintf_54d_badSink(char * data)
{
    CWE134_Uncontrolled_Format_String__char_console_fprintf_54e_badSink(data);
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B uses the GoodSource with the BadSink */
void CWE134_Uncontrolled_Format_String__char_console_fprintf_54e_goodG2BSink(char * data);

void CWE134_Uncontrolled_Format_String__char_console_fprintf_54d_goodG2BSink(char * data)
{
    CWE134_Uncontrolled_Format_String__char_console_fprintf_54e_goodG2BSink(data);
}

/* goodB2G uses the BadSource with the GoodSink */
void CWE134_Uncontrolled_Format_String__char_console_fprintf_54e_goodB2GSink(char * data);

void CWE134_Uncontrolled_Format_String__char_console_fprintf_54d_goodB2GSink(char * data)
{
    CWE134_Uncontrolled_Format_String__char_console_fprintf_54e_goodB2GSink(data);
}

#endif /* OMITGOOD */

/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE134_Uncontrolled_Format_String__char_console_snprintf_68b.c
Label Definition File: CWE134_Uncontrolled_Format_String.label.xml
Template File: sources-sinks-68b.tmpl.c
*/
/*
 * @description
 * CWE: 134 Uncontrolled Format String
 * BadSource: console Read input from the console
 * GoodSource: Copy a fixed string into data
 * Sinks: snprintf
 *    GoodSink: snprintf with "%s" as the third argument and data as the fourth
 *    BadSink : snprintf with data as the third argument
 * Flow Variant: 68 Data flow: data passed as a global variable from one function to another in different source files
 *
 * */

#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

#ifdef _WIN32
#define SNPRINTF _snprintf
#else
#define SNPRINTF snprintf
#endif

extern char * CWE134_Uncontrolled_Format_String__char_console_snprintf_68_badData;
extern char * CWE134_Uncontrolled_Format_String__char_console_snprintf_68_goodG2BData;
extern char * CWE134_Uncontrolled_Format_String__char_console_snprintf_68_goodB2GData;

#ifndef OMITBAD

void CWE134_Uncontrolled_Format_String__char_console_snprintf_68b_badSink()
{
    char * data = CWE134_Uncontrolled_Format_String__char_console_snprintf_68_badData;
    {
        char dest[100] = "";
        /* POTENTIAL FLAW: Do not specify the format allowing a possible format string vulnerability */
        SNPRINTF(dest, 100-1, data);
        printLine(dest);
    }
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B uses the GoodSource with the BadSink */
void CWE134_Uncontrolled_Format_String__char_console_snprintf_68b_goodG2BSink()
{
    char * data = CWE134_Uncontrolled_Format_String__char_console_snprintf_68_goodG2BData;
    {
        char dest[100] = "";
        /* POTENTIAL FLAW: Do not specify the format allowing a possible format string vulnerability */
        SNPRINTF(dest, 100-1, data);
        printLine(dest);
    }
}

/* goodB2G uses the BadSource with the GoodSink */
void CWE134_Uncontrolled_Format_String__char_console_snprintf_68b_goodB2GSink()
{
    char * data = CWE134_Uncontrolled_Format_String__char_console_snprintf_68_goodB2GData;
    {
        char dest[100] = "";
        /* FIX: Specify the format disallowing a format string vulnerability */
        SNPRINTF(dest, 100-1, "%s", data);
        printLine(dest);
    }
}

#endif /* OMITGOOD */

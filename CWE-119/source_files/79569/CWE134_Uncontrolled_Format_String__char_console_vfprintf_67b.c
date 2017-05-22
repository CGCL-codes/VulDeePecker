/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE134_Uncontrolled_Format_String__char_console_vfprintf_67b.c
Label Definition File: CWE134_Uncontrolled_Format_String.vasinks.label.xml
Template File: sources-vasinks-67b.tmpl.c
*/
/*
 * @description
 * CWE: 134 Uncontrolled Format String
 * BadSource: console Read input from the console
 * GoodSource: Copy a fixed string into data
 * Sinks: vfprintf
 *    GoodSink: vfprintf with a format string
 *    BadSink : vfprintf without a format string
 * Flow Variant: 67 Data flow: data passed in a struct from one function to another in different source files
 *
 * */

#include <stdarg.h>
#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

typedef struct _CWE134_Uncontrolled_Format_String__char_console_vfprintf_67_structType
{
    char * structFirst;
} CWE134_Uncontrolled_Format_String__char_console_vfprintf_67_structType;

#ifndef OMITBAD

static void badVaSink(char * data, ...)
{
    {
        va_list args;
        va_start(args, data);
        /* POTENTIAL FLAW: Do not specify the format allowing a possible format string vulnerability */
        vfprintf(stdout, data, args);
        va_end(args);
    }
}

void CWE134_Uncontrolled_Format_String__char_console_vfprintf_67b_badSink(CWE134_Uncontrolled_Format_String__char_console_vfprintf_67_structType myStruct)
{
    char * data = myStruct.structFirst;
    badVaSink(data, data);
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B uses the GoodSource with the BadSink */
static void goodG2BVaSink(char * data, ...)
{
    {
        va_list args;
        va_start(args, data);
        /* POTENTIAL FLAW: Do not specify the format allowing a possible format string vulnerability */
        vfprintf(stdout, data, args);
        va_end(args);
    }
}

void CWE134_Uncontrolled_Format_String__char_console_vfprintf_67b_goodG2BSink(CWE134_Uncontrolled_Format_String__char_console_vfprintf_67_structType myStruct)
{
    char * data = myStruct.structFirst;
    goodG2BVaSink(data, data);
}

/* goodB2G uses the BadSource with the GoodSink */
static void goodB2GVaSink(char * data, ...)
{
    {
        va_list args;
        va_start(args, data);
        /* FIX: Specify the format disallowing a format string vulnerability */
        vfprintf(stdout, "%s", args);
        va_end(args);
    }
}

void CWE134_Uncontrolled_Format_String__char_console_vfprintf_67b_goodB2GSink(CWE134_Uncontrolled_Format_String__char_console_vfprintf_67_structType myStruct)
{
    char * data = myStruct.structFirst;
    goodB2GVaSink(data, data);
}

#endif /* OMITGOOD */

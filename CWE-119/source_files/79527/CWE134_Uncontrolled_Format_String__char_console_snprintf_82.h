/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE134_Uncontrolled_Format_String__char_console_snprintf_82.h
Label Definition File: CWE134_Uncontrolled_Format_String.label.xml
Template File: sources-sinks-82.tmpl.h
*/
/*
 * @description
 * CWE: 134 Uncontrolled Format String
 * BadSource: console Read input from the console
 * GoodSource: Copy a fixed string into data
 * Sinks: snprintf
 *    GoodSink: snprintf with "%s" as the third argument and data as the fourth
 *    BadSink : snprintf with data as the third argument
 * Flow Variant: 82 Data flow: data passed in a parameter to an virtual method called via a pointer
 *
 * */

#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

namespace CWE134_Uncontrolled_Format_String__char_console_snprintf_82
{

class CWE134_Uncontrolled_Format_String__char_console_snprintf_82_base
{
public:
    /* pure virtual function */
    virtual void action(char * data) = 0;
};

#ifndef OMITBAD

class CWE134_Uncontrolled_Format_String__char_console_snprintf_82_bad : public CWE134_Uncontrolled_Format_String__char_console_snprintf_82_base
{
public:
    void action(char * data);
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE134_Uncontrolled_Format_String__char_console_snprintf_82_goodG2B : public CWE134_Uncontrolled_Format_String__char_console_snprintf_82_base
{
public:
    void action(char * data);
};

class CWE134_Uncontrolled_Format_String__char_console_snprintf_82_goodB2G : public CWE134_Uncontrolled_Format_String__char_console_snprintf_82_base
{
public:
    void action(char * data);
};

#endif /* OMITGOOD */

}

/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE134_Uncontrolled_Format_String__char_console_fprintf_81.h
Label Definition File: CWE134_Uncontrolled_Format_String.label.xml
Template File: sources-sinks-81.tmpl.h
*/
/*
 * @description
 * CWE: 134 Uncontrolled Format String
 * BadSource: console Read input from the console
 * GoodSource: Copy a fixed string into data
 * Sinks: fprintf
 *    GoodSink: fprintf with "%s" as the second argument and data as the third
 *    BadSink : fprintf with data as the second argument
 * Flow Variant: 81 Data flow: data passed in a parameter to an virtual method called via a reference
 *
 * */

#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

namespace CWE134_Uncontrolled_Format_String__char_console_fprintf_81
{

class CWE134_Uncontrolled_Format_String__char_console_fprintf_81_base
{
public:
    /* pure virtual function */
    virtual void action(char * data) const = 0;
};

#ifndef OMITBAD

class CWE134_Uncontrolled_Format_String__char_console_fprintf_81_bad : public CWE134_Uncontrolled_Format_String__char_console_fprintf_81_base
{
public:
    void action(char * data) const;
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE134_Uncontrolled_Format_String__char_console_fprintf_81_goodG2B : public CWE134_Uncontrolled_Format_String__char_console_fprintf_81_base
{
public:
    void action(char * data) const;
};

class CWE134_Uncontrolled_Format_String__char_console_fprintf_81_goodB2G : public CWE134_Uncontrolled_Format_String__char_console_fprintf_81_base
{
public:
    void action(char * data) const;
};

#endif /* OMITGOOD */

}

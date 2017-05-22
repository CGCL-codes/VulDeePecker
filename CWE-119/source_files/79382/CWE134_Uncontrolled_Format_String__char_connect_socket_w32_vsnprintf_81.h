/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_81.h
Label Definition File: CWE134_Uncontrolled_Format_String.vasinks.label.xml
Template File: sources-vasinks-81.tmpl.h
*/
/*
 * @description
 * CWE: 134 Uncontrolled Format String
 * BadSource: connect_socket Read data using a connect socket (client side)
 * GoodSource: Copy a fixed string into data
 * Sinks: w32_vsnprintf
 *    GoodSink: vsnprintf with a format string
 *    BadSink : vsnprintf without a format string
 * Flow Variant: 81 Data flow: data passed in a parameter to an virtual method called via a reference
 *
 * */

#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

namespace CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_81
{

class CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_81_base
{
public:
    /* pure virtual function */
    virtual void action(char * data) const = 0;
};

#ifndef OMITBAD

class CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_81_bad : public CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_81_base
{
public:
    void action(char * data) const;
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_81_goodG2B : public CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_81_base
{
public:
    void action(char * data) const;
};

class CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_81_goodB2G : public CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_81_base
{
public:
    void action(char * data) const;
};

#endif /* OMITGOOD */

}

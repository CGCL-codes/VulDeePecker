/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_82.h
Label Definition File: CWE134_Uncontrolled_Format_String.vasinks.label.xml
Template File: sources-vasinks-82.tmpl.h
*/
/*
 * @description
 * CWE: 134 Uncontrolled Format String
 * BadSource: connect_socket Read data using a connect socket (client side)
 * GoodSource: Copy a fixed string into data
 * Sinks: w32_vsnprintf
 *    GoodSink: vsnprintf with a format string
 *    BadSink : vsnprintf without a format string
 * Flow Variant: 82 Data flow: data passed in a parameter to an virtual method called via a pointer
 *
 * */

#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

namespace CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_82
{

class CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_82_base
{
public:
    /* pure virtual function */
    virtual void action(char * data) = 0;
};

#ifndef OMITBAD

class CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_82_bad : public CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_82_base
{
public:
    void action(char * data);
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_82_goodG2B : public CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_82_base
{
public:
    void action(char * data);
};

class CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_82_goodB2G : public CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_82_base
{
public:
    void action(char * data);
};

#endif /* OMITGOOD */

}

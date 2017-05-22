/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE124_Buffer_Underwrite__wchar_t_declare_cpy_81.h
Label Definition File: CWE124_Buffer_Underwrite.stack.label.xml
Template File: sources-sink-81.tmpl.h
*/
/*
 * @description
 * CWE: 124 Buffer Underwrite
 * BadSource:  Set data pointer to before the allocated memory buffer
 * GoodSource: Set data pointer to the allocated memory buffer
 * Sinks: cpy
 *    BadSink : Copy string to data using wcscpy
 * Flow Variant: 81 Data flow: data passed in a parameter to an virtual method called via a reference
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

namespace CWE124_Buffer_Underwrite__wchar_t_declare_cpy_81
{

class CWE124_Buffer_Underwrite__wchar_t_declare_cpy_81_base
{
public:
    /* pure virtual function */
    virtual void action(wchar_t * data) const = 0;
};

#ifndef OMITBAD

class CWE124_Buffer_Underwrite__wchar_t_declare_cpy_81_bad : public CWE124_Buffer_Underwrite__wchar_t_declare_cpy_81_base
{
public:
    void action(wchar_t * data) const;
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE124_Buffer_Underwrite__wchar_t_declare_cpy_81_goodG2B : public CWE124_Buffer_Underwrite__wchar_t_declare_cpy_81_base
{
public:
    void action(wchar_t * data) const;
};

#endif /* OMITGOOD */

}

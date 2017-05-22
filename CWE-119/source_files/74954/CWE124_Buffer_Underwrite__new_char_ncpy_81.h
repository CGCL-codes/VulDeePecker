/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE124_Buffer_Underwrite__new_char_ncpy_81.h
Label Definition File: CWE124_Buffer_Underwrite__new.label.xml
Template File: sources-sink-81.tmpl.h
*/
/*
 * @description
 * CWE: 124 Buffer Underwrite
 * BadSource:  Set data pointer to before the allocated memory buffer
 * GoodSource: Set data pointer to the allocated memory buffer
 * Sinks: ncpy
 *    BadSink : Copy string to data using strncpy
 * Flow Variant: 81 Data flow: data passed in a parameter to a virtual method called via a reference
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

namespace CWE124_Buffer_Underwrite__new_char_ncpy_81
{

class CWE124_Buffer_Underwrite__new_char_ncpy_81_base
{
public:
    /* pure virtual function */
    virtual void action(char * data) const = 0;
};

#ifndef OMITBAD

class CWE124_Buffer_Underwrite__new_char_ncpy_81_bad : public CWE124_Buffer_Underwrite__new_char_ncpy_81_base
{
public:
    void action(char * data) const;
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE124_Buffer_Underwrite__new_char_ncpy_81_goodG2B : public CWE124_Buffer_Underwrite__new_char_ncpy_81_base
{
public:
    void action(char * data) const;
};

#endif /* OMITGOOD */

}

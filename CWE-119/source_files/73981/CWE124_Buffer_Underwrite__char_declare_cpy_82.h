/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE124_Buffer_Underwrite__char_declare_cpy_82.h
Label Definition File: CWE124_Buffer_Underwrite.stack.label.xml
Template File: sources-sink-82.tmpl.h
*/
/*
 * @description
 * CWE: 124 Buffer Underwrite
 * BadSource:  Set data pointer to before the allocated memory buffer
 * GoodSource: Set data pointer to the allocated memory buffer
 *    BadSink : Copy string to data using strcpy
 * Flow Variant: 82 Data flow: data passed in a parameter to an virtual method called via a pointer
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

namespace CWE124_Buffer_Underwrite__char_declare_cpy_82
{

class CWE124_Buffer_Underwrite__char_declare_cpy_82_base
{
public:
    /* pure virtual function */
    virtual void action(char * data) = 0;
};

#ifndef OMITBAD

class CWE124_Buffer_Underwrite__char_declare_cpy_82_bad : public CWE124_Buffer_Underwrite__char_declare_cpy_82_base
{
public:
    void action(char * data);
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE124_Buffer_Underwrite__char_declare_cpy_82_goodG2B : public CWE124_Buffer_Underwrite__char_declare_cpy_82_base
{
public:
    void action(char * data);
};

#endif /* OMITGOOD */

}

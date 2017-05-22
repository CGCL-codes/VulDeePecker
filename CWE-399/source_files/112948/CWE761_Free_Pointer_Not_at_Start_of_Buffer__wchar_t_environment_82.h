/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_environment_82.h
Label Definition File: CWE761_Free_Pointer_Not_at_Start_of_Buffer.label.xml
Template File: source-sinks-82.tmpl.h
*/
/*
 * @description
 * CWE: 761 Free Pointer not at Start of Buffer
 * BadSource: environment Read input from an environment variable
 * Sinks:
 *    GoodSink: free() memory correctly at the start of the buffer
 *    BadSink : free() memory not at the start of the buffer
 * Flow Variant: 82 Data flow: data passed in a parameter to an virtual method called via a pointer
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

namespace CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_environment_82
{

class CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_environment_82_base
{
public:
    /* pure virtual function */
    virtual void action(wchar_t * data) = 0;
};

#ifndef OMITBAD

class CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_environment_82_bad : public CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_environment_82_base
{
public:
    void action(wchar_t * data);
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_environment_82_goodB2G : public CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_environment_82_base
{
public:
    void action(wchar_t * data);
};

#endif /* OMITGOOD */

}

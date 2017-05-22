/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close_82.h
Label Definition File: CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close.label.xml
Template File: source-sinks-82.tmpl.h
*/
/*
 * @description
 * CWE: 775 Missing Release of File Descriptor or Handle After Effective Lifetime
 * BadSource:  Open a file using open()
 * Sinks:
 *    GoodSink: Close the file using close()
 *    BadSink : Do not close file
 * Flow Variant: 82 Data flow: data passed in a parameter to an virtual method called via a pointer
 *
 * */

#include "std_testcase.h"

#ifdef _WIN32
#define OPEN _open
#define CLOSE _close
#else
#include <unistd.h>
#define OPEN open
#define CLOSE close
#endif

namespace CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close_82
{

class CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close_82_base
{
public:
    /* pure virtual function */
    virtual void action(int data) = 0;
};

#ifndef OMITBAD

class CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close_82_bad : public CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close_82_base
{
public:
    void action(int data);
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close_82_goodB2G : public CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close_82_base
{
public:
    void action(int data);
};

#endif /* OMITGOOD */

}

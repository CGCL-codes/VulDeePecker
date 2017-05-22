/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_82.h
Label Definition File: CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open.label.xml
Template File: source-sinks-82.tmpl.h
*/
/*
 * @description
 * CWE: 773 Missing Reference to Active File Descriptor or Handle
 * BadSource:  Create a file descriptor using open()
 * Sinks:
 *    GoodSink: Close the file before reusing the file descriptor
 *    BadSink : Reassign the file descriptor before closing the file
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

namespace CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_82
{

class CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_82_base
{
public:
    /* pure virtual function */
    virtual void action(int data) = 0;
};

#ifndef OMITBAD

class CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_82_bad : public CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_82_base
{
public:
    void action(int data);
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_82_goodB2G : public CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_82_base
{
public:
    void action(int data);
};

#endif /* OMITGOOD */

}

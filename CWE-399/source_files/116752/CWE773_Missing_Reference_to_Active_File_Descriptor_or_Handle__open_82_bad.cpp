/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_82_bad.cpp
Label Definition File: CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open.label.xml
Template File: source-sinks-82_bad.tmpl.cpp
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
#ifndef OMITBAD

#include "std_testcase.h"
#include "CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_82.h"

namespace CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_82
{

void CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_82_bad::action(int data)
{
    /* FLAW: Assign data to another file descriptor without closing the descriptor from the source */
    data = OPEN("BadSink_open.txt", O_RDWR|O_CREAT, S_IREAD|S_IWRITE);
    /* avoid incidental for not closing the file */
    if (data != -1)
    {
        CLOSE(data);
    }
}

}
#endif /* OMITBAD */

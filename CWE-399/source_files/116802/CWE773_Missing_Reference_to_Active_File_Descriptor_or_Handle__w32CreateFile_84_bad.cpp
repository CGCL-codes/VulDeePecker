/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__w32CreateFile_84_bad.cpp
Label Definition File: CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__w32CreateFile.label.xml
Template File: source-sinks-84_bad.tmpl.cpp
*/
/*
 * @description
 * CWE: 773 Missing Reference to Active File Descriptor or Handle
 * BadSource:  Create a file handle using CreateFile()
 * Sinks:
 *    GoodSink: Close the file handle before reusing it
 *    BadSink : Reassign the file handle before closing it
 * Flow Variant: 84 Data flow: data passed to class constructor and destructor by declaring the class object on the heap and deleting it after use
 *
 * */
#ifndef OMITBAD

#include "std_testcase.h"
#include "CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__w32CreateFile_84.h"

namespace CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__w32CreateFile_84
{
CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__w32CreateFile_84_bad::CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__w32CreateFile_84_bad(HANDLE dataCopy)
{
    data = dataCopy;
    /* POTENTIAL FLAW: Create a file handle using CreateFile() that may not be closed properly */
    data = CreateFile("BadSource_w32CreateFile.txt",
                      (GENERIC_WRITE|GENERIC_READ),
                      0,
                      NULL,
                      OPEN_ALWAYS,
                      FILE_ATTRIBUTE_NORMAL,
                      NULL);
}

CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__w32CreateFile_84_bad::~CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__w32CreateFile_84_bad()
{
    /* FLAW: Point data to another file handle without closing the handle from the source */
    data = CreateFile("BadSink_w32CreateFile.txt",
                      (GENERIC_WRITE|GENERIC_READ),
                      0,
                      NULL,
                      OPEN_ALWAYS,
                      FILE_ATTRIBUTE_NORMAL,
                      NULL);
    /* avoid incidental for not closing the file handle */
    if (data != INVALID_HANDLE_VALUE)
    {
        CloseHandle(data);
    }
}
}
#endif /* OMITBAD */

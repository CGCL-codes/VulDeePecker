/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE775_Missing_Release_of_File_Descriptor_or_Handle__w32CreateFile_no_close_84_goodB2G.cpp
Label Definition File: CWE775_Missing_Release_of_File_Descriptor_or_Handle__w32CreateFile_no_close.label.xml
Template File: source-sinks-84_goodB2G.tmpl.cpp
*/
/*
 * @description
 * CWE: 775 Missing Release of File Descriptor or Handle After Effective Lifetime
 * BadSource:  Open a file using CreateFile()
 * Sinks:
 *    GoodSink: Close the file using CloseHandle()
 *    BadSink : Do not close file
 * Flow Variant: 84 Data flow: data passed to class constructor and destructor by declaring the class object on the heap and deleting it after use
 *
 * */
#ifndef OMITGOOD

#include "std_testcase.h"
#include "CWE775_Missing_Release_of_File_Descriptor_or_Handle__w32CreateFile_no_close_84.h"

namespace CWE775_Missing_Release_of_File_Descriptor_or_Handle__w32CreateFile_no_close_84
{
CWE775_Missing_Release_of_File_Descriptor_or_Handle__w32CreateFile_no_close_84_goodB2G::CWE775_Missing_Release_of_File_Descriptor_or_Handle__w32CreateFile_no_close_84_goodB2G(HANDLE dataCopy)
{
    data = dataCopy;
    /* POTENTIAL FLAW: Open a file without closing it */
    data = CreateFile("BadSource_w32CreateFile.txt",
                      (GENERIC_WRITE|GENERIC_READ),
                      0,
                      NULL,
                      OPEN_ALWAYS,
                      FILE_ATTRIBUTE_NORMAL,
                      NULL);
}

CWE775_Missing_Release_of_File_Descriptor_or_Handle__w32CreateFile_no_close_84_goodB2G::~CWE775_Missing_Release_of_File_Descriptor_or_Handle__w32CreateFile_no_close_84_goodB2G()
{
    /* FIX: If the file is still opened, close it */
    if (data != INVALID_HANDLE_VALUE)
    {
        CloseHandle(data);
    }
}
}
#endif /* OMITGOOD */

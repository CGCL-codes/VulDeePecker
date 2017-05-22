/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close_84_bad.cpp
Label Definition File: CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close.label.xml
Template File: source-sinks-84_bad.tmpl.cpp
*/
/*
 * @description
 * CWE: 775 Missing Release of File Descriptor or Handle After Effective Lifetime
 * BadSource:  Open a file using open()
 * Sinks:
 *    GoodSink: Close the file using close()
 *    BadSink : Do not close file
 * Flow Variant: 84 Data flow: data passed to class constructor and destructor by declaring the class object on the heap and deleting it after use
 *
 * */
#ifndef OMITBAD

#include "std_testcase.h"
#include "CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close_84.h"

namespace CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close_84
{
CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close_84_bad::CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close_84_bad(int dataCopy)
{
    data = dataCopy;
    /* POTENTIAL FLAW: Open a file without closing it */
    data = OPEN("BadSource_open.txt", O_RDWR|O_CREAT, S_IREAD|S_IWRITE);
}

CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close_84_bad::~CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close_84_bad()
{
    /* FLAW: No attempt to close the file */
    ; /* empty statement needed for some flow variants */
}
}
#endif /* OMITBAD */

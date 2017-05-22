/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE775_Missing_Release_of_File_Descriptor_or_Handle__fopen_no_close_84_goodB2G.cpp
Label Definition File: CWE775_Missing_Release_of_File_Descriptor_or_Handle__fopen_no_close.label.xml
Template File: source-sinks-84_goodB2G.tmpl.cpp
*/
/*
 * @description
 * CWE: 775 Missing Release of File Descriptor or Handle After Effective Lifetime
 * BadSource:  Open a file using fopen()
 * Sinks:
 *    GoodSink: Close the file using fclose()
 *    BadSink : Do not close file
 * Flow Variant: 84 Data flow: data passed to class constructor and destructor by declaring the class object on the heap and deleting it after use
 *
 * */
#ifndef OMITGOOD

#include "std_testcase.h"
#include "CWE775_Missing_Release_of_File_Descriptor_or_Handle__fopen_no_close_84.h"

namespace CWE775_Missing_Release_of_File_Descriptor_or_Handle__fopen_no_close_84
{
CWE775_Missing_Release_of_File_Descriptor_or_Handle__fopen_no_close_84_goodB2G::CWE775_Missing_Release_of_File_Descriptor_or_Handle__fopen_no_close_84_goodB2G(FILE * dataCopy)
{
    data = dataCopy;
    /* POTENTIAL FLAW: Open a file without closing it */
    data = fopen("BadSource_fopen.txt", "w+");
}

CWE775_Missing_Release_of_File_Descriptor_or_Handle__fopen_no_close_84_goodB2G::~CWE775_Missing_Release_of_File_Descriptor_or_Handle__fopen_no_close_84_goodB2G()
{
    /* FIX: If the file is still opened, close it */
    if (data != NULL)
    {
        fclose(data);
    }
}
}
#endif /* OMITGOOD */

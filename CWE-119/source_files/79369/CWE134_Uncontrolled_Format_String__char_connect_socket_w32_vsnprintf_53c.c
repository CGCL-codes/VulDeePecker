/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_53c.c
Label Definition File: CWE134_Uncontrolled_Format_String.vasinks.label.xml
Template File: sources-vasinks-53c.tmpl.c
*/
/*
 * @description
 * CWE: 134 Uncontrolled Format String
 * BadSource: connect_socket Read data using a connect socket (client side)
 * GoodSource: Copy a fixed string into data
 * Sinks: w32_vsnprintf
 *    GoodSink: vsnprintf with a format string
 *    BadSink : vsnprintf without a format string
 * Flow Variant: 53 Data flow: data passed as an argument from one function through two others to a fourth; all four functions are in different source files
 *
 * */

#include <stdarg.h>
#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <direct.h>
#pragma comment(lib, "ws2_32") /* include ws2_32.lib when linking */
#define CLOSE_SOCKET closesocket
#else /* NOT _WIN32 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define CLOSE_SOCKET close
#define SOCKET int
#endif

#define TCP_PORT 27015
#define IP_ADDRESS "127.0.0.1"

#ifndef OMITBAD

/* bad function declaration */
void CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_53d_badSink(char * data);

void CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_53c_badSink(char * data)
{
    CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_53d_badSink(data);
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B uses the GoodSource with the BadSink */
void CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_53d_goodG2BSink(char * data);

void CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_53c_goodG2BSink(char * data)
{
    CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_53d_goodG2BSink(data);
}

/* goodB2G uses the BadSource with the GoodSink */
void CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_53d_goodB2GSink(char * data);

void CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_53c_goodB2GSink(char * data)
{
    CWE134_Uncontrolled_Format_String__char_connect_socket_w32_vsnprintf_53d_goodB2GSink(data);
}

#endif /* OMITGOOD */

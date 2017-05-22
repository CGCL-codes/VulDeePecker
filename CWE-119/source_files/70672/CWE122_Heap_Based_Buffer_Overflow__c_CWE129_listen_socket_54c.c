/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE122_Heap_Based_Buffer_Overflow__c_CWE129_listen_socket_54c.c
Label Definition File: CWE122_Heap_Based_Buffer_Overflow__c_CWE129.label.xml
Template File: sources-sinks-54c.tmpl.c
*/
/*
 * @description
 * CWE: 122 Heap Based Buffer Overflow
 * BadSource: listen_socket Read data using a listen socket (server side)
 * GoodSource: Larger than zero but less than 10
 * Sinks:
 *    GoodSink: Ensure the array index is valid
 *    BadSink : Improperly check the array index by not checking the upper bound
 * Flow Variant: 54 Data flow: data passed as an argument from one function through three others to a fifth; all five functions are in different source files
 *
 * */

#include "std_testcase.h"

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <direct.h>
#pragma comment(lib, "ws2_32") /* include ws2_32.lib when linking */
#define CLOSE_SOCKET closesocket
#else
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
#define LISTEN_BACKLOG 5
#define CHAR_ARRAY_SIZE (3 * sizeof(data) + 2)

#ifndef OMITBAD

/* bad function declaration */
void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_listen_socket_54d_badSink(int data);

void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_listen_socket_54c_badSink(int data)
{
    CWE122_Heap_Based_Buffer_Overflow__c_CWE129_listen_socket_54d_badSink(data);
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B uses the GoodSource with the BadSink */
void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_listen_socket_54d_goodG2BSink(int data);

void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_listen_socket_54c_goodG2BSink(int data)
{
    CWE122_Heap_Based_Buffer_Overflow__c_CWE129_listen_socket_54d_goodG2BSink(data);
}

/* goodB2G uses the BadSource with the GoodSink */
void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_listen_socket_54d_goodB2GSink(int data);

void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_listen_socket_54c_goodB2GSink(int data)
{
    CWE122_Heap_Based_Buffer_Overflow__c_CWE129_listen_socket_54d_goodB2GSink(data);
}

#endif /* OMITGOOD */

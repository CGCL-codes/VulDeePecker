/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE123_Write_What_Where_Condition__connect_socket_43.cpp
Label Definition File: CWE123_Write_What_Where_Condition.label.xml
Template File: sources-sink-43.tmpl.cpp
*/
/*
 * @description
 * CWE: 123 Write-What-Where Condition
 * BadSource: connect_socket Overwrite linked list pointers using a connect socket (client side)
 * GoodSource: Don't overwrite linked list pointers
 * Sinks:
 *    BadSink : Remove element from list
 * Flow Variant: 43 Data flow: data flows using a C++ reference from one function to another in the same source file
 *
 * */

#include "std_testcase.h"

typedef struct _linkedList
{
    struct _linkedList *next;
    struct _linkedList *prev;
} linkedList;

typedef struct _badStruct
{
    linkedList list;
} badStruct;

static linkedList *linkedListPrev, *linkedListNext;

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

namespace CWE123_Write_What_Where_Condition__connect_socket_43
{

#ifndef OMITBAD

static void badSource(badStruct &data)
{
    {
#ifdef _WIN32
        WSADATA wsaData;
        int wsaDataInit = 0;
#endif
        int recvResult;
        struct sockaddr_in service;
        SOCKET connectSocket = INVALID_SOCKET;
        do
        {
#ifdef _WIN32
            if (WSAStartup(MAKEWORD(2,2), &wsaData) != NO_ERROR)
            {
                break;
            }
            wsaDataInit = 1;
#endif
            connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (connectSocket == INVALID_SOCKET)
            {
                break;
            }
            memset(&service, 0, sizeof(service));
            service.sin_family = AF_INET;
            service.sin_addr.s_addr = inet_addr(IP_ADDRESS);
            service.sin_port = htons(TCP_PORT);
            if (connect(connectSocket, (struct sockaddr*)&service, sizeof(service)) == SOCKET_ERROR)
            {
                break;
            }
            /* Abort on error or the connection was closed, make sure to recv one
             * less char than is in the recv_buf in order to append a terminator */
            /* FLAW: overwrite linked list pointers with data */
            recvResult = recv(connectSocket, (char*)&data, sizeof(data), 0);
            if (recvResult == SOCKET_ERROR || recvResult == 0)
            {
                break;
            }
        }
        while (0);
        if (connectSocket != INVALID_SOCKET)
        {
            CLOSE_SOCKET(connectSocket);
        }
#ifdef _WIN32
        if (wsaDataInit)
        {
            WSACleanup();
        }
#endif
    }
}

void bad()
{
    badStruct data;
    linkedList head = { &head, &head };
    /* This simulates a Microsoft-style linked list insertion */
    data.list.next = head.next;
    data.list.prev = head.prev;
    head.next = &data.list;
    head.prev = &data.list;
    badSource(data);
    /* POTENTIAL FLAW: The following removes 'a' from the list.  Because of the possible overflow this
     * causes a "write-what-where" aka "write4".  It does another write as
     * well.  But this is the prototypical "write-what-where" at least from
     * the Windows perspective.
     *
     * linkedListPrev = a->list->prev  WHAT
     * linkedListNext = a->list->next  WHERE
     * linkedListPrev->next = linkedListNext  "at the address that prev/WHERE points, write
     *                    next/WHAT"
     *                    aka "write-what-where"
     * linkedListNext->prev = linkedListPrev  "at the address that next/WHAT points plus 4
     *                    (because prev is the second field in 'list' hence
     *                    4 bytes away on 32b machines), write prev/WHERE"
     */
    linkedListPrev = data.list.prev;
    linkedListNext = data.list.next;
    linkedListPrev->next = linkedListNext;
    linkedListNext->prev = linkedListPrev;
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B() uses the GoodSource with the BadSink */
static void goodG2BSource(badStruct &data)
{
    /* FIX: don't overwrite linked list pointers */
    ; /* empty statement needed by some flow variants */
}

static void goodG2B()
{
    badStruct data;
    linkedList head = { &head, &head };
    /* This simulates a Microsoft-style linked list insertion */
    data.list.next = head.next;
    data.list.prev = head.prev;
    head.next = &data.list;
    head.prev = &data.list;
    goodG2BSource(data);
    /* POTENTIAL FLAW: The following removes 'a' from the list.  Because of the possible overflow this
     * causes a "write-what-where" aka "write4".  It does another write as
     * well.  But this is the prototypical "write-what-where" at least from
     * the Windows perspective.
     *
     * linkedListPrev = a->list->prev  WHAT
     * linkedListNext = a->list->next  WHERE
     * linkedListPrev->next = linkedListNext  "at the address that prev/WHERE points, write
     *                    next/WHAT"
     *                    aka "write-what-where"
     * linkedListNext->prev = linkedListPrev  "at the address that next/WHAT points plus 4
     *                    (because prev is the second field in 'list' hence
     *                    4 bytes away on 32b machines), write prev/WHERE"
     */
    linkedListPrev = data.list.prev;
    linkedListNext = data.list.next;
    linkedListPrev->next = linkedListNext;
    linkedListNext->prev = linkedListPrev;
}

void good()
{
    goodG2B();
}

#endif /* OMITGOOD */

} /* close namespace */

/* Below is the main(). It is only used when building this testcase on
 * its own for testing or for building a binary to use in testing binary
 * analysis tools. It is not used when compiling all the testcases as one
 * application, which is how source code analysis tools are tested.
 */

#ifdef INCLUDEMAIN

using namespace CWE123_Write_What_Where_Condition__connect_socket_43; /* so that we can use good and bad easily */

int main(int argc, char * argv[])
{
    /* seed randomness */
    srand( (unsigned)time(NULL) );
#ifndef OMITGOOD
    printLine("Calling good()...");
    good();
    printLine("Finished good()");
#endif /* OMITGOOD */
#ifndef OMITBAD
    printLine("Calling bad()...");
    bad();
    printLine("Finished bad()");
#endif /* OMITBAD */
    return 0;
}

#endif

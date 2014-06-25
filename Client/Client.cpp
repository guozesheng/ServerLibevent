// Client.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#pragma comment(lib, "ws2_32.lib") 

SOCKET connectServer(const char *hostip, u_short port);
void connectionClose(SOCKET s);
int SendtoServer(SOCKET s, const char *buf, int buflen);
int RecvFromServer(SOCKET s, char *buf, int buflen);

int _tmain(int argc, _TCHAR* argv[])
{
    const int BUF_SIZE = 64;  
    const char *SERVERADDR = "127.0.0.1";
    const u_short PORT = 5222;

    int recvNum = 0;
    char *buf = "hello, world";
    char bufRecv[BUF_SIZE];
    SOCKET sHost;

    sHost = connectServer(SERVERADDR, PORT);
    if (sHost == NULL)
    {
        return -1;
    }

    if (SendtoServer(sHost, buf, strlen(buf) + 1))
    {
        return -1;
    }

    recvNum = RecvFromServer(sHost, bufRecv, BUF_SIZE);
    if (recv < 0)
    {
        return -1;
    }
    if (recvNum == BUF_SIZE)
    {
        recvNum = BUF_SIZE - 1;
    }
    bufRecv[recvNum] = '\0';
    printf("RECV: %s\n", bufRecv);

    connectionClose(sHost);
    getchar();
	return 0;
}

/***
    hostip: �����IP
    port:   ����˶˿�

    return: Socket
*/
SOCKET connectServer(const char *hostip, u_short port)
{
    WSADATA         wsd;            //WSADATA����  
    SOCKET          sHost;          //�������׽���  
    SOCKADDR_IN     servAddr;       //��������ַ  

    //��ʼ���׽��ֶ�̬��  
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)  
    {  
        perror("WSAStartup failed");
        return NULL;  
    }  
      
    //�����׽���  
    sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);    
    if(INVALID_SOCKET == sHost)  
    {  
        perror("socket failed");
        WSACleanup();//�ͷ��׽�����Դ  
        return  NULL;  
    }  

    //���÷�������ַ  
    servAddr.sin_family =AF_INET;  
    servAddr.sin_addr.s_addr = inet_addr(hostip);  
    servAddr.sin_port = htons(port);  
    int nServAddlen  = sizeof(servAddr);  

    //���ӷ�����  
    if (connect(sHost,(LPSOCKADDR)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
    {
        perror("connect failed");
        closesocket(sHost); //�ر��׽���  
        WSACleanup();       //�ͷ��׽�����Դ  
        return NULL;  
    }

    return sHost;
}

/***
    s: Ҫ�رյ�socket
*/
void connectionClose(SOCKET s)
{
    closesocket(s); // �ر��׽���
    WSACleanup();       //�ͷ��׽�����Դ  
}

/***
    s:      socket
    buf:    �����͵�����
    buflen: ���ݳ���

    return: 0��������ͳɹ�
*/
int SendtoServer(SOCKET s, const char *buf, int buflen)
{
    if (send(s, buf, buflen, 0) == SOCKET_ERROR)
    {
        perror("send failed");
        connectionClose(s);
        return -1;
    }

    return 0;
}

/***
    s:      socket
    buf:    �������ݻ�����
    buflen: ����������

    return: ���յ������ݳ��ȣ�С��0Ϊ���մ���
*/
int RecvFromServer(SOCKET s, char *buf, int buflen)
{
    int n = 0;

    n = recv(s, buf, buflen, 0);
    if (n < 0)
    {
        perror("recv failed");
        connectionClose(s);
    }

    return n;
}
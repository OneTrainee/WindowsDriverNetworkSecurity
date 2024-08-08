#if 1
#include <iostream>
#include <WinSock2.h>
#pragma comment(lib,"WS2_32") // ���ӵ�WS2_32.lib
#define _WINSOCK_DEPRECATED_NO_WARNINGS
int init_socket(BYTE minorVer = 2, BYTE majorVer = 2) {
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(minorVer, majorVer);
	if (::WSAStartup(sockVersion, &wsaData) != 0) {
		return -1;
	}

	return 0;
}

void close_sock() {
	::WSACleanup();
}

int main() {
	do {
		if (init_socket(2, 2) != 0) {
			break;
		}

		SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (s == INVALID_SOCKET) {
			printf("Failed socket() \n");
			break;
		}

		sockaddr_in servAddr;
		servAddr.sin_family = AF_INET;
		servAddr.sin_port = htons(40007);
		servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

		if (::connect(s, (LPSOCKADDR)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
			printf("Failed connect() \n");
			break;
		}
		
        const char* helloWorld = "Hello World!\n";
        send(s, helloWorld, strlen(helloWorld), 0);
		char buff[256];
		int nRecv = ::recv(s, buff, 256, 0);
		if (nRecv > 0) {
			buff[nRecv] = '\0';
			printf("Receive Data: %s\n", buff);
		}
		else {
			printf("δ�յ�����\n");
		}
		::closesocket(s);
	} while (0);

	return 0;
}
#else

#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")  // Winsock ��

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    const char* message;

    // ��ʼ�� Winsock
    printf("��ʼ�� Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("��ʼ��ʧ��. ������: %d\n", WSAGetLastError());
        return 1;
    }
    printf("��ʼ���ɹ�.\n");

    // �����ͻ����׽���
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("�����׽���ʧ��. ������: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    printf("�׽��ִ����ɹ�.\n");

    // ׼�� sockaddr_in �ṹ
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // ������ IP ��ַ
    server_addr.sin_port = htons(1233);

    // ���ӷ�����
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("����ʧ��. ������: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }
    printf("���ӳɹ�.\n");

    // ��������
    message = "Hello, Server!";
    if (send(client_socket, message, strlen(message), 0) < 0) {
        printf("����ʧ��. ������: %d\n", WSAGetLastError());
    }
    else {
        printf("��Ϣ����: %s\n", message);
    }

    // �ر��׽���
    closesocket(client_socket);
    WSACleanup();
    return 0;
}

#endif

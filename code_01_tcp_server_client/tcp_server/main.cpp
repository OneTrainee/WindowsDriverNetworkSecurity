#if 1
#include <iostream>
#include <WinSock2.h>
#pragma comment(lib,"WS2_32") // 链接到WS2_32.lib
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

		// 创建套接字
		SOCKET sListen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sListen == INVALID_SOCKET) {
			printf("Failed socket() \n");
			break;
		}

		// 绑定端口
		sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_port = htons(1233);
		sin.sin_addr.S_un.S_addr = INADDR_ANY;
		if (::bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR) {
			printf("Failed bind() \n");
			break;
		}
		if (::listen(sListen, 3) == SOCKET_ERROR) {
			printf("Failed listen() \n");
			break;
		}

		sockaddr_in remoteAddr;
		int nAddrLen = sizeof(remoteAddr);
		SOCKET sClient;
		char szText[] = "Tcp Server Demo! \r\n";
		while (TRUE) {
			sClient = ::accept(sListen, (SOCKADDR*)&remoteAddr, &nAddrLen);

			if (sClient == INVALID_SOCKET) {
				printf("Failed accept()");
				continue;
			}

			printf("Receive a connection : %s\r\n", inet_ntoa(remoteAddr.sin_addr));
			::send(sClient, szText, strlen(szText), 0);
			::closesocket(sClient);
		}
		::closesocket(sListen);
	} while (0);
}
#else

#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")  // Winsock 库

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_size;
    char buffer[1024];

    // 初始化 Winsock
    printf("初始化 Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("初始化失败. 错误码: %d\n", WSAGetLastError());
        return 1;
    }
    printf("初始化成功.\n");

    // 创建服务器套接字
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("创建套接字失败. 错误码: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    printf("套接字创建成功.\n");

    // 准备 sockaddr_in 结构
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8888);

    // 绑定
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("绑定失败. 错误码: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    printf("绑定成功.\n");

    // 监听
    listen(server_socket, 3);
    printf("等待连接...\n");

    // 接受连接
    client_addr_size = sizeof(struct sockaddr_in);
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
    if (client_socket == INVALID_SOCKET) {
        printf("接受连接失败. 错误码: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    printf("连接接受.\n");

    // 接收数据
    int recv_size;
    if ((recv_size = recv(client_socket, buffer, sizeof(buffer), 0)) == SOCKET_ERROR) {
        printf("接收失败. 错误码: %d\n", WSAGetLastError());
    }
    else {
        buffer[recv_size] = '\0';
        printf("收到消息: %s\n", buffer);
    }

    // 关闭套接字
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
    return 0;
}

#endif

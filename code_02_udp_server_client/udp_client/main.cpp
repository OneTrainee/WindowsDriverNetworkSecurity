#if 1
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")  // Winsock 库

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    const char* message;
    char buffer[1024];
    int server_addr_size;

    // 初始化 Winsock
    printf("初始化 Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("初始化失败. 错误码: %d\n", WSAGetLastError());
        return 1;
    }
    printf("初始化成功.\n");

    // 创建客户端套接字
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
        printf("创建套接字失败. 错误码: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    printf("套接字创建成功.\n");

    // 准备 sockaddr_in 结构
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 服务器 IP 地址
    server_addr.sin_port = htons(8888);

    // 发送数据
    message = "Hello, Server!";
    if (sendto(client_socket, message, strlen(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("发送失败. 错误码: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }
    printf("消息发送: %s\n", message);

    // 接收数据
    server_addr_size = sizeof(server_addr);
    int recv_len;
    if ((recv_len = recvfrom(client_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_addr, &server_addr_size)) == SOCKET_ERROR) {
        printf("接收失败. 错误码: %d\n", WSAGetLastError());
    }
    else {
        buffer[recv_len] = '\0';
        printf("收到消息: %s\n", buffer);
    }

    // 关闭套接字
    closesocket(client_socket);
    WSACleanup();
    return 0;
}

#else
#include <WinSock2.h>
#include <stdio.h>
#pragma comment(lib, "WS2_32")	// 链接到WS2_32.lib
#define PORT 8888
int main()
{
    WSADATA wsaData;
    int Ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
    printf("Client..!\n");
    if (Ret != 0)
    {
        printf("无法初始化winsock.\n");
        WSACleanup();
    }
    else {

        //    printf("初始化winsock成功\n");
    }

    SOCKET socketc = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//IPPROTO_UDP
    sockaddr_in addr;
    addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(PORT);

    if (socketc == INVALID_SOCKET)
    {
        printf("socket Error!");
    }

    int len = sizeof(sockaddr);


    while (1)//client
    {
        char buf[200] = "\0";
        char rbuf[300] = "\0";
        printf("\n:");
        scanf_s("%s", &buf);
        sendto(socketc, buf, strlen(buf) + 1, 0, (SOCKADDR*)&addr, len);//用回原来的地址结构

        recvfrom(socketc, rbuf, sizeof(rbuf), 0, (SOCKADDR*)&addr, &len);
        if (strcmp(rbuf, "exit") == 0)
        {
            break;
        }
        printf("%s", rbuf);

    }

    closesocket(socketc);
    WSACleanup();
    printf("end\n");
    return 0;
}
#endif
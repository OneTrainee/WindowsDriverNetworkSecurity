#if 1
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")  // Winsock ��

int main() {
    WSADATA wsa;
    SOCKET server_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    int client_addr_size, recv_len;

    // ��ʼ�� Winsock
    printf("��ʼ�� Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("��ʼ��ʧ��. ������: %d\n", WSAGetLastError());
        return 1;
    }
    printf("��ʼ���ɹ�.\n");

    // �����������׽���
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
        printf("�����׽���ʧ��. ������: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    printf("�׽��ִ����ɹ�.\n");

    // ׼�� sockaddr_in �ṹ
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(4567);

    // ��
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("��ʧ��. ������: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    printf("�󶨳ɹ�.\n");

    // �ȴ�����
    printf("�ȴ�����...\n");
    client_addr_size = sizeof(struct sockaddr_in);
    while ((recv_len = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_addr_size)) != SOCKET_ERROR) {
        buffer[recv_len] = '\0';
        printf("�յ���Ϣ: %s\n", buffer);

        // ������Ӧ
        const char* response = "Hello, Client!";
        sendto(server_socket, response, strlen(response), 0, (struct sockaddr*)&client_addr, client_addr_size);
    }

    if (recv_len == SOCKET_ERROR) {
        printf("����ʧ��. ������: %d\n", WSAGetLastError());
    }

    // �ر��׽���
    closesocket(server_socket);
    WSACleanup();
    return 0;
}

#else 
#include <WinSock2.h> 
#include <stdio.h>
#pragma comment(lib, "WS2_32")	// ���ӵ�WS2_32.lib
#define PORT 8888
int main() //server
{
    WSADATA wsaData;
    int Ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
    printf("Server!\n");
    if (Ret != 0)
    {
        printf("�޷���ʼ��winsock.\n");
        WSACleanup();
    }
    else {

        //    printf("��ʼ��winsock�ɹ�\n");
    }

    if (LOBYTE(wsaData.wVersion) != 2 ||
        HIBYTE(wsaData.wVersion) != 2)
    {
        WSACleanup();
        return 1;
    }

    SOCKET socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//IPPROTO_UDP
    sockaddr_in addr;
    addr.sin_addr.S_un.S_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(PORT);



    if (socket == INVALID_SOCKET)
    {
        printf("socket Error!");
    }
    int rs = bind(socket, (sockaddr*)&addr, sizeof(addr));

    if (rs != 0)
    {
        printf("bind failed!");
        return 1;
    }
    char buf[200] = "\0";
    char temp[200] = "\0";
    sockaddr_in addrcl;
    int len = sizeof(sockaddr);

    while (1) {

        recvfrom(socket, buf, sizeof(buf), 0, (sockaddr*)&addrcl, &len);//���½��ĵ�ַ�ṹ����ͻ��˵���Ϣ 

        sprintf_s(temp, "%s:%s", inet_ntoa(addrcl.sin_addr), buf);
        sendto(socket, temp, strlen(temp) + 1, 0, (sockaddr*)&addrcl, sizeof(addr));
        if (strcmp(buf, "srvexit") == 0)
        {
            break;
        }
        printf("%s", buf);
    }
    closesocket(socket);

    WSACleanup();
    printf("exit\n");
    return 0;
}
#endif


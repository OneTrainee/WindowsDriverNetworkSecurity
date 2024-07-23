
# SELECT网络模型

### SELECT网络模型解决的问题

Select网络模型解决的是“傻等阻塞”，比如accept,recv函数，其一直再堵塞，没有意义；而不能解决“执行堵塞”，比如send和recv，其执行时必须产生堵塞，这个是select解决不了的。

### SELCET编程细节

将ReadSet和WriteSet都置为0，准备往里面添加Socket。
```c++
FD_ZERO(&ReadSet);
FD_ZERO(&WriteSet);
```

将ListenSocket添加进去，如果该socket被置位，则表示需要accept接收新的数据。

```c++
FD_SET(ListenSocket, &ReadSet);
```

下面是一个echo模型，如果存在输入数据，则将其放入WriteSet，表示将要进行输出；反之同理。
```c++
for (i = 0; i < TotalSockets; i++)
    if (SocketArray[i]->BytesRECV > SocketArray[i]->BytesSEND)    
        FD_SET(SocketArray[i]->Socket, &WriteSet);
    else
        FD_SET(SocketArray[i]->Socket, &ReadSet);
```

接下来调用select，该函数返回recv和write的数量。

```c++
Total = select(0, &ReadSet, &WriteSet, NULL, NULL)
```

先判断listenSocket有没有被置位，如果被置位，则表示有新的连接过来，需要调用accpet接收该参数。
```c++
if (FD_ISSET(ListenSocket, &ReadSet))
{
    Total--;
    if ((AcceptSocket = accept(ListenSocket, NULL, NULL)) != INVALID_SOCKET)
    {

        // Set the accepted socket to non-blocking mode so the server will
        // not get caught in a blocked condition on WSASends

        NonBlock = 1;
        if (ioctlsocket(AcceptSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
        {
            printf("ioctlsocket() failed with error %d\n", WSAGetLastError());
            return;
        }

        if (CreateSocketInformation(AcceptSocket) == FALSE)
            return;

    }
    else
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            printf("accept() failed with error %d\n", WSAGetLastError());
            return;
        }
    }
}
```
之后开始遍历每个socket，我们写的是一个echo服务器，收发逻辑要清楚，逻辑很简单的。
```c++
for (i = 0; Total > 0 && i < TotalSockets; i++)
{
    LPSOCKET_INFORMATION SocketInfo = SocketArray[i];
    if (FD_ISSET(SocketInfo->Socket, &ReadSet))
    {
        Total--;

        SocketInfo->DataBuf.buf = SocketInfo->Buffer;
        SocketInfo->DataBuf.len = DATA_BUFSIZE;

        Flags = 0;
        if (WSARecv(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &RecvBytes,
            &Flags, NULL, NULL) == SOCKET_ERROR)
        {
            //....

            continue;
        }
        else
        {
            SocketInfo->BytesRECV = RecvBytes;
            if (RecvBytes == 0)
            {
                FreeSocketInformation(i);
                continue;
            }
        }
    }

    if (FD_ISSET(SocketInfo->Socket, &WriteSet))
    {
        Total--;

        SocketInfo->DataBuf.buf = SocketInfo->Buffer + SocketInfo->BytesSEND;
        SocketInfo->DataBuf.len = SocketInfo->BytesRECV - SocketInfo->BytesSEND;

        if (WSASend(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &SendBytes, 0,
            NULL, NULL) == SOCKET_ERROR)
        {
            // .... 
            continue;
        }
        else
        {
            SocketInfo->BytesSEND += SendBytes;

            if (SocketInfo->BytesSEND == SocketInfo->BytesRECV)
            {
                SocketInfo->BytesSEND = 0;
                SocketInfo->BytesRECV = 0;
            }
        }
    }
}
```



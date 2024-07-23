# OverlapIO网络模型

### OverlapIO网络模型解决的问题

OverlapIO网络模型是真正的异步网络模型，其真正实现了异步的操作。 与前面的SELECT模型不同，其直接进行异步IO的操作，微软相当于重写了一系列函数来支持Overlap的网络模型。

### OverlapIO网络模型的核心代码

其代码如下，其本质要创建一个Event，然后调用时就要等待该Event事件完成，多余的就不多说了，很简单。

```cpp
   // 配置重叠结构
    overlapped.hEvent = WSACreateEvent();

    // 设置缓冲区
    buffer.buf = recvBuf;
    buffer.len = sizeof(recvBuf);

    // 启动异步读取操作
    if (WSARecv(sock, &buffer, 1, &bytesReceived, &flags, &overlapped, CompletionRoutine) == SOCKET_ERROR) {
        if (WSAGetLastError() == WSA_IO_PENDING) {
            printf("I/O operation is pending\n");
        } else {
            printf("WSARecv failed with error %d\n", WSAGetLastError());
            return 1;
        }
    }

    // 等待操作完成
    WSAWaitForMultipleEvents(1, &overlapped.hEvent, TRUE, INFINITE, TRUE);
```
#  CompleteIo网络模型

### CompleteIo网络模型解决的问题

CompleteIo要解决的是高并发的问题，IOCP利用一个线程池来处理所有的I/O请求，并根据系统负载动态调整线程池的大小。这样可以避免为每个连接创建一个线程，减少线程开销。

### CompleteIo代码核心介绍

核心显示创建完成IO端口，代码如下，很简单。将其句柄传入IO完成端口。

```c++
if (CreateIoCompletionPort((HANDLE)Accept, CompletionPort, (DWORD)PerHandleData,
    0) == NULL)
{
    printf("CreateIoCompletionPort failed with error %d\n", GetLastError());
    return;
}
```

传入IO完成端口之后，其有任何IO完成操作之后，GetQueuedCompletionStatus函数会返回其相关的调用值，通知其IO操作完成。

```c++
if (GetQueuedCompletionStatus(CompletionPort, &BytesTransferred,
    (PULONG_PTR)&PerHandleData, (LPOVERLAPPED*)&PerIoData, INFINITE) == 0)
{
    printf("GetQueuedCompletionStatus failed with error %d\n", GetLastError());
    return 0;
}
```

因此要创建多个线程，来多线程处理这类IO完成通知，这个很简单。

```c++
for (i = 0; i < SystemInfo.dwNumberOfProcessors * 2; i++)
{
    HANDLE ThreadHandle;

    // Create a server worker thread and pass the completion port to the thread.

    if ((ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, CompletionPort,
        0, &ThreadID)) == NULL)
    {
        printf("CreateThread() failed with error %d\n", GetLastError());
        return;
    }

    // Close the thread handle
    CloseHandle(ThreadHandle);
}
```
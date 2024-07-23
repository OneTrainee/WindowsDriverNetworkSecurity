# EventSelect网络模型

### EventSelect网络模型解决的问题

前面讲过的AsyncSelect异步模型，可以很好的解决消息堵塞的问题。但是，其有一个明显的缺点，其利用Windows的消息机制，其只能在Windows平台上使用，而不能跨平台使用，现在来很好的解决这个问题。

### EventSelect编程细节

其核心函数是这个 WSAEventSelect，以及 WSAWaitForMultipleEvents。
```c++
int WSAAPI WSAEventSelect(
  [in] SOCKET   s,
  [in] WSAEVENT hEventObject,
  [in] long     lNetworkEvents
);

DWORD WSAAPI WSAWaitForMultipleEvents(
  [in] DWORD          cEvents,
  [in] const WSAEVENT *lphEvents,
  [in] BOOL           fWaitAll,
  [in] DWORD          dwTimeout,
  [in] BOOL           fAlertable
);
```

其核心调用的代码如下，很好理解，本质是等待Event对象。
```c++
if (WSAEventSelect(Accept, EventArray[EventTotal - 1], FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
{
    printf("WSAEventSelect() failed with error %d\n", WSAGetLastError());
    return;
}

if ((Event = WSAWaitForMultipleEvents(EventTotal, EventArray, FALSE,
    WSA_INFINITE, FALSE)) == WSA_WAIT_FAILED)
{
    printf("WSAWaitForMultipleEvents failed with error %d\n", WSAGetLastError());
    return;
}


if (NetworkEvents.lNetworkEvents & FD_ACCEPT){
    ...
}
```
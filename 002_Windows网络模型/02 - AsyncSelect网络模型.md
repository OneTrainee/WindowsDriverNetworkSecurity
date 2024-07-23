# AsyncSelect网络模型

### AsyncSelect网络模型解决的问题

WSAAsyncSelect 函数将网络事件与窗口消息关联起来，当指定的网络事件发生时，系统会向指定的窗口发送一条消息。这使得应用程序能够在事件驱动的框架中处理网络事件，而不是在阻塞的套接字操作上浪费时间。

之前提到过select网络模型，其必须进行一系列堵塞操作，比如 recv 和 send，这是不可避免的。而异步Select网络模型直接与窗口结合。 其窗口消息中有一种 `WM_SOCKET`网络消息，表示是网络消息。

### AsyncSelect网络模型编程细节

其核心函数是，把一个窗口句柄和对应的套接字绑定起来。
```c++
int WSAAsyncSelect(
  SOCKET s,
  HWND hWnd,
  unsigned int wMsg,
  long lEvent
);
```
lEvent 参数是对应的socket所产生的事件，当对应事件发生时，会调用窗口回调函数进行处理。
```c++
FD_READ：套接字可读。
FD_WRITE：套接字可写。
FD_OOB：带外数据可用。
FD_ACCEPT：对套接字的连接请求。
FD_CONNECT：连接完成。
FD_CLOSE：连接被关闭。
```

其初始化时，只接收ACCEPT事件与CLOSE事件，因为listenSocket是不管读写的，这个编写逻辑要清楚。
```c++
 WSAAsyncSelect(Listen, Window, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
```

其核心操作与Select无异，举个简单例子，一旦有ACCEPT事件，其创建一个对应的socket，然后再此放入进去

```c++
switch (WSAGETSELECTEVENT(lParam))
{
case FD_ACCEPT:

    if ((Accept = accept(wParam, NULL, NULL)) == INVALID_SOCKET)
    {
        printf("accept() failed with error %d\n", WSAGetLastError());
        break;
    }

    // Create a socket information structure to associate with the
    // socket for processing I/O.

    CreateSocketInformation(Accept);

    printf("Socket number %d connected\n", Accept);

    WSAAsyncSelect(Accept, hwnd, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE);

    break;

    ...

}
```
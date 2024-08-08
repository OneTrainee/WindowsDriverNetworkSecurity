# WSK网络模型

### 什么是 WSK网络模型

WSK网络模型是运行在内核层的网络模型，其全称是 windows socket kernel。

我们之前学习的socket网络模型，都是在三环，而wsk在零环帮你实现相关操作。

### WSK TCP Echo Server 内核源码解读

WSK TCP Echo Server 是微软的项目，代码在`code_10_ssk_echosrv`中。

其核心函数如下，通过回调函数来实现Accept，然后发送。

```c++
WskProviderNpi->Dispatch->WskSocket(
            WskProviderNpi->Client,
            AF_INET6,
            SOCK_STREAM,
            IPPROTO_TCP,
            WSK_FLAG_LISTEN_SOCKET,
            socketContext,
            &WskSampleClientListenDispatch,
            NULL, // Process
            NULL, // Thread
            NULL, // SecurityDescriptor
            irp);
```

其 WskSampleClientListenDispatch 是一个回调函数目录，比如当有新的请求accept时，其会调用里面的回调函数。

```c++
const WSK_CLIENT_LISTEN_DISPATCH WskSampleClientListenDispatch = {
    WskSampleAcceptEvent,
    NULL, // WskInspectEvent is required only if conditional-accept is used.
    NULL  // WskAbortEvent is required only if conditional-accept is used.
};
```
# Windows-Web-Server
Windows TCP web server implementation


### Example usage 
Please see WinServer.cpp for full example usage <br />
```cpp
#include "WindowsServer.h"

const char* callBack() {
	const char* msg =
	"HTTP/1.1 200 OK\n"
	"Server: JDG Server\n"
	"Content-Type: text/html\n"
	"Content-Length: 3000\n"
	"Accept-Ranges: bytes\n"
	"Connection: keep-alive\n\n"
	"<h1>Hello World!</h1>\n"
	"<img src=\"https://ih1.redbubble.net/image.917633532.5454/flat,750x1000,075,f.jpg\">\n";
	return msg;
}

int main(int argc, char* argv[]) {
	WindowsServer winServer;
	if (!winServer.init(argc, argv)) return 1;
	winServer.addCallBack("/test", callBack);	
	winServer.runServer();
	return 0;
}
```


### Demo code
Compiler: MSVC 19.34.31933 <br />
Compile using: cl WinServer.cpp WindowsServer.cpp /EHsc <br />
Run using: WinServer <br />

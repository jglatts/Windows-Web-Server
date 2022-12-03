/**
 * @file WinServer.cpp
 * 
 * @author John Glatts
 * @brief  Driver source file for the Windows web server.
 * 		   Compile using: cl WinServer.cpp WindowsServer.cpp /EHsc
 * @version 0.1
 * @date 2022-12-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "WindowsServer.h"

const char* callBack() {
	const char* msg =
	"HTTP/1.1 200 OK\n"
	"Server: JDG Server\n"
	"Content-Type: text/html\n"
	"Content-Length: 3000\n"
	"Accept-Ranges: bytes\n"
	"Connection: keep-alive\n\n"
	"<h1>Hello World!</h1>\n";
	return msg;
}

int main(int argc, char* argv[]) {
	WindowsServer winServer;

	if (winServer.init(argc, argv)) { 
		winServer.addCallBack("/", callBack);	
		winServer.runServer();
	}

	return true;
}
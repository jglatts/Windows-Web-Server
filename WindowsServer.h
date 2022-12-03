/**
 * @file WindowsServer.h
 * 
 * @author John Glatts
 * @brief  Class definiton for the WindowsServer
 * @version 0.1
 * @date 2022-12-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __WINDOWS_SERVER__
#define __WINDOWS_SERVER__

#include <winsock2.h>
#include <ws2ipdef.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#include <vector>

#define DEFAULT_PORT 8888
#define MAX_BUF_LEN 1024

using namespace std;

class WindowsServer {
public:
	WindowsServer();
	WindowsServer(int);
	bool init(int, char**);
	bool routeURL(char*);
	bool runServer();
	void addURL(const char*);
	void addCallBack(const char*, const char* (*)(void));
	void testCallBacks(const char*);
private:
	typedef struct URL {
		const char* url;
		const char* (*url_callback)(void);
	} URL;
	bool initServer();
	int checkURL(char*);
	void parseURL(SOCKET, char*, char*);
	bool getPacket(SOCKET, char*);
	void sendResponse(SOCKET, char*);
	int port;
	SOCKET server_socket;
	vector<const char*> valid_urls;
	vector<URL> response_callbacks;
};


#endif // !__WINDOWS_SERVER__

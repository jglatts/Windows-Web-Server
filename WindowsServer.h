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
	bool init();
	bool routeURL(char*);
	bool runServer();
	void addURL(const char*);
private:
	bool initServer();
	bool checkURL(char*);
	void parseURL(SOCKET, char*, char*);
	bool getPacket(SOCKET, char*);
	void sendResponse(SOCKET, char*);
	int port;
	SOCKET server_socket;
	vector<const char*> valid_urls;
};


#endif // !__WINDOWS_SERVER__

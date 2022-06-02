#include "WindowsServer.h"

int main(int argc, char* argv[]) {
	WindowsServer winServer;

	winServer.addURL("/");
	winServer.addURL("/hello/");

	if (winServer.init()) 
		winServer.runServer();

	return true;
}
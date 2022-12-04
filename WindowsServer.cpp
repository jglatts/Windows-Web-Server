/**
 * @file WindowsServer.cpp
 * 
 * @author John Glatts
 * @brief  Source file for the WindowsServer class
 * @version 0.1
 * @date 2022-12-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "WindowsServer.h"
#include <stdio.h>
#include <string.h>
#include <vector>

using namespace std;

// Winsock Library
#pragma comment(lib, "ws2_32.lib") 

WindowsServer::WindowsServer(){
	port = DEFAULT_PORT;
}

WindowsServer::WindowsServer(int port_num) {
	port = port_num;
}

bool WindowsServer::init(int argc, char** argv) {
	WSADATA wsa;

	if (argc > 1) {
		if (isdigit(argv[1][0])) {
			port = atoi(argv[1]);
		}
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("Failed. Error Code : %d\n", WSAGetLastError());
		return false;
	}

	if (!initServer()) {
		return false;
	}

	return true;
}

bool WindowsServer::initServer() {
	struct sockaddr_in server;
	char ip[500];

	// TCP socket
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d\n", WSAGetLastError());
		return false;
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;	// default source address
	server.sin_port = htons(port);			// port supplied by user

	if (inet_ntop(AF_INET, (PVOID) & ((PSOCKADDR_IN)&server)->sin_addr, ip, 500)) {
		printf("Server Socket Created\nServer IP: %s (localhost)\nPort: %d\n", ip, port);
	}

	if (bind(server_socket, (struct sockaddr*) & server, sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed with error code : %d\n", WSAGetLastError());
		return false;
	}

	listen(server_socket, 3);
	return true;
}

void WindowsServer::testCallBacks(const char* s) {
	// todo
}

void WindowsServer::addURL(const char* url) {
	if (*url && *url != ' ')
		valid_urls.push_back(url);
}

bool WindowsServer::runServer() {
	char ip[500];
	char recv_buf[MAX_BUF_LEN];
	int c = sizeof(struct sockaddr_in);

	puts("\nWaiting for incoming connections...");
	while (1) {
		SOCKET client_socket;
		struct sockaddr_in client;
		client_socket = accept(server_socket, (struct sockaddr*) & client, &c);
		if (client_socket == INVALID_SOCKET) {
			printf("accept() failed with error : %d", WSAGetLastError());
			return false;
		}

		puts("Connection accepted");
		if (inet_ntop(AF_INET, (PVOID) & ((PSOCKADDR_IN)&client)->sin_addr, ip, 100)) {
			printf("Client IP: %s\n", ip);
		}

		if (getPacket(client_socket, recv_buf)) {
			parseURL(client_socket, ip, recv_buf);
			closesocket(client_socket);
		}
	}
	closesocket(server_socket);
	return true;
}

void WindowsServer::parseURL(SOCKET client_socket, char* ip, char* recv_buf) {
	int i;
	const char* msg;
	printf("%s\n", recv_buf);

	if ((i = checkURL(recv_buf)) != -1) {
		// get the HTTP response message provided
		// by the user specified callback 
		msg = response_callbacks.at(i).url_callback();
	}
	else {
		msg =
		"HTTP/1.1 200 OK\n"
		"Server: JDG Server\n"
		"Content-Type: text/html\n"
		"Content-Length: 3000\n"
		"Accept-Ranges: bytes\n"
		"Connection: keep-alive\n\n"
		"<h1>Sorry this URL is not valid!</h1>\n"
		"<img src=\"https://www.iconpacks.net/icons/2/free-sad-face-icon-2691-thumb.png\">\n";
	}
	send(client_socket, msg, strlen(msg), 0);
}

void WindowsServer::addCallBack(const char* url_str, const char* (fp)(void)) {
	URL url = { url_str, fp};
	response_callbacks.push_back(url);
}

int WindowsServer::checkURL(char* buff) {
	char* token;
	char* next;
	char  url[1024];
	int i;
	int ret = -1;

	// extract the GET request from client 
	token = strtok_s(buff, "\n", &next);
	if (token == NULL) return false;
	if (strstr(token, "GET") == NULL) {
		while (token != NULL) {
			if (strstr(token, "GET") != NULL) {
				printf("%s\n\n", token);
				break;
			}
			token = strtok_s(NULL, "\n", &next);
		}
	}

	strtok_s(token, " ", &next);	
	token = strtok_s(NULL, " ", &next);
	for (i = 0; token[i] && token[i] != ' ' && i < 1023; ++i) {
		url[i] = token[i];
	}

	url[i] = '\0';
	printf("URL from client: %s\n", url);
	for (int i = 0; i < response_callbacks.size(); ++i) {
		if (strcmp(response_callbacks.at(i).url, url) == 0) {
			ret = i;
			break;
		}
	}

	return ret;
}

bool WindowsServer::getPacket(SOCKET client_sock, char* recv_buff) {
	int bytes_read = recv(client_sock, recv_buff, MAX_BUF_LEN, 0);
	if (bytes_read > MAX_BUF_LEN) {
		puts("Error - Buffer to small for HTTP packet sent from client, please increase size");
		return false;
	}
	if (bytes_read < MAX_BUF_LEN)
		recv_buff[bytes_read] = '\0';
	else
		return false;
	return true;
}

void WindowsServer::sendResponse(SOCKET client_sock, char* client_ip) {
	char buf[3000];
	// HTTP header to send to clientsponse(SOCKET client_sock, char* client_ip) {
	SYSTEMTIME t;
	GetLocalTime(&t);
	char reply[1024] =
		"HTTP/1.1 200 OK\n"
		"Server: JDG Server\n"
		"Content-Type: text/html\n"
		"Content-Length: 3000\n"
		"Accept-Ranges: bytes\n"
		"Connection: keep-alive\n"
		"\n";

	// HTTP data to send to client
	snprintf(buf, 100, "<title>JDG HTTP Server</title>\n");
	strcat_s(reply, buf);
	snprintf(buf, 150, "<img src=\"https://ih1.redbubble.net/image.917633532.5454/flat,750x1000,075,f.jpg\">\n");
	strcat_s(reply, buf);
	snprintf(buf, 100, "<h2 style=\"color:red;\">Your IP Address: %s</h2>\n", client_ip);
	strcat_s(reply, buf);
	snprintf(buf, 300, "<h2 style=\"color:red\">It Is Now: %d-%d-%d %d:%d:%d</h2>\n",
		t.wMonth, t.wDay, t.wYear, t.wHour, t.wMinute, t.wSecond);
	strcat_s(reply, buf);
	send(client_sock, reply, strlen(reply), 0);
	// wait until client disconnects
	//while (recv(client_sock, buf, 300, 0) > 0) {}

}

bool WindowsServer::routeURL(char* url) {
	return true;
}


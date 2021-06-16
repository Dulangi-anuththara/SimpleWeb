#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include <string.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library


char html[2000], jpg[5000], txt[2000];
void filePrep(){
	int index;
	FILE *F1, *F2, *F3;
	F1 = fopen("index.txt", "r");
	F2 = fopen("index.html", "r");
	F3 = fopen("abc.jpg", "rb");
	
	if(F1 != NULL){

		char line[200];
		while(fgets(line,sizeof(line),F1) != NULL){
			strcat(txt,line);
		}
		
		puts(txt);
	}
	
	if(F2 != NULL){
		char line[200];
		while(fgets(line,sizeof(line),F2) != NULL){
			strcat(html,line);
		}
		
		puts(html);
	}
	
		if(F3 != NULL){
		char line[200];
		while(fgets(line,sizeof(line),F3) != NULL){
			strcat(jpg,line);
		}
		
		puts(html);
	}
	

}

int main(int argc , char *argv[])
{
	WSADATA wsa;
	SOCKET s , new_socket;
	struct sockaddr_in server , client;
	
	int c, recv_size;
	
	char *message, *msg, server_reply[2000], data[2000], *ptr = NULL;
	char *request, *method, *target;
	
	// Read various files
	filePrep();
	
//	pdf = fopen("index.txt", "r");
	
//	if(pdf == NULL){
//		printf("Cannot open the pdf file\n");
//	}else{
//		if(fgets(data,2000,pdf) != NULL){
//			puts(data);
//		}else{
//			printf("Cannot read the file\n");
//		}
//	}
//	
//	
	msg = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 88\n\n<html><body><p>Fork me? Fork you, @octocat!</p><p>Sean made a change</p></body></html>";


	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		return 1;
	}
	
	printf("Initialised.\n");
	
	//Create a socket
	if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d" , WSAGetLastError());
	}

	printf("Socket created.\n");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8888 );
	
	//Bind
	if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d" , WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	
	puts("Bind done");

	//Listen to incoming connections
	listen(s , 3);
	
	//Accept an incoming connection
	puts("Waiting for incoming connections...");
	
	c = sizeof(struct sockaddr_in);

	while( (new_socket = accept(s , (struct sockaddr *)&client, &c)) != INVALID_SOCKET )
	{
		puts("Connection accepted");
				
		//Receive message from client
			
		while(!((recv_size = recv(new_socket,server_reply, 2000, 0)) == SOCKET_ERROR)){
			
			
			puts("\nRequest received");
//			message = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 10\n\nBad world!";  
//			int val =send(new_socket , message , strlen(message) , 0);		
				
			//Add a NULL terminating character to make it a proper string before printing
			server_reply[recv_size] = '\0';
			request = strtok(server_reply, "\n");
			method = strtok(request, " ");
			target = strtok(NULL, " ");
						
			//Seperate the request
			puts(target);
			if(strcmp(target, "/index.html") == 0){
			
			send(new_socket , msg , strlen(msg) , 0);
			}
			else if(strcmp(target, "/index.txt") == 0){
				
				printf("Text");
			}
			else if(strcmp(target, "/index.jpeg") == 0){
				printf("Image");
			}
			else if(strcmp(target, "/") == 0){
				printf("plaint-text");
			}
			else{
				printf("Bad Request");
			}
			
		}

	}
	
	if (new_socket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d" , WSAGetLastError());
		return 1;
	}
	

	


	closesocket(s);
	WSACleanup();
	
	return 0;
}

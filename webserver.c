#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include <string.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library


char *jpg, html[2000], txt[2000];
void filePrep(){
	
	int num_read = 0;
	unsigned long fileLen;
	char s;
	FILE *F1, *F2, *F3;
	F1 = fopen("index.txt", "r");
	F2 = fopen("index.html", "r");
	F3 = fopen("abc.jpg", "rb");
	
	//Read .txt file
	if(F1 != NULL){

		char line[200];
		while(fgets(line,sizeof(line),F1) != NULL){
			strcat(txt,line);
		}			
	}
	
	//Read .html file
	if(F2 != NULL){
		char line[200];
		while(fgets(line,sizeof(line),F2) != NULL){
			strcat(html,line);
		}
		
	}

	//Read .jpg file
	fseek(F3, 0, SEEK_END);
	fileLen=ftell(F3);
	rewind(F3);
	jpg=malloc((fileLen)*sizeof(char));
	if (jpg == NULL){
	    printf("Memory error"); exit (2);
	}
	while ((num_read = fread(&s, 1, 1, F3))) {
	    strncat(jpg,&s,1);
	}
	

	fclose(F3);
	fclose(F2);
	fclose(F1);

}

int main(int argc , char *argv[])
{
	WSADATA wsa;
	SOCKET s , new_socket;
	struct sockaddr_in server , client;
	
	int c, recv_size;
	
	char *message, server_reply[2000], data[2000], *ptr = NULL;
	char *request, *method, *target;
	
	// Read various files
	filePrep();
	
	//msg = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 88\n\n<html><body><p>Fork me? Fork you, @octocat!</p><p>Sean made a change</p></body></html>";


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
			
			recv_size = recv(new_socket,server_reply, 2000, 0);			
			
			puts("\nRequest received");
		
				
			//Add a NULL terminating character to make it a proper string before printing
			server_reply[recv_size] = '\0';
			
			//Get the request
			request = strtok(server_reply, "\n");
			method = strtok(request, " ");
			target = strtok(NULL, " ");
						
			//Serve the request
			
			puts(target);

			if(strcmp(target, "/index.html") == 0){
				
				char message[] = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 66\n\n";
				strcat(message,html);
				send(new_socket , message , strlen(message) , 0);
		
			}
			else if(strcmp(target, "/index.txt") == 0){
				
				char message[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 136\n\n";  
				strcat(message,txt);
				puts(txt);
				send(new_socket , message , strlen(message) , 0);

			}
			else if(strcmp(target, "/index.jpeg") == 0){
				printf(jpg);
			}
			else if(strcmp(target, "/") == 0){
				
				char message[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello World!";  
				send(new_socket , message , strlen(message) , 0);	
			}
			else{
				printf("Bad Request\n");
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

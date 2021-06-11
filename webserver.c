#include<stdlib.h> 
#include <stdio.h>
#include <winsock2.h> // header file for socket in windows

#pragma comment(lib,"ws2_32.lib") // library file to be linked to be able to use winsock functions

int main(int argc, char *argv[]){
	
	WSADATA wsa;
	
	printf("\nInitialising Winsock....");
	
	if(WSAStartup(MAKEWORD(2,2),&wsa) != 0){
//		printf("Failed. Error Code: %d", WSAGetLastError());
//		return 1;
	}
//	
//	printf("Initialised");
	
	return 0;

}

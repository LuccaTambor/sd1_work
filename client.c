#include <stdio.h>	
#include <string.h>	
#include <sys/socket.h>	
#include <arpa/inet.h>	
#include <unistd.h>
#define PORT 8882

int main(int argc , char *argv[]) {
	int sock;
	struct sockaddr_in server;
	char message[1000] , server_reply[2000];
	int value_buffer = 0;
	
	//Creating socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1) {
		printf("Could not create socket");
	}
	puts("Socket created");
	
	//Prepare the sockaddr_in structure
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
		perror("connect failed. Error");
		return 1;
	}
	
	puts("Connected\n");

	//Receive a reply from the server
	if( recv(sock , server_reply , 2000 , 0) < 0) {
		puts("recv failed");
	}

	puts("Server reply :");
	puts(server_reply);
	server_reply[0] = '\0';

	//waiting all clients connect to start Monte Carlo
	while(1) {
		if( recv(sock , server_reply , 2000 , 0) < 0){
			puts("recv failed");
		}

		puts("Server reply :");
		puts(server_reply);
		server_reply[0] = '\0';


		if( recv(sock , &value_buffer , sizeof(value_buffer) , 0) < 0){
			puts("recv failed");
		}
		printf("\nValue from server: %d", value_buffer);
	}

	
	
	//keep communicating with server
	/*while(1)
	{
		//printf("Enter message : ");
		//scanf("%s" , message);
		
		//Send some data
		/*if( send(sock , message , strlen(message) , 0) < 0)
		{
			puts("Send failed");
			return 1;
		}
		
		
		
		
		puts("Server reply :");
		puts(server_reply);
	}
	*/
	
	
	close(sock);
	return 0;
}




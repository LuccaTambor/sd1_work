#include <stdio.h>	
#include <string.h>	
#include <time.h>
#include <stdlib.h>
#include <sys/socket.h>	
#include <arpa/inet.h>	
#include <unistd.h>
#define PORT 8881

int main(int argc , char *argv[]) {
	int sock, check = 0, i, points_in = 0;
	struct sockaddr_in server;
	char message[1000] , server_reply[2000], server_reply2[2000];
	double value_buffer = 0, x, y;
	srand(time(NULL));
	
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
	
	//waiting all clients connect to start Monte Carlo
	do {
		if( recv(sock , server_reply2 , 2000 , 0) < 0){
			puts("recv failed");
		}

		puts("Server reply :");
		puts(server_reply2);
		check = 1;
	}while (check == 0);

	if( recv(sock , &value_buffer , sizeof(value_buffer) , 0) < 0){
			puts("recv failed");
	}
	printf("\nValue from server: %f \n", value_buffer);

	for( i = 0; i < value_buffer; i++) {//Loop where the points will be put on the quadrant
		x = (double)rand() * ( 1.0 - 0.0 ) / (double)RAND_MAX + 0.0;//Coord of point x
		y = (double)rand() * ( 1.0 - 0.0 ) / (double)RAND_MAX + 0.0;//Coord of point y
		
		if(x * x + y * y < 1) {//The point is in the circle
			points_in++;
		}
    }

	printf("\nPoints in: %d \n", points_in);

	double pi = 4.0 * points_in/value_buffer;//Pi
	printf("Value of pi: %f \n", pi);
	
	send(sock, &pi, sizeof(pi),0);
	
	printf("flag\n");
	close(sock);
	return 0;
}




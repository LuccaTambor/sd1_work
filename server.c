#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>	
#include<math.h>
#include<sys/socket.h>
#include<arpa/inet.h>	
#include<unistd.h>	
#include<pthread.h> 
#define PORT 8887

int startMonte = 0;//Global Var for starting the Monte Carlo
double final_pi = 0, points;
int controller = 0;
int number_clients;

//functions header
void *connection_handler(void *);

int main(int argc , char *argv[]) {
	int socket_server , socket_client , c , *new_sock, qtd_clients, check = 0;
	struct sockaddr_in server , client;
	
	number_clients = qtd_clients;
	printf("How many clients you will use:");
	scanf("%d", &qtd_clients);
	srand(time(NULL));
	//double points_seed = (rand() % (10 - 3 + 1)) + 3;//number of points that will be used in monte Carlo	
	double points_seed = 4;
	printf("seed: %f\n", points_seed);
	points = pow((double)10.0,points_seed);
	printf("points: %f\n", points);
	
	//Creating server socket
	socket_server = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_server == -1) {
		printf("Could not create socket");
	}
	//puts("Socket created");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( PORT);
	
	//Binding the server
	if( bind(socket_server,(struct sockaddr *)&server , sizeof(server)) < 0) {
		perror("bind failed. Error");
		return 1;
	}
	//puts("bind done");
	
	//Listening to connections
	listen(socket_server , qtd_clients);
	
	//Accept and incoming connections
	//puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);

	//Loop that will wait all clients connect
	while( (socket_client = accept(socket_server, (struct sockaddr *)&client, (socklen_t*)&c)) && check == 0) {
		//puts("Connection accepted");
		
		
		pthread_t sniffer_thread;
		new_sock = malloc(sizeof(int));
		*new_sock = socket_client;
		qtd_clients--;
		//Verifing if all clients are connect, if yes, Start Monte Carlo
		if(qtd_clients == 0) {
			startMonte = 1;
		}
		
		//creating a thread to handle this client
		if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0) {
			perror("could not create thread");
			return 1;
		}

		
		
		//Now join the thread , so that we dont terminate before the thread
		//pthread_join( sniffer_thread , NULL);
		
		puts("Handler assigned");
		if(controller == number_clients) {
			check = 1;
		}
	}
	
	double final_value_of_pi = final_pi / number_clients;
	printf("Final Value of Pi: %f\n", final_value_of_pi);
	close(socket_server);
	
	return 0;
}

//This will handle connection for each client
void *connection_handler(void *socket_server) {
	//Get the socket descriptor
	int sock = *(int*)socket_server;
	double pi_buffer;
	char message[2000] = "You are connected to the server... Waiting all other clients connect...";
	char message2[2000] = "All clients connected!";
	
	//Sending messages to the client
	send(sock , message , strlen(message), 0);
	int check = 0;

	//Waiting all clients be connected
	do {
		if(startMonte == 1) {
			send(sock , message2 , strlen(message2),0);
			check = 1;
		}
	}while (check == 0);

	send(sock , &points, sizeof(points),0);
	printf("%d\n", sock);
	
	recv(sock, &pi_buffer, sizeof(pi_buffer), 0);
	final_pi = final_pi + pi_buffer;
	printf("pi = %f\n", final_pi);
	controller++;
		
	//Free the socket pointer
	free(socket_server);
	
	return 0;
}
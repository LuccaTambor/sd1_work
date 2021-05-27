#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>	
#include<math.h>
#include<sys/socket.h>
#include<arpa/inet.h>	
#include<unistd.h>	
#include<pthread.h> 
#define PORT 8888

int startMonte = 0;//Global Var for starting the Monte Carlo
double final_pi = 0, points;
int controller = 0;
int number_clients;
int cont = 0;

//functions header
void *client_handler(void *);

int main(int argc , char *argv[]) {
	int socket_server , socket_client , c , *new_sock, qtd_clients=0, check = 0;
	double points_seed;
	struct sockaddr_in server , client;
	pthread_t sniffer_thread;
	srand(time(NULL));
	
	printf("How many clients you will use [2 | 4 | 8 | 16]:");
	scanf("%d", &number_clients);
	printf("How many points will you use : 10^");
	scanf("%lf", &points_seed);
	points = pow((double)10.0,points_seed);

	//Creating server socket
	socket_server = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_server == -1) {
		printf("Could not create socket");
	}
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( PORT);
	
	//Binding the server
	if( bind(socket_server,(struct sockaddr *)&server , sizeof(server)) < 0) {
		perror("bind failed. Error");
		return 1;
	}
	
	//Listening to connections
	listen(socket_server , number_clients);
	
	//Accept and incoming connections
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);

	//Loop that will wait all clients connect
	while(check == 0) {
		//puts("Connection accepted");
		if(qtd_clients < number_clients) {
			socket_client = accept(socket_server, (struct sockaddr *)&client, (socklen_t*)&c);
			
			new_sock = malloc(sizeof(int));
			*new_sock = socket_client;
			qtd_clients++;
			//Verifing if all clients are connect, if yes, Start Monte Carlo
			if(qtd_clients == number_clients) {
				startMonte = 1;
				//começo do clock
			}
		
			//creating a thread to handle this client
			if( pthread_create( &sniffer_thread , NULL ,  client_handler , (void *)new_sock) < 0) {
				perror("could not create thread");
				return 1;
			}
		
			puts("Client assigned to thread");
		}
		
		if(controller == number_clients) {
			check = 1;

		}
	}
	double final_value_of_pi = final_pi / number_clients;
	printf("Final Value of Pi: %f\n", final_value_of_pi);
	//fim do clock
	close(socket_server);
	
	return 0;
}

//This will handle connection for each client
void *client_handler(void *socket_client) {
	//Get the socket descriptor
	int sock = *(int*)socket_client;
	double pi_buffer, points_client;
	cont++;
	points_client = points/number_clients;

	//Waiting all clients be connected
	while(cont != number_clients) { }

	send(sock , &points_client, sizeof(points_client),0);
	
	recv(sock, &pi_buffer, sizeof(pi_buffer), 0);
	
	final_pi = final_pi + pi_buffer;
	printf("pi = %f\n", final_pi);
	controller++;
	
	//Free the socket pointer
	free(socket_client);
	
	return 0;
}
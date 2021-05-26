#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>	
#include<math.h>
#include<sys/socket.h>
#include<arpa/inet.h>	
#include<unistd.h>	
#include<pthread.h> 
#define PORT 8881

int startMonte = 0;//Global Var for starting the Monte Carlo
double final_pi = 0;

//Struct for a list of client sockets
typedef struct clients{
     struct sockaddr_in clientAddr;
	 int sck_client;
     struct clients *next;
 }CLIENT;

//functions header
void *connection_handler(void *);
void InsertEnd(CLIENT **start, CLIENT **end, int cl_sck);

int main(int argc , char *argv[]) {
	int socket_server , socket_client , c , *new_sock, qtd_clients;
	struct sockaddr_in server , client;
	CLIENT *start = NULL, *end = NULL;
	

	printf("How many clients you will use:");
	scanf("%d", &qtd_clients);
	
	//Creating server socket
	socket_server = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_server == -1) {
		printf("Could not create socket");
	}
	puts("Socket created");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( PORT);
	
	//Binding the server
	if( bind(socket_server,(struct sockaddr *)&server , sizeof(server)) < 0) {
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");
	
	//Listening to connections
	listen(socket_server , qtd_clients);
	
	//Accept and incoming connections
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);

	//Loop that will wait all clients connect
	while( (socket_client = accept(socket_server, (struct sockaddr *)&client, (socklen_t*)&c)) ) {
		puts("Connection accepted");
		qtd_clients--;
		InsertEnd(&start, &end, socket_client);
		
		pthread_t sniffer_thread;
		new_sock = malloc(sizeof(int));
		*new_sock = socket_client;

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
	}
	
	if (socket_client < 0) {
		perror("accept failed");
		return 1;
	}
	close(socket_server);
	
	return 0;
}

//Linked List Insertion Function
void InsertEnd(CLIENT **start, CLIENT **end, int cl_sck) {
    CLIENT *new = (CLIENT *)malloc(sizeof(CLIENT));
	new->sck_client = cl_sck;
    if(*start==NULL) { //Empty list 
        *start = new;
        *end = new;
        new->next = NULL;
    }
    else {//List is not empty
        (*end)->next=new;
        *end = new;
        (*end)->next = NULL;
    }
}


//This will handle connection for each client
void *connection_handler(void *socket_server) {
	//Get the socket descriptor
	int sock = *(int*)socket_server;
	int read_size;
	double pi_buffer;
	char *message , *message2, client_message[2000];
	srand(time(NULL));
	double points_seed = (rand() % (10 - 3 + 1)) + 3;//number of points that will be used in monte Carlo	
	printf("seed: %f\n", points_seed);
	double points = pow((double)10.0,points_seed);
	printf("points: %f\n", points);
	//Sending messages to the client
	message = "You are connected to the server... Waiting all other clients connect...";
	send(sock , message , strlen(message), 0);
	int check = 0;

	//Waiting all clients be connected
	if(startMonte == 0) {
		do {
			if(startMonte == 1) {
				message2 = "All clients connected!";
				send(sock , message2 , strlen(message2),0);

				check = 1;
			}
		}while (check == 0);
	}
	else if(startMonte == 1) {
		message2 = "All clients connected!";
		send(sock , message2 , strlen(message2),0);
	}

	send(sock , &points, sizeof(points),0);
	
	recv(sock, &pi_buffer, sizeof(pi_buffer), 0);
	final_pi = final_pi + pi_buffer;
	printf("pi = %f\n", final_pi);
		
	//Free the socket pointer
	free(socket_server);
	
	return 0;
}
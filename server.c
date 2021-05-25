#include<stdio.h>
#include<string.h>
#include<stdlib.h>	
#include<sys/socket.h>
#include<arpa/inet.h>	
#include<unistd.h>	
#include<pthread.h> 
#define PORT 8888

int startMonte = 0;//Global Var for starting the Monte Carlo

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
		new_sock = malloc(1);
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
	char *message , *message2, client_message[2000];
	
	//Sending messages to the client
	message = '\0';
	message = "You are connected to the server... Waiting all other clients connect...";
	write(sock , message , strlen(message));
	int number = 23, check = 0;

	//Waiting all clients be connected
	do {
		if(startMonte == 1) {
			message2 = "All clients connected!";
			write(sock , message2 , strlen(message2));

			int num = 23;
			send(sock , &num, sizeof(num),0);
			
			check = 1;
		}
	}while (check == 0);

	
	//Receive a message from client
	/*while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
	{
		//Send the message back to client
		write(sock , client_message , strlen(client_message));
	}
	
	if(read_size == 0)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}
	*/
		
	//Free the socket pointer
	free(socket_server);
	
	return 0;
}
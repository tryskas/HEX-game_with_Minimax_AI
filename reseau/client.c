#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 12345
//#define SERVER_IP "127.0.0.1" // Adresse IP du serveur
//#define SERVER_PORT 20200 // Port du serveur
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024
 
// Fonction pour envoyer des données au serveur
void send_data_to_server(int client_socket, const char* data) {
    ssize_t bytes_sent;
 
    // Envoyer les données au serveur
    bytes_sent = send(client_socket, data, strlen(data), 0);
    if (bytes_sent < 0) {
        perror("Erreur lors de l'envoi des données au serveur");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
}
 
// Fonction pour recevoir des données du serveur
void handle_server(int client_socket,char* buffer) {
    ssize_t bytes_received;
 
    // Attendre et recevoir des données du client
    bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received < 0) {
        //perror("Erreur lors de la réception du message du client");
        //close(client_socket);
        return;
    }
 
    
    // Fermer la connexion avec le client
    //close(client_socket);

	// Afficher le message reçu du client
	printf("Message du client : %s\n", buffer);
	
}
 
int mainclient() {
    

    //char SERVER_IP[20];
    //printf("\n Tapez l'addresse ip du serveur: ");
    //scanf("%s",&SERVER_IP);

    int client_socket;
    struct sockaddr_in server_addr;
 
    // Créer une socket client
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Erreur lors de la création de la socket client");
        exit(EXIT_FAILURE);
    }
 
    // Configurer l'adresse du serveur
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);
    //printf("tentative de connexion en %s ",SERVER_IP);
    while(1){
        // Se connecter au serveur
        if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
            perror("Erreur lors de la connexion au serveur");
            close(client_socket);
            exit(EXIT_FAILURE);
        }
        return client_socket;
    }
    // Recevoir la réponse du serveur
    receive_data_from_server(client_socket);
 
    // Fermer la connexion avec le serveur
    close(client_socket);
 
    return 0;
}
 
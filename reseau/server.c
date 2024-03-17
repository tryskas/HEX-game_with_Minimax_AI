#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>

#define MAX_CLIENTS 5

#define PORT 12345
#define BUFFER_SIZE 1024
 

char* get_local_ip_address() {
    struct ifaddrs *ifaddr, *ifa;
    char ipstr[INET_ADDRSTRLEN];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return NULL;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET)
            continue;

        struct sockaddr_in *ipv4 = (struct sockaddr_in *)ifa->ifa_addr;
        inet_ntop(AF_INET, &(ipv4->sin_addr), ipstr, sizeof(ipstr));

        // Exclude loopback IP
        if (strcmp(ipstr, "127.0.0.1") != 0) {
            freeifaddrs(ifaddr);
            return strdup(ipstr);
        }
    }

    freeifaddrs(ifaddr);
    return NULL; // No suitable IPv4 address found (excluding loopback)
}

void handle_client(int client_socket,char* buffer) {
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

void send_data_to_client(int client_socket, const char* data) {
    ssize_t bytes_sent;
 
    // Envoyer les données au client
    bytes_sent = send(client_socket, data, strlen(data), 0);
    if (bytes_sent < 0) {
        perror("Erreur lors de l'envoi des données au client");
        close(client_socket);
    }
}
 
int mainserv() {

    

    char* ip_address = get_local_ip_address();
    printf("\n Votre adresse IPV4 est : %s\n",ip_address);


    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
 
    // Créer une socket serveur
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Erreur lors de la création de la socket serveur");
        exit(EXIT_FAILURE);
    }
 
    // Configurer l'adresse du serveur
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
 
    // Lier la socket serveur à l'adresse et au port spécifiés
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Erreur lors de la liaison de la socket serveur");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
 
    // Mettre le serveur en écoute
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Erreur lors de la mise en écoute du serveur");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
 
    printf("Serveur en attente de connexions...\n");
 
    while (1) {
        // Accepter une connexion entrante
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("Erreur lors de l'acceptation de la connexion du client");
            continue;
        }
 
        // Gérer la communication avec le client
		return client_socket;
        //handle_client(client_socket);
    }
 
    // Fermer la socket serveur (ce code ne sera jamais atteint)
    close(server_socket);
 
    return 0;
}
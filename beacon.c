#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>


void error(char *);

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: beacon <groupNumber>\n");
        exit(-1);
    }
    char groupNum[10];
    strcpy(groupNum, argv[1]);
    char stub[10] = "222";
    char broad_addr[20] = "239.10.5.";
    strcat(broad_addr, groupNum);
    strcat(stub, groupNum);
    int port = atoi(stub);
    struct sockaddr_in interface_addr;
    memset(&interface_addr, 0, sizeof(struct sockaddr_in));

    // create socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket < 0)
        error("---> Could not create a socket\n");

    // beacon target ip and port setup
    interface_addr.sin_family = AF_INET;
    interface_addr.sin_port = htons(port);
    interface_addr.sin_addr.s_addr = inet_addr(broad_addr);

    // socket binding
    if (bind(sockfd, (struct sockaddr *)&interface_addr,sizeof(struct sockaddr_in)) < 0)
        error("---> Problem binding the socket\n");
    unsigned char ttl = 20;
    setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));
    setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF, &interface_addr, sizeof(interface_addr));

    int count = 0;
    while (1) {
        char mesg[100];
        memset(mesg, 0, 100);
        sprintf(mesg, "This is message %d from the Group %s beacon\n", count, groupNum);

        // send beacon
        if (sendto(sockfd, mesg, strlen(mesg), 0, (struct sockaddr *)&interface_addr, sizeof(struct sockaddr_in)) < 0)
                error("---> Problem sending the message\n");
        count++;
        // wait one second
        sleep(1);
    }
}

void error(char * mesg) {
    fprintf(stderr, "%s", mesg);
    perror("Error: ");
    exit(-1);
}

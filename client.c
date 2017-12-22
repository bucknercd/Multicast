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

#define BUFSIZE 1024
#define PORT 22210

void error(char *);

int main(int argc, char **argv) {
    struct sockaddr_in interface_addr;
    struct ip_mreq imreq;
    time_t mytime;
    char buf[BUFSIZE];
    memset(&interface_addr, 0, sizeof(struct sockaddr_in));
    memset(&imreq, 0, sizeof(struct ip_mreq));
    memset(buf, 0, BUFSIZE);

    // create socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket < 0)
        error("---> Could not create a socket\n");

    // interface setup
    interface_addr.sin_family = AF_INET;
    interface_addr.sin_port = htons(PORT);
    interface_addr.sin_addr.s_addr = INADDR_ANY;


    // socket binding to interface
    if (bind(sockfd, (struct sockaddr *)&interface_addr,sizeof(struct sockaddr_in)) < 0)
        error("---> Problem binding the socket\n");

    // mulicast struct setup
    imreq.imr_multiaddr.s_addr = inet_addr("239.10.5.10");
    imreq.imr_interface.s_addr = INADDR_ANY;

    // JOIN multicast group on default interface
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const void *)&imreq, sizeof(struct ip_mreq)) < 0)
        error("---> Problem setting socket options");

    unsigned int socklen = sizeof(struct sockaddr_in);

    // receive packet from socket
    while (1) {
        char mesg[BUFSIZE];
        mytime = time(NULL);
        recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *)&interface_addr, &socklen);
        sprintf(mesg,"%s%s\n\n", buf, ctime(&mytime));
        fprintf(stdout, "%s", mesg);
    }
}

void error(char * mesg) {
    fprintf(stderr, "%s", mesg);
    perror("Error: ");
    exit(-1);
}

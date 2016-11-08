#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>

#define BUF_SIZE 500


// 1. Modify this code to use TCP/IP non blocking sockets
// 2. Use main loop Select() to decide when read is availble and write

int main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s, j;
    size_t len;
    ssize_t nread;
    char buf[BUF_SIZE];
    
    if (argc < 3) {
        fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    /* Obtain address(es) matching host/port */
    
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; // TCP Stream Socket
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */
    
    s = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }
    
    /* getaddrinfo() returns a list of address structures.
     Try each address until we successfully connect(2).
     If socket(2) (or connect(2)) fails, we (close the socket
     and) try the next address. */
    
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        // Open Socket
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        
        // Check to see if socket opens
        if (sfd == -1)
            continue;
        
        // Attempt to connect socket
        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            printf("Connected\n");
        break;                  /* Success */
        
        close(sfd);
    }
    
    if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not Bind\n");
        exit(EXIT_FAILURE);
    }
    
    freeaddrinfo(result);            /* No longer needed */
    
    /* Send remaining command-line arguments as separate
     datagrams, and read responses from server */
    
    for (j = 3; j < argc; ) {
        len = strlen(argv[j]) + 1;
        /* +1 for terminating null byte */
        
        // Check for buffer overflow
        if (len + 1 > BUF_SIZE) {
            fprintf(stderr,
                    "Ignoring long message in argument %d\n", j);
            continue;
        }
        
        // Write message to socket
        if (write(sfd, argv[j], len) != len) {
            fprintf(stderr, "partial/failed write\n");
            exit(EXIT_FAILURE);
        }
        
        printf("Written first message to Socket and now waiting to recive Message\n");
        nread = read(sfd, buf, BUF_SIZE);
        if (nread == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        
        printf("Received %zd bytes, Message: %s\n", nread, buf);
        sleep(2);
    }
    
    exit(EXIT_SUCCESS);
}

//
//  scComms.cpp
//  scComms
//
//  Created by Seth on 10/31/16.
//  Copyright © 2016 Seth. All rights reserved.
//


#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

class SpacecraftComms{
    int fd, fd_connect, localport;
    char hostname[256];
    
    int acceptSC() {
        
        // Open Socket
        std::cout << "Entered acceptSC() attempting to open socket ..." << std::endl;
        
        // Command to open Socket
        fd = socket(AF_INET, SOCK_STREAM, 0);
        
        // Check to see if Socket has been opened sucessfully
        if (fd == -1) {
            std::cerr << "failed to allocate socket\n";
            return -1;
        }
        
        
        // Bind Socket
        //Get computers hostname
        if (gethostname(hostname, sizeof (hostname)) != 0) {
            std::cerr << "Error unable to get hostname, errno = %d\n" << errno;
            return -2;
        }
        
        struct addrinfo *addrs, *addrs_head;
        std::cout << "HOSTNAME:\n" << hostname << "\n" << std::endl;
        
        int s = getaddrinfo(hostname, NULL, NULL, &addrs);
        
        
        if (s != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
            exit(EXIT_FAILURE);
        }
        
        
        std::cout << "Get ADDRINFO Completed" << std::endl;
        
        addrs_head = addrs; // Save the head to free the memory
        
        // iterate through all the alias until we find one that fits our parameters
        while (addrs != NULL) {
            if ( (addrs->ai_family == AF_INET) && (addrs->ai_socktype == SOCK_STREAM)
                ) {
                break;
            }
            addrs = addrs->ai_next;
        }
        
        if (addrs == NULL) {
            fprintf(stderr, "No address found\n");
            return -3;
        }
        
        sockaddr_in* sin = (sockaddr_in*) addrs->ai_addr;
        sin->sin_port = localport;
        
        
        if (bind(fd, (sockaddr*) sin, sizeof (*sin)) != 0) {
            fprintf(stderr, "bind() failed, errno = %d\n", errno);
            perror("errno = 49: ");
            return -4;
        }
        
        std::cout << "socket openned & bound" << std::endl;
        
        sockaddr saddr;
        saddr.sa_family = AF_INET; // CORE DUMPING HERE HA WHY???
        
        bcopy( hostname, saddr.sa_data, strlen(hostname));
        
        
        std::cout << "A" << std::endl;
        socklen_t len = sizeof(saddr);
        fd_connect = accept(fd, & saddr, &len);
        
        
        std::cout << "B" << std::endl;
        
        freeaddrinfo(addrs_head);
        
        std::cout << "C" << std::endl;
        
        return 0;
    }
    
    void error(std::string str) {
        std::cerr << str;
        exit(0);
    }
    
public:
    SpacecraftComms(){
        
    }
    
    SpacecraftComms(char * hostname, int port) {
        localport = port;
        bcopy(hostname,
              this->hostname,
              strlen(hostname));
    }
    
    int run() {
        // Connect to S/C Comms
        if (acceptSC() != 0) {
            fprintf(stderr, "failed to allocate socket\n");
            return -1;
        }
        
        std::cout << "D" << std::endl;
        
        int n;
        char buffer[256];
        while (1) {
            bzero(buffer,256);
            n = read(fd_connect, buffer, strlen(buffer));
            if (n < 0) {
                error("ERROR reading to socket");
                return -2;
            }
            std::cout << buffer << std::endl;
        }
    }
};


int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "usage %s hostname, localport";
        exit(0);
    }
    
    SpacecraftComms scComms(argv[1], atoi(argv[2]));
    scComms.run();
}
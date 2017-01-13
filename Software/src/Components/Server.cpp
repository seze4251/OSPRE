//
//  Server.h
//  Server
//
//  Created by Seth on 11/10/2016.
//  Copyright © 2016 Seth. All rights reserved.
//

#include "Server.h"
#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/select.h>
#include <signal.h>
#include <errno.h>
#include <sys/ioctl.h>

Server* Server::appl;

Server::Server() {
    std::cout << "Server Constructor called" << std::endl;
}

Server::~Server() {
    
}

Selector& Server::getSelector() {
//    std::cout << "getSelector \n";
    return sel;
}

Server* Server::getAppl() {
    return appl;
}

void Server::setAppl(Server* srv) {
    appl = srv;
}

void Server::handleTimeout() {
    std::cout << "Server::handleTimeout() I should do something here" << std::endl;
}

ErrorCode Server::run() {
    
    bool terminateFlag = false;
    while (terminateFlag == false) {
        timeval t;
        
        t.tv_sec = 5;
        t.tv_usec = 0;
        
        if (sel.select(&t) == -1) {
            std::cerr << "Server::run() select() error, exiting" << std::endl;
            break;
        }
        
        handleTimeout();
    }
    
    return SUCCESS;
}



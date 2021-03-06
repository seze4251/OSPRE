//
//  Selector.h
//  Selector
//
//  Created by Seth on 11/10/2016.
//  Copyright © 2016 Seth. All rights reserved.
//


#ifndef SELECTOR_H
#define SELECTOR_H

#include <sys/time.h>

class Service;

class Selector {
public:
    // Constructors
    Selector();
    
    // Destructor
    ~Selector();
    
    //Public Methods
    void registerService(int fd, Service *srv);
    void unregisterService(int fd);
    
    void interestInRead(int fd);
    void interestInWrite(int fd);
    
    void noInterestInRead(int fd);
    void noInterestInWrite(int fd);
    
    int select(timeval *timeout);
    
    void printFds(fd_set* set);
    
private:
    Service *services[FD_SETSIZE];
    fd_set readFds, writeFds;
    fd_set tempReadFds, tempWriteFds;
};

#endif

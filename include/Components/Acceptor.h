//
//  Acceptor.h
//  Acceptor
//
//  Created by Seth on 11/24/2016.
//  Copyright © 2016 Seth. All rights reserved.
//


#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "Service.h"
#include <string>

class Acceptor : public Service {
public:
    // Constructor
    Acceptor(Selector &sel);
    
    bool open(std::string hostName, int portNumber);
    void registerCallback(void (*callBackFunc)(int));
    
    virtual void handleRead();
    virtual void handleWrite();
    virtual bool isConnected() { return fd != -1 ? true : false; }
    virtual void closeConnection();
    
private:
    int port;
    std::string hostName;
    int fd;
    void (*callBack)(int);
};

#endif

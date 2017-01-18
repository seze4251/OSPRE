//
//  Server.h
//  Server
//
//  Created by Seth on 11/10/2016.
//  Copyright © 2016 Seth. All rights reserved.
//


#ifndef SERVER_H
#define SERVER_H

#include "ErrorCode.h"
#include "Selector.h"
#include "Acceptor.h"

class Server {
public:
    // Constructors
    Server();

    //Destructor
    virtual ~Server();
    
    //Public Methods
    ErrorCode run();
    Selector& getSelector();
    
    static Server* getAppl();

protected:
    static void setAppl(Server* srv);
    static Server* appl;
    virtual void handleTimeout();
    timeval t_val;
private:
    Selector sel;
};

#endif

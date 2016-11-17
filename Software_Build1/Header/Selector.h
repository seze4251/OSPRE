//
//  Selector.h
//  Selector
//
//  Created by Seth on 11/10/2016.
//  Copyright © 2016 Seth. All rights reserved.
//


#ifndef SELECTOR_H
#def SELECTOR_H

#include "ErrorCode.h"
#include <sys/select.h>

// *****Needed???********
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
// *****Needed???********

class Selector {
public:
    // Constructors
    Selector();
    
    // Destructor
    ~Selector();
    
    //Public Methods
    ErrorCode select();
    ErrorCode select(timeval* timeout);
    ErrorCode selectNow();
    
private:
    fd_set readFds, writeFds;
    int *fd;

    
};

#endif
//
//  WatchDogClientHandler.cpp
//  WatchDogClientHandler
//
//  Created by Seth on 11/27/2016.
//  Copyright © 2016 Seth. All rights reserved.
//


#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#include "ServiceInternal.h"

// Constructor
ServiceInternal::ServiceInternal(Selector& sel, int fd, int buffSize) : Service(sel), fd(fd), readbuf(buffSize), writebuf(buffSize), build(writebuf), parse(readbuf), partialMessage(false) {
    std::cout << "ServiceInternal Constructor Called" << std::endl;
}

// Destructor
ServiceInternal::~ServiceInternal() {
    
}

//Initialize InternalService Methods
bool ServiceInternal::open(int fd) {
    std::cout << "Service Internal Open(int fd)" << std::endl;
    this->fd = fd;
    getSelector().registerService(fd, this);
    getSelector().interestInRead(fd);
    return true;
}

bool ServiceInternal::open(std::string hostName, int portNumber) {
    fd = Service::connectToServer(hostName.c_str(), portNumber);
    
    if (fd == -1) {
        return false;
    }
    
    getSelector().registerService(fd, this);
    getSelector().interestInRead(fd);
    return true;
}

void ServiceInternal::registerCallback(void (*messageCallBackFunc)(Message*, ServiceInternal*)) {
    std::cout << "ServiceInternal registerCallback()" << std::endl;
    messageCallBack = messageCallBackFunc;
}

// Virtual Methods from Server
void ServiceInternal::handleRead() {
    //std::cout << "Entering ServiceInternal::handleRead()" << std::endl;
    int length = readbuf.remaining();
    
    if (length == 0) {
        std::cout << "ServiceInternal::handleRead(): No room to read into buffer" << std::endl;
    }
    
    char* buf = readbuf.getBuffer();
    int amountRead = -1;
    
    while (amountRead < 0) {
        amountRead = ::read(fd, buf, length);
        //  std::cout << "handleRead(): Amount Read: " << amountRead << std::endl;
        
        // ToDo: Revisit code confiriming MANPAGE on read error conditions
        if (amountRead == 0) {
            std::cerr << "ServiceInternal::handleRead(): read() returned 0, closing connection" << std::endl;
            closeConnection();
            return;
            
        } else if (amountRead == -1) {
            if (errno == EINTR) {
                continue;
            } else {
                perror("Error: ");
                closeConnection();
                return;
            }
        }
    }
    
    readbuf.positionRead(amountRead);
    
    ///**************
    //TEMP
    // readbuf.printBuffer();
    //TEMP
    //********************
    
    Message* msg = nullptr;
    int count = 0;
    
    // Flip Buffer
    readbuf.flip();
    
    while (true) {
        msg = parse.parseMessage(&partialMessage);
        if (msg == nullptr) {
            break;
        }
        
        (*messageCallBack)(msg, this);
        count++;
    }
    
    //Compact and flip buffer
    readbuf.compact();
    readbuf.flip();
    
    if ((count == 0) && (partialMessage = false)) {
        std::cerr << "ServiceInternal::handleRead(): Message is big for buffer" << std::endl;
        closeConnection();
        return;
    }
    
    std::cout << "ServiceInternal::handleRead(): messagesParsed: " << count << std::endl;
}



void ServiceInternal::handleWrite() {
    //  std::cout << "Entering ServiceInternal::handleWrite()" << std::endl;
    int length = writebuf.used();
    //  std::cout << "handleWrite writebuf.used: " << length << std::endl;
    
    if (length == 0) {
        std::cout << "Nothing Left to Write to Socket" << std::endl;
        getSelector().noInterestInWrite(fd);
        return;
    }
    
    
    //std::cout << "ServiceInternal::handleWrite(): Printing Write Buffer" << std::endl;
    //writebuf.printBuffer();
    
    
    writebuf.flip();
    
    char* buf = writebuf.getBuffer();
    
    int amountWritten = write(fd, buf, length);
    if (amountWritten == -1) {
        perror("Write Error: ");
    }
    
    std::cout << "Wrote " << amountWritten << " Bytes" << std::endl;
    if (amountWritten == length) {
        writebuf.clear();
        getSelector().noInterestInWrite(fd);
    } else {
        // Compact Buffer and keep the interest in writing with selector
        writebuf.positionWrite(amountWritten);
        writebuf.compact();
        writebuf.flip();
    }
}

void ServiceInternal::closeConnection() {
    if (fd != -1) {
        getSelector().unregisterService(fd);
        ::close(fd);
        fd = -1;
    }
    readbuf.clear();
    writebuf.clear();
}




void ServiceInternal::sendMessage(Message* msg) {
    if (isConnected() == false) {
        std::cout << "ServiceInternal::sendMessage Service is not Connected, returning" << std::endl;
        return;
    }
    
    //std::cout << "ServiceInternal::sendMessage() Sending Message" << std::endl;
    
    switch (msg->iden) {
        case I_CaptureImageRequest:
            build.buildCaptureImageRequest(*((CaptureImageRequest*) msg));
            break;
            
        case I_DataMessage:
            build.buildDataMessage(*((DataMessage*) msg));
            break;
            
        case I_ImageAdjustment:
            build.buildImageAdjustment(*((ImageAdjustment*) msg));
            break;
            
        case I_ImageMessage:
            build.buildImageMessage(*((ImageMessage*) msg));
            break;
            
        case I_OSPREStatus:
            build.buildOSPREStatus(*((OSPREStatus*) msg));
            break;
            
        case I_PointingRequest:
            build.buildPointingRequest(*((PointingRequest*) msg));
            break;
            
        case I_ProcessHealthAndStatusRequest:
            build.buildProcessHealthAndStatusRequest(*((ProcessHealthAndStatusRequest*) msg));
            break;
            
        case I_ProcessHealthAndStatusResponse:
            build.buildProcessHealthAndStatusResponse(*((ProcessHealthAndStatusResponse*) msg));
            break;
        case I_SolutionMessage:
            build.buildSolutionMessage(*((SolutionMessage*) msg));
            
            break;
        case I_ProcessedImageMessage:
            build.buildProcessedImageMessage(*((ProcessedImageMessage*) msg));
            break;
        default:
            std::cout << "ServiceInternal::sendMessage() msgID unknown, process exiting ..." << std::endl;
            exit(-1);
    }
    
    // Register Intrest in Write
    getSelector().interestInWrite(fd);
}








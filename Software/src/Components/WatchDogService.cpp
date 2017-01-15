//
//  WatchDogService.cpp
//  WatchDogService
//
//  Created by Seth on 11/27/2016.
//  Copyright © 2016 Seth. All rights reserved.
//

#include "WatchDogService.h"
#include <unistd.h>
#include <iostream>

WatchDogService::WatchDogService(Selector &sel, std::string hostName, int portNumber) : ServiceInternal(sel), hostName(hostName), portNumber(portNumber) {
    std::cout << "WatchDogService Constructor called" << std::endl;
    p_ID = P_NA;
}

WatchDogService::~WatchDogService() {
    
}

bool WatchDogService::open() {
    // Connect to WatchDog
    fd = Service::connectToServer(hostName.c_str(), portNumber);
    getSelector().registerService(fd, this);
    getSelector().interestInRead(fd);
    return true;
}

void WatchDogService::closeConnection() {
    //std::cout << "WatchDogService closeConnection() " << std::endl;
    if (fd != -1) {
        ::close(fd);
        fd = -1;
    }
    readbuf.clear();
    writebuf.clear();
}

void WatchDogService::sendStatusResponseMessage() {
    if (isConnected() == false) {
        return;
    }
    
    // Create Message
    ProccessHealthAndStatusResponse* msg = new ProccessHealthAndStatusResponse(p_ID);
    // Put Message in Write Buffer
    build.buildProccessHealthAndStatusResponse(*msg);
    // Register Intrest in Write
    getSelector().interestInWrite(fd);
}


// Message Handlers
void WatchDogService::handleProccessHealthAndStatusRequest(ProccessHealthAndStatusRequest* msg) {
    std::cout << "WatchDogService::handleProccessHealthAndStatusRequest(): Process Health and Status Response Received" << std::endl;
    sendStatusResponseMessage();
    
}

// *******************************
//
// Message Handler Defualt Behavior Below
//
// ********************************

void WatchDogService::handleProccessHealthAndStatusResponse(ProccessHealthAndStatusResponse* msg) {
    std::cerr << "handleProccessHealthAndStatusResponse() Not Supported for WatchDogService" << std::endl;
}

void WatchDogService::handleCaptureImageRequest(CaptureImageRequest* msg) {
    std::cerr << "handleCaptureImageRequest() Not Supported for WatchDogService" << std::endl;
}

void WatchDogService::handleDataRequest(DataRequest* msg) {
    std::cerr << "handleDataRequest() Not Supported for WatchDogService" << std::endl;
}
void WatchDogService::handleEphemerisMessage(EphemerisMessage* msg) {
    std::cerr << "handleEphemerisMessage() Not Supported for WatchDogService" << std::endl;
}
void WatchDogService::handleImageAdjustment(ImageAdjustment* msg) {
    std::cerr << "handleImageAdjustment() Not Supported for WatchDogService" << std::endl;
}
void WatchDogService::handleImageMessage(ImageMessage* msg) {
    std::cerr << "handleImageMessage() Not Supported for WatchDogService" << std::endl;
}
void WatchDogService::handleOSPREStatus(OSPREStatus* msg) {
    std::cerr << "handleOSPREStatus() Not Supported for WatchDogService" << std::endl;
}
void WatchDogService::handlePointingRequest(PointingRequest* msg) {
    std::cerr << "handlePointingRequest() Not Supported for WatchDogService" << std::endl;
}

void WatchDogService::handleSolutionMessage(SolutionMessage* msg){
    std::cerr << "handleSolutionMessage() Not Supported for WatchDogService" << std::endl;
}





//
//  Spacecraft.h
//  Spacecraft
//
//  Created by Seth on 11/10/2016.
//  Copyright © 2016 Seth. All rights reserved.
//


#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include <string>
#include <ctime>
#include <stdio.h>

#include "Server.h"
#include "ServiceExternal.h"
#include "External_Messages.h"
#include "SpacecraftDataReader.h"

class Spacecraft : public Server {
public:
    // Constructor
    Spacecraft(std::string hostName, int localPort);
    
    // Destructor
    ~Spacecraft();
    
    virtual void handleTimeout();
    void open();
    
    // Generic Message Handler for External Messages
    static void handleExternalMessage(Message_External* msg, ServiceExternal* service);
    
    // Additional Message Handler for Spacecraft
    void handleExternalDataMessage(External_DataMessage* msg, ServiceExternal* service);
    void handleExternalOSPREStatusMessage(External_OSPREStatus* msg, ServiceExternal* service);
    void handleExternalPointingMessage(External_PointingRequest* msg, ServiceExternal* service);
    void handleExternalSolutionMessage(External_SolutionMessage* msg, ServiceExternal* service);
    
private:
    // Result File
    FILE* resultFile;
    
    // Connect to OSPRE
    std::string ospreHostName;
    int osprePort;
    
    // System Architecture
    time_t pollTime;
    static ServiceExternal* scComms;
    
    // Pointer To Hold Messages that Are being sent
    External_DataMessage* dataMessage;
    
    SpacecraftDataReader scDataReader;
};

#endif



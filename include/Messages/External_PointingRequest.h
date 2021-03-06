//
//  External_PointingRequest.h
//  External_PointingRequest
//
//  Created by Seth on 11/24/2016.
//  Copyright © 2016 Seth. All rights reserved.
//


#ifndef EXTERNAL_POINTINGREQUEST_H
#define EXTERNAL_POINTINGREQUEST_H

#include "Message_External.h"
#include "PointEarthMoon.h"

class External_PointingRequest : public Message_External {
public:
    
    // Need to set: packetType, sequenceFlags, packetDataLength
    External_PointingRequest(unsigned int applicationProcessID) : Message_External(applicationProcessID, E_PointingRequest) {
        header.header_struct.packetType = 1;
        header.header_struct.sequenceFlags = 3;
        header.header_struct.packetSequence = 0;
        header.header_struct.packetDataLength = (8) - 1;
    }
    
    // Update Message
    void update(PointEarthMoon point) {
        this->point = point;
    }
    
    // Print Message
    void print(FILE* logFile) {
        fprintf(logFile, "Printing External Pointing Request\n");
        printHeader(logFile);
        printEarthMoon(point, logFile);
    }
    
    // Specific Data Members
    PointEarthMoon point;
    
};
#endif

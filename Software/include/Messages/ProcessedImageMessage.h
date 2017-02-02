//
//  ProcessedImageMessage.h
//  ProcessedImageMessage
//
//  Created by Seth on 1/26/2017.
//  Copyright © 2016 Seth. All rights reserved.
//


#ifndef PROCESSEDIMAGEMESSAGE_H
#define PROCESSEDIMAGEMESSAGE_H

#include <vector>

#include "Message.h"

class ProcessedImageMessage : public Message {
public:
    ProcessedImageMessage() : Message(getMessageID(), time(0)){}
    
    MessageID getMessageID() {return I_ProcessedImageMessage;}
    
    void update(double distance, double error, PointEarthMoon point) {
        this->distance = distance;
        this->error = error;
        this->point = point;
    }
    
    // Specific Data Members
    double distance;
    double error;
    PointEarthMoon point;
};

#endif

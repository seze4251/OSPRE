//
//  OSPREStatus.h
//  OSPREStatus
//
//  Created by Seth on 11/10/2016.
//  Copyright © 2016 Seth. All rights reserved.
//


#ifndef OSPRESTATUS_H
#define OSPRESTATUS_H

#include "Message.h"

class OSPREStatus : public Message {
public:
    MessageID getMessageID() { return I_OSPREStatus; }
};


#endif

//
//  ImageMessage.h
//  ImageMessage
//
//  Created by Seth on 11/24/2016.
//  Copyright © 2016 Seth. All rights reserved.
//


#ifndef IMAGEMESSAGE_H
#define IMAGEMESSAGE_H

#include "Message.h"

class ImageMessage : public Message {
public:
    MessageID getMessageID() { return I_ImageMessage; }
};

#endif

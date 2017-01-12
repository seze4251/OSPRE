//
//  mainScComms.cpp
//
//  Created by Seth on 11/24/2016.
//  Copyright © 2016 Seth. All rights reserved.
//

#include <unistd.h>
#include <iostream>
#include "ScComms.h"

int main(int argc, char **argv) {
    std::cout << "\nScComms Application Starting\n";
    ScComms comms(5000);
    std::cout << "ScComms Initalized\n";
    comms.run();
    std::cout << "ScComms Application Terminating\n";
    return 0;

}
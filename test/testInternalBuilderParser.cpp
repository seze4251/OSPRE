//
//  testBuilder.cpp
//
//  Created by Seth on 02/14/2017.
//  Copyright © 2016 Seth. All rights reserved.
//

#include <iostream>

#include "Builder.h"

int main(int argc, char **argv) {
    std::cout << "\nTesting InternalBuilderParser Starting\n";
    bool passedAllTests = true;
    
    
    
    if (passedAllTests == true) {
        std::cout << "InternalBuilderParser Passed All Tests" << std::endl;
    } else {
        std::cout << "InternalBuilderParser Failed one or multiple Tests" << std::endl;
    }

    std::cout << "InternalBuilderParser Builder Terminating\n\n";
    return 0;
}

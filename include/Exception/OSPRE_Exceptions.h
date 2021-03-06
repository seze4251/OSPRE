//
//  OSPRE_Exceptions.h
//  OSPRE_Exceptions
//
//  Created by Seth on 2/14/2017.
//  Copyright © 2017 Seth. All rights reserved.
//

// Link to all std::exceptions
// http://en.cppreference.com/w/cpp/error/exception

#ifndef OSPREEXCEPTIONS_H
#define OSPREEXCEPTIONS_H

#include <string>
#include <exception> // for std::exception

class OSPRE_Exception {
public:
    OSPRE_Exception(std::string error) : m_error(error) {}
    
    const char* what() const { return m_error.c_str(); }
    
private:
    std::string m_error;
};

//Image Processor
class NoBodyInImage : public OSPRE_Exception {
public:
    NoBodyInImage(std::string error) : OSPRE_Exception(error) {}
};

//Image Processor
class InvalidInputs : public OSPRE_Exception {
public:
    InvalidInputs(std::string error) : OSPRE_Exception(error) {}
};

//Image Processor
class InvalidAlphaBetaTheta : public OSPRE_Exception {
public:
    InvalidAlphaBetaTheta(std::string error) : OSPRE_Exception(error) {}
};

//Image Processor
class InvalidPosition : public OSPRE_Exception {
public:
    InvalidPosition(std::string error) : OSPRE_Exception(error) {}
};

// GNC
class TextFileImproperFormat : public OSPRE_Exception {
public:
    TextFileImproperFormat(std::string error) : OSPRE_Exception(error) {}
};

// GNC
class InvalidOutput : public OSPRE_Exception {
public:
    InvalidOutput(std::string error) : OSPRE_Exception(error) {}
};

// GNC
// Camera Controller
class InvalidFileName : public OSPRE_Exception {
public:
    InvalidFileName(std::string error) : OSPRE_Exception(error) {}
};

//Camera Controller
class InvalidImageDimensions : public OSPRE_Exception {
public:
    InvalidImageDimensions(std::string error) : OSPRE_Exception(error) {}
};


#endif







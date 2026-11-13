#include "encryptPin.h"
#include "systemFiles.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>


#ifndef CHECKPIN_H
#define CHECKPIN_H

//  declaration for called & caller function checkPin()
//  
void checkPin( const size_t& line, int promptInput );

//  declaration for called function getUserPin()
//  
std::pair<bool, int> getUserPin( const size_t& userLine );

//  declaration for called function getNewPin()
//  
std::pair<bool, int> getNewPin();

#endif

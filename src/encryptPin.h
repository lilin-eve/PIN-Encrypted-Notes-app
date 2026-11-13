#include <iostream>
#include <vector>
#include <string>


#ifndef ENCRYPTPIN_H
#define ENCRYPTPIN_H

//  declaration for called & caller function encryptPin()
//  function calls splitDigits(), encryptPinDigits(), & combineDigits()
//  
int encryptPin( const int& pin );

//  declaration for called & caller function decryptPin()
//  function calls splitDigits(), decryptPinDigits(), & combineDigits()
//  
int decryptPin( const int& pin );

#endif

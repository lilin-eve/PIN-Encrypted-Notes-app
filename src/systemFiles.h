#include "encryptPin.h"
#include "userFiles.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>


#ifndef SYSTEMFILES_H
#define SYSTEMFILES_H

//  declaration for called & caller function confirmTargetLine()
//  function calls encryptPin()
//  
bool confirmTargetLine( const std::string& fileName, const size_t& lineTarget, const std::string& searchTarget );

//  declaration for called & caller function deleteFileLine()
//  function calls getSystemFile() & saveFile()
//  
bool deleteFileLine( const std::string& fileName, const size_t& lineTarget );

//  declaration for called function errorRefUser()
//  
void errorRefUser( const size_t& userLine );

//  declaration for called function errorSearchFile()
//  
void errorSearchFile( const std::string& dirName, const std::string& fileName );

//  declaration for called & caller function getFileLine()
//  function calls getSystemFile()
//  
std::pair<bool, std::string> getFileLine( const std::string& fileName, const size_t& lineNum );

//  declaration for called function getSystemFile()
//  
std::pair<bool, std::vector<std::string>> getSystemFile( const std::string& fileName );

//  declaration for called function saveFile()
//  
bool saveFile( const std::string& dirName, const std::string& fileName, const std::vector<std::string>& fileLines );

//  declaration for called & caller function searchSystemFile()
//  function calls encryptPin()
//  
std::tuple<bool, size_t, size_t> searchSystemFile( const std::string& fileName, const std::string& searchTarget );

//  declaration for called & caller function updateFileLine()
//  function calls getSystemFile() & saveFile()
//  
bool updateFileLine( const std::string& fileName, const size_t& lineTarget, const std::string& newLineContent );

#endif







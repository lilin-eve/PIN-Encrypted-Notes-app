#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


#ifndef USERFILES_H
#define USERFILES_H

//  declaration for called function createDirectory()
//  
//  bool createDirectory( const std::string& dirName );

//  declaration for called function deleteDirectory()
//  
std::pair<bool, std::uintmax_t> deleteDirectory( const std::string& dirName );

//  definition for called & caller function deleteUserNote()
//  
bool deleteUserFile( const std::string& userName, const std::string& fileName );

//  definition for called function encryptDecryptFile()
//  
std::vector<std::string> encryptDecryptUserFile( const std::vector<std::string>& fileWords, const std::string& pinString );

//  definition for called & caller function getFileContent()
//  function calls encryptDecryptUserFile()
//  
std::pair<bool, std::vector<std::string>> getUserFile( const std::string& userName, const std::string& fileName, const int& pin );

#endif

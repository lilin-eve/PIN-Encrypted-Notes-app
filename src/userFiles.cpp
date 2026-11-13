#include "userFiles.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


//  declaration for function clearScreen()
void clearScreen();

//  declaration for function printBorderLine()
void printBorderLine();


//  definition for called function deleteDirectory()
//  function is type pair, returns bool & std::uintmax_t values to caller function
//  function receives 1 const ref to 1 string value, locally named 'dirName'
//  
std::pair<bool, std::uintmax_t> deleteDirectory( const std::string& dirName ){

    bool verifyDeleteDir{false};
    std::uintmax_t fileDeleteCount{};

    try {
        std::uintmax_t filesRemoved = std::filesystem::remove_all( dirName );
        fileDeleteCount = filesRemoved - 1;
        verifyDeleteDir = true;
    } catch ( const std::filesystem::filesystem_error& err ){
        (void)err;
        fileDeleteCount = 0;
        verifyDeleteDir = false;
    }

    return { verifyDeleteDir, fileDeleteCount };

}

//  definition for called function deleteUserFile()
//  function is type bool, returns true or false value to caller function
//  function receives 2 const refs to 2 strings values, locally named 'userName' & 'fileName'
//  
bool deleteUserFile( const std::string& userName, const std::string& fileName ){

    bool verifyFileDelete{false};

    const std::filesystem::path filePath( "../users/" + userName + "/" + fileName );

    if ( !std::filesystem::remove(filePath) ){
        verifyFileDelete = false;
    }
    else {
        verifyFileDelete = true;
    }

    return verifyFileDelete;

}

//  definition for called function encryptDecryptUserFile()
//  function is type string vector, returns a vector of string elements to caller function
//  function receives 2 const refs to 1 string vector, locally named 'fileWords, & 1 string value, locally named 'pinString'
//  
std::vector<std::string> encryptDecryptUserFile( const std::vector<std::string>& fileWords, const std::string& pinString ){

    std::vector<std::string> fileWordsEncrypt{};

    for ( size_t i = 0; i < fileWords.size(); i++ ){
        std::string fileWordEncrypt = fileWords[i];
        for ( size_t x = 0; x < fileWords[i].size(); x++ ){
            fileWordEncrypt[x] = fileWords[i][x] ^ pinString[ x % pinString.size() ]; 
        }
        fileWordsEncrypt.push_back( fileWordEncrypt );
    }

    return fileWordsEncrypt;

}

//  definition for called & caller function getUserFile()
//  function is type pair, returns bool & string vector
//  function receives 3 const refs to 2 string values, locally named 'userName' & 'fileName, & 1 int value, locally named 'pin'
//  function calls encryptDecryptUserFile()
//  
std::pair<bool, std::vector<std::string>> getUserFile( const std::string& userName, const std::string& fileName, const int& pin ){

    const std::filesystem::path filePath{ "../users/" + userName + "/" + fileName };
    std::string line{};
    std::string word{};
    std::vector<std::string> fileWordsEncrypt{};
    std::vector<size_t> fileLineWordCount{};

    std::vector<std::string> fileLinesDecrypt{};

    bool gotFileContent{false};

    if ( std::filesystem::exists( filePath ) ){
        std::ifstream readFile{ filePath };
        if ( readFile.is_open() ){
            while ( getline( readFile, line )){
                size_t lineWordCount{0};
                std::istringstream stream(line);
                while ( stream >> word ){
                    fileWordsEncrypt.push_back(word);
                    lineWordCount++;
                }
                fileLineWordCount.push_back(lineWordCount);
                lineWordCount = 0;
            }
            readFile.close();
        }
        else {
            gotFileContent = false;
        }

        //  function decrypts words from file with user-input pin, so file lines returned by this function don't need to be decrypted, saves time 
        const std::vector<std::string> fileWordsDecrypt{ encryptDecryptUserFile( fileWordsEncrypt, std::to_string(pin) ) };

        //  std::vector<std::string> fileLinesDecrypt{};

        size_t beginWordNum{0};

        for ( size_t i = 0; i < fileLineWordCount.size(); i++ ){
            //  std::string fileLine{};
            for ( size_t x = 0; x < fileLineWordCount[i]; x++ ){
                if ( x == 0 ){
                    line = fileWordsDecrypt[ beginWordNum ] + " ";
                }
                else {
                    line = line + fileWordsDecrypt[ beginWordNum ] + " ";
                }
                beginWordNum++;
            }
            fileLinesDecrypt.push_back(line);
            line.clear();
        }

        gotFileContent = true;

    }
    else {
        gotFileContent = false;
    }
    

    return { gotFileContent, fileLinesDecrypt };

}

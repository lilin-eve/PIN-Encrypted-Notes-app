#include "systemFiles.h"
#include "encryptPin.h"
#include "userFiles.h"
#include <iostream>
#include <filesystem>
#include <fstream>
//  #include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <system_error>


//  declaration for called function printBorderLine()
void printBorderLine();


//  definition for called & caller function confirmTargetLine()
//  function is type bool, returns true or false value to caller function
//  function receives 3 const refs to 2 string values, locally named 'fileName' & 'searchTarget', & 1 size_t value, locally named 'lineTarget'
//  function calls encryptPin()
//  
bool confirmTargetLine( const std::string& fileName, const size_t& lineTarget, const std::string& searchTarget ){

    bool verifyTargetLine{false};
    std::string line{};
    size_t lineNum{0};

    const std::filesystem::path filePath{ "../sys/" + fileName };
    std::ifstream readSystemFile{ filePath };

    if ( readSystemFile.is_open() ){
        if ( fileName == "users.txt" ){
            while (getline(readSystemFile, line)){
                if ( line.find(searchTarget) != std::string::npos && lineNum == lineTarget ){
                    verifyTargetLine = true;
                }
                lineNum++;
            }
        }
        else if ( fileName == "user-pins.txt" ){
            int pin{std::stoi(searchTarget)};
            int encryptedPin{ encryptPin(pin) };
            std::string encryptPinString{ std::to_string( encryptedPin ) };
            while (getline(readSystemFile, line)){
                if ( line.find(encryptPinString) != std::string::npos && lineNum == lineTarget ){
                    verifyTargetLine = true;
                }
                lineNum++;
            }
        }
        readSystemFile.close();
    }
    else {
        verifyTargetLine = false;
    }

    return verifyTargetLine;

}

//  definition for called function errorRefUser()
//  function is type void, returns no value to caller function
//  function receives 1 const ref to 1 size_t value, locally named 'userLine'
//  
void errorRefUser( const size_t& userLine ){

    printBorderLine();
    std::cout << "Error! User profile unable to be referenced properly from line " << userLine << " of file ' sys/users.txt '. " << std::endl;

}

//  definition for called function errorSearchFile()
//  function is type void, returns no value to caller function
//  function receives 2 const refs to 2 string values, locally named 'dirName' & 'fileName'
//  
void errorSearchFile( const std::string& dirName, const std::string& fileName ){

    printBorderLine();
    std::cout << "File ' " << dirName << "/" << fileName << " ' not found. " << std::endl;

}

//  definition for called function getSystemFile()
//  function is type pair, returns bool & string vector to caller function
//  function receives 2 const refs to 2 string values, locally named 'dirName' & 'fileName'
//  removed parameter 'dirName'
//  
std::pair<bool, std::vector<std::string>> getSystemFile( const std::string& fileName ){

    bool gotSystemFile{false};

    //  const std::filesystem::path filePath{ "../" + dirName + fileName };
    const std::filesystem::path filePath{ "../sys/" + fileName };

    std::vector<std::string> fileLines{};
    std::string line{};

    if ( std::filesystem::exists(filePath) ){
        std::ifstream readSystemFile{ filePath };
        if ( readSystemFile.is_open() ){
            while (getline(readSystemFile, line )){
                fileLines.push_back(line);
            }
            gotSystemFile = true;
            readSystemFile.close();
        }
        else {
            gotSystemFile = false;
        }
    }   
    else {
        gotSystemFile = false;
    }

    return {gotSystemFile, fileLines};

}

//  defintion for called & caller function getFileLine()
//  function is type pair, returns bool & string value to caller function
//  function receives 3 const refs to 2 string values, locally named 'dirName' & 'fileName', & 1 size_t value, locally named 'lineNum'
//  function calls getSystemFile()
//  removed parameter 'dirName'
//  
std::pair<bool, std::string> getFileLine( const std::string& fileName, const size_t& lineNum ){

    bool gotFileLine{false};

    std::string lineContent{};

    auto [ gotFile, fileLines ] = getSystemFile( fileName );

    if ( gotFile == true ){
        if ( !fileLines[lineNum].empty() ){
            lineContent = fileLines[lineNum];
            gotFileLine = true;
        }
        else{
            gotFileLine = false;
        } 
    }

    return {gotFileLine, lineContent};

}

//  definition for called function initSystemFiles()
//  function is type bool, returns true or false value to caller function
//  
bool initSystemFiles(){

    bool verifySystemFiles{false};

    int checkSystemFiles{0};

    const std::filesystem::path pathSys{ "../sys" };
    const std::filesystem::path pathUsers{ "../users" };

    //  function creates local directory 'sys/' if doesn't already exist 
    //  1
    if ( !std::filesystem::exists( pathSys ) ){
        if ( !std::filesystem::create_directory( pathSys ) ){
            checkSystemFiles = 0;
        }
        else {
            checkSystemFiles++;
        }
    }
    else {
        checkSystemFiles++;
    }

    //  function creates local directory 'users/' if doesn't already exist
    //  2
    if ( !std::filesystem::exists( pathUsers ) ){
        if ( !std::filesystem::create_directory( pathUsers ) ){
            checkSystemFiles = 0;
        }
        else {
            checkSystemFiles++;
        }
    }
    else {
        checkSystemFiles++;
    }

    //  function creates file 'users.txt' in directory 'sys/' if doesn't already exist
    //  3
    std::ofstream openFileUsers{ "../sys/users.txt", std::ios::app };
    if ( openFileUsers.is_open() ){
        openFileUsers.close();
        checkSystemFiles++;
    }
    else {
        checkSystemFiles = 0;
    }

    //  function creates file 'user-pins.txt' in directory 'sys/' if doesn't already exist
    //  4
    std::ofstream openFileUserPins{ "../sys/user-pins.txt", std::ios::app };
    if ( openFileUserPins.is_open() ){
        openFileUserPins.close();
        checkSystemFiles++;
    }
    else {
        checkSystemFiles = 0;
    }

    if ( checkSystemFiles == 4 ){
        verifySystemFiles = true;
        checkSystemFiles = 0;
    }
    else {
        verifySystemFiles = false;
        checkSystemFiles = 0;
    }

    return verifySystemFiles;

}

//  definition for called function saveFile()
//  function is type bool, return true or false value to caller function
//  function receives 3 const refs to 2 string values, locally named 'dirName' & 'fileName', & 1 string vector, locally named 'fileLines'
//  
bool saveFile( const std::string& dirName, const std::string& fileName, const std::vector<std::string>& fileLines ){

    bool verifyFileSave{false};

    const std::filesystem::path filePath{ dirName + fileName };

    std::ofstream editFile{ filePath };

    if ( editFile.is_open() ){
        for ( size_t i = 0; i < fileLines.size(); i++ ){
            editFile << fileLines[i] + "\n";
        }
        editFile.close();
        verifyFileSave = true;
    }
    else{
        verifyFileSave = false;
    }

    return verifyFileSave;

}

//  definition for called & caller function searchSystemFile()
//  function is type tuple, returns bool & 2 size_t values to caller function
//  function receives 2 const refs to 2 string values, locally named 'fileName' & 'searchTarget'
//  function calls encryptPin()
//  
std::tuple<bool, size_t, size_t> searchSystemFile( const std::string& fileName, const std::string& searchTarget ){

    bool foundSearchTarget{false};
    std::string line{};
    size_t lineTotal{0};
    size_t lineTarget{}; 

    const std::filesystem::path filePath{ "../sys/" + fileName };
    std::ifstream readSystemFile{ filePath };

    if ( readSystemFile.is_open() ){
        if ( fileName == "users.txt" ){
            while (getline( readSystemFile, line )){
                if ( line.find(searchTarget) != std::string::npos ){
                    lineTarget = lineTotal;
                    foundSearchTarget = true;
                }
                lineTotal++;
            }
        }
        else if ( fileName == "user-pins.txt" ){
            int pin{std::stoi(searchTarget)};
            int encryptedPin{ encryptPin(pin) };
            std::string encryptPinString{ std::to_string( encryptedPin ) };
            while (getline(readSystemFile, line)){
                if ( line.find(encryptPinString) != std::string::npos ){
                    lineTarget = lineTotal;
                    foundSearchTarget = true;
                }
                lineTotal++;
            }
        }
        readSystemFile.close();
    }
    else {
        foundSearchTarget = false;
        lineTarget = 0;
        lineTotal = 0;
    }

    return {foundSearchTarget, lineTotal, lineTarget};

}

//  definition for called & caller function deleteFileLine()
//  function is type bool, returns true or false value to caller function
//  function receives 3 const refs to 2 string values, locally named 'dirName' & 'fileName', & 1 size_t value, locally named 'lineTarget'
//  function calls getSystemFile() & saveFile()
//  
bool deleteFileLine( const std::string& fileName, const size_t& lineTarget ){

    bool verifyLineDelete{false};

    auto [ gotFile, fileLines ] = getSystemFile( fileName );

    const std::string dirName{ "../sys/" };

    if ( gotFile == true ){
        std::vector<std::string> newFileLines{};
        for ( size_t i = 0; i < fileLines.size(); i++ ){
            if ( i != lineTarget ){
                newFileLines.push_back(fileLines[i]);
            }
        }
        if ( saveFile( dirName, fileName, newFileLines ) == true ) {
            verifyLineDelete = true;
        }
    }

    return verifyLineDelete;

}

//  definition for called & caller function updateFileLine()
//  function is type bool, returns true or false value to caller function
//  function receives 4 const refs to 3 string values, locally named 'dirName', 'fileName', & 'newLineContent', & 1 size_t value, locally named 'lineTarget'
//  function calls getSystemFile() & saveFile()
//  
bool updateFileLine( const std::string& fileName, const size_t& lineTarget, const std::string& newLineContent ){

    bool verifyUpdateFile{false};

    auto [ gotFile, fileLines ] = getSystemFile( fileName );

    const std::string dirName{ "../sys/" };

    if ( gotFile == true ){
        std::vector<std::string> newFileLines{};
        if ( lineTarget < fileLines.size() && !fileLines[lineTarget].empty() ){
            for ( size_t i = 0; i < fileLines.size(); i++ ){
                if ( i == lineTarget ){
                    newFileLines.push_back(newLineContent);
                }
                else {
                    newFileLines.push_back(fileLines[i]);
                }
            }
        }
        else if ( lineTarget == fileLines.size() && fileLines.size() < 5 ){
            for ( size_t i = 0; i < fileLines.size(); i++ ){
                newFileLines.push_back(fileLines[i]);
            }
            newFileLines.push_back(newLineContent);
        }
        if ( saveFile( dirName, fileName, newFileLines ) == true ){
            verifyUpdateFile = true;
        }
    }
    else {
        verifyUpdateFile = false;
    }

    return verifyUpdateFile;

}

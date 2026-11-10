#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <tuple>
#include <system_error>


void printBorderLine();

//  declaration for called & caller function encryptPin()
//  function is type int, returns int value to caller function
//  function receives 1 const reference for int value, locally named 'pin'
//  function performs all the steps for encrypting 4-digit int value, call this function when encrypting int values
//  
int encryptPin( const int& pin );

//  declaration for called & caller function decryptPin()
//  function is type int, returns int value to caller function
//  function receives 1 const reference for int value, locally named 'pin'
//  function performs all steps for decrypting 4-digit int value, call this function when decrypting int values
//  
int decryptPin( const int& pin );

bool confirmTargetLine( const std::string& fileName, const size_t& lineTarget, const std::string& searchTarget ){

    bool verifyTargetLine{false};
    std::string line{};
    size_t lineNum{0};

    std::filesystem::path filePath{ "sys/" + fileName };
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

std::tuple<bool, size_t, size_t> searchSystemFile( const std::string& fileName, const std::string& searchTarget ){

    bool foundSearchTarget{false};
    std::string line{};
    size_t lineTotal{0};
    size_t lineTarget{}; 

    std::filesystem::path filePath{ "sys/" + fileName };
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

bool saveFile( const std::string& dirName, const std::string& fileName, const std::vector<std::string>& fileLines ){

    bool verifyFileSave{false};

    std::filesystem::path filePath{ dirName + fileName };

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

std::pair<bool, std::vector<std::string>> getSystemFile( const std::string& dirName, const std::string& fileName ){

    bool gotSystemFile{false};

    std::filesystem::path filePath{ dirName + fileName };

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

std::pair<bool, std::string> getFileLine( const std::string& dirName, const std::string& fileName, const size_t& lineNum ){

    bool gotFileLine{false};

    std::string lineContent{};

    auto [ gotFile, fileLines ] = getSystemFile( dirName, fileName );

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

void errorRefUser( const size_t& userLine ){

    printBorderLine();
    std::cout << "Error! User profile unable to be referenced properly from line " << userLine << " of file ' sys/users.txt '. " << std::endl;

}

void errorSearchFile( const std::string& dirName, const std::string& fileName ){

    printBorderLine();
    std::cout << "File ' " << dirName << "/" << fileName << " ' not found. " << std::endl;

}

bool createDirectory( const std::string& dirName ){

    bool verifyCreateDir{false};

    std::error_code err{};

    if ( !std::filesystem::create_directory( dirName, err )){
        if ( std::filesystem::exists( dirName )){
            verifyCreateDir = false;
        }
        else if ( err ) {
            verifyCreateDir = false;
        }
    }
    else {
        verifyCreateDir = true;    
    }

    return verifyCreateDir;

}

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



bool deleteFileLine( const std::string& dirName, const std::string& fileName, const size_t& lineTarget ){

    bool verifyLineDelete{false};

    auto [ gotFile, fileLines ] = getSystemFile( dirName, fileName );

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

bool updateFileLine( const std::string& dirName, const std::string& fileName, const size_t& lineTarget, const std::string& newLineContent ){

    bool verifyUpdateFile{false};

    auto [ gotFile, fileLines ] = getSystemFile( dirName, fileName );

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

bool initSystemFiles(){

    bool verifySystemFiles{false};

    int checkSystemFiles{0};

    //  function creates local directory 'sys/' if doesn't already exist 
    //  1
    if ( !createDirectory( "sys" ) ){
        if ( !std::filesystem::exists( "sys" ) ){
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
    //  2
    std::ofstream createFileUsers{ "sys/users.txt", std::ios::app };
    if ( createFileUsers.is_open() ){
        createFileUsers.close();
        checkSystemFiles++;
    }
    else {
        checkSystemFiles = 0;
    }

    //  function creates file 'user-pins.txt' in directory 'sys/' if doesn't already exist
    //  3
    std::ofstream createFileUserPins{ "sys/user-pins.txt", std::ios::app };
    if ( createFileUserPins.is_open() ){
        createFileUserPins.close();
        checkSystemFiles++;
    }
    else {
        checkSystemFiles = 0;
    }

    if ( checkSystemFiles == 3 ){
        verifySystemFiles = true;
        checkSystemFiles = 0;
    }
    else {
        verifySystemFiles = false;
        checkSystemFiles = 0;
    }

    return verifySystemFiles;

}

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>

void clearScreen();

void printBorderLine();

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

std::pair<bool, std::vector<std::string>> getUserFile( const std::string& userName, const std::string& fileName, const int& pin ){

    std::filesystem::path filePath{ userName + "/" + fileName };
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

        std::vector<std::string> fileWordsDecrypt{ encryptDecryptUserFile( fileWordsEncrypt, std::to_string(pin) ) };

        //  std::vector<std::string> fileLinesDecrypt{};

        size_t beginWordNum{0};

        for ( size_t i = 0; i < fileLineWordCount.size(); i++ ){
            std::string fileLine{};
            for ( size_t x = 0; x < fileLineWordCount[i]; x++ ){
                if ( x == 0 ){
                    fileLine = fileWordsDecrypt[ beginWordNum ] + " ";
                }
                else {
                    fileLine = fileLine + fileWordsDecrypt[ beginWordNum ] + " ";
                }
                beginWordNum++;
            }
            fileLinesDecrypt.push_back(fileLine);
            fileLine.clear();
        }

        gotFileContent = true;

    }
    else {
        gotFileContent = false;
    }
    

    return { gotFileContent, fileLinesDecrypt };

}

bool saveUserFile( const std::string& userName, const std::string& fileName, const std::vector<std::string>& fileLines ){

    bool saveUserNote{false};

    std::filesystem::path filePath{ userName + "/" + fileName };

    std::ofstream updateFile{ filePath };

    if ( updateFile.is_open() ){
        for ( size_t i = 0; i < fileLines.size(); i++ ){
            updateFile << fileLines[i] + "\n";
        }
        updateFile.close();
        saveUserNote = true;
    }
    else{
        saveUserNote = false;
    }

    return saveUserNote;

}

bool deleteUserFile( const std::string& userName, const std::string& fileName ){

    bool verifyFileDelete{false};

    std::filesystem::path filePath( userName + "/" + fileName );

    if ( !std::filesystem::remove(filePath) ){
        verifyFileDelete = false;
    }
    else {
        verifyFileDelete = true;
    }

    return verifyFileDelete;

}

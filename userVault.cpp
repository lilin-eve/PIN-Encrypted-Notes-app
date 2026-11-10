#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <sstream>

//  global bool value 'sessionOver'
//  'sessionOver' set to false by default
//  when 'sessionOver' is false, userVault() loops, when 'sessionOver' is true, userVault() exits loop
bool sessionOver = false;

//  declaration for called function clearScreen()
//  function is type void, returns no value to caller function
//
void clearScreen();

//  declaration for called function printBorderLine()
//  function is type void, returns no value to caller function
//  
void printBorderLine();

//  declaration for called function confirmPin()
//  function is type bool, returns true or false value to caller function
//  function receives 1 int value & 1 size_t value from caller function
//  
bool confirmPin( const int& pin, const size_t& lineTarget );

//  definition for called & caller function deleteUserNote()
//  function is type void, returns no value to caller function
//  function receives 2 string values, locally named 'userName' & 'fileName'
//  
bool deleteUserFile( const std::string& userName, const std::string& fileName );

void errorRefUser( const size_t& userLine );

void errorSearchFile( const std::string& dirName, const std::string& fileName );


//  definition for called function encryptDecryptFile()
//  function is type string vector, returns a vector of string values to caller function
//  function receives 1 string vector & 1 string from caller function
//  function must receive individual file WORDS not file LINES, words individually broken up & encrypted w/ pin-number
//  
std::vector<std::string> encryptDecryptUserFile( const std::vector<std::string>& fileWords, const std::string& pinString );

//  definition for called function getFileContent()
//  function is type string vector, returns a vector of string values to caller function
//  function receives 2 string values, locally named 'userName' & 'fileName', & 1 int value, locally named 'pin'
//  
std::pair<bool, std::vector<std::string>> getUserFile( const std::string& userName, const std::string& fileName, const int& pin );

std::pair<bool, std::string> getFileLine( const std::string& dirName, const std::string& fileName, const size_t& lineNum );

//  definition for called function getPin()
//  function is type pair, returns 1 bool value & 1 int value to caller function
//  function receives 1 size_t value from caller function, locally named 'userLine'
//  
std::pair<bool, int> getUserPin( const size_t& userLine );

//  definition for called function saveUserFile()
//  function is type bool, returns true or false value to caller function
//  function receives 2 string values, locally named 'userName' & 'fileName', & 1 string vector, locally named 'fileLines'
//  
bool saveUserFile( const std::string& userName, const std::string& fileName, const std::vector<std::string>& fileLines );

//  declaration for called function userSettings()
//  function is type bool, returns no value to caller function
//  function receives 1 string value from caller function
//  
void userSettings( const size_t& userLine );



//  definition for called function getNoteName()
//  function is type string, returns string value to caller function
//  
std::string getNoteName(){

    std::string fileName{};

    printBorderLine();
    std::cout << "[ excluding file extension e.g. '.txt' ] | Enter note name: ";
    std::cin >> fileName;

    fileName = fileName + ".txt";

    return fileName;

}

//  definition for called function typeUserNote()
//  function is type pair, returns 2 string vectors to caller function
//  function receives 1 int value from caller function
//  
std::pair<std::vector<std::string>, std::vector<std::string>> typeUserNote( const int& pin ){

    //  int pin{ getPin( userLine ) }; 

    std::string fileInput{};
    std::vector<std::string> fileLines{};
    std::vector<size_t> fileLineWordCount{};
    //  int fileLineCount{0};
    //  clearScreen();
    printBorderLine();
    std::cout << " | Type your note below. | Type 'EXIT' followed by new-line(ENTER key) on blank line to finish. | " << std::endl;
    printBorderLine();
    while (true){
        getline( std::cin, fileInput );
        if ( fileInput == "EXIT" ){
            break;
        }
        fileLines.push_back( fileInput );
    }
    std::vector<std::string> fileWords{};
    for ( size_t i = 0; i < fileLines.size(); i++ ){
        //  fileLineCount++;
        size_t lineWordCount{0};
        std::istringstream stream(fileLines[i]);
        while ( stream >> fileInput ){
            lineWordCount++;
            fileWords.push_back( fileInput );
        }
        fileLineWordCount.push_back( lineWordCount );
        lineWordCount = 0;
    }

    std::vector<std::string> fileWordsEncrypt{ encryptDecryptUserFile( fileWords, std::to_string(pin) ) };

    std::vector<std::string> fileLinesEncrypt{};

    size_t beginWordNum{ 0 };

    for ( size_t i = 0; i < fileLines.size(); i++ ){
        std::string fileLineEncrypt{};
        if ( i != 0 ){
            for ( size_t x = 0; x < fileLineWordCount[i]; x++ ){
                if ( x == 0 ){
                    fileLineEncrypt = fileWordsEncrypt[ beginWordNum ] + " ";
                }
                else {
                    fileLineEncrypt = fileLineEncrypt + fileWordsEncrypt[ beginWordNum ] + " ";
                }
                beginWordNum++;
            }
            fileLinesEncrypt.push_back(fileLineEncrypt);
            fileLineEncrypt.clear();
        }
    }

    std::vector<std::string> fileWordsDecrypt{ encryptDecryptUserFile( fileWordsEncrypt, std::to_string(pin) ) };

    std::vector<std::string> fileLinesDecrypt{};

    beginWordNum = 0;

    for ( size_t i = 0; i < fileLines.size(); i++ ){
        std::string fileLineDecrypt{};
        if ( i != 0 ){
            for ( size_t x = 0; x < fileLineWordCount[i]; x++ ){
                if ( x == 0 ){
                    fileLineDecrypt = fileWordsDecrypt[ beginWordNum ] + " ";
                }
                else {
                    fileLineDecrypt = fileLineDecrypt + fileWordsDecrypt[ beginWordNum ] + " ";
                }
                beginWordNum++;
            }
            fileLinesDecrypt.push_back(fileLineDecrypt);
            fileLineDecrypt.clear();
        }
    }

    //  fileLinesDecrypt[0].erase();

    //  fileLinesDecrypt[0].clear();

    //  fileLinesEncrypt[0].erase();

    //  fileLinesEncrypt[0].clear();
    //  clearScreen();

    return { fileLinesEncrypt, fileLinesDecrypt };

}

//  definition for called & caller function editUserNote()
//  function is type void, returns no value to caller funciton
//  function receives 2 string values, locally named 'userName' & 'fileName', 1 int value, locally named 'pin', & 1 string vector, locally named 'fileLines'
//  
void editUserNote( const std::string& userName, const std::string& fileName, const int& pin, const std::vector<std::string>& fileLines ){

    //  std::filesystem::path filePath{ userName + "/" + fileName };

    int choice{};

    std::vector<std::string> fileLinesNew{};
    clearScreen();
    do {
        printBorderLine();
        std::cout << "File ' " << userName << "/" << fileName << " ' currently reads: " << std::endl;
        printBorderLine();
        for ( size_t i = 0; i < fileLines.size(); i++ ){
            std::cout << "Line [ " << i + 1 << " ] | " << fileLines[i] << std::endl;
        }
        auto [ fileLinesEncrypt, fileLinesDecrypt ] = typeUserNote(pin);
        fileLinesNew = fileLinesEncrypt;
        clearScreen();
        printBorderLine();
        std::cout << "File ' " << userName << "/" << fileName << " ' previously read: " << std::endl;
        printBorderLine();
        for ( size_t i = 0; i < fileLines.size(); i++ ){
            std::cout << "Line [ " << i + 1 << " ] | " << fileLines[i] << std::endl;
        }
        printBorderLine();
        std::cout << "File ' " << userName << "/" << fileName << " ' now reads: " << std::endl;
        printBorderLine();
        for ( size_t i = 0; i < fileLinesDecrypt.size(); i++ ){
            std::cout << "Line [ " << i + 1 << " ] | " << fileLinesDecrypt[i] << std::endl;
        }
        printBorderLine();
        std::cout << "Does this look correct?" << std::endl;
        std::cout << "[1] Yes." << std::endl;
        std::cout << "[2] No." << std::endl;
        std::cout << "[3] Discard & go back." << std::endl;
        printBorderLine();
        std::cout << ">";
        std::cin >> choice;
        if ( choice != 1 && choice != 2 && choice != 3 ){
            clearScreen();
            printBorderLine();
            std::cout << "Invalid input! Input must be '1', '2', or '3'." << std::endl;
        }
    } while ( choice != 1 && choice != 3 );
       if ( choice == 1 ){
        if ( saveUserFile( userName, fileName, fileLinesNew ) == true ){
                    clearScreen();
                    printBorderLine();
                    std::cout << "Success! Content written to file ' " << userName << "/" << fileName << " '." << std::endl;
                    return;
                }
                else {
                    clearScreen();
                    printBorderLine();
                    std::cout << "Error! Content unable to be written to file ' " << userName << "/" << fileName << " '." << std::endl;
                    return;
                }
    }
    else {
        clearScreen();
        return;
    }
    
}

//  definition for called & caller function readUserNote()
//  function is type void, returns no value to caller function
//  function receives 2 string values, locally named 'userName' & 'fileName', 1 int value, locally named 'pin', & 1 string vector, locally named 'fileLines'
//  
void readUserNote( const std::string& userName, const std::string& fileName, const int& pin, const std::vector<std::string>& fileLines ){

    //  std::vector<std::string> fileLines{ getFileContent( userName, fileName, pin )};

    int choice{};

    clearScreen();
    do {
        printBorderLine();
        std::cout << "File ' " << userName << "/" << fileName << " ' reads: " << std::endl;
        printBorderLine();
        for ( size_t i = 0; i < fileLines.size(); i++ ){
            std::cout << "Line [ " << i + 1 << " ] | " << fileLines[i] << std::endl;
        }
        printBorderLine();
        std::cout << "Would you like to: " << std::endl;
        std::cout << "[1] Edit file." << std::endl;
        std::cout << "[2] Go back to " << userName << "'s vault." << std::endl;
        printBorderLine();
        std::cout << ">";
        std::cin >> choice;
        if ( choice != 1 && choice != 2 ){
            clearScreen();
            printBorderLine();
            std::cout << "Invalid input! Input must be '1' or '2'." << std::endl;
        }
    } while ( choice != 1 && choice != 2 );

    if ( choice == 1 ){
        editUserNote( userName, fileName, pin, fileLines );
        return;
    }
    else if ( choice == 2 ){
        clearScreen();
        return;
    }
    

}

//  definition for called & caller function createUserNote()
//  function is type void, returns no value to caller function
//  function receives 2 string values, locally named 'userName' & 'fileName', & 1 int value, locally named 'pin'
//  
void createUserNote( const std::string& userName, const std::string& fileName, const int& pin ){

        clearScreen();
        int choice{};
        std::vector<std::string> fileLines{};
        do {
            auto [ fileLinesEncrypt, fileLinesDecrypt ] =  typeUserNote( pin );
            fileLines = fileLinesEncrypt;
            //  int pin{ getPin( userLine ) };
            //  fileWordsEncrypt = encryptUserNote( fileWords, std::to_string(pin) );
            //  std::vector<std::string> fileWordsDecrypt{ encryptUserNote( fileWords, std::to_string(pin) ) }; 
            clearScreen();
            printBorderLine();
            std::cout << "File ' " << userName << "/" << fileName << " ' reads: " << std::endl;
            printBorderLine();
            for ( size_t i = 0; i < fileLinesDecrypt.size(); i++ ){
                //  std::cout << fileWords[i] << std::endl;
                std::cout << "Line [ " << i + 1 << " ] | " << fileLinesDecrypt[i] << std::endl;
            }
            printBorderLine();
            std::cout << "Does this look correct?" << std::endl;
            std::cout << "[1] Yes." << std::endl;
            std::cout << "[2] No." << std::endl;
            std::cout << "[3] Discard & go back." << std::endl;
            printBorderLine();
            std::cout << ">";
            std::cin >> choice;
            if ( choice != 1 && choice != 2 && choice != 3 ){
                clearScreen();
                printBorderLine();
                std::cout << "Invalid input! Input must be '1', '2', or '3'." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3));
            }
        } while ( choice != 1 && choice != 3 );
        if ( choice == 1 ){
            if ( saveUserFile( userName, fileName, fileLines ) == true ){
                clearScreen();
                printBorderLine();
                std::cout << "Success! Content written to file ' " << userName << "/" << fileName << " '." << std::endl;
                return;
            }
            else {
                clearScreen();
                printBorderLine();
                std::cout << "Error! Content unable to be written to file ' " << userName << "/" << fileName << " '." << std::endl;
                return;
            }
        }
        else if ( choice == 3 ){
            clearScreen();
            return;
        }  


}

//  definition for called & caller function userVault()
//  function is type void, returns no value to caller function
//  function receives 1 string value, locally named 'userName', & 1 size_t value, locally named 'userLine'
//  
void userVault( const size_t& userLine ){

    sessionOver = false;

    clearScreen();
    while (!sessionOver){
        auto [ gotFileLine, lineContent ] = getFileLine("sys/", "users.txt", userLine );
        if ( gotFileLine == true ){
            std::string userName{ lineContent };

            int choice{};
            do {
                printBorderLine();
                std::cout << "Hello, " << userName << ". :)" << std::endl;
                std::cout << "Pick menu option: " << std::endl;
                printBorderLine();
                std::cout << "[1] Write a new note." << std::endl;
                std::cout << "[2] Read an existing note." << std::endl;
                std::cout << "[3] Edit an existing note." << std::endl;
                std::cout << "[4] Delete an existing note." << std::endl;
                std::cout << "[5] Open user settings." << std::endl;
                std::cout << "[6] Exit vault." << std::endl;
                printBorderLine();
                std::cout << ">";
                std::cin >> choice;
                if ( choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6 ){
                    clearScreen();
                    printBorderLine();
                    std::cout << "Invalid input! Input must be '1', '2', '3', '4', '5', or '6'." << std::endl;
                } 
            } while ( choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6 );

            int choice_1{};
            clearScreen();
            if ( choice < 5 ){
                std::string fileName{ getNoteName() };
                auto [ verifyPin, pin ] = getUserPin( userLine );
                if ( verifyPin == true ){
                    auto [ gotFileContent, fileLines ] = getUserFile( userName, fileName, pin );
                    if ( choice == 1 ){
                        if ( !gotFileContent ){
                            createUserNote( userName, fileName, pin );
                        }
                        else {
                            clearScreen();
                            do {
                                printBorderLine();
                                std::cout << "File ' " << userName << "/" << fileName << " ' already exists." << std::endl;
                                std::cout << "Would you like to: " << std::endl;
                                std::cout << "[1] Edit existing file." << std::endl;
                                std::cout << "[2] Go back." << std::endl;
                                printBorderLine();
                                std::cout << ">";
                                std::cin >> choice_1;
                                if ( choice_1 != 1 && choice_1 != 2 ){
                                    clearScreen();
                                    printBorderLine();
                                    std::cout << "Invalid input! Input must be '1' or '2'." << std::endl;
                                }
                            } while ( choice_1 != 1 && choice_1 != 2 );
                            if ( choice_1 == 1 ){
                                editUserNote( userName, fileName, pin, fileLines );
                            }
                            else if ( choice_1 == 2 ) {
                                clearScreen();
                            }
                        }
                    }
                    else if ( choice == 2 ) {
                        if ( gotFileContent == true ){
                            readUserNote( userName, fileName, pin, fileLines );
                        }
                        else {
                            errorSearchFile( userName, fileName );
                        }
                    }
                    else if ( choice == 3 ){
                        if ( gotFileContent == true ){
                            editUserNote( userName, fileName, pin, fileLines );
                        }
                        else {
                            errorSearchFile( userName, fileName );
                        }
                    }
                    else if ( choice == 4 ){
                        if ( gotFileContent == true ){
                            do {
                                printBorderLine();
                                std::cout << "Are you sure you want to delete file ' " << userName << "/" << fileName << " ' ? " << std::endl;
                                std::cout << "[1] Yes. " << std::endl;
                                std::cout << "[2] No. " << std::endl;
                                printBorderLine();
                                std::cout << ">";
                                std::cin >> choice_1;
                                if ( choice_1 != 1 && choice_1 != 2 ){
                                    clearScreen();
                                    printBorderLine();
                                    std::cout << "Invalid input! Input must be '1' or '2'." << std::endl;
                                }
                            } while ( choice_1 != 1 && choice_1 != 2 );
                            clearScreen();
                            if ( choice_1 == 1 ){
                                if ( deleteUserFile( userName, fileName ) == true ){
                                    printBorderLine();
                                    std::cout << "File ' " << userName << "/" << fileName << " ' successfully deleted." << std::endl;
                                }
                                else {
                                    printBorderLine();
                                    std::cout << "Error! File ' " << userName << "/" << fileName << " ' unable to be deleted." << std::endl;
                                }
                            }
                        }
                        else {
                            errorSearchFile( userName, fileName );
                        }
                    
                    }
                }
            
            }
            else if ( choice == 5 ){
                userSettings( userLine );
            }
            else if ( choice == 6 ){
                sessionOver = true;
            }

        }
        else {
            errorRefUser( userLine );
            std::this_thread::sleep_for(std::chrono::seconds(3));
            sessionOver = true;
        }

    }

    return;
    
}

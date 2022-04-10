/* XE LINK EDITOR
* Authors: Kyle Krueger & Brett Gallagher
* Created at San Diego State University for CS530 System Software Spring 2022
*
* Description:
* The program will open assembler listing file(s) from the command line and perform
* the required steps to translate the listing file into an XE executable object file.
*
* --- GENERAL TASKS FOR PROJECT ---
* TODO: Clearly define work needed
* TODO: Divide work up, or develop a strategy to complete assignment
*
* --- TASKS FOR FILE HANDLING ---
* TODO: Create a way to test for valid input files (Check for args(DONE), MemoryCheck)
* TODO: Create a way to accept multiple input files (can we create a method to input files and process one file at a time?)
* TODO:
*
* --- TASKS FOR LINKER RELATED FUNCTIONS ---
* TODO: Generate ESTAB
* TODO: Memory Check
* TODO: Program Pass 1
* TODO: Program Pass 2
* TODO:
*
* --- ESTAB FORMAT ---
* CTRLSC||SYMNAM||ADDRES||LENGTH
* Control Section should be only declared once at the start, and then left null for the symbols declared underneath
* Symname should be added when the symbol is defined with EXTDEF directives
* ADDRESS will be the defined with the symbol, and for the control sections, it will be calculated
* Length will be only used for control sections
*
* ADDRESS = Control Section Starting Address + Local Address in control sections
*
* How to handle the ESTAB? I'm not so sure
*
*
*
*/

//FOR ANY DEBUGING FUNCTIONS, surround it in an if statement and change this to true
bool DEBUG = true;
#include "ESTAB.h"
#include "ObjectFile.h"
#include <string>
#include <cstring>



int main(int argc, char **argv) {
//Check for arguments
    if (argc == 0){
        std::cout<<"PROGRAM TERMINATED: ARGUMENTS NOT FOUND\n";
        return 0;
    }
    int startingProgAddr = 0;
    ESTAB mainESTAB(startingProgAddr);
    for (int i = 1; i < argc; i++){

        //Process the files one at a time, passing in the similar
        //std::vector<char> mainFileContents = readFile(argv[i]);
        std::string line;
        std::ifstream readingFile{argv[i]};
        string memLocation;
        string symContents;
        string directiveContents;
        string argumentContents;
        char argumentMod;
        string fileName = argv[i]; //Generate the obj file name;
        fileName = fileName.erase(fileName.size()-3,3) + "obj";
        string opCode;
        ObjectFile objFile;
        //string variableNames[5] = {"memLocation","symContents","directiveContents","argumentContents","opCode"};
        int columnCount = 0; // 0 = Memory col; 1 = Symbol col; 2 = Op col; 3 = Argument col; 4 = OpCode col;
        int strtAdd;
        int endAdd;
        //Our Pointer like variable that will be used to access
        int scanInt = 0;
        bool exitLine, commentSeen = false,extendedFormat = false;

        while(std::getline(readingFile, line)) {
        //GETTING THE LINE CONTENTS
        //Create code to extract string of the file name

            for (int i = 0; i < line.length();i++){
                if (line[i] == '.'){ // check for comments
                    commentSeen = true;
                }
                if(commentSeen == true){
                    //continue;
                }
                else {
                    if (i >= 0 && i <= 3) {// Fetching the memory address
                        memLocation+=line[i];
                    }
                    if (i >= 8 && i <= 13) {// Fetching the Symbol name
                        if (line[i] == ' ') {
                            //continue;       //go to top of for loop to ignore spaces
                        } else {
                            symContents+=line[i];
                        }
                    }
                    if (i == 16) {
                        if (line[i] == ' ') {
                            //continue;
                        } else if (line[i] == '+') {
                            extendedFormat = true;
                        } else if (line[i] == '=') {
                            //Check to see if the characters are end of file. AWAITING PROFESSOR RESPONSE TO EMAIL

                        }
                    }
                    if (i >= 17 && i <= 22) {//Fetching the Directive
                        directiveContents+=line[i];
                    }
                    if (i == 25) {//Fetching Argument Modifier
                        argumentMod = line[i];

                    }
                    if (i >= 26 && i <= 50) {//Fetching the arguments
                        if (line[i] == ' ') {
                           // continue;
                        }
                        else{
                            argumentContents+=line[i];
                        }
                    }
                    if (i >= 51 && i <= 58) {//Fetching the opCodef
                        if (line[i] == ' ') {
                            continue;
                        }
                        else {
                            opCode+=line[i];
                        }
                    }
                }
            }

            //Processing the Line
            //Generating Modification Records:
            /*
             * Any time a symbol is encountered:
             * Check if it exists on the external reference list
             *      if it is, generate a modification record that takes the memory address,
             */
            //
            //ASSEMBLER DIRECTIVES: START, END, BYTE, WORD, RESB, RESW
            if(directiveContents == "EXTREF"){
                objFile.generateReferString(argumentContents);

            }
            std::cout << line << '\n';
            std::cout <<endl<<"REVISED FILE NAME: "<<fileName<<endl;
            std::cout<< "MEM: "<<memLocation<<endl<<"SYM: "<<symContents<<endl<<"EXT: "<<extendedFormat<<endl<<"DIR: "<<directiveContents<<endl<<"AMD: "<<argumentMod<<endl<<"ARG: "<<argumentContents<<endl<<"OPC: "<<opCode<<endl<<endl;
            //Resetting Line contents after Processing
            memLocation = "";
            fileName = "";
            commentSeen = false;
            extendedFormat = false;
            symContents = "";
            directiveContents = "";
            argumentContents = "";
            opCode = "";
            argumentMod = ' ';



        }
        strtAdd = endAdd;

    }
}

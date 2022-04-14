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
#include <stdlib.h>
#include <cstring>
#include <map>


int main(int argc, char **argv) {
//Check for arguments
    if (argc == 0) {
        std::cout << "PROGRAM TERMINATED: ARGUMENTS NOT FOUND\n";
        return 0;
    }
    int startingProgAddr = 0;
    int strtAdd;
    int endAdd = 0;
    string lastAdd;
    ESTAB mainESTAB(startingProgAddr);
    for (int i = 1; i < argc; i++) {

        //Process the files one at a time, passing in the similar
        //std::vector<char> mainFileContents = readFile(argv[i]);
        if (i == 1) {
            strtAdd = startingProgAddr;
        } else {
            strtAdd = endAdd;
        }
        std::string line;
        std::ifstream readingFile{argv[i]};
        string memLocation;
        string ctrlName;
        string symContents;
        string testes;
        string directiveContents;
        string argumentContents;
        TextRecord mainText;
        ObjectFile mainOBJ;
        char argumentMod;
        string fileName = argv[i]; //Generate the obj file name;
        fileName = fileName.erase(fileName.size() - 3, 3) + "obj";
        string opCode;
        ObjectFile objFile;
        bool commentSeen = false, extendedFormat = false;

        mainText.ctrlLoadAdd = strtAdd;

        while (std::getline(readingFile, line)) {
            //GETTING THE LINE CONTENTS
            //Create code to extract string of the file name

            for (int j = 0; j < line.length(); j++) {
                if (line[j] == '.') { // check for comments
                    commentSeen = true;
                }
                if (commentSeen == true) {
                    //continue;
                } else {
                    if (j >= 0 && j <= 3) {// Fetching the memory address
                        if (line[j] <=32){
                            //continue;
                        }
                        else {
                            memLocation += line[j];
                        }
                    }
                    if (j >= 8 && j <= 13) {// Fetching the Symbol name
                        if (line[j] == ' ') {
                            //continue;       //go to top of for loop to ignore spaces
                        } else {
                            symContents += line[j];
                        }
                    }
                    if (j == 16) {
                        if (line[j] == ' ') {
                            //continue;
                        } else if (line[j] == '+') {
                            extendedFormat = true;
                        } else if (line[j] == '=') {
                            //Check to see if the characters are end of file. AWAITING PROFESSOR RESPONSE TO EMAIL

                        }
                    }
                    if (j >= 17 && j <= 22) {//Fetching the Directive
                        if (line[j] == ' ') {
                            //continue;       //go to top of for loop to ignore spaces
                        } else {
                            directiveContents += line[j];
                        }
                    }
                    if (j == 25) {//Fetching Argument Modifier
                        argumentMod = line[j];

                    }
                    if (j >= 26 && j <= 50) {//Fetching the arguments
                        if (line[j] == ' ') {
                            // continue;
                        } else {
                            argumentContents += line[j];
                        }
                    }
                    if (j >= 51 && j <= 58) {//Fetching the opCodef
                        if (line[j] == ' ') {
                            //continue;
                        } else {
                            opCode += line[j];
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

            if (directiveContents == "EXTREF") { // Handling the Assembler Directive EXTREF
                testes = objFile.generateReferString(argumentContents);

            }
            else if (directiveContents == "EXTDEF") {
                //Look at the arguments
                unsigned short int lBound = 0, boundDiff = 0;
                for (int k = 0; k <= argumentContents.length(); k++) {
                    if (argumentContents[k] == ',' || k == argumentContents.length()) {
                        boundDiff = k - lBound;
                        string subArg = "";
                        subArg = argumentContents.substr(lBound, boundDiff);
                        if (subArg.at(subArg.length() - 1) <= 32) {
                            subArg.erase(subArg.length() - 1, 1);
                        }
                        //Add a new row to this ctrl section's ESTAB w/o mem location
                        mainESTAB.addNewESTABRow();
                        mainESTAB.ESTABtest.at(mainESTAB.ESTABrows - 1).isHeader = false;
                        mainESTAB.ESTABtest.at(mainESTAB.ESTABrows - 1).symName = subArg;
                        mainESTAB.ESTABtest.at(mainESTAB.ESTABrows - 1).ctrlSection = i;
                        lBound = k + 1;
                    }

                }
                //Seperate the arguments (commas will seperate them, as well as the final space)
                //Add arguments to a Symbol Map
                //Generate new entries into the ESTAB
            }
            else if (directiveContents == "START") {
                //Initialize a new control section row in ESTAB and include everything but length
                mainESTAB.addNewESTABRow();
                mainESTAB.ESTABtest.at(mainESTAB.ESTABrows - 1).isHeader = true;
                mainESTAB.ESTABtest.at(mainESTAB.ESTABrows - 1).ctrlSection = i;
                mainESTAB.ESTABtest.at(mainESTAB.ESTABrows - 1).symName = symContents;
                ctrlName = symContents;
                mainESTAB.ESTABtest.at(mainESTAB.ESTABrows - 1).Address = strtAdd;
            }
            //EOF processing, generate End record
           else if (directiveContents == "C'EOF'") {
                mainESTAB.endControlSection(i, memLocation);
                endAdd = strtol(&memLocation[0], nullptr,16);
               // cout<<endl<<"END ADD: "<<endAdd<<endl;
            }
            else if (directiveContents == "RESW"){
                mainText.generateTextRecord();

            }
            else if (directiveContents == "BASE"){

            }

            else if (opCode != ""&&directiveContents!="C'EOF'") {

                mainText.addTextRecordInstruction(opCode,memLocation,strtAdd);
                if (mainText.recordStartingAdd == "") {
                    if (memLocation != "") {
                        mainText.recordStartingAdd = strtol(&memLocation[0], nullptr, 10) + strtAdd;
                    }
                }

            }
            else{
                if(mainText.getTextRecordLength()!=0){
                    mainText.generateTextRecord();

                }
            }
            if(symContents!=""){
                for (int k=0;k<mainESTAB.ESTABrows-1;k++){
                    if (symContents == mainESTAB.ESTABtest.at(k).symName){
                        mainESTAB.ESTABtest.at(k).Address = stoi(memLocation);

                    }
                }
            }
            //std::cout << line << endl;
            //std::cout <<endl<<"REVISED FILE NAME: "<<fileName<<endl;
            // std::cout<< "MEM: "<<memLocation<<endl<<"SYM: "<<symContents<<endl<<"EXT: "<<extendedFormat<<endl<<"DIR: "<<directiveContents<<endl<<"AMD: "<<argumentMod<<endl<<"ARG: "<<argumentContents<<endl<<"OPC: "<<opCode<<endl<<endl;

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

        //Header Record
        int ctrlSectLine = 0;
        for(int k=0;k<mainESTAB.ESTABrows;k++){
            if(mainESTAB.ESTABtest.at(k).isHeader == true){
                if(mainESTAB.ESTABtest.at(k).ctrlSection == i){
                    ctrlSectLine = k;
                }
            }
        }
        cout <<std::hex<<objFile.generateHeaderString(ctrlName, to_string(mainESTAB.ESTABtest.at(ctrlSectLine).Address),
                                                      to_string(mainESTAB.ESTABtest.at(ctrlSectLine).Length))<<endl;
        //Define Record
        cout<<"D";
        for (int i = ctrlSectLine;i<mainESTAB.ESTABrows;i++){
            if (mainESTAB.ESTABtest.at(i).isHeader == false && mainESTAB.ESTABtest.at(i).ctrlSection == mainESTAB.ESTABtest.at(ctrlSectLine).ctrlSection){
                cout<<objFile.generateDefineString(mainESTAB.ESTABtest.at(i).symName, to_string(mainESTAB.ESTABtest.at(i).Address));
           }
        }
        //Refer Record
        cout<<endl<<testes;
        //Text Record
        cout<<endl<<mainText.finalTextRecord;
        //End Record
        cout<<objFile.generateEndString(mainESTAB.ESTABtest.at(ctrlSectLine).Address);
        //My depression
        //oWo
        //


    }
    mainESTAB.createESTABfile();
}

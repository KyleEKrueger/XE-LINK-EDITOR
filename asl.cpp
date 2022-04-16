/* XE LINK EDITOR
* Kyle Krueger and Brett Gallagher
    cssc0413, cssc0423
    CS530, Spring 2022
    Assignment 2
    asl.cpp
*
* Description:
* The program will open assembler listing file(s) from the command line and perform
* the required steps to translate the listing file into an XE executable object file.
*/
#include "ESTAB.h"
#include "ObjectFile.h"
#include <string>
#include <stdlib.h>
//Takes in parsed information in order to generate modification records for a given line if necessary
string processArgument(ModificationRecord mainMod,vector<string> refSymbols,string memLocation, string argumentContents, bool extendedFormat,string ctrlSectName){
    bool isAddition = true;
    //Check argument for refrence symbols
    unsigned short int lBound,rBound;
    string tempString;
    lBound = 0;
    for (int i=0;i<argumentContents.length();i++){
        while(argumentContents[i]>=48 && argumentContents[i]<=122){ //Checks to see if argument contents is a number or letter
            tempString+=argumentContents[i];
            i++;
        }
        if (tempString!=""){
            for(int j=0; j<refSymbols.size();j++){
                if(tempString == refSymbols[j]){ // check to see if the argument is a referenced symbol, if so generate a modification record
                    return mainMod.generateModificationRecord(memLocation,refSymbols[j],extendedFormat,isAddition);
                }
            }
        }
    }

    //If contains ref symbols, then delete the underscores
    if (extendedFormat){
        return mainMod.generateModificationRecord(memLocation,ctrlSectName,extendedFormat,true);
        //Build a mod record
    }
    return "";
}

int main(int argc, char **argv) {
//Check for arguments
    if (argc == 0) {
        std::cout << "PROGRAM TERMINATED: ARGUMENTS NOT FOUND\n";
        return 0;
    }
    //Declarations for variables used in multiple and/or in between files
    int startingProgAddr = 0;
    int strtAdd;
    int endAdd = 0;
    string lastAdd;
    string mainModRecord;
    ESTAB mainESTAB(startingProgAddr);
    for (int i = 1; i < argc; i++) {

        //Process the files one at a time, passing in the similar
        //std::vector<char> mainFileContents = readFile(argv[i]);
        if (i == 1) {
            strtAdd = startingProgAddr;
        } else {
            strtAdd = endAdd;
        }
        //Declarations for variables used per file
        mainModRecord = "";
        std::string line;
        std::string addressMemCheck;
        std::ifstream readingFile{argv[i]};
        string memLocation;
        string ctrlName;
        string symContents;
        string tempModLine;
        string extRefString;
        string directiveContents;
        string argumentContents;
        TextRecord mainText;
        vector<string> refSymbols;
        vector<string> memAddressses;
        ObjectFile mainOBJ;
        ModificationRecord mainMod;
        char argumentMod;
        string fileName = argv[i]; //Generate the obj file name;
        fileName = fileName.erase(fileName.size() - 3, 3) + "obj";
        ofstream objWriteFile;
        objWriteFile.open(fileName);
        string opCode;
        ObjectFile objFile;
        bool commentSeen = false, extendedFormat = false;
        //Loads the control section at the starting address
        mainText.ctrlLoadAdd = strtAdd;

        while (std::getline(readingFile, line)) {
            //GETLINE
            for (int j = 0; j < line.length(); j++) {
                if (line[j] == '.') { // check for comments
                    commentSeen = true;
                }
                if (commentSeen) { // If a comment is seen, skip all processing from that point until the rest of the line
                    //continue;
                } else {
                    if (j >= 0 && j <= 3) {// Fetching the memory address
                        if (line[j] <=32){
                            //continue;
                        }
                        else {
                            memLocation += line[j];
                            memAddressses.push_back(memLocation); // store memory address to a stack for memory check
                        }
                    }
                    if (j >= 8 && j <= 13) {// Fetching the Symbol name
                        if (line[j] == ' ') {
                            //continue
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
                            // 

                        }
                    }
                    if (j >= 17 && j <= 22) {//Fetching the Directive
                        if (line[j] == ' ') {
                            //continue
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
                    if (j >= 51 && j <= 58) {//Fetching the opCode
                        if (line[j] == ' ') {
                            //continue;
                        } else {
                            opCode += line[j];
                        }
                    }
                }
            }
            //*************
            //*PROCESSLINE*
            //*************
            //
            //
            //Generating Modification Records:
            /*
             * Any time a symbol is encountered:
             * Check if it exists on the external reference list
             *      if it is, generate a modification record that takes the memory address,
             */
            //
            //ASSEMBLER DIRECTIVES: START, END, BYTE, WORD, RESB, RESW

            if (directiveContents == "EXTREF") { // Handling the Assembler Directive EXTREF
                extRefString = objFile.generateReferString(argumentContents);
                //Processing for Modification Records
                //Create a vector of reference strings to use for generating modification records down below
                string tempString;
                for (int iR = 1; iR < extRefString.length(); iR+=6) {
                    int diffOfiandLen;
                    if (extRefString.length() - iR < 6) {
                        diffOfiandLen = extRefString.length() - iR;
                    } else {
                        diffOfiandLen = 6;
                    }
                    for (int jR = 0; jR < diffOfiandLen; jR++) {
                        if (extRefString[iR + jR] != '_') {
                            tempString += extRefString[iR + jR];
                        }

                    }
                    refSymbols.push_back(tempString);
                    tempString = "";
                }

            }
            else if (directiveContents == "EXTDEF") {// EXTDEF handling
                //Look at the arguments
                unsigned short int lBound = 0, boundDiff = 0;
                for (int k = 0; k <= argumentContents.length(); k++) {//Parsing
                    if (argumentContents[k] == ',' || k == argumentContents.length()) {
                        boundDiff = k - lBound;
                        string subArg = "";
                        subArg = argumentContents.substr(lBound, boundDiff);
                        if (subArg.at(subArg.length() - 1) <= 32) { //Erase any unimportant characters
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
            }
            else if (directiveContents == "RESW"){ // RESW handling, skip this record and generate a text file from the lines above
                mainText.generateTextRecord();

            }
            else if (directiveContents == "BASE"){
                //continue
            }

            else if (opCode != ""&&directiveContents!="C'EOF'") {//Processing OpCodes for text records
                //Generate Text record for this line
                mainText.addTextRecordInstruction(opCode,memLocation,strtAdd);
                if (mainText.recordStartingAdd == "") {
                    if (memLocation != "") {
                        //We use strtol alot in this program, here is one of its uses. To convert a string to a base 10 int
                        mainText.recordStartingAdd = strtol(&memLocation[0], nullptr, 10) + strtAdd;
                    }
                }

            }
            else{ //We have reached a point in code where we need to generate a text record for all other conditions have been met
                if(mainText.getTextRecordLength()!=0){
                    mainText.generateTextRecord();
                }

            }
            //If there is an argument content that is not being externally defined or referenced, process the argument and check for symbols
            if (!argumentContents.empty()&&directiveContents!="EXTREF"&&directiveContents!="EXTDEF"){
                bool argCheck = false;
                for (int k=0;k<argumentContents.size()-1;k++){
                    if (argumentContents[k] >= 65){//Checks if there is a non numerical value stored within arguments for further processing
                        argCheck = true;
                    }
                }
                if (argCheck){
                    //process the argument, and generate a modification record
                    tempModLine = processArgument(mainMod,refSymbols,memLocation,argumentContents,extendedFormat,ctrlName);
                    if (!tempModLine.empty()) {
                        if (tempModLine[0] != 'M') {
                            if (tempModLine.length()<10){
                                tempModLine="";
                            }
                            else {
                                while (tempModLine[0] != 'M') {
                                    tempModLine.erase(0, 1);
                                }

                            }

                        }
                    }
                    mainModRecord += tempModLine;
                }
            }

            if(!symContents.empty()){ // if there is a symbol declaration, check to see if it exists in the ESTAB
                for (int k=0;k<=mainESTAB.ESTABrows-1;k++){
                    if (symContents == mainESTAB.ESTABtest.at(k).symName){
                        if (memLocation!="") {
                            mainESTAB.ESTABtest.at(k).Address = strtol(&memLocation[0], nullptr, 16);
                        }
                    }
                }
            }

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
        objWriteFile <<std::hex<<objFile.generateHeaderString(ctrlName, to_string(mainESTAB.ESTABtest.at(ctrlSectLine).Address),
                                                              to_string(mainESTAB.ESTABtest.at(ctrlSectLine).Length))<<endl;
        //Define Record
        objWriteFile<<"D";
        for (int i = ctrlSectLine;i<mainESTAB.ESTABrows;i++){
            if (mainESTAB.ESTABtest.at(i).isHeader == false && mainESTAB.ESTABtest.at(i).ctrlSection == mainESTAB.ESTABtest.at(ctrlSectLine).ctrlSection){
                objWriteFile<<objFile.generateDefineString(mainESTAB.ESTABtest.at(i).symName, to_string(mainESTAB.ESTABtest.at(i).Address));
            }
        }
        //Refer Record
        objWriteFile << endl << extRefString;
        //Text Record
        objWriteFile<<endl<<mainText.finalTextRecord;
        //Modification Record
        objWriteFile<<mainModRecord;
        //End Record
        objWriteFile<<objFile.generateEndString(mainESTAB.ESTABtest.at(ctrlSectLine).Address);
       
        objWriteFile.close();           //close file after writing

        while (!memAddressses.empty()){
            string addressToCheck = memAddressses[memAddressses.size() - 1];        //
            memAddressses.pop_back();
            int iAddMemCheck = strtol(&addressToCheck[0], nullptr, 10);
            if(iAddMemCheck < startingProgAddr || iAddMemCheck > endAdd){
                cout << "MEMCHECK: Address is out of bounds.";
                return 0;
            }
            
        }
        
    }
    //reset argv to 1, while loop to read in lines, compare each address to the start and end address
    //increment argv to go to next file to read in
    mainESTAB.createESTABfile();
}

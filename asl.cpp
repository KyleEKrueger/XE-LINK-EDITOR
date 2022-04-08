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
/*
//printVectorConents takes the address to a character vector, and prints the contents to the console.
void printVectorContents(std::vector<char> *vectorPointer){
    for (int i=0; i<vectorPointer->size();i++){
        std::cout<< vectorPointer->at(i);
    }
    std::cout<<std::endl;
}
//readFile will use ifstream to create a character vector containing the information of the input files
std::vector<char> readFile(std::string fileName){
    std::vector<char> fileContents;
    std::ifstream fileStream;
    fileStream.open(fileName, std::ifstream::in);
    fileStream.seekg(0, std::ifstream::end);
    size_t fileSizeBytes = fileStream.tellg();
    fileContents.resize(fileSizeBytes);
    fileStream.seekg(0,std::ifstream::beg);
    fileStream.read(&fileContents[0],fileSizeBytes);
    /*if (DEBUG){
        printVectorContents(&fileContents);
    }
    return fileContents;
}
int main(int argc, char **argv) {

    void createESTABfile(ESTAB mainESTAB);

    ESTAB mainESTAB(0);
    if (DEBUG){ //Debug to create a sample ESTAB and create a file
        mainESTAB.addNewESTABRow();
        mainESTAB.addNewESTABRow();
        mainESTAB.addNewESTABRow();
        mainESTAB.addNewESTABRow();
        mainESTAB.ESTABtest.at(0).symName = "Tacos";
        mainESTAB.ESTABtest.at(0).isHeader = true;
        mainESTAB.ESTABtest.at(1).symName = "Brett";
        mainESTAB.ESTABtest.at(2).symName = "Kyle";
        mainESTAB.ESTABtest.at(3).symName = "Guy";
        createESTABfile(mainESTAB);
    }

    //Check for arguments
    if (argc == 0){
        std::cout<<"PROGRAM TERMINATED: ARGUMENTS NOT FOUND\n";
        return 0;
    }
    for (int i = 0; i < argc; i++){
        //Process the files one at a time, passing in the similar
        //std::vector<char> mainFileContents = readFile(argv[i]);
        std::string line;
        std::ifstream readingFile{argv[i]};
        while(std::getline(readingFile, line)) {
             std::cout << line << '\n';
        }
    }
//To pass the contents of a file to a module, reference the address of the fileContents vector
    return 0;
}
void createESTABfile(ESTAB mainESTAB){
		//put code for opening file here
    ofstream writeFile;
    writeFile.open("ESTAB.st");
    if (writeFile.is_open()){
        for (int i = 0; i < mainESTAB.ESTABrows; i++){
            writeFile << setfill('0') << setw(6) << mainESTAB.ESTABtest.at(i).ctrlSection;
    	    writeFile << "||";
    	    writeFile << setfill(' ') << setw(6) << left << mainESTAB.ESTABtest.at(i).symName;
    	    writeFile << "||";
    	    writeFile << setfill('0') << setw(6) << std::hex << mainESTAB.ESTABtest.at(i).Address;
    	    writeFile << "||";
    	    writeFile << setfill('0') << setw(6) << std::hex << mainESTAB.ESTABtest.at(i).Length;
    	    writeFile << std::endl;
			//Put code for writing to file here
			//ESTABtest.at(i).ctrSection;
		}
		//put code for closing file here
		cout<<"File Created: ESTAB.st";
    } else{
        cout << "\nError opening ESTAB.st\n";
    }
    writeFile.close();
}*/

int main(int argc, char **argv) {
//Check for arguments
    if (argc == 0){
        std::cout<<"PROGRAM TERMINATED: ARGUMENTS NOT FOUND\n";
        return 0;
    }
    for (int i = 1; i < argc; i++){
        //Process the files one at a time, passing in the similar
        //std::vector<char> mainFileContents = readFile(argv[i]);
        std::string line;
        std::ifstream readingFile{argv[i]};
        char memLocation[4];
        char symContents[6];
        char directiveContents[6];
        int columnCount = 0; // 0 = Memory col; 1 = Symbol col; 2 = Op col; 3 = Argument col; 4 = OpCode col;
        int strtAdd;
        int endAdd;
        //Our Pointer like variable that will be used to access
        int scanInt = 0;
        bool exitLine, commentSeen = false,extendedFormat = false;

        while(std::getline(readingFile, line)) {
        //GETTING THE LINE CONTENTS
            for (int i = 0; i < line.length()-1;i++){
                if (line[i] == '.'){ // check for comments
                    commentSeen = true;
                }
                if (i >= 0 && i <= 3){// Fetching the memory address
                    memLocation[i] = line[i];
                }
                if (i>=8 && i<=15){// Fetching the Symbol name
                    if (line[i] == ' '){
                        continue;       //go to top of for loop to ignore spaces
                    }
                    else{
                        symContents[i] = line[i];
                    }
                }
                if (i == 16){
                    if(line[i] == ' '){
                        continue;
                    }
                    else if (line[i] == '+'){
                        extendedFormat = true;
                    }
                    else if (line[i] == '='){
                        //Check to see if the characters are end of file. AWAITING PROFESSOR RESPONSE TO EMAIL

                    }
                }
            }

                while (!exitLine)
                {
                    if (line[scanInt] == '.')
                    { // check if we have found a comment, if so end the scanning of the line
                        exitLine = true;
                    }
                    else{
                    if (columnCount == 0){
                        memLocation[scanInt] = line[scanInt];
                    }
                    else if (columnCount == 1){
                        symContents[scanInt] = line[scanInt];
                    }
                    else if (columnCount == 2){
                        directiveContents[6] = line[scanInt];
                    }

                    if (line[scanInt] == ' '){
                        columnCount++;
                    }
                    //Store memory variable from first line until first space
                    //Skip leading spaces, and check for symbol name Deal with symbol name
                    //Skip leading spaces, and check for directives
                    //Skip leading spaces, and check for object code
                    //if there is no object code, process assembler directive
                    //Process last line

                }
                scanInt++;
                

            }
            //Processing the Line
            std::cout << line << '\n';
        }
        strtAdd = endAdd;

    }
}

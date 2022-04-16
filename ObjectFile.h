/*
Kyle Krueger and Brett Gallagher
cssc0413, cssc0423
CS530, Spring 2022
Assignment 2
ObjectFile.h
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>

using namespace std;

typedef class TextRecord {
public:
    string textRecordLine; //0-68 chars for total text record, 60 chars max of instructions
    string recordStartingAdd;
    string ctrlLoadAdd;
    string finalTextRecord;
    int textRecordLength;


    int getTextRecordLength(){
        return textRecordLine.length()/2;
    }

    //Generates the text record file and resets the Text record variables
    string generateTextRecord() {
        //textRecordLine
        string outputString;
        //Build the text record by copying textRecordLine into a string stream for formatting
        if (textRecordLine.length()!=0) {
            outputString = "T";
            string intermString;
            stringstream ss;
            ss.clear();
            ss << setw(6) << setfill('0') << right << recordStartingAdd;
            ss >> intermString;
            ss.clear();
            outputString += intermString;
            intermString = "";
            ss << std::hex << setw(2) << setfill('0') << right << getTextRecordLength();
            ss >> intermString;
            outputString += intermString;
            intermString = "";
            outputString += textRecordLine;
            textRecordLine = "";
            outputString+='\n';
            finalTextRecord+=outputString;
        }
        //reset the record starting address, and return the processed text record
        recordStartingAdd = "";
        return finalTextRecord;


    }

    //Adds an opCode to a text record, generates text record if text record has reached maximum length
    void addTextRecordInstruction(string opCode, string memLocation,int startAddress ) {
        memLocation = to_string(stoi(memLocation)+startAddress);

        if (recordStartingAdd == ""){//upon the first text record entry, store its starting address to the text record
            recordStartingAdd = memLocation;
        }
        if (textRecordLine.length() + opCode.length() > 60) {
            generateTextRecord();
            addTextRecordInstruction(opCode,memLocation,stoi(memLocation));
        } else {
            for (int i=0; i<opCode.length();i++){
                if (opCode[i]<=32){
                    opCode.erase(i,1);
                }
            }
            textRecordLine += opCode;
        }
    }

    
} TextRecord;

typedef class ModificationRecord{
public:
    string modRecordContents;
    //Generate modification record using the contents of modRecordContents
    string generateModificationRecord(string memLocation, string modName, bool extendedFormat,bool isAddition){
        string modRecordLine = "M";
        stringstream ss;
        string tempString;
        int tempInt;
        if (memLocation!="") { // memecheck to check if there is a memory address
            tempInt = strtol(&memLocation[0], nullptr, 16);
        }
        else {
            cout<<"MEMCHECK: Memory fault generating Modification record using the symbol "<<modName<<endl;
        }
        tempInt++;
        //string stream for processsing output
        ss<<tempInt;
        ss>>std::hex>>tempString;
        ss.clear();
        ss<<setw(6)<<setfill('0')<<tempString;
        ss>>tempString;
        ss.clear();
        modRecordLine+=tempString;
        tempString="";
        if(extendedFormat){
            modRecordLine+="05";
        }
        else{
            modRecordLine+="03";
        }
        if (isAddition){
            modRecordLine+="+";
        }
        else{
            modRecordLine+="-";
        }
        modRecordLine+=modName;
        modRecordLine+='\n';
        //cout<<modRecordLine;
        return modRecordLine;
    }

}ModificationRecord;
typedef struct ObjectFile {
public:
    string headerString;        //string to store and print header
    string defineString;        //string to store and print define
    string referString;         //string to store and print refer
    string endString;           //string to store and print end

public:

    //create header string to print in object file
    string generateHeaderString(string programName, string startAddress, string endAddress) {           //take in the program name, the starting address, and the ending address
        string startAdd = startAddress;
        string lengthStr = endAddress;
        string tempStr;
        
        stringstream ss;
        ss<<"H";                                            //leading H
        ss<<setw(6)<<left<<setfill('_')<<programName;
        ss>>headerString;                                   //add program name to string that will be returned
        ss.clear();
        ss<<setw(6)<<right<<setfill('0')<<startAdd;
        ss>>tempStr;
        headerString += tempStr;                            //add starting address to string that will be returned
        ss.clear();
        tempStr ="";
        ss<<setw(6)<<right<<setfill('0')<<lengthStr;
        ss>>tempStr;
        headerString += tempStr;                            //add ending address to string that will be returned
        ss.clear();
        tempStr ="";
        return headerString;                                //return string of program name + start address + end address
    }

    //create define string to print in object file
    string generateDefineString(string name, string address) {      //take in the symbol name and address
        string tempString;
        defineString ="";

        stringstream ss;
        ss<<setw(6)<<left<<setfill('_')<<name;                  //add symbol name to string that will be returned
        ss>>tempString;
        ss.clear();
        defineString+=tempString;
        tempString="";
        ss<<setw(6)<<setfill('0')<<right<<address;              //add address to string that will be returned
        ss>>tempString;
        ss.clear();
        defineString+=tempString;
        tempString="";
        return defineString;                                    //return string of symbol name + address
    }

    //create refer string to print in object file
    string generateReferString(string inputString) {            
        //input string processing
        string processingString;
        string processingSubString;
        string returnString;
        string tempString;
        stringstream ss;
        unsigned short int j = 0; // This is the beginning of a substring [j, ',']
        unsigned short int k = 0; // K is the distance between substrings
        for (int i = 0; i <= inputString.size(); i++) {
            //Grab sub string from j (end of last sub string) to i-1 (string before the last char / comma is)
            if (inputString[i] == ',' || i == inputString.length()) {
                //Find the length between j and i
                k = i - j;
                processingSubString = inputString.substr(j, k); // Starting at J create a substring for k length
                ss<<setw(6)<<left<<setfill('_')<<processingSubString;
                ss>>tempString;
                ss.clear();
                returnString +=tempString;
                tempString="";
                j = i + 1;      //start j at the char to the right of the comma

            }
        }
        returnString.insert(0,"R");
        return returnString;                        //return R + names of referred symbols
    }

    //create end string to print in object file
    string generateEndString(int startAddress) {            //take in the starting address of the section
        stringstream ss;
        string tempString;
        endString = 'E';                                    //leading E
        ss<<setw(6)<<setfill('0')<<startAddress;            //add address
        ss>>tempString;
        endString+=tempString;
        ss.clear();
        tempString="";
        endString+='\n';
        return endString;                                   //return E + address
    }
} ObjectFile;
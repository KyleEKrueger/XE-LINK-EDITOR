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

            //cout << outputString << endl;//eventually add to something else
            textRecordLine = "";
            outputString+='\n';
            finalTextRecord+=outputString;
        }
        recordStartingAdd = "";


    }

    //Adds an opCode to a text record, generates text record if text record has reached maximum length
    void addTextRecordInstruction(string opCode, string memLocation) {

        if (recordStartingAdd == ""){
            recordStartingAdd = memLocation;
        }
        if (textRecordLine.length() + opCode.length() > 60) {
            generateTextRecord();
            addTextRecordInstruction(opCode,memLocation);
        } else {
            for (int i=0; i<opCode.length();i++){
                if (opCode[i]<=32){
                    opCode.erase(i,1);
                }
            }
            textRecordLine += opCode;
        }
    }

    string stringToStringDecToHex(string inputString){
        stringstream ss;
        string intermString;
        ss<<std::hex<<stoi(inputString);
        ss>>intermString;
        return intermString;
    }
} TextRecord;
typedef struct ObjectFile {
public:
    string headerString;
    string defineString;
    string referString;
    string textString;
    string modificationString;
    string endString;

public:

    string generateHeaderString(string programName, int startAddress, int endAddress) {
        string startAdd = to_string(startAddress);
        int length = endAddress;
        string lengthStr = to_string(length);

        while (programName.size() < 6) {
            programName += ' ';
        }

        headerString = "H" + programName + startAdd + lengthStr;
//        strstream ss;
//        ss<<"H"<<programName<<setw(6)<<startAdd<<lengthStr;
//        ss>>headerString;
//        ss.clear();
        return headerString;
    }

    string generateDefineString(string name, string address) {

        while (name.size() < 6) {
            name += ' ';
        }

        defineString = "D" + name + address;
        return defineString;
    }

    string generateReferString(string inputString) {
        //input string processing
        string processingString;
        string processingSubString;
        unsigned short int j = 0; // This is the beginning of a substring [j, ',']
        unsigned short int k = 0; // K is the distance between substrings
        for (int i = 0; i <= inputString.size(); i++) {
            //Grab sub string from j (end of last sub string) to i-1 (string before the last char / comma is)
            if (inputString[i] == ',' || i == inputString.length()) {
                //Find the length between j and i
                k = i - j;
                processingSubString = inputString.substr(j, k); // Starting at J create a substring for k length
                while (processingSubString.size() < 6) { //add spaces until the substring is 6 chars in length
                    processingSubString += ' ';
                }
                processingString += processingSubString;// add substring to the final string
                j = i + 1;//start j at the char to the right of the comma
            }
        }
        string returnString = "R" + processingString + "\n";
       // cout << endl << returnString << "|Length: " << returnString.length() << endl;
        return returnString;
    }

    string generateTextString() {

        return "GenerateTestString()";
    }

    string generateModificationString() {
        return "GenerateModificationString()";
    }


    string generateEndString(int startAddress) {
        string endString = to_string(startAddress);
        endString = "E" + endString + "\n";
        return endString;
    }


}ObjectFile;


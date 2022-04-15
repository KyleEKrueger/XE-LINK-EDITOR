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
        return finalTextRecord;


    }

    //Adds an opCode to a text record, generates text record if text record has reached maximum length
    void addTextRecordInstruction(string opCode, string memLocation,int startAddress ) {
        memLocation = to_string(stoi(memLocation)+startAddress);

        if (recordStartingAdd == ""){
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

    string stringToStringDecToHex(string inputString){
        stringstream ss;
        string intermString;
        ss<<std::hex<<stoi(inputString);
        ss>>intermString;
        return intermString;
    }
} TextRecord;

typedef class ModificationRecord{
public:
    string modRecordContents;

    void generateModificationRecord(string memLocation, string modName, bool extendedFormat,bool isAddition){
        string modRecordLine = "M";
        stringstream ss;
        string tempString;
        int tempInt = strtol(&memLocation[0], nullptr,16);
        tempInt++;
        ss<<tempInt;
        ss>>std::hex>>tempString;
        ss.clear();
        ss<<setw(6)<<setfill('0')<<tempString;
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
        modRecordContents = modRecordContents+ (modRecordLine+"\n");
    }

}ModificationRecord;
typedef struct ObjectFile {
public:
    string headerString;
    string defineString;
    string referString;
    string textString;
    string modificationString;
    string endString;

public:

    string generateHeaderString(string programName, string startAddress, string endAddress) {
        string startAdd = startAddress;
        string lengthStr = endAddress;
        string tempStr;
        char space = ' ';



//        while (programName.size() < 6) {
//            programName += '_';
//        }
        // headerString = "H" + programName + startAdd + lengthStr;
        stringstream ss;
        ss<<"H";
        ss<<setw(6)<<left<<setfill('_')<<programName;
        ss>>headerString;
        ss.clear();
        ss<<setw(6)<<right<<setfill('0')<<startAdd;
        ss>>tempStr;
        headerString += tempStr;
        ss.clear();
        tempStr ="";
        ss<<setw(6)<<right<<setfill('0')<<lengthStr;
        ss>>tempStr;
        headerString += tempStr;
        ss.clear();
        tempStr ="";




//        strstream ss;
//        ss<<"H"<<programName<<setw(6)<<startAdd<<lengthStr;
//        ss>>headerString;
//        ss.clear();
        return headerString;
    }

    string generateDefineString(string name, string address) {
        string tempString;
        defineString ="";

//        while (name.size() < 6) {
//            name += ' ';
//        }
        stringstream ss;
        ss<<setw(6)<<left<<setfill('_')<<name;
        ss>>tempString;
        ss.clear();
        defineString+=tempString;
        tempString="";
        ss<<setw(6)<<setfill('0')<<right<<address;
        ss>>tempString;
        ss.clear();
        defineString+=tempString;
        tempString="";
        //defineString =  name + address;
        return defineString;
    }

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
                //cout<<returnString;
                tempString="";
                j = i + 1;//start j at the char to the right of the comma

            }
        }
        returnString.insert(0,"R");
        //cout<<returnString;

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
        stringstream ss;
        string tempString;
        endString = 'E';
        ss<<setw(6)<<setfill('0')<<startAddress;
        ss>>tempString;
        endString+=tempString;
        ss.clear();
        tempString="";
        //endString = "E" + endString + "\n";
        endString+='\n';
        return endString;
    }


}ObjectFile;

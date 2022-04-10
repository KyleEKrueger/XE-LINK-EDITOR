#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
using namespace std;

struct ObjectFile{
public:
string headerString;
string defineString;
string referString;
string textString;
string modificationString;
string endString;

public: 

string generateHeaderString(string programName, int startAddress, int endAddress){
        string startAdd = to_string(startAddress);
        int length = endAddress - startAddress;
        string lengthStr = to_string(length);
        
        while (programName.size() < 6){
                programName += ' ';
        }
    
        string headerString = "H" + programName + startAdd + lengthStr;
    }

string generateDefineString(string name, string address){
        
        while (name.size() < 6){
                name += ' ';
        }
    
        string headerString = "D" + name + address;
}

string generateReferString(string inputString){
    //input string processing
    string processingString;
    string processingSubString;
    unsigned short int j=0; // This is the beginning of a substring [j, ',']
    unsigned short int k=0; // K is the distance between substrings
    for(int i=0; i<=inputString.size();i++){
        //Grab sub string from j (end of last sub string) to i-1 (string before the last char / comma is)
        if (inputString[i] == ','|| i == inputString.length()){
            //Find the length between j and i
            k = i-j;
            processingSubString = inputString.substr(j,k); // Starting at J create a substring for k length
            while (processingSubString.size()<6){ //add spaces until the substring is 6 chars in length
                processingSubString += ' ';
           }
            processingString += processingSubString;// add substring to the final string
            j = i+1;//start j at the char to the right of the comma
        }
    }
    string returnString = "R"+processingString+"\n";
    cout<<endl<<returnString<< "|Length: " << returnString.length()<<endl;
    return returnString;
    }

string generateTestString(){

}

string generateModificationString(){
    
}


string generateEndString(int startAddress){
        string endString = to_string(startAddress);
        endString = "E" + endString + "\n";
        return endString;
    }
};
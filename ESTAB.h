#include <iostream>
#include <vector>
#include <strstream>
#include <fstream>
#include <iomanip>
using namespace std;

/*
* 2-D Array: [Control Section][Variable in Control Section]
* CTRLSC||SYMNAM||ADDRES||LENGTH
*
ADD   ||  	||000000||002F06
  	||Count ||000020||
  	||Table ||000023||
  	||Table2||001793||
Write ||SYMNAM||ADDRES||LENGTH
CTRLSC||SYMNAM||ADDRES||LENGTH
*/

//char[argc][2] char[30]

typedef class ESTABEntry {
public:
    //All entries in the sym tab should be 6 chars long
    bool isHeader = false; // if this is true, then symname will be the name of the control section
    unsigned short int ctrlSection = 0; // int that incriments with each control section
    string symName = "NULL";
    unsigned int Address = 0;
    unsigned int Length = 0;

    char outputRow[30];


    //Prints the contents of a row of the ESTAB
public:
    void printESTABEntry(){ // TODO: Create if statement for isHEader
        cout << setfill('0') << setw(6) << ctrlSection;
        cout << "||";
        cout << setfill(' ') << setw(6) << left <<symName;
        cout << "||";
        cout << setfill('0') << setw(6) << std::hex << Address;
        cout << "||";
        cout << setfill('0') << setw(6) << std::hex << Length;
        cout << std::endl;
    }


}ESTABEntry;



typedef struct ESTAB{
public:
    int ctrlSectionEnd; // The address of the end of the last control section
    std::vector<ESTABEntry> ESTABtest;
    unsigned short int ESTABrows = 0;
    //Pass 1: Create new ESTABEntry for Header (isHeader = true, so symname is the name of the ctrlsection; Set the address of the ctrl section) (Starting address = PROGADDR for the first ctrl section, then Starting Address = ctrlSectionEnd)
    //    	Search for EXTDEF
    //    	If EXTDEF create ESTABEntry for each EXTDef symbol with Address = Null, isHeader = False, ctrlSection
    //    	If Symbol defined = ESTABEntry SymName, then calculate address (Address = HeaderAddress + Symbol Address)
    //    	If EOF Set header length to EOF Address - Header Address Increment ctrlsection variable +1

    ESTAB(int progADDR){
        ctrlSectionEnd = progADDR;
    }

    //Creates a new ESTABEntry at the back of the array (Bottom of the list)
    void addNewESTABRow(){
        ESTABEntry nullEntry;
        ESTABtest.push_back(nullEntry);
        ESTABrows++;
    }
    //Ends the control section:
    //Calculates the length (DEC) of the control section and stores it to the ESTAB
    void endControlSection(int ctrlSection,string endingAddress){
        for (int i=0; i<ESTABrows;i++){
            if (ESTABtest.at(i).isHeader && ESTABtest.at(i).ctrlSection == ctrlSection){
                //String stream will take input as if it was a cout statement, and dump the input out with >> operator
                strstream ss;
                int iAddress,iEndingAddress;
                ss<<std::hex<<ESTABtest.at(i).Address; // Tells the ss that it is a hex value being input
                ss>>iAddress;
                ss.clear();
                ss<<std::hex<<endingAddress; // Tells the ss that it is a hex value being input
                ss>>iEndingAddress;
                ESTABtest.at(i).Length = iAddress + iEndingAddress; // Value calculated will be in DEC format
                cout<<"ENDCONTROLSECTION: The length calculated for control section "<<ctrlSection<<" is "<<std::hex<<ESTABtest.at(i).Length<<endl;

            }
        }
    }
} ESTAB;

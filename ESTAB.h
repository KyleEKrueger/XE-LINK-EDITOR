#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

/*
* 2-D Array: [Control Section][Variable in Control Section]
* CTRLSC||SYMNAM||ADDRES||LENGTH
*
ADD   ||      ||000000||002F06
      ||Count ||000020||      
      ||Table ||000023||      
      ||Table2||001793||      
Write ||SYMNAM||ADDRES||LENGTH
CTRLSC||SYMNAM||ADDRES||LENGTH
*/

//char[argc][2] char[30] 

class ESTABEntry {
    //All entries in the sym tab should be 6 chars long
    bool isHeader; // if this is true, then it will be the name of the control section
    unsigned short int ctrlSection; // int that incriments with each control section
    char symName[6]; 
    unsigned int Address;
    unsigned int Length;
    
    char outputRow[30];
    
    //Prints the contents of a row of the ESTAB
    void printESTABEntry(){
        cout << setfill('0') << setw(6) << ctrlSection;
        cout << "||";
        cout << setfill(' ') << setw(6) << symName;
        cout << "||";
        cout << setfill('0') << setw(6) << std::hex << Address;
        cout << "||";
        cout << setfill('0') << setw(6) << std::hex << Length;
        cout << std::endl;
    }

};

typedef struct ESTAB{
    int ctrlSectionEnd; // The address of the end of the last control section
    std::vector<ESTABEntry> ESTABtest;
    //Pass 1: Create new ESTABEntry for Header (isHeader = true, so symname is the name of the ctrlsection; Set the address of the ctrl section) (Starting address = PROGADDR for the first ctrl section, then Starting Address = ctrlSectionEnd)
    //        Search for EXTDEF
    //        If EXTDEF create ESTABEntry for each EXTDef symbol with Address = Null, isHeader = False, ctrlSection
    //        If Symbol defined = ESTABEntry SymName, then calculate address (Address = HeaderAddress + Symbol Address)
    //        If EOF Set header length to EOF Address - Header Address Increment ctrlsection variable +1

    ESTAB(int progADDR){
        ctrlSectionEnd = progADDR;
    }

} ESTAB;
/*Kyle Krueger and Brett Gallagher
cssc0413, cssc0423
CS530, Spring 2022
Assignment 2
ESTAB.h
*/

#include <iostream>
#include <vector>
#include <strstream>
#include <fstream>
#include <iomanip>
using namespace std;

typedef class ESTABEntry {
public:
    //All entries in the sym tab should be 6 chars long
    bool isHeader = false; // if this is true, then symname will be the name of the control section
    unsigned short int ctrlSection = 0; // int that incriments with each control section
    string symName = "NULL";            //string to store symbol name
    int Address = 0;                    //int to store address
    int Length = 0;                     //int to store length of section

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
        ctrlSectionEnd = progADDR;      //initialize estab, program address is where we load the program and end the previous section
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
                int iAddress =getPreviousEndAddress();
                int iEndingAddress = 0;
                ss<<std::hex<<"0x"<<ESTABtest.at(i).Address; // Tells the ss that it is a hex value being input
                ss>>iAddress;
                ss.clear();
                ss<<std::hex<<"0x"<<endingAddress; // Tells the ss that it is a hex value being input
                ss>>iEndingAddress;
                ss.clear();
                ss.flush();
                ESTABtest.at(i).Length = iEndingAddress - iAddress; // Value calculated will be in DEC format
                // cout<<"ENDCONTROLSECTION: The length calculated for control section "<<ctrlSection<<" is "<<std::hex<<ESTABtest.at(i).Length<<endl;

            }
        }
    }

    void createESTABfile(){
        
        //put code for opening file here
        ofstream writeFile;
        writeFile.open("ESTAB.st");
        if (writeFile.is_open()){
            for (int i = 0; i < ESTABrows; i++) {
                //format and print out the ESTAB for header
                if (ESTABtest.at(i).isHeader) {
                    writeFile << setfill(' ') << setw(6) << right << ESTABtest.at(i).symName;
                    writeFile <<flush<< "||";
                    writeFile << setfill(' ') << setw(8); //Why is this 8? IDK
                    writeFile <<flush<< "||";
                    writeFile << setfill('0') << setw(6) <<  std::hex  << ESTABtest.at(i).Address;
                    writeFile <<flush<< "||";
                    writeFile << setfill('0') << setw(6) << std::hex << ESTABtest.at(i).Length;
                } //format and print out the ESTAB
                else{
                    writeFile << setfill(' ') << setw(6) << ESTABtest.at(i).ctrlSection;
                    writeFile <<flush<< "||";
                    writeFile << setfill(' ') << setw(6) << right << ESTABtest.at(i).symName;
                    writeFile << flush<<"||";
                    writeFile << setfill('0') << setw(6) <<std::hex<< ESTABtest.at(i).Address;
                    writeFile <<flush<< "||";
                    if (ESTABtest.at(i).Length != 0) {
                        writeFile << setfill(' ') << setw(6) << std::hex << ESTABtest.at(i).Length;
                    }
                }
                writeFile <<std::flush<< std::endl;
            }
        } else{
            cout << "\nError opening ESTAB.st\n";       //tell user about file error
        }
        writeFile.close();                              //close file
    }

    int getPreviousEndAddress(){                        //get the previous end address
        int lastCS = ESTABtest.at(ESTABrows-1).ctrlSection;
            for (int i = 0; i < ESTABrows; i++) {
                if (ESTABtest.at(i).isHeader && ESTABtest.at(i).ctrlSection == lastCS - 1) {
                    return ESTABtest.at(i).Address + ESTABtest.at(i).Length;
                }
            }
        return 0;
    }

} ESTAB;
Kyle Krueger and Brett Gallagher
cssc0413, cssc0423
CS530, Spring 2022
Assignment 2
README

Name of all files in project: asl.cpp, ObjectFile.h, ESTAB.h, Makefile 
Compile instructions:
*******
STEPS TO COMPILE
1. Place the listing files to be converted into the same directory as the project files
2. type "make" in the terminal command line while in the directory
3. to run the program, type "./asl listing file name etc"
*******

Operating instructions (command line/inputs):
The user can use the command line to input the listing files that will generate the object files and ESTAB.

List of significant design decisions: 
We decided to use a one pass method to cycle between get line and process line in order to generate the seperate records and ESTAB entries,
and then combine the seperate records into a single .obj file, and then move to the next file, generating an ESTAB at the end.

List of extra features/algorithms/functionality that were not required: 

List of known deficiencies or bugs:
When printing the object files, the address is printed in decimal instead of hexadecimal. The address is correct except the format when printing is incorrect.

Lessons learned:
We grew our understanding of the C++ library, including the use of string streams, strtol, and some functions to avoid if at all possible (stoi). 
We learned how to parse files into individual tokens and then how to process lines of a file based on the tokens we parsed. 
Understanding what the ESTAB and listing files are and their importance in generating object files for the computer to run. 
We grew our skills for collaborative methods, specifically pair programming. 
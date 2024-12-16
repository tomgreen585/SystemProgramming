/**
 * abstractdb.cpp
 * C++ source file that should contain implementation for member functions
 * - loadCSV()
 * - saveCSV()
 * 
 * You need to modify this file to implement the above-mentioned member functions
 * as specified in the hand-out (Tasks 4 and 5)
 */ 

#include "abstractdb.hpp"
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>


using namespace nwen;
using namespace std;

//save contents to csv
bool AbstractDbTable::saveCSV(string filename){
	
	ofstream File(filename); //open file to read
	
	if(!File.is_open()){return false;} //error if unable to open file
	
	for(int i = 0; i < rows(); i++){
		try{
			movie* movieDesc = get(i); //pointer to movieRec
			//FILE FORMAT
			File 
			<< movieDesc->id << ",\"" 
			<< movieDesc->title << "\"," 
			<< movieDesc->year << ",\"" 
			<< movieDesc->director << "\"\n";
		} catch(exception &e){File.close(); return false;} //unable to write to file
	}
	
	File.close();
	return true; //successful in saving
	
};

//HPP - bool saveCSV(string fname)
bool AbstractDbTable::loadCSV(string filename){
	
	ifstream File(filename); //open file to read
	string index;
	movie m;
	
	if(!File.is_open()){return false;} //error if unable to open file
	
	while(getline(File, index)){
		string components[4];
		string comp;
		int i = 0;
		istringstream line(index); //read each line from file
		
		while(getline(line, comp, ',')){
			if(i > 3){File.close();return false;} //false if line doesnt follow FILE FORMAT
			components[i++] = comp; //split line into components
		}
		
		try{
			m.id = stoul(components[0], NULL, 10); //[0] string to int (id) 
			strcpy(m.title, components[1].c_str()); //[1] string to array (title)
			m.year = (unsigned int) stoul(components[2], NULL, 10); //[2] string to int (year)
			strcpy(m.director, components[3].c_str()); //[3] string to array (director)
			
			add(m); //add to table
		
		} catch(exception &e){
			File.close();
			return false; //error while adding
		}
	}
	
	File.close();
	return true; //sucessful loading 
}




/**
 * vectordb.cpp
 * C++ source file that should contain implementation for member functions
 * - rows()
 * - get()
 * - add()
 * - update()
 * - remove()
 * 
 * You need to modify this file to implement the above-mentioned member functions
 * as specified in the hand-out (Task 3)
 */ 

#include "vectordb.hpp"
#include <iostream>
#include <vector>

using namespace nwen;

//default constructor
VectorDbTable::VectorDbTable(){} 

//return num of rows in movieRec
int VectorDbTable::rows(){return movieRec.size();} 

//return pointer to movieRec at i(index)
movie* VectorDbTable::get(unsigned int i){
	
	if(i > rows()){return 0;} //error if outofrange
	
	return &movieRec[i]; //return pointer 
}

//add record to movieRec
bool VectorDbTable::add(movie &movieName){
	
	for(int i = 0; i < rows(); i++){//iterate through rows
		if(movieRec[i].id == movieName.id){return false;}//id is in movieRec return false
	}
	
	movieRec.push_back(movieName); //add to movieRec
	return true;
}

//updates movie in movieRec
bool VectorDbTable::update(unsigned long int id, movie &movieName){
	
	bool movieExists = false; 
	vector<movie>movieDesc; //temporary vector to store
	
	for(int i = 0; i < rows(); i++){ //iterate through rows
		if(movieRec[i].id == id){ //if id equal to one in movieRec
			movieDesc.push_back(movieName); //update info to vect
			movieExists = true;
		} else {movieDesc.push_back(movieRec[i]);} //copy existing info to vect
	}
	
	if(movieExists){movieRec = movieDesc; return true;} //replace movieRec with info
	
	return false; //does not exist
}

//removes movie from movieRec
bool VectorDbTable::remove(unsigned long int id){
	
	for(int i = 0; i < rows(); i++){ //iterate through the rows
		if(movieRec[i].id == id){ //if id equal to one in movieRec
			movieRec.erase(movieRec.begin()+i); //clear movieRec[i]
			return true; //if removed successfully
		}
	}
	
	return false; //does not exist
}

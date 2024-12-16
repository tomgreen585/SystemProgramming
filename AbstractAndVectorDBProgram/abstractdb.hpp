/**
 * abstractdb.hpp
 * C++ header file that should contain declaration for
 * - struct movie (given)
 * - AbstractDbTable abstract class
 * 
 * You need to modify this file to declare AbstractDbTable abstract class 
 * as specified in the hand-out (Task 1)
 */ 

#ifndef __ABSTRACT_DB_HPP__
#define __ABSTRACT_DB_HPP__
#include <string>

using namespace std;
namespace nwen 
{
    struct movie {
        unsigned long id; 
        char title[50];
        unsigned short year;
        char director[50];
        
    };
    
    class AbstractDbTable {
		public:
			virtual int rows() = 0; //returns int 
			virtual movie* get(unsigned int i) = 0; //accepts int para -> returns pointer
			virtual bool add(movie &movieName) = 0; //movie structure
			virtual bool update(unsigned long int id, movie &movieName) = 0; //accepts int & movie -> return bool
			virtual bool remove(unsigned long int id) = 0; //accepts int -> return bool
			
			bool loadCSV(string filename); //accepts string -> returns bool
			bool saveCSV(string filename); //accepts string -> returns bool
	};	
}

#endif /* __ABSTRACT_DB_HPP__ */

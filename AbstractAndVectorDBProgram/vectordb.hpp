/**
 * vectordb.hpp
 * C++ header file that should contain declaration for
 * - VectorDbTable class
 * 
 * You need to modify this file to declare VectorDbTable class 
 * as specified in the hand-out (Task 2)
 */ 

#include <vector>
#include "abstractdb.hpp"

namespace nwen{
	
	class VectorDbTable : public AbstractDbTable{
		
		public:
			VectorDbTable();
			
			int rows(); //returns int 
			movie* get(unsigned int i); //accepts int para -> returns pointer
			bool add(movie &movieName); //movie structure
			bool update(unsigned long int id, movie &movieName); //accepts int & movie -> return bool
			bool remove(unsigned long int id); //accepts int -> return bool
		
		private:
			std::vector<movie>movieRec; //store movie records table
	};
}

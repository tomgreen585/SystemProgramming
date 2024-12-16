#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbms.h"

int db_show_row(const struct db_table *db, unsigned int row){
	
	if(row < 0){return 0;} 
	if(row > db->rows_used){return 0;}
	
	const struct album table = db->table[row];
	
	//store character array
	char year[5], ID[7];
	
	//string to array
	snprintf(year, 5, "%d", table.year); 
	snprintf(ID, 7, "%lu", table.id);
	
	//ID
	if(strlen(ID) < 6){
		for(int i = 0; i < 6 - strlen(ID); i++){//6 space before
			printf(" "); //null spaces
		}
	}
	printf("%s:", ID); //print id string

	//TITLE
	int tL; //title length
    for (int i = 0; i < sizeof(table.title) / sizeof(char); i++){ 
        if (table.title[i] == '\0'){ //if end of title is reached
            tL = i; //set length to null
            break;
        }
    }
    
    int tP = 20 - tL; //num of spaces to fill up title
    if (tP > 0) {
        for (int i = 0; i < tL + tP; i++){//adds spaces when needed
            if (i < tP){printf(" ");}
            else{printf("%c", table.title[i - tP]);}//prints title
        }
    } else {
        int c = 0;
        while (c < 20){//title is 20 char then print
            printf("%c", table.title[c]);
            c++;
        }
    }
    printf(":");
	
	//ARTIST
	int aL;
    for (int i = 0; i < sizeof(table.artist) / sizeof(char); i++){
        if (table.artist[i] == '\0'){//if end of artist is reached
            aL = i; //set length to null
            break;
        }
    }
    
    int aP = 20 - aL; //num of spaces to fill up artist
    if (aP > 0) {
        for (int i = 0; i < aP; i++) {printf(" ");}//adds spaces where needed
    }
    
    for (int i = 0; i < aL; i++) {printf("%c", table.artist[i]);}//prints artist
    
    //YEAR
    if (strlen(year) < 4) {
        for (int i = 0; i < 4 - strlen(year); i++) {//adds spaces where needed
            printf(" ");
        }
    }
    printf(":%s\n", year); //prints year 

	return 1;
}

int db_add_row(struct db_table *db, struct album *a) {
    if (db->rows_used == db->rows_total) { //if full then create new struct
		struct album *newtab;
		db->rows_total += 5; //increase size by 5
        newtab = realloc(db->table, db->rows_total*sizeof(struct album)); //relocate mem for struct
        
        if (newtab == NULL) {return 0;}//if mem allocate fails
        db->table = newtab;//update pointer
    }
	
	db->table[db->rows_used++] = *a;//add album to struct
    return 1;
}

int db_remove_row(struct db_table *db, unsigned long id){
    
    for(int i = 0; i < db->rows_used; i++){ //loop to find entry that has matched ID
        if(db->table[i].id == id){ //loop does not match
            
            for(int k = i; k < db->rows_used-1; k++) //overwrite so it shuffles
            {db->table[k] = db->table[k+1];}

            db->rows_used--; //decremented by one
                   
            if (db->rows_used % 5 == 0){db->rows_total-=5;} //decrease by 5 if div
            if (db->rows_used == 0 && db->rows_total == 0) {db->table = NULL;} //if emputy then table is null     
            
            return 1;      
        }    
    }

return 0;        
}













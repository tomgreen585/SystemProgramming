#include <stdio.h>
#include <string.h>
#include "editor.h"

//stage1
int editor_insert_char(char editing_buffer[], int editing_buflen, char to_insert, int pos){

	for(int i = editing_buflen; i>pos;i--){ 
		editing_buffer[i] = editing_buffer[i-1];
	}
		
	if(pos<0 || pos >= editing_buflen){ return 0; }
		

	editing_buffer[pos] = to_insert;

	return 1;
}

//stage2
int editor_delete_char(char editing_buffer[], int editing_buflen, char to_delete, int offset){

    if( offset > editing_buflen || offset < 0 ){ 
		return 0; 
		}

//for (int i = offset; i < editing_buflen; i++)
    //{
        //if (editing_buffer[i] == to_delete)


    int f = 0;

    for (int i = offset; i < editing_buflen; i++ ) {
        if(editing_buffer[i] == to_delete){
            f = 1;
        }
        if(f == 1){
			editing_buffer[i] = editing_buffer[i+1];
        }
    }

    if(f == 1){
		return 1;
		} else {
			return 0;
			}
}

//stage 3
int editor_replace_str(char editing_buffer[], int editing_buflen, const char *str, const char *replacement, int offset) {
    int strLen = 0;
    int strPos = 0;

    if (offset < 0 || offset >= editing_buflen || str[0] == '\0') {
        return -1;
    }


//if(offset<0 || offset >= editing_buflen || str[0] == '\0'){ return -1; }

//if (found != NULL) {

//if (editing_buffer[i] == str[0]){
           
      //for (int j = 0; j < strLength + 1; j++){
           
		//foundStr[j] = editing_buffer[

    //for (int i = offset; i < editing_buflen; i++) {
        //if (editing_buffer[i] == 
        
		//editing_buffer[i] = editing_buffer[i -];




    for (int i = offset; i < editing_buflen; i++) {
        if (editing_buffer[i] == '\0') {
            break;
        }

        int y = 0;
        strLen = 0;

        while (str[y] != '\0' && editing_buffer[i+y] == str[y]) {
            strLen++;
            y++;
        }

        if (str[y] == '\0') {
            strPos = i;
            break;
        }
    }

    if (strLen == 0) {
        return -1;
    }



	//for (int i = editing_buflen; i > wordPoint + strLength; i--)

    for (int i = 0; i < strLen; i++) {
        editor_delete_char(editing_buffer, editing_buflen, editing_buffer[strPos], offset);
    }

    int replaceLen = 0;
    while (replacement[replaceLen] != '\0' && strPos + replaceLen < editing_buflen) {
        editor_insert_char(editing_buffer, editing_buflen, replacement[replaceLen], strPos+replaceLen);
        replaceLen++;
    }

    return strPos+replaceLen-1;
}




void editor_view(int rows, int cols, char viewing_buffer[rows][cols], const char editing_buffer[], int editing_buflen, int wrap) {
    // Set every character in viewing_buffer to null character
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            viewing_buffer[i][j] = '\0';
        }
    }
    
    // for(int i = 0 ; i<= rows; i++){
    //     for(int j = 0; j <= cols ; j++){
    //     viewing_buffer[i] = 0
    //     }
    // }
    // for(int i = 0 ; i< rows; i++){
    
    //     }
    
    int row = 0;
    int col = 0;
    
    for (int i = 0; i < editing_buflen; i++) {
        char n = editing_buffer[i];
        
        //newline
        if (n == '\n') {
            row++;
            col = 0;
            //beyond line
            if (row >= rows) {
                break;
            }
       
        } else {
            if (col >= cols - 1) {
                
                if (wrap) {
                    row++;
                    col = 0;
                    
                    //beyond line
                    if (row >= rows) {
                        break;
                    }
                } else {
                    continue;
                }
            }
            
            viewing_buffer[row][col] = n;
            col++;
        }
    }
}




// int main(void){




// editor_view(8, 11, viewing_buffer, "The quick brown\nfox jumps over\n\nthe lazy dog\n", 48, 0);
// }

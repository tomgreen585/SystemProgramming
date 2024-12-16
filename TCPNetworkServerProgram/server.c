#include<ctype.h>
// Include necessary header files
#include<stdio.h>
#include<sys/socket.h> //socket
#include<netinet/in.h> // socket address struct sockaddr_in 
#include<string.h> //copy string 
#include<stdlib.h> //exit()
#include<unistd.h> //read and write


//#define SERVER_PORT 12345 //ADDED BUT UNSURE

//Error 
void error(const char * msg){
    printf("Error : %s\n",msg);
    exit(1);
}

//GET 
void open_file(char* fname, int clfd) {
	
	fname[strlen(fname)-1]='\0'; //add null to end of file name
	printf("GET - Client opening file - %s\n",fname);

    FILE* f = fopen(fname, "r"); //open file - check name matches "filename"
    if (f == NULL) { //name not found
		ssize_t sClient = write(clfd, "SERVER 404 Not Found\n", strlen("SERVER 404 Not Found\n")); //return server error
		return;
    }
    
    //memory tb allocated
    fseek(f, 0, SEEK_END);
    long fSize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* fBuffer = (char*)malloc(fSize + 1); //allocate mem to filebuff
    
    if (fBuffer == NULL) { //if buff is null 
		error("GET - Cannot allocate memory"); //error didnt allocate correctly
		fclose(f); //close file
		return;
    }
    
    fBuffer[fSize] = '\0'; //add null
    fread(fBuffer, 1, fSize, f); //reads the file
    
    ssize_t sClient1 = send(clfd, "SERVER 200 OK\n\n", strlen("SERVER 200 OK\n\n"),0); //opening successful
    ssize_t sClient2 = send(clfd, fBuffer, fSize, 0); //send file
    ssize_t sClient3 = send(clfd, "\n\n\n", strlen("\n\n\n"),0); // new lines after printing file
    
    if (sClient2 == -1) {
        error("GET - Cannot send file"); //error if cant send file
    }
    
    fclose(f); //close file
    free(fBuffer); //free memory from fbuff
    
}

//PUT
void edit_file(char* fname, int clfd) {
    
    fname[strlen(fname)-1] = '\0'; //add null to end of file name
    printf("PUT - Client writing to file - %s\n", fname);

    FILE* f = fopen(fname, "w"); //open file = check name matches "filename"
    if (f == NULL) { //name is not found
		ssize_t sClient = write(clfd, "SERVER 404 Not Found\n", strlen("SERVER 404 Not Found\n")); //return server error
		return;
    }

    if (truncate(fname, 0) == -1) { //clear file conent
		error("PUT - Cannot clear contents");
		fclose(f);
		return;
    }

    char msg[100]; //store client msg
    ssize_t bR; //bytes
    int nLine = 0; //counter for new lines

    while ((bR = read(clfd, msg, sizeof(msg))) > 0) { //loop to read file to write client msg
		fwrite(msg, sizeof(char), bR, f); //write file

		for (int i = 0; i < bR; i++) {
			if (msg[0] == '\n') { //buffer contains \n
				nLine++; //increment for every newline
				if (nLine >= 2) {//if \n\n then finish
					bR = 0; //exit loop 
					break;
				}
			} else {
				nLine = 0;
			} //reset count if !\n    	  
		}
		if (bR == 0){ //exit loop
			break;
		}             
    }
    if (bR == -1){ //if unable to read
		error("PUT - Error reading input"); //throw error
	}    
    
    fclose(f); //close file
    ssize_t sClient = send(clfd, "SERVER 201 Created\n", strlen("SERVER 201 Created\n"),0); //file created successfully
}

//MAIN
int main(int argc, char *argv[]){
	
    if(argc == 1){ 
		printf("MAIN - no port"); 
		return -1;
	}
    
    int tempValue;
    sscanf(argv[1], "%d",&tempValue);
    
    if(tempValue < 1024){ //check port number less than 1024
		printf("MAIN - bad port"); 
		return -1;
	}
   
    int fd = socket(AF_INET, SOCK_STREAM,0); //create socket
    if(fd == -1){ 
        error("MAIN - Cannot create socket"); //error if cannot create socket
    }
    printf("MAIN - Created socket \n");
    
    //server address to bind socket to
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(tempValue);
    serveraddr.sin_addr.s_addr = INADDR_ANY; //socket accepts connection
    
    printf("MAIN - Created address \n");
    
    int bd; //bind socket to server address
    bd = bind(fd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)); //Bind: address -> socket
    
    if(bd ==-1){ //if binding unsuccessful
        error("MAIN - Error binding\n");
    }
    printf("MAIN - Binding successful\n");
    
    
    while(1){
       
		if(listen(fd,SOMAXCONN)<0){ //error while listening for connections
			error("MAIN - Listening error");   
		}
		printf("MAIN - Listen successful\n");
		
		//accept client connection
		struct sockaddr_in client_addr;
		int addrlen = sizeof(client_addr);
		int client_fd = accept(fd,(struct sockaddr *)&client_addr,(socklen_t*)&addrlen);
		
		if(client_fd<0){ //error while accepting client connection
			error("MAIN - Accepting Client error");   
		}
		printf("MAIN - Accept successful\n");
		
		char cntd[] = "Hello\n"; //write hello when succesfully conntected
		int sending = send(client_fd, cntd, strlen(cntd), 0); //send message
		
		if (sending < 0) { //error sending message
			error("MAIN - Error writing to client");
		}
		printf("MAIN - Write success\n");
		
		char msg[100]; //client input
		char start[4]; //start of client input
		char end[5]; //end of client input
		memset(msg,0,100);

		while (1) { //program loop
		
			int incoming = recv(client_fd, msg, sizeof(msg), 0); //client recv
			if (incoming < 0) {
				error("MAIN - Error reading from client message"); //error reading msg
			}
			   
			memcpy(start, msg, 3);  //add start to tempory buffer
			memcpy(end, msg + strlen(msg) - 5, 5);  //add end to tempory buffer
			start[3] = '\0'; //add null to end of start buffer
			end[4] = '\0'; //add null to end of end buffer
			
			//BYE
			if (strcasecmp(msg, "bye\n") == 0) {  //check bye
				break;
			}
			
			//GET
			else if (strcasecmp(start, "get") == 0) { //check get 
				
				char fname[100]; //filename
				int fFormat = 0; //format of file
				int fFound = 0; //file is found
				 
				for (int i = 4; i<strlen(msg); i++) { //iterate over msg
					fname[fFormat] = msg[i]; //store char
					fFormat++; //increment for next char
				}
				fname[fFormat] = '\0'; //null to terminate
				
				if(strlen(fname) > 1){ //if filename found
					fFound = 1; 
				}
				
				if(fFound == 1 && strcmp(end, ".txt") == 0){ //filename && includes .txt 
					open_file(fname, client_fd); //call get method to return file
				} else { //if not found throw
					ssize_t sCLient = send(client_fd, "SERVER 500 Get Error\n", strlen("SERVER 500 Get Error\n"), 0); 
				}
				 
				memset(fname, 0, sizeof(fname)); //reset
			}
			
			//PUT
			else if (strcasecmp(start, "put") == 0)  { //check put
			
				char fname[100]; //filename
				int fFormat = 0; //format of file
				int fFound = 0; //file is found
				
				for (int i = 4; i<strlen(msg); i++) { //iterate over msg
					fname[fFormat] = msg[i]; //store char
					fFormat++; //increment for next char
				}
				fname[fFormat] = '\0'; //null to terminate
				
				if(strlen(fname) > 1){ //if filename found
					fFound = 1;
				}
				
				if(fFound == 1 && strcmp(end, ".txt") == 0){ //filename && includes .txt
					edit_file(fname, client_fd); //call put method to edit file
				} else { //if not found throw
					ssize_t sCLient = send(client_fd, "SERVER 501 Put Error\n", strlen("SERVER 501 Pit Error\n"), 0); 
				}
				
				
				memset(fname, 0, sizeof(fname)); //reset
			
			} else {
				//error if put or get functions do not run
				ssize_t sClientprint = send(client_fd, "SERVER 502 Command Error\n", strlen("SERVER 502 Command Error\n"),0);
			}
			
			memset(start, 0, sizeof(start)); //reset put && get
			memset(msg, 0, sizeof(msg)); //reset buffer
		}
		close(client_fd); //BYE
	}
return 0;
}

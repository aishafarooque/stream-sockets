// https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h>

int low;
int high;

void error(char *msg) {
    perror(msg);
    exit(0);
}

void printResults(char * msg) {
    /*Seperate low price from high and print it to the user.*/
    char * token = strtok(msg, " ");
    if (strcmp(token, "-1") == 0) {
        printf("That make and model is not in the database.\n");
        return;
    }
    printf("The low price for that car is $%s.\n", token);
    token = strtok(NULL, " ");
    printf("The high price for that car is $%s.\n", token);
}

int main(int argc, char *argv[]) {

    printf("Enter the server host name: ");
    char hostname[256];
    fgets(hostname, 255, stdin);
    char *pos;
    if ((pos=strchr(hostname, '\n')) != NULL)
        *pos = '\0';  /*Remove new line from the end*/
    else 
        printf("Input is too long for buffer!\n");
    argv[1] = hostname;

    char str[20];
    printf("Enter the server port number: ");
    fgets(str, 19, stdin);
    argv[2] = str;

    printf("\n\n--Your're using %s and port #%s\n", argv[1], argv[2]);

    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char termination[256] = "quit\n";

    char temp[256];

   portno = atoi(argv[2]);						// port num
        
   server = gethostbyname(argv[1]);  			// host name
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }



    //run the client
    while (1) {
       sockfd = socket(AF_INET, SOCK_STREAM, 0);
       if (sockfd < 0)
         error("ERROR opening socket");


     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     bcopy((char *)server->h_addr,
       (char *)&serv_addr.sin_addr.s_addr,
       server->h_length);
     serv_addr.sin_port = htons(portno);

     if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
         error("ERROR connecting");
    

    // get the message
        char buffer[256];
    	printf("Please enter the make and model: ");
    	bzero(buffer,256);
    	fgets(buffer,255,stdin);
        

    //check for termination
    	if (strcmp(buffer,termination)==0) {
    		n = write(sockfd,buffer,strlen(buffer));
    		if (n < 0) 
    			error("ERROR writing to socket");
    		// bzero(buffer,256);

    		printf("---The client is closing---\n");
            exit(0);
    		break;
    	}

    //send the message
    	n = write(sockfd,buffer,strlen(buffer));
    	if (n < 0) 
    		error("ERROR writing to socket");
    	bzero(buffer,256);

    //receive the message
    	n = read(sockfd,buffer,255);
    	// printf("%s\n",buffer);
        printResults(buffer);
    	if (n < 0) 
    		error("ERROR reading from socket");

        printf("\n");
        // sleep(1);
    }
    return 0;
}

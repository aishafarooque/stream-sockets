// https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

int counterOfEntries = 0;
int foundLow = 0;
int foundHigh = 0;
int found = -1;

struct node {
   char make_model[256];
   int low;
   int high;
};

struct node database[512];

void removeSpaces(char * str) {
   /*Code credit to https://stackoverflow.com/questions/1514660/how-to-
   remove-all-spaces-and-tabs-from-a-given-string-in-c-language*/
   int count = 0;
   int i = 0;
   for (i = 0; str[i]; i++)
      if (str[i] != ' ')
         str[count++] = str[i];
   str[count] = '\0';
}

void readFromFile(char * filename) {
   FILE * inFile;
   char line[512], _make_model[20], *p;
   int _low, _high;
   inFile = fopen(filename, "r");
   int i = 0;

   if (!inFile) {
      printf("Couldn't open %s for reading\n", filename); //----------------------------------> test this! 
      return;
   } else {
      while (i < 512 && fgets(line, sizeof(line), inFile) != NULL) {
         removeSpaces(line);
         sscanf(line, "%s\t%d\t%d\t[^\n]", _make_model, & _low, & _high);
         strcpy(database[counterOfEntries].make_model, _make_model);
         database[counterOfEntries].low = _low;
         database[counterOfEntries++].high = _high;
         i++;
      }
   }
   fclose(inFile);
}

void printFileContents() {
   char _make_model[256];
   int _high, _low, i;
   for (i = 0; i < counterOfEntries; i++) {
      strcpy(_make_model, database[i].make_model);
      _low = database[i].low;
      _high = database[i].high;
      printf("%s \t %d \t %d\n", _make_model, _low, _high);
   }
}

// new find to work regardless of whitespaces inside query 
void find(char * query) {
   found = -1;
   int i, _high, _low;
   int len = strlen(query);
   if (query[len - 1] == '\n') {
      query[len - 1] = 0;
   }
   for (i = 0; i < counterOfEntries; i++) {
      char temp[sizeof(database[i].make_model)];
      strcpy(temp, database[i].make_model);
      //        printf("Comparing query %s with %s\n", query, temp);
      if (strcmp(temp, query) == 0) {
         printf("Make and model found, here is the high: ");
         foundLow = database[i].low;
         foundHigh = database[i].high;
         printf("%d and low: %d.\n", foundLow, foundHigh);
         found = 1;
         break;
      }
   }

   if (found == -1) {
    foundLow = -1;
    foundHigh = -1;
      printf("That make and model is not in the database.");
   }
}

void error(char * msg) {
   perror(msg);
   exit(1);
}

int main(int argc, char * argv[]) {
   int sockfd, newsockfd[256], portno, clilen;
   unsigned int i;
   // file io
   char filename[99], * p;
   printf("Enter name of file: ");
   fgets(filename, sizeof(filename), stdin);
   if ((p = strchr(filename, '\n')) != NULL) {
      * p = '\0'; /* remove newline */
   }

   readFromFile(filename);
   printFileContents();

   // port no
   char str[20];
   printf("\n\nEnter server port number: ");
   fgets(str, 19, stdin);
   argv[1] = str;

   char buffer[2000];

   char termination[256] = "quit\n";
   struct sockaddr_in serv_addr, cli_addr;
   int n;

   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0)
      error("ERROR opening socket");
   bzero((char * ) & serv_addr, sizeof(serv_addr));

   portno = atoi(argv[1]);
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   if (bind(sockfd, (struct sockaddr * ) & serv_addr,
         sizeof(serv_addr)) < 0)
      error("ERROR on binding");
  
  listen(sockfd, 10);
  clilen = sizeof(cli_addr);

   //run the server
   while (1) {
    // accept connection
      newsockfd[i] = accept(sockfd, (struct sockaddr * ) & cli_addr, & clilen);
      if (newsockfd < 0)
         error("ERROR on accept");

      bzero(buffer, 256);

    // receive message from client
      n = read(newsockfd[i], buffer, 255);
      if (n < 0) error("ERROR reading from socket");

    // check for termination
      if (strcmp(termination, buffer) == 0) {
        // We don't need the server to close.
         // printf("\n\n---The server is closing---\n");
         // break;
      } else {
    // find it
         removeSpaces(buffer);
         printf("\n\n--Finding high and low for %s", buffer);
         find(buffer);
         char temp[256];
         sprintf(buffer, "%d", foundLow);
         strcpy(temp,buffer);
         sprintf(buffer, "%d", foundHigh);
         strcat(temp, " ");
         strcat(temp,buffer);

         strcpy(buffer, temp);

         n = write(newsockfd[i], buffer, strlen(buffer)); // send the high and low
         if (n < 0) error("ERROR writing to socket");
      } //if-else

      i++;

   } //while
   return 0;
}
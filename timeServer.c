#include<stdio.h>
#include<string.h>    
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h>   
#include<time.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
  int socket_desc, new_socket, c;
  struct sockaddr_in server, client;

  char str[1000]; // input string from user

  // Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_desc == -1) {
    puts("Could not create socket");
    return 1;
  }

  server.sin_family = AF_INET; //IPv4 Internet protocols
  server.sin_addr.s_addr = INADDR_ANY; //IPv4 local host address
  server.sin_port = htons(8888); // server will listen to 8888 port

  // Bind
  if (bind(socket_desc, (struct sockaddr * ) & server, sizeof(server)) < 0) {
    puts("Binding failed");
    return 1;
  }
  puts("Socket is binded");
  // Listen
  listen(socket_desc, 3);
  // Accept and incoming connection
  puts("Waiting for incoming connections...");
  puts("Type    telnet localhost 8888      to connect");
  c = sizeof(struct sockaddr_in);
  new_socket = accept(socket_desc, (struct sockaddr * ) & client, (socklen_t * ) & c);
  if (new_socket < 0) {
    puts("Accept failed");
    return 1;
  } 
 
  puts("Connection accepted\n");

   // SERVER DESCRIPTIONS
  puts(" to separate date only   :  /  -   strings are accepted  \n");
  puts("GET_DATE is required before date request string   \n");
  puts("empty string or string with full of space character is not accepted \n");
  puts(" number of space character before GET_DATE or between GET_DATE and date request string is not important \n ");
  puts("**** Accepted Sample Requests **** \n");
  puts("GET_DATE %S:%H \n");
  puts("     GET_DATE       %S::%H/%M-%S \n");
  puts("**** Incorrect Sample Requests **** \n");
  puts("EMPTY \n");
  puts("GET_DATE \n ");
  puts("GET_DATE %E \n");
  puts("Full of space character \n ");
  puts("%S \n");
  puts("get %S \n");
  puts("GET_DATE %S %H \n");
  puts("GET_DATE %S:%E \n");

  while (1) {
    write(new_socket, "\n", strlen("\n"));
    write(new_socket, "\nENTER DATE REQUEST :", strlen("\nENTER DATE REQUEST :"));

    read(new_socket, str, 1000); // take input from user

    str[strcspn(str, "\r\n")] = 0; // \n at the end of user input is a problem in format checking algorithm , so it removed from the input string      

    int i;
    int len1;
    len1 = strlen(str);
    int fullSpace = 0;
    for (i = 0; i < len1; ++i) { //check if input full of space (" ") character
      if (str[i] == ' ')
        fullSpace++;
    }

    if (fullSpace == len1) { // check if input has only space(" ") character

      write(new_socket, "\nINCORRECT REQUEST ", strlen("\nINCORRECT REQUEST "));

    } else {
      char * pPosition = strchr(str, ' ');
      if (pPosition == NULL) { // check if input has only 1 string ,  like "GET_DATE"  or "%S"

        write(new_socket, "\nINCORRECT REQUEST ", strlen("\nINCORRECT REQUEST "));
      } else {
        int i = 0;
        char * p = strtok(str, " ");
        char * array[3] = {
          NULL
        };

        while (p != NULL) {
          array[i++] = p;
          p = strtok(NULL, " ");
        }

        int boolean = 1;
        for (i = 0; i < 3; ++i) {
          if (array[i] == NULL) {
            boolean = 0;
          }
        }

        if (boolean) { // check if input has more than 3 arguments  like "GET_DATE %S  %e"                 

          write(new_socket, "\nINCORRECT REQUEST ", strlen("\nINCORRECT REQUEST "));
        } else {

          if (array[1] == NULL) { // check if input has  the "%S:%H" type of date request part

            write(new_socket, "\nINCORRECT REQUEST ", strlen("\nINCORRECT REQUEST "));
          } else {
            // if there is 2 arguments , check if  arg1 is GET_DATE and arg2 is in correct format

            char GET_DATE[] = {
              "GET_DATE"
            };
            if (!strcmp(GET_DATE, array[0])) {
              char d[1000];
              strcpy(d, array[1]);

              int lenDeneme = strlen(d);
              int i;

              int boolean2 = 1;
              for (i = 0; i < lenDeneme; i++) { // check the format

                if (d[i] == '%' && d[i + 1] != ':') {
                  if (d[i + 1] == '%' || d[i + 1] == 'A' || d[i + 1] == 'a' || d[i + 1] == 'B' || d[i + 1] == 'b' || d[i + 1] == 'C' || d[i + 1] == 'c' || d[i + 1] == 'D' || d[i + 1] == 'd' || d[i + 1] == 'e' || d[i + 1] == 'F' ||
                    d[i + 1] == 'g' || d[i + 1] == 'G' || d[i + 1] == 'h' || d[i + 1] == 'H' || d[i + 1] == 'I' || d[i + 1] == 'j' || d[i + 1] == 'k' || d[i + 1] == 'l' || d[i + 1] == 'm' || d[i + 1] == 'M' || d[i + 1] == 'n' ||
                    d[i + 1] == 'N' || d[i + 1] == 'p' || d[i + 1] == 'P' || d[i + 1] == 'q' || d[i + 1] == 'r' || d[i + 1] == 'R' || d[i + 1] == 's' || d[i + 1] == 'S' || d[i + 1] == 't' || d[i + 1] == 'T' || d[i + 1] == 'u' ||
                    d[i + 1] == 'U' || d[i + 1] == 'V' || d[i + 1] == 'w' || d[i + 1] == 'W' || d[i + 1] == 'x' || d[i + 1] == 'X' || d[i + 1] == 'y' || d[i + 1] == 'Y' || d[i + 1] == 'Z' || d[i + 1] == 'z') {
                    i++;
                  } else {
                    boolean2 = 0;
                  }

                } else if (d[i] == '/' || d[i] == '-' || d[i] == ':') { // seperate chars are accepted   

                } else if (d[i] == '%' && d[i + 1] == ':') { // %:z , %::z , %:::z  special format check
                  if (d[i + 2] == 'z') {
                    i = i + 2;
                  } else if (d[i + 2] == ':' && d[i + 3] == 'z') {
                    i = i + 3;
                  } else if (d[i + 2] == ':' && d[i + 3] == ':' && d[i + 4] == 'z') {
                    i = i + 4;
                  }
                } else {
                  boolean2 = 0;
                }
              }
              if (boolean2) { // print date to user

                char date[] = {"date +"};
                strcat(date, d);
                char buffer[1000];
                FILE * file = popen(date, "r");
                fgets(buffer, 1000, file);
                pclose(file);
                write(new_socket, "\n", strlen("\n"));
                write(new_socket, "SERVER’S RESPONSE :  ", strlen("SERVER’S RESPONSE :  "));
                write(new_socket, buffer, strlen(buffer));

              } else { // check if date format is correct
                write(new_socket, "\nINCORRECT REQUEST ", strlen("\nINCORRECT REQUEST "));

              }

            } else { // check if request has GET_DATE command 
              write(new_socket, "\nINCORRECT REQUEST ", strlen("\nINCORRECT REQUEST "));
            }
          }

        }
      }
    }

  } // end of date calculations ,  since i wanted the server live there is no  connection close

  return 0;
}

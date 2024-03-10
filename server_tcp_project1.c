/***************Server program**************************/

/* server_tcp.c is on eros.cs.txstate.edu
   compile and run the server program first:
   $gcc -o s server_tcp_project1.c
   $./s 12000
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>

struct Student {
    int ID;
    char Fname[10];
    char Lname[10];
    int score;
};

int main(int argc, char **argv){
    int welcomeSocket, newSocket;
    char buffer[2048];
    /* sockaddr_in is an IPv4-specific address structure used for storing internet addresses. */
    struct sockaddr_in serverAddr;
    /* sockaddr_storage is a generic address structure used for storing addresses of various types, such as IPv4 and IPv6. */
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    unsigned short port; /* port server binds to */

    int userInput;
    int ID, cID;
    char Fname[10];
    char Lname[10];
    int score, cscore;

    uint32_t num, FnameSize, LnameSize;
    char msg[1024];

    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
    
    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;

    /* Set port number, using htons function to use proper byte order */
    port = (unsigned short) atoi(argv[1]);
    serverAddr.sin_port = htons(port);

    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

    /*---- Bind the address struct to the socket ----*/
    bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    /*---- Listen on the socket, with 5 max connection requests queued ----*/
    if(listen(welcomeSocket,5)==0)
        printf("Listening\n");
    else
        printf("Error\n");

    /*---- Accept call creates a new socket for the incoming connection ----*/
    addr_size = sizeof serverStorage;
    newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

    // Create a text file to serve as student database
    FILE *databaseFile = fopen("student_database.txt", "a+");

    if (databaseFile == NULL)
    {
        perror("Error opening the database file");
        exit(EXIT_FAILURE);
    }

    // communication starts from here

    // receive an integer from the client
    recv(newSocket, &num, sizeof(num), 0);
    printf("Integer received: %d\n",ntohl(num));   

    // send a reply message to the client
    strcpy(msg, "Integer received");
    send(newSocket, msg, sizeof(msg), 0);

    // receive menu option from the client
    recv(newSocket, &userInput, sizeof(userInput), 0);
    printf("User selection: %d\n",ntohl(userInput)); 

    // send a reply message to the client
    strcpy(msg, "User selection received");
    send(newSocket, msg, sizeof(msg), 0);

    while (userInput != 6)
    {
        if (ntohl(userInput) == 1)
        {
            // receive student information
            recv(newSocket, &ID, sizeof(ID), 0);
            printf("ID: %d\n",ntohl(ID));   
            strcpy(msg, "ID received");
            send(newSocket, msg, sizeof(msg), 0);

            recv(newSocket, &FnameSize, sizeof(FnameSize), 0);
            recv(newSocket, Fname, FnameSize, 0);
            Fname[FnameSize] = '\0';
            printf("Fname: %s\n",Fname);   
            strcpy(msg, "Fname received");
            send(newSocket, msg, sizeof(msg), 0);

            recv(newSocket, &LnameSize, sizeof(LnameSize), 0);
            recv(newSocket, Lname, LnameSize, 0);
            Lname[LnameSize] = '\0';
            printf("Lname: %s\n",Lname);   
            strcpy(msg, "Lname received");
            send(newSocket, msg, sizeof(msg), 0);

            recv(newSocket, &score, sizeof(score), 0);
            printf("score: %d\n",ntohl(score));   
            strcpy(msg, "score received");
            send(newSocket, msg, sizeof(msg), 0);

            // write student into the database file
            struct Student newStudent;
            newStudent.ID = ntohl(ID);
            strncpy(newStudent.Fname, Fname, sizeof(newStudent.Fname));
            strncpy(newStudent.Lname, Lname, sizeof(newStudent.Lname));
            newStudent.score = ntohl(score);
            fprintf(databaseFile, "%d %s %s %d\n", newStudent.ID, newStudent.Fname, newStudent.Lname, newStudent.score);
            fflush(databaseFile);

            // send a reply message to the client
            strcpy(msg, "New student information has been added to the database");
            send(newSocket, msg, sizeof(msg), 0);
        }
        else if (ntohl(userInput) == 2)
        {
            // receive ID from the client
            recv(newSocket, &ID, sizeof(ID), 0);
            cID = ntohl(ID);
            printf("ID: %d\n", cID);
            strcpy(msg, "ID received");
            send(newSocket, msg, sizeof(msg), 0);

            // search for the student with matching ID 
            fseek(databaseFile, 0, SEEK_SET);
            struct Student currentStudent;
            int targetID = cID;
            int found = 0;
            while (fscanf(databaseFile, "%d %s %s %d", &currentStudent.ID, currentStudent.Fname, currentStudent.Lname, &currentStudent.score) == 4)
            {
                if (currentStudent.ID == targetID)
                {
                    found = 1;
                    break;
                }
            }

            // send student information or a flag indicating not found
            if (found)
            {
                send(newSocket, &currentStudent, sizeof(currentStudent), 0);
            }
            else
            {
                struct Student notFoundStudent;
                notFoundStudent.ID = -1;
                send(newSocket, &notFoundStudent, sizeof(notFoundStudent), 0);
            }
        }
        else if (ntohl(userInput) == 3) 
        {
            // receive score from the client
            recv(newSocket, &score, sizeof(score), 0);
            printf("score: %d\n",ntohl(score));   
            strcpy(msg, "score received");
            send(newSocket, msg, sizeof(msg), 0);

            // send information of all those students found to the client
            struct Student currentStudent;
            int targetScore = ntohl(score);
            int found = 0;
            fseek(databaseFile, 0, SEEK_SET);

            while (fscanf(databaseFile, "%d %s %s %d", &currentStudent.ID, currentStudent.Fname, currentStudent.Lname, &currentStudent.score) == 4)
            {
                if (currentStudent.score > targetScore)
                {
                    send(newSocket, &currentStudent, sizeof(currentStudent), 0);
                    found = 1;
                }
            }

            if (!found)
            {
                struct Student noDataFlag;
                noDataFlag.ID = -2;
                send(newSocket, &noDataFlag, sizeof(noDataFlag), 0);
            }
            else
            {
                struct Student endFlag;
                endFlag.ID = -1;
                send(newSocket, &endFlag, sizeof(endFlag), 0);
            }
        }
        else if (ntohl(userInput) == 4)
        {
            // send information of all the students
            struct Student currentStudent;
            fseek(databaseFile, 0, SEEK_SET);
            while (fscanf(databaseFile, "%d %s %s %d", &currentStudent.ID, currentStudent.Fname, currentStudent.Lname, &currentStudent.score) == 4)
            {
                send(newSocket, &currentStudent, sizeof(currentStudent), 0);
            }

            struct Student endFlag;
            endFlag.ID = -1;
            send(newSocket, &endFlag, sizeof(endFlag), 0);
        }
        else if (ntohl(userInput) == 5)
        {
            // receive ID from the client
            recv(newSocket, &ID, sizeof(ID), 0);
            printf("ID: %d\n",ntohl(ID)); 

            // send a reply message to the client
            strcpy(msg, "ID received");
            send(newSocket, msg, sizeof(msg), 0);

            // create a temporary file to store the updated database
            FILE *tempFile = fopen("temp_database.txt", "w");
            if (tempFile == NULL)
            {
                perror("Error opening the temporary file");
                exit(EXIT_FAILURE);
            }

            struct Student currentStudent;
            int targetID = ntohl(ID);
            int found = 0;
            
            fseek(databaseFile, 0, SEEK_SET);
            while (fscanf(databaseFile, "%d %s %s %d", &currentStudent.ID, currentStudent.Fname, currentStudent.Lname, &currentStudent.score) == 4)
            {
                if (currentStudent.ID == targetID)
                {
                    found = 1;
                    continue;
                }

                fprintf(tempFile, "%d %s %s %d\n", currentStudent.ID, currentStudent.Fname, currentStudent.Lname, currentStudent.score);
            }

            fclose(tempFile);
            fclose(databaseFile);

            remove("student_database.txt");

            if (rename("temp_database.txt", "student_database.txt") != 0)
            {
                perror("Error renaming the temporary file");
                exit(EXIT_FAILURE);
            }

            databaseFile = fopen("student_database.txt", "a+");

            if (databaseFile == NULL)
            {
                perror("Error opening the database file");
                exit(EXIT_FAILURE);
            }

            // send a reply message to the client
            if (found)
                strcpy(msg, "Student information deleted from the database");
            else
                strcpy(msg, "Student with that ID was not found");

            send(newSocket, msg, sizeof(msg), 0);
        }

        recv(newSocket, &userInput, sizeof(userInput), 0);
        printf("User selection: %d\n",ntohl(userInput)); 

        strcpy(msg, "User selection received");
        send(newSocket, msg, sizeof(msg), 0);
    }
    
    fclose(databaseFile);
    close(newSocket);
    close(welcomeSocket);

    return 0;
}

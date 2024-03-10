/****************Client program********************************/

/* client_tcp.c is on zeus.cs.txstate.edu
   compile:
   $gcc -o c client_tcp_project1.c
   $./c eros.cs.txstate.edu 12000
*/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

int printMenu();
void sendCommand(int userInput);

int clientSocket;
int userInput, cuserInput;
int ID, cID;
char Fname[10];
char Lname[10];
int score, cscore;
char msg[1024];

struct Student {
    int ID;
    char Fname[10];
    char Lname[10];
    int score;
};

int main(int argc, char **argv){
    char buffer[2048];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    unsigned short port;
    struct hostent *hostnm;

    uint32_t num, cnum;


    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    
    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;

    /* set port number*/ 
    port = (unsigned short)atoi(argv[2]);

    /* htons() stands for "host to network short" and is used to convert the port number stored in the "port" variable from host byte order to network byte order, ensuring that data is not misinterpreted when sent over the network. */
    serverAddr.sin_port = htons(port);

    /* Set IP address to localhost */
    hostnm = gethostbyname(argv[1]);

    /* This sets the serverAddr structure's sin_addr member to the host address provided by the hostnm->h_addr variable. The s_addr field contains the IP address of the host in network byte order, and the *((unsigned long *)hostnm->h_addr) expression casts the data to an unsigned long type. This ensures that the IP address taken from hostnm is formatted correctly for network communication. */
    serverAddr.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr); 

    /* Set all bits of the padding field to 0. It is used to ensure that the serverAddr structure is correctly zero initialized before use, which is necessary for certain network operations. */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

    /*---- Connect the socket to the server using the address struct ----*/
    addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);


    // communication starts from here

    // send an integer to the server
    printf("To establish communication, enter an integer: ");
    scanf("%d", &num);

    /* htonl stands for "host to network long" and is a function used in networking applications to convert values from host byte order to network byte order.*/
    cnum = htonl(num);
    send(clientSocket, &cnum, sizeof(cnum), 0);

    // receive a reply message from the server
    recv(clientSocket, msg, sizeof(msg), 0);
    printf("%s\n", msg);

    // print menu 
    userInput = printMenu();

    // send user input
    cuserInput = htonl(userInput);
    send(clientSocket, &cuserInput, sizeof(cuserInput), 0);

    // receive a reply message from the server
    recv(clientSocket, msg, sizeof(msg), 0);
    printf("%s\n", msg);

    while (userInput != 6)
    {
        // send command to server and receive message
        sendCommand(userInput);

        userInput = printMenu();
        cuserInput = htonl(userInput);
        send(clientSocket, &cuserInput, sizeof(cuserInput), 0);

        recv(clientSocket, msg, sizeof(msg), 0);
        printf("%s\n", msg);
    }

    close(clientSocket);

    return 0;
}

int printMenu()
{
    int userChoice;

    printf("\n\nStudent Database Options Menu\n");
    printf("~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*\n");
    printf("1. add(ID, Fname, Lname, score): this request adds new student information to the database.\n");
    printf("2. display(ID): this request sends the ID of a student to the server, and the server returns the student's information to the client.\n");
    printf("3. display(score): this request sends a score to the server, and the server returns the information of all students whose scores are above the sent score to the client.\n");
    printf("4. display_all: this request displays the information of all the students currently in the database.\n");
    printf("5. delete(ID): this request deletes the student entry with that ID from the database.\n");
    printf("6. exit: this request terminates the program.\n\n");

    printf("Enter the number associated with the command you wish to perform: ");
    scanf("%d", &userChoice);

    return userChoice;
}

void sendCommand(int userInput)
{
    if (userInput == 1)
    {
        // user input student information
        printf("You chose add, enter ID: ");
        scanf("%d", &ID);
        cID = htonl(ID);
        send(clientSocket, &cID, sizeof(cID), 0);
        recv(clientSocket, msg, sizeof(msg), 0);
        printf("%s\n", msg);

        printf("Enter Fname: ");
        scanf("%s", &Fname);
        uint32_t FnameSize = (strlen(Fname));
        send(clientSocket, &FnameSize, sizeof(FnameSize), 0);
        send(clientSocket, Fname, strlen(Fname), 0);
        recv(clientSocket, msg, sizeof(msg), 0);
        printf("%s\n", msg);

        printf("Enter Lname: ");
        scanf("%s", &Lname);
        uint32_t LnameSize = (strlen(Lname));
        send(clientSocket, &LnameSize, sizeof(LnameSize), 0);
        send(clientSocket, Lname, strlen(Lname), 0);
        recv(clientSocket, msg, sizeof(msg), 0);
        printf("%s\n", msg);

        printf("Enter score: ");
        scanf("%d", &score);
        cscore = htonl(score);
        send(clientSocket, &cscore, sizeof(cscore), 0);
        recv(clientSocket, msg, sizeof(msg), 0);
        printf("%s\n", msg);

        //receive confirmation that student was added
        recv(clientSocket, msg, sizeof(msg), 0);
        printf("%s\n", msg);
    }
    else if (userInput == 2)
    {
        // user input ID
        printf("You chose display(ID), enter ID: ");
        scanf("%d", &ID);
        cID = htonl(ID);
        send(clientSocket, &cID, sizeof(cID), 0);
        recv(clientSocket, msg, sizeof(msg), 0);
        printf("%s\n", msg);

        // receive student information from the server
        struct Student receivedStudent;
        recv(clientSocket, &receivedStudent, sizeof(receivedStudent), 0);

        if (receivedStudent.ID != -1)
        {
            printf("Student Information:\n");
            printf("%d %s %s %d\n", receivedStudent.ID, receivedStudent.Fname, receivedStudent.Lname, receivedStudent.score);
        }
        else
        {
            printf("Student with ID %d not found.\n", ID);
        }
    }
    else if (userInput == 3)
    {   
        // user input score
        printf("You chose display(score), enter score: ");
        scanf("%d", &score);
        cscore = htonl(score);
        send(clientSocket, &cscore, sizeof(cscore), 0);
        recv(clientSocket, msg, sizeof(msg), 0);
        printf("%s\n", msg);     

        // receive and display student information from the server
        struct Student receivedStudent;
        recv(clientSocket, &receivedStudent, sizeof(receivedStudent), 0);

        if (receivedStudent.ID == -2)
        {
            printf("No students found with a score higher than %d\n", score);
        }
        else
        {
            printf("Student Information:\n");
            while (receivedStudent.ID != -1)
            {
                printf("%d %s %s %d\n", receivedStudent.ID, receivedStudent.Fname, receivedStudent.Lname, receivedStudent.score);
                recv(clientSocket, &receivedStudent, sizeof(receivedStudent), 0);
            }
        }
    }
    else if (userInput == 4)
    {
        // receive and display all student information
        struct Student receivedStudent;
        recv(clientSocket, &receivedStudent, sizeof(receivedStudent), 0);
        printf("Student Information:\n");
        while (receivedStudent.ID != -1)
        {
            printf("%d %s %s %d\n", receivedStudent.ID, receivedStudent.Fname, receivedStudent.Lname, receivedStudent.score);
            recv(clientSocket, &receivedStudent, sizeof(receivedStudent), 0);
        }
    }
    else if (userInput == 5)
    {
        // user input ID
        printf("You chose delete, enter ID: ");
        scanf("%d", &ID);
        cID = htonl(ID);
        send(clientSocket, &cID, sizeof(cID), 0);
        recv(clientSocket, msg, sizeof(msg), 0);
        recv(clientSocket, msg, sizeof(msg), 0);
        printf("%s\n", msg);
    }
    else
    {
        printf("That is not a valid menu selection. Please enter 1-6.");
    }
}
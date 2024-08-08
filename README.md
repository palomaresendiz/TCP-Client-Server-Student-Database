# TCP Client-Server Program For a Student Database

####

A client-server program written in C to serve as communication to edit and display a student database in the CLI.

The client can send the following requests to the server:
1. add(ID, Fname, Lname, score): this request adds new student information to the database
2. display(ID): this request sends the ID of a student to the server, and the server returns the
student’s information to the client
3. display(score): this request sends a score to the server, and the server returns the information of
all students whose scores are above the sent score to the client
4. display_all: this request displays the information of all the students currently in the database
5. delete(ID): this request deletes the student entry with that ID from the database
6. exit: this request terminates the program

The server processes the requests and manages the student database through a .txt file, sending several response messages throughout the process.
## Screenshots

#### 
The client displays the menu and the user inputs the selection 1, to add a student to the database
<img width="750" alt="add student" src="https://github.com/palomaresendiz/TCP-Client-Server-Student-Database/assets/127056165/1e09736d-9708-4835-9b97-86589a99eb07">

After adding more students to the database, the client sends a request to display them
<img width="495" alt="display students" src="https://github.com/palomaresendiz/TCP-Client-Server-Student-Database/assets/127056165/2bc796e9-3ff1-4b52-8584-da6e48d1d89f">

The server program processing the requests  
<img width="238" alt="server side" src="https://github.com/palomaresendiz/TCP-Client-Server-Student-Database/assets/127056165/2d6d96f6-fe5d-4fbd-b9b4-7f07df0a97b3">

## Setup and Execution Instructions

####  

Clone down this repository. This program can be run in the CLI using two distinct networks. I am using `zeus.cs.txstate.edu` and `eros.cs.txstate.edu` in this example.

Compile the client file: 

`$gcc -o c client_tcp_project1.c` 

Compile the server file: 

`$gcc -o s server_tcp_project1.c`

Run the server file first: 

`$./s 12000`

Run the client file second: 

`$./c eros.cs.txstate.edu 12000`

## Reflection

####

This started as a project for my Computer Networks class that I decided to build upon. The original project only required a smaller list of requests so I decided to add more. This project was my introduction to TCP and setting up communication between a client and a server program.

While developing this student database program, I learned the intricacies of socket programming and the establishment of communication between a client and server using TCP. Understanding the nuances of setting up sockets, configuring server addresses, and managing the flow of messages between the two entities was crucial. I practiced converting data between host and network byte orders, utilizing functions like htonl and ntohl to ensure proper data interpretation. I gained experience in debugging and refining the communication process because I encountered a lot of errors along the way. 

My biggest challenge in this project was coordinating the messages between the client and the server. In the beginning, my server was sending messages out of order, or so I thought. It was because I didn't organize receiving the messages on the client side or I was missing some receive message functions. I had to map out all the messages being sent and received on a piece of paper to visualize the communication and deeply understand how the transmission between a client and a server works. 

Overall, this project enhanced my comprehension of TCP communication and the programming language C, laying a foundation for future endeavors in network programming.



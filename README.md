# TCP Client-Server Program For a Student Database

####

A client-server program written in C to serve as communication to edit and display a student database.

The client can send the following requests to the server:
1. add(ID, Fname, Lname, score): this request adds new student information to the database
2. display(ID): this request sends the ID of a student to the server, and the server returns the
student’s information to the client
3. display(score): this request sends a score to the server, and the server returns the information of
all students whose scores are above the sent score to the client
4. display_all: this request displays the information of all the students currently in the database
5. delete(ID): this request deletes the student entry with that ID from the database
6. exit: this request terminates the program

The server processes the requests and sends several responses along the way to verify communication.

## Screenshots

####

Screenshots Here

## Setup and Execution Instructions

####  

Clone down this repository. This program can be run completely in the CLI using a network host. I am using `eros.cs.txstate.edu` in this example.

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

Reflection here of what I learned


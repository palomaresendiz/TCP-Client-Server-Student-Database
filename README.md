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

This started as a project for my Computer Networks class that I decided to build upon.
The original project only required a smaller list of requests so I decided to add more. This project was my introduction to TCP and setting up communication between a client and a server program.

I learned (sockets, sending and receving messages)

My biggest challenge in this project was coordinating the messages between the client and the server. In the beginning, my server was sending messages out of order, or so I thought. It was because I didn't organize receiving the messages on the client or I was missing some receive message functions. I had to map out all the messages being sent and received on a piece of paper to visualize the communication and deeply understand how a client-server program works. 


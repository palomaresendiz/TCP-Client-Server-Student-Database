# TCP Client-Server Program 

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

## Installation and Setup Instructions

####  

Clone down this repository. You will need a network host. I am using 'zeus.cs.txstate.edu' and 'eros.cs.txstate.edu' in this example 

Verify Flutter is Ready:

`flutter doctor`  

Open Simulator:  

`open -a simulator`  

To Start Project:

`flutter run`  

## Reflection

####

This was a 2-week long personal project aimed to familiarize myself with building mobile apps. I was inspired to create this timer app because the functionality is something I wanted to use in my daily life. Project goals included becoming skilled at using the Flutter framework and learning a new programming language, Dart.

One of the main obstacles I ran into was adding sound. The current audio plugin, audioplayers, had some bugs that weren't allowing me to play sound effects. Ultimately, I had to upgrade the plugin to audioplayers 5.2.1 to get the sound to work. I learned to inspect and configure the pubspec.yaml because of this hurdle.

I chose to work with the open source framework, Flutter because it is growing in popularity and I wanted to challenge myself by developing my app in Dart, a language I hadn't used before. I gained experience in setting up an SDK, configuring plugins and designing pages for a mobile app.


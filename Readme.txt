YAMOTD Client-MultiServer Program
		written by:
Grp# 5: Saad Ahmad & Shahriyar Chowdhury
		submitted to:
Dr. Jinhua Guo
		for the course:
CIS 427

Project - 2

README file

This project is written by Saad Ahmad and Shahriyar Chowdhury, student at
University of Michigan for the CIS 427 - Computer Networks course. This
project has two programs; sclient and multiThreadServer, which work in coordination of
one another. The server program should be running before the client is executed.
The client program requires an IP address as a commandline argument of the server.

How to Compile:
1. Create a directory for the programs.
2. When creating a directory, make sure it has the following files:
	sclient.c
	multiThreadServer.c
	Makefile
3. Go to the directory in terminal and type in "make" command.
4. It should output: 
	g++ -g -c sclient.c
	g++  -o sclient sclient.o
	g++ -g -c multiThreadServer.c
	g++ -lpthread  -o multiThreadServer multiThreadServer.o
5. Now you can run the programs.

How to run:
1. Open a terminal window.
2. In the first terminal window, type in the command, "./multiThreadServer"
3. Open another terminal window.
4. In the second terminal window, type in the next command: "./client 127.0.0.1"
5. Go ahead and try these commands

Commands: 
1. MSGGET -or- W(shortcut character)
This command from client sends a message to server to return the
message of the day. The server has access to file messages.txt and 
returns each message in that file in chronological order.
Ex) MSGGET
	S: Message of the day

2. SHUTDOWN
It will allow the root user at client side to shutdown the server.
Additionally, the client and server program stops execution.
ONLY ROOT has access to shutdown the server. The reply from server
comes to each connected user.
Ex) SHUTDOWN
    S: 200 OK Server is about to shutdown
	
3. LOGIN username password
It will allow the user at client side to log in to access the server.
The login command is followed by username and password. If the credentials
are authenticated, the user logins, otherwise, wrong username/ password.
Ex) LOGIN john john01
    S: 200 OK john logged in

4. LOGOUT -or- V(shortcut character)
It will allow the logged in user to logout from the server. If the user
is not logged in, it says no users logged in
Ex) LOGOUT
    S: 200 OK

5. QUIT 
It will terminate the client connection from server. The server will still
remain awake and will accept other connections from the client.
Ex) QUIT
    S: 200 OK

6. MSGSTORE message
It will allow the user to upload one message to the server. The server will save
the message to the messages.txt file. The limit for messages in 20.
Ex) MSGSTORE
	S: 200 OK, Type message...
    message
	S : 200 OK
	
7. SEND username
It will allow a logged in user to send a message to a logged in user one
the other terminal. Both users MUST be logged in order to send receive the
message.
Ex) SEND john
	S: 200 OK, Type message...
	Hey John
	S: 200 OK
	
	*** On John's Window ***
	MSG: Hey John

8. WHO
It displays the all the terminal's IP addresses who attempted to connect
to the multiThreadServer since the server started
Ex) WHO
	200 OK
	The list of users
	127.0.0.1

CAUTION:
	The commands DO WORK as mentioned in this document
but sometimes SEND **** don't acknowledge to the same client most specifically
for socket 7, 9 I don't know why. For 4, 5 sockets it acknowledges perfectly. 
That is SOMETIMES the output is not displayed on the client. That's the only issue.

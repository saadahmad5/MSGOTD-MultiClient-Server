/*
 * multiThreadServer.c -- a multithreaded server
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <cstring>
#include <cstdlib>
#include <fstream>

using namespace std;

#define PORT 3542  // port we're listening on
#define MAX_LINE 256

fd_set master;   // master file descriptor list
int listener;    // listening socket descriptor
int fdmax;

// the child thread
void *ChildThread(void *newfd) {
    char buf[MAX_LINE];
    int nbytes;
    
    long childSocket = (long) newfd;
	
	int i = 0;
	int itotal = 0;
	string messages[20];
	string quit = "quit";
	string msgget = "msgget";
	bool isJlogin = false;
	bool isMlogin = false;
	bool isDlogin = false;
	bool isRlogin = false;
	string sendjohn = "send john";
	string sendmary = "send mary";
	string senddavid = "send david";
	string sendroot = "send root";
	string login = "login";
	string logout = "logout";
	string stdown = "shutdown";
	string msgstore = "msgstore";
	bool shutdown = false;
	// Credentials
	string loginAcc1 = "login root root01";
	string loginAcc2 = "login john john01";
	string loginAcc3 = "login david david01";
	string loginAcc4 = "login mary mary01";
	
	ifstream ifile;
	ofstream ofile;
	
	// File read and store messages from
	ifile.open("messages.txt");
	int j = 0;
	if(ifile.is_open())
	{
		getline(ifile, messages[j]);
		messages[j] += '\n';
		messages[j] += '\0';
		while(ifile)
		{	
			++j;
			++itotal;
			getline(ifile, messages[j]);
			messages[j] += '\n';
			messages[j] += '\0';
		}
	}
	ifile.close();
	
	// File write and store messages to
	
	ofile.open("messages.txt", ios::out | ios::app);
	
	string temp;
	
    while(1) {
        // handle data from a client
        
		if ( (nbytes = recv(childSocket, buf, sizeof(buf), 0)) <= 0) {
            // got error or connection closed by client
            if (nbytes == 0) {
                // connection closed
                cout << "multiThreadServer: socket " << childSocket <<" hung up" << endl;
            } else {
                perror("recv");
            }
            close(childSocket); // bye!
            FD_CLR(childSocket, &master); // remove from master set
            pthread_exit(0);
        } else {
            // we got some data from a client
            cout << "Data: " << buf ;

			for(j = 0; j <= fdmax; j++) {
                // send to everyone!
                if (FD_ISSET(j, &master)) 					
				{				
					if (j != listener && j != childSocket) // except the listener and ourselves
					{
						/*temp = "The other window\n";
						strcpy(buf,temp.c_str());
						if (send(j, buf, sizeof(buf), 0) == -1) 
							perror("send");*/
					}
					if(j == childSocket) // for ourselves except listener
					{
						if(strcmp(buf, msgget.c_str()) == 10)
						{
							temp = messages[i];
							temp += "\n";
							strcpy(buf, temp.c_str());
							i++;
							if(i == itotal)
								i = 0;
							if (send(j, buf, sizeof(buf), 0) == -1) 
							perror("send");
						}
						/*temp = "The same window\n";
						strcpy(buf,temp.c_str());
						if (send(j, buf, sizeof(buf), 0) == -1) 
							perror("send");*/
					}
				}
            }
        }
    }
}


int main(void)
{

	
    struct sockaddr_in myaddr;     // server address
    struct sockaddr_in remoteaddr; // client address
    int newfd;        // newly accept()ed socket descriptor
    int yes=1;        // for setsockopt() SO_REUSEADDR, below
    socklen_t addrlen;

    pthread_t cThread;

    FD_ZERO(&master);    // clear the master and temp sets

    // get the listener
    if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // lose the pesky "address already in use" error message
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    // bind
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = INADDR_ANY;
    myaddr.sin_port = htons(PORT);
    memset(&(myaddr.sin_zero), '\0', 8);
    if (bind(listener, (struct sockaddr *)&myaddr, sizeof(myaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    // listen
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(1);
    }

    // add the listener to the master set
    FD_SET(listener, &master);

    // keep track of the biggest file descriptor
    fdmax = listener; // so far, it's this one

    addrlen = sizeof(remoteaddr);

    // main loop
    for(;;) {
        // handle new connections
        if ((newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen)) == -1) {
            perror("accept");
	        exit(1);
        } else {
            FD_SET(newfd, &master); // add to master set
            cout << "multiThreadServer: new connection from "
		 		 << inet_ntoa(remoteaddr.sin_addr)
                 << " socket " << newfd << endl;

            if (newfd > fdmax) {    // keep track of the maximum
                fdmax = newfd;
            }

	        if (pthread_create(&cThread, NULL, ChildThread, (void *) newfd) <0) {
                perror("pthread_create");
                exit(1);
            }
        }

    }
    return 0;
}


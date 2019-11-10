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

void substring(char s[], char sub[], int p, int l) {
   int c = 0;
   
   while (c < l) {
      sub[c] = s[p+c-1];
      c++;
   }
   sub[c] = '\0';
}

// the child thread
void *ChildThread(void *newfd) {
    char buf[MAX_LINE];
    int nbytes;
    
    long childSocket = (long) newfd;
	
	int i = 0;
	int itotal = 0;
	string messages[20];
	string quit = "QUIT";
	string msgget = "MSGGET";
	bool isJlogin = false;
	bool isMlogin = false;
	bool isDlogin = false;
	bool isRlogin = false;
	string sendjohn = "SEND john";
	string sendmary = "SEND mary";
	string senddavid = "SEND david";
	string sendroot = "SEND root";
	string login = "LOGIN";
	string logout = "LOGOUT";
	string stdown = "SHUTDOWN";
	string msgstore = "MSGSTORE";
	bool getmessage = false;
	bool shutdown = false;
	//Who
	string who = 'WHO';
	// Credentials
	string loginAcc1 = "LOGIN root root01";
	string loginAcc2 = "LOGIN john john01";
	string loginAcc3 = "LOGIN david david01";
	string loginAcc4 = "LOGIN mary mary01";
	
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

			if(strcmp(buf, stdown.c_str()) == 10)	// SHUTDOWN
			{	
				if(isRlogin)
				{
					shutdown = true;
				}
			}
			
			for(j = 0; j <= fdmax; j++) {
                // send to everyone!
                if (FD_ISSET(j, &master)) 					
				{

					if(j == childSocket) // for ourselves except listener
					{
						
						if(strcmp(buf, stdown.c_str()) == 10) // SHUTDOWN
						{
							if(!isRlogin)
							{	
								temp = "402 User not allowed\n";
								strcpy(buf, temp.c_str());
							}
							if (send(j, buf, sizeof(buf), 0) == -1) 
									perror("send");
						}
						
						if(shutdown)	// SHUTDOWN
						{	
							temp = "200 OK Server is about to shutdown\n";
							strcpy(buf, temp.c_str());
							if (send(j, buf, sizeof(buf), 0) == -1) 
								perror("send");
						}
						
						if(getmessage)   // Only enabled when MSGSTORE is sent
						{
							getmessage = false;
							temp = buf;
							if(itotal < 20) 
							{
								messages[itotal] = temp;
								++itotal;
								ofile.write(temp.c_str(),strlen(temp.c_str()));
								temp = "200 OK\n";
								strcpy(buf, temp.c_str());
							}
							else
							{
								temp = "402 No more space max limit exceed\n";
								strcpy(buf, temp.c_str());
							}
							if (send(j, buf, sizeof(buf), 0) == -1) 
								perror("send");
						}
						
						if(strcmp(buf, msgget.c_str()) == 10) // MSGGET
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
						
						if(strcmp(buf, logout.c_str()) == 10)	// LOGOUT
						{
							temp = "401 No users logged in\n";
							if(isRlogin) {
								temp = "200 OK\n";
								isRlogin = false;
							} else if(isDlogin) {
								temp = "200 OK\n";
								isDlogin = false;
							} else if(isJlogin) {
								temp = "200 OK\n";
								isJlogin = false;
							} else if(isMlogin) {
								temp = "200 OK\n";
								isMlogin = false;
							}
							strcpy(buf, temp.c_str());
							if (send(j, buf, sizeof(buf), 0) == -1) 
								perror("send");
						}
						
						if(strcmp(buf, loginAcc1.c_str()) == 10) // LOGIN root root01
						{
							isRlogin = true;
							temp = "200 OK\n";
							strcpy(buf, temp.c_str());
							if (send(j, buf, sizeof(buf), 0) == -1) 
								perror("send");
						}
						else if(strcmp(buf, loginAcc2.c_str()) == 10) // LOGIN john john01
						{
							isJlogin = true;
							temp = "200 OK\n";
							strcpy(buf, temp.c_str());
							if (send(j, buf, sizeof(buf), 0) == -1) 
								perror("send");
						}
						else if(strcmp(buf, loginAcc2.c_str()) == 10) // LOGIN david david01
						{
							isDlogin = true;
							temp = "200 OK\n";
							strcpy(buf, temp.c_str());
							if (send(j, buf, sizeof(buf), 0) == -1) 
								perror("send");
						}
						else if(strcmp(buf, loginAcc2.c_str()) == 10) // LOGIN mary mary01
						{
							isMlogin = true;
							temp = "200 OK\n";
							strcpy(buf, temp.c_str());
							if (send(j, buf, sizeof(buf), 0) == -1) 
								perror("send");
						}
						else if(strcmp(buf, login.c_str()) == 32)	// LOGIN <anything else> unspecified
						{
							temp = "410 Wrong UserID or Password\n";
							strcpy(buf, temp.c_str());
							if (send(j, buf, sizeof(buf), 0) == -1) 
								perror("send");
						}
						
						if(strcmp(buf, msgstore.c_str()) == 10)	// MSGSTORE
						{
							if(isDlogin || isJlogin || isMlogin || isRlogin)
							{
								temp = "200 OK\n";
								strcpy(buf, temp.c_str());
								getmessage = true;
							}
							else
							{
								temp = "401 You are not currently logged in, login first\n";
								strcpy(buf, temp.c_str());
							}
							if (send(j, buf, sizeof(buf), 0) == -1) 
								perror("send");
						}
						
						
						/*temp = "The same window\n";
						strcpy(buf,temp.c_str());
						if (send(j, buf, sizeof(buf), 0) == -1) 
							perror("send");*/
					}
					
					if (j != listener && j != childSocket) // except the listener and ourselves
					{
						/*temp = "The other window\n";
						strcpy(buf,temp.c_str());
						if (send(j, buf, sizeof(buf), 0) == -1) 
							perror("send");*/
						
						if(shutdown)	// SHUTDOWN
						{	
							temp = "200 OK Server is about to shutdown\n";
							strcpy(buf, temp.c_str());
							if (send(j, buf, sizeof(buf), 0) == -1) 
								perror("send");
						}
						
					}
				}
            }
			if(shutdown)	// SHUTDOWN
			{
				exit(1);
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
		system("who");
    }
    return 0;
}


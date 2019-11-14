/*
 * sclient.c
 */

#include <iostream>
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>

using namespace std;

#define SERVER_PORT 1997
#define MAX_LINE 256
#define STDIN 0

void substring(char s[], char sub[], int p, int l) {
   int c = 0;
   
   while (c < l) {
      sub[c] = s[p+c-1];
      c++;
   }
   sub[c] = '\0';
}

int main(int argc, char * argv[]) {

    fd_set master;   // master file descriptor list
    fd_set read_fds; // temp file descriptor list for select()
    int fdmax;       // maximum file descriptor number

    struct sockaddr_in sin;
    char buf[MAX_LINE];
	char fbuf[MAX_LINE];
    int len;
    int s;
	bool strmsg = false;
	string temp;

	string quit = "QUIT";
	string msgget = "MSGGET";
	string login = "LOGIN";
	string logout = "LOGOUT";
	string shutdown = "SHUTDOWN";
	string msgstore = "MSGSTORE";
	string senduser = "SEND";
	string stdownmsg = "200 OK Server is about to shutdown\n";
	string windowRoot = "200 OK root logged in\n";
	string windowJohn = "200 OK john logged in\n";
	string windowDavid = "200 OK david logged in\n";
	string windowMary = "200 OK mary logged in\n";
	string msgsndjohn = "MSGSNDJOHN\n";
	string msgsndroot = "MSGSNDROOT\n";
	string msgsnddavid = "MSGSNDDAVID\n";
	string msgsndmary = "MSGSNDMARY\n";
	string who = "WHO";
	bool sndmsg = false;
	bool showmsgj = false;
	bool showmsgr = false;
	bool showmsgd = false;
	bool showmsgm = false;
	string loginAcc1 = "LOGIN root root01";
	string loginAcc2 = "LOGIN john john01";
	string loginAcc3 = "LOGIN david david01";
	string loginAcc4 = "LOGIN mary mary01";
	
	bool isRoot = false;
	bool isJohn = false;
	bool isDavid = false;
	bool isMary = false;
	
	string sendjohn = "SEND john";
	string sendroot = "SEND root";
	string senddavid = "SEND david";
	string sendmary = "SEND mary";

    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);

    /* active open */
    if ((s = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("select client: socket");
		exit(1);
    }

    /* build address data structure */
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr  = inet_addr(argv[1]);
    sin.sin_port = htons (SERVER_PORT);

    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		perror("select client: connect");
		close(s);
		exit(1);
    }
	
    // add the STDIN to the master set
    FD_SET(STDIN, &master);

    // add the listener to the master set
    FD_SET(s, &master);

    // keep track of the biggest file descriptor
    fdmax = s; // so far, it's this one
	
    /* main loop; get and send lines of text */
    while (1) {
		
        read_fds = master; // copy it
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(1);
        }

        // looking for data to read either from the server or the user
        if (FD_ISSET(STDIN, &read_fds)) {
			
			
			// handle the user input
			if (fgets(buf, sizeof(buf), stdin)){
				
				buf[MAX_LINE -1] = '\0';
				len = strlen(buf) + 1;
				
				// LOGIN
				if(strcmp(buf,loginAcc1.c_str()) == 10)
				{
					send(s, buf, len, 0);
				} else if(strcmp(buf,loginAcc2.c_str()) == 10)
				{
					send(s, buf, len, 0);
				} else if(strcmp(buf,loginAcc3.c_str()) == 10)
				{
					send(s, buf, len, 0);
				} else if(strcmp(buf,loginAcc4.c_str()) == 10)
				{
					send(s, buf, len, 0);
				}
				
				// WHO
		
				if(strcmp(buf, who.c_str()) == 10)
				{
					send (s, buf, len, 0);
				}
				
				// MSGGET
		
				if((strcmp(buf, msgget.c_str()) == 10) && (strlen(buf) == strlen(msgget.c_str())+1))
				{

					send (s, buf, len, 0);
				}
				
				// MSGSTORE
				
				if(strmsg)
				{

					strmsg = false;
					send (s, buf, len, 0);
				}
				
				if(strcmp(buf, msgstore.c_str()) == 10)
				{
					strmsg = true;
					send (s, buf, len, 0);
					
				}
				
				// QUIT
				
				if(strcmp(buf, quit.c_str()) == 10)
				{
					break;
				}
				
				// LOGOUT
		
				if(strcmp(buf, logout.c_str()) == 10)
				{
					send (s, buf, len, 0);
				}
				
				// SHUTDOWN
		
				if(strcmp(buf, shutdown.c_str()) == 10)
				{
					send (s, buf, len, 0);
				}
				
				// SEND 
				if(sndmsg)
				{
					sndmsg = false;
					send (s, buf, len, 0);
					cout << "S: 200 OK" << endl;
				}
				
				if(strcmp(buf, sendjohn.c_str()) == 10) 
				{
					sndmsg = true;
					send (s, buf, len, 0);
				}
				if(strcmp(buf, sendroot.c_str()) == 10) 
				{
					sndmsg = true;
					send (s, buf, len, 0);
				}
				if(strcmp(buf, senddavid.c_str()) == 10) 
				{
					sndmsg = true;
					send (s, buf, len, 0);
				}
				if(strcmp(buf, sendmary.c_str()) == 10) 
				{
					sndmsg = true;
					send (s, buf, len, 0);
				}
				
			} else {
				break;
            }
		}

		if (FD_ISSET(s, &read_fds)) {
			// handle data from the server
			if (recv(s, buf, sizeof(buf), 0) > 0) {
				cout.flush();
				
				if(showmsgj)
				{
					showmsgj = false;
					if(isJohn)
						cout << "MSG: " << buf;
				}
				else if(showmsgr)
				{
					showmsgr = false;
					if(isRoot)
						cout << "MSG: " << buf;
					
				}
				else if(showmsgd)
				{
					showmsgd = false;
					if(isDavid)
						cout << "MSG: " << buf;
				}
				else if(showmsgm)
				{
					showmsgm = false;
					if(isMary)
						cout << "MSG: " << buf;
				}
				
				else if(strcmp(buf,msgsndjohn.c_str()) == 0)
				{
					cout << "";
					
					showmsgj = true;
				}
				else if(strcmp(buf,msgsndroot.c_str()) == 0)
				{
					cout << "";
					
					showmsgr = true;
				}
				else if(strcmp(buf,msgsnddavid.c_str()) == 0)
				{
					cout << "";
					
					showmsgd = true;
				}
				else if(strcmp(buf,msgsndmary.c_str()) == 0)
				{
					cout << "";
					
					showmsgm = true;
				}
				else
					cout << "S: " << buf;
			
				
			
				if(strcmp(buf,windowJohn.c_str()) == 0)
				{
					cout << "";
					isJohn = true;
					
				}
				if(strcmp(buf,windowRoot.c_str()) == 0)
				{
					cout << "";
					isRoot = true;
					
				}
				if(strcmp(buf,windowDavid.c_str()) == 0)
				{
					cout << "";
					isDavid = true;
					
				}
				if(strcmp(buf,windowMary.c_str()) == 0)
				{
					cout << "";
					isMary = true;
					
				}
				
				
			}
			
			if(strcmp(buf,stdownmsg.c_str()) == 0)
			{
				break;
			}
				
        }
    }

    close(s);
}


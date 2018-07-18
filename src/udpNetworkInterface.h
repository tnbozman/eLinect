/*
 * udpNetworkInterface.h
 *
 *  Created on: 02/02/2012
 *      Author: troy
 */

#ifndef UDPNETWORKINTERFACE_H_
#define UDPNETWORKINTERFACE_H_

#include <stdio.h>
#include <stdlib.h>
// Sockets
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
using namespace std;

//====== set the host name and port here.
#define LISTENER_PORT  55555
#define buf_size 8192

class udpNetworkInterface{
  public:
	// Constructor
	udpNetworkInterface()	{
		for(uint i = 0; i < sizeof(buf); i++){
				   buf[i] = 0;
			   }
	}

	// Public Member Functions
	bool getHostByName(const char *host_name);
	bool sin_init(sockaddr_in *sin);
	bool createSocket();
	bool bindSocket();
	void setSinLength();
	bool recvFrom();
	void clearBuf();

	// Public Member Variables
	char buf[buf_size];
 	struct sockaddr_in sin ;

  private:

 	// Private Member Variables
 	int  socket_descriptor ;
 	uint sin_length ;
 	struct hostent *server_host_name ;
 	ssize_t recvfromRetVal;

};

#endif /* UDPNETWORKINTERFACE_H_ */

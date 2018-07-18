/*
 * udpNetworkInterface.cpp
 *
 *  Created on: 02/02/2012
 *      Author: troy
 */

#include "udpNetworkInterface.h"


bool udpNetworkInterface::getHostByName(const char *host_name){
	if ( (server_host_name = gethostbyname(host_name)) == 0){
		perror("   Error resolving local host\n\n") ;
		return false;
	}
	return true;
}

bool udpNetworkInterface::sin_init(sockaddr_in *sin){

	sin->sin_family = AF_INET ;
	sin->sin_addr.s_addr = htonl( INADDR_ANY) ;
	sin->sin_port = htons( LISTENER_PORT) ;
	return true;

}

bool udpNetworkInterface::createSocket(){

	if ((socket_descriptor = socket( AF_INET, SOCK_DGRAM, 0)) == -1){
		perror("  Opening of socket failed\n\n") ;
		return false;
	}
	return true;
}


bool udpNetworkInterface::bindSocket(){

	if ( (bind( socket_descriptor, (struct sockaddr *)&sin, sizeof(sin)))== -1 ){
		perror("   Binding socket failed\n\n") ;
		return false ;
	}
	return true;
}

void udpNetworkInterface::setSinLength(){
	sin_length = sizeof( sin_length) ;
}


bool udpNetworkInterface::recvFrom(){

	recvfromRetVal = recvfrom( socket_descriptor,  buf, sizeof( buf), MSG_DONTWAIT/*0*/,( struct sockaddr *)&sin, &sin_length) ;

	if(recvfromRetVal > 0)
		return true;
	else
		return false;
}

void udpNetworkInterface::clearBuf(){
	for(uint i = 0; i < sizeof(buf); i++){
		buf[i] = 0;
	}
}

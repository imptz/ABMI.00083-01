#include <iostream>

#include "dispatcherProtocol.h"
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

using namespace std;

int main(int argc, const char* argv[]) {
	int iSockFd=-1;
	int iLength=0;
	struct sockaddr_in servAddr,cliAddr;
	char buff[1024];

	iSockFd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

	memset(&servAddr,0,sizeof(servAddr));
	memset(&cliAddr,0,sizeof(cliAddr));

	servAddr.sin_family=AF_INET;
	servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	servAddr.sin_port=htons(9856);

	int cliAddrLen=sizeof(struct sockaddr_in);
	int bindRet=bind(iSockFd,(struct sockaddr*)&servAddr,sizeof(servAddr));

	cout <<"Bind returned "<<bindRet<<endl;

	int iRcvdBytes=recvfrom(iSockFd,buff,1024,0,(struct	sockaddr*)&cliAddr,(socklen_t*)&cliAddrLen);

	cout<<"Received "<<iRcvdBytes<<" bytes from the client"<<endl;

	close(iSockFd);

	return 0;
}

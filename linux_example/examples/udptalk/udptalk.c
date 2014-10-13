#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>

#define BUFLEN 255

int main(int argc, char **argv)
{
	struct sockaddr_in peeraddr, localaddr;
	int sockfd;
	char recmsg[BUFLEN+1];
	int socklen, n;

	if(argc!=5){
		printf("%s <dest IP address> <dest port> <source IP address> <source port>\n", argv[0]);
		exit(0);
	}

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd<0){
		printf("socket creating err in udptalk\n");
		exit(1);
	}
	socklen = sizeof(struct sockaddr_in);
	memset(&peeraddr, 0, socklen);
	peeraddr.sin_family=AF_INET;
	peeraddr.sin_port=htons(atoi(argv[2]));
	if(inet_pton(AF_INET, argv[1], &peeraddr.sin_addr)<=0){
		printf("Wrong dest IP address!\n");
		exit(0);
	}
	memset(&localaddr, 0, socklen);
	localaddr.sin_family=AF_INET;
	if(inet_pton(AF_INET, argv[3], &localaddr.sin_addr)<=0){
		printf("Wrong source IP address!\n");
		exit(0);
	}
	localaddr.sin_port=htons(atoi(argv[4]));
	if(bind(sockfd, &localaddr, socklen)<0){	
		printf("bind local address err in udptalk!\n");
		exit(2);
	}

	if(fgets(recmsg, BUFLEN, stdin) == NULL) exit(0);
	if(sendto(sockfd, recmsg, strlen(recmsg), 0, &peeraddr, socklen)<0){
		printf("sendto err in udptalk!\n");
		exit(3);
	}

	for(;;){
		/*recv&send message loop*/
		n = recvfrom(sockfd, recmsg, BUFLEN, 0, &peeraddr, &socklen);
		if(n<0){
			printf("recvfrom err in udptalk!\n");
			exit(4);
		}else{
			recmsg[n]=0;
			printf("peer:%s", recmsg);	
		}
		if(fgets(recmsg, BUFLEN, stdin) == NULL) exit(0);
		if(sendto(sockfd, recmsg, strlen(recmsg), 0, &peeraddr, socklen)<0){
			printf("sendto err in udptalk!\n");
			exit(3);
		}
	}
}

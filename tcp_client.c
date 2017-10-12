//Basic FTP Client in TCP
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>

#define SERVER_TCP_PORT 3000
#define BUFLEN		256	        // buffer length 

int main(int argc, char **argv)
{
	int 	n;
	int 	sd, port;
	struct	hostent		*hp;
	struct	sockaddr_in server;
	char	*host, *bp;
	char    filename[BUFLEN],rec[BUFLEN];

	switch(argc){
	case 2:
		host = argv[1];
		port = SERVER_TCP_PORT;
		break;
	case 3:
		host = argv[1];
		port = atoi(argv[2]);
		break;
	default:
		fprintf(stderr, "Usage: %s host [port]\n", argv[0]);
		exit(1);
	}

	/* Create a stream socket	*/	
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "Can't creat a socket\n");
		exit(1);
	}

	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	if (hp = gethostbyname(host)) 
	  bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);
	else if ( inet_aton(host, (struct in_addr *) &server.sin_addr) ){
	  fprintf(stderr, "Can't get server's address\n");
	  exit(1);
	}

	/* Connecting to the server */
	if (connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1){
	  fprintf(stderr, "Can't connect \n");
	  exit(1);
	}

	FILE *fp= fopen("dest.txt","ab");
	if (fp == NULL) 
	 	{printf("Cannot open file\n");
	     return (0);}
	printf("Please enter the target file name:\n");
	scanf("%s",filename);
	write(sd,filename,strlen(filename));
	n=read(sd,rec,BUFLEN);
	rec[n]='\0';
	if (strcmp(rec,"FAILURE")!=0){

	  	printf("Writing data to file\n");
	  	fwrite(rec,1,strlen(rec),fp);	
		}
	else {
	  	fprintf(stderr, "File transfer failed.\n");
		}
	close(sd);
	return(0);
}

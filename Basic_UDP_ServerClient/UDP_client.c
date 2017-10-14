// Basic UDP File Transfer Client

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define	BUFSIZE 128

int
main(int argc, char *argv[])
{
	char	*host = "localhost";	/* host to use if none supplied	*/
	char	*service = "3000";
	char	now[100];		/* 32-bit integer to hold time	*/ 
	struct hostent	*phe;	/* pointer to host information entry	*/
	struct sockaddr_in sin;	/* an Internet endpoint address		*/
	int	s, n, type;	/* socket descriptor and socket type	*/
	struct PDU {
	char type;
	char data[100];
	}rx,tx;
	FILE *fp;

	switch (argc) {
	case 1:
		host = "localhost";
		break;
	case 3:
		service = argv[2];
		/* FALL THROUGH */
	case 2:
		host = argv[1];
		break;
	default:
		fprintf(stderr, "usage: UDPtime [host [port]]\n");
		exit(1);
	}

	memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
                                                                                
    /* Map service name to port number */
        sin.sin_port = htons((u_short)atoi(service));
                                                                                
        if ( phe = gethostbyname(host) ){
                memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
                }
        else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE ){
		fprintf(stderr, "Can't get host entry \n");
		exit(1);
	}                                                                        
                                                                                
    /* Allocate a socket */
        s = socket(PF_INET, SOCK_DGRAM, 0);
        if (s < 0){
		fprintf(stderr, "Can't create socket \n");
		exit(1);
	}
                                                                                
    /* Connect the socket */
        if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0){
		fprintf(stderr, "Can't connect to %s %s \n", host, service);
		exit(1);
	}

//Send File Name
	printf("Please enter a file name:\n");
	scanf("%s",tx.data);
	tx.type = 'C';
	(void) write(s, &tx, BUFSIZE);

	fp = fopen("dest.txt","ab");
	  if (fp==NULL){
	    printf("Unable to open file on local\n");
	  }

	while(1){
	n = read(s, &rx, BUFSIZE);
	if (rx.type=='D'){
	    fwrite(rx.data,1,strlen(rx.data),fp);
	    printf("Data transfer successful!\n");
       	}
	else if (rx.type=='E'){
	   printf("Error Message: %s\n",rx.data);  
	}
	else if (rx.type=='F'){
	  fclose(fp);
	  break;
	}
	}
	exit(0);
}

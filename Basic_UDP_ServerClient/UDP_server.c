// Basic UDP File Transfer Server

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>


#define BUFSIZE 128

int
main(int argc, char *argv[])
{
	struct sockaddr_in fsin;	/* the from address of a client	*/
	char	*service = "3000";	/* service name or port number	*/
	char	buf[100];		/* "input" buffer; any size > 0	*/
	char    *pts;
	int	sock,n;			/* server socket		*/
	time_t	now;			/* current time			*/
	int	alen;			/* from-address length		*/
        struct sockaddr_in sin; /* an Internet endpoint address         */
        int     s, type;        /* socket descriptor and socket type    */
	u_short	portbase = 0;	/* port base, for non-root servers	*/
        struct PDU {
	char type;
	char data[100];
	}rx,tx;                                                                         FILE *fp;


	switch (argc) {
	case	1:
		break;
	case	2:
		service = argv[1];
		break;
	default:
		fprintf(stderr, "usage: time_server [host [port]]\n");

	}                                                                              
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = INADDR_ANY;

   /* Map service name to port number */
        sin.sin_port = htons((u_short)atoi(service));
                                                                                
    /* Allocate a socket */
        s = socket(AF_INET, SOCK_DGRAM, 0);
        if (s < 0){
		fprintf(stderr, "can't creat socket\n");
		exit(1);
	}
                                                                                
    /* Bind the socket */
        if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		fprintf(stderr, "can't bind to %s port\n", service);

	while (1) {
		alen = sizeof(fsin);
		if ((n = recvfrom(s, &rx, BUFSIZE, 0,
				(struct sockaddr *)&fsin, &alen)) < 0)
			fprintf(stderr, "recvfrom error\n");
		printf("%s\n", tx.data);

		switch(rx.type){
		case 'C':
		  if (access(rx.data,F_OK)!=-1){
		    FILE *fp = fopen(rx.data,"rb");
		    int fileread = fread(tx.data,1,100,fp);
		    while(!feof(fp)) {
		    tx.type='D';
		    (void) sendto(s, &tx, fileread+1,0,
				(struct sockaddr *)&fsin, sizeof(fsin));
		    int fileread = fread(tx.data,1,100,fp);
		    }
		    fclose(fp);
		    tx.type='F';
		    (void) sendto(s, &tx, sizeof(tx),0,
				(struct sockaddr *)&fsin, sizeof(fsin));
		  }
		  else {
		    tx.type='E';
		    strcpy(tx.data,"Unable to open file.");
		    (void)sendto(s,&tx,sizeof(tx),0,
				 (struct sockaddr *)&fsin, sizeof(fsin));
		  }

		}
	}
}

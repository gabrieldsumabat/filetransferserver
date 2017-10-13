//Basic FTP Server in TCP
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>


#define SERVER_TCP_PORT 3000
#define BUFLEN		256	        // buffer length 

int forks(int);
void reaper(int);

int main(int argc, char **argv)
{
	int 	sd, new_sd, client_len, port;
	struct	sockaddr_in server, client;

	switch(argc){
	case 1:
		port = SERVER_TCP_PORT;
		break;
	case 2:
		port = atoi(argv[1]);
		break;
	default:
		fprintf(stderr, "Usage: %d [port]\n", argv[0]);
		exit(1);
	}

	/* Create a stream socket	*/	
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "Can't creat a socket\n");
		exit(1);
	}

	/* Bind an address to the socket	*/
	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1){
		fprintf(stderr, "Can't bind name to socket\n");
		exit(1);
	}

	/* Queue up to 5 connect requests  */
	listen(sd, 5);

	(void) signal(SIGCHLD, reaper);

	while(1) {
	  client_len = sizeof(client);
	  new_sd = accept(sd, (struct sockaddr *)&client, &client_len);
	  if(new_sd < 0){
	    fprintf(stderr, "Can't accept client \n");
	    exit(1);
	  }
	  switch (fork()){
	  case 0:		/* child */
		(void) close(sd);
		exit(forks(new_sd));
	  default:		/* parent */
		(void) close(new_sd);
		break;
	  case -1:
		fprintf(stderr, "Fork: error\n");
	  }
	}
}

//	Fork
int forks(int sd)
{
	char	buf[BUFLEN];
	int 	n, bytes_to_read;
	char    filename[BUFLEN];

	n=read(sd,filename,BUFLEN);
	filename[n]='\0';
	if (access(filename,F_OK)!=-1) {
	  	printf("File access successful\n");
		FILE *fp = fopen(filename,"rb");
			if (fp == NULL) 
	  			{printf ("Cannot open file\n");
	    			return (0);}
	 	int fileread = fread(buf,1,BUFLEN,fp);
	 	printf("Bytes in file = %d \n", fileread);
	 	printf("Sending file now\n");
	 	write(sd, buf, fileread);
		}
	else {
	  	fprintf(stderr, "Access failed\n");
	  	strcpy(filename,"FAILURE");
	  	write(sd,filename,strlen(filename));
		}
	close(sd);
	return(0);
}

/*	reaper		*/
void	reaper(int sig)
{
	int	status;
	while(wait3(&status, WNOHANG, (struct rusage *)0) >= 0);
}

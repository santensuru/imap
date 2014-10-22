// IMAP

#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
	
void main(int argc, char *argv[])
{
	long sockfd;
	struct sockaddr_in server;
	
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if(sockfd == -1)
	{
        perror(strerror(errno));
		exit(-1);
	}	
	
	server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    inet_aton(argv[1], &(server.sin_addr));
    
    printf("Trying %s ...\n", argv[1]);
	
    int result = connect(sockfd, (struct sockaddr*) &server, sizeof(server));
	
	if(result == -1)
	{
        perror(strerror(errno));
		exit(-1);	
	}
	
	printf("Connected to %s\nEscape character is 'BREAK':\n", argv[1]);
	
	char *buf , readbuf[128];
	int fd = open(argv[3], O_CREAT | O_WRONLY, S_IWRITE | S_IREAD);
	
	while (1) {
		int y = read(sockfd, readbuf, 128); //recv(sockfd, readbuf, 128, 0); //
		int i;
		for (i=0; i<y; i++) {
			printf("%c", readbuf[i]);
		}
		write(fd, readbuf, y);
		while (y == 128) {
			y = read(sockfd, readbuf, 128); //recv(sockfd, readbuf, 128, 0); //
			for (i=0; i<y; i++) {
				printf("%c", readbuf[i]);
			}
			write(fd, readbuf, y);
		}
        
        gets(buf);fflush(stdin);
		if (strcmp(buf, "BREAK")==0) break;
		strcat(buf, "\r\n");
		
		y = write(sockfd, buf, strlen(buf)); //send(sockfd, buf, strlen(buf), 0); //
		write(fd, buf, y);
	}
	
	close(sockfd);
	close(fd);
	return;
}

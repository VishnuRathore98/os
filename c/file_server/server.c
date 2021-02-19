/* 
	This is a server program that can serve a file to the client.
*/

#include <stdio.h>	// For printf();
#include <string.h>	// For memset();
#include <unistd.h>	// For read(), write(), close();
#include <stdlib.h>	// For exit();
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT 12345
#define BUF_SIZE 4096
#define QUEUE_SIZE 10

void fatal(char *string);

int main(int argc, char *argv)
{
	int s, b, l, fd, bytes, sa, on = 1 ;
	char buf[BUF_SIZE];
	struct sockaddr_in channel;
	
	/* Build address structure to bind to socket. */
	memset(&channel, 0, sizeof(channel));
	channel.sin_family = AF_INET;
	channel.sin_addr.s_addr = htonl(INADDR_ANY);
	channel.sin_port = htons(SERVER_PORT);
	
	// Passive open. Wait for connection.
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s< 0) fatal("Socket Failed");
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));

	b = bind(s, (struct sockaddr *) &channel, sizeof(channel));
	if (b< 0) fatal("Bind failed");

	l = listen(s, QUEUE_SIZE);
	if (l< 0) fatal("Listen Failed");

	/* Socket is now set up and bound. Wait for connection and process it. */
	while(1)
	{
		sa = accept(s, 0, 0);
		if (sa< 0) fatal("Accept Failed");
		
		read(sa, buf, BUF_SIZE);

		// Get and return file.
		fd = open(buf, O_RDONLY);
		if (fd< 0) fatal("Open Failed");

		while(1)
		{
			bytes = read(fd, buf, BUF_SIZE);
			if (bytes<= 0) break;
			write(sa, buf, bytes);
		}
		close(fd);
		close(sa);
	}
}

void fatal(char *string)
{
	printf("%s\n", string);
	exit(1);
}

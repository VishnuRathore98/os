/* 
	This is a client program that can request a file from the server.
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT 12345
#define BUF_SIZE 4096

void fatal(char *string);

int main(int argc, char *argv)
{
	int c, s, bytes;
	char buf[BUF_SIZE];
	struct hostent *h;
	struct sockaddr_in channel;

	if (argc != 3)	fatal("Usage: client server-name file-name");
	h = gethostbyname(argv[1]);
	if (!h) fatal("gethostbyname failed");

	s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s< 0) fatal("Socket");
	memset(&channel, 0, sizeof(channel));
	channel.sin_family = AF_INET;
	memcpy(&channel.sin_addr.s_addr, h-> h_addr, h-> h_length);
	channel.sin_port = htons(SERVER_PORT);
	
	c = connect(s, (struct sockaddr *) &channel, sizeof(channel));
	if (c< 0) fatal("Connect Failed");

	/*	Connection is now established. Send file name including 0 byte at end.	*/
	write(s, argv[2], strlen(argv[2])+1);
	
	/*	Go get the file and write it to standard output. */
	while(1)
	{
		bytes = read(s, buf, BUF_SIZE);		// read from socket
		if (bytes <= 0) exit(0);		// check for end of file
		write(1, buf, bytes);			// write to standard output
	}
}

fatal(char *string)
{
	printf("%s\n", string);
	exit(1);
}

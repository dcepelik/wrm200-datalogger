/*
 * wmrq.c:
 * Client component to query for real-time data
 *
 * This software may be freely used and distributed according to the terms
 * of the GNU GPL version 2 or 3. See LICENSE for more information.
 *
 * Copyright (c) 2015 David Čepelík <cepelik@gymlit.cz>
 */

#include <stdio.h>
#include <string.h>
#include <err.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/socket.h>


#define BUF_LEN		256	/* B */
#define DEFAULT_PORT	20892


int
main(int argc, const char *argv[]) {
	int fd, ret;
	const char *hostname, *portstr, *query;
	struct addrinfo hints;
	struct addrinfo *srvinfo, *rp;

	if (argc < 3)
		errx(1, "usage: %s <hostname> <portstr> <query>\n", argv[0]);

	hostname = argv[1];
	portstr = argv[2];
	query = argv[3];

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	if ((ret = getaddrinfo(hostname, portstr, &hints, &srvinfo)) != 0)
		errx( 1, "getaddrinfo failed for '%s': %s\n", hostname, gai_strerror(ret));

	for (rp = srvinfo; rp != NULL; rp = rp->ai_next) {
		fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

		if (fd == -1)
			continue;

		if (connect(fd, rp->ai_addr, rp->ai_addrlen) != -1)
			break;

		close(fd);
	}

	freeaddrinfo(srvinfo);

	if (rp == NULL) { /* no address succeeded */
		errx(1, "Cannot connect to '%s'\n", hostname);
	}

	fprintf(stderr, "Connected to '%s'\n", hostname);

	uint query_len = strlen(query);
	if (write(fd, query, query_len) != query_len) {
		errx(1, "write: cannot write %u bytes\n", query_len);
	}


	close(fd);
	return (0);
}

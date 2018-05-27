
// i want to test each function:
// _open - 
//	invalid protocol
//	port numbers that are too big (or too small)
//  sockets failing to open
//
// _bind, _listen, _accept (usually these are put together)
//
// tcp_recv, send
// udp_recv, send	


//Try testing socket backend.
struct SockTest {
	const char *protocol, *url;
	int port;
	int isServer;	
	uint8_t *request, *response;
	int requestLen; 
	int responseLen;
} sock_tests[] = {
	{ "tcp", "localhost", 8989, 1 }
 ,{ "tcp", "ramarcollins.com", 80, 0 }
 ,{ NULL }
}; 

TEST( socket )
{
	struct SockTest *st = sock_tests;

	while ( st->protocol ) {
		Socket s = { .port = st->port };
		if ( !st->isServer ) {
			//clients need to get the address info
			//...and connect
			//then show what came back	
	#if 0
		if ( isServer ) {
			pid_t pid = fork();
			if ( pid == -1 )
				return 0;
			else if ( pid ) {
				//should be the parent process
			}
			else {
				//what would child do
			}
		}
	#endif
		}
	
		st++;
	}	

	return 1;
}

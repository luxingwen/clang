#include <libssh2.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    int rc;
    int sock;
    struct sockaddr_in sin;
    const char *hostname = "your_server_ip";
    const char *username = "your_username";
    const char *password = "your_password";
    LIBSSH2_SESSION *session;

    /* Initialize the library */
    rc = libssh2_init(0);
    if (rc != 0) {
        fprintf(stderr, "Error initializing libssh2: %d\n", rc);
        exit(1);
    }

    /* Create a socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        fprintf(stderr, "Error creating socket\n");
        exit(1);
    }

    /* Connect to the server */
    sin.sin_family = AF_INET;
    sin.sin_port = htons(22);
    sin.sin_addr.s_addr = inet_addr(hostname);
    if (connect(sock, (struct sockaddr *)(&sin), sizeof(struct sockaddr_in)) != 0) {
        fprintf(stderr, "Error connecting to %s\n", hostname);
        exit(1);
    }

    /* Create a session instance */
    session = libssh2_session_init();
    if (!session) {
        fprintf(stderr, "Error creating session\n");
        exit(1);
    }

    /* Start the SSH session */
    rc = libssh2_session_handshake(session, sock);
    if (rc != 0) {
        fprintf(stderr, "Error starting the SSH session: %d\n", rc);
        exit(1);
    }

    /* Authenticate using a password */
    rc = libssh2_userauth_password(session, username, password);
    if (rc != 0) {
        fprintf(stderr, "Error authenticating with password: %d\n", rc);
        exit(1);
    }

    /* Do some work here */

    /* Disconnect */
    libssh2_session_disconnect(session, "Bye!");
    libssh2_session_free(session);
    close(sock);
    libssh2_exit();
    return 0;
}

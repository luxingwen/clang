#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libwebsockets.h>

static int callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    switch (reason) {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            printf("Client has connected to the server.\n");
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE:
            printf("Client received: %s\n", (char *)in);
            break;

        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            printf("Client failed to connect to the server.\n");
            break;

        case LWS_CALLBACK_CLOSED:
            printf("Client connection has been closed.\n");
            break;

        default:
            break;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    struct lws_context *context;
    struct lws_context_creation_info info;
    struct lws *wsi;
    const char *protocol, *temp;
    char address[100];
    int port = 80;

    if (argc < 3) {
        printf("Usage: %s <address> <port> [<protocol>]\n", argv[0]);
        return 1;
    }

    memset(&info, 0, sizeof info);
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = lws_get_protocols(&protocol);

    context = lws_create_context(&info);
    if (context == NULL) {
        printf("Failed to create libwebsockets context.\n");
        return 1;
    }

    strncpy(address, argv[1], sizeof address);
    address[sizeof address - 1] = '\0';
    port = atoi(argv[2]);

    if (argc > 3)
        temp = argv[3];
    else
        temp = "http";

    wsi = lws_client_connect(context, address, port, 0, "/", address, temp, NULL, -1);
    if (wsi == NULL) {
        printf("Failed to connect to the server.\n");
        lws_context_destroy(context);
        return 1;
    }

    while (1) {
        lws_service(context, 50);
    }

    lws_context_destroy(context);

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <libwebsockets.h>

static int callback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            printf("Client connected.\n");
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE:
            printf("Received data: %s\n", (char *)in);
            break;

        case LWS_CALLBACK_CLIENT_WRITEABLE:
            {
                const char *msg = "Hello, WebSocket server!";
                unsigned char buf[LWS_PRE + strlen(msg)];
                unsigned char *p = &buf[LWS_PRE];
                size_t n = strlen(msg);

                memcpy(p, msg, n);
                lws_write(wsi, p, n, LWS_WRITE_TEXT);
            }
            break;
        case LWS_CALLBACK_CLOSED:
            printf("Client disconnected.\n");
            break;

        default:
            break;
    }

return 0;
}

static struct lws_protocols protocols[] = {
{
"example_protocol",
callback,
0,
0,
},
{ NULL, NULL, 0, 0 } // terminator
};

int main(int argc, char **argv) {
    struct lws_context_creation_info info;
    struct lws_client_connect_info ccinfo;
    struct lws_context *context;
    struct lws *wsi;
    int n = 0;
    memset(&info, 0, sizeof(info));
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;

    context = lws_create_context(&info);
    if (context == NULL) {
        fprintf(stderr, "Error creating context.\n");
        return -1;
    }

    memset(&ccinfo, 0, sizeof(ccinfo));
    ccinfo.context = context;
    ccinfo.address = "localhost"; // Replace with your server address
    ccinfo.port = 9000; // Replace with your server port
    ccinfo.path = "/";
    ccinfo.host = ccinfo.address;
    ccinfo.origin = ccinfo.address;
    ccinfo.protocol = protocols[0].name;
    wsi = lws_client_connect_via_info(&ccinfo);
    if (wsi == NULL) {
        fprintf(stderr, "Error connecting to the server.\n");
        lws_context_destroy(context);
        return -1;
    }

    // Main loop
    while (n >= 0) {
        n = lws_service(context, 50);
    }

    // Clean up
    lws_context_destroy(context);

    return 0;
}



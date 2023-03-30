#include <libwebsockets.h>
#include <string.h>

#define EXAMPLE_URI "/example"

static int callback_example(struct lws *wsi,
                            enum lws_callback_reasons reason,
                            void *user, void *in, size_t len) {
  switch (reason) {
    case LWS_CALLBACK_CLIENT_ESTABLISHED:
      printf("Client established\n");
      break;
    case LWS_CALLBACK_CLIENT_RECEIVE:
      printf("Client received data: %s\n", (char *) in);
      break;
    case LWS_CALLBACK_CLIENT_CLOSED:
      printf("Client closed\n");
      break;
    default:
      break;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  struct lws_context_creation_info info;
  memset(&info, 0, sizeof(info));
  info.port = CONTEXT_PORT_NO_LISTEN;
  info.protocols = (struct lws_protocols *) &callback_example;
  info.uri = EXAMPLE_URI;
  struct lws_context *context = lws_create_context(&info);
  if (!context) {
    printf("Error creating context\n");
    return 1;
  }
  struct lws *wsi = lws_client_connect(context, "localhost", 8080, 0,
                                        EXAMPLE_URI, NULL, NULL, NULL, 0);
  if (!wsi) {
    printf("Error connecting\n");
    return 1;
  }
  while (1) {
    lws_service(context, 0);
  }
  lws_context_destroy(context);
  return 0;
}


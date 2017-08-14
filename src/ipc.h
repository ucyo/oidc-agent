#ifndef IPC_H
#define IPC_H

#include <sys/un.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include <syslog.h>

#include "oidc_array.h"

#define RESPONSE_STATUS "{\"status\":\"%s\"}"
#define RESPONSE_STATUS_REFRESH "{\"status\":\"%s\", \"refresh_token\":\"%s\"}"
#define RESPONSE_STATUS_ACCESS "{\"status\":\"%s\", \"access_token\":\"%s\"}"
#define RESPONSE_STATUS_PROVIDER "{\"status\":\"%s\", \"provider_list\":\"%s\"}"
#define RESPONSE_ERROR "{\"status\":\"failure\", \"error\":\"%s\"}"


struct connection {
  int* sock;
  int* msgsock;
  struct sockaddr_un* server;
};

char* init_socket_path(const char* env_var_name) ;
int ipc_init(struct connection* con, const char* env_var_name, int isServer) ;
int ipc_bindAndListen(struct connection* con) ;
struct connection* ipc_async(struct connection listencon, struct connection** clientcons_addr, size_t* size) ;
int ipc_connect(struct connection con) ;
char* ipc_read(int _sock);
int ipc_write(int _sock, char* msg, ...);
int ipc_close(struct connection* con);
int ipc_closeAndUnlink(struct connection* con);

struct connection* addConnection(struct connection* cons, size_t* size, struct connection client) ;
struct connection* findConnection(struct connection* cons, size_t size, struct connection key) ;
struct connection* removeConnection(struct connection* cons, size_t* size, struct connection* key) ;


#endif // IPC_H

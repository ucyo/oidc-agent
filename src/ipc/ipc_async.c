#include "ipc_async.h"

#include "ipc.h"

#include <syslog.h>

#include <sys/select.h>
#include <sys/socket.h>

/** @fn struct connection* ipc_async(struct connection listencon, struct
 * connection** clientcons_addr, size_t* size)
 * @brief handles asynchronous communication
 *
 * listens for incoming connections on the listencon and for incoming messages
 * on multiple client sockets. If a new client connects it is added to the list
 * of current client connections.  If on any client socket is a message
 * available for reading, a pointer to this connection is returned.
 * @param listencon the connection struct for the socket accepting new client
 * connections. The list is updated if a new client connects.
 * @param clientcons_addr a pointer to an array of client connections
 * @param size a pointer to the number of client connections. The number is
 * updated if a new client connects.
 * @return A pointer to a client connection. On this connection is either a
 * message avaible for reading or the client disconnected.
 */
struct connection* ipc_async(struct connection listencon, list_t* connections) {
  while (1) {
    fd_set readSockSet;
    FD_ZERO(&readSockSet);
    FD_SET(*(listencon.sock), &readSockSet);
    // Determine maxSock
    int          maxSock = *(listencon.sock);
    unsigned int i;
    for (i = 0; i < connections->len; i++) {
      struct connection* con = list_at(connections, i)->val;
      FD_SET(*(con->msgsock), &readSockSet);
      if (*(con->msgsock) > maxSock) {
        maxSock = *(con->msgsock);
      }
    }
    // Waiting for incoming connections and messages
    syslog(LOG_AUTHPRIV | LOG_DEBUG, "Selecting maxSock is %d", maxSock);
    int ret = select(maxSock + 1, &readSockSet, NULL, NULL, NULL);
    if (ret >= 0) {
      if (FD_ISSET(*(listencon.sock),
                   &readSockSet)) {  // if listensock read something it means a
                                     // new client connected
        syslog(LOG_AUTHPRIV | LOG_DEBUG, "New incoming client");
        struct connection* newClient = secAlloc(sizeof(struct connection));
        newClient->msgsock           = secAlloc(sizeof(int));
        *(newClient->msgsock)        = accept(*(listencon.sock), 0, 0);
        if (*(newClient->msgsock) >= 0) {
          syslog(LOG_AUTHPRIV | LOG_DEBUG, "accepted new client sock: %d",
                 *(newClient->msgsock));
          list_rpush(connections, list_node_new(newClient));
          syslog(LOG_AUTHPRIV | LOG_DEBUG, "updated client list");
        } else {
          syslog(LOG_AUTHPRIV | LOG_ERR, "%m");
        }
      }
      // Check all client sockets for new messages
      int j;
      for (j = connections->len - 1; j >= 0; j--) {
        struct connection* con = list_at(connections, j)->val;
        syslog(LOG_AUTHPRIV | LOG_DEBUG, "Checking client %d", *(con->msgsock));
        if (FD_ISSET(*(con->msgsock), &readSockSet)) {
          syslog(LOG_AUTHPRIV | LOG_DEBUG, "New message for read av");
          return con;
        }
      }
    } else {
      syslog(LOG_AUTHPRIV | LOG_ERR, "%m");
    }
  }
  return NULL;
}

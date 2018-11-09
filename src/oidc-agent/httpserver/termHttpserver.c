#define _GNU_SOURCE
#include "termHttpserver.h"
#include "list/list.h"
#include "running_server.h"
#include "utils/memory.h"

#include <microhttpd.h>
#include <signal.h>
#include <sys/types.h>
#include <syslog.h>

void stopHttpServer(struct MHD_Daemon** d_ptr) {
  syslog(LOG_AUTHPRIV | LOG_DEBUG, "HttpServer: Stopping HttpServer");
  MHD_stop_daemon(*d_ptr);
  secFree(d_ptr);
}

void termHttpServer(const char* state) {
  if (state == NULL) {
    return;
  }
  pid_t pid = removeServer(state);
  if (pid > 0) {
    kill(pid, SIGTERM);
    syslog(LOG_AUTHPRIV | LOG_DEBUG,
           "killed webserver for state %s with pid %d", state, pid);
  }
}
#include "oidc-token-exchange.h"
#include "exchange_handler.h"
#include "privileges/exchange_privileges.h"

#include <syslog.h>

int main(int argc, char** argv) {
  openlog("oidc-token-exchange", LOG_CONS | LOG_PID, LOG_AUTHPRIV);
  setlogmask(LOG_UPTO(LOG_NOTICE));

  struct arguments arguments;
  initArguments(&arguments);
  argp_parse(&argp, argc, argv, 0, 0, &arguments);
  if (!arguments.noSeccomp) {
    initOidcExchangePrivileges(&arguments);
  }
  if (arguments.debug) {
    setlogmask(LOG_UPTO(LOG_DEBUG));
  }
  if (arguments.args[1] != NULL) {  // more than 1 argument provided
    handleTokenExchange(&arguments);
  }
  if (arguments.remove) {
    handleRemove(&arguments);
  } else {
    handleTokenRequest(&arguments);
  }
}
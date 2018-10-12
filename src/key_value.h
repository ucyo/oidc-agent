#ifndef KEY_VALUE_H
#define KEY_VALUE_H

#include "utils/cleaner.h"

#include <stddef.h>

struct key_value {
  const char* key;
  char*       value;
};

static inline void clearFreeKeyValuePairs(struct key_value* pairs,
                                          size_t            size) {
  size_t i;
  for (i = 0; i < size; i++) { clearFreeString(pairs[i].value); }
}

#endif  // KEY_VALUE_H
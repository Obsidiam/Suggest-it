
#include "suggest.h"
#include <string.h>
#include <assert.h>

///////////////////////////////////////////////////////////////////////////// 
// do not modify this constants:
const int SUGGEST_MAX_STRLEN = 63;
/////////////////////////////////////////////////////////////////////////////

// time: O(n*(logn + m*logn))
struct triple_s *suggest(const struct triple_s *dict, const char *word) {
  assert(dict); 
  assert(word);
  // requires: word only contains lowercase letters (or numbers)
  struct triple_s *sss = sss_create();
  char current[64];
  int len = strlen(word);
  assert(len <= SUGGEST_MAX_STRLEN);

  // single deletion
  for (int i = 0; i < len; ++i) {
    strcpy(current, word);
    for (int j = 0; j < len; ++j) {
      if (i <= j) {
        current[j] = current [j+1];
      }
    }
    if (sss_search(dict, current) != -1) {
      sss_add(sss, current);
    }
  }

  // single insertion
  for (int n = 0; n <= len; ++n) {
    for (int j = 97; j < 123; ++j) {
      strcpy(current, word);
      char change = j;
      char next = current[n];
      current[n] = change;
      for (int i = (n + 1); i <= (len + 1); ++i) {
        char prev = current[i];
        current[i] = next;
        next = prev;
      }
      if (sss_search(dict, current) != -1) {
        sss_add(sss, current);
      }
    }
  }

  // single substitution
  for (int i = 0; i < len; ++i) {
    strcpy(current, word);
    for (int j = 97; j < 123; ++j) {
      char change = j;
      current[i] = change;
      if (sss_search(dict, current) != -1) {
        sss_add(sss, current);
      }
    }
  }

  // adjacent swap
  for (int i = 0; i < (len - 1); ++i) {
    strcpy(current, word);
    char first = current[i];
    current[i] = current[i + 1];
    current[i + 1] = first;
    if (sss_search(dict, current) != -1) {
      sss_add(sss, current);
    }
  }
  if (sss_search(sss, word) != -1) {
    sss_remove(sss, word);
  }
  return sss;
}

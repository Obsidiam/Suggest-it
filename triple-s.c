#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>


// The following applies to all functions
// requires: all sss parameters are valid (not NULL) 
//           all s parameters are valid (not NULL)
// time: (n) is the number of strings in the sss
//       (m) is the length of the string s

struct triple_s {
  char **str_arr;
  int len;
  int maxlen;
};

// sss_create() creates a new SSS ADT
// effects: allocates memory (you must call sss_destroy)
// time: O(1)
struct triple_s *sss_create(void) {
  struct triple_s *sss = malloc(sizeof(struct triple_s));
  sss->len = 0;
  sss->maxlen = 1;
  sss->str_arr = malloc(sss->maxlen * sizeof(char *));
  return sss;
}

// sss_destroy(sss) destroys sss
// requires: sss is valid (not NULL)
// effects: sss is no longer valid
// time: O(n)
void sss_destroy(struct triple_s *sss) {
  assert(sss);
  for (int i = 0; i < sss->len; ++i) {
    free(sss->str_arr[i]);
  }
  free(sss->str_arr);
  free(sss);
}

// Helper function
// my_strdup(s) creates a duplicate of s 
// (which will be used to use instead of strcpy in the 2 add functions);
char *my_strdup(const char *s) {
  assert(s);
  char *newstr = malloc((strlen(s) + 1) * sizeof(char));
  strcpy(newstr, s);
  return newstr;
}

// sss_add(sss, s) adds s to sss in the correct index (lexicographically)
// notes: if s already exists, there is no effect
//        when s is added, the indices of words that lexicographically
//          follow s will be changed (+1) because s is added into the "middle"
// effects: may modify sss
// time: O(n + m*logn)
void sss_add(struct triple_s *sss, const char *s) {
  assert(sss);
  assert(s);
  if (sss->len == sss->maxlen) {
    sss->maxlen *= 2;
    sss->str_arr = realloc(sss->str_arr, sss->maxlen * sizeof(char *));
  }
  int place = -1;
  if (sss->len == 0) {
    place = 0;
  }
  for (int i = 0; i < sss->len; ++i) {
    int x = strcmp(sss->str_arr[i], s);
    if (x == 0) {
      break;
    }
    if (x > 0) {
      place = i;
      break;
    }
    if (i == sss->len - 1) {
      place = sss->len;
    }
  }
  if (place != -1) {
    if (place == sss->len) {
      sss->str_arr[place] = my_strdup(s);
    } else {
      char *change = sss->str_arr[place];
      sss->str_arr[place] = my_strdup(s);
      for (int i = place + 1; i < sss->len; ++i) {
        char *current = sss->str_arr[i];
        free(sss->str_arr[i]);
        sss->str_arr[i] = malloc((strlen(change) + 1) * sizeof(char));
        sss->str_arr[i] = change;
        change = current;
      }
      sss->str_arr[sss->len] = change;
    }
    sss->len += 1;
  }
}

// sss_add_follows(sss, s) adds s to the "end" of the sss
// requires: s lexicographically follows all strings already in sss
// effects: modifies sss
// time: O(m) [amortized]
void sss_add_follows(struct triple_s *sss, const char *s) {
  assert(sss);
  assert(s);
  if (sss->len == sss->maxlen) {
    sss->maxlen *= 2;
    sss->str_arr = realloc(sss->str_arr, sss->maxlen * sizeof(char *));
  }
  sss->str_arr[sss->len] = my_strdup(s);
  sss->len += 1;
}

// sss_remove(sss, s) removes s from sss (if it exists)
// notes: if s is removed, the indices of words that lexicographically
//          follow s will be changed (-1)
// effects: may modify sss
// time: O(n + m*logn)
void sss_remove(struct triple_s *sss, const char *s) {
  assert(s);
  assert(sss);
  int pos = -1;
  for (int i = 0; i < sss->len;  ++i) {
    if (strcmp(sss->str_arr[i], s) == 0) {
      pos = i;
      break;
    }
  }
  if (pos != -1) {
    for (int i = pos; i < sss->len - 1; ++i) {
      free(sss->str_arr[i]);
      int x = strlen(sss->str_arr[i+1]) + 1;
      sss->str_arr[i] = malloc(x * sizeof(char));
      strcpy(sss->str_arr[i], sss->str_arr[i+1]);
    }
    free(sss->str_arr[sss->len - 1]);
    sss->len += -1;
  }
}

// sss_count(sss) gets the number of strings in sss
// time: O(1)
int sss_count(const struct triple_s *sss) {
  assert(sss);
  return sss->len;
}

// sss_search(sss, s) finds the index of s in sss
// note: returns -1 if s does not appear in the sss
// time: O(m*logn)
int sss_search(const struct triple_s *sss, const char *s) {
  assert(sss);
  assert(s);
  int low = 0;
  int high = sss->len - 1;
  while (low <= high) {
    int mid = (low + high) / 2;
    if (strcmp(sss->str_arr[mid], s) == 0) {
      return mid;
    } else if (strcmp(sss->str_arr[mid], s) < 0)  {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }
  return -1;
}

// sss_get(sss, idx) retrieves a pointer to the string in sss
//   at the given idx (index)
// requires: 0 <= idx < sss_count(sss)
// time: O(1)
const char *sss_get(const struct triple_s *sss, int idx) {
  assert(sss);
  assert(0 <= idx);
  assert(idx < sss->len);
  return sss->str_arr[idx];
}

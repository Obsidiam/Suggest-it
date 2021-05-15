// This is an I/O test client for the suggest module

#include <assert.h>
#include <stdio.h>
#include "words.h"
#include "suggest.h"

int main(void) {
  char input[64] = "";
  struct triple_s *dict = words_create();
  while (scanf("%s", input) == 1) {
    printf("%s:", input);
    int index = sss_search(dict, input);
    if (index != -1) {
      printf(" Word is correct");
    } else {
      struct triple_s *output = suggest(dict, input);
      int n = sss_count(output);
      if (n) {
        for (int i = 0; i < n; ++i) {
          printf(" %s", sss_get(output, i));
        }
      } else {
        printf(" No Such Word Exists");
      }
      sss_destroy(output);
    }
    printf("\n");
  }
  sss_destroy(dict);
}

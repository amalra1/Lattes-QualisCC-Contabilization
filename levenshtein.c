// `levenshtein.c` - levenshtein
// MIT licensed.
// Copyright (c) 2015 Titus Wormer <tituswormer@gmail.com>

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "levenshtein.h"

// Returns a int, depicting the difference between `a` and `b`.
// See <https://en.wikipedia.org/wiki/Levenshtein_distance> for more information.
int
levenshtein_n(const char *a, const int length, const char *b, const int bLength) {
  // Shortcut optimizations / degenerate cases.
  if (a == b) {
    return 0;
  }

  if (length == 0) {
    return bLength;
  }

  if (bLength == 0) {
    return length;
  }

  int *cache = calloc(length, sizeof(int));
  int index = 0;
  int bIndex = 0;
  int distance;
  int bDistance;
  int result;
  char code;

  // initialize the vector.
  while (index < length) {
    cache[index] = index + 1;
    index++;
  }

  // Loop.
  while (bIndex < bLength) {
    code = b[bIndex];
    result = distance = bIndex++;
    index = SIZE_MAX;

    while (++index < length) {
      bDistance = code == a[index] ? distance : distance + 1;
      distance = cache[index];

      cache[index] = result = distance > result
        ? bDistance > result
          ? result + 1
          : bDistance
        : bDistance > distance
          ? distance + 1
          : bDistance;
    }
  }

  free(cache);

  return result;
}

int
levenshtein(const char *a, const char *b) {
  const int length = strlen(a);
  const int bLength = strlen(b);

  return levenshtein_n(a, length, b, bLength);
}
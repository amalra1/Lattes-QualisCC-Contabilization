#ifndef LEVENSHTEIN_H
#define LEVENSHTEIN_H

#include <stddef.h>

// `levenshtein.h` - levenshtein
// MIT licensed.
// Copyright (c) 2015 Titus Wormer <tituswormer@gmail.com>

// Returns a int, depicting the difference between `a` and `b`.
// See <https://en.wikipedia.org/wiki/Levenshtein_distance> for more information.
#ifdef __cplusplus
extern "C" {
#endif

int
levenshtein(const char *a, const char *b);

int
levenshtein_n (const char *a, const int length, const char *b, const int bLength);

#ifdef __cplusplus
}
#endif

#endif // LEVENSHTEIN_H
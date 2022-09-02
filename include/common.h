// common includes/typedefs 

#pragma once

#include <stdbool.h>

typedef void* Pointer;

typedef unsigned int uint;

typedef unsigned char byte;

/* pointer to function that compares two elements a and b. Returns:
   < 0 if a < b
   = 0 if a = b (based on given comparison)
   > 0 if a > b                                                   */
typedef int (*CompareFunc)(void* a, void* b);

// pointer to function that destroys given element
typedef void (*DestroyFunc)(void*);
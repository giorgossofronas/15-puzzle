#pragma once

// common includes/typedefs 

#include <stdlib.h>
#include <stdbool.h>

typedef unsigned int uint;

// pointer to function that destroys given element
typedef void (*DestroyFunc)(void*);
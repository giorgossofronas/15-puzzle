#pragma once

#include <stdbool.h>
#include <stdlib.h>

// unsigned int typedef
typedef unsigned int uint;

// pointer to function that destroys given element
typedef void (*DestroyFunc)(void*);
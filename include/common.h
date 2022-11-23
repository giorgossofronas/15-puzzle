#pragma once

#include <stdbool.h>
#include <stdlib.h>

// pointer to function that destroys given element
typedef void (*DestroyFunc)(void*);
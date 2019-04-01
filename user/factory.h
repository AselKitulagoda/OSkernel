#ifndef __FACTORY_H
#define __FACTORY_H
#include "hilevel.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "PL011.h"

typedef struct {
    int phid;
    int forks[2];
}ph1_t;

#endif
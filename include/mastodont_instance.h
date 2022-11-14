/*
 * Licensed under BSD 2-Clause License
 */

#ifndef MASTODONT_INSTANCE_H
#define MASTODONT_INSTANCE_H
#include "mastodont_types.h"

struct mstdnt_instance
{
    int a;
};

// TODO
int mstdnt_instance(mastodont_t* api, struct mstdnt_args* args,
                       struct mstdnt_instance* inst);

#endif /* MASTODONT_INSTANCE_H */

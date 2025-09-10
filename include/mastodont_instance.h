/*
 * Licensed under BSD 3-Clause License
 */

#ifndef MASTODONT_INSTANCE_H
#define MASTODONT_INSTANCE_H
#include "mastodont_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct mstdnt_instance
{
    int a;
};

// TODO
int mstdnt_instance(mastodont_t* api, struct mstdnt_args* args,
                       struct mstdnt_instance* inst);

#ifdef __cplusplus
}
#endif
					   
#endif /* MASTODONT_INSTANCE_H */

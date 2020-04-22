#ifndef _SCHEDUL
#define _SCHEDUL

    #include <stdint.h>
    #include <stddef.h>

    void * scheduler(void * resp);
    int addPCB(void * rsp, size_t priority);

#endif

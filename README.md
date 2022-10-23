# Embedded Garage ga_queue Queue library
## Prerequisities
User should put eg_queue_cfg.h file in queue directory! File shoud contain own implenenattion of given macros.
```
#ifndef _EG_QUEUE_CFG_H_
#define _EG_QUEUE_CFG_H_

/** User defieable macro to enter to critical section */
#define CRITICAL_SECTION_ENTER()
/** User defieable macro to exit from critical section */
#define CRITICAL_SECTION_EXIT()

#endif /* _EG_QUEUE_CFG_H_ */
```

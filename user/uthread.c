#include "kernel/types.h"
#include "user/user.h"
#include "user/clone_common.h" // for PGSIZE
#include <stdint.h>

#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE-1))

// typedef struct thread {
//     void *stack;
// } thread_t;

// #define MAX_THREADS 64
// static void *thread_stacks[MAX_THREADS] = {0};

int thread_create(void (*fn)(uint64, uint64), uint64 arg1, uint64 arg2) {
    char * stack = (char *) malloc(PGSIZE * 3);
    char * stack_clone = 0;

    uint64 stack_address = (uintptr_t)&(*stack);

    uint64 stack_clone_address = PGROUNDUP(PGROUNDUP(stack_address)+1);

    stack_clone = &stack[stack_clone_address - stack_address];

    *((char **) (stack_clone - 8)) = stack;

    int pid = -1;
    if ((pid = clone(fn, arg1, arg2, stack_clone)) == -1) {
        free(stack);
        return -1;
    }
    return pid;
}

int thread_join(int *retval) {
    char * stack_clone;
    int pid = -1;

    if (retval) {
        pid = join((void **) &stack_clone, retval);
    } else {
        pid = join((void **) &stack_clone, 0);
    }

    if (pid != -1) {
        char * pointer_to_stack = (char *) (stack_clone - 8);
        char * stack = *((char **) pointer_to_stack);
        free(stack);
    }


    return pid;
}

void lock_init(lock_t * lock) {
    lock->lastTicket = 0;
    lock->currentTicket = 0;
}

void lock_acquire(lock_t * lock) {
    int myTicket = __sync_fetch_and_add(&lock->lastTicket, 1);
    while (lock->currentTicket != myTicket) {
        continue;
    }
}

void lock_release(lock_t * lock) {
    __sync_fetch_and_add(&lock->currentTicket, 1);
}
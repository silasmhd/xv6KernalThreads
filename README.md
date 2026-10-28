# Kernel Threads Implementation for xv6

## Description
This project implements kernel threads that share an address space in the xv6 kernel. It adds `clone()` and `join()` system calls, a user-level thread library, and a ticket lock for synchronization.

## Objectives
- Implement `clone()` to create threads sharing an address space
- Implement `join()` to wait for thread termination
- Build a user-level thread library (`thread_create`, `thread_join`)
- Implement a spinning ticket lock (`lock_init`, `lock_acquire`, `lock_release`)

## Implementation
- `clone(fn, arg1, arg2, stack)` — creates a thread sharing the caller's address space; returns child PID or -1
- `join(stack, retval)` — waits for a child thread; returns its stack address and exit status
- `thread_create(fn, arg1, arg2)` — allocates page-aligned stack, calls `clone()`, returns PID
- `thread_join(retval)` — call `join()`, frees stack, returns waited PID
- `lock_init(lock)` — initializes ticket lock to zero
- `lock_acquire(lock)` — atomic fetch-and-add to acquire lock
- `lock_release(lock)` — increments owner ticket to release

## Build and Run Instructions

To build and run xv6, you need a RISC-V toolchain and QEMU emulator

I used WSL and ran Ubuntu 24.04 specifically, so there may be differences with different distributions and versions. Once you have wsl installed this will install the version of Ubuntu I used, but removing "-24.04" will install the latest.

```bash
wsl.exe --install Ubuntu-24.04
```

Next update and install the packages I used

```bash
sudo apt-get update && sudo apt-get upgrade

sudo apt-get install git build-essential qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu  
```

Lastly clone this repository, change directory into it, and run QEMU.

```bash
git clone https://github.com/silasmhd/xv6KernalThreads

cd xv6KernalThreads

make qemu
```

To exit QEMU press `ctrl + a` then `x`. 
_This won't work with capslock on._

## Tests

Once you have QEMU running, you can run the test for this project and look at the them in `test-kthreads`.

```bash
make test
```

## Files Modified
- `kernel/syscall.h` — syscall numbers for clone and join
- `kernel/syscall.c` — syscall dispatch
- `kernel/sysproc.c` — clone() and join() implementations
- `user/usys.pl` — user-space syscall stubs
- `user/user.h` — prototypes for clone, join, thread library, locks
- `kernel/proc.h` — struct proc extended for shared pagetable, stack, refcount
- `kernel/proc.c` — allocproc, freeproc, fork modifications
- `kernel/vm.c` — address validation, page table reference counting
- `kernel/memlayout.h` — TRAPFRAME mapping for shared address spaces
- `kernel/spinlock.c` — lock for address space refcount
- `user/uthread.c` — thread_create, thread_join, lock_init, lock_acquire, lock_release
- `Makefile` — add uthread.o to ULIB


## References
- Operating Systems: Three Easy Pieces Chapters 26 - Cuncurrency: An Introduction, 27 - Interlude: Thread API, and 28 - Locks
- xv6 book, Chapters 6: Locking and 9: Cuncurrencry Revisited
- "Design of the BSD Kernel Threads Implementation" for inspiration
- Original xv6-riscv repository: https://github.com/mit-pdos/xv6-riscv

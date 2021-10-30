### Consumer-Producer Program
# Introduction
Using semaphores and shared memory, one can develop a solution for a consumer and a producer process that consumes and produces in a shared memory block, respectively.
A consumer process is something that extracts elements out of memory, while a producer process is something that produces elements that are stored in memory.

The central idea of this problem is the managing of the consumption and production in the memory.

Semaphores are used to prevent reckless production or consumption, including:
1. Producing when shared memory block is full
2. Consuming when shared memory block is empty
3. Producing and consuming in the shared memory block at the same time

Semaphores offer protections with its sem_wait() and sem_post() functions.
sem_wait() pauses the thread if the semaphore value is 0, while sem_post() increases the semaphore value by 1.
As semaphores can be accessed across processes, they allow for process synchronization.

Using these two methods, one can code it so that a process must wait until another one finishes a task (sem_wait() occurs until another thread runs sem_post()).
This allows for process management.

## Steps to run a local copy of this program

# Setup
Compile using Linux
* e.g.
    * install VirtualBox
    * install Ubuntu 20.04 on VirtualBox
    * install Virtual Studio Code on Ubuntu

Additional prerequisties:
* gcc
``` 
sudo apt-get install gcc 
```

Copy this repository's link for this program
```
git clone https://github.com/davidbian1/OS.git 
```

# Compilation
Commands for compilation:

```
gcc producer.c -pthread -lrt -o producer
gcc consumer.c -pthread -lrt -o consumer
./producer & ./consumer &
```

# Output
Expected Sample Output
```
Consumer ready to receive 20 items.

Producer ready to create 20 items.
Item produced, there are now 1 item(s) in the table.
Item produced, there are now 2 item(s) in the table.
Item consumed, there are now 1 item(s) remaining in the table.
Item produced, there are now 2 item(s) in the table.
Item consumed, there are now 1 item(s) remaining in the table.
Item consumed, there are now 0 item(s) remaining in the table.
Item produced, there are now 1 item(s) in the table.
Item consumed, there are now 0 item(s) remaining in the table.
Item produced, there are now 1 item(s) in the table.
Item produced, there are now 2 item(s) in the table.
Item consumed, there are now 1 item(s) remaining in the table.
Item consumed, there are now 0 item(s) remaining in the table.
Item produced, there are now 1 item(s) in the table.
Item produced, there are now 2 item(s) in the table.
Item consumed, there are now 1 item(s) remaining in the table.
Item produced, there are now 2 item(s) in the table.
Item consumed, there are now 1 item(s) remaining in the table.
Item consumed, there are now 0 item(s) remaining in the table.
item produced, there are now 1 item(s) in the table.
Item consumed, there are now 0 item(s) remaining in the table.
Item produced, there are now 1 item(s) in the table.
Item produced, there are now 2 item(s) in the table.
Item consumed, there are now 1 item(s) remaining in the table.
Item produced, there are now 2 item(s) in the table.
Item consumed, there are now 1 item(s) remaining in the table.
Item consumed, there are now 0 item(s) remaining in the table.
Item produced, there are now 1 item(s) in the table.
Item consumed, there are now 0 item(s) remaining in the table.
Item produced, there are now 1 item(s) in the table.
Item produced, there are now 2 item(s) in the table.
Item consumed, there are now 1 item(s) remaining in the table.
Item produced, there are now 2 item(s) in the table.
Item consumed, there are now 1 item(s) remaining in the table.
Item produced, there are now 2 item(s) in the table.
Item consumed, there are now 1 item(s) remaining in the table.
Item produced, there are now 2 item(s) in the table.
Item consumed, there are now 1 item(s) remaining in the table.
item produced, there are now 2 item(s) in the table.
Item consumed, there are now 1 item(s) remaining in the table.
Item consumed, there are now 0 item(s) remaining in the table.
Producer cleaned up!
Consumer cleaned up!
```

# Work and Acknowledgements
Thank you to Dr. Qiang Guan for supplying his example code.
* Most of the code orginates from his code, with a few tweaks here and there
* Work done by me were mainly the dense comments explaining each line of code and how they worked to demonstrate my thorough understanding of the code.
* If the code does not compile, it is likely due to the sem_post() and sem_wait() functions.
   * When I was running the code and with debugging, I found that the threads would hang on the sem_post() and sem_wait() functions (as well as sem_init()), 
   * resulting in a segmentation error when I exited it. 
      * I found no clear answer as to why this occurs (I searched quite a lot online about it), as a simple semaphore declaration and sem_post() hangs the thread, despite a correct initalization of the semaphore. If you could find why this occurs (and how to fix), I would much appreciate it (I spent several hours trying to resolve it.)
   * Most examples of the consumer-producer problem dealt with 2+ threads in a single problem, so I found them not useful applying for the multi-program requirement for this programming assignment.


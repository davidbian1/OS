### Consumer-Producer Program
# Introduction
Using semaphores and shared memory, one can create a consumer and a producer process that consumes and produces in a shared memory block, respectively.

Semaphores are used to prevent overhead/dangerous production or consumption, including:
1. Producing when shared memory block is full
2. Consuming when shared memory block is empty
3. Producing and consuming in the shared memory block at the same time


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

# Compilation
Commands for compilation:

```
gcc producer.c -pthread -lrt -o producer
gcc consumer.c -pthread -lrt -o consumer
./producer & ./consumer &
```

# Output
Expected Output
'''


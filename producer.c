#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
 
int main()
{
        // Allocated shared memory and semaphore

                //Open a shared block of memory called "table",
                //created if not already existent,
                //with all bits able to be written or read
        int shm_fd = shm_open("table", O_CREAT, 0666);

                //Instantiate a pointer to a created map of virtual memory
                //allocated by default (0) by the kernel
                //with a size of 32 bits (size of int)
                //with all bits able to be written or read
                //and all updates shared between processes
                //of the shared block of memory
                //starting at the first location (0)
        int *table = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);


        //Instantiate semaphores each with their given names,
        //created if not already existent
        // all bits able to be read or written
        // and an initial semaphore value

                        //fill - number of elements in the map currently (0)
                        //-empty
        sem_t *fill = sem_open("fill", O_CREAT, 0666, 0);

                        //avail - number of spots remaining in the map
                        //currently all (2)
        sem_t *avail = sem_open("available", O_CREAT, 0666, 2);

                        //mutex semaphore - keeps track of the number of
                        //available processes able to enter the critical section
                        //only ONE at a time to access the map (mutual exclusion)
                        //currently 1 process spot
        sem_t *mutex = sem_open("mutex", O_CREAT, 0666, 1);


        // Here we wait until space is available in
        // the shared buffer. We then wait a random
        // number of ms, use mutual exclusion on
        // incrementing the table, and signaling
        // the fill semaphore for the consumer side
        // which will cause it to wait if there are
        // no items in the shared memory.

                //Number of times to produce
        int loop = 20;

                //Print statement of the number of items the producer
                //will produce
        printf("\nProducer ready to create %d items.\n", loop);

        for(int i = 1; i <= loop; ++i) {

                        //Producer must wait until there are
                        //available spots to produce
                        //(avail != 0 - i.e. not full)
                sem_wait(avail);

                        //Generate a random number of milliseconds to sleep
                       //(%2 + 1 --> either 1 (0+1) or 2 (1+1))
                int ms = rand() % 2 + 1;

                        //Sleep that many milliseconds
                sleep(ms);

                        //Producer must wait until no process is in the critical
                        //section (i.e. mutex != 0 / wait does not
                        //drop mutex semaphore to < 0) - mutual exclusion
                        //(wait == --)
                sem_wait(mutex);

                        //Increment the memory address to symbolize the
                        //production of a new item and the update of the
                        //new first remaining spot to produce
                ++(*table);

                        //Print statement of item production and an updates of
                        //the number of items in the table
                printf("Item produced, there are now %d item(s) in the table.\n", *table);

                        //Exit the critical section and update accordingly (mutex)
                sem_post(mutex);

                        //Update the new number of items in the map with fill
                        //(post == ++)
                sem_post(fill);
        }

        //Close and unlink semaphores
        //Deallocate shared memory

        //Close the named semaphore at the given pointers
        sem_close(fill);
        sem_close(avail);
        sem_close(mutex);

       //Deallocate the resources used to created the named semaphores
        sem_unlink("fill");
        sem_unlink("available");
        sem_unlink("mutex");

        //Deallocate all of the resources (32 bits - size of int)
        //used to form the map
        munmap(table, sizeof(int));

        //Close the shared block of memory
        close(shm_fd);

        //Deallocate the resources used for the given shared block of memory
        shm_unlink("table");

        //Print statement indicating producer cleanup has finished
        printf("Producer2 cleaned up!");
        return 0;
}

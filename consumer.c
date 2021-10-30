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
        // Allocated shared memory and semaphores
                //Open a block of shared memory named "table",
                //created if it did not already exist,
                //with the mode set to all bits for read or write
        int tbl = shm_open("table", O_CREAT, 0666);

                //Set the size of the block of shared memory
                //to be the size of the number of bits an int has (32)
        ftruncate(tbl,sizeof(int));

                //Create a mapping of a virtual adress space,
		//(default of 0 indicates -)
                //kernel decides where the mapping should be
                //the size of the mapping 32 bits (size of an int)
                //allow for other processes to read and write
                //the bits of the mapping
                //all updates in the mapping are shared between processes
        int *table = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, tbl, 0);

                //Initialize the semaphores, each with a pointer to it,
                //with a specified "name",
                //created if not existing already,
                //all bits able to be read or written
                //and initialized with the intial value

                        //fill - indicates the current number of elements
                        //in the map, starts off as empty (0)
        sem_t *fill = sem_open("fill", O_CREAT, 0666, 0);

                        //avail - indicates the number of spots remaning
                        //in the map, starts off as all (2)
        sem_t *avail = sem_open("available", O_CREAT, 0666, 2);

                        //mutex - a mutex semaphore, restricting access to the
                        //the map (critical section) to only ONE process at a time
                        //(mutual exclusion) - indicates the number of reserved
                        //spaces remaining for process (to enter the critical
                        //section) - MAX 1
        sem_t *mutex = sem_open("mutex", O_CREAT, 0666, 1);

        // Here we wait until space is available in 
        // the shared buffer. We then wait a random 
        // number of ms, use mutual exclusion on
        // incrementing the table, and signaling
        // the fill semaphore for the consumer side
        // which will cause it to wait if there are
        // no items in the shared memory.

                //Number of elements the consumer has to consumer (20)
        int loop = 20;

                //Print state that indicates the number of elements the consumer
                //will consume (in total) 
        printf("\nConsumer ready to receive %d items.\n", loop);

                //consume for 20 times
        for(int i = 1; i <= loop; ++i)
        {
                        //consumer must wait if semaphore is empty (nothing
                        //to consume) - i.e. fill = 0
                                //function will decrement the semaphore's value
                                //only if that will result in a nonnegtive value
                                //else it waits for fill to be incremented to
                                //satisfy the condition (wait == --)
                sem_wait(fill);

//Debugging - printf("unpaused\n");

                                //Generate a random number of milliseconds
                                //(% 2 + 1) will result in an equal chance of
                                //either 1 (0+1) or 2 (1+1) milliseconds
                int ms = rand() % 2 + 1;

//Debugging - printf("%d", ms);

                                //Process goes to sleep for that many milliseconds
                sleep(ms);

//Debuggin - printf("Awoke\n");

                //Entering the critical section

                        //Semaphore must wait if there is another process
                        //in the critical section
                sem_wait(mutex);

                        //Decrement the array  to reflect the new position
                        //of the first element in the stack (reflects the
                        //consumption of the old first element of the stack) 
                --(*table);

                        //Print statement to indicate consumption has occurred
                        //and the updated status of the map (number of
                        //elements remaining)
                printf("Item consumed, there are now %d item(s) remaining in the table\n", *table);

                        //Updating mutex semaphore to reflect the exiting of
                        //the critical section (wait == ++)
                sem_post(mutex);

                        //Updating the status of the number of spots remaining
                        //in the map
                sem_post(avail);
        }

        //Close and unlink semaphores
        //Deallocate shared memory
        //Because consumer will always be after producer in terms
        //of reaching the goal of 20 items, it should take clean up the
        //semaphores and shared memory as to ensure that processes do not
        //close or free a semaphore before others are finished using them

                //Close the named semaphore at the address of the given pointer
        sem_close(fill);
        sem_close(avail);
        sem_close(mutex);

                //Free the resources used to instantiate the semaphores with th$
        sem_unlink("fill");
        sem_unlink("available");
        sem_unlink("mutex");

                //Delete the specified mapping at the given location entirely
                //size-length of map --> all elements of map are deleted
        munmap(table, sizeof(int));

                //Close the block of shared memory at the given pointer
        close(tbl);

                //Free the resources used to instantiate the named shared
                //block of memory
		shm_unlink("table");

                //Print statement to indicate consumer cleanup has finished
        printf("Consumer cleaned up!\n");
        return 0;
}

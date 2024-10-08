# Exercise 7

* Write a program that will use shared memory mapped to a file as a backing store.
* When the program is activated with no arguments it will create the backing store file.
    It will read messages and will store them in memory mapped to the backing store file.
* When the program is activated with more than 1 argument it will use the backing store file.
    It will read the messages from memory mapped to the existing backing store file and will display them.

## Exercise 7a
* Write a program similar to Exercise 7.
* Messages will be written to 3 different shared memory mapped to 3 different backing store files.
    * Each message line will have a type (one digit integer) and message text.
    * Messages having type 1 or 2 will be stored in the first shared memory.
    * Messages having type 3 or 4 will be stored in the second shared memory.
    * Messages having any other type (except 1,2,3 or 4) will be stored in the third shared memory.
* When the program is activated with a parameter it will map shared memory to the three existing backing
    store files and will display the messages stored in each one of them.
Hint: You must take care of the addresses to each of shared memories.
    use `ACE_MMAP_Memory_Pool_Options::FIRSTCALL_FIXED`
    and `ACE_MMAP_Memory_Pool_Options::NEVER_FIXED`

## Exercise 7b
* Write a program that will use Active Object for the following operation.
* The main part will read messages and will send it to a queue according to the following conditions:
    a. Each message line will have a type (one digit integer) and message text.
    b. Messages having type 1 or 3 will be sent to the tail of the queue.
    c. Messages having type 2 or 4 will be sent to the head of the queue.
    d. Messages having any other type (except 1,2,3 or 4) will be sent to the queue with priority 10.
3. The data will be read by the ACE_Method_Request and will display each message that is being read.
4. Verify if the order of the displayed messages is correct.

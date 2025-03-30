# Producer-Consumer Problem with Bounded Buffer

## Table of Contents

- [Purpose](#purpose)
- [Key Components](#key-components)
  - [Synchronization](#synchronization)
  - [Bounded Buffer](#bounded-buffer)
  - [Semaphores](#semaphores)
  - [Mutex](#mutex)
- [How It Works](#how-it-works)
  - [Producer Thread](#producer-thread)
  - [Consumer Thread](#consumer-thread)
  - [Error Handling](#error-handling)
- [Compilation & Usage](#compilation--usage)
  - [Requirements](#requirements)
  - [Compile the Program](#compile-the-program)
  - [Run the Program](#run-the-program)
  - [Output](#output)
- [Conclusion](#conclusion)

---

## Purpose

This project implements the **Producer-Consumer Problem** with a **bounded buffer**, using **semaphores** and **mutexes** for synchronization. The problem involves two types of threads—**producers** and **consumers**—that share a fixed-size buffer. The producers add items to the buffer, and consumers remove items from it. The synchronization mechanisms ensure that:

- Producers do not produce if the buffer is full.
- Consumers do not consume if the buffer is empty.
- Both producer and consumer threads do not interfere with each other when accessing the shared buffer.

This project demonstrates how to synchronize concurrent threads using semaphores and mutexes to avoid race conditions and ensure proper coordination between producers and consumers.

---

## Key Components

### Synchronization
- In concurrent programming, synchronization refers to mechanisms used to ensure that multiple threads or processes operate in a coordinated manner, avoiding conflicts when they access shared resources. Without synchronization, issues such as race conditions, deadlocks, and data corruption can occur when threads interfere with each other.
- In this project, synchronization is achieved using semaphores and mutexes to manage access to the shared bounded buffer. These tools ensure that:

   - Producers cannot add items when the buffer is full.

   - Consumers cannot remove items when the buffer is empty.

   - Threads do not interfere with each other when accessing the shared buffer.

   - Semaphores (empty and full) help coordinate the producers and consumers by keeping track of buffer space and item availability. They allow threads to wait until a certain condition is met (e.g., when there is space in the buffer or an item to consume).

   - Mutexes ensure that only one thread (either a producer or consumer) can access the shared buffer at a time, preventing race conditions.
- Through this synchronized approach, the program guarantees safe and efficient communication between producer and consumer threads, allowing them to operate correctly without overlapping inappropriately.

### Bounded Buffer
- The shared buffer is of a fixed size (`BUFFER_SIZE`), which can hold a certain number of items.
- **Producers** insert items into the buffer, and **consumers** remove items from the buffer.

### Semaphores
- **`empty` semaphore:** Keeps track of available empty slots in the buffer for producers to insert items.
- **`full` semaphore:** Keeps track of items available in the buffer for consumers to remove.
- **Functionality:** Semaphores prevent producers from adding items when the buffer is full and prevent consumers from removing items when the buffer is empty.

### Mutex
- Ensures **mutual exclusion** when accessing and modifying the shared buffer.
- Only one thread (either producer or consumer) can access the buffer at a time to prevent race conditions.

---

## How It Works

### Producer Thread
1. The producer thread will **wait** until there is space in the buffer (`sem_wait(&empty)`).
2. It then acquires the mutex lock (`pthread_mutex_lock(&mutex)`) and inserts the item into the buffer.
3. After modifying the buffer, it releases the mutex (`pthread_mutex_unlock(&mutex)`) and signals that the buffer has a new item (`sem_post(&full)`).

### Consumer Thread
1. The consumer thread will **wait** until there is an item in the buffer to consume (`sem_wait(&full)`).
2. It acquires the mutex lock (`pthread_mutex_lock(&mutex)`), removes an item from the buffer, and releases the mutex (`pthread_mutex_unlock(&mutex)`).
3. After consuming the item, it signals that space is available in the buffer (`sem_post(&empty)`).

### Error Handling
- If the producer tries to insert an item when the buffer is full, or the consumer tries to remove an item when the buffer is empty, the threads will block until space or items are available.

---

## Compilation & Usage

### Requirements
- GCC or any C compiler supporting POSIX threads and semaphores.
- Make sure you have pthreads and semaphore libraries installed on your system.

### Compile the Program
```bash
gcc -o buffer buffer.c -lpthread
```
### Run the Program
```
./buffer <buffer_size> <num_producers> <num_consumers>
```
### Example
```
./buffer 5 3 3
```
### Output
```
producer produced 93
consumer consumed 93
producer produced 86
consumer consumed 86
...
```
### Conclusion
This implementation demonstrates how synchronization tools like semaphores and mutexes can be used to solve the bounded buffer problem in concurrent programming. The project ensures that producers and consumers operate in a coordinated manner, preventing race conditions and ensuring safe access to the shared buffer.



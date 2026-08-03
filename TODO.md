# Codexion TODO

---

# Day 1 - Understand the Project

## Subject
- [X] Read the subject completely
- [X] Understand the coder lifecycle
- [X] Understand all program arguments
- [X] Understand the mandatory requirements

## Threads Basics
- [X] Learn what a thread is
- [X] Learn pthread_create()
- [X] Learn pthread_join()

## Project
- [X] Create project structure
- [X] Create Makefile
- [X] Create header files

---

# Day 2 - Design

## Structures
- [X] Design simulation struct
- [X] Design coder struct
- [X] Design dongle struct
- [X] Decide shared vs private data
- [X] Decide ownership of resources

## Parsing
- [X] Parse command line arguments
- [X] Validate arguments
- [X] Handle invalid inputs

---

# Day 3 - Threads

## Threads
- [X] Create one thread per coder
- [X] Create monitor thread
- [x] Create coder routine
- [X] Create monitor routine
- [X] Join all threads
- [X] Test thread creation
---

# Day 4 - Mutexes

## Dongles
- [X] Create one mutex per dongle
- [X] Lock mutex
- [X] Unlock mutex

## Mutexes
- [X] Initialize mutexes
- [X] Destroy mutexes

## Logging
- [X] Create print mutex

## Testing
- [X] Test race conditions

---

# Day 5 - Simulation

## Coder Routine
- [X] Take left dongle
- [X] Take right dongle
- [X] Compile
- [X] Update last_compile_start
- [X] Increase compile count
- [X] Release both dongles
- [X] Debug
- [X] Refactor

## Timing
- [X] Implement sleeping
- [X] Print timestamps

---

# Day 6 - Monitor

## Monitor Thread
- [X] Create monitor thread
- [X] Detect burnout
- [X] Stop simulation
- [ ] Protect simulation stop flag

## Timing
- [ ] Burnout within 10ms
- [ ] Test monitor

---

# Day 7 - Dongle Cooldown

## Cooldown
- [ ] Store release time
- [ ] Prevent immediate reuse
- [ ] Test cooldown

## Waiting
- [ ] Add condition variables
- [ ] Wake waiting coders

---

# Day 8 - Scheduler

## Heap
- [ ] Build priority queue
- [ ] Push request
- [ ] Pop request

## FIFO
- [ ] Implement FIFO scheduling

## EDF
- [ ] Implement EDF scheduling
- [ ] Tie-breaker

---

# Day 9 - Finish Mandatory

## Simulation
- [ ] number_of_compiles_required
- [ ] Stop when everyone finishes

## Cleanup
- [ ] Destroy mutexes
- [ ] Destroy condition variables
- [ ] Free memory

## Testing
- [ ] Test edge cases
- [ ] Test with many coders
- [ ] Check for memory leaks
- [ ] Test one coder
- [ ] Test two coders
- [ ] Test invalid arguments

---

# Day 10 - Final

## Documentation
- [ ] Write README

## Quality
- [ ] Norm
- [ ] Valgrind
- [ ] Stress tests

## Evaluation
- [ ] Prepare peer evaluation
- [ ] Review synchronization
- [ ] Review deadlocks
- [ ] Review starvation
- [ ] Review FIFO
- [ ] Review EDF
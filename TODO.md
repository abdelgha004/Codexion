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
- [ ] Design simulation struct
- [ ] Design coder struct
- [ ] Design dongle struct

## Parsing
- [ ] Parse command line arguments
- [ ] Validate arguments
- [ ] Handle invalid inputs

---

# Day 3 - Threads

## Coder Threads
- [ ] Create one thread per coder
- [ ] Create coder routine
- [ ] Join all threads
- [ ] Test thread creation

---

# Day 4 - Mutexes

## Dongles
- [ ] Create one mutex per dongle
- [ ] Lock mutex
- [ ] Unlock mutex

## Logging
- [ ] Create print mutex
- [ ] Protect all output

## Testing
- [ ] Test race conditions

---

# Day 5 - Simulation

## Coder Routine
- [ ] Take left dongle
- [ ] Take right dongle
- [ ] Compile
- [ ] Release both dongles
- [ ] Debug
- [ ] Refactor

## Timing
- [ ] Implement sleeping
- [ ] Print timestamps

---

# Day 6 - Monitor

## Monitor Thread
- [ ] Create monitor thread
- [ ] Detect burnout
- [ ] Stop simulation

## Timing
- [ ] Burnout within 10ms
- [ ] Test monitor

---

# Day 7 - Dongle Cooldown

## Cooldown
- [ ] Store release time
- [ ] Prevent immediate reuse
- [ ] Test cooldown

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
# Before pushing: what you should know about this project

## 1. The real problem is not just threads

This project is a concurrency and scheduling problem, not only a C programming exercise.

Each coder is a thread, and each dongle is a shared resource. The real difficulty is not writing pthreads, but making the whole system safe, fair, and deterministic.

The key ideas are:

- shared state must never be accessed concurrently without protection;
- waiting threads must not starve forever;
- deadlock must be prevented by design;
- the simulation must stop correctly when burnout or completion happens;
- the scheduler must choose the next valid request fairly and predictably.

---

## 2. What the threads are doing

Each coder thread follows a loop:

1. request access to its two neighboring dongles;
2. wait until both are valid and eligible;
3. acquire them;
4. compile;
5. release them;
6. debug and refactor;
7. repeat until the required number of compilations is reached or it burns out.

The monitor thread is separate and continuously checks:

- whether all coders are done;
- whether a coder missed its burnout deadline.

This means the project has at least two synchronization actors:

- coders: producers of resource requests and work;
- monitor: global observer and termination controller.

The danger is that they both read and update shared state at the same time, which is where mutexes and condition variables matter.

---

## 3. Why mutexes are mandatory

The code protects shared variables with mutexes such as:

- coder count mutex: protects last_compile_time and compile_count;
- dongle availability mutex: protects is_available, heap state, and release timestamps;
- end mutex: protects the simulation_ends flag;
- print mutex: serializes output so logs are not mixed.

Without these, you would get races like:

- one thread reads last_compile_time while another thread writes it;
- one thread checks if a dongle is available while another thread changes it;
- log lines are interleaved in the middle of different messages.

The important rule is always:

- lock the mutex;
- read or write the shared state;
- unlock the mutex.

This is the basic law of safe multithreaded code.

---

## 4. Why condition variables matter

Each dongle has a condition variable called waiters.

This is used so that waiting threads do not spin endlessly in a useless loop. When a resource state changes, the code broadcasts to waiting threads so they can re-check their condition.

Typical use:

- a dongle is released;
- the code sets is_available to 1;
- it broadcasts on the condition variable;
- waiting coders wake up and decide whether they can proceed.

This is the correct producer/consumer pattern for resource waiting: wake up only when the state changes.

---

## 5. Deadlock theory: why the sorting trick matters

A deadlock happens when threads wait on each other in a cycle.

In this project, each coder needs two dongles. If thread A locks dongle 2 and wants dongle 5, while thread B locks dongle 5 and wants dongle 2, you get circular waiting.

The project prevents this by sorting the two dongles before locking them:

- first = smaller index;
- second = larger index;

Then all threads lock in the same order.

This prevents Coffman’s circular-wait condition.

In other words:

- mutual exclusion is unavoidable;
- hold-and-wait is possible, but controlled;
- circular wait is prevented by consistent ordering;
- no deadlock cycle can form as long as all threads respect the same ordering rule.

This is one of the most important theoretical points to know before pushing.

---

## 6. Starvation theory: fairness and waiting queues

Starvation means a thread waits forever because other requests keep being served first.

The project avoids this with a heap per dongle.

Each waiting request has:

- coder id;
- enter_time;
- time_to_burnout.

And the heap orders requests using the scheduler:

- FIFO: earlier enter_time wins;
- EDF: earlier time_to_burnout wins;
- id is used as a tiebreaker.

So the scheduler chooses the next request fairly and in a deterministic way.

What to remember:

- FIFO gives fairness by arrival order;
- EDF gives urgency by deadline;
- the heap keeps the “best” request always at the root.

This prevents a request from being constantly delayed behind newer ones.

---

## 7. Binary heap theory for this project

A binary heap is a complete tree stored in an array.

For this project, the heap is used as a priority queue:

- root = highest priority request;
- parent is always higher priority than its children;
- insertion uses heapify_up;
- extraction uses heapify_down.

This allows fast operations:

- push request: O(log n)
- pop highest priority: O(log n)
- remove request: O(log n) in the normal heap approach

Key theoretical point: the heap keeps the queue ordered without scanning all entries each time.

That is why it is ideal for a resource scheduler in a multithreaded simulation.

---

## 8. Cooldown logic: why it is not optional

A just-released dongle is not immediately reusable.

The code stores last_release_time for each dongle and checks:

- now - last_release_time >= dongle_cooldown

This prevents the same dongle from being rapidly reacquired by another thread and avoids invalid oscillation in the scheduling logic.

This is important because the project is simulating a real system where resources are not instantly reusable. The cooldown is a time-based safety rule, not just a tuning parameter.

---

## 9. Burnout detection logic

The monitor checks each coder’s last compile time.

It is trying to answer:

- “Has this coder exceeded the allowed time since its last successful compile?”

The condition is:

- last_compile_time + time_to_burnout < current_time

If true, the coder burned out and the simulation ends.

This is a deadline-monitoring problem. It matters because the monitor must check the global state without racing with coder updates.

This is why the coder mutex is used when reading or updating the timestamp.

---

## 10. Why the end state is protected

The simulation_ends flag is shared across threads. It is updated by the monitor and read by many coders.

Because of that, it must be under a mutex. Otherwise, one thread may read stale or inconsistent state and keep waiting even after the simulation has ended.

The pattern is:

- monitor sets simulation_ends under end_mutex;
- coders check it under the same protection;
- all waiting threads are broadcast and woken up.

This is a clean synchronization pattern for graceful shutdown.

---

## 11. Why the print mutex is essential

The output is protected by a print mutex because logs are shared global state.

Without this, a line like:

- coder 2 is compiling

can be interleaved with:

- coder 6 burned out

and the log becomes unreadable and misleading.

Project requirement: logs must be clean and serialized. This is part of correctness, not just aesthetics.

---

## 12. What to know before pushing

Before submission, you should be able to explain these clearly:

- how each thread gets access to two resources without deadlock;
- why the resource ordering matters;
- why mutexes protect coder time and dongle state;
- why condition variables are used for waiting threads;
- how the heap implements FIFO or EDF scheduling;
- why cooldown and burnout detection are both time-sensitive;
- why the monitor and coder threads must synchronize on shared state;
- why the print mutex is necessary for log safety.

If you can explain those points in plain English, you understand the project.

---

## 13. Short theory summary

This project combines four important concepts:

1. thread synchronization;
2. deadlock prevention;
3. fair resource scheduling;
4. deadline-based monitoring.

The core model is:

- threads compete for shared resources;
- requests are queued in a heap;
- mutexes protect shared data;
- condition variables wake waiting threads;
- the monitor ends the simulation when the deadline or completion condition is reached.

That is the real theory behind Codexion.

---

## 14. Final advice

Do not think of this project as just “threads and C.”

Think of it as:

- a resource-allocation system;
- a priority scheduler;
- a deadlock-safe concurrency model;
- a monitor-driven distributed state machine.

If you understand those concepts, your push is not just code—it is a coherent, defensible concurrency design.

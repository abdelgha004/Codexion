# How to explain the project

## Short version

This project simulates a concurrent resource-allocation system. Several coder threads compete to acquire two shared USB dongles before they can compile. Each dongle is a shared resource, and each coder must wait until it is allowed to use both resources safely. A monitor thread supervises the simulation and detects burnout or completion conditions.

The main challenge is to keep the system correct under concurrency. We must avoid deadlock, avoid starvation, prevent race conditions, and ensure that the logs stay consistent. The project uses threads, mutexes, condition variables, and a priority heap scheduler to control access to the shared dongles.

---

## The problem in plain English

Imagine several developers working on the same machine, each needing two USB dongles to compile. If two developers try to take the same dongles at the same time, there can be conflicts. If the program does not control access properly, threads may block forever, read invalid values, or print inconsistent logs.

The project is a simulation of this problem. Each coder is a thread, each dongle is a shared resource, and the scheduler decides which waiting request should be served next.

---

## Important concepts

### 1. Threads

Each coder is represented by a pthread. They execute the same logic in parallel: request dongles, wait if needed, acquire them, compile, release them, debug, refactor, and repeat.

This introduces concurrency: several threads are active at the same time and all share global state, so synchronization is necessary.

### 2. Shared resources

The dongles are the critical shared resources. A coder cannot compile unless it holds both required dongles. If the program does not coordinate access, multiple coders could try to take the same dongle at once, causing data races or inconsistent behavior.

### 3. Mutexes

Mutexes protect shared data. For example:

- the coder’s last_compile_time and compile_count are protected by a coder mutex;
- the dongle state and heap are protected by a dongle mutex;
- the global end state is protected by an end mutex;
- logs are protected by a print mutex.

This prevents concurrent modification of shared information.

### 4. Condition variables

Each dongle has a condition variable. When a dongle is released or its waiting queue changes, we broadcast to waiting threads so they wake up and re-check whether they can continue. This avoids busy-waiting and allows threads to sleep efficiently until the resource becomes available.

### 5. Deadlock prevention

A deadlock is a situation where threads wait on each other forever. In this project, each coder needs two dongles. If Thread A holds dongle 2 and waits for dongle 5, while Thread B holds dongle 5 and waits for dongle 2, then a circular wait occurs.

To prevent this, the project sorts the two dongles before locking them. All threads follow the same ordering rule, so they never create a circular dependency.

### 6. Starvation prevention

Starvation happens when a thread waits forever because others always take priority. The program prevents this by using a heap-based priority queue per dongle. The scheduler decides which request is served next:

- FIFO: earliest arrival first;
- EDF: earliest burnout deadline first;
- ID is used as a tie-breaker.

This ensures fair and deterministic scheduling.

### 7. Binary heap

The heap is used as a priority queue. It keeps the highest-priority waiting request at the root and lets us efficiently insert and remove requests.

The binary heap is important because the scheduler must be fast and consistent, especially when many coders are waiting for a dongle.

### 8. Burnout detection

The monitor thread periodically checks whether a coder has exceeded its allowed time since the last successful compile. If it has, the thread is considered burned out and the simulation ends.

This is a timing-based safety mechanism. It prevents a coder from continuing indefinitely if it cannot keep up with the required workload.

### 9. Cooldown

A dongle cannot be reused immediately after it is released. The project enforces a cooldown time. This prevents rapid re-acquisition and keeps the scheduling model realistic and stable.

### 10. Log serialization

All output is protected by a print mutex. Without this, lines from different threads could be mixed together, making the log unreadable and invalid. This is important because the simulation is highly concurrent and the logs are used to trace what happened.

---

## How to explain the architecture

A good explanation is:

"The project is a multithreaded resource-allocation simulation. Each coder is a thread that requests two dongles, waits if necessary, and then compiles. The system uses mutexes to protect shared state, condition variables to wake waiting threads, and a heap-based scheduler to decide which request is served next. The monitor checks if the simulation must end due to burnout or successful completion. The design prevents deadlock, starvation, and race conditions by enforcing a consistent lock order, using fair scheduling, and serializing log output."

---

## Very short oral answer

"This project simulates multiple coders competing for shared dongles. The challenge is to coordinate access safely and fairly. We use pthreads for each coder, mutexes for shared state, condition variables for waiting threads, and a binary heap scheduler to prioritize requests. A monitor checks deadlines and termination conditions. The key point is that we prevent deadlock by locking dongles in a fixed order, prevent starvation by using fair priority ordering, and protect logs and timestamps with mutexes to avoid races."

---

## If you want to sound strong in front of staff

You can say:

"The project is a realistic concurrency problem: multiple threads compete for limited resources under timing constraints. The complexity is not just in synchronization, but in ensuring fairness, deadlock prevention, and deterministic behavior. The implementation uses a custom heap scheduler, per-resource condition variables, and strict mutex protection around shared state. The reasoning is based on classic operating systems concepts: mutual exclusion, deadlock avoidance, starvation prevention, and resource scheduling."

---

## Final idea to remember

When explaining the project, do not focus only on the code. Focus on the concurrency principles behind it:

- shared resources need coordination;
- waiting threads need fairness;
- deadlines require monitoring;
- race conditions are prevented with locks;
- deadlock is avoided with ordering rules;
- logs are serialized to remain meaningful.

That is the real project story.

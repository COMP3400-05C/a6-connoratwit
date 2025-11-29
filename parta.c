#include "parta.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Initializes an array of PCBs from an array of CPU bursts.
 * Each PCB is assigned a PID (index), burst_left (from bursts array),
 * and wait time (initially 0).
 * Returns dynamically allocated array of PCBs.
 */
struct pcb* init_procs(int* bursts, int blen) {
    struct pcb* procs = (struct pcb*)malloc(blen * sizeof(struct pcb));
    
    for (int i = 0; i < blen; i++) {
        procs[i].pid = i;
        procs[i].burst_left = bursts[i];
        procs[i].wait = 0;
    }
    
    return procs;
}

/**
 * Prints all PCB information for debugging purposes.
 * Shows PID, burst_left, and wait time for each process.
 */
void printall(struct pcb* procs, int plen) {
    printf("PID\tBurst Left\tWait\n");
    for (int i = 0; i < plen; i++) {
        printf("%d\t%d\t\t%d\n", procs[i].pid, procs[i].burst_left, procs[i].wait);
    }
    printf("\n");
}

/**
 * Simulates running a process for a given amount of time.
 * Decreases the current process's burst_left by amount,
 * and increases all other processes' wait time by amount.
 */
void run_proc(struct pcb* procs, int plen, int current, int amount) {
    // Reduce current process's burst_left
    procs[current].burst_left -= amount;
    
    // Increase wait time for all other processes
    for (int i = 0; i < plen; i++) {
        if (i != current && procs[i].burst_left > 0) {
            procs[i].wait += amount;
        }
    }
}

/**
 * Implements First-Come-First-Serve scheduling algorithm.
 * Runs each process to completion in order (starting from PID 0).
 * Returns total time elapsed.
 */
int fcfs_run(struct pcb* procs, int plen) {
    int time = 0;
    
    for (int i = 0; i < plen; i++) {
        int burst = procs[i].burst_left;
        run_proc(procs, plen, i, burst);
        time += burst;
    }
    
    return time;
}

/**
 * Finds the next process to run in round-robin scheduling.
 * Returns the next process with burst_left > 0, wrapping around.
 * Returns -1 if all processes are complete.
 */
int rr_next(int current, struct pcb* procs, int plen) {
    // Check if all processes are complete
    int all_done = 1;
    for (int i = 0; i < plen; i++) {
        if (procs[i].burst_left > 0) {
            all_done = 0;
            break;
        }
    }
    
    if (all_done) {
        return -1;
    }
    
    // Find next process with burst_left > 0
    int next = (current + 1) % plen;
    
    while (procs[next].burst_left == 0) {
        next = (next + 1) % plen;
    }
    
    return next;
}

/**
 * Implements Round-Robin scheduling algorithm.
 * Each process runs for at most 'quantum' time units before switching.
 * Returns total time elapsed.
 */
int rr_run(struct pcb* procs, int plen, int quantum) {
    int time = 0;
    int current = 0;
    
    // Find first process with burst_left > 0
    while (current < plen && procs[current].burst_left == 0) {
        current++;
    }
    
    if (current >= plen) {
        return 0; // No processes to run
    }
    
    while (1) {
        // Determine how much time to run current process
        int run_time = procs[current].burst_left < quantum ? 
                       procs[current].burst_left : quantum;
        
        // Run the process
        run_proc(procs, plen, current, run_time);
        time += run_time;
        
        // Get next process
        int next = rr_next(current, procs, plen);
        
        if (next == -1) {
            break; // All processes complete
        }
        
        current = next;
    }
    
    return time;
}
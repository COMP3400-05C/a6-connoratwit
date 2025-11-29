#include "parta.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // Check minimum arguments
    if (argc < 2) {
        printf("ERROR: Missing arguments\n");
        return 1;
    }
    
    char* algorithm = argv[1];
    
    // Handle FCFS
    if (strcmp(algorithm, "fcfs") == 0) {
        if (argc < 3) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }
        
        printf("Using FCFS\n\n");
        
        // Parse burst times
        int blen = argc - 2;
        int* bursts = (int*)malloc(blen * sizeof(int));
        
        for (int i = 0; i < blen; i++) {
            bursts[i] = atoi(argv[i + 2]);
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }
        
        // Initialize processes and run scheduler
        struct pcb* procs = init_procs(bursts, blen);
        fcfs_run(procs, blen);
        
        // Calculate average wait time
        double total_wait = 0;
        for (int i = 0; i < blen; i++) {
            total_wait += procs[i].wait;
        }
        double avg_wait = total_wait / blen;
        
        printf("Average wait time: %.2f\n", avg_wait);
        
        // Clean up
        free(bursts);
        free(procs);
    }
    // Handle Round-Robin
    else if (strcmp(algorithm, "rr") == 0) {
        if (argc < 4) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }
        
        int quantum = atoi(argv[2]);
        printf("Using RR(%d).\n\n", quantum);
        
        // Parse burst times
        int blen = argc - 3;
        int* bursts = (int*)malloc(blen * sizeof(int));
        
        for (int i = 0; i < blen; i++) {
            bursts[i] = atoi(argv[i + 3]);
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }
        
        // Initialize processes and run scheduler
        struct pcb* procs = init_procs(bursts, blen);
        rr_run(procs, blen, quantum);
        
        // Calculate average wait time
        double total_wait = 0;
        for (int i = 0; i < blen; i++) {
            total_wait += procs[i].wait;
        }
        double avg_wait = total_wait / blen;
        
        printf("Average wait time: %.2f\n", avg_wait);
        
        // Clean up
        free(bursts);
        free(procs);
    }
    else {
        printf("ERROR: Missing arguments\n");
        return 1;
    }
    
    return 0;
}   
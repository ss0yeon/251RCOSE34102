#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "structure.h"

Process backup_list[MAX_PROCESSES];  // 백업

void backup_processes() {
    for (int i = 0; i < process_count; i++) {
        backup_list[i] = process_list[i];
    }
}

void restore_processes() {
    for (int i = 0; i < process_count; i++) {
        process_list[i] = backup_list[i];
    }
}

int main() {
    int quantum;

    printf("===== CPU Scheduling Simulator =====\n");

    do {
        printf("Enter number of processes (max %d): ", MAX_PROCESSES);
        scanf("%d", &process_count);
        if (process_count > MAX_PROCESSES || process_count <= 0) {
            printf("Invalid input. Please enter a number between 1 and %d.\n", MAX_PROCESSES);
        }
    } while (process_count > MAX_PROCESSES || process_count <= 0);


    printf("Enter Time Quantum for Round Robin: ");
    scanf("%d", &quantum);

    Create_Process();
    backup_processes();  // 원본 process 상태 저장

    // 1. FCFS
    printf("\n--- FCFS ---\n");
    restore_processes();
    FCFS();
    print_gantt_chart();
    Evaluation();

    // 2. SJF
    printf("\n--- SJF (Non-preemptive) ---\n");
    restore_processes();
    SJF();
    print_gantt_chart();
    Evaluation();

    // 3. SJF Preemptive
    printf("\n--- SJF (Preemptive) ---\n");
    restore_processes();
    SJF_Preemptive();
    print_gantt_chart();
    Evaluation();

    // 4. Priority
    printf("\n--- Priority (Non-preemptive) ---\n");
    restore_processes();
    Priority();
    print_gantt_chart();
    Evaluation();

    // 5. Priority Preemptive
    printf("\n--- Priority (Preemptive) ---\n");
    restore_processes();
    Priority_Preemptive();
    print_gantt_chart();
    Evaluation();

    // 6. Round Robin
    printf("\n--- Round Robin (Quantum = %d) ---\n", quantum);
    restore_processes();
    RoundRobin(quantum);
    print_gantt_chart();
    Evaluation();

    return 0;
}
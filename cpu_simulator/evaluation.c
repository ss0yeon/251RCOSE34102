#include <stdio.h>
#include "structure.h"

void logging(int pid) {
    char type = 'P';

    if (pid < 0) {
        type = 'I';
        pid = -pid; // -pid로 받고 다시 원래대로 복구
    }

    if (gantt_count > 0 &&
        gantt_chart[gantt_count - 1].pid == pid &&
        gantt_chart[gantt_count - 1].type == type) {
        gantt_chart[gantt_count - 1].end_time += 1;
    } else {
        gantt_chart[gantt_count++] = (GanttEntry){
            .pid = pid,
            .start_time = current_time,
            .end_time = current_time + 1,
            .type = type
        };
    }
}



void print_gantt_chart() {
    printf("\n===== Gantt Chart =====\n");

    int current = 0;

    printf("Time : ");
    for (int i = 0; i < gantt_count; i++) {
        GanttEntry g = gantt_chart[i];

        // idle gap 앞에 출력
        for (int t = current; t < g.start_time; t++) {
            printf("%2d ", t);
        }

        for (int t = g.start_time; t < g.end_time; t++) {
            printf("%2d ", t);
        }

        current = g.end_time;
    }
    printf("%2d\n", current); // 마지막 시간 표시

    current = 0;
    
    printf("Gantt: ");
    for (int i = 0; i < gantt_count; i++) {
        GanttEntry g = gantt_chart[i];

        // idle 구간 출력
        for (int t = current; t < g.start_time; t++) {
            printf(" -  ");
        }

        // 실제 작업 출력
        char label = g.type;
        for (int t = g.start_time; t < g.end_time; t++) {
            printf("%c%-2d", label, g.pid);
        }

        current = g.end_time;
    }

    printf("\n==================\n\n");
}



// wating_time, turnaround_time에 대한 평가
void Evaluation() {
    printf("\n===== Evaluation Result =====\n");

    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    for (int i = 0; i < process_count; i++) {
        Process* p = &process_list[i];
        total_waiting_time += p->waiting_time;
        total_turnaround_time += p->turnaround_time;

        printf("P%-2d | Waiting Time: %-2d | Turnaround Time: %-2d\n",
               p->pid, p->waiting_time, p->turnaround_time);
    }

    double avg_waiting_time = (double)total_waiting_time / process_count;
    double avg_turnaround_time = (double)total_turnaround_time / process_count;

    printf("\nAverage Waiting Time    : %.2f\n", avg_waiting_time);
    printf("Average Turnaround Time : %.2f\n", avg_turnaround_time);

    printf("======================\n\n");
}
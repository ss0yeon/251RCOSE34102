#include <stdio.h>
#include "structure.h"


// ready queue에 도착한 process 업데이트
void check_arrivals() {
    for (int i = 0; i < process_count; i++) {
        if (process_list[i].arrival_time == current_time
            && !process_list[i].enqueued) {
            enqueue(&ready_queue, process_list[i].pid);
            process_list[i].enqueued = 1;
        }
    }
}

// io 진행시키고, 끝난 process 다시 ready queue로 복귀
void check_io_completion() {
    if (!is_empty(&waiting_queue)) {
        int pid = waiting_queue.data[waiting_queue.front];
        Process* p = &process_list[pid-1];
        p->io_burst[p->io_index]--;

        if (p->io_burst[p->io_index] == 0) {
            p->io_index++;
            dequeue(&waiting_queue);
            enqueue(&ready_queue, pid);
        }
    }
}


void FCFS() {

    Config();

    while (completed < process_count) {

        check_arrivals();
        check_io_completion();

        if (!is_empty(&ready_queue)) {
            int pid = dequeue(&ready_queue);
            Process* p = &process_list[pid-1];

            if (p->start_time == -1) p->start_time = current_time;

            while (p->remaining_time > 0) {
                // io 처리
                if (p->io_index < p->io_count &&
                    p->io_request_time[p->io_index] == (p->cpu_burst - p->remaining_time)) {

                    enqueue(&waiting_queue, pid);
                    logging(-pid);
                    break;
                }
                logging(pid);
                current_time++;
                p->remaining_time--;

                check_arrivals();
                check_io_completion();
            }

            if (p->remaining_time == 0) {
                p->end_time = current_time;
                p->turnaround_time = p->end_time - p->arrival_time;
                p->waiting_time = p->turnaround_time - p->cpu_burst;
                completed++;
            }

        } else {    // ready queue가 비어있음
            current_time++;
        }
    }
}

void SJF() {

    Config();

    while (completed < process_count) {

        check_arrivals();
        check_io_completion();

        // ready_queue 정렬
        sort_queue(&ready_queue, "remaining_time");

        if (!is_empty(&ready_queue)) {
            int pid = dequeue(&ready_queue);
            Process* p = &process_list[pid-1];

            if (p->start_time == -1) p->start_time = current_time;

            while (p->remaining_time > 0) {
                // io 처리
                if (p->io_index < p->io_count &&
                    p->io_request_time[p->io_index] == (p->cpu_burst - p->remaining_time)) {

                    enqueue(&waiting_queue, pid);
                    logging(-pid);
                    break;
                }
                logging(pid);
                current_time++;
                p->remaining_time--;

                check_arrivals();
                check_io_completion();
            }

            if (p->remaining_time == 0) {
                p->end_time = current_time;
                p->turnaround_time = p->end_time - p->arrival_time;
                p->waiting_time = p->turnaround_time - p->cpu_burst;
                completed++;
            }
        } else {    // ready queue가 비어있음
            current_time++;
        }
    }
}

void SJF_Preemptive() {

    Config();

    while (completed < process_count) {

        check_arrivals();
        check_io_completion();

        // ready_queue 정렬
        sort_queue(&ready_queue, "remaining_time");

        if (!is_empty(&ready_queue)) {
            int pid = ready_queue.data[ready_queue.front];
            Process* p = &process_list[pid-1];

            if (p->start_time == -1) p->start_time = current_time;

            // io 처리
            if (p->io_index < p->io_count &&
                p->io_request_time[p->io_index] == (p->cpu_burst - p->remaining_time)) {
                
                dequeue(&ready_queue);
                enqueue(&waiting_queue, pid);
                logging(-pid);

            } else {    // 1초만 실행
                logging(pid);
                p->remaining_time--;
                current_time++;

                if (p->remaining_time == 0) {
                    dequeue(&ready_queue);
                    p->end_time = current_time;
                    p->turnaround_time = p->end_time - p->arrival_time;
                    p->waiting_time = p->turnaround_time - p->cpu_burst;
                    completed++;
                }
            }    
        } else {    // ready queue가 비어있음
            current_time++;
        }
    }
}

void Priority() {

    Config();

    while (completed < process_count) {

        check_arrivals();
        check_io_completion();

        // ready_queue 정렬
        sort_queue(&ready_queue, "priority");

        if (!is_empty(&ready_queue)) {
            int pid = dequeue(&ready_queue);
            Process* p = &process_list[pid-1];

            if (p->start_time == -1)
                p->start_time = current_time;

            while (p->remaining_time > 0) {
                // io 처리
                if (p->io_index < p->io_count &&
                    p->io_request_time[p->io_index] == (p->cpu_burst - p->remaining_time)) {
                    
                    enqueue(&waiting_queue, pid);
                    logging(-pid);
                    break;
                }

                logging(pid);
                current_time++;
                p->remaining_time--;

                check_arrivals();
                check_io_completion();
            }

            if (p->remaining_time == 0) {
                p->end_time = current_time;
                p->turnaround_time = p->end_time - p->arrival_time;
                p->waiting_time = p->turnaround_time - p->cpu_burst;
                completed++;
            }

        } else {    // ready queue가 비어있음
            current_time++;
        }
    }
}

void Priority_Preemptive() {

    Config();

    while (completed < process_count) {
        check_arrivals();
        check_io_completion();

        // ready_queue 우선순위 기준 정렬
        sort_queue(&ready_queue, "priority");

        if (!is_empty(&ready_queue)) {
            int pid = ready_queue.data[ready_queue.front];
            Process* p = &process_list[pid-1];

            if (p->start_time == -1)
                p->start_time = current_time;

            // io 처리
            if (p->io_index < p->io_count &&
                p->io_request_time[p->io_index] == (p->cpu_burst - p->remaining_time)) {
                
                dequeue(&ready_queue);
                enqueue(&waiting_queue, pid);
                logging(-pid);

            } else {    // 1초만 실행
                logging(pid);
                p->remaining_time--;
                current_time++;

                if (p->remaining_time == 0) {
                    dequeue(&ready_queue);
                    p->end_time = current_time;
                    p->turnaround_time = p->end_time - p->arrival_time;
                    p->waiting_time = p->turnaround_time - p->cpu_burst;
                    completed++;
                }
            }
        } else {
            current_time++;
        }
    }
}

void RoundRobin(int quantum) {

    Config();

    while (completed < process_count) {

        check_arrivals();
        check_io_completion();

        if (!is_empty(&ready_queue)) {
            int pid = dequeue(&ready_queue);
            Process* p = &process_list[pid-1];

            if (p->start_time == -1)
                p->start_time = current_time;

            int time_slice = 0;

            while (time_slice < quantum && p->remaining_time > 0) {
                // io 처리
                if (p->io_index < p->io_count &&
                    p->io_request_time[p->io_index] == (p->cpu_burst - p->remaining_time)) {

                    enqueue(&waiting_queue, pid);
                    logging(-pid);
                    break;
                }

                logging(pid);
                p->remaining_time--;
                current_time++;
                time_slice++;

                check_arrivals();
                check_io_completion();
            }

            if (p->remaining_time == 0) {
                p->end_time = current_time;
                p->turnaround_time = p->end_time - p->arrival_time;
                p->waiting_time = p->turnaround_time - p->cpu_burst;
                completed++;
                
            } else if (time_slice == quantum) { // io 때문에 waiting queue에 들어간 경우도 있을 수 있음
                enqueue(&ready_queue, pid);
            }
        } else {    // ready queue가 비어있음
            current_time++;
        }
    }
}

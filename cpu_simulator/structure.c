#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structure.h"

Process process_list[MAX_PROCESSES];
int process_count;

Queue ready_queue;
Queue waiting_queue;

GanttEntry gantt_chart[1000];
int gantt_count;

int current_time;
int completed;


int random_between(int a, int b){
    return a + rand() % (b-a+1);
}


// 프로세스 생성
void Create_Process() {
    srand(time(NULL));

    for (int i = 0; i < process_count; i++) {
        process_list[i].pid = i+1;
        process_list[i].arrival_time = random_between(0, 20);
        process_list[i].cpu_burst = random_between(3, 10);
        process_list[i].remaining_time = process_list[i].cpu_burst;

        process_list[i].io_count = random_between(0, MAX_IO_REQUESTS); // 0~2회
        int lbound = 1;
        int ubound = 0;

        // 1 ~ cpu_burst-1 사이에서 io_request_time 선택
        // 1~3초 io_burst 선택
        for (int j = 0; j < process_list[i].io_count; j++){    
            if (j != 0) lbound = process_list[i].io_request_time[j-1] + 1;
            ubound = process_list[i].cpu_burst - process_list[i].io_count + j;
            process_list[i].io_request_time[j] = random_between(lbound, ubound);
            
            process_list[i].io_burst[j] = random_between(1,3);
        }

        // 나머지 초기화
        for (int j = process_list[i].io_count; j < MAX_IO_REQUESTS; j++) {
            process_list[i].io_request_time[j] = -1;
            process_list[i].io_burst[j] = -1;
        }


        process_list[i].io_index = 0;

        process_list[i].priority = rand() % 10; // 0~9 우선순위
        process_list[i].start_time = -1;
        process_list[i].end_time = 0;
        process_list[i].waiting_time = 0;
        process_list[i].turnaround_time = 0;

        process_list[i].enqueued = 0;
    }

    printf("\n===== Process Information =====\n");
    printf("PID | Arrival | Burst | Priority | I/O Requests\n");
    printf("----+---------+-------+----------+----------------------------\n");

    for (int i = 0; i < process_count; i++) {
        Process* p = &process_list[i];

        printf("P%-2d |   %-5d |  %-4d |    %-6d | ",
               p->pid, p->arrival_time, p->cpu_burst, p->priority);

        if (p->io_count == 0) {
            printf("None");
        } else {
            for (int j = 0; j < p->io_count; j++) {
                printf("(%d, %d)", p->io_request_time[j], p->io_burst[j]);
                if (j < p->io_count - 1) printf(", ");
            }
        }
        printf("\n");
    }
    printf("================================\n\n");

}


// 큐 관련 함수
void init_queue(Queue* q) {
    q->front = 0;
    q->rear = 0;
}

int is_empty(Queue* q) {
    return q->front == q->rear;
}

int is_full(Queue* q) {
    return (q->rear + 1) % QUEUE_SIZE == q->front;
}

void enqueue(Queue* q, int pid) {
    if (!is_full(q)) {
        q->data[q->rear] = pid;
        q->rear = (q->rear + 1) % QUEUE_SIZE;
    }
}

int dequeue(Queue* q) {
    if (!is_empty(q)) {
        int pid = q->data[q->front];
        q->front = (q->front + 1) % QUEUE_SIZE;
        return pid;
    }
    return -1; // 큐가 비어있음
}

void sort_queue(Queue* q, const char* criteria) {
    int size = (q->rear - q->front + QUEUE_SIZE) % QUEUE_SIZE;
    if (size <= 1) return;

    int temp[QUEUE_SIZE];

    // 큐에서 size만큼 pid 추출
    for (int i = 0; i < size; i++) {
        temp[i] = dequeue(q);
    }

    // 선택 정렬
    for (int i = 0; i < size - 1; i++) {
        int min_idx = i;

        for (int j = i + 1; j < size; j++) {
            Process* p1 = &process_list[temp[min_idx]-1];
            Process* p2 = &process_list[temp[j]-1];

            int v1 = 0, v2 = 0;
            if (strcmp(criteria, "priority") == 0) {
                v1 = p1->priority;
                v2 = p2->priority;
            } else if (strcmp(criteria, "remaining_time") == 0) {
                v1 = p1->remaining_time;
                v2 = p2->remaining_time;
            } else {
                return; // 정의되지 않은 기준이면 정렬 생략
            }

            if (v2 < v1) {
                min_idx = j;
            }
        }

        // swap
        if (min_idx != i) {
            int tmp = temp[i];
            temp[i] = temp[min_idx];
            temp[min_idx] = tmp;
        }
    }

    // 정렬된 pid를 다시 큐에 삽입
    for (int i = 0; i < size; i++) {
        enqueue(q, temp[i]);
    }
}


// 큐 초기화, 세팅 초기화
void Config() {

    current_time = 0;
    completed = 0;
    gantt_count = 0;

    init_queue(&ready_queue);
    init_queue(&waiting_queue);
}

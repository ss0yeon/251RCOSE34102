#ifndef HEADER
#define HEADER

#define MAX_PROCESSES 10
#define MAX_IO_REQUESTS 2
#define QUEUE_SIZE MAX_PROCESSES

// 프로세스 구조체 정의
typedef struct {
    int pid;                        // 프로세스 ID
    int arrival_time;              // 도착 시간
    int cpu_burst;                 // 전체 CPU burst 시간
    int remaining_time;            // 남은 CPU burst 시간

    int io_burst[MAX_IO_REQUESTS]; // 각 I/O 작업의 소요 시간
    int io_request_time[MAX_IO_REQUESTS]; // I/O 요청이 발생하는 시간
    int io_index;                  // 다음 I/O 요청 인덱스
    int io_count;                  // I/O 총 발생 횟수

    int priority;                  // 우선순위

    int start_time;                // 처음 CPU를 받은 시간
    int end_time;                  // 종료 시간
    int waiting_time;             // 대기 시간
    int turnaround_time;          // 반환 시간

    int enqueued;
} Process;

// 큐 구조체 정의
typedef struct {
    int data[QUEUE_SIZE];
    int front;
    int rear;
} Queue;

// log기록용 gantt구조체 정의
typedef struct {
    int pid;
    int start_time;
    int end_time;
    char type;  // P(process) I(I/O)
} GanttEntry;

// 전역 변수 선언
extern Process process_list[MAX_PROCESSES];
extern int process_count; // 프로세스 개수
extern Queue ready_queue;
extern Queue waiting_queue;
extern GanttEntry gantt_chart[1000];
extern int gantt_count;
extern int current_time;
extern int completed;

// 함수 선언
void Create_Process();

void init_queue(Queue* q);
int is_empty(Queue* q);
int is_full(Queue* q);
void enqueue(Queue* q, int pid);
int dequeue(Queue* q);
void sort_queue(Queue* q, const char* criteria);

void Config();

void check_arrivals();
void check_io_completion();

void FCFS();
void SJF();
void SJF_Preemptive();
void Priority();
void Priority_Preemptive();
void RoundRobin();

void logging(int pid);
void print_gantt_chart();
void Evaluation();

#endif

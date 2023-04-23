#include <semaphore.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define ISLAND_SIZE 20
#define MAX_NUMBER_OF_PIRATE_GROPUS 5

const char* shm_name = "/shared-memory";

const char* next_section_ready_sem_name = "/next-section-ready-semaphore";
const char* need_next_section_sem_name = "/need-next-section-semaphore";
const char* pirate_group_sem_name = "/pirate-group-semaphore";

typedef struct {
    bool sections[ISLAND_SIZE];
    int next_section_to_check;
    int found_treasure_index;
    pid_t pirate_groups[MAX_NUMBER_OF_PIRATE_GROPUS];
    int active_groups;
} island;

int pirate_group_id;

sem_t* next_section_ready_sem;
sem_t* need_next_section_sem;
sem_t* pirate_group_sem;

island* shm_ptr;

void sigfunc(int sig) {
    if (sig != SIGINT && sig != SIGTERM) {
        return;
    }

    printf("Pirate group %d is stopping it's work\n", pirate_group_id);

    sem_close(next_section_ready_sem);
    sem_close(need_next_section_sem);
    sem_close(pirate_group_sem);

    shm_ptr->active_groups--;

    exit(0);
}

int main(int argc, char **argv) {
    signal(SIGINT, sigfunc);
    signal(SIGTERM, sigfunc);

    // Инициализация генератора псевдослучайных чисел
    srand(time(NULL));

    // Открытие семафоров
    next_section_ready_sem = sem_open(next_section_ready_sem_name, 0);
    need_next_section_sem = sem_open(need_next_section_sem_name, 0);
    pirate_group_sem = sem_open(pirate_group_sem_name, 0);

    // Открытие разделяемой памяти
    int shm_fd = shm_open(shm_name, O_RDWR, 0666);
    shm_ptr = mmap(NULL, sizeof(island), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Код группы пиратов
    sem_wait(pirate_group_sem);

    pirate_group_id = shm_ptr->active_groups;
    shm_ptr->active_groups++;

    sem_post(pirate_group_sem);

    shm_ptr->pirate_groups[pirate_group_id] = getpid();

    while (true) {
        sem_wait(pirate_group_sem);
        sem_wait(next_section_ready_sem);

        int index = shm_ptr->next_section_to_check;

        sem_post(need_next_section_sem);
        sem_post(pirate_group_sem);

        printf("Pirate group %d is searching section %d\n", pirate_group_id, index);

        sleep(rand() % 5 + 1);

        if (shm_ptr->sections[index]) {
            shm_ptr->found_treasure_index = index;
            printf("Pirate group %d found treasure in section %d!\n", pirate_group_id, index);
        }
    }

    return 0;
}

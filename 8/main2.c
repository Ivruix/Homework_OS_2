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
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define ISLAND_SIZE 20
#define MAX_NUMBER_OF_PIRATE_GROPUS 5

#define SHM_KEY 0x1234

#define SEM_KEY 0x1337
#define SEM_NEXT_SECTION_READY 0
#define SEM_NEED_NEXT_SECTION 1
#define SEM_PIRATE_GROUP 2

typedef struct {
    bool sections[ISLAND_SIZE];
    int next_section_to_check;
    int found_treasure_index;
    pid_t pirate_groups[MAX_NUMBER_OF_PIRATE_GROPUS];
    int active_groups;
} island;

int pirate_group_id;

island* shm_ptr;
int shm_id;

int sem_id;

void sigfunc(int sig) {
    if (sig != SIGINT && sig != SIGTERM) {
        return;
    }

    printf("Pirate group %d is stopping it's work\n", pirate_group_id);

    shm_ptr->active_groups--;

    shmdt(shm_ptr);

    exit(0);
}

void sem_wait(int sem_id, int sem_num)
{
    struct sembuf semops;
    semops.sem_num = sem_num;
    semops.sem_op = -1;
    semops.sem_flg = 0;
    semop(sem_id, &semops, 1);
}

void sem_post(int sem_id, int sem_num)
{
    struct sembuf semops;
    semops.sem_num = sem_num;
    semops.sem_op = 1;
    semops.sem_flg = 0;
    semop(sem_id, &semops, 1);
}

int main(int argc, char **argv) {
    signal(SIGINT, sigfunc);
    signal(SIGTERM, sigfunc);

    // Инициализация генератора псевдослучайных чисел
    srand(time(NULL));

    // Инициализация разделяемой памяти
    shm_id = shmget(SHM_KEY, sizeof(island), 0);
    shm_ptr = shmat(shm_id, NULL, 0);

    // Инициализация семафоров
    sem_id = semget(SEM_KEY, 3, 0);

    // Код группы пиратов
    sem_wait(sem_id, SEM_PIRATE_GROUP);

    pirate_group_id = shm_ptr->active_groups;
    shm_ptr->active_groups++;

    sem_post(sem_id, SEM_PIRATE_GROUP);

    shm_ptr->pirate_groups[pirate_group_id] = getpid();
            
    while (true) {
        sem_wait(sem_id, SEM_PIRATE_GROUP);
        sem_wait(sem_id, SEM_NEXT_SECTION_READY);

        int index = shm_ptr->next_section_to_check;

        sem_post(sem_id, SEM_NEED_NEXT_SECTION);
        sem_post(sem_id, SEM_PIRATE_GROUP);

        printf("Pirate group %d is searching section %d\n", pirate_group_id, index);

        sleep(rand() % 5 + 1);

        if (shm_ptr->sections[index]) {
            shm_ptr->found_treasure_index = index;
            printf("Pirate group %d found treasure in section %d!\n", pirate_group_id, index);
        }
    }

    return 0;
}

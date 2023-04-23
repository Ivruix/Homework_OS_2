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
#define NUMBER_OF_PIRATE_GROPUS 5

#define SHM_KEY 0x1234

#define SEM_KEY 0x1337
#define SEM_NEXT_SECTION_READY 0
#define SEM_NEED_NEXT_SECTION 1
#define SEM_PIRATE_GROUP 2

typedef struct {
    bool sections[ISLAND_SIZE];
    int next_section_to_check;
    int found_treasure_index;
    pid_t pirate_groups[NUMBER_OF_PIRATE_GROPUS];
    int active_groups;
} island;

int pirate_group_id;

island* shm_ptr;
int shm_id;

int sem_id;

void pirate_sigfunc(int sig) {
    if (sig != SIGINT && sig != SIGTERM) {
        return;
    }

    printf("Pirate group %d is stopping it's work\n", pirate_group_id);

    shm_ptr->active_groups--;

    shmdt(shm_ptr);

    exit(0);
}

void sigfunc(int sig) {
    if (sig != SIGINT && sig != SIGTERM) {
        return;
    }

    for (int i = 0; i < NUMBER_OF_PIRATE_GROPUS; i++) {
        kill(shm_ptr->pirate_groups[i], SIGTERM);
    }

    semctl(sem_id, SEM_NEXT_SECTION_READY, IPC_RMID, 0);
    semctl(sem_id, SEM_NEED_NEXT_SECTION, IPC_RMID, 0);
    semctl(sem_id, SEM_PIRATE_GROUP, IPC_RMID, 0);
    shmctl(shm_id, IPC_RMID, NULL);

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
    shm_id = shmget(SHM_KEY, sizeof(island), IPC_CREAT | 0666);
    shm_ptr = shmat(shm_id, NULL, 0);
    
    int treasured_index = rand() % ISLAND_SIZE;
    for (int i = 0; i < ISLAND_SIZE; i++) {
        shm_ptr->sections[i] = false;
    }
    shm_ptr->sections[treasured_index] = true;
    shm_ptr->found_treasure_index = -1;
    shm_ptr->active_groups = NUMBER_OF_PIRATE_GROPUS;

    // Инициализация семафоров
    sem_id = semget(SEM_KEY, 3, IPC_CREAT | 0666);
    semctl(sem_id, SEM_NEXT_SECTION_READY, SETVAL, 0);
    semctl(sem_id, SEM_NEED_NEXT_SECTION, SETVAL, 1);
    semctl(sem_id, SEM_PIRATE_GROUP, SETVAL, 1);

    // Запуск групп пиратов
    for (int i = 0; i < NUMBER_OF_PIRATE_GROPUS; i++) {
        // Код групп пиратов
        if (fork() == 0) {
            signal(SIGINT, pirate_sigfunc);
            signal(SIGTERM, pirate_sigfunc);

            shm_ptr->pirate_groups[i] = getpid();

            pirate_group_id = i;
            
            while (true) {
                sem_wait(sem_id, SEM_PIRATE_GROUP);
                sem_wait(sem_id, SEM_NEXT_SECTION_READY);

                int index = shm_ptr->next_section_to_check;

                sem_post(sem_id, SEM_NEED_NEXT_SECTION);
                sem_post(sem_id, SEM_PIRATE_GROUP);

                printf("Pirate group %d is searching section %d\n", i, index);

                sleep(rand() % 5 + 1);

                if (shm_ptr->sections[index]) {
                    shm_ptr->found_treasure_index = index;
                    printf("Pirate group %d found treasure in section %d!\n", i, index);
                }
            }

            return 0;
        }
    }
    
    // Код Джона Сильвера
    int next_section_to_check = 0;
    while (next_section_to_check != ISLAND_SIZE) {
        if (shm_ptr->found_treasure_index != -1) {
            printf("John Silver was notified that the treasure has been found in section %d\n", shm_ptr->found_treasure_index);
            break;
        }

        sem_wait(sem_id, SEM_NEED_NEXT_SECTION);
        
        shm_ptr->next_section_to_check = next_section_to_check;
        next_section_to_check++;

        printf("John Silver assigns section %d to search\n", shm_ptr->next_section_to_check);

        sem_post(sem_id, SEM_NEXT_SECTION_READY);
    }

    if (shm_ptr->found_treasure_index == -1) {
        while (shm_ptr->found_treasure_index == -1) {
            sleep(1);
        }
        printf("John Silver was notified that the treasure has been found in section %d\n", shm_ptr->found_treasure_index);
    }
    
    // Заврешение программы
    for (int i = 0; i < NUMBER_OF_PIRATE_GROPUS; i++) {
        kill(shm_ptr->pirate_groups[i], SIGTERM);
    }

    while (shm_ptr->active_groups != 0) {
        sleep(1);
    }

    semctl(sem_id, SEM_NEXT_SECTION_READY, IPC_RMID, 0);
    semctl(sem_id, SEM_NEED_NEXT_SECTION, IPC_RMID, 0);
    semctl(sem_id, SEM_PIRATE_GROUP, IPC_RMID, 0);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}

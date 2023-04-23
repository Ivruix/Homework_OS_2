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
#define NUMBER_OF_PIRATE_GROPUS 5

const char* shm_name = "/shared-memory";

const char* next_section_ready_sem_name = "/next-section-ready-semaphore";
const char* need_next_section_sem_name = "/need-next-section-semaphore";
const char* pirate_group_sem_name = "/pirate-group-semaphore";

typedef struct {
    bool sections[ISLAND_SIZE];
    int next_section_to_check;
    int found_treasure_index;
    pid_t pirate_groups[NUMBER_OF_PIRATE_GROPUS];
    int active_groups;
} island;

int pirate_group_id;

sem_t* next_section_ready_sem;
sem_t* need_next_section_sem;
sem_t* pirate_group_sem;

island* shm_ptr;

void pirate_sigfunc(int sig) {
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

void sigfunc(int sig) {
    if (sig != SIGINT && sig != SIGTERM) {
        return;
    }

    for (int i = 0; i < NUMBER_OF_PIRATE_GROPUS; i++) {
        kill(shm_ptr->pirate_groups[i], SIGTERM);
    }

    sem_unlink(next_section_ready_sem_name);
    sem_unlink(need_next_section_sem_name);
    sem_unlink(pirate_group_sem_name);
    shm_unlink(shm_name);

    exit(0);
}

int main(int argc, char **argv) {
    signal(SIGINT, sigfunc);
    signal(SIGTERM, sigfunc);

    // Инициализация генератора псевдослучайных чисел
    srand(time(NULL));

    // Инициализация семафоров
    next_section_ready_sem = sem_open(next_section_ready_sem_name, O_CREAT, 0666, 0);
    need_next_section_sem = sem_open(need_next_section_sem_name, O_CREAT, 0666, 1);
    pirate_group_sem = sem_open(pirate_group_sem_name, O_CREAT, 0666, 1);

    // Инициализация разделяемой памяти
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(island));
    shm_ptr = mmap(NULL, sizeof(island), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    
    int treasured_index = rand() % ISLAND_SIZE;
    for (int i = 0; i < ISLAND_SIZE; i++) {
        shm_ptr->sections[i] = false;
    }
    shm_ptr->sections[treasured_index] = true;
    shm_ptr->found_treasure_index = -1;
    shm_ptr->active_groups = NUMBER_OF_PIRATE_GROPUS;

    // Запуск групп пиратов
    for (int i = 0; i < NUMBER_OF_PIRATE_GROPUS; i++) {
        // Код групп пиратов
        if (fork() == 0) {
            signal(SIGINT, pirate_sigfunc);
            signal(SIGTERM, pirate_sigfunc);

            shm_ptr->pirate_groups[i] = getpid();

            pirate_group_id = i;

            while (true) {
                sem_wait(pirate_group_sem);
                sem_wait(next_section_ready_sem);

                int index = shm_ptr->next_section_to_check;

                sem_post(need_next_section_sem);
                sem_post(pirate_group_sem);

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

        sem_wait(need_next_section_sem);
        
        shm_ptr->next_section_to_check = next_section_to_check;
        next_section_to_check++;

        printf("John Silver assigns section %d to search\n", shm_ptr->next_section_to_check);

        sem_post(next_section_ready_sem);
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

    sem_unlink(next_section_ready_sem_name);
    sem_unlink(need_next_section_sem_name);
    sem_unlink(pirate_group_sem_name);
    shm_unlink(shm_name);
    return 0;
}

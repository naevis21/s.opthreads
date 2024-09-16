#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/shm.h>

struct dado {
    unsigned long long int resultado;
    int inicio;
    int fim;
};

void *calcula_fatorial(void *arg) {
    struct dado *dados = (struct dado *)arg;
    unsigned long long int resultado = 1;
    for (int i = dados->inicio; i <= dados->fim; i++) {
        resultado *= i;
    }
    dados->resultado = resultado;
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("usage: %s <N>\n", argv[0]); // Imprime a parada no certo pelo certo
        return 1; //indica que foi noggers (deu erro)
    }
    // Converte o argumento passado para inteiro
    int N = atoi(argv[1]);
    pthread_t threads[2];
    int shm_id = shmget(IPC_PRIVATE, 2 * sizeof(struct dado), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget");
        exit(1);
    }

    struct dado *shm_ptr = (struct dado *)shmat(shm_id, NULL, 0);
    if (shm_ptr == (void *)-1) {
        perror("shmat");
        exit(1);
    }

    int mid = N / 2;
    shm_ptr[0].inicio = 1;
    shm_ptr[0].fim = mid;
    shm_ptr[1].inicio = mid + 1;
    shm_ptr[1].fim = N;

    for (int i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, calcula_fatorial, (void *)&shm_ptr[i]);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    unsigned long long int resultado_final = shm_ptr[0].resultado * shm_ptr[1].resultado;
    printf("%llu\n", resultado_final);

    shmdt(shm_ptr);
    shmctl(shm_id, IPC_RMID, NULL);
    return 0;
}
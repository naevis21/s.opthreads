#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/shm.h>

// Definição da estrutura 'dado' que contém o resultado do fatorial e os limites do intervalo
struct dado {
    unsigned long long int resultado;
    int inicio;
    int fim;
};

int main(int argc, char **argv) {
      // Verifica se o número de argumentos passados é diferente de 2
    if (argc != 2) {
        printf("usage: %s <N>\n", argv[0]); // Imprime a parada no certo pelo certo
        return 1; //indica que foi noggers (deu erro)
    }

    // Converte o argumento passado para inteiro
    int N = atoi(argv[1]);
    pid_t pid1, pid2; // Variáveis para armazenar os IDs dos processos filhos
    int status1, status2; // Variáveis para armazenar os status dos processos filhos

    // Cria um segmento de memória compartilhada para armazenar dois objetos da estrutura 'dado'
    int shm_id = shmget(IPC_PRIVATE, 2 * sizeof(struct dado), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget"); // Imprime mensagem de erro se a criação falhar
        exit(1); // Sai do programa com código de erro 1
    }
    char shm_id_str[20];
    sprintf(shm_id_str, "%d", shm_id); // Converte o ID da memória compartilhada para string

    // Anexa a memória compartilhada ao espaço de endereçamento do processo
    struct dado *shm_ptr = (struct dado *)shmat(shm_id, NULL, 0);
    if (shm_ptr == (void *)-1) {
        perror("shmat"); // Imprime mensagem de erro se a anexação falhar
        exit(1); // Sai do programa com código de erro 1
    }

    int mid = N / 2; // Calcula o ponto médio para dividir o trabalho entre dois processos

    // Define os intervalos de cálculo para os dois processos
    shm_ptr[0].inicio = 1;
    shm_ptr[0].fim = mid;
    shm_ptr[1].inicio = mid + 1;
    shm_ptr[1].fim = N;

    // Cria o primeiro processo filho
    if ((pid1 = fork()) == 0) {
        execl("./fatorial_auxiliar", "fatorial_auxiliar_1", shm_id_str, "0", (char *)NULL);
        perror("execl"); // Imprime mensagem de erro se a execução do programa auxiliar falhar
        exit(1); // Sai do processo filho com código de erro 1
    }

    // Cria o segundo processo filho
    if ((pid2 = fork()) == 0) {
        execl("./fatorial_auxiliar", "fatorial_auxiliar_2", shm_id_str, "1", (char *)NULL);
        perror("execl"); // Imprime mensagem de erro se a execução do programa auxiliar falhar
        exit(1); // Sai do processo filho com código de erro 1
    }

    // Espera os processos filhos terminarem
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);

    // Calcula o resultado final multiplicando os resultados parciais
    unsigned long long int resultado_final = shm_ptr[0].resultado * shm_ptr[1].resultado;

    // Imprime o resultado final
    printf("%lld\n", resultado_final);

    // Desanexa a memória compartilhada do espaço de endereçamento do processo
    shmdt(shm_ptr);
    // Marca o segmento de memória compartilhada para destruição
    shmctl(shm_id, IPC_RMID, NULL);

    return 0; // poggers!!! (sucesso)
}

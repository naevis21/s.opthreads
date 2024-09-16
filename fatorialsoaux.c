#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>

// Definição da estrutura 'dado' que contém o resultado do fatorial e os limites do intervalo
struct dado {
    unsigned long long int resultado;
    int inicio;
    int fim;
};

// Função que calcula o fatorial de 'x' até 'n'
long long int fatorial(int x, int n) {
    unsigned long long int resultado = 1;
    for (int i = x; i <= n; i++) {
        resultado *= i;
    }
    return resultado;
}

int main(int argc, char *argv[]) {
    // Verifica se o número de argumentos passados é diferente de 3
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <shm_id> <index>\n", argv[0]); // Imprime a forma correta de uso
        exit(1); // Sai do programa com código de erro 1
    }

    // Converte os argumentos passados para inteiros
    int shm_id = atoi(argv[1]);
    int index = atoi(argv[2]);

    // Anexa a memória compartilhada ao espaço de endereçamento do processo
    struct dado *shm_ptr = (struct dado *)shmat(shm_id, NULL, 0);
    if (shm_ptr == (void *)-1) {
        perror("shmat"); // Imprime mensagem de erro se a anexação falhar
        exit(1); // Sai do programa com código de erro 1
    }

    // Calcula o fatorial do intervalo especificado e armazena o resultado na memória compartilhada
    shm_ptr[index].resultado = fatorial(shm_ptr[index].inicio, shm_ptr[index].fim);

    // Desanexa a memória compartilhada do espaço de endereçamento do processo
    shmdt(shm_ptr);

    return 0; // Indica que foi poggers (sucesso)
}

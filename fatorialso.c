#include <stdio.h>
#include <stdlib.h>

// Função que calcula o fatorial de um número entre 'inicio' e 'n', dale
unsigned long long int fatorial(int inicio, int n) {
  unsigned long long int resultado = 1;

  for (int i = inicio; i <= n; i++) {
    resultado *= i;
  }

  return resultado;
}

int main(int argc, char **argv) {
  // Verifica se o número de argumentos passados é diferente de 2
  if (argc != 2) {
    printf("usage: %s <N>\n", argv[0]); // Imprime a parada no certo pelo certo
    return 1; //indica que foi noggers (deu erro)
  }

  // Converte o argumento passado para inteiro
  int numero = atoi(argv[1]);

  // Chama a função fatorial e imprime o resultado
  printf("%llu\n", fatorial(1, numero));

  return 0; // indica qud foi poggers (sucesso!!)
}

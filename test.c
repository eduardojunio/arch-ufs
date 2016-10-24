#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char nome[31];
    char sobrenome[31];
    char email[3][61];
    char numero[3][20];
} Contato;

int main() {

  Contato *contatos;
  contatos = malloc(sizeof(Contato));


  strcpy(contatos[0].nome, "Eduardo");
  realloc(contatos, sizeof(*contatos) + sizeof(Contato));
  strcpy(contatos[1].nome, "Macedo");
  printf("%s\n", contatos[0].nome);
  printf("%s\n", contatos[1].nome);

  /*
  // int a[5];
  int *a;
  a = malloc(sizeof(int) * 5);
  a[0] = 1;
  a[1] = 2;
  a[2] = 3;
  a[3] = 4;
  a[4] = 5;
  realloc(a, sizeof(*a) + sizeof(int));
  // a+5 = malloc(sizeof(int));
  int i;
  for (i = 0; i < 6; i++) {
    printf("%d\n", a[i]);
  }*/
}
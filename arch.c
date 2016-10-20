#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MENUS_QUANTIDADE 1

/**
 * Models
 */

typedef struct {
    char nome[31];
    char sobrenome[31];
    char email[3][61];
    char numero[3][20];
} Contato;

/**
 * Database connection
 */

FILE *db;

void dbopen() {
    if ((db = fopen("db.txt", "a")) == NULL) {
        printf("\n\nErro ao abrir o banco de dados (db.txt).\n\n");
        exit(1);
    }
}

/**
 * Forward declarations
 */

void erroGUI();

/**
 * Business Logic
 */

void pressContinuar() {
    printf("PRESSIONE [ENTER] PARA CONTINUAR...");
    getchar();
}

int inserirContato(Contato c) {
    // validar nome
    if (strcmp(c.nome, "") == 0) {
        erroGUI("O nome nao pode ser vazio!");
        return 0;
    }

    // validar emails
    int i;
    for (i = 0; i < 3; i++) {
        if (strcmp(c.email[i], "") != 0) {
            if (strstr(c.email[i], "@") == NULL) {
                erroGUI("E-mail invalido!");
                return 0;
            }
        }
    }

    int inserir = fprintf(db, "n: %s s: %s e1: %s e2: %s e3: %s n1: %s n2: %s n3: %s\n",
        c.nome,
        c.sobrenome,
        c.email[0],
        c.email[1],
        c.email[2],
        c.numero[0],
        c.numero[1],
        c.numero[2]);
    if (inserir < 0) {
        erroGUI("Falha ao inserir o contato!");
        return 0;
    }
    return 1;
}

/*int verificarMatricula(int m) {
    int i;
    for (i = 0; i < ESTUDANTES; i++)
        if (m == estudantes[i].matricula)
            return i;
    return -1;
}

int calcularMaiorMedia() {
    float media = 0.0f;
    int i, iEstudante;
    for (i = 0; i < ESTUDANTES; i++) {
        if (estudantes[i].media > media) {
            media = estudantes[i].media;
            iEstudante = i;
        }
    }
    return estudantes[iEstudante].matricula;
}

int calcularMenorMedia() {
    int iEstudante = verificarMatricula(calcularMaiorMedia());
    if (iEstudante == -1) {
        return 0;
    } 
    float media = estudantes[iEstudante].media;
    int i, j;
    for (i = 0; i < ESTUDANTES; i++) {
        if (estudantes[i].media > 0 && estudantes[i].media < media) {
            media = estudantes[i].media;
            j = i;
        }
    }
    return estudantes[j].matricula;
}

int indexLivre() {
    int i;
    for (i = 0; i < ESTUDANTES; i++) {
        if (estudantes[i].matricula == 0)
            break;
    }
    if ((i + 1) == ESTUDANTES)
        return -1;
    return i;
}

int adicionarEstudante(estudante e) {
    int iEstudante = indexLivre();
    if (iEstudante != -1) {
        estudantes[iEstudante] = e;
        return 1;
    }
    return 0;
}

int removerEstudante(int m) {
    int iEstudante = verificarMatricula(m);
    if (iEstudante != -1) {
        estudantes[iEstudante].matricula = 0;
        return 1;
    }
    return 0;
}

int buscarEstudante(int m) {
    int i;
    for (i = 0; i < ESTUDANTES; i++) {
        if (estudantes[i].matricula == m) {
            exibirEstudanteGUI(estudantes[i].matricula);
            return 1;
        }
    }
    return 0;
}

int buscarEstudanteNome(char nome[]) {
    int i, j = 0;
    for (i = 0; i < ESTUDANTES; i++) {
        if (strstr(estudantes[i].nome, nome)) {
            exibirEstudanteGUI(estudantes[i].matricula);
            j++;
        }
    }
    if (j)
        return 1;
    return 0;
}

void removerNewLineStr(char s[]) {
    int i;
    for (i = 0; i < 256; i++)
        if (s[i] == '\n')
            break;
    s[i] = '\0';
}*/

/**
 * Views
 */

void erroGUI(char msg[]) {
    printf("---------------------------------------------\n");
    if (strcmp(msg, "") != 0) {
        printf("ERRO: %s\n", msg);
    } else {
        printf("ERRO.\n");
    }
    printf("---------------------------------------------\n");
}

void inserirContatoGUI() {
    Contato c;
    system("clear");
    printf("=============================================\n");
    printf("INSERIR CONTATO:\n");
    printf("NOME (30 caracteres max): ");
    gets(c.nome);
    printf("SOBRENOME (30 caracteres max): ");
    gets(c.sobrenome);
    printf("E-MAILS:\nE-MAIL 1: ");
    gets(c.email[0]);
    printf("E-MAIL 2: ");
    gets(c.email[1]);
    printf("E-MAIL 3: ");
    gets(c.email[2]);
    printf("TELEFONES:\nTELEFONE 1: ");
    gets(c.numero[0]);
    printf("TELEFONE 2: ");
    gets(c.numero[1]);
    printf("TELEFONE 3: ");
    gets(c.numero[2]);
    printf("=============================================\n");
    inserirContato(c);
    pressContinuar();
}

/*void exibirEstudantesPorMediaDescGUI() {
    // todo: extract bussisness logic to function
    estudante eDesc[ESTUDANTES];
    estudante e;
    memcpy(&eDesc, &estudantes, sizeof(estudantes));
    int i, j;
    for (i = 0; i < ESTUDANTES; i++) {
        for (j = i + 1; j < ESTUDANTES; j++) {
            if (eDesc[i].media < eDesc[j].media) {
                e = eDesc[i];
                eDesc[i] = eDesc[j];
                eDesc[j] = e;
            }
        }
    }
    printf("=============================================\n");
    printf("ESTUDANTES ORDENADOS POR MEDIA\n");
    printf("=============================================\n");
    for (i = 0; i < ESTUDANTES; i++)
        if (eDesc[i].matricula != 0)
            exibirEstudanteGUI(eDesc[i].matricula);
    printf("=============================================\n");
    pressContinuar();
}

void exibirEstudante404GUI() {
    printf("---------------------------------------------\n");
    printf("\tESTUDANTE NAO ENCONTRADO.\n");
    printf("---------------------------------------------\n");
}

void exibirAtualizarEstudanteGUI() {
    printf("=============================================\n");
    printf("ATUALIZANDO ESTUDANTE:\n");
    printf("NUMERO DA MATRICULA: ");
    int m;
    scanf("%d", &m);
    getchar();
    int iEstudante = verificarMatricula(m);
    if (iEstudante != -1) {
        printf("---------------------------------------------\n");
        printf("Pressione apenas [ENTER] para não alterar um campo\n");
        printf("---------------------------------------------\n");
        printf("NOME (255 caracteres max): ");
        char tempN[256];
        if (*(fgets(tempN, 256, stdin)+0) != '\n')
            strcpy(estudantes[iEstudante].nome, tempN);
        printf("SEXO (M/F): ");
        char tempS = fgetc(stdin);
        if (tempS != '\n')
            estudantes[iEstudante].sexo = tempS;
        printf("---------------------------------------------\n");
        printf("Informe o valor -1 para não alterar um campo\n");
        printf("---------------------------------------------\n");
        printf("NOTAS DAS PROVAS:\nPROVA 1: ");
        float n;
        scanf("%f", &n);
        if (n != -1.0f)
            estudantes[iEstudante].notasProva[0] = n;
        printf("PROVA 2: ");
        scanf("%f", &n);
        if (n != -1.0f)
            estudantes[iEstudante].notasProva[1] = n;
        printf("PROVA 3: ");
        scanf("%f", &n);
        if (n != -1.0f)
            estudantes[iEstudante].notasProva[2] = n;
        // atualizar media
        estudantes[iEstudante].media = (estudantes[iEstudante].notasProva[0] + estudantes[iEstudante].notasProva[1] +
            estudantes[iEstudante].notasProva[2]) / 3;
        printf("NOTA DO PROJETO: ");
        scanf("%f", &n);
        if (n != -1.0f)
            estudantes[iEstudante].notaProjeto = n;
    } else {
        exibirErroGUI();
    }
    printf("=============================================\n");
}

void exibirApagarEstudanteGUI() {
    printf("=============================================\n");
    printf("\tAPAGAR ESTUDANTE\n");
    printf("MATRICULA DO ESTUDANTE A SER APAGADO: ");
    int m;
    scanf("%d", &m);
    printf("=============================================\n");
    if (!removerEstudante(m)) {
        exibirErroGUI();
    }
    pressContinuar();
}

void exibirEncontrarEstudanteGUI() {
    printf("=============================================\n");
    printf("\tENCONTRAR ESTUDANTE\n");
    printf("MATRICULA DO ESTUDANTE: ");
    int m;
    scanf("%d", &m);
    if (!buscarEstudante(m)) {
        exibirErroGUI();
    }
    pressContinuar();
    printf("=============================================\n");
}

void exibirEncontrarEstudanteNomeGUI() {
    printf("=============================================\n");
    printf("\tENCONTRAR ESTUDANTE\n");
    printf("NOME DO ESTUDANTE: ");
    char nome[256];
    fgets(nome, 256, stdin);
    removerNewLineStr(nome);
    if (!buscarEstudanteNome(nome)) {
        exibirErroGUI();
    }
    pressContinuar();
    printf("=============================================\n");
}

void exibirEstudanteGUI(int m) {
    int iEstudante = verificarMatricula(m);
    if (iEstudante != -1) {
        printf("---------------------------------------------\n");
        printf("NOME: %s", estudantes[iEstudante].nome);
        printf("MATRICULA: %d\tSEXO: %c\n",
            estudantes[iEstudante].matricula, estudantes[iEstudante].sexo);
        float *notasProva = estudantes[iEstudante].notasProva;
        printf("NOTAS DAS PROVAS:\nPROVA 1: %.1f\tPROVA 2: %.1f\tPROVA 3: %.1f\n", 
            *(notasProva+0), *(notasProva+1), *(notasProva+2));
        printf("NOTA DO PROJETO: %.1f\n", estudantes[iEstudante].notaProjeto);
        printf("---------------------------------------------\n");
    } else {
        exibirEstudante404GUI();
    }
}

void exibirMaiorMediaGUI() {
    int m = calcularMaiorMedia();
    if (m) {
        printf("=============================================\n");
        printf("O ESTUDANTE COM MAIOR MEDIA E:\n");
        exibirEstudanteGUI(m);
        printf("=============================================\n");
    } else {
        exibirEstudante404GUI();
    }
    pressContinuar();
}

void exibirMenorMediaGUI() {
    int m = calcularMenorMedia();
    if (m) {
        printf("=============================================\n");
        printf("O ESTUDANTE COM MENOR MEDIA E:\n");
        exibirEstudanteGUI(m);
        printf("=============================================\n");
    } else {
        exibirEstudante404GUI();
    }
    pressContinuar();
}

void exibirMediaEstudanteGUI() {
    printf("=============================================\n");
    printf("\tMEDIA ESTUDANTE\n");
    printf("NUMERO DA MATRICULA: ");
    int m;
    scanf("%d", &m);
    int iEstudante = verificarMatricula(m);
    if (iEstudante != -1) {
        printf("A media do estudante %s e: %.1f\n",
            estudantes[iEstudante].nome, estudantes[iEstudante].media);
        printf("=============================================\n");
    } else {
        exibirEstudante404GUI();
    }
    pressContinuar();
}

void exibirListaEstudantesGUI() {
    printf("=============================================\n");
    printf("\t\tLISTA DE ESTUDANTES\n");
    printf("=============================================\n");
    int i;
    for (i = 0; i < ESTUDANTES; i++) {
        if (estudantes[i].matricula == 0)
            continue;
        exibirEstudanteGUI(estudantes[i].matricula);
    }
    pressContinuar();
}*/

void gui() {
    system("clear");
    printf("=============================================\n");
    printf("\t\tMENU\n");
    printf("=============================================\n");
    char menus[][200] = {
        "Inserir contato"
    };
    int i;
    for (i = 0; i < MENUS_QUANTIDADE; i++) {
        printf("%d. %s\n", i+1, menus[i]);
    }
    printf("0. Sair do programa e salvar alteracoes\n");
}

void menu() {
    int n;
    do {
        gui();
        printf("Ingresse sua opcao: ");
        scanf("%d", &n);
        getchar();
        switch(n) {
            case 1:
                inserirContatoGUI();
                break;
        }
    } while (n != 0);
}

/**
 * Program bootstrapping
 * @return Status code to OS
 */
int main()
{
    dbopen();
    menu();
    fclose(db);
    return 0;
}
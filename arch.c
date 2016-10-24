#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

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
 * Bootstrapping
 */

Contato **contatos;
int freeAtIndex = 0;

/**
 * Forward declarations
 */

void erroGUI();

/**
 * Business Logic
 */

FILE *openDb(const char *path, const char *mode) {
    FILE *db = NULL;
    if (strcmp(mode, "loadData") == 0) {
        db = fopen(path, "a+");
    } else if (strcmp(mode, "saveData") == 0) {
        db = fopen(path, "w");
    }
    if (db == NULL) {
        printf("Erro ao abrir o banco de dados (db.txt).\n");
        exit(1);
    }
    return db;
}

void freeIndex() {
    freeAtIndex++;
    contatos[freeAtIndex] = malloc(sizeof(Contato));
}

void emptyField(char *s) {
    if (strcmp(s, "") == 0) {
        strcpy(s, "VAZIO");
    } else if (strcmp(s, "VAZIO") == 0) {
        strcpy(s, "");
    }
}

void emptyFields() {
    int i = 0;
    while (i < freeAtIndex) {
        emptyField(contatos[i]->sobrenome);
        emptyField(contatos[i]->email[0]);
        emptyField(contatos[i]->email[1]);
        emptyField(contatos[i]->email[2]);
        emptyField(contatos[i]->numero[0]);
        emptyField(contatos[i]->numero[1]);
        emptyField(contatos[i]->numero[2]);
        i++;
    }
}

void saveDataDb() {
    FILE *db = openDb("db.txt", "saveData");
    emptyFields();
    int i = 0;
    while (i < freeAtIndex) {
        int inserir = fprintf(db, "n: %s s: %s e1: %s e2: %s e3: %s n1: %s n2: %s n3: %s\n",
            contatos[i]->nome,
            contatos[i]->sobrenome,
            contatos[i]->email[0],
            contatos[i]->email[1],
            contatos[i]->email[2],
            contatos[i]->numero[0],
            contatos[i]->numero[1],
            contatos[i]->numero[2]);
        if (inserir < 0) {
            erroGUI("Falha ao inserir o contato!");
            exit(1);
        }
        i++;
    }
    fclose(db);
}

void loadDataDb() {
    FILE *db = openDb("db.txt", "loadData");
    rewind(db);
    int i = 0;
    contatos[i] = malloc(sizeof(Contato));
    while (fscanf(db, "n: %s s: %s e1: %s e2: %s e3: %s n1: %s n2: %s n3: %s\n",
        contatos[i]->nome,
        contatos[i]->sobrenome,
        contatos[i]->email[0],
        contatos[i]->email[1],
        contatos[i]->email[2],
        contatos[i]->numero[0],
        contatos[i]->numero[1],
        contatos[i]->numero[2]) == 8) {
        i++;
        contatos[i] = malloc(sizeof(Contato));
        freeAtIndex++;
    }
    emptyFields();
    fclose(db);
}

int chrPos(const char *s, int c) {
    int i = 0;
    while (*(s+i) != '\0') {
        if (*(s+i) == c) {
            return i;
        }
        i++;
    }
    return -1;
}

int validMailUser(const char *s) {
    int i = 0;
    while (*(s+i) != '@') {
        if (*(s+i) == '.') {
            // ok
        } else if (*(s+i) == '_') {
            // ok
        } else if (!isalnum(*(s+i))) {
            return 0;
        }
        i++;
    }
    return 1;
}

int validDomain(const char *s) {
    int i = 0;
    while (*(s+i) != '\0') {
        if (*(s+i) == '.') {
            // ok
        } else if (!isalnum(*(s+i))) {
            return 0;
        }
        i++;
    }
    return 1;
}

int inserirContato(Contato c) {
    // validar nome
    if (strcmp(c.nome, "") == 0) {
        erroGUI("O nome nao pode ser vazio!");
        return 0;
    }

    // validar exclusividade
    char *nomeCompleto = malloc(sizeof(c.nome) + sizeof(c.sobrenome) + sizeof(char));
    strcpy(nomeCompleto, c.nome);
    strcat(nomeCompleto, " ");
    strcat(nomeCompleto, c.sobrenome);

    int i = 0;
    char *nomeCompletoAtIndex;
    while (i < freeAtIndex) {
        nomeCompletoAtIndex = malloc(sizeof(contatos[i]->nome) + sizeof(contatos[i]->sobrenome) + sizeof(char));
        strcpy(nomeCompletoAtIndex, contatos[i]->nome);
        strcat(nomeCompletoAtIndex, " ");
        strcat(nomeCompletoAtIndex, contatos[i]->sobrenome);
        if (strcmp(nomeCompletoAtIndex, nomeCompleto) == 0) {
            erroGUI("Um contato com esse nome ja existe!");
            return 0;
        }
        i++;
    }

    // validar emails
    for (i = 0; i < 3; i++) {
        if (strcmp(c.email[i], "") != 0) {
            char *at = strchr(c.email[i], '@');
            // int atIndex = chrPos(c.email[i], '@');
            if (at == NULL) {
                erroGUI("E-mail invalido! #ERRMAILC000");
                return 0;
            }
            if (*(at+1) == '\0') {
                erroGUI("E-mail invalido! #ERRMAILC001");
                return 0;
            }
            if (c.email[i][0] == '@') {
                erroGUI("E-mail invalido! #ERRMAILC002");
                return 0;
            }
            if (!validDomain(at+1)) {
                erroGUI("E-mail invalido! #ERRMAILC003");
                return 0;
            }
            if (!validMailUser(c.email[i])) {
                erroGUI("E-mail invalido! #ERRMAILC004");
                return 0;
            }
        }
    }

    *contatos[freeAtIndex] = c;
    freeIndex();

    if (strcmp(contatos[freeAtIndex-1]->nome, c.nome) != 0) {
        erroGUI("Falha ao inserir o contato!");
        return 0;
    }
    return 1;
}

int *buscarContatoNome(const char *s) {
    static int *indexes;
    indexes = malloc(sizeof(int));
    int i, j = 0, k = 0;
    for (i = 0; i < freeAtIndex; i++) {
        if (strstr(contatos[i]->nome, s)) {
            indexes[j] = i;
            j++;
            realloc(indexes, sizeof(*indexes) + sizeof(int));
            k++;
        }
    }
    if (k == 0) {
        indexes[0] = -1;
    }
    return indexes;
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
}*/

/**
 * Views
 */

void pressContinuar() {
    printf("PRESSIONE [ENTER] PARA CONTINUAR...");
    getchar();
}

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

void contatoGUI(const int i) {
    printf("---------------------------------------------\n");
    printf("NOME: %s SOBRENOME: %s\n", contatos[i]->nome, contatos[i]->sobrenome);
    printf("E-MAILS:\nE-MAIL 1: %s\n", contatos[i]->email[0]);
    printf("E-MAIL 2: %s\n", contatos[i]->email[1]);
    printf("E-MAIL 3: %s\n", contatos[i]->email[2]);
    printf("TELEFONES:\nTELEFONE 1: %s\n", contatos[i]->numero[0]);
    printf("TELEFONE 2: %s\n", contatos[i]->numero[1]);
    printf("TELEFONE 3: %s\n", contatos[i]->numero[2]);
    printf("---------------------------------------------\n");
}

void contato404GUI() {
    printf("---------------------------------------------\n");
    printf("\tCONTATO NAO ENCONTRADO.\n");
    printf("---------------------------------------------\n");
}

void listarContatosGUI(int indexes[]) {
    printf("=============================================\n");
    printf("\t\tLISTA DE CONTATOS\n");
    printf("=============================================\n");
    int indexesSize = sizeof(*indexes) / sizeof(int);
    if (indexesSize > 0) {
        int i = 0;
        while (i < indexesSize) {
            contatoGUI(indexes[i]);
            i++;
        }
    } else {
        contato404GUI();
    }
}

void buscarContatoNomeGUI() {
    printf("=============================================\n");
    printf("\tENCONTRAR CONTATO PELO NOME\n");
    printf("NOME DO CONTATO: ");
    char nome[256];
    fgets(nome, 256, stdin);
    printf("=============================================\n");
    strtok(nome, "\n");
    int *indexes = buscarContatoNome(nome);
    if (indexes[0] == -1) {
        contato404GUI();
    } else {
        listarContatosGUI(indexes);
    }
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
    // para adicionar novos menus basta por mais uma string na array
    // e chamar a func responsavel pelo menu na func menu()
    char menus[][200] = {
        "Inserir contato",
        "Procurar por uma pessoa pelo nome",
        "Procurar por uma pessoa pelo sobrenome",
        "Procurar por uma pessoa pelo nome e sobrenome"
    };
    int i;
    int quantidadeMenus = (sizeof(menus) / sizeof(menus[0]));
    for (i = 0; i < quantidadeMenus; i++) {
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
            case 2:
                buscarContatoNomeGUI();
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
    contatos = malloc(sizeof(*contatos));
    loadDataDb();
    menu();
    saveDataDb();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Models
 */

typedef struct contato {
    char nome[31];
    char sobrenome[31];
    char email[3][61];
    char numero[3][20];
} Contato;

/**
 * Bootstrapping
 */

Contato *contatos;
size_t contatosTamanho = (size_t)0;
int contatos_t = 0;

/**
 * Forward declarations
 */

void erroGUI();

/**
 * Business Logic
 */

int compare_names(const void * a, const void * b) {
  Contato *contatoA = (Contato *)a;
  Contato *contatoB = (Contato *)b;
  return strcmp(contatoA->nome, contatoB->nome);
}

void expandirContatos() {
    contatosTamanho += sizeof(Contato);
    contatos_t = (int)(contatosTamanho / sizeof(Contato));
    contatos = realloc(contatos, contatosTamanho);
}

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

void emptyField(char *s) {
    if (strcmp(s, "") == 0) {
        strcpy(s, "VAZIO");
    } else if (strcmp(s, "VAZIO") == 0) {
        strcpy(s, "");
    }
}

void emptyFields() {
    int i = 0;
    while (i < contatos_t) {
        emptyField(contatos[i].sobrenome);
        emptyField(contatos[i].email[0]);
        emptyField(contatos[i].email[1]);
        emptyField(contatos[i].email[2]);
        emptyField(contatos[i].numero[0]);
        emptyField(contatos[i].numero[1]);
        emptyField(contatos[i].numero[2]);
        i++;
    }
}

void saveDataDb() {
    FILE *db = openDb("db.txt", "saveData");
    emptyFields();
    int i = 0;
    while (i < contatos_t) {
        int inserir = fprintf(db, "n: %s s: %s e1: %s e2: %s e3: %s n1: %s n2: %s n3: %s\n",
            contatos[i].nome,
            contatos[i].sobrenome,
            contatos[i].email[0],
            contatos[i].email[1],
            contatos[i].email[2],
            contatos[i].numero[0],
            contatos[i].numero[1],
            contatos[i].numero[2]);
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
    int i = 0;
    char tNome[31], tSobrenome[31], tEmail[3][61], tNumero[3][20];
    while (fscanf(db, "n: %s s: %s e1: %s e2: %s e3: %s n1: %s n2: %s n3: %s\n",
        tNome,
        tSobrenome,
        tEmail[0],
        tEmail[1],
        tEmail[2],
        tNumero[0],
        tNumero[0],
        tNumero[0]) == 8) {

        expandirContatos();
        strcpy(contatos[i].nome, tNome);
        strcpy(contatos[i].sobrenome, tSobrenome);
        strcpy(contatos[i].email[0], tEmail[0]);
        strcpy(contatos[i].email[1], tEmail[1]);
        strcpy(contatos[i].email[2], tEmail[2]);
        strcpy(contatos[i].numero[0], tNumero[0]);
        strcpy(contatos[i].numero[1], tNumero[0]);
        strcpy(contatos[i].numero[2], tNumero[0]);

        i++;
    }
    emptyFields();
    fclose(db);
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
    while (i < contatos_t) {
        nomeCompletoAtIndex = malloc(sizeof(contatos[i].nome) + sizeof(contatos[i].sobrenome) + sizeof(char));
        strcpy(nomeCompletoAtIndex, contatos[i].nome);
        strcat(nomeCompletoAtIndex, " ");
        strcat(nomeCompletoAtIndex, contatos[i].sobrenome);
        if (strcmp(nomeCompletoAtIndex, nomeCompleto) == 0) {
            erroGUI("Um contato com esse nome ja existe!");
            free(nomeCompletoAtIndex);
            free(nomeCompleto);
            return 0;
        }
        i++;
        free(nomeCompletoAtIndex);
    }

    free(nomeCompleto);

    // validar emails
    for (i = 0; i < 3; i++) {
        if (strcmp(c.email[i], "") != 0) {
            char *at = strchr(c.email[i], '@');
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

    expandirContatos();
    contatos[contatos_t-1] = c;

    if (strcmp(contatos[contatos_t-1].nome, c.nome) != 0) {
        erroGUI("Falha ao inserir o contato!");
        return 0;
    }
    return 1;
}

// TODO: Binary search
int *buscarContatoNome(const char *nome) {
    // Ordena contatos em ordem alfabética
    qsort(contatos, contatos_t, sizeof(Contato), compare_names);
    size_t indexesSize = (size_t)0;
    int *indexes = malloc(indexesSize);
    int i, j = 0;
    for (i = 0; i < contatos_t; i++) {
        if (strcmp(nome, contatos[i].nome) == 0) {
            indexesSize += sizeof(int);
            indexes = realloc(indexes, indexesSize);
            indexes[j] = i;
            j++;
        }
    }
    indexesSize += sizeof(int);
    indexes = realloc(indexes, indexesSize);
    indexes[j] = -1;
    return indexes;
}

/**
 * Views
 */

void pressContinuar() {
    printf("PRESSIONE [ENTER] PARA CONTINUAR...");
    getchar();
}

void erroGUI(const char *msg) {
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
    printf("NOME: %s SOBRENOME: %s\n", contatos[i].nome, contatos[i].sobrenome);
    printf("E-MAILS:\nE-MAIL 1: %s\n", contatos[i].email[0]);
    printf("E-MAIL 2: %s\n", contatos[i].email[1]);
    printf("E-MAIL 3: %s\n", contatos[i].email[2]);
    printf("TELEFONES:\nTELEFONE 1: %s\n", contatos[i].numero[0]);
    printf("TELEFONE 2: %s\n", contatos[i].numero[1]);
    printf("TELEFONE 3: %s\n", contatos[i].numero[2]);
    printf("---------------------------------------------\n");
    pressContinuar();
    system("clear");
}

void contato404GUI() {
    printf("---------------------------------------------\n");
    printf("\tCONTATO NAO ENCONTRADO.\n");
    printf("---------------------------------------------\n");
}

void listarContatosGUI(int *indexes) {
    system("clear");
    printf("=============================================\n");
    printf("\t\tLISTA DE CONTATOS\n");
    printf("=============================================\n");
    if (indexes[0] >= 0) {
        int i = 0;
        while (indexes[i] != -1) {
            contatoGUI(indexes[i]);
            i++;
        }
    } else {
        contato404GUI();
    }
}

void buscarContatoNomeGUI() {
    system("clear");
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
    free(indexes);
}

void gui() {
    system("clear");
    printf("=============================================\n");
    printf("\t\tMENU\n");
    printf("=============================================\n");
    // para adicionar novos menus basta por mais uma string na array
    // e chamar a func responsavel pelo menu na func menu()
    char menus[][200] = {
        "Inserir contato",
        "Buscar contato pelo nome"
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
    contatos = malloc(contatosTamanho);
    loadDataDb();
    menu();
    saveDataDb();
    free(contatos);
    return 0;
}

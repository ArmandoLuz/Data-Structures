#include <stdio.h>
#include "tree23.h"
#include <time.h>

int readText(Tree23 **tree);
void menu();
void search(Tree23 **tree);
void searchWithTrack(Tree23 **tree);
void insert(Tree23 **tree);
void delete(Tree23 **tree);
void timeSearch(Tree23 **tree);

int main() {
    Tree23 *tree = NULL;
    int option, status;

    do {
        menu();
        printf("\n: ");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            status = readText(&tree);
            if(status == 0) {
                printf("Lido com sucesso\n");
            }
            break;
        
        case 2:
            search(&tree);
            break;

        case 3:
            printTree(tree);
            break;

        case 4:
            insert(&tree);
            break;

        case 5:
            delete(&tree);
            break;

        case 6:
            printTreeWithLevel(tree, 0);
            break;

        case 7:
            searchWithTrack(&tree);
            break;

        case 8:
            timeSearch(&tree);
            break;
        
        case 9:
            break;

        default:
            printf("Escolha uma opção válida.");
        }
    } while (option != 9);

    return 0;
}

void delete(Tree23 **tree) {
    char word[100];
    int line;

    printf("Insira a palavra: ");
    scanf("%s", word);
    printf("Insira a linha: ");
    scanf("%d", &line);

    removeWordFromALine(tree, word, line);
}

void insert(Tree23 **tree) {
    char word[100];
    int line;
    Info *up = NULL;

    printf("\nInsira a palavra: ");
    scanf("%s", word);
    printf("Insira a linha: ");
    scanf("%d", &line);

    insertIn23Tree(tree, createInfo(word, line), NULL, &up);
}

void menu() {
    printf("\n1. Ler texto");
    printf("\n2. Buscar palavra");
    printf("\n3. Exibir todas as palavras");
    printf("\n4. Inserir uma palavra");
    printf("\n5. Excluir uma palavra de uma linha");
    printf("\n6. Exibir todos os niveis");
    printf("\n7. Procurar caminho de uma palavra");
    printf("\n8. Checar tempo de busca");
    printf("\n9. Sair");
}

void search(Tree23 **tree) {
    char word[100];
    Info **infoFound;

    printf("\nInsira a palavra que deseja procurar: ");
    scanf("%s", word);

    infoFound = searchInfo(tree, word);

    if((*infoFound) != NULL) {
        printWord((*infoFound));
    } else {
        printf("Palavra inexistente.");
    }
}

void searchWithTrack(Tree23 **tree) {
    char word[100];
    Info **infoFound;

    printf("\nInsira a palavra que deseja encontrar seu caminho: ");
    scanf("%s", word);

    infoFound = searchInfoWithTrack(tree, word, 0);

    if((*infoFound) == NULL)
        printf("Palavra inexistente.");
}

int readText(Tree23 **tree) {
    int count = 0;
    FILE *text;
    Info *up = NULL;
    char word[100], nameFile[100];
    int line = 0, status;

    printf("\nInsira o nome do arquivo: ");
    scanf("%s", nameFile);

    text = fopen(nameFile, "r");

    if (text == NULL) {
        printf("\nNao foi possivel abrir o texto.\n");
        status = 1;
    } else {
        while (fscanf(text, "%s", word) != EOF) {
            count++;
            if(word[0] == '\n')
            {
                line++;
            } else insertIn23Tree(tree, createInfo(word, line), NULL, &up);
        }

        status = 0;
    }

    fclose(text);

    return status;
}

void timeSearch(Tree23 **tree)
{
    clock_t timeInit, timeEnd;
    double total = 0.0;
    int repeat;
    char word[50];

    printf("\nInsira a palavra que deseja buscar -> ");
    scanf("%s", word);
    printf("\nInsira o número de vezes que deseja repetir a busca -> ");
    scanf("%d", &repeat);

    for(int i = 0; i < repeat; i++) {
        timeInit = clock();
        searchInfo(tree, word);
        timeEnd = clock();

        total += ((double)(timeEnd - timeInit) * 1000 / CLOCKS_PER_SEC);
    }

    printf("\nTempo médio: %.5f", (double)total / repeat);
}

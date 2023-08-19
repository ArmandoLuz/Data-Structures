#include <stdio.h>
#include <stdlib.h>
#include "tree45.h"
#include <time.h>

void menu();
void insert(Tree45 **root);
void sellProduct(Tree45 **tree);
void search(Tree45 *root);
void readData(Tree45 **root);
void writeData(Tree45 *tree);
void timeSearch(Tree45 *tree);
void searchWithTrack(Tree45 *tree);

int main() {
    Tree45 *tree = NULL;
    int option;

    // Quando o programa é iniciado, a árvore é construida automaticamente.
    readData(&tree);

    do
    {
        menu();
        printf("\n-> ");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            insert(&tree);
            break;

        case 2:
            search(tree);
            break;
        
        case 3:
            printTreeWithLevel(tree, 0);
            break;
        
        case 4:
            printTreeInOrder(tree);
            break;

        case 5:
            sellProduct(&tree);
            break;

        case 6:
            timeSearch(tree);
            break;

        case 7:
            searchWithTrack(tree);
            break;

        case 8:
            break;
        
        default:
            printf("\nInsira uma opcao valida!");
            break;
        }
    } while (option != 8);

    // Ao fechar o programa, as informações são guardadas junto com todas as alterações feitas.
    writeData(tree);

    return 0;
}

void menu()
{
    printf("\n1. Inserir calcado");
    printf("\n2. Buscar calcado");
    printf("\n3. Exibir niveis");
    printf("\n4. Exibir em ordem");
    printf("\n5. Venda");
    printf("\n6. Checar tempo de busca");
    printf("\n7. Procurar caminho de uma informacao");
    printf("\n8. Sair");
}

void insert(Tree45 **root)
{
    Tree45 *discard = NULL;
    Info *up, *addressFound;
    int id, quantity, size;
    float price;
    char type[50], mark[50];

    printf("\nInsira o tipo -> ");
    scanf("%s", type);
    setbuf(stdin, NULL);
    printf("\nInsira a marca -> ");
    scanf("%s", mark);
    setbuf(stdin, NULL);
    printf("\nInsira a quantidade -> ");
    scanf("%d", &quantity);
    setbuf(stdin, NULL);
    printf("\nInsira o tamanho -> ");
    scanf("%d", &size);
    setbuf(stdin, NULL);
    printf("\nInsira o preco -> ");
    scanf("%f", &price);
    setbuf(stdin, NULL);

    do
    {
        id = rand() % 100000;
        addressFound = searchInfo((*root), id);
    } while (addressFound != NULL);

    discard = insertIn45Tree(root, createInfo(id, quantity, mark, price, size, type), NULL, &up);
    discard = NULL;
}

void sellProduct(Tree45 **tree)
{
    int id, quantity; 

    printf("\nInsira o id do produto -> ");
    scanf("%d", &id);
    printf("\nInsira a quantidade -> ");
    scanf("%d", &quantity);
    sell(tree, id, quantity);
}

void search(Tree45 *root)
{
    int id;
    Info *infoFound;

    printf("\nInsira o id do calcado ->");
    scanf("%d", &id);

    infoFound = searchInfo(root, id);

    printInfo(infoFound);
}

void writeData(Tree45 *tree)
{
    FILE *archive;

    archive = fopen("calcados.txt", "w");
    
    saveData(tree, archive);

    fclose(archive);
}

void readData(Tree45 **root)
{
    FILE *archive;
    Info *up = NULL;
    Tree45 *discard = NULL;
    int code, quantity, size;
    float price;
    char type[50], mark[50];

    archive = fopen("calcados.txt", "r");

    if(archive != NULL)
    {
        while(fscanf(archive, "%d %d %d %f %s %s\n", &code, &quantity, &size, &price, type, mark) == 6)
        {
            discard = insertIn45Tree(root, createInfo(code, quantity, mark, price, size, type), NULL, &up);
        }

        discard = NULL;
    }

    fclose(archive);
}

void searchWithTrack(Tree45 *tree) {
    int value;
    Info *infoFound;

    printf("\nInsira o valor que deseja encontrar seu caminho: ");
    scanf("%d", &value);

    infoFound = searchInfoWithTrack(tree, value, 0);

    if(infoFound == NULL)
        printf("Palavra inexistente.");
}

void timeSearch(Tree45 *tree)
{
    clock_t timeInit, timeEnd;
    double total = 0.0;
    int repeat, id;

    printf("\nInsira o valor  que deseja buscar -> ");
    scanf("%d", &id);
    printf("\nInsira o número de vezes que deseja repetir a busca -> ");
    scanf("%d", &repeat);

    for(int i = 0; i < repeat; i++) {
        timeInit = clock();
        searchInfo(tree, id);
        timeEnd = clock();

        total += ((double)(timeEnd - timeInit) * 1000 / CLOCKS_PER_SEC);
    }

    printf("\nTempo médio: %.5f", (double)total / repeat);
}

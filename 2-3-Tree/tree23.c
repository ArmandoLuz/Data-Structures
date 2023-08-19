#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree23.h"

struct tree23
{
    int nInfos;
    Tree23 *left;
    Info *info1;
    Tree23 *middle;
    Info *info2;
    Tree23 *right;
};

struct chainedlist
{
    int line;
    ChainedList *next;
};

struct info
{
    char word[1000];
    ChainedList *listOfLines;
};

// Insere um nó na árvore.
Tree23 *insertIn23Tree(Tree23 **root, Info *info, Tree23 *fatherRoot, Info **up)
{
    Tree23 *biggestNode = NULL, *oldBiggestNode = NULL;

    if((*root) == NULL)
    {
        (*root) = createNode(info, NULL, NULL, NULL);
    } else {
        if(isLeaf((*root)))
        {
            if((*root)->nInfos == 1)
            {
                addNode(root, info, NULL);
            } else {
                // Ao encontrar uma informação igual, é inserido somente a linha em que esta se econtra.
                // Se a informação ainda não existe e há duas informações nó, então o nó deve ser quebrado.
                // Se o pai da raiz da atual for nulo, então é necessário criar uma nova raiz.
                if(insertIfEqual((*root), info) == 0){
                    biggestNode = nodeBreaker(root, info, up);
                    if(fatherRoot == NULL)
                    {
                        (*root) = createNode((*up), (*root), biggestNode, NULL);
                        biggestNode = NULL;
                    }
                }
            }
        } else if (insertIfEqual((*root), info) == 0) {
            if(strcmp(info->word, (*root)->info1->word) < 0)
                biggestNode = insertIn23Tree(&(*root)->left, info, (*root), up);
            else if(((*root)->nInfos == 1) || ((*root)->nInfos == 2 && strcmp(info->word, (*root)->info2->word) < 0))
                biggestNode = insertIn23Tree(&(*root)->middle, info, (*root), up);
            else biggestNode = insertIn23Tree(&(*root)->right, info, (*root), up);
        }
        // biggestNode->info1 != (*root)->info2 garante que este bloco seja executado
        // somente a partir da penultima chamada recursiva. Caso contrário, ele modificaria os nós
        // quebrados em operações anteriores.
        if(biggestNode != NULL && biggestNode->info1 != (*root)->info2)
        {
            if((*root)->nInfos == 1)
            {
                addNode(root, (*up), biggestNode);
                biggestNode = NULL;
            } else {
                // Nesse caso é necessário guardar o maior nó antes de chamar o quebra nó novamente.
                oldBiggestNode = biggestNode;
                biggestNode = nodeBreaker(root, (*up), up);
                // Após quebrar o nó, atribui-se o maior nó antigo a esquerda do maior nó atual,
                // se caso não houver inconsistencias entre os nós, neste caso, os nós não podem estar
                // invertidos.
                if(strcmp((*root)->middle->info1->word, oldBiggestNode->info1->word) > 0)
                {
                    biggestNode->left = (*root)->middle;
                    (*root)->middle = oldBiggestNode;
                } else if(biggestNode->left == NULL){
                    biggestNode->left = oldBiggestNode;
                } else if(biggestNode->left != NULL){
                    biggestNode->middle = oldBiggestNode;
                }

                if(fatherRoot == NULL)
                {
                    (*root) = createNode((*up), (*root), biggestNode, NULL);
                    biggestNode = NULL;
                }
            }
        }
    }

    return biggestNode;
}

// Cria um nó.
Tree23 *createNode(Info *info, Tree23 *childLeft, Tree23 *childMiddle, Tree23 *childRight)
{
    Tree23 *newNode = (Tree23 *) malloc(sizeof(Tree23));

    newNode->info1 = info;
    newNode->info2 = NULL;
    newNode->nInfos = 1;
    newNode->left = childLeft;
    newNode->middle = childMiddle;
    newNode->right = childRight;

    return newNode;
}

// Cria uma informação já inicializando a lista com um valor.
Info *createInfo(char *word, int line)
{
    Info *newInfo;
    newInfo = (Info *) malloc(sizeof(Info));

    strcpy(newInfo->word, word);
    newInfo->listOfLines = NULL;
    insertLine(&newInfo->listOfLines, line);

    return newInfo;
}

// Insere uma linha na lista de linhas de uma informação.
// Se a lista for vazia, ela é criada e uma linha é inserida.
void insertLine(ChainedList **listOfLines, int line)
{
    if((*listOfLines) == NULL)
    {
        (*listOfLines) = (ChainedList *) malloc(sizeof(ChainedList));
        (*listOfLines)->line = line;
        (*listOfLines)->next = NULL;
    } else insertLine(&(*listOfLines)->next, line);
}

// Adiciona uma informação em um nó já existente.
void addNode(Tree23 **root, Info *info, Tree23 *childRight)
{
    if(strcmp((*root)->info1->word, info->word) != 0){
        if(strcmp((*root)->info1->word, info->word) > 0)
        {
            (*root)->info2 = (*root)->info1;
            (*root)->info1 = info;
        } else (*root)->info2 = info;

        if(childRight != NULL && (*root)->middle != NULL && strcmp(childRight->info1->word, (*root)->middle->info1->word) < 0)
        {
            (*root)->right = (*root)->middle;
            (*root)->middle = childRight;
        } else (*root)->right = childRight;

        (*root)->nInfos = 2;
    // Se o valor da primeira informação for igual ao da segunda, a linha somente é
    // concatenada na lista da primeira informação.
    } else {
        insertLine(&(*root)->info1->listOfLines, info->listOfLines->line);
    }
}

// Faz a quebra de um nó.
Tree23 *nodeBreaker(Tree23 **root, Info *info, Info **up)
{
    Tree23 *newNode;

    if(strcmp(info->word, (*root)->info2->word) > 0)
    {
        (*up) = (*root)->info2;
        newNode = createNode(info, (*root)->right, NULL, NULL);
    } else if(strcmp(info->word, (*root)->info1->word) > 0)
    {
        (*up) = info;
        newNode = createNode((*root)->info2, NULL, (*root)->right, NULL);
    } else {
        (*up) = (*root)->info1;
        newNode = createNode((*root)->info2, NULL, (*root)->right, NULL);
        (*root)->info1 = info;
    }

    (*root)->nInfos = 1;

    return newNode;
}

// Função para associar uma linha a uma informação, caso ela já exista.
int insertIfEqual(Tree23 *root, Info *info)
{
    int isEqual = 0;
    if(root->nInfos == 2){
        if(strcmp(root->info1->word, info->word) == 0)
        {
            insertLine(&(root->info1->listOfLines), info->listOfLines->line);
            isEqual = 1;
        } else if(strcmp(root->info2->word, info->word) == 0)
        {
            insertLine(&(root->info2->listOfLines), info->listOfLines->line);
            isEqual = 1;
        }
    } else if(root->nInfos == 1) {
        if(strcmp(root->info1->word, info->word) == 0)
        {
            insertLine(&(root->info1->listOfLines), info->listOfLines->line);
            isEqual = 1;
        }
    }
        

    return isEqual;
}

void printLines(ChainedList *node) {
    if(node != NULL) {
        printf("%d ", node->line);
        printLines(node->next);
    }
}

void printWord(Info *node) {
    printf("\n%s: ", node->word);
    printLines(node->listOfLines);
}

void printTree(Tree23 *root) 
{
    if(root != NULL) {
        printTree(root->left);

        printf("%s: ", root->info1->word);
        printLines(root->info1->listOfLines);
        printf("\n");

        printTree(root->middle);

        if(root->nInfos == 2) {
            printf("%s: ", root->info2->word);
            printLines(root->info2->listOfLines);
            printf("\n");

            printTree(root->right);
        }
    }
}

void printTreeWithLevel(Tree23 *root, int level) 
{
    if(root != NULL) {

        for(int i = 0; i < level; i++)
            printf("\t");

        printf("Level %d: ", level);
        printf("info 1 -> '%s'   ", root->info1->word);

        if(root->nInfos == 2)
            printf("info 2 -> '%s' ", root->info2->word);
        printf("\n");

        printTreeWithLevel(root->left, level + 1);
        printTreeWithLevel(root->middle, level + 1);

        if(root->nInfos == 2)
            printTreeWithLevel(root->right, level + 1);
    }
}

// Verifica se o nó é folha.
int isLeaf(Tree23 *node)
{
    return node->left == NULL && node->middle == NULL && node->right == NULL ? 1 : 0;
}

// Remove uma linha da lista.
void removeLine(ChainedList **node, int line) {
    if((*node) != NULL){
        if((*node)->line == line) {
            ChainedList *aux = (*node);
            (*node) =  (*node)->next;
            free(aux);
        } else {
            removeLine(&(*node)->next, line);
        }
    }
}

// Procura uma info.
Info **searchInfo(Tree23 **root, char *word) {
    Info **addressInfoFound = NULL;

    if ((*root) != NULL){
        if(strcmp(word, (*root)->info1->word) < 0)
            addressInfoFound = searchInfo(&(*root)->left, word);
        else if(strcmp(word, (*root)->info1->word) == 0)
            addressInfoFound = &(*root)->info1;
        else if(((*root)->nInfos == 1) || ((*root)->nInfos == 2 && strcmp(word, (*root)->info2->word) < 0))
            addressInfoFound = searchInfo(&(*root)->middle, word);
        else if(strcmp(word, (*root)->info2->word) == 0)
            addressInfoFound = &(*root)->info2;
        else addressInfoFound = searchInfo(&(*root)->right, word);
    }

    return addressInfoFound;
}

// Faz o balanceamento dos nós.
void balance(Tree23 **root, Tree23 **father) {
    if ((*root) != NULL && (*root)->nInfos == 0) {
        if ((*father) != NULL) {
            if ((*root) == (*father)->left) {

                if ((*father)->middle->nInfos == 2) {
                    (*root)->info1 = (*father)->info1;
                    (*root)->nInfos = 1;

                    (*father)->info1 = (*father)->middle->info1;
                    (*father)->middle->info1 = (*father)->middle->info2;
                    (*father)->middle->nInfos = 1;

                    (*root)->left = (*root)->middle;
                    (*root)->middle = (*father)->middle->left;

                    (*father)->middle->left = (*father)->middle->middle;
                    (*father)->middle->middle = (*father)->middle->right;
                    (*father)->middle->right = NULL;

                } else {

                    if ((*father)->nInfos == 2) {
                        (*root)->info1 = (*father)->info1;
                        (*root)->info2 = (*father)->middle->info1;
                        (*root)->nInfos = 2;

                        (*root)->left = (*root)->middle;
                        (*root)->middle = (*father)->middle->left;
                        (*root)->right = (*father)->middle->middle;

                        (*father)->info1 = (*father)->info2;
                        (*father)->nInfos = 1;

                        free((*father)->middle);
                        (*father)->middle = (*father)->right;
                        (*father)->right = NULL;

                    } else {
                        (*root)->info1 = (*father)->info1;
                        (*root)->info2 = (*father)->middle->info1;

                        (*root)->left = (*root)->middle;
                        (*root)->middle = (*father)->middle->left;
                        (*root)->right = (*father)->middle->middle;

                        (*father)->nInfos = 0;
                        (*root)->nInfos = 2;

                        free((*father)->middle);
                        (*father)->middle = (*root);
                        (*father)->left = NULL;
                    }
                }

            } else if ((*root) == (*father)->middle) {

                if ((*father)->left->nInfos == 2) {
                    (*root)->info1 = (*father)->info1;
                    (*root)->nInfos = 1;

                    (*father)->info1 = (*father)->left->info2;
                    (*father)->left->nInfos = 1;
                    
                    (*root)->left = (*father)->left->right; 
                    (*father)->left->right = NULL; 

                } else {

                    if ((*father)->nInfos == 2) {
                        (*root)->info2 = (*father)->info1;
                        (*root)->info1 = (*father)->left->info1;
                        (*root)->nInfos = 2;

                        (*root)->right = (*root)->middle; 
                        (*root)->middle = (*father)->left->middle; 
                        (*root)->left = (*father)->left->left;

                        free((*father)->left);
                        (*father)->left = (*root);

                        (*father)->info1 = (*father)->info2;
                        (*father)->middle = (*father)->right;
                        (*father)->nInfos = 1;
                        (*father)->right = NULL;

                    } else {
                        (*root)->info1 = (*father)->left->info1;
                        (*root)->info2 = (*father)->info1;
                        (*root)->nInfos = 2;
                        (*father)->nInfos = 0;

                        (*root)->right = (*root)->middle; 
                        (*root)->middle = (*father)->left->middle; 
                        (*root)->left = (*father)->left->left;

                        free((*father)->left);
                        (*father)->left = NULL;
                    }
                }

            } else if((*root) == ((*father)->right)) {

                if ((*father)->middle->nInfos == 2) {
                    (*root)->info1 = (*father)->info2;
                    (*root)->nInfos = 1;

                    (*father)->info2 = (*father)->middle->info2;
                    (*father)->middle->nInfos = 1;

                    (*root)->left = (*father)->middle->right; 
                    (*father)->middle->right = NULL; 

                } else {
                    (*root)->info1 = (*father)->middle->info1;
                    (*root)->info2 = (*father)->info2;
                    (*root)->nInfos = 2;
                    (*father)->nInfos = 1;

                    (*root)->right = (*root)->middle; 
                    (*root)->middle = (*father)->middle->middle; 
                    (*root)->left = (*father)->middle->left;

                    free((*father)->middle);
                    (*father)->middle = (*root);
                    (*father)->right = NULL;
                }
            }
        } else (*root) = (*root)->middle; 
    }
}

// Substitui um nó raiz pelo maior a sua esquerda.
void swapWithBiggestLeft(Tree23 **root, Tree23 **father, Tree23 **biggestNode, int nInfo) {
    if (biggestNode != NULL) {
        if (isLeaf((*biggestNode))) {
            Info *aux;

            aux = nInfo == 2 ? (*root)->info2 : (*root)->info1;

            // Caso em que a info a ser removida é a primeira do nó.
            if (nInfo == 1) {
                // Se o maior nó encontrado possuir duas infos, a maior será considerada para a troca
                if ((*biggestNode)->nInfos == 2) {
                    (*root)->info1 = (*biggestNode)->info2;
                    (*biggestNode)->info2 = aux;
                } else {
                    (*root)->info1 = (*biggestNode)->info1;
                    (*biggestNode)->info1 = aux;
                }
            // Caso em que a info a ser removida é a segunda do nó.
            } else if (nInfo == 2) {
                if ((*biggestNode)->nInfos == 2) {
                    (*root)->info2 = (*biggestNode)->info2;
                    (*biggestNode)->info2 = aux;
                } else {
                    (*root)->info2 = (*biggestNode)->info1;
                    (*biggestNode)->info1 = aux;
                }
            }

            // Aproveita a recursão e remove a info que foi trocada para o maior nó.
            removeInfo(biggestNode, aux->word, father);
        } 
        else if ((*biggestNode)->nInfos == 2)
            swapWithBiggestLeft(root, biggestNode, &((*biggestNode)->right), nInfo);
        else swapWithBiggestLeft(root, biggestNode, &((*biggestNode)->middle), nInfo); 
    }
    // Balanceia a árvore.
    balance(biggestNode, father);
}

// Remove uma info.
void removeInfo(Tree23 **root, char *word, Tree23 **father) {
    if ((*root) != NULL)
    {
        if (strcmp(word, (*root)->info1->word) == 0) 
        {
            // Caso em que o nó é folha a info procurada é a primeira.
            if (isLeaf((*root))) 
            {
                (*root)->info1 = (*root)->info2;
                (*root)->nInfos--;
            // Se o nó não é folha, e a info encontrada é a primeira, então ela será trocada pela maior info a sua esquerda.
            } else swapWithBiggestLeft(root, root, &(*root)->left, 1);

        } else if ((*root)->nInfos == 2 && strcmp(word, (*root)->info2->word) == 0) 
        {
            // O número de infos somente é decrementado.
            if (isLeaf(*root))
                (*root)->nInfos--;
            // Se o nó não é folha, e a info encontrada é a segunda, então ela será trocada pela maior info a sua esquerda, no caso, a do meio.
            else swapWithBiggestLeft(root, root, &(*root)->middle, 2);
        }
		else if (strcmp(word, (*root)->info1->word) < 0)
            removeInfo(&(*root)->left, word, root);
        else if ((*root)->nInfos == 1 || ((*root)->nInfos == 2 && strcmp(word, (*root)->info2->word) < 0))
            removeInfo(&(*root)->middle, word, root);
        else removeInfo(&(*root)->right, word, root);
    }

    balance(root, father);
}

void removeWordFromALine(Tree23 **root, char *word, int line) {
    Info **addressInfoFound = searchInfo(root, word);
    Tree23 *father = NULL;

    if(addressInfoFound != NULL) {
        removeLine(&(*addressInfoFound)->listOfLines, line);
        
        // Se não restarem mais linhas, o nó é removido.
        if((*addressInfoFound)->listOfLines == NULL) {
            removeInfo(root, word, &father);
        }
    }
}

Info **searchInfoWithTrack(Tree23 **root, char *word, int level) {
    Info **addressInfoFound = NULL;

    if ((*root) != NULL){
        if(strcmp(word, (*root)->info1->word) < 0)
        {
            printf("\nLevel: %d -> left", level);
            addressInfoFound = searchInfoWithTrack(&(*root)->left, word, level + 1);
        }
        else if(strcmp(word, (*root)->info1->word) == 0)
        {
            printf("\nLevel: %d -> found", level);
            addressInfoFound = &(*root)->info1;
        }
        else if(((*root)->nInfos == 1) || ((*root)->nInfos == 2 && strcmp(word, (*root)->info2->word) < 0))
        {
            printf("\nLevel: %d -> middle", level);
            addressInfoFound = searchInfoWithTrack(&(*root)->middle, word, level + 1);
        }
        else if(strcmp(word, (*root)->info2->word) == 0)
        {
            printf("\nLevel: %d -> found", level);
            addressInfoFound = &(*root)->info2;
        } else {
            printf("\nLevel: %d -> right", level);
            addressInfoFound = searchInfoWithTrack(&(*root)->right, word, level + 1);  
        } 
    }

    return addressInfoFound;
}


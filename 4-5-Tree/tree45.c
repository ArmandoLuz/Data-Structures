#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree45.h"

struct tree45
{
    int nInfos;
    Tree45 *left;
    Info *info1;
    Tree45 *middleA;
    Info *info2;
    Tree45 *middleB;
    Info *info3;
    Tree45 *middleC;
    Info *info4;
    Tree45 *right;
};

struct info
{
    int code, quantity, size;
    float price;
    char type[50], mark[50];
};

Tree45 *insertIn45Tree(Tree45 **root, Info *info, Tree45 *fatherRoot, Info **up)
{
    Tree45 *biggestNode = NULL, *oldBiggestNode = NULL;

    if((*root) == NULL)
    {
        (*root) = createNode(info, NULL, NULL, NULL, NULL, NULL, NULL);
    } else {
        if(isLeaf((*root)))
        {
            if((*root)->nInfos < 4)
            {
                addNode(root, info, NULL); 
            } else {
                if(insertIfEqual((*root), info) == 0){
                    biggestNode = nodeBreaker(root, info, up);
                    if(fatherRoot == NULL)
                    {
                        (*root) = createNode((*up), NULL, (*root), biggestNode, NULL, NULL, NULL);
                        biggestNode = NULL;
                    }
                }
            }
        } else if (insertIfEqual((*root), info) == 0) {
            if(info->code < (*root)->info1->code)
                biggestNode = insertIn45Tree(&(*root)->left, info, (*root), up);
            else if(((*root)->nInfos == 1) || ((*root)->nInfos >= 2 && info->code < (*root)->info2->code))
                biggestNode = insertIn45Tree(&(*root)->middleA, info, (*root), up);
            else if(((*root)->nInfos == 2) || ((*root)->nInfos >= 3 && info->code < (*root)->info3->code))
                biggestNode = insertIn45Tree(&(*root)->middleB, info, (*root), up);
            else if(((*root)->nInfos == 3) || ((*root)->nInfos >= 4 && info->code < (*root)->info4->code))
                biggestNode = insertIn45Tree(&(*root)->middleC, info, (*root), up);
            else biggestNode = insertIn45Tree(&(*root)->right, info, (*root), up);  
        }

        if(biggestNode != NULL && biggestNode->info1 != (*root)->info3 )
        {
            if((*root)->nInfos < 4)
            {
                addNode(root, (*up), biggestNode);
                biggestNode = NULL;
            } else {
                oldBiggestNode = biggestNode;
                biggestNode = nodeBreaker(root, (*up), up); 

                if((*root)->middleB == biggestNode->left && oldBiggestNode->info1->code > biggestNode->left->info1->code)
                {
                    if(biggestNode->middleA->info1->code > oldBiggestNode->info1->code)
                    {
                        biggestNode->left = oldBiggestNode;
                    } else if(biggestNode->middleA->info1->code < oldBiggestNode->info1->code && oldBiggestNode->info1->code < biggestNode->middleB->info1->code)
                    {
                        biggestNode->left = biggestNode->middleA;
                        biggestNode->middleA = oldBiggestNode;
                    } else {
                        biggestNode->left = biggestNode->middleA;
                        biggestNode->middleA = biggestNode->middleB;
                        biggestNode->middleB = oldBiggestNode;
                    }
                } else if((*root)->middleB == biggestNode->left && oldBiggestNode->info1->code < biggestNode->left->info1->code)
                {
                    if((*root)->middleA->info1->code > oldBiggestNode->info1->code) {
                        (*root)->middleB = (*root)->middleA;
                        (*root)->middleA = oldBiggestNode;

                    } else (*root)->middleB = oldBiggestNode;
                }

                if(fatherRoot == NULL)
                {
                    (*root) = createNode((*up), NULL, (*root), biggestNode, NULL, NULL, NULL);
                    biggestNode = NULL;
                }
            }
        }
    }

    return biggestNode;
}

Tree45 *createNode(Info *info1, Info *info2, Tree45 *childLeft, Tree45 *childMiddleA, Tree45 *childMiddleB, Tree45 *childMiddleC, Tree45 *childRight)
{
    Tree45 *newNode = (Tree45 *) malloc(sizeof(Tree45));

    newNode->info1 = info1;
    newNode->info2 = info2;
    newNode->info3 = NULL;
    newNode->info3 = NULL;
    newNode->info4 = NULL;

    if(info2)
        newNode->nInfos = 2;
    else newNode->nInfos = 1;

    newNode->left = childLeft;
    newNode->middleA = childMiddleA;
    newNode->middleB = childMiddleB;
    newNode->middleC = childMiddleC;
    newNode->right = childRight;

    return newNode;
}

Info *createInfo(int id, int quantity, char *mark, float price, int size, char *type)
{
    Info *newInfo;
    newInfo = (Info *) malloc(sizeof(Info));

    newInfo->code = id;
    strcpy(newInfo->mark, mark);
    newInfo->price = price;
    newInfo->quantity = quantity;
    newInfo->size = size;
    strcpy(newInfo->type, type);

    return newInfo;
}

void addNode(Tree45 **root, Info *info, Tree45 *childRight)
{
    if((*root)->info1->code < info->code && (*root)->nInfos > 1)
    {
        if((*root)->info2->code < info->code && (*root)->nInfos > 2) 
        {
            if((*root)->info3->code < info->code)
            {
                (*root)->info4 = info;

                (*root)->right = childRight;
            } else {
                (*root)->info4 = (*root)->info3;
                (*root)->info3 = info;

                (*root)->right = (*root)->middleC;
                (*root)->middleC = childRight;
            } 
        } else if((*root)->info2->code > info->code)
        {
            (*root)->info4 = (*root)->info3;
            (*root)->info3 = (*root)->info2;
            (*root)->info2 = info;

            (*root)->right = (*root)->middleC;
            (*root)->middleC = (*root)->middleB;
            (*root)->middleB = childRight;

        } else {
            (*root)->info3 = info;

            (*root)->middleC = childRight;
        }

    } else if((*root)->info1->code > info->code)
    {
        (*root)->info4 = (*root)->info3;
        (*root)->info3 = (*root)->info2;
        (*root)->info2 = (*root)->info1;
        (*root)->info1 = info;

        (*root)->right = (*root)->middleC;
        (*root)->middleC = (*root)->middleB;
        (*root)->middleB = (*root)->middleA;
        (*root)->middleA = childRight;

    } else {
        (*root)->info2 = info;

        (*root)->middleB = childRight;
    }

    (*root)->nInfos++;
}

Tree45 *nodeBreaker(Tree45 **root, Info *info, Info **up)
{
    Tree45 *newNode;

    if(info->code < (*root)->info2->code)
    {
        (*up) = (*root)->info2;

        if(info->code < (*root)->info1->code) 
        {
            (*root)->info2 = (*root)->info1;
            (*root)->info1 = info;
        } else (*root)->info2 = info;
    
    } else if(info->code > (*root)->info3->code)
    {
        (*up) = (*root)->info3;

        if(info->code > (*root)->info4->code)
        {
            (*root)->info3 = (*root)->info4;
            (*root)->info4 = info;
        } else (*root)->info3 = info;

    } else (*up) = info;

    newNode = createNode((*root)->info3, (*root)->info4, (*root)->middleB, (*root)->middleC, (*root)->right, NULL, NULL);
    (*root)->nInfos = 2;

    return newNode;
}

int insertIfEqual(Tree45 *root, Info *info)
{
    int isEqual = 0;
    if(root->nInfos >= 1){
        if(root->info1->code == info->code)
        {
            root->info1->quantity += info->quantity;
            isEqual = 1;
        } 
    }

    if(root->nInfos >= 2)
    {
        if(root->info2->code == info->code)
        {
            root->info2->quantity += info->quantity;
            isEqual = 1;
        } 
    }

    if(root->nInfos >= 3)
    {
        if(root->info3->code == info->code)
        {
            root->info3->quantity += info->quantity;
            isEqual = 1;
        } 
    }
    
    if(root->nInfos == 4) {
        if(root->info4->code == info->code)
        {
            root->info4->quantity += info->quantity;
            isEqual = 1;
        }
    }
        

    return isEqual;
}

int isLeaf(Tree45 *node)
{
    return !node->left && !node->middleA && !node->middleB && !node->middleC && !node->right ? 1 : 0;
}

void printTreeWithLevel(Tree45 *root, int level) {
    if(root != NULL)
    {
        for(int i = 0; i < level; i++)
            printf("\t");

        printf("Level: %d   ", level);
        printf("info 1 -> %d   ", root->info1->code);

        if(root->nInfos >= 2)
            printf("info 2 -> %d   ", root->info2->code);

        if(root->nInfos >= 3)
            printf("info 3 -> %d   ", root->info3->code);

        if(root->nInfos == 4)
            printf("info 4 -> %d   ", root->info4->code);
        
        printf("\n");

        printTreeWithLevel(root->left, level + 1);
        printTreeWithLevel(root->middleA, level + 1);
        
        if(root->nInfos >= 2)
            printTreeWithLevel(root->middleB, level + 1);

        if(root->nInfos >= 3)
            printTreeWithLevel(root->middleC, level + 1);

        if(root->nInfos == 4)
            printTreeWithLevel(root->right, level + 1);
    }
}

void printTreeInOrder(Tree45 *root)
{
    if(root != NULL)
    {
        printTreeInOrder(root->left);

        printf("%d\n", root->info1->code);

        printTreeInOrder(root->middleA);

        if(root->nInfos >= 2)
        {
            printf("%d\n", root->info2->code);
            printTreeInOrder(root->middleB);
        }

        if(root->nInfos >= 3)
        {
            printf("%d\n", root->info3->code);
            printTreeInOrder(root->middleC);
        }

        if(root->nInfos == 4)
        {
            printf("%d\n", root->info4->code);
            printTreeInOrder(root->right);
        }
    }
}

void saveData(Tree45 *root, FILE *archive)
{
    if(root != NULL)
    {
        fprintf(archive, "%d %d %d %.2f %s %s\n", root->info1->code, root->info1->quantity, root->info1->size, root->info1->price, root->info1->type, root->info1->mark);

        if(root->nInfos >= 2)
            fprintf(archive, "%d %d %d %2.f %s %s\n", root->info2->code, root->info2->quantity, root->info2->size, root->info2->price, root->info2->type, root->info2->mark);

        if(root->nInfos >= 3)
            fprintf(archive, "%d %d %d %2.f %s %s\n", root->info3->code, root->info3->quantity, root->info3->size, root->info3->price, root->info3->type, root->info3->mark);

        if(root->nInfos == 4)
            fprintf(archive, "%d %d %d %2.f %s %s\n", root->info4->code, root->info4->quantity, root->info4->size, root->info4->price, root->info4->type, root->info4->mark);
    
        saveData(root->left, archive);
        saveData(root->middleA, archive);

        if(root->nInfos >= 2)
            saveData(root->middleB, archive);
        
        if(root->nInfos >= 3)
            saveData(root->middleC, archive);

        if(root->nInfos == 4)   
            saveData(root->right, archive);
    }
}

Info *searchInfo(Tree45 *root, int value)
{
    Info *addressFound = NULL;

    if(root != NULL)
    {
        if(value < root->info1->code)
            addressFound = searchInfo(root->left, value);
        else if(value == root->info1->code)
            addressFound = root->info1;
        else if((root->nInfos == 1) || (root->nInfos >= 2 && value < root->info2->code))
            addressFound = searchInfo(root->middleA, value);
        else if(value == root->info2->code)
            addressFound = root->info2;
        else if((root->nInfos == 2) || (root->nInfos >= 3 && value < root->info3->code))
            addressFound = searchInfo(root->middleB, value);
        else if(value == root->info3->code)
            addressFound = root->info3;
        else if((root->nInfos == 3) || (root->nInfos >= 4 && value < root->info4->code))
            addressFound = searchInfo(root->middleC, value);
        else if(value == root->info4->code)
            addressFound = root->info4;
        else addressFound = searchInfo(root->right, value);  
    }

    return addressFound;
}

void printInfo(Info *info)
{
    if(info)
    {
        printf("\nCode: %d", info->code);
        printf("\nQuant: %d", info->quantity);
        printf("\nSize: %d", info->size);
        printf("\nPrice: %.2f", info->price);
        printf("\nType: %s", info->type);
        printf("\nMark: %s", info->mark);
    } else printf("\nProduto inexistente!");
}

// Faz o balanceamento dos nós.
void balance(Tree45 **root, Tree45 **father) 
{
    // A árvore 4-5 não pode ter um nó com menos de duas informações.
    if ((*root) != NULL && (*root)->nInfos == 1) {
        if ((*father) != NULL) {
            //Caso em que a remoção foi feita na esquerda.
            if ((*root) == (*father)->left) {
                
                (*root)->info2 = (*father)->info1;
                (*root)->nInfos++;
                (*father)->nInfos--;

                if((*father)->middleA->nInfos > 2)
                {
                    (*father)->info1 = (*father)->middleA->info1;
                    (*father)->nInfos++;
                    (*father)->middleA->nInfos--;
                    
                    //Afastando as informações para a esquerda
                    (*father)->middleA->info1 = (*father)->middleA->info2;
                    (*father)->middleA->info2 = (*father)->middleA->info3;
                    (*father)->middleA->info3 = (*father)->middleA->info4;
                    (*father)->middleA->info4 = NULL;

                    //Afastando os nós para a esquerda.
                    (*root)->middleB = (*father)->middleA->left;
                    (*father)->middleA->left = (*father)->middleA->middleA;
                    (*father)->middleA->middleA = (*father)->middleA->middleB;
                    (*father)->middleA->middleB = (*father)->middleA->middleC;
                    (*father)->middleA->middleC = (*father)->middleA->right;
                    (*father)->middleA->right = NULL;

                } else {
                    //Ajustando o pai.
                    (*father)->info1 = (*father)->info2;
                    (*father)->info2 = (*father)->info3;
                    (*father)->info3 = (*father)->info4;
                    (*father)->info4 = NULL;

                    // União de left com middleA
                    (*root)->info3 = (*father)->middleA->info1;
                    (*root)->info4 = (*father)->middleA->info2;

                    (*root)->nInfos += (*father)->middleA->nInfos;

                    //Conservando as ligações.
                    (*root)->middleB = (*father)->middleA->left;
                    (*root)->middleC = (*father)->middleA->middleA;
                    (*root)->right = (*father)->middleA->middleB;

                    //Ajustando os ponteiros do pai.
                    (*father)->left = (*root);
                    (*father)->middleA = (*father)->middleB;
                    (*father)->middleB = (*father)->middleC;
                    (*father)->middleC = (*father)->right;
                    (*father)->right = NULL;
                }
            //Caso em que a remoção foi feita no meio A.
            } else if((*root) == (*father)->middleA)
            {
                // Caso em que a esquerda não possui infos suficientes para trocar e o meio B possui.
                if((*father)->left->nInfos == 2 && (*father)->nInfos >= 2 && (*father)->middleB->nInfos > 2)
                {
                    (*root)->info2 = (*father)->info2;
                    (*root)->nInfos++;

                    (*father)->info2 = (*father)->middleB->info1;
                    (*father)->middleB->nInfos--;

                    //Afastando as infos para a esquerda.
                    (*father)->middleB->info1 = (*father)->middleB->info2;
                    (*father)->middleB->info2 = (*father)->middleB->info3;
                    (*father)->middleB->info3 = (*father)->middleB->info4;
                    (*father)->middleB->info4 = NULL;

                    //Ajustando os filhos.
                    (*root)->middleB = (*father)->middleB->left;
                    (*father)->middleB->left = (*father)->middleB->middleA;
                    (*father)->middleB->middleA = (*father)->middleB->middleB;
                    (*father)->middleB->middleB = (*father)->middleB->middleC;
                    (*father)->middleB->middleC = (*father)->middleB->right;
                    (*father)->middleB->right = NULL;

                } else if((*father)->left->nInfos == 2){
                    //Ajustando infos da raiz.
                    (*root)->info4 = (*root)->info1;
                    (*root)->info3 = (*father)->info1;
                    (*root)->info2 = (*father)->left->info2;
                    (*root)->info1 = (*father)->left->info1;
                    (*root)->nInfos = 4;

                    // Ajustando os ponteiros da raiz.
                    (*root)->right = (*root)->middleA;
                    (*root)->middleC = (*root)->left;
                    (*root)->middleB = (*father)->left->middleB;
                    (*root)->middleA = (*father)->left->middleA;
                    (*root)->left = (*father)->left->left;

                    //Ajustando infos do pai.
                    (*father)->info1 = (*father)->info2;
                    (*father)->info2 = (*father)->info3;
                    (*father)->info3 = (*father)->info4;
                    (*father)->info4 = NULL;
                    (*father)->nInfos--;

                    //Ajustando os ponteiros.
                    (*father)->left = (*root); // Quando a remoção é no meio A ou na esquerda, o nó raiz tem de ser atribuido a esquerda do nó pai.
                    (*father)->middleA = (*father)->middleB;
                    (*father)->middleB = (*father)->middleC;
                    (*father)->middleC = (*father)->right;
                    (*father)->right = NULL;
                } else {
                    // Ajustando as infos da raiz.
                    (*root)->info2 = (*root)->info1;
                    (*root)->info1 = (*father)->info1;
                    (*root)->nInfos++;

                    // Ajustando as infos do pai.
                    (*father)->info1 = (*father)->left->nInfos == 4 ? (*father)->left->info4 : (*father)->left->info3;
                    (*father)->left->nInfos--;

                    //Ajustando os filhos.
                    (*root)->middleB = (*root)->middleA;
                    (*root)->middleA = (*root)->left;
                    (*root)->left = (*father)->left->nInfos == 4 ? (*father)->left->right : (*father)->left->middleC;
                }
            //Caso em que a remoção foi feita no meio B.
            } else if((*root) == (*father)->middleB)
            {
                if((*father)->middleA->nInfos == 2 && (*father)->nInfos >= 3 && (*father)->middleC->nInfos > 2)
                {
                    (*root)->info2 = (*father)->info3;
                    (*root)->nInfos++;

                    (*father)->info3 = (*father)->middleC->info1;
                    (*father)->middleC->nInfos--;

                    //Afastando as infos para a esquerda.
                    (*father)->middleC->info1 = (*father)->middleC->info2;
                    (*father)->middleC->info2 = (*father)->middleC->info3;
                    (*father)->middleC->info3 = (*father)->middleC->info4;
                    (*father)->middleC->info4 = NULL;

                    //Ajustando os filhos.
                    (*root)->middleB = (*father)->middleC->left;
                    (*father)->middleC->left = (*father)->middleC->middleA;
                    (*father)->middleC->middleA = (*father)->middleC->middleB;
                    (*father)->middleC->middleB = (*father)->middleC->middleC;
                    (*father)->middleC->middleC = (*father)->middleC->right;
                    (*father)->middleC->right = NULL;

                } else if((*father)->middleA->nInfos == 2)
                {
                    //Ajustando infos da raiz.
                    (*root)->info4 = (*root)->info1;
                    (*root)->info3 = (*father)->info2;
                    (*root)->info2 = (*father)->middleA->info2;
                    (*root)->info1 = (*father)->middleA->info1;
                    (*root)->nInfos = 4;

                    // Ajustando os ponteiros da raiz.
                    (*root)->right = (*root)->middleA;
                    (*root)->middleC = (*root)->left;
                    (*root)->middleB = (*father)->middleA->middleB;
                    (*root)->middleA = (*father)->middleA->middleA;
                    (*root)->left = (*father)->middleA->left;

                    //Ajustando infos do pai.
                    (*father)->info2 = (*father)->info3;
                    (*father)->info3 = (*father)->info4;
                    (*father)->info4 = NULL;
                    (*father)->nInfos--;

                    //Ajustando os ponteiros.
                    (*father)->middleA = (*root);
                    (*father)->middleB = (*father)->middleC;
                    (*father)->middleC = (*father)->right;
                    (*father)->right = NULL;

                } else {
                    // Ajustando as infos da raiz.
                    (*root)->info2 = (*root)->info1;
                    (*root)->info1 = (*father)->info2;
                    (*root)->nInfos++;

                    // Ajustando as infos do pai.
                    (*father)->info2 = (*father)->middleA->nInfos == 4 ? (*father)->middleA->info4 : (*father)->middleA->info3;
                    (*father)->middleA->nInfos--;

                    //Ajustando os filhos.
                    (*root)->middleB = (*root)->middleA;
                    (*root)->middleA = (*root)->left;
                    (*root)->left = (*father)->middleA->nInfos == 4 ? (*father)->middleA->right : (*father)->middleA->middleC;
                }
            //Caso em que a remoção foi feita no meio C.
            } else if((*root) == (*father)->middleC)
            {
                if((*father)->middleB->nInfos == 2 && (*father)->nInfos == 4 && (*father)->right->nInfos > 2) // Corrigir em todo o código.
                {
                    (*root)->info2 = (*father)->info4;
                    (*root)->nInfos++;

                    (*father)->info4 = (*father)->right->info1;
                    (*father)->right->nInfos--;

                    //Afastando as infos para a esquerda.
                    (*father)->right->info1 = (*father)->right->info2;
                    (*father)->right->info2 = (*father)->right->info3;
                    (*father)->right->info3 = (*father)->right->info4;
                    (*father)->right->info4 = NULL;

                    //Ajustando os filhos.
                    (*root)->middleB = (*father)->right->left;
                    (*father)->right->left = (*father)->right->middleA;
                    (*father)->right->middleA = (*father)->right->middleB;
                    (*father)->right->middleB = (*father)->right->middleC;
                    (*father)->right->middleC = (*father)->right->right;
                    (*father)->right->right = NULL;
                } else if((*father)->middleB->nInfos == 2)
                {
                    //Ajustando infos da raiz.
                    (*root)->info4 = (*root)->info1;
                    (*root)->info3 = (*father)->info3;
                    (*root)->info2 = (*father)->middleB->info2;
                    (*root)->info1 = (*father)->middleB->info1;
                    (*root)->nInfos = 4; // Corrigir em todo o código.

                    // Ajustando os ponteiros da raiz.
                    (*root)->right = (*root)->middleA;
                    (*root)->middleC = (*root)->left;
                    (*root)->middleB = (*father)->middleB->middleB;
                    (*root)->middleA = (*father)->middleB->middleA;
                    (*root)->left = (*father)->middleB->left;

                    //Ajustando infos do pai.
                    (*father)->info3 = (*father)->info4;
                    (*father)->info4 = NULL;
                    (*father)->nInfos--;

                    //Ajustando os ponteiros.
                    (*father)->middleB = (*root);
                    (*father)->middleC = (*father)->right;
                    (*father)->right = NULL;
                } else {
                    // Ajustando as infos da raiz.
                    (*root)->info2 = (*root)->info1;
                    (*root)->info1 = (*father)->info3;
                    (*root)->nInfos++;

                    // Ajustando as infos do pai.
                    (*father)->info3 = (*father)->middleB->nInfos == 4 ? (*father)->middleB->info4 : (*father)->middleB->info3; 
                    (*father)->middleB->nInfos--;

                    //Ajustando os filhos.
                    (*root)->middleB = (*root)->middleA;
                    (*root)->middleA = (*root)->left;
                    (*root)->left = (*father)->middleB->nInfos == 4 ? (*father)->middleB->right : (*father)->middleB->middleC;

                }
            } else {
                // Movendo a info 1 para a info 2
                (*root)->info2 = (*root)->info1;
                
                //Preenchendo a info 1 com o 4º nó do pai.
                (*root)->info1 = (*father)->info4;
                (*root)->nInfos++;
                (*father)->nInfos--;

                if((*father)->middleC->nInfos > 2)
                {
                    // Preenchendo o 4º nó do pai.
                    (*father)->info4 = (*father)->middleC->nInfos == 4 ? (*father)->middleC->info4 : (*father)->middleC->info3;
                    (*father)->middleC->nInfos--;
                    (*father)->nInfos++;

                    //Ajustando os filhos.
                    (*root)->middleB = (*root)->middleA;
                    (*root)->middleA = (*root)->left;
                    (*root)->left = (*father)->middleC->nInfos == 4 ? (*father)->middleC->right : (*father)->middleC->middleC;
                } else {
                    // Ajustando as infos da raiz.
                    (*root)->info4 = (*root)->info1;
                    (*root)->info3 = (*father)->info4;
                    (*root)->info2 = (*father)->middleC->info2;
                    (*root)->info1 = (*father)->middleC->info1;

                    // Ajustando o pai.
                    (*father)->nInfos--;
                    (*father)->middleC = (*root);
                    (*father)->right = NULL;
                }
            }

            if((*father) != NULL && (*father)->nInfos == 0)
                (*father) = (*father)->left;   
        }
    } 
    else if((*root) != NULL && (*root)->nInfos == 0)
        (*root) = NULL;
}

// Substitui um nó raiz pelo maior a sua esquerda.
void swapWithBiggestLeft(Tree45 **root, Tree45 **father, Tree45 **biggestNode, int nInfo) 
{
    if (biggestNode != NULL) {
        if (isLeaf((*biggestNode))) {
            Info *aux;

            // Caso em que a info a ser removida é a primeira do nó raiz.
            if (nInfo == 1) {
                aux = (*root)->info1;
                // Se o maior nó encontrado possuir mais de uma info, a maior será considerada para a troca com o nó raiz, o elemento a ser removido.
                if((*biggestNode)->nInfos == 4)
                {
                    (*root)->info1 = (*biggestNode)->info4;
                    (*biggestNode)->info4 = aux;
                } else if((*biggestNode)->nInfos == 3)
                {
                    (*root)->info1 = (*biggestNode)->info3;
                    (*biggestNode)->info3 = aux;
                }else if ((*biggestNode)->nInfos == 2) {
                    (*root)->info1 = (*biggestNode)->info2;
                    (*biggestNode)->info2 = aux;
                } else {
                    (*root)->info1 = (*biggestNode)->info1;
                    (*biggestNode)->info1 = aux;
                }
            // Caso em que a info a ser removida é a segunda do nó raiz.
            } else if (nInfo == 2) {
                aux = (*root)->info2;

                if((*biggestNode)->nInfos == 4)
                {
                    (*root)->info2 = (*biggestNode)->info4;
                    (*biggestNode)->info4 = aux;
                } else if((*biggestNode)->nInfos == 3)
                {
                    (*root)->info2 = (*biggestNode)->info3;
                    (*biggestNode)->info3 = aux;
                }else if ((*biggestNode)->nInfos == 2) {
                    (*root)->info2 = (*biggestNode)->info2;
                    (*biggestNode)->info2 = aux;
                } else {
                    (*root)->info2 = (*biggestNode)->info1;
                    (*biggestNode)->info1 = aux;
                }
            } else if (nInfo == 3) {
                aux = (*root)->info3;

                if((*biggestNode)->nInfos == 4)
                {
                    (*root)->info3 = (*biggestNode)->info4;
                    (*biggestNode)->info4 = aux;
                } else if((*biggestNode)->nInfos == 3)
                {
                    (*root)->info3 = (*biggestNode)->info3;
                    (*biggestNode)->info3 = aux;
                }else if ((*biggestNode)->nInfos == 2) {
                    (*root)->info3 = (*biggestNode)->info2;
                    (*biggestNode)->info2 = aux;
                } else {
                    (*root)->info3 = (*biggestNode)->info1;
                    (*biggestNode)->info1 = aux;
                }
            } else {
                aux = (*root)->info4;

                if((*biggestNode)->nInfos == 4)
                {
                    (*root)->info4 = (*biggestNode)->info4;
                    (*biggestNode)->info4 = aux;
                } else if((*biggestNode)->nInfos == 3)
                {
                    (*root)->info4 = (*biggestNode)->info3;
                    (*biggestNode)->info3 = aux;
                }else if ((*biggestNode)->nInfos == 2) {
                    (*root)->info4 = (*biggestNode)->info2;
                    (*biggestNode)->info2 = aux;
                } else {
                    (*root)->info4 = (*biggestNode)->info1;
                    (*biggestNode)->info1 = aux;
                }
            } 

            // Aproveita a recursão e remove a info que foi trocada para o maior nó.
            removeInfo(biggestNode, aux->code, father);
        } 
        else if ((*biggestNode)->nInfos == 1)
            swapWithBiggestLeft(root, biggestNode, &((*biggestNode)->middleA), nInfo);
        else if((*biggestNode)->nInfos == 2) 
            swapWithBiggestLeft(root, biggestNode, &((*biggestNode)->middleB), nInfo);
        else if((*biggestNode)->nInfos == 3)
            swapWithBiggestLeft(root, biggestNode, &((*biggestNode)->middleC), nInfo);
        else swapWithBiggestLeft(root, biggestNode, &((*biggestNode)->right), nInfo);
    }
    // Balanceia a árvore.
    balance(biggestNode, father);
}

// Remove uma info.
void removeInfo(Tree45 **root, int id, Tree45 **father) 
{
    if ((*root) != NULL)
    {
        if (id == (*root)->info1->code) 
        {
            // Caso em que o nó é folha a info procurada é a primeira.
            if (isLeaf((*root))) 
            {
                // As infos são afastadas para a esquerda.
                (*root)->info1 = (*root)->info2;
                (*root)->info2 = (*root)->info3;
                (*root)->info3 = (*root)->info4;
                (*root)->nInfos--;

            // Se o nó não é folha, e a info encontrada é a primeira, então ela será trocada pela maior info a sua esquerda.
            } else swapWithBiggestLeft(root, root, &(*root)->left, 1); // Ajeitar a função de troca.

        } else if ((*root)->nInfos >= 2 && id == (*root)->info2->code) 
        {
            // Caso em que o nó é folha a info procurada é a segunda.
            if (isLeaf((*root)))
            {
                // As infos são afastadas para a esquerda a partir da segunda.
                (*root)->info2 = (*root)->info3;
                (*root)->info3 = (*root)->info4;
                (*root)->nInfos--;
            }
            // Se o nó não é folha, e a info encontrada é a segunda, então ela será trocada pela maior info a sua esquerda, no caso, a do meio A.
            else swapWithBiggestLeft(root, root, &(*root)->middleA, 2);
        } else if ((*root)->nInfos >= 3 && id == (*root)->info3->code)
        {
            // Caso em que o nó é folha a info procurada é a terceira.
            if(isLeaf((*root)))
            {
                // As infos são afastadas para a esquerda a partir da terceira.
                (*root)->info3 = (*root)->info4;
                (*root)->nInfos--;
            }
            // Se o nó não é folha, e a info encontrada é a segunda, então ela será trocada pela maior info a sua esquerda, no caso, a do meio B. 
            else swapWithBiggestLeft(root, root, &(*root)->middleB, 3);
        } else if((*root)->nInfos == 4 && id == (*root)->info4->code)
        {
            // Caso em que o nó é folha a info procurada é a quarta.
            if(isLeaf((*root)))
                (*root)->nInfos--; //Número de infos somente é decrementado.

            // Se o nó não é folha, e a info encontrada é a segunda, então ela será trocada pela maior info a sua esquerda, no caso, a do meio B. 
            else swapWithBiggestLeft(root, root, &(*root)->middleC, 4);
        }
		else if (id < (*root)->info1->code)
            removeInfo(&(*root)->left, id, root);
        else if ((*root)->nInfos == 1 || ((*root)->nInfos >= 2 && id < (*root)->info2->code))
            removeInfo(&(*root)->middleA, id, root);
        else if ((*root)->nInfos == 2 || ((*root)->nInfos >= 3 && id < (*root)->info3->code))
            removeInfo(&(*root)->middleB, id, root);
        else if ((*root)->nInfos == 3 || ((*root)->nInfos == 4 && id < (*root)->info4->code))
            removeInfo(&(*root)->middleC, id, root);
        else removeInfo(&(*root)->right, id, root);
    }

    balance(root, father);
}

void sell(Tree45 **root, int id, int quantity) {
    Info *infoFound = searchInfo((*root), id);
    Tree45 *father = NULL;

    if(infoFound != NULL) {
        if(infoFound->quantity >= quantity)
        {
            infoFound->quantity -= quantity;
        
            // Se não restar mais estoque, o nó é removido.
            if(infoFound->quantity == 0)
                removeInfo(root, id, &father);

        } else printf("Estoque insuficiente");
    } else printf("Produto nao encontrado");
}

Info *searchInfoWithTrack(Tree45 *root, int value, int level)
{
    Info *addressFound = NULL;

    if(root != NULL)
    {
        if(value < root->info1->code)
        {
            printf("\nLevel: %d -> left", level);
            addressFound = searchInfoWithTrack(root->left, value, level + 1);
        }
        else if(value == root->info1->code)
        {
            printf("\nLevel: %d -> found", level);
            addressFound = root->info1;
        }
        else if((root->nInfos == 1) || (root->nInfos >= 2 && value < root->info2->code))
        {
            printf("\nLevel: %d -> middleA", level);
            addressFound = searchInfoWithTrack(root->middleA, value, level + 1);
        }
        else if(value == root->info2->code)
        {
            printf("\nLevel: %d -> found", level);
            addressFound = root->info2;
        }
        else if((root->nInfos == 2) || (root->nInfos >= 3 && value < root->info3->code))
        {
            printf("\nLevel: %d -> middleB", level);
            addressFound = searchInfoWithTrack(root->middleB, value, level + 1);
        }
        else if(value == root->info3->code)
        {
            printf("\nLevel: %d -> found", level);
            addressFound = root->info3;
        }
        else if((root->nInfos == 3) || (root->nInfos >= 4 && value < root->info4->code))
        {
            printf("\nLevel: %d -> middleC", level);
            addressFound = searchInfoWithTrack(root->middleC, value, level + 1);
        }
        else if(value == root->info4->code)
        {
            printf("\nLevel: %d -> found", level);
            addressFound = root->info4;
        }
        else {
            printf("\nLevel: %d -> right", level);
            addressFound = searchInfoWithTrack(root->right, value, level + 1);
        }  
    }

    return addressFound;
}


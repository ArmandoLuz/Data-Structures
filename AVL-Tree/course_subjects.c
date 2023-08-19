#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "course_subjects.h"

struct courseSubject
{
    int id, block, workload, height;
    char name[30];
    CourseSubject *left, *right;
};

CourseSubject *createSubjectTree()
{
    return NULL;
}

//----------------------------BUSCA------------------------------------------
CourseSubject *searchInTheSubjectTree(CourseSubject *tree, int id)
{
    CourseSubject *addressFound;

    if(tree != NULL)
    {
        if(tree->id > id)
            addressFound = searchInTheSubjectTree(tree->left, id);
        else if (tree->id < id)
            addressFound = searchInTheSubjectTree(tree->right, id);
        else addressFound = tree;
    } else addressFound = tree;

    return addressFound;
}
//---------------------------------------------------------------------------

//-----------------------------BALANCEAMENTO---------------------------------
void balanceSubjectTree(CourseSubject **tree){
    int balancingFactor;

    balancingFactor = calculeBalancingFactorOfSubjectTree((*tree));

    if(balancingFactor == 2)
    {
        if(calculeBalancingFactorOfSubjectTree((*tree)->left) < 0)
            leftRotationInTheSubjectTree(&(*tree)->left);
        rightRotationInTheSubjectTree(tree);
    } else if(balancingFactor == -2)
    {
        if(calculeBalancingFactorOfSubjectTree((*tree)->right) > 0)
            rightRotationInTheSubjectTree(&(*tree)->right);
        leftRotationInTheSubjectTree(tree);
    }

    updateSubjectTreeHeight(tree);
}

void leftRotationInTheSubjectTree(CourseSubject **tree){
    CourseSubject *aux;

    aux = (*tree)->right;
    (*tree)->right = (*tree)->right->left;
    aux->left = (*tree);
    (*tree) = aux;


    updateSubjectTreeHeight(tree);
    updateSubjectTreeHeight(&aux->left);
}

void rightRotationInTheSubjectTree(CourseSubject **tree){
    CourseSubject *aux;

    aux = (*tree)->left;
    (*tree)->left = (*tree)->left->right;
    aux->right = (*tree);
    (*tree) = aux;

    updateSubjectTreeHeight(tree);
    updateSubjectTreeHeight(&aux->right);
}

int calculeBalancingFactorOfSubjectTree(CourseSubject *tree){
    int factorBalancing;

    if(tree->left != NULL && tree->right != NULL)
        factorBalancing = tree->left->height - tree->right->height;
    else if(tree->left != NULL)
        factorBalancing = tree->left->height + 1;
    else if(tree->right != NULL)
        factorBalancing = -1 - tree->right->height;
    else 
        factorBalancing = 0;

    return factorBalancing;
}

void updateSubjectTreeHeight(CourseSubject **tree){
    if((*tree)->left != NULL && (*tree)->right != NULL)
        (*tree)->height = (*tree)->left->height > (*tree)->right->height ? (*tree)->left->height + 1 : (*tree)->right->height + 1;
    else if((*tree)->left != NULL)
        (*tree)->height = (*tree)->left->height + 1;
    else if((*tree)->right != NULL)
        (*tree)->height = (*tree)->right->height + 1;
    else (*tree)->height = 0;
}
//---------------------------------------------------------------------------

//----------------------------INSERÇÃO DE DISCIPLINAS------------------------
void **insertInTheSubjectTree(CourseSubject **root, CourseSubject *node)
{
    if((*root) == NULL)
        (*root) = node;
    else if(node->id < (*root)->id)
        insertInTheSubjectTree(&(*root)->left, node);
    else insertInTheSubjectTree(&(*root)->right, node);

    balanceSubjectTree(root);
}

void insertSubject(CourseSubject **tree, int limitBlocks)
{
    CourseSubject *newSubject;

    newSubject = (CourseSubject *) malloc(sizeof(CourseSubject));
    newSubject->id = rand() % 1000;
    newSubject->block = 1 + rand() % limitBlocks;
    newSubject->workload = 60;
    newSubject->height = 0;
    strcpy(newSubject->name, "Estruturas");
    newSubject->left = NULL; newSubject->right = NULL;

    insertInTheSubjectTree(tree, newSubject);
}
//---------------------------------------------------------------------------

//------------------------------BUSCAS E IMPRENSÕES--------------------------
void printSubject(CourseSubject *node)
{
    printf("\nCode of Subject: %d", node->id);
    printf("\nBlock: %d", node->block);
    printf("\nWorkload: %d", node->workload);
    printf("\nName: %s\n", node->name);
}

void printSubjectsInOrder(CourseSubject *tree)
{
    if(tree != NULL)
    {
        printSubjectsInOrder(tree->left);
        printSubject(tree);
        printSubjectsInOrder(tree->right);
    }
}

void printSubjectById(CourseSubject *tree, int id)
{
    CourseSubject *nodeAddress;
    nodeAddress = searchInTheSubjectTree(tree, id);

    if(nodeAddress != NULL)
        printSubject(nodeAddress);
}

void printSubjectsFromABlock(CourseSubject *tree, int nBlock)
{
    if(tree != NULL)
    {
        printSubjectsFromABlock(tree->left, nBlock);
        if(tree->block == nBlock)
            printSubject(tree);
        printSubjectsFromABlock(tree->right, nBlock);
    }
}

void printSubjectsWithTheSameWorkload(CourseSubject *tree, int workload)
{
    if(tree != NULL)
    {
        printSubjectsWithTheSameWorkload(tree->left, workload);
        if(tree->workload == workload)
            printSubject(tree);
        printSubjectsWithTheSameWorkload(tree->right, workload);
    }
}
//---------------------------------------------------------------------------

//----------------------------REMOÇÃO DE DISCIPLINAS-------------------------

CourseSubject **searchBiggestNodeLeftOfTheSubjectTree(CourseSubject **tree)
{
    if((*tree)->right != NULL)
        searchBiggestNodeLeftOfTheSubjectTree(&(*tree)->right);
    else return tree;
}

void swapToRemoveSubject(CourseSubject **nodeToRemove, CourseSubject **replacementNode)
{
    CourseSubject *aux = (*replacementNode);

    if((*nodeToRemove)->left != (*replacementNode) && (*nodeToRemove)->left != NULL)
    {
        if((*replacementNode)->left == NULL)
        {
            (*replacementNode)->left = (*nodeToRemove)->left;
            (*replacementNode) = NULL;
        }
        else
        {
            (*replacementNode) = (*replacementNode)->left;
            aux->left = (*nodeToRemove)->left;
        }
    }

    if(aux != (*nodeToRemove)->right)
    {
        if(aux->right == NULL)
        {
            aux->right = (*nodeToRemove)->right;
        }
        else
        {
            aux = (*replacementNode)->right;
            aux->right = (*nodeToRemove)->right;
        }
    }

    free((*nodeToRemove));
    (*nodeToRemove) = aux;
}

void removeSubject(CourseSubject **tree, int idSubject)
{
    if((*tree) != NULL)
    {
        if((*tree)->id == idSubject)
            (*tree)->left ? swapToRemoveSubject(tree, searchBiggestNodeLeftOfTheSubjectTree(&(*tree)->left)) : swapToRemoveSubject(tree, &(*tree)->right);
        else if((*tree)->id > idSubject)
            removeSubject(&(*tree)->left, idSubject);
        else removeSubject(&(*tree)->right, idSubject);

        if((*tree) != NULL)
            balanceSubjectTree(tree);
    }
}
//---------------------------------------------------------------------------

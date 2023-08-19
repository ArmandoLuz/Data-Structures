#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "course_subjects.h"

struct courseSubject
{
    int id, block, workload;
    char name[30];
    CourseSubject *left, *right;
};

CourseSubject *createSubjectTree()
{
    return NULL;
}

//----------------------------BUSCA------------------------------------------
CourseSubject **searchInTheSubjectTree(CourseSubject **tree, int id)
{
    CourseSubject **addressFound;

    if((*tree) != NULL)
    {
        if((*tree)->id > id)
        {
            addressFound = searchInTheSubjectTree(&(*tree)->left, id);
        } else if ((*tree)->id < id)
        {
            addressFound = searchInTheSubjectTree(&(*tree)->right, id);
        } else addressFound = tree;
    } else addressFound = tree;

    return addressFound;
}
//---------------------------------------------------------------------------

//----------------------------INSERÇÃO DE DISCIPLINAS------------------------
void insertSubject(CourseSubject **tree, int limitBlocks)
{
    CourseSubject *newSubject, **insertNodeAddress;

    newSubject = (CourseSubject *) malloc(sizeof(CourseSubject));
    newSubject->id = rand() % 1000;
    newSubject->block = 1 + rand() % limitBlocks;
    newSubject->workload = 60;
    strcpy(newSubject->name, "Estruturas");
    newSubject->left = NULL; newSubject->right = NULL;
    insertNodeAddress = searchInTheSubjectTree(tree, newSubject->id);

    (*insertNodeAddress) = (*insertNodeAddress) ? (*insertNodeAddress) : newSubject;
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
    CourseSubject **nodeAddress;
    nodeAddress = searchInTheSubjectTree(&tree, id);

    if((*nodeAddress) != NULL)
    {
        printSubject((*nodeAddress));
    }
}

void printSubjectsFromABlock(CourseSubject *tree, int nBlock)
{
    if(tree != NULL)
    {
        printSubjectsFromABlock(tree->left, nBlock);
        if(tree->block == nBlock)
        {
            printSubject(tree);
        }
        printSubjectsFromABlock(tree->right, nBlock);
    }
}

void printSubjectsWithTheSameWorkload(CourseSubject *tree, int workload)
{
    if(tree != NULL)
    {
        printSubjectsWithTheSameWorkload(tree->left, workload);
        if(tree->workload == workload)
        {
            printSubject(tree);
        }
        printSubjectsWithTheSameWorkload(tree->right, workload);
    }
}
//---------------------------------------------------------------------------

//----------------------------REMOÇÃO DE DISCIPLINAS-------------------------
CourseSubject **searchBiggestNodeLeftOfTheSubjectTree(CourseSubject **tree)
{
    CourseSubject **addressFound;

    if((*tree)->right != NULL)
        addressFound = searchBiggestNodeLeftOfTheSubjectTree(&(*tree)->right);
    else addressFound = tree;

    return addressFound;
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
    CourseSubject **addressFound;
    addressFound = searchInTheSubjectTree(tree, idSubject);

    if((*addressFound) != NULL)
    {
        (*addressFound)->left ? swapToRemoveSubject(addressFound, searchBiggestNodeLeftOfTheSubjectTree(&(*addressFound)->left)) : swapToRemoveSubject(addressFound, &(*addressFound)->right); 
    }
}
//---------------------------------------------------------------------------
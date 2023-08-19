#include <stdio.h>
#include <stdlib.h>
#include "course.h"
#include "string.h"
#include "course_subjects.h"

struct course
{
    int id, nBlocks, nWeeks, height;
    char name[30];
    CourseSubject *subjects;
    Course *left, *right;
};

Course *createCourseTree()
{
    return NULL;
}

//----------------------------BUSCA------------------------------------------

Course *searchInTheCourseTree(Course *tree, int id)
{
    Course *addressFound;

    if(tree != NULL)
    {
        if(tree->id > id)
            addressFound = searchInTheCourseTree(tree->left, id);
        else if (tree->id < id)
            addressFound = searchInTheCourseTree(tree->right, id);
        else addressFound = tree;
    } else addressFound = tree;

    return addressFound;
}

//----------------------------------------------------------------------------

//-----------------------------BALANCEAMENTO----------------------------------

void balanceCourseTree(Course **tree)
{
    int balancingFactor;

    balancingFactor = calculeBalancingFactorOfCourseTree((*tree));

    if(balancingFactor == 2)
    {
        if(calculeBalancingFactorOfCourseTree((*tree)->left) < 0)
            leftRotationInTheCourseTree(&(*tree)->left);
        rightRotationInTheCourseTree(tree);
    } else if(balancingFactor == -2)
    {
        if(calculeBalancingFactorOfCourseTree((*tree)->right) > 0)
            rightRotationInTheCourseTree(&(*tree)->right);
        leftRotationInTheCourseTree(tree);
    }

    updateHeightOfCourseTree(tree);
}

void leftRotationInTheCourseTree(Course **tree)
{
    Course *aux;

    aux = (*tree)->right;
    (*tree)->right = (*tree)->right->left;
    aux->left = (*tree);
    (*tree) = aux;


    updateHeightOfCourseTree(tree);
    updateHeightOfCourseTree(&aux->left);
}
void rightRotationInTheCourseTree(Course **tree)
{
    Course *aux;

    aux = (*tree)->left;
    (*tree)->left = (*tree)->left->right;
    aux->right = (*tree);
    (*tree) = aux;

    updateHeightOfCourseTree(tree);
    updateHeightOfCourseTree(&aux->right);
}

int calculeBalancingFactorOfCourseTree(Course *tree)
{
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

void updateHeightOfCourseTree(Course **tree)
{
    if((*tree)->left != NULL && (*tree)->right != NULL)
        (*tree)->height = (*tree)->left->height > (*tree)->right->height ? (*tree)->left->height + 1 : (*tree)->right->height + 1;
    else if((*tree)->left != NULL)
        (*tree)->height = (*tree)->left->height + 1;
    else if((*tree)->right != NULL)
        (*tree)->height = (*tree)->right->height + 1;
    else (*tree)->height = 0;
}

//----------------------------------------------------------------------------


//----------------------------INSERÇÃO DE CURSO-------------------------------

void insertInTheCourseTree(Course **root, Course *node)
{
    if((*root) == NULL)
        (*root) = node;
    else if(node->id < (*root)->id)
        insertInTheCourseTree(&(*root)->left, node);
    else insertInTheCourseTree(&(*root)->right, node);

    balanceCourseTree(root);
}

void insertCourse(Course **tree, int id)
{
    Course *newCourse;

    newCourse = (Course *) malloc(sizeof(Course));
    newCourse->id = id;
    newCourse->nBlocks = 1 + rand() % 7;
    newCourse->nWeeks = 15;
    newCourse->height = 0;
    strcpy(newCourse->name, "Sistemas");
    newCourse->subjects = createSubjectTree();
    newCourse->left = NULL; newCourse->right = NULL;

    insertInTheCourseTree(tree, newCourse);
}

//---------------------------------------------------------------------------

//------------------------INSERÇÃO DE DISCIPLINAS----------------------------

void insertCourseSubject(Course *tree, int idCourse)
{
    Course *addressFound;
    addressFound = searchInTheCourseTree(tree, idCourse);

    if(addressFound != NULL)
        insertSubject(&addressFound->subjects, addressFound->nBlocks);
}

//---------------------------------------------------------------------------

//------------------------------BUSCAS E IMPRENSÕES--------------------------

void printCourse(Course *node)
{
    printf("\nCode of course: %d", node->id);
    //printf("\nNº of blocks: %d", node->nBlocks);
    //printf("\nName of course: %s", node->name);
    //printf("\nNumber of weeks: %d\n", node->nWeeks);
}

void printCourseInOrder(Course *tree)
{
    if(tree != NULL)
    {
        printCourseInOrder(tree->left);
        printCourse(tree);
        printCourseInOrder(tree->right);
    }
}

void printCourseById(Course *tree, int id)
{
    Course *nodeAddress;
    nodeAddress = searchInTheCourseTree(tree, id);

    if(nodeAddress != NULL)
        printCourse(nodeAddress);
}

void printCoursesWithTheSameAmountOfBlocks(Course *tree, int nBlocks)
{
    if(tree != NULL)
    {
        printCoursesWithTheSameAmountOfBlocks(tree->left, nBlocks);
        if(tree->nBlocks == nBlocks)
            printCourse(tree);
        printCoursesWithTheSameAmountOfBlocks(tree->right, nBlocks);
    }
}

void printCourseSubject(Course *tree, int idCourse, int idSubject)
{
    Course *addressFound;
    addressFound = searchInTheCourseTree(tree, idCourse);
    
    if(addressFound != NULL)
        printSubjectById(addressFound->subjects, idSubject);
}

void printCourseSubjectsInOrder(Course *tree, int idCourse)
{
    Course *addressFound;
    addressFound = searchInTheCourseTree(tree, idCourse);

    if(addressFound != NULL)
        printSubjectsInOrder(addressFound->subjects);
}
void printSubjectsFromABlockOfACourse(Course *tree, int idCourse, int nBlock)
{
    Course *addressFound;
    addressFound = searchInTheCourseTree(tree, idCourse);

    if(addressFound != NULL)
        printSubjectsFromABlock(addressFound->subjects, nBlock);
}

void printSubjectsOfACourseWithTheSameWorkload(Course *tree, int idCourse, int workload)
{
    Course *addressFound;
    addressFound = searchInTheCourseTree(tree, idCourse);

    if(addressFound != NULL)
        printSubjectsWithTheSameWorkload(addressFound->subjects, workload);
}

//-------------------------------------------------------------------------------------

//----------------------------REMOÇÃO DE CURSOS----------------------------------------

Course **searchBiggestNodeLeftOfTheCourseTree(Course **tree)
{
    if((*tree)->right != NULL)
        searchBiggestNodeLeftOfTheCourseTree(&(*tree)->right);
    else return tree;
}

void swapToRemoveCourse(Course **nodeToRemove, Course **replacementNode)
{
    Course *aux = (*replacementNode);

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

void removeCourse(Course **tree, int idCourse)
{
    if((*tree) != NULL)
    {
        if((*tree)->id == idCourse)
        {
            if((*tree)->subjects == NULL)
                (*tree)->left ? swapToRemoveCourse(tree, searchBiggestNodeLeftOfTheCourseTree(&(*tree)->left)) : swapToRemoveCourse(tree, &(*tree)->right);
        } else if((*tree)->id > idCourse)
            removeCourse(&(*tree)->left, idCourse);
        else removeCourse(&(*tree)->right, idCourse);

        if((*tree) != NULL)
            balanceCourseTree(tree);
    }  
}

//---------------------------------------------------------------------------

//-----------------------------REMOÇÃO DE DISCIPLINA-------------------------

void removeSubjectFromACourse(Course *tree, int idCourse, int idSubject)
{
    Course *addressFound;
    addressFound = searchInTheCourseTree(tree, idCourse);

    if(addressFound != NULL)
        removeSubject(&addressFound->subjects, idSubject);
}

//----------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "course.h"
#include "course_subjects.h"

struct course
{
    int id, nBlocks, nWeeks;
    char name[30];
    CourseSubject *subjects;
    Course *left, *right;
};

Course *createCourseTree()
{
    return NULL;
}

//----------------------------BUSCA------------------------------------------
Course **searchInTheCourseTree(Course **tree, int id)
{
    Course **addressFound;

    if((*tree) != NULL)
    {
        if((*tree)->id > id)
        {
            addressFound = searchInTheCourseTree(&(*tree)->left, id);
        } else if ((*tree)->id < id)
        {
            addressFound = searchInTheCourseTree(&(*tree)->right, id);
        } else 
        {
            addressFound = tree;
        }
    } else 
    {
        addressFound = tree;
    }

    return addressFound;
}
//---------------------------------------------------------------------------

//----------------------------INSERÇÃO DE CURSO------------------------------
void insertCourse(Course **tree, int id)
{
    Course *newCourse, **insertNodeAddress;

    newCourse = (Course *) malloc(sizeof(Course));
    newCourse->id = id;
    newCourse->nBlocks = 1 + rand() % 7;
    newCourse->nWeeks = 15;
    strcpy(newCourse->name, "Sistemas");
    newCourse->left = NULL; newCourse->right = NULL;
    insertNodeAddress = searchInTheCourseTree(tree, newCourse->id);

    (*insertNodeAddress) = (*insertNodeAddress) ? (*insertNodeAddress) : newCourse;
}
//---------------------------------------------------------------------------

//------------------------INSERÇÃO DE DISCIPLINAS----------------------------
void insertCourseSubject(Course *tree, int idCourse)
{
    Course **addressFound;
    addressFound = searchInTheCourseTree(&tree, idCourse);

    if((*addressFound) != NULL)
    {
        insertSubject(&(*addressFound)->subjects, (*addressFound)->nBlocks);
    }
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
    Course **nodeAddress;
    nodeAddress = searchInTheCourseTree(&tree, id);

    if((*nodeAddress) != NULL)
    {
        printCourse((*nodeAddress));
    }
}

void printCoursesWithTheSameAmountOfBlocks(Course *tree, int nBlocks)
{
    if(tree != NULL)
    {
        printCoursesWithTheSameAmountOfBlocks(tree->left, nBlocks);
        if(tree->nBlocks == nBlocks)
        {
            printCourse(tree);
        }
        printCoursesWithTheSameAmountOfBlocks(tree->right, nBlocks);
    }
}

void printCourseSubject(Course *tree, int idCourse, int idSubject)
{
    Course **addressFound;
    addressFound = searchInTheCourseTree(&tree, idCourse);
    
    if((*addressFound) != NULL)
    {
        printSubjectById((*addressFound)->subjects, idSubject);
    }
}

void printCourseSubjectsInOrder(Course *tree, int idCourse)
{
    Course **addressFound;
    addressFound = searchInTheCourseTree(&tree, idCourse);

    if((*addressFound) != NULL)
    {
        printSubjectsInOrder((*addressFound)->subjects);
    }
}
void printSubjectsFromABlockOfACourse(Course *tree, int idCourse, int nBlock)
{
    Course **addressFound;
    addressFound = searchInTheCourseTree(&tree, idCourse);

    if((*addressFound) != NULL)
    {
        printSubjectsFromABlock((*addressFound)->subjects, nBlock);
    }
}

void printSubjectsOfACourseWithTheSameWorkload(Course *tree, int idCourse, int workload)
{
    Course **addressFound;
    addressFound = searchInTheCourseTree(&tree, idCourse);

    if((*addressFound) != NULL)
    {
        printSubjectsWithTheSameWorkload((*addressFound)->subjects, workload);
    }
}
//---------------------------------------------------------------------------

//----------------------------REMOÇÃO DE CURSOS------------------------------
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
    Course **addressFound;
    addressFound = searchInTheCourseTree(tree, idCourse);

    if((*addressFound) != NULL)
    {
        if((*addressFound)->subjects == NULL)
        {
            (*addressFound)->left ? swapToRemoveCourse(addressFound, searchBiggestNodeLeftOfTheCourseTree(&(*addressFound)->left)) : swapToRemoveCourse(addressFound, &(*addressFound)->right);
        }
    }  
}

//---------------------------------------------------------------------------

//-----------------------------REMOÇÃO DE DISCIPLINA-------------------------
void removeSubjectFromACourse(Course *tree, int idCourse, int idSubject)
{
    Course **addressFound;
    addressFound = searchInTheCourseTree(&tree, idCourse);

    if((*addressFound) != NULL)
    {
        removeSubject(&(*addressFound)->subjects, idSubject);
    }
}
//---------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "course.h"
#include "course_subjects.h"

#define SIZE 50000

void generateValues(int *valuesVector, int size);
void shuffle(int *valuesVector, int size);

int main()
{
    srand(42);

    Course *tree, *adressFound;
    int valuesVector[SIZE];
    clock_t initTimeForInsertion, endTimeForInsertion, initTimeForSearch, endTimeForSearch;
    double sumInsertion = 0, sumSearch = 0;

    generateValues(valuesVector, SIZE);
    shuffle(valuesVector, SIZE);

    for(int j = 0; j < 30; j++)
    {
        tree = createCourseTree();

        initTimeForInsertion = clock();
        for(int i = 0; i < SIZE; i++)
        {
            insertCourse(&tree, valuesVector[i]);
        }
        endTimeForInsertion = clock();

        sumInsertion += ((double)(endTimeForInsertion - initTimeForInsertion)) * 1000 / CLOCKS_PER_SEC;
    }

    shuffle(valuesVector, SIZE);

    for(int i = 0; i < 30; i++)
    {
        initTimeForSearch = clock();
        for(int k = 0; k < 10; k++)
        {
            adressFound = searchInTheCourseTree(tree, valuesVector[k]);
        }
        endTimeForSearch = clock();

        sumSearch += ((double)(endTimeForSearch - initTimeForSearch)) * 1000000000.0 / CLOCKS_PER_SEC;
    }

    printf("\nMedia de tempo para inserção: %.5f miliseconds", (double)sumInsertion/30);
    printf("\nMedia de tempo para busca: %.5f nanoseconds", (double)sumSearch/30);

    return 0;
}

void generateValues(int *valuesVector, int size)
{
    for(int i = 0; i < size; i++)
        valuesVector[i] = i;
}

void shuffle(int *valuesVector, int size)
{
    for(int i = 0; i < size; i++)
    {
        int temp = valuesVector[i], indexToSwap = rand() % size;
        valuesVector[i] = valuesVector[indexToSwap];
        valuesVector[indexToSwap] = temp;
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define NVERTICES 81
#define LENVERTICES 10

void menu();
void search(int edges[NVERTICES][NVERTICES], char configurations[NVERTICES][LENVERTICES], int algorithm);
void readCsv(char vertices[NVERTICES][LENVERTICES], int edges[NVERTICES][NVERTICES]);
int findConfigIndex(char config[LENVERTICES], char configurations[NVERTICES][LENVERTICES]);
int minDistance(int distances[NVERTICES], int visited[NVERTICES]);
void dijkstra(int graph[NVERTICES][NVERTICES], char start[LENVERTICES], char end[LENVERTICES], char configurations[NVERTICES][LENVERTICES]);
void bellmanFord(int graph[NVERTICES][NVERTICES], char start[LENVERTICES], char end[LENVERTICES], char configurations[NVERTICES][LENVERTICES]);

int main()
{
    char vertices[NVERTICES][LENVERTICES];
    int edges[NVERTICES][NVERTICES], option;
    
    readCsv(vertices, edges); // Leitura do grafo e seus vertices.

    do
    {
        menu();
        printf("\n-> ");
        scanf("%d", &option);
        switch(option)
        {
            case 1:
                search(edges, vertices, 1);
                break;

            case 2:
                search(edges, vertices, 2);
                break;

            case 3:
                break;
            
            default:
                printf("Insira uma opcao valida!");
                break;
        }
    } while (option != 3);

    return 0;
}

void menu()
{
    printf("\n-------------------------------------------------------Torre de Hanoi-------------------------------------------------------");
    printf("\nAs jogadas sao descritas seguindo este padrao: '[A,A,A,A]'");
    printf("\nEste padrao indica que os 4 discos estao no poste A, sendo que a esquerda ficam os maiores discos e a direita os menores.");
    printf("\n\n1. Pesquisa do menor caminho com Dijkistra.");
    printf("\n2. Pesquisa do menor caminho com Bellman.");
    printf("\n3. Sair.");

}

void search(int edges[NVERTICES][NVERTICES], char configurations[NVERTICES][LENVERTICES], int algorithm)
{
    char start[LENVERTICES], end[LENVERTICES];
    printf("\nInsira um ponto de partida no seguinte formato: [A,A,A,A]\n-> ");
    scanf("%s", start);
    printf("\nInsira um ponto de destino no seguinte formato: [A,A,A,A]\n-> ");
    scanf("%s", end);

    if(algorithm == 1)
        dijkstra(edges, start, end, configurations);
    else bellmanFord(edges, start, end, configurations);
}

// Carrega o grafo e os vértices na memoria a partir de um csv.
void readCsv(char vertices[NVERTICES][LENVERTICES], int edges[NVERTICES][NVERTICES])
{
    FILE *dataFile;
    int value;
    dataFile = fopen("vertices.txt", "r");

    if(dataFile)
    {
        for(int verticeIndex = 0; verticeIndex < NVERTICES; verticeIndex++)
            fscanf(dataFile, "%s", vertices[verticeIndex]);

        fclose(dataFile);
    }

    dataFile = fopen("matrix.txt", "r");

    if(dataFile)
    {
        for(int line = 0; line < NVERTICES; line++)
        {
            for(int column = 0; column < NVERTICES; column++)
            {
                fscanf(dataFile, "%d,", &edges[line][column]);
            }
        }

        fclose(dataFile);
    }

}

// Função para encontrar o índice da configuração em um nó
int findConfigIndex(char config[LENVERTICES], char configurations[NVERTICES][LENVERTICES]) 
{
    int indexFound = -1;

    for(int configIndex = 0; configIndex < NVERTICES && indexFound == -1; configIndex++) 
        if(strcmp(config, configurations[configIndex]) == 0)
            indexFound = configIndex;

    return indexFound;
}

// Função para encontrar o nó não visitado com menor distância
int minDistance(int distances[NVERTICES], int visited[NVERTICES]) 
{
    int min = INT_MAX;
    int minIndex = -1;

    for(int i = 0; i < NVERTICES; i++) {
        if(!visited[i] && distances[i] <= min) 
        {
            min = distances[i];
            minIndex = i;
        }
    }

    return minIndex;
}

// Algoritmo de Dijkstra
void dijkstra(int graph[NVERTICES][NVERTICES], char start[LENVERTICES], char end[LENVERTICES], char configurations[NVERTICES][LENVERTICES]) {
    int startIndex = findConfigIndex(start, configurations);
    int endIndex = findConfigIndex(end, configurations);
    int distances[NVERTICES];
    int visited[NVERTICES];
    int predecessors[NVERTICES];
    clock_t initTime, endTime;

    initTime = clock();
    for(int i = 0; i < NVERTICES; i++) {
        distances[i] = INT_MAX;
        visited[i] = 0;
        predecessors[i] = -1; // Inicializa os predecessores como -1
    }

    distances[startIndex] = 0;

    for(int count = 0; count < NVERTICES - 1; count++) {
        int i = minDistance(distances, visited);
        visited[i] = 1;

        for (int j = 0; j < NVERTICES; j++) {
            if (!visited[j] && graph[i][j] && distances[i] != INT_MAX && distances[i] + graph[i][j] < distances[j]) {
                distances[j] = distances[i] + graph[i][j];
                predecessors[j] = i; // Armazena o nó predecesssor
            }
        }
    }
    endTime = clock();

    printf("\nTempo de busca: %.2f microsegundos", (double)(endTime - initTime) * 1000000 / CLOCKS_PER_SEC);
    printf("\nMenor número de movimentos de %s até %s: %d\n", start, end, distances[endIndex]);

    // Imprimir o caminho percorrido
    printf("Caminho percorrido: ");
    int current = endIndex;
    while (current != startIndex) {
        printf("%s <- ", configurations[current]);
        current = predecessors[current];
    }
    printf("%s\n", configurations[startIndex]);
}

// Algoritmo de bellmanFord
void bellmanFord(int graph[NVERTICES][NVERTICES], char start[LENVERTICES], char end[LENVERTICES], char configurations[NVERTICES][LENVERTICES]) {
    int startIndex = findConfigIndex(start, configurations);
    int endIndex = findConfigIndex(end, configurations);
    int distances[NVERTICES];
    int predecessors[NVERTICES];
    clock_t initTime, endTime;

    initTime = clock();
    for(int i = 0; i < NVERTICES; i++) {
        distances[i] = INT_MAX;
        predecessors[i] = -1;
    }
    distances[startIndex] = 0;

    for(int k = 0; k < NVERTICES - 1; k++) {
        for(int i = 0; i < NVERTICES; i++) {
            for(int j = 0; j < NVERTICES; j++) {
                if(graph[i][j] == 1 && distances[i] != INT_MAX && distances[i] + 1 < distances[j]) {
                    distances[j] = distances[i] + graph[i][j];
                    predecessors[j] = i;
                }
            }
        }
    }
    endTime = clock();

    printf("\nTempo de busca: %.2f microsegundos", (double)(endTime - initTime) * 1000000 / CLOCKS_PER_SEC);
    printf("\nMenor número de movimentos de %s até %s: %d\n", configurations[startIndex], configurations[endIndex], distances[endIndex]);

    // Imprimir o caminho percorrido
    printf("Caminho percorrido: %s", configurations[endIndex]);
    int current = endIndex;
    while (predecessors[current] != -1) {
        printf(" <- %s", configurations[predecessors[current]]);
        current = predecessors[current];
    }
    printf("\n");
}



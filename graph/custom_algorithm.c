#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>

#define NVERTEX 6 // Número de vértices no grafo

/*
    É NECESSÁRIO COMPILAR USANDO O COMANDO -lm.
*/

typedef struct {
    int vertex;
    double probability; // A probabilidade associada à aresta
} Edge;

typedef struct {
    Edge edges[NVERTEX]; // Lista de arestas
    int numEdges;
} Graph[NVERTEX]; // Grafo geral.

double logMaxProb(double probability);
int minDistance(double distances[NVERTEX], bool visited[NVERTEX]);
void mostReliablePath(Graph graph, int start, int end);
void menu();
void search(Graph graph);

int main() {
    Graph graph;
    int option;

    // Preenchendo o grafo com as arestas e probabilidades
    graph[0].edges[0].vertex = 1;
    graph[0].edges[0].probability = 0.8;
    graph[0].edges[1].vertex = 2;
    graph[0].edges[1].probability = 0.7;
    graph[0].numEdges = 2;

    graph[1].edges[0].vertex = 2;
    graph[1].edges[0].probability = 0.9;
    graph[1].edges[1].vertex = 4;
    graph[1].edges[1].probability = 0.5;
    graph[1].numEdges = 2;

    graph[2].edges[0].vertex = 3;
    graph[2].edges[0].probability = 0.3;
    graph[2].numEdges = 1;

    graph[3].edges[0].vertex = 4;
    graph[3].edges[0].probability = 0.6;
    graph[3].edges[1].vertex = 5;
    graph[3].edges[1].probability = 0.95;
    graph[3].numEdges = 2;

    graph[4].numEdges = 0;

    graph[5].edges[0].vertex = 4;
    graph[5].edges[0].probability = 0.67;
    graph[5].numEdges = 1;

    do
    {
        menu();
        printf("\n-> ");
        scanf("%d", &option);
        switch(option)
        {
            case 1:
                search(graph);
                break;

            case 2:
                break;
            
            default:
                printf("Insira uma opcao valida!");
                break;
        }
    } while (option != 2);

    return 0;
}

void menu()
{
    printf("\n1. Procurar menor caminho.");
    printf("\n2. Sair.");
}

void search(Graph graph)
{
    int start, end;

    printf("Digite o vértice de origem: ");
    scanf("%d", &start);

    printf("Digite o vértice de destino: ");
    scanf("%d", &end);

    mostReliablePath(graph, start, end);
}

// Calcula o logaritmo negativo da probabilidade máxima.
double logMaxProb(double probability) 
{
    return probability == 0.0 ? -INFINITY : -log(probability);
} 

// Encontra o índice do vértice não visitado com a menor distância.
int minDistance(double distances[NVERTEX], bool visited[NVERTEX]) 
{
    double min = INFINITY;
    int minIndex = -1;

    for (int i = 0; i < NVERTEX; i++) {
        if (!visited[i] && distances[i] < min) {
            min = distances[i];
            minIndex = i;
        }
    }

    return minIndex;
}

// Procura o caminho mais confiável entre dois vértices.
void mostReliablePath(Graph graph, int start, int end) 
{
    double distances[NVERTEX];
    bool visited[NVERTEX];
    int predecessors[NVERTEX];

    for(int i = 0; i < NVERTEX; i++) 
    {
        distances[i] = INFINITY;
        visited[i] = false;
        predecessors[i] = -1;
    }

    distances[start] = 0.0;

    for(int count = 0; count < NVERTEX - 1; count++) 
    {
        int u = minDistance(distances, visited);
        visited[u] = true;

        if(u != -1)
        {
            for(int i = 0; i < graph[u].numEdges; i++) 
            {
                int v = graph[u].edges[i].vertex;
                double probability = graph[u].edges[i].probability;
                double negLogProb = logMaxProb(probability);

                if(!visited[v] && distances[u] != INFINITY && distances[u] + negLogProb < distances[v]) 
                {
                    distances[v] = distances[u] + negLogProb;
                    predecessors[v] = u;
                }
            }
        }
    }

    printf("Caminho mais confiável de %d até %d com probabilidade %.4lf\n", start, end, exp(-distances[end]));

    // Imprimir o caminho percorrido
    printf("Caminho percorrido: %d", end);
    int current = end;

    while(predecessors[current] != -1) 
    {
        printf(" <- %d", predecessors[current]);
        current = predecessors[current];
    }
    printf("\n");
}



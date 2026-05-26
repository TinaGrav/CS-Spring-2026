#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void prim(int vertices, int edges, int incidence[][edges], int result[][edges]) {
    int key[vertices];
    int in_tree[vertices];
    int parent[vertices];
    for (int i = 0; i <= vertices; i++) {
        for (int e = 0; e < edges; e++) {
            result[i][e] = 0;
        }
    }
    for (int i = 0; i < vertices; i++) {
        key[i] = INT_MAX;
        in_tree[i] = 0;
        parent[i] = -1;
    }
    key[0] = 0;
    
    for (int count = 0; count < vertices; count++) {
        int min_key = INT_MAX;
        int u = -1;
        for (int v = 0; v < vertices; v++) {
            if (!in_tree[v] && key[v] < min_key) {
                min_key = key[v];
                u = v;
            }
        }
        if (u == -1) break;
        in_tree[u] = 1;
        for (int e = 0; e < edges; e++) {
            if (incidence[u][e] == 1) {
                int v = -1;
                for (int i = 0; i < vertices; i++) {
                    if (i != u && incidence[i][e] == 1) {
                        v = i;
                        break;
                    }
                }
                int weight = incidence[vertices][e];
                if (v != -1 && !in_tree[v] && weight < key[v]) {
                    key[v] = weight;
                    parent[v] = u;
                    result[u][e] = 1;
                    result[v][e] = 1;
                    result[vertices][e] = weight;
                }
            }
        }
    }
}

void print_matrix(int vertices, int edges, int incidence[][edges]) {
    printf("     ");
    for (int e = 0; e < edges; e++) {
        printf("E%-3d", e);
    }
    printf("\n");
    for (int i = 0; i < vertices; i++) {
        printf("V%-3d ", i);
        for (int e = 0; e < edges; e++) {
            printf("%-4d", incidence[i][e]);
        }
        printf("\n");
    }
    printf("W    ");
    for (int e = 0; e < edges; e++) {
        printf("%-4d", incidence[vertices][e]);
    }
    printf("\n");
}

int main() {
    FILE *file = fopen("test.txt", "r");
    if (!file) {
        printf("Mistake");
        return 0;
    }
    
    int vertices, edges;
    fscanf(file, "%d %d", &vertices, &edges);
    int incidence[vertices + 1][edges];
    int result[vertices + 1][edges];
    for (int i = 0; i <= vertices; i++) {
        for (int j = 0; j < edges; j++) {
            fscanf(file, "%d", &incidence[i][j]);
        }
    }
    fclose(file);
    printf("\nOriginal matrix\n");
    print_matrix(vertices, edges, incidence);
    prim(vertices, edges, incidence, result);
    printf("\nMinimum tree\n");
    print_matrix(vertices, edges, result);
    
    return 0;
}
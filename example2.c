/* parcurgere graf cu DFS/BFS */

// Imi cer scuze in avans

#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    int data;
    struct Node *next;
} NODE;

typedef struct Graph {
    int vertices;
    int *visited;
    NODE **adjacency_lists;
} GPH;

/// utils

NODE *create_node(int v) {
    NODE *new_node = malloc(sizeof(NODE));
    if (!new_node) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    new_node->data = v;
    new_node->next = NULL;
    return new_node;
}

GPH *create_graph(int vertices) {
    int i;
    GPH *graph = malloc(sizeof(GPH));
    if (!graph) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    graph->vertices = vertices;
    graph->adjacency_lists = malloc(vertices * sizeof(NODE *));
    graph->visited = malloc(sizeof(int) * vertices);
    if (!graph->adjacency_lists || !graph->visited) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < vertices; i++) {
        graph->adjacency_lists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

void add_edge(GPH *graph, int src, int dest) {
    // Adaugă muchia de la src la dest (presupunem indicii 0-indexați)
    NODE *new_node = create_node(dest);
    new_node->next = graph->adjacency_lists[src];
    graph->adjacency_lists[src] = new_node;
    // Deoarece graful este nedreptat, adăugăm și muchia inversă
    new_node = create_node(src);
    new_node->next = graph->adjacency_lists[dest];
    graph->adjacency_lists[dest] = new_node;
}

void insedg(int nr_of_vertices, int nr_of_edges, GPH *graph) {
    int src, dest, i;
    printf("Adauga %d muchii (nodurile de la 1 la %d):\n", nr_of_edges, nr_of_vertices);
    for (i = 0; i < nr_of_edges; i++) {
        // Citim muchia (nodurile sunt 1-indexate în input)
        scanf("%d%d", &src, &dest);
        // Convertim la 0-indexat și adăugăm muchia
        add_edge(graph, src - 1, dest - 1);
    }
}

/// bfs utils

int is_empty(NODE *queue) {
    return (queue == NULL);
}

void enqueue(NODE **queue, int data) {
    NODE *new_node = create_node(data);
    if (is_empty(*queue))
        *queue = new_node;
    else {
        NODE *temp = *queue;
        while (temp->next)
            temp = temp->next;
        temp->next = new_node;
    }
}

int dequeue(NODE **queue) {
    int data = (*queue)->data;
    NODE *temp = *queue;
    *queue = (*queue)->next;
    free(temp);
    return data;
}

void print_graph(GPH *graph) {
    int i;
    for (i = 0; i < graph->vertices; i++) {
        NODE *temp = graph->adjacency_lists[i];
        while (temp) {
            // Afișăm nodurile ca 1-indexate
            printf("%d ", temp->data + 1);
            temp = temp->next;
        }
        printf("\n");
    }
}

void print_queue(NODE *queue) {
    while (queue != NULL) {
        printf("%d ", queue->data + 1);
        queue = queue->next;
    }
}

void wipe_visited_list(GPH *graph, int nr_of_vertices) {
    int i;
    for (i = 0; i < nr_of_vertices; i++) {
        graph->visited[i] = 0;
    }
}

// parcurgeri

void DFS(GPH *graph, int vertex_nr) {
    NODE *adj_list = graph->adjacency_lists[vertex_nr];
    NODE *temp = adj_list;
    graph->visited[vertex_nr] = 1;
    // Afișăm nodul ca 1-indexat
    printf("%d->", vertex_nr + 1);
    while (temp != NULL) {
        int connected_vertex = temp->data;
        if (graph->visited[connected_vertex] == 0) {
            DFS(graph, connected_vertex);
        }
        temp = temp->next;
    }
}

void BFS(GPH *graph, int start) {
    NODE *queue = NULL;
    graph->visited[start] = 1;
    enqueue(&queue, start);
    while (!is_empty(queue)) {
        int current = dequeue(&queue);
        // Afișăm nodul ca 1-indexat
        printf("%d ", current + 1);
        NODE *temp = graph->adjacency_lists[current];
        while (temp) {
            int adj_vertex = temp->data;
            if (graph->visited[adj_vertex] == 0) {
                graph->visited[adj_vertex] = 1;
                enqueue(&queue, adj_vertex);
            }
            temp = temp->next;
        }
    }
}

int main() {
    int nr_of_vertices;
    int nr_of_edges;
    int starting_vertex;

    printf("Cate noduri are graful? ");
    scanf("%d", &nr_of_vertices);

    printf("Cate muchii are graful? ");
    scanf("%d", &nr_of_edges);

    GPH *graph = create_graph(nr_of_vertices);
    insedg(nr_of_vertices, nr_of_edges, graph);

    printf("De unde plecam in DFS? ");
    scanf("%d", &starting_vertex);
    // Convertim de la 1-indexat la 0-indexat
    DFS(graph, starting_vertex - 1);

    wipe_visited_list(graph, nr_of_vertices);

    printf("\nDe unde plecam in BFS? ");
    scanf("%d", &starting_vertex);
    BFS(graph, starting_vertex - 1);

}

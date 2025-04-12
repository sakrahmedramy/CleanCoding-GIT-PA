#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    int data;
    struct Node* next;
} NODE;

typedef struct g {
    int v;
    int* vis;
    NODE** alst;
} GPH;

typedef struct s {
    int t;
    int scap;
    int* arr;
} STK;

NODE* create_node(int v) {
    NODE* nn = malloc(sizeof(NODE));
    nn->data = v;
    nn->next = NULL;
    return nn;
}

GPH* create_g(int v) {
    int i;
    GPH* g = malloc(sizeof(GPH));
    g->v = v;
    g->alst = malloc(v * sizeof(NODE*));
    g->vis = malloc(v * sizeof(int));
    for (i = 0; i < v; i++) {
        g->alst[i] = NULL;
        g->vis[i] = 0;
    }
    return g;
}

STK* create_s(int scap) {
    STK* s = malloc(sizeof(STK));
    s->arr = malloc(scap * sizeof(int));
    s->t = -1;
    s->scap = scap;
    return s;
}

void push(int pshd, STK* s) {
    s->t = s->t + 1;
    s->arr[s->t] = pshd;
}

void add_edge(GPH* g, int src, int dest) {
    NODE* nn = create_node(dest);
    nn->next = g->alst[src];
    g->alst[src] = nn;
    nn = create_node(src);
    nn->next = g->alst[dest];
    g->alst[dest] = nn;
}

void insert_edges(GPH* g, int edg_nr) {
    int src, dest, i;
    printf("Adauga %d muchii (nodurile de la 1 la %d):\n", edg_nr, g->v);
    for (i = 0; i < edg_nr; i++) {
        scanf("%d%d", &src, &dest);
        add_edge(g, src - 1, dest - 1);
    }
}

void wipe(GPH* g) {
    for (int i = 0; i < g->v; i++) {
        g->vis[i] = 0;
    }
}

void DFS(GPH* g, STK* s, int v_nr) {
    NODE* aux = g->alst[v_nr];
    g->vis[v_nr] = 1;
    printf("%d ", v_nr + 1);
    push(v_nr, s);
    while (aux != NULL) {
        int connected_vertex = aux->data;
        if (g->vis[connected_vertex] == 0)
            DFS(g, s, connected_vertex);
        aux = aux->next;
    }
}

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

void print_graph(GPH *g) {
    for (int i = 0; i < g->v; i++) {
        NODE *temp = g->alst[i];
        while (temp) {
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

void canbe(GPH* g, STK* s1, STK* s2) {
    int ans = 0;
    for (int i = 0; i <= s1->t; i++) {
        for (int j = 0; j <= s2->t; j++) {
            if (s1->arr[i] == s2->arr[j]) {
                ans = 1;
                break;
            }
        }
        if (ans) break;
    }
    if (ans)
        printf("\nDrum existent intre restaurante.\n");
    else
        printf("\nDrum inexistent intre restaurante.\n");
}

int main() {
    int nrv, edg_nr, starting_vertex;
    printf("Cate noduri are graful? ");
    scanf("%d", &nrv);
    printf("Cate muchii are graful? ");
    scanf("%d", &edg_nr);
    GPH* g = create_g(nrv);
    insert_edges(g, edg_nr);
    STK* s1 = create_s(2 * nrv);
    STK* s2 = create_s(2 * nrv);
    printf("De unde plecam in DFS? ");
    scanf("%d", &starting_vertex);
    DFS(g, s1, starting_vertex - 1);
    wipe(g);
    printf("\nDe unde plecam in DFS pentru al doilea drum? ");
    scanf("%d", &starting_vertex);
    DFS(g, s2, starting_vertex - 1);
    canbe(g, s1, s2);
    return 0;
}

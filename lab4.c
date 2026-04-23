#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 4           // максимум ключей в узле 2 * T
#define MIN 2           // минимум ключей в узле T
#define KEYLEN 7        // 6 символов + \0

struct Node {
    char keys[MAX][KEYLEN];    
    double values[MAX];        
    struct Node* child[MAX+1];  
    int n;                      //кол-в ключей
    int leaf;                   // 1 если лист
};

struct Node* root = NULL;

struct Node* create_node(int leaf) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->n = 0;
    node->leaf = leaf;
    for (int i = 0; i <= MAX; i++){ 
        node->child[i] = NULL;
    }
    return node;
}


void split_child(struct Node* parent, int i, struct Node* child) {
    struct Node* newnode = create_node(child->leaf); 
    newnode->n = MAX - MIN;
    for (int j = 0; j < newnode->n; j++) {  // копируем правые ключи 
        strcpy(newnode->keys[j], child->keys[j+MIN]);
        newnode->values[j] = child->values[j+MIN];
    }
    if (child->leaf != 1)  // если узел не лист копируем указатели детей
        for (int j = 0; j <= newnode->n; j++){
            newnode->child[j] = child->child[j+MIN];
        }
    child->n = MIN - 1;
    for (int j = parent->n; j >= i+1; j--)  //оставляем место для указателя на реьенка
        parent->child[j+1] = parent->child[j];
    parent->child[i+1] = newnode;
    for (int j = parent->n-1; j >= i; j--) {  // оставляем место для нового ключа
        strcpy(parent->keys[j+1], parent->keys[j]);
        parent->values[j+1] = parent->values[j];
    }
    strcpy(parent->keys[i], child->keys[MIN-1]);  //перемещаем средний ключ в родителя
    parent->values[i] = child->values[MIN-1];
    parent->n++;
}


void add_not_full(struct Node* node, char* key, double value) { //вставка в неполный узел
    int i = node->n - 1;
    if (node->leaf) { //если узел это лист, вставляем его на нужную позицию
        while (i >= 0 && strcmp(key, node->keys[i]) < 0) {
            strcpy(node->keys[i+1], node->keys[i]);
            node->values[i+1] = node->values[i];
            i--;
        }
        if (i >= 0 && strcmp(key, node->keys[i]) == 0) {
            node->values[i] = value;
            return;
        }
        strcpy(node->keys[i+1], key);
        node->values[i+1] = value;
        node->n++;
    } else { // иначе ищем дочерний для встаки
        while (i >= 0 && strcmp(key, node->keys[i]) < 0) i--;
        i++;
        if (i > 0 && strcmp(key, node->keys[i-1]) == 0) {
            node->values[i-1] = value;
            return;
        }
        if (node->child[i]->n == MAX) {
            split_child(node, i, node->child[i]);
            if (strcmp(key, node->keys[i]) > 0) i++;
        }
        add_not_full(node->child[i], key, value);
    }
}

void insert(char* key, double value) { //функция вставки
    if (root == NULL) {
        root = create_node(1);
        strcpy(root->keys[0], key);
        root->values[0] = value;
        root->n = 1;
        return;
    }
    if (root->n == MAX) { //если корень заполнен, создаем новый
        struct Node* newroot = create_node(0);
        newroot->child[0] = root;
        split_child(newroot, 0, root);
        int i = 0;
        if (strcmp(key, newroot->keys[0]) > 0) i++;
        add_not_full(newroot->child[i], key, value);
        root = newroot;
    } else {
        add_not_full(root, key, value);
    }
}


double* search(struct Node* node, char* key) { // поиск значения
    if (node == NULL) return NULL;
    int i = 0;
    while (i < node->n && strcmp(key, node->keys[i]) > 0) i++;
    if (i < node->n && strcmp(key, node->keys[i]) == 0){
        return &node->values[i];
    }
    if (node->leaf){
        return NULL;
    }
    return search(node->child[i], key);
}

int find_key(struct Node* node, char* key) { // поиск индекса ключа
    int i = 0;
    while (i < node->n && strcmp(key, node->keys[i]) > 0) i++;
    return i;
}

char* getPred(struct Node* node, int i) { // функция для получения предшественника
    struct Node* cur = node->child[i];
    while (!cur->leaf) cur = cur->child[cur->n];
    return cur->keys[cur->n-1];
}

char* getSucc(struct Node* node, int i) { // функция для получения преемника
    struct Node* cur = node->child[i+1];
    while (!cur->leaf) cur = cur->child[0];
    return cur->keys[0];
}

void removeLeaf(struct Node* node, int i) { // удаление ключа из листа
    for (int j = i+1; j < node->n; j++) {
        strcpy(node->keys[j-1], node->keys[j]);
        node->values[j-1] = node->values[j];
    }
    node->n--;
}

void borrowLeft(struct Node* node, int i) { // взять ключ у левого соседа
    struct Node* child = node->child[i];
    struct Node* sibling = node->child[i-1];
    for (int j = child->n-1; j >= 0; j--) { // сдвигаем ключи вправо
        strcpy(child->keys[j+1], child->keys[j]);
        child->values[j+1] = child->values[j];
    }
    if (!child->leaf) { //если у узла есть потомки, сдвигаем и их указатели
        for (int j = child->n; j >= 0; j--)
            child->child[j+1] = child->child[j];
        child->child[0] = sibling->child[sibling->n];
    }
    strcpy(child->keys[0], node->keys[i-1]);
    child->values[0] = node->values[i-1];
    strcpy(node->keys[i-1], sibling->keys[sibling->n-1]);
    node->values[i-1] = sibling->values[sibling->n-1];
    child->n++;
    sibling->n--;
}

void borrowRight(struct Node* node, int i) { // взять ключ у правого соседа
    struct Node* child = node->child[i];
    struct Node* sibling = node->child[i+1];
    strcpy(child->keys[child->n], node->keys[i]);
    child->values[child->n] = node->values[i];
    if (!child->leaf)
        child->child[child->n+1] = sibling->child[0];
    strcpy(node->keys[i], sibling->keys[0]);
    node->values[i] = sibling->values[0];
    for (int j = 1; j < sibling->n; j++) {  // сдвигаем ключи соседа влево
        strcpy(sibling->keys[j-1], sibling->keys[j]);
        sibling->values[j-1] = sibling->values[j];
    }
    if (!sibling->leaf){ // сдвигаем дочерние указатели соседа влево
        for (int j = 1; j <= sibling->n; j++)
            sibling->child[j-1] = sibling->child[j];
    }
    child->n++;
    sibling->n--;
}


void mergeNodes(struct Node* node, int i) { //слияние узлов
    struct Node* child = node->child[i];
    struct Node* sibling = node->child[i+1];
    strcpy(child->keys[child->n], node->keys[i]);
    child->values[child->n] = node->values[i];
    for (int j = 0; j < sibling->n; j++) {  // копируем ключи правого узла в левый
        strcpy(child->keys[j+child->n+1], sibling->keys[j]);
        child->values[j+child->n+1] = sibling->values[j];
    }
    if (child->leaf != 1){ //копируем дочерние указатели правого узла в левый
        for (int j = 0; j <= sibling->n; j++)
            child->child[j+child->n+1] = sibling->child[j];
    }
    for (int j = i+1; j < node->n; j++) { // Сдвигаем ключи родителя влево
        strcpy(node->keys[j-1], node->keys[j]);
        node->values[j-1] = node->values[j];
    }
    for (int j = i+2; j <= node->n; j++){  //Сдвигаем дочерние указатели родителя влево
        node->child[j-1] = node->child[j];
    }
    child->n += sibling->n + 1;
    node->n--;
    free(sibling);
}


void fill(struct Node* node, int i) {  //заполнение узла при нехватке ключей
    if (i > 0 && node->child[i-1]->n >= MIN){
        borrowLeft(node, i);
    }
    else if (i < node->n && node->child[i+1]->n >= MIN){
        borrowRight(node, i);
    }
    else {
        if (i < node->n){
            mergeNodes(node, i);
        }else{
            mergeNodes(node, i-1);
        }
    }
}


int deleteFromNode(struct Node* node, char* key) { //удаление из узла
    int i = find_key(node, key);
    if (i < node->n && strcmp(key, node->keys[i]) == 0) {
        if (node->leaf){
            removeLeaf(node, i);  //удаление если элемент это лист
        }
        else {
            if (node->child[i]->n >= MIN) { //заменяем удаляемый ключ его родителем, если у левого ребенка достаточно ключей
                char* pred = getPred(node, i);
                char tempKey[KEYLEN];
                strcpy(tempKey, pred);
                double tempVal;
                struct Node* predNode = node->child[i];
                while (!predNode->leaf) predNode = predNode->child[predNode->n];
                tempVal = predNode->values[predNode->n-1];
                strcpy(node->keys[i], tempKey);
                node->values[i] = tempVal;
                deleteFromNode(node->child[i], tempKey);
            }
            else if (node->child[i+1]->n >= MIN) { //заменяем удаляемый ключ его ребенком, если у правого ребенка достаточно ключей
                char* succ = getSucc(node, i);
                char tempKey[KEYLEN];
                strcpy(tempKey, succ);
                double tempVal;
                struct Node* succNode = node->child[i+1];
                while (!succNode->leaf) succNode = succNode->child[0];
                tempVal = succNode->values[0];
                strcpy(node->keys[i], tempKey);
                node->values[i] = tempVal;
                deleteFromNode(node->child[i+1], tempKey);
            }
            else { // сливаем ключи, если оба ребенка имеют минимум ключей
                mergeNodes(node, i);
                deleteFromNode(node->child[i], key);
            }
        }
        return 1;
    }
    if (node->leaf) {
        return 0;
    } 
    int flag = (i == node->n);
    if (node->child[i]->n < MIN) {
        fill(node, i); // заполненяем узел если не хватает ключей
        i = find_key(node, key);
    }
    return deleteFromNode(node->child[i], key);
}

int delete(char* key) { //функция удаления
    if (root == NULL) return 0;
    int result = deleteFromNode(root, key);
    if (root != NULL && root->n == 0) {
        struct Node* tmp = root;
        if (root->leaf) root = NULL;
        else root = root->child[0];
        free(tmp);
    }
    return result;
}

void printTree(struct Node* node, int level, FILE* out) {
    if (node == NULL) return;
     for (int i = 0; i < level; i++) {
        fprintf(out, "│   "); 
    }
    if (level > 0) {
        fprintf(out, "├── ");
    }
    fprintf(out, "[");
    for (int i = 0; i < node->n; i++) {
        fprintf(out, "%s:%.2f", node->keys[i], node->values[i]);
        if (i < node->n - 1) {
            fprintf(out, " | ");
        }
    }
    fprintf(out, "]\n");
    if (!node->leaf) {
        for (int i = 0; i <= node->n; i++) {
            printTree(node->child[i], level + 1, out);
        }
    }
}

int main() {
    FILE* in = fopen("input_file.txt", "r");
    FILE* out = fopen("output_file.txt", "w");
    if (in == NULL || out == NULL) {
        printf("Ошибка\n");
        return 1;
    }
    char line[256];
    while (fgets(line, sizeof(line), in)) {
        fprintf(out, "%s", line);
        int op;
        char key[KEYLEN];
        double val;
        char* token = strtok(line, " \t\n");
        if (token == NULL) continue;
        op = atoi(token);
        switch(op) {
            case 1: // добавление узла
                token = strtok(NULL, " \t\n");
                if (token) {
                    strcpy(key, token);
                    token = strtok(NULL, " \t\n");
                    if (token) {
                        val = atof(token);
                        insert(key, val);
                        fprintf(out, "Added %s = %.2f\n", key, val);
                    }
                }
                break;
                
            case 2: // удаление узла
                token = strtok(NULL, " \t\n");
                if (token) {
                    strcpy(key, token);
                    if (delete(key))
                        fprintf(out, "%s deleted\n", key);
                    else
                        fprintf(out, "ERROR: key %s not found\n", key);
                }
                break;
                
            case 3: // печать дерева
                fprintf(out, "Printing tree:\n");
                if (root == NULL)
                    fprintf(out, "Tree is empty\n");
                else
                    printTree(root, 0, out);
                break;
                
            case 4: // поиск
                token = strtok(NULL, " \t\n");
                if (token) {
                    strcpy(key, token);
                    double* res = search(root, key);
                    if (res)
                        fprintf(out, "FOUND: %s = %.2f\n", key, *res);
                    else
                        fprintf(out, "NOT FOUND: %s\n", key);
                }
                break;
                
            default:
                fprintf(out, "ERROR: unknown operation\n");
        }
        fprintf(out, "\n");
    }
    fclose(in);
    fclose(out);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node{
    int type;  
    char operation;
    int number;
    char var;
    struct Node* left; 
    struct Node* right; 
} Node;

typedef struct {
    char *str;
    int size;
} PostfixString;

typedef struct {
    char *items;
    int top;
    int size;
} CharStack;

typedef struct {
    Node **items;
    int top;
    int size;
} NodeStack;

void add_char(PostfixString *pf, char c) {
    pf->str = (char*)realloc(pf->str, (pf->size + 1) * sizeof(char));
    pf->str[pf->size++] = c;
}

void push_char(CharStack *s, char c) {
    s->items = (char*)realloc(s->items, (s->size + 1) * sizeof(char));
    s->items[++s->top] = c;
    s->size++;
}

char pop_char(CharStack *s) {
    if (s->top >= 0) {
        return s->items[s->top--];
    }
    return '\0';
}

char peek_char(CharStack *s) {
    if (s->top >= 0) {
        return s->items[s->top];
    }
    return '\0';
}

int priority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}


void make_postfix(const char *infix, PostfixString *pf) {
    CharStack opStack;
    opStack.items = NULL;
    opStack.top = -1;
    opStack.size = 0;
    
    int i = 0;
    int len = strlen(infix);
    
    while (i < len) {
        char c = infix[i];
        if (c == ' ') {
            i++;
            continue;
        }
        if (isdigit(c)) {
            while (i < len && isdigit(infix[i])) {
                add_char(pf, infix[i]);
                i++;
            }
            add_char(pf, ' ');
            continue;
        }
        if (isalpha(c)) {
            add_char(pf, c);
            add_char(pf, ' ');
            i++;
            continue;
        }
        if (c == '(') {
            push_char(&opStack, c);
            i++;
            continue;
        }
        if (c == ')') {
            while (opStack.top != -1 && peek_char(&opStack) != '(') {
                add_char(pf, pop_char(&opStack));
                add_char(pf, ' ');
            }
            pop_char(&opStack); 
            i++;
            continue;
        }
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            while (opStack.top != -1 && peek_char(&opStack) != '(' && (priority(peek_char(&opStack)) > priority(c) || (priority(peek_char(&opStack)) == priority(c) && c != '^'))){
                add_char(pf, pop_char(&opStack));
                add_char(pf, ' ');
            }
            push_char(&opStack, c);
            i++;
            continue;
        }
        i++;
    }
    
    while (opStack.top != -1) {
        add_char(pf, pop_char(&opStack));
        add_char(pf, ' ');
    }
    
    free(opStack.items);
    opStack.items = NULL;
    opStack.top = -1;
    opStack.size = 0;
}

void push_node(NodeStack *s, Node* node) {
    s->items = (Node**)realloc(s->items, (s->size + 1) * sizeof(Node*));
    s->items[++s->top] = node;
    s->size++;
}

Node* pop_node(NodeStack *s) {
    if (s->top >= 0) {
        return s->items[s->top--];
    }
    return NULL;
}

Node* make_number(int val) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = 0;
    node->number = val;
    node->operation = '0';
    node->var = '0';  
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* make_var(char var) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = 1;
    node->var = var;
    node->operation = '0'; 
    node->number = 0;  
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* make_oper(char op, Node* left, Node* right) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = 2;
    node->operation = op;
    node->number = 0;
    node->var = '0'; 
    node->left = left;
    node->right = right;
    return node;
}

Node* make_tree(const char* postfix) {
    NodeStack nodeStack;
    nodeStack.items = NULL;
    nodeStack.top = -1;
    nodeStack.size = 0;
    
    int i = 0;
    while (postfix[i] != '\0') {
        char c = postfix[i];
        if (c == ' ') {
            i++;
            continue;
        }
        if (isdigit(c)) {
            int num = 0;
            while (isdigit(postfix[i])) {
                num = num * 10 + (postfix[i] - '0');
                i++;
            }
            push_node(&nodeStack, make_number(num));
            continue;
        }
        if (isalpha(c)) {
            push_node(&nodeStack, make_var(c));
            i++;
            continue;
        }
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            Node* right = pop_node(&nodeStack);
            Node* left = pop_node(&nodeStack);
            push_node(&nodeStack, make_oper(c, left, right));
            i++;
            continue;
        }
        i++;
    }
    Node* result = pop_node(&nodeStack);
    free(nodeStack.items);
    nodeStack.items = NULL;
    nodeStack.top = -1;
    nodeStack.size = 0;
    return result;
}

void print_tree(Node* element, int level) {
    if (element == NULL) return;
    print_tree(element->right, level + 1);
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    if (element->type == 2) {
        printf("%c\n", element->operation);
    } else if (element->type == 1) {
        printf("%c\n", element->var);
    } else {
        printf("%d\n", element->number);
    }
    print_tree(element->left, level + 1);
}

void free_tree(Node* node) {
    if (node == NULL) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

int check_change(Node* node) {
    if (node == NULL){
        return 0;
    }
    if (node->left == NULL || node->right == NULL){
    return 0;
    }
    if (node->operation == '*'){
        if (node->left->type == 1 && node->right->type == 0){
            return 1;
        }else if(node->left->type == 0 && node->right->type == 1){
            return 1;
        }else{
            return 0;
        }
    }else{
        return 0;
    }
}

Node* create_sum(Node* node) {
    int n;
    char varName;
    if (node->right->type == 0){
        n = node->right->number;  
        varName = node->left->var; 
    }else{
        n = node->left->number;  
        varName = node->right->var; 
    }
    Node* sumTree = make_var(varName);
    for (int i = 1; i < n; i++) {
        sumTree = make_oper('+', sumTree, make_var(varName));
    }
    return sumTree;
}

Node* simplifyTree(Node* node) {
    if (node == NULL) return NULL;
    node->left = simplifyTree(node->left);
    node->right = simplifyTree(node->right);
    if (check_change(node)) {
        Node* newNode = create_sum(node);
        free(node->left);   
        free(node->right); 
        free(node);     
        return newNode;
    }  
    return node;
}

int main() {
    FILE* input_file = fopen("input_file.txt", "r");
    if (input_file == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    
    char expression[100];
    int i = 0;
    char c;
    while (fscanf(input_file, "%c", &c) == 1 && i < 99) {
        expression[i++] = c;
    }
    expression[i] = '\0';
    fclose(input_file);
    
    printf("Expression in file: %s\n", expression);
    
    PostfixString postfix;
    postfix.str = NULL;
    postfix.size = 0;
    
    make_postfix(expression, &postfix);
    
    printf("Postfix: ");
    for (int i = 0; i < postfix.size; i++) {
        printf("%c", postfix.str[i]);
    }
    printf("\n");
    
    printf("\nTree before changing:\n");
    Node* root = make_tree(postfix.str);
    print_tree(root, 0);
    
    root = simplifyTree(root);
    printf("\nTree after changing:\n");
    print_tree(root, 0);
    
    free(postfix.str);
    postfix.str = NULL;
    postfix.size = 0;
    free_tree(root);
    
    return 0;
}
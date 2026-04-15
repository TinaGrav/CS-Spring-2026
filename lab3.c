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
}Node;

char *postfix = NULL;  
int postfix_size = 0;  
char *stack = NULL;
int stackTop = -1;
int stackSize = 0;

Node **st = NULL;
int stTop = -1;
int stSize = 0;

void addChar(char c) {
    postfix = (char*)realloc(postfix, (postfix_size + 1) * sizeof(char));
    postfix[postfix_size++] = c;
}

void push(char c) {
    stack = (char*)realloc(stack, (stackSize + 1) * sizeof(char));
    stack[++stackTop] = c;
    stackSize++;
}

char pop() {
    if (stackTop >= 0) {
        return stack[stackTop--];
    }
    return '\0';
}

char peek() {
    if (stackTop >= 0) {
        return stack[stackTop];
    }
    return '\0';
}

int isEmpty() {
    return stackTop == -1;
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

void infixToPostfix(const char *infix) {
    free(postfix);
    free(stack);
    postfix = NULL;
    stack = NULL;
    postfix_size = 0;
    stackTop = -1;
    stackSize = 0;
    
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
                addChar(infix[i]);
                i++;
            }
            addChar(' ');
            continue;
        }
        if (isalpha(c)) {
            addChar(c);
            addChar(' ');
            i++;
            continue;
        }
        if (c == '(') {
            push(c);
            i++;
            continue;
        }
        if (c == ')') {
            while (!isEmpty() && peek() != '(') {
                addChar(pop());
                addChar(' ');
            }
            pop(); 
            i++;
            continue;
        }
        if (c=='+' || c=='-' || c=='*' || c=='/' || c=='^') {
            while (!isEmpty() && peek() != '(' &&
                   (precedence(peek()) > precedence(c) ||
                    (precedence(peek()) == precedence(c) && c != '^'))) {
                addChar(pop());
                addChar(' ');
            }
            push(c);
            i++;
            continue;
        }
        i++;
    }
    while (!isEmpty()) {
        addChar(pop());
        addChar(' ');
    }
}

void pushNode(Node* node) {
    st = (Node**)realloc(st, (stSize + 1) * sizeof(Node*));
    st[++stTop] = node;
    stSize++;
}

Node* popNode() {
    if (stTop >= 0) {
        return st[stTop--];
    }
    return NULL;
}

Node* make_number(int val) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = 0;
    node->number = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* make_var(char var) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = 1;
    node->var = var;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* make_oper(char op, Node* left, Node* right) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = 2;
    node->operation = op;
    node->left = left;
    node->right = right;
    return node;
}

Node* make_tree(char* postfix) {
    free(st);
    st = NULL;
    stTop = -1;
    stSize = 0;
    
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
            pushNode(make_number(num));
            continue;
        }
        if (isalpha(c)) {
            pushNode(make_var(c));
            i++;
            continue;
        }
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            Node* right = popNode();
            Node* left = popNode();
            pushNode(make_oper(c, left, right));
            i++;
            continue;
        }
        i++;
    }
    return popNode();
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
    printf("%s\n", expression);
    infixToPostfix(expression);
    printf("Postfix: ");
    for (int i = 0; i < postfix_size; i++) {
        printf("%c", postfix[i]);
    }
    printf("\n");
    Node* root = make_tree(postfix);
    print_tree(root, 0);
    
    free(postfix);
    free(stack);
    free(st);
    free_tree(root);
    
    return 0;
}
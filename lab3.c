#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node{
    char operation;
    int number;
    char var;
    struct Node* left; 
    struct Node* right; 
}Node;

typedef struct Tree{
    struct Node* head;
}Tree;



char *postfix = NULL;  
int postfix_size = 0;  
char *stack = NULL;
int stackTop = -1;
int stackSize = 0;

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
    return '0';
}

char peek() {
    if (stackTop >= 0) {
        return stack[stackTop];
    }
    return '0';
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}


void infixToPostfix(const char *infix) {
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
            while ((stackTop != -1) && peek() != '(') {
                addChar(pop());
                addChar(' ');
            }
            pop(); 
            i++;
            continue;
        }
        if ((c=='+') || (c=='-') || (c=='*') || (c=='/') || (c=='^')) {
            while ((stackTop != -1) && peek() != '(' &&
                   (precedence(peek()) > precedence(c) ||
                    (precedence(peek()) == precedence(c) && (c != '^')))) {
                addChar(pop());
                addChar(' ');
            }
            push(c);
            i++;
            continue;
        }
        i++;
    }
    while ((stackTop != -1)) {
        addChar(pop());
        addChar(' ');
    }
}

int main() {
    free(postfix);
    free(stack);
    return 0;
}
typedef struct Node{
    char operation;
    int number;
    char var;
    struct Node* left; 
    struct Node* right; 
}Operator;

typedef struct Tree{
    struct Node* head;
}Tree;



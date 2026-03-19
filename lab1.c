#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct tree_element{
    int data;
    struct tree_element* next_left;
    struct tree_element* next_right;
}tree_element;

typedef struct tree{
    tree_element* head;
}tree;

int add(int n, tree* Tree){
    tree_element* new_el = (tree_element*)malloc(sizeof(tree_element));
    if (new_el == NULL){
        return 0;
    }
    new_el->data = n;
    new_el->next_right = NULL;
    new_el->next_left = NULL;
    if (Tree->head != NULL){
        tree_element* cur = Tree->head;
        while(true){
            if (new_el->data > cur->data){
                if (cur->next_right != NULL){
                    cur = cur->next_right;
                }else{
                    cur->next_right = new_el;
                    break;
                }
            }else{
                if (cur->next_left != NULL){
                    cur = cur->next_left;
                }else{
                    cur->next_left = new_el;
                    break;
                }
            }

        }
    }else{
        Tree->head = new_el;
    }
    return 0;
}


int delete(int n, tree* Tree){
    tree_element* current = Tree->head;
    tree_element* prev = NULL;
    while(true){
        if (current->data == n){ 

            if (current->next_left == NULL){
                if (current->next_right == NULL){
                    if (prev==NULL){
                        Tree->head = NULL;
                    }else if(prev->data > current->data){
                        prev->next_left = NULL;
                    }else{
                        prev->next_right = NULL;
                    }
                    free(current);
                    return 0;
                }else{
                    if (prev==NULL){
                        Tree->head = current->next_right;
                    }else if(prev->data > current->data){
                        prev->next_left = current->next_right;
                    }else{
                        prev->next_right = current->next_right;
                    }
                    free(current);
                    return 0;
                }

            }else if (current->next_right == NULL){
                if (prev==NULL){
                        Tree->head = current->next_left;
                }else if(prev->data > current->data){
                        prev->next_left = current->next_left;
                }else{
                        prev->next_right = current->next_left;
                }
                free(current);
                return 0;
            }else{
                tree_element* max_left = current->next_left;
                tree_element* max_parent = current;
                while (max_left->next_right != NULL) {
                    max_parent = max_left;
                    max_left = max_left->next_right;
                }
                current->data = max_left->data;
                if (max_parent == current) {
                    max_parent->next_left = max_left->next_left;
                }else{
                    max_parent->next_right = max_left->next_left;
                }
                free(max_left);
                return 0;
            }
        }else{
            if ((current->next_left == NULL) && (current->next_right == NULL)){
                printf("No such element found\n");
                return 0;
            }else{
                prev = current;
                if (current->data > n){
                    if (current->next_left == NULL){
                        printf("No such element found\n");
                        return 0;
                    }
                    current = current->next_left;
                }else{
                    if (current->next_right == NULL){
                        printf("No such element found\n");
                        return 0;
                    }
                    current = current->next_right;
                }
            }
        }
    }
    return 0;
}

int check_values(tree* Tree){
    int a = 0;
    int b = 0;
    if (Tree->head == NULL){
        printf("Tree is empty");
        return 0;
    }
    printf("Enter values: ");
    scanf("%d %d", &a, &b);
    tree_element* min_leaf;
    tree_element* max_leaf;
    tree_element* current = Tree->head;
    while(true){
        if (current->next_left != NULL){
            current = current->next_left;
        }else if (current->next_right != NULL){
            current = current->next_right;
        }else{
            break;
        }
    }
    int min = current->data;
    current = Tree->head;
    while(true){
        if (current->next_right != NULL){
            current = current->next_right;
        }else if (current->next_left != NULL){
            current = current->next_left;
        }else{
            break;
        }
    }
    int max = current->data;
    if ((max <= b) && (min >= a)){
        printf("True");
    }else{
        printf("False");
    }
}


void print_tree(tree_element* element, int level) {
    if (element == NULL) return;
    
    print_tree(element->next_right, level + 1);
    
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("%d\n", element->data);
    
    print_tree(element->next_left, level + 1);
}

int main(void)
{
    tree test_tree = {NULL};
    add(5, &test_tree);  
    add(9, &test_tree);
    add(7, &test_tree);
    add(6, &test_tree);
    add(3, &test_tree);  
    add(4, &test_tree);
    add(1, &test_tree);
    delete(9, &test_tree);
    print_tree(test_tree.head, 0);
    return 0;
}
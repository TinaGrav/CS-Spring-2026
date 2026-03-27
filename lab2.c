#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct el{
    int data;
    float key;
    struct el *next;
    struct el *parent;
}el;


void shell_sort(arr* Arr, size){


}

void add_element(el* Arr, float key, int data){
    el* new_el = (el*)malloc(sizeof(el));
    new_el->next = NULL;
    new_el->key = key;
    new_el->data = data;
    Arr = new_el;
}

int main(void)
{
    el arr[15];
    FILE* input_file = fopen("input_file.txt", "r");
    if (input_file == NULL) {
        printf("Error");
        return 0;
    }

    for (int i = 0; i < 15; i++){ 
        int data;
        float key;
        fscanf(input_file, "%f %d", &key, &data);
        add_element(&arr[i], key, data);
    }
    for (int i = 0; i < 15, i++){
        printf(arr[i]);
    }
    fclose(input_file);
    return 0;
}
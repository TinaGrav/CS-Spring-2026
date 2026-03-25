#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
struct el{
    int data;
    int key;
    struct el next;
    stract el parent;
}

struct arr{
    el* head;
}

void shell_sort(arr* Arr, size){

}

int main(void)
{
    arr = {NULL};
    FILE* input_file = fopen("input_file.txt", "r");
    if (input_file == NULL) {
        printf("Error");
        return 0;
    }

    for (int i = 0, 15, i++){ 
        el* new_el = (el*)malloc(sizeof(el));
        fscanf(input_file, "%d", &new_el.data);
        new_el.key = i;
    }
    return 0;
}
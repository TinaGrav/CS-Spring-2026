#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct el{
    int data;
    float key;
    struct el *next;
    struct el *parent;
}el;


void shell_sort(el* arr, int size) {
    for (int step = size / 2; step > 0; step /= 2) {
        for (int i = step; i < size; ++i) {
            el temp = arr[i];
            int j;
            for (j = i; j >= step && arr[j - step].key > temp.key; j -= step) {
                arr[j] = arr[j - step];
            }
            arr[j] = temp;
        }
    }
}

int binary_search(float num, el* arr, int size){
    int left = 0;
    int right = size - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid].key == num) {
            return  printf("Element found: %f %d\n", arr[mid].key, arr[mid].data);
        }
        else if (arr[mid].key < num) {
            left = mid + 1; 
        }
        else {
            right = mid - 1;
        }
    }
    printf("Element not found");
}

void add_element(el* new_el, float key, int data){
    new_el->next = NULL;
    new_el->parent = NULL;
    new_el->key = key;
    new_el->data = data;
}

void print_arr(el* arr){
    for (int i = 0; i < 15; i++){
        printf("%f %d\n", arr[i].key, arr[i].data);
    }
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
        if (fscanf(input_file, "%f %d", &key, &data)!=2) {
            printf("Error");
            break;
        }
        add_element(&arr[i], key, data);
    }

    printf("You can test programm. what do you want to do:\n");
    printf("(1) - sort already sorted file, (2) - sort reversed file, (3) - sort file with random values, (4) - test binary search\n");
    printf("Write in number of action: ");
    int act;
    scanf("%d", &act);
    
    switch (act)
    {
    case 1:
        printf("\nArray from input_file1.txt:\n");
        print_arr(arr);
        printf("\nSorted array:\n");
        shell_sort(arr, 15);
        print_arr(arr);
        break;
    case 2:
        printf("\nArray from input_file2.txt:\n");
        print_arr(arr);
        printf("\nSorted array:\n");
        shell_sort(arr, 15);
        print_arr(arr);
        break;
    case 3:
        printf("\nArray from input_file2.txt:\n");
        print_arr(arr);
        printf("\nSorted array:\n");
        shell_sort(arr, 15);
        print_arr(arr);
        break;
    case 4:
        float num;
        scanf("%f", &num);
        binary_search(num, arr, 15);
    default:
        break;
    }
    fclose(input_file);
    return 0;
}
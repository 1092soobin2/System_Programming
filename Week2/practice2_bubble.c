//Template
#include <stdio.h>
#include <stdlib.h>
#define SIZE 10

int compare(int a, int b){
    if(a>b)  
    	return 1;
    else 
       	return -1;
}
    

/***************************************************************
define bubble sort that uses pointer to 'compare' function above
***************************************************************/
void bubble_sort(int array[]) {
    for (int i = 0; i < SIZE - 1; i++)
        for (int j = 0; j < SIZE - 1 - i; j++)
            if (compare(array[j], array[j + 1]) == 1) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }

}

int main(){
    int n_array[SIZE] = {2,-9, 10, 15, 1, 3, -12, 5, 4, 1};

    //source code
    bubble_sort(n_array);

    printf("{");
    for (int i = 0; i < SIZE; i++)
        printf("%d ", n_array[i]);
    printf("}\n");

    return 0;
}

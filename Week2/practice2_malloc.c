#include <stdio.h>
#include <stdlib.h>

int main(void){
	char** pp_data = NULL;
	int n;
	int length;
	int i;

	scanf("%d", &n);


	//1. define & dynamic allocation
	pp_data = (char**)malloc(sizeof(char*) * n);

	printf("length: ");
	scanf(" %d", &length);
	for (i = 0; i < n; i++)
	    pp_data[i] = (char*)malloc(sizeof(char) * length);

	//2. print string
	for (i = 0; i < n; i++) {
	    printf("Enter string. (length: maximum %d): ", length);
	    scanf("%s", pp_data[i]);
	}
	for (i = 0; i < n; i++)
	    printf("%s\n", pp_data[i]);

	//3. free memory
	for (i = 0; i < n; i++)
	    free(pp_data[i]);
	free(pp_data);

	return 0;
}

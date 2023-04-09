//Template
#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int nData;
    struct Node *next;
}Node;

typedef struct Stack{
    Node *top;
}Stack;

void InitializeStack(Stack *stack);
void Push(Stack *stack, int nData);
int Pop(Stack *stack);

void MakeEmpty(Stack *stack);
void PrintStack(Stack *stack);

int main(void){
    //source code
    Stack stack;
    printf("Initialize Stack\n");
    InitializeStack(&stack);
    PrintStack(&stack);

    printf("push: '1'\n");
    Push(&stack, 1);
    PrintStack(&stack);
    printf("push: '2'\n");
    Push(&stack, 2);
    PrintStack(&stack);
    printf("push: '4'\n");
    Push(&stack, 4);
    PrintStack(&stack);
    printf("push: '2'\n");
    Push(&stack, 2);
    PrintStack(&stack);
    printf("push: '8'\n");
    Push(&stack, 8);
    PrintStack(&stack);

    printf("pop\n");
    Pop(&stack);
    PrintStack(&stack);
    printf("pop\n");
    Pop(&stack);
    PrintStack(&stack);
    printf("pop\n");
    Pop(&stack);
    PrintStack(&stack);

    printf("Make empty");
    MakeEmpty(&stack);
    PrintStack(&stack);
    return 0;
}

void InitializeStack(Stack *stack){
    //source code
    stack->top = (Node*)malloc(sizeof(Node));
    stack->top->next = NULL;
}

void Push(Stack *stack, int nData){
    //source code
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->nData = nData;
    newNode->next = stack->top->next;
    stack->top->next = newNode;
}

int Pop(Stack *stack){
    //source code
    Node* poppedNode = stack->top->next;
    int backupData = poppedNode->nData;
    stack->top->next = poppedNode->next;
    free(poppedNode);
    return backupData;
}

void MakeEmpty(Stack *stack) {
    while (stack->top->next != NULL)
        Pop(stack);
}

void PrintStack(Stack *stack) {
    Node *pos = stack->top->next;
    printf("stack\n|   |\n");
    while (pos != NULL) {
        printf("| %d |\n", pos->nData);
        pos = pos->next;
    }
    printf("|___|\n\n");
}
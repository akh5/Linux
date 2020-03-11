#include <stdio.h>
#include <stdlib.h>
typedef struct node
{
  int data;
  struct node* next;
}Node;



  void create(Node* head)
  {
    head= (Node*)malloc(sizeof(Node));
    head = NULL;
  }

  void push_back(Node* head,int d)
  {
    Node* pcur = head;
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = d;
    temp->next = NULL;
    while(pcur->next)
    {
      pcur = pcur->next;
    }
    pcur->next = temp;
  }

  void printlist(Node* head)
  {
    Node* pcur = head;
    while(pcur->next!=NULL)
    {
      printf("%d-->",pcur->data);
    }
    printf("\n");
  }

int main()
{
    Node* list;
    create(list);
    push_back(list,1);

    push_back(list,2);
    push_back(list,3);
    push_back(list,4);
    push_back(list,5);
    printlist(list);
  return 0;
}

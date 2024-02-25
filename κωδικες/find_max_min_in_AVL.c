#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define size 1000


struct Data
{
char direction[10];
int year;
char date[20];
char weekday[20];
char country[20];
char commodity[20];
char transport[20];
char measure[20];
int value;
int cumulative;
} data;

struct Node {
  struct Data key;
  struct Node *left;
  struct Node *right;
  int height;
};

int max(int a, int b);

int findmin(struct Node *root);
void printmin(struct Node *root, int min);

int findmax(struct Node *root);
void printmax(struct Node *root, int max);


int height(struct Node *N) { // ��������� ��� ��� ���������� ��� ����� ��� ������.
  if (N == NULL)
    return 0;
  return N->height;
}

int max(int a, int b) {
  return (a > b) ? a : b;
}


struct Node *newNode(struct Data key) { // ���������� ������.
  struct Node *node = (struct Node *)malloc(sizeof(struct Node)); //�������� ������ ��� ��� �����.
  node->key = key; //������� ������. ��������! �� ������ ����� ����� struct data.
  node->left = NULL;    //������������ ��������� ������
  node->right = NULL;   //������������ ������ ������
  node->height = 1;//������������ �����.
  return (node);
}


struct Node *rightRotate(struct Node *y) { // ����� ����������.
  struct Node *x = y->left; //� ������� � ������� ��� �������� ��������� ��� Y.
  struct Node *T2 = x->right; //� �2 ������� ��� ���� ��������� ��� �.

  x->right = y; //������ ������ ��� � �� �.
  y->left = T2; //��������� ������ ��� � �� ��������� �2.

  y->height = max(height(y->left), height(y->right)) + 1; //��������� ��� ����.
  x->height = max(height(x->left), height(x->right)) + 1;

  return x;
}


struct Node *leftRotate(struct Node *x) {   // �������� ����������. ���� ������ �� ��� �����.
  struct Node *y = x->right;
  struct Node *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = max(height(x->left), height(x->right)) + 1;
  y->height = max(height(y->left), height(y->right)) + 1;

  return y;
}


int getBalance(struct Node *N) { // ����������� ��� ������ ���������� ��� ���� ������.
  if (N == NULL)
    return 0;
  return height(N->left) - height(N->right);
}

struct Node *insertNode(struct Node *node, struct Data key) {  // �������� ������.

  if (node == NULL)
    return (newNode(key));
// ����������� ��� ���������� ����� ��� �� �������� � ������.
  if (key.value <= node->key.value)
    node->left = insertNode(node->left, key);
  else if (key.value > node->key.value)
    node->right = insertNode(node->right, key);
  else
    return node;

 // ��������� ��� ������ ���������� ��� ���� ������ ��� ����������� ��� �������.
  node->height = 1 + max(height(node->left),
               height(node->right));

  int balance = getBalance(node);
  if (balance > 1 && key.value <= node->left->key.value)
    return rightRotate(node);

  if (balance < -1 && key.value > node->right->key.value)
    return leftRotate(node);

  if (balance > 1 && key.value > node->left->key.value) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }

  if (balance < -1 && key.value <= node->right->key.value) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  return node;
}

struct Node *minValueNode(struct Node *node) {
  struct Node *current = node;

  while (current->left != NULL)
    current = current->left;

  return current;
}


void printInOrder(struct Node *root) { //�������� ��� ������� �� �������������.
  if (root != NULL) {
    printInOrder(root->left);//����� ��������
    printf(" -> %s, %d", root->key.date, root->key.value);// ���� ����������� ��� �����.
    printInOrder(root->right);//��� ��� ����� ���� �����
  }
}


int findmin(struct Node *root) {//��������� ��� ��� ������ ��� ��������� �����.

  struct Node *current = root;
  while (current->left != NULL) {//���� ��� �������� ����� �� ������ ��� ��� �������� ����.
    current = current->left;
  }

  return current->key.value;
}


void printmin(struct Node *root, int min) { //��������� ��� ������� ��� �������� ��� ������ �� ��� �������� ����.
  if (root != NULL) {
        if (root->key.value == min)
            printf("\nThe lowest value %d is found on day %s.\n", min, root->key.date);
    printmin(root->left, min); //�������� �������� ����� �������� ����� <= �����.
  }
}


int findmax(struct Node *root) { //��������� ��� ��� ������ ��� �������� �����.

  struct Node *current = root;
  while (current->right != NULL) {//���� ��� ����� ����� �� ������ ��� ��� ���� ����.
    current = current->right;
  }
  return current->key.value;
}


void printmax(struct Node *root, int max) { //��������� ��� ������� ��� �������� ��� ������ �� ��� ������� ����.
  if (root != NULL) {
        if (root->key.value == max){
            printf("\nThe maximum value %d is found on day %s.\n", max, root->key.date);
            printmax(root->left, max);  //���� �������� ���� ���� ��� ���� ����� �������� <=. �� ���������� � �������. ����������� ������ ���� �������.
            }
    printmax(root->right, max); //�������� ����� ����� �� ��� �� �������.
  }
}

void readfile(struct Data pinakas[size])
{
int i=0;
    FILE *fp ;
    char *string;
    const char s[2] = "\t";

    fp = fopen("F:\\C_project\\testfiletxt.txt", "r");

if (fp == NULL)
    {
        printf("Error: could not open file");
        exit(0);
    }

    // reading line by line, max 256 bytes
    const unsigned MAX_LENGTH = 512;
    char buffer[MAX_LENGTH];

    fgets(buffer, MAX_LENGTH, fp); // ��� ����� ���� � buffer ������� ��� ������������.

    while (fgets(buffer, MAX_LENGTH, fp) && i < size)
    {

    string = strtok(buffer, s);
    strcpy(pinakas[i].direction,string);

    string = strtok(NULL, s);
    pinakas[i].year = atoi(string);

    string = strtok(NULL, s);
    strcpy(pinakas[i].date,string);

    string = strtok(NULL, s);
    strcpy(pinakas[i].weekday,string);

    string = strtok(NULL, s);
    strcpy(pinakas[i].country,string);

    string = strtok(NULL, s);
    strcpy(pinakas[i].commodity,string);

    string = strtok(NULL, s);
    strcpy(pinakas[i].transport,string);

    string = strtok(NULL, s);
    strcpy(pinakas[i].measure,string);

    string = strtok(NULL, s);
    pinakas[i].value = atoi(string);

    string = strtok(NULL, s);
    pinakas[i].cumulative = atoi(string);

    i = i + 1;
    }

    fclose(fp);
}

void printdata(struct Data pinakas[])
{
int i;
for(i=0; i<size; i++){
    printf("%d)%s %d %s %s %s %s %s %s %d %ld \n",i+1,pinakas[i].direction,pinakas[i].year,pinakas[i].date,
           pinakas[i].weekday,pinakas[i].country,pinakas[i].commodity,pinakas[i].transport,
           pinakas[i].measure,pinakas[i].value,pinakas[i].cumulative);
    }
}


int main()
{
    int choice,min,max;
    int i,z;
    struct Node *root = NULL;

    struct Data pinakas[size];

    min = pinakas[0].value;
    max = pinakas[0].value;

    readfile(pinakas);

    for (i = 0; i<size; i++)
        root = insertNode(root, pinakas[i]);

    printf("The AVL tree is: "); //���� ��� �������� ��� ������� ��� �� ����������� �� ����� ������������.

    printInOrder(root);

    printf("\n-------------------Menu--------------------\n");
    printf("(1)Display the dates with the lowest value.\n");
    printf("(2)Display the dates with the highest value.\n");
    printf("(3)Exit program.\n");
    printf("Enter your choice: ");
    scanf("%d",&choice);

    while (choice != 3)
    {
    switch(choice)
    {
        case 1:
            z = findmin(root);
            printmin(root,z);
            break;

        case 2:
            z = findmax(root);
            printmax(root,z);
            break;

        default:
            printf("Wrong choice. Please try again.\n");
            break;
    }
    printf("\nEnter your choice: ");
    scanf("%d",&choice);
    }

    printf("Exiting program...");
    return 0;
}

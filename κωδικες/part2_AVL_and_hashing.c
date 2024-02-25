#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define size 100


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
int hash;
} data;

struct Node {
  struct Data key;
  struct Node *left;
  struct Node *right;
  int height;
};

struct Nodehash {
    struct Data data;
    struct Nodehash* next;
};

int max(int a, int b);
int comparedates(char date1[],char date2[]);


int hash(char* str);
void calculatehash(struct Data d[]);
void insert(struct Nodehash* hashtable[], struct Data d);
void printdatahash(struct Nodehash* hashtable[]);

void insert(struct Nodehash* hash_table[], struct Data d) {
    int index = d.hash;
    struct Nodehash* nodehash = (struct Nodehash*)malloc(sizeof(struct Nodehash));
    nodehash->data = d;
    nodehash->next = hash_table[index];
    hash_table[index] = nodehash;
}

int hash(char* str) {
    int sum, re;
    sum = 0;
    for (int i = 0; str[i]; i++) {
        sum = sum + str[i];
    }
    re = sum % 11;
    return re;
}

void calculatehash(struct Data d[])
{
    int i = 0;
    while (i<size)
    {
        d[i].hash = hash(d[i].date);
        i = i + 1;
    }
}


int comparedates(char date1[],char date2[])
{

    int d1,m1,y1,d2,m2,y2;

    // Convert the date strings to integers
    sscanf(date1, "%d/%d/%d", &d1, &m1, &y1);
    sscanf(date2, "%d/%d/%d", &d2, &m2, &y2);

    // Compare the dates
    if (y1 > y2) {
        return 1;
    } else if (y1 < y2) {
        return -1;
    } else if (m1 > m2) {
        return 1;
    } else if (m1 < m2) {
        return -1;
    } else if (d1 > d2) {
        return 1;
    } else if (d1 < d2) {
        return -1;
    } else {
        return 0;
    }

}


int height(struct Node *N) {
  if (N == NULL)
    return 0;
  return N->height;
}

int max(int a, int b) {
  return (a > b) ? a : b;
}


struct Node *newNode(struct Data key) {
  struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    malloc(sizeof(struct Node));
  node->key = key;
  node->left = NULL;
  node->right = NULL;
  node->height = 1;
  return (node);
}


struct Node *rightRotate(struct Node *y) {
  struct Node *x = y->left;
  struct Node *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->height = max(height(y->left), height(y->right)) + 1;
  x->height = max(height(x->left), height(x->right)) + 1;

  return x;
}


struct Node *leftRotate(struct Node *x) {
  struct Node *y = x->right;
  struct Node *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = max(height(x->left), height(x->right)) + 1;
  y->height = max(height(y->left), height(y->right)) + 1;

  return y;
}


int getBalance(struct Node *N) {
  if (N == NULL)
    return 0;
  return height(N->left) - height(N->right);
}


struct Node *insertNodeValue(struct Node *node, struct Data key) {//Για την δημιουργια δεντρου αναλογα του πεδιου value.

  if (node == NULL)
    return (newNode(key));

  if (key.value <= node->key.value)
    node->left = insertNodeValue(node->left, key);
  else if (key.value > node->key.value)
    node->right = insertNodeValue(node->right, key);
  else
    return node;


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


struct Node *insertNodeDate(struct Node *node, struct Data key) { //Για την δημιουργια δεντρου αναλογα με τις ημερομηνιες.

  if (node == NULL)
    return (newNode(key));

  if (comparedates(key.date, node->key.date) < 0)
    node->left = insertNodeDate(node->left, key);
  else if (comparedates(key.date, node->key.date) > 0)
    node->right = insertNodeDate(node->right, key);
  else
    return node;


  node->height = 1 + max(height(node->left),height(node->right));

  int balance = getBalance(node);
  if (balance > 1 && comparedates(key.date, node->left->key.date) < 0)
    return rightRotate(node);

  if (balance < -1 && comparedates(key.date, node->right->key.date) > 0)
    return leftRotate(node);

  if (balance > 1 && comparedates(key.date, node->left->key.date) > 0)
  {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }

  if (balance < -1 && comparedates(key.date, node->right->key.date) < 0)
  {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  return node;
}


void printInOrder(struct Node *root) { //Εκτυπωση του δεντρου με ενδοδιαπεραση.
  if (root != NULL) {
    printInOrder(root->left);//Πρωτα αριστερα
    printf("%s %d %s %s %s %s %s %s %ld %ld \n", root->key.direction, root->key.year, root->key.date, root->key.weekday,
            root->key.country, root->key.commodity, root->key.transport, root->key.measure, root->key.value, root->key.cumulative);// Μετα εκτυπωνουμε τον κομβο.
    printInOrder(root->right);//Και στο τελος παμε δεξια
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


    const unsigned MAX_LENGTH = 512;
    char buffer[MAX_LENGTH];

    fgets(buffer, MAX_LENGTH, fp); // Την πρωτη φορα ο buffer παιρνει τις επικεφαλιδες.

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

void printdata(struct Nodehash* hash_table[]) {
    for (int i = 0; i < 11; i++) {
        printf("Chain %d:\n", i);
        struct Nodehash* nodehash = hash_table[i];
        while (nodehash) {
            printf("%s %d %s %s %s %s %s %s %ld %ld \n",
                nodehash->data.direction, nodehash->data.year, nodehash->data.date, nodehash->data.weekday,
                nodehash->data.country, nodehash->data.commodity, nodehash->data.transport, nodehash->data.measure,
                nodehash->data.value, nodehash->data.cumulative);
            nodehash = nodehash->next;
        }
    }
}


int main()
{
    int choice,i;
    struct Node *rootvalue  = NULL;
    struct Node *rootdate  = NULL;
    struct Nodehash* hashtable[11] = {NULL};

    struct Data pinakas[size];

    readfile(pinakas);

    printf("-------------------Menu--------------------\n");
    printf("(1)Create a BST tree.\n");
    printf("(2)Create hashing with chains.\n");
    printf("Enter your choice: ");
    scanf("%d",&choice);

    switch(choice)
    {
        case 1:
            printf("\nHow would you like the tree to be made?\n");
            printf("(1)According to dates.\n");
            printf("(2)According to values.\n");
            printf("Enter your choice: ");
            scanf("%d",&choice);

                switch(choice)
                {
                    case 1:
                        for (i = 0; i<size; i++)
                            rootdate = insertNodeDate(rootdate, pinakas[i]);

                        printf("\nThe AVL tree based on dates is: \n");
                        printf("Direction Year Date Weekday Country Commodity Transport Measure Value Cumulative\n");
                        printInOrder(rootdate);
                        break;

                    case 2:
                        for (i = 0; i<size; i++)
                            rootvalue = insertNodeValue(rootvalue, pinakas[i]);

                        printf("\n\nThe AVL tree based on values is: \n");
                        printf("Direction Year Date Weekday Country Commodity Transport Measure Value Cumulative\n");
                        printInOrder(rootvalue);
                        break;

                    default:
                        printf("\nInvalid choice. Exiting...");
                        break;
                }
                break;

        case 2:
            calculatehash(pinakas);

            for(int i=0; i < size; i ++)
                insert(hashtable, pinakas[i]);

            printf("\nThe chains are: \n");
            printdata(hashtable);
            break;

        default:
             printf("\nInvalid choice. Exiting...");
             break;
    }
    return 0;
}

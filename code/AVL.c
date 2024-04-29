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
} data;

struct Node { //struct για τους κόμβους στο δέντρο.
  struct Data key;
  struct Node *left;
  struct Node *right;
  int height;
};

struct Node *newNode(struct Data key) { // Δημιουργία κομβων.
  struct Node *node = (struct Node *)malloc(sizeof(struct Node)); //Δεσμυεση μνημης για τον κομβο.
  node->key = key; //Ορισμος κομβου. Σημειωση! Οι κομβου ειναι τυπου struct data.
  node->left = NULL;    //Αρχικοποιηση αριστερου δεικτη
  node->right = NULL;   //Αρχικοποιηση δεξιου δεικτη
  node->height = 1;//Αρχικοποιηση υψους.
  return (node);
}

int max(int a, int b);
int comparedates(char date1[],char date2[]);


int height(struct Node *N) { // Συναρτηση για τον υπολογισμο του υψους των κομβων.
  if (N == NULL)
    return 0;
  return N->height;
}

int max(int a, int b) {
  return (a > b) ? a : b;
}

struct Node *rightRotate(struct Node *y) { // Δεξια περιστροφη.
  struct Node *x = y->left; //Ο δεικτης Χ δειχνει στο αριστερο υποδεντρο του Y.
  struct Node *T2 = x->right; //Ο Τ2 δειχνει στο δεξι υποδεντρο του Χ.

  x->right = y; //Δεξιος κομβος του Χ το Υ.
  y->left = T2; //Αριστερος κομβος του Υ το υποδεντρο Τ2.

  y->height = max(height(y->left), height(y->right)) + 1; //Ενημερωση των υψων.
  x->height = max(height(x->left), height(x->right)) + 1;

  return x;
}


struct Node *leftRotate(struct Node *x) {   // Αριστερη περιστρογη. Ιδια λογικη με την δεξιά.
  struct Node *y = x->right;
  struct Node *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = max(height(x->left), height(x->right)) + 1;
  y->height = max(height(y->left), height(y->right)) + 1;

  return y;
}


int getBalance(struct Node *N) { // Υπολογισμος του βαθμου ισορροπιας του καθε κομβου.
  if (N == NULL)
    return 0;
  return height(N->left) - height(N->right);
}


struct Node *insertNode(struct Node *node, struct Data key) {   // Εισαγωγη Κομβου.

  if (node == NULL)
    return (newNode(key));

// Υπολογισμος της καταλληλης θεσης για να εισαχθει ο κομβος.

  if (comparedates(key.date, node->key.date) <= 0)
    node->left = insertNode(node->left, key);

  else if (comparedates(key.date, node->key.date) > 0)
    node->right = insertNode(node->right, key);

  else
    return node;

  // Ενημερωση των βαθμων ισσοροπιας του καθε κομβου και ισοσταθμιση του δεντρου.
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

struct Node *minValueNode(struct Node *node) {
  struct Node *current = node;

  while (current->left != NULL)
    current = current->left;

  return current;
}


struct Node *deleteNode(struct Node *root, char date[]) {// Διαγραφη κομβου.


  if (root == NULL)//Αν δεν υπαρχει ο κομβος.
    {
    printf("\nDate for deletion not found.\n");
    return root;
    }

    // Ευρεση κομβου και διαγραφη του.

  if (comparedates(date, root->key.date) < 0) //Αν εχει μικροτερη τιμη πάμε στο αριστερό υποδεντρο.
    root->left = deleteNode(root->left, date);

  else if (comparedates(date, root->key.date) > 0) //Αν εχει μεγαλυτερη τιμη παμε στο δεξι υποδεντρο.
    root->right = deleteNode(root->right, date);

  else {
    if ((root->left == NULL) || (root->right == NULL)) { //Αλλιως ο κομβος βρεθηκε.
      struct Node *temp = root->left ? root->left : root->right;

      if (temp == NULL) {
        temp = root;
        root = NULL;
      } else
        *root = *temp;
      free(temp);   //Ελευθερωση μνημης που καταλαμβανε ο κομβος.
      printf("Successful deletion.\n");

    } else {
      struct Node *temp = minValueNode(root->right);

      root->key = temp->key;

      root->right = deleteNode(root->right, temp->key.date);
    }
  }

  if (root == NULL)
      return root;


    //Ενημερωση του βαθμου ισορροπιας του καθε κομβου και διορθωση του δεντρου.
  root->height = 1 + max(height(root->left),
               height(root->right));

  int balance = getBalance(root);
  if (balance > 1 && getBalance(root->left) >= 0)
    return rightRotate(root);

  if (balance > 1 && getBalance(root->left) < 0) {
    root->left = leftRotate(root->left);
    return rightRotate(root);
  }

  if (balance < -1 && getBalance(root->right) <= 0)
    return leftRotate(root);

  if (balance < -1 && getBalance(root->right) > 0) {
    root->right = rightRotate(root->right);
    return leftRotate(root);
  }

  return root;
}


void SearchDate(struct Node* node, char date[]) {//Συναρτηση για ευρεση συγκεκριμενης ημερομηνιας.
    if(node == NULL) {
        printf("The date %s was not found.\n", date);
        return;
    }
    if(comparedates(date, node->key.date) < 0) {//Αν η ημερομηνια που ψαχνουμε ειναι μικροτερη απο την ημερομηνια
                                                //του κομβου παμε στο αριστερο υποδεντρο.
        SearchDate(node->left, date);
    }
    else if(comparedates(date, node->key.date) > 0) { //Αν η ημερομηνια που ψαχνουμε ειναι μεγαλυτερη απο την ημερομηνια
                                                        //του κομβου παμε στο δεξι υποδεντρο.
        SearchDate(node->right, date);
    }
    else {
        printf("The value of the date %s is %ld.\n", date, node->key.value); //Αν η ημερομηνια που ψαχνουμε βρεθηκε.
    }
}

struct Node *updateNode(struct Node *root, char date[], long int value) { //Συναρτηση για ενημερωση της τιμης ενος κομβου.
  if (root == NULL) //Αν η ημερομηνια του κομβου δεν βρεθηκε.
  {
    printf("The date %s was not found.\n", date);
    return root;
  }
  if (comparedates(date, root->key.date) < 0)       //Αν η ημερομηνια που ψαχνουμε ειναι μικροτερη απο την ημερομηνια
                                                    //του κομβου παμε στο αριστερο υποδεντρο.
    root->left = updateNode(root->left, date, value);
  else if (comparedates(date, root->key.date) > 0)      //Αν η ημερομηνια που ψαχνουμε ειναι μεγαλυτερη απο την ημερομηνια
                                                        //του κομβου παμε στο δεξι υποδεντρο.
    root->right = updateNode(root->right, date, value);
  else
    root->key.value = value;    //Αλλιως βρεθηκε η ημερομηνια που ψαχνουμε και επιστρεφετε το value της.
    return root;
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

    // reading line by line, max 512 bytes
    const unsigned MAX_LENGTH = 512;
    char buffer[MAX_LENGTH];

    fgets(buffer, MAX_LENGTH, fp); // Την πρωτη φορα ο buffer παιρνει τις επικεφαλιδες.
    //printf("%s", buffer);

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
    printf("%d)%s %d %s %s %s %s %s %s %ld %ld \n",i+1,pinakas[i].direction,pinakas[i].year,pinakas[i].date,
           pinakas[i].weekday,pinakas[i].country,pinakas[i].commodity,pinakas[i].transport,
           pinakas[i].measure,pinakas[i].value,pinakas[i].cumulative);
    }
}

void printInOrder(struct Node *root) { //Εκτυπωση του δεντρου με ενδοδιαπεραση.
  if (root != NULL) {
    printInOrder(root->left);//Πρωτα αριστερα
    printf("%s %d %s %s %s %s %s %s %ld %ld \n", root->key.direction, root->key.year, root->key.date, root->key.weekday,
            root->key.country, root->key.commodity, root->key.transport, root->key.measure, root->key.value, root->key.cumulative);// Μετα εκτυπωνουμε τον κομβο.
    printInOrder(root->right);//Και στο τελος παμε δεξια
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

int main()
{
    int i, choice,value;
    int d,m,y;
    char search[15];
    struct Node *root = NULL;
    struct Data pinakas[size];

    readfile(pinakas);

    //printdata(pinakas);

    for (i = 0; i<size; i++)
        root = insertNode(root, pinakas[i]);

    printf("-------------------Menu--------------------\n"); //Το menu επιλογων.
    printf("(1)Display BST Tree with in-order traversal.\n");
    printf("(2)Search a value of a specific date.\n");
    printf("(3)Change the value of a specific date.\n");
    printf("(4)Delete an entry for a specific date.\n");
    printf("(5)Exit program.\n");
    printf("Enter your choice: ");
    scanf("%d",&choice);

    while(choice !=5) //Το 5 ειναι η τιμη για την εξοδο. Με το while loop το switch-case θα τρεχει μεχρι να εισαχθει απο τον χρηστη η τιμη 5.
    {
    switch(choice)
    {
        case 1:
            printf("\nThe AVL tree is: \n");
            printf("Direction Year Date Weekday Country Commodity Transport Measure Value Cumulative\n");
            printInOrder(root);
            printf("\n");
            break;

        case 2:
            printf("\nPlease enter the date you are searching the value for.\n");
            printf("Please enter the day: ");
            scanf("%d", &d);
            printf("Please enter the month: ");
            scanf("%d", &m);
            printf("Please enter the year: ");
            scanf("%d", &y);

             while(d<1 || d>31 || m<1 || m>12 || y<2000 || y>2023 || (d>28 && m==2) || ((m == 4 || m == 6 || m==9 || m == 11) && d ==31)) //Αμυντικος προγραμματισμος για τις ημερομηνιες.
            {
            printf("Incorrect date. Please try again.\nPlease enter the day.");
            scanf("%d", &d);
            printf("Please enter the month: ");
            scanf("%d", &m);
            printf("Please enter the year: ");
            scanf("%d", &y);
            }

            snprintf(search, 15, "%d/%d/%d", d,m,y);
            SearchDate(root,search);
            break;

        case 3:
            printf("\nPlease enter the date you want to modify the value for.\n");
            printf("Please enter the day: ");
            scanf("%d", &d);
            printf("Please enter the month: ");
            scanf("%d", &m);
            printf("Please enter the year: ");
            scanf("%d", &y);

            while(d<1 || d>31 || m<1 || m>12 || y<2000 || y>2023 || (d>28 && m==2) || ((m == 4 || m == 6 || m==9 || m == 11) && d ==31)) //Αμυντικος προγραμματισμος για τις ημερομηνιες.
            {
            printf("Incorrect date. Please try again.\nPlease enter the day.");
            scanf("%d", &d);
            printf("Please enter the month: ");
            scanf("%d", &m);
            printf("Please enter the year: ");
            scanf("%d", &y);
            }

            snprintf(search, 15, "%d/%d/%d", d,m,y);
            printf("Enter the desired value: ");
            scanf("%d", &value);
            updateNode(root,search,value);
            break;

        case 4:
            printf("\nPlease enter the date you want to delete.\n");
            printf("Enter the day: ");
            scanf("%d", &d);
            printf("please enter the month: ");
            scanf("%d", &m);
            printf("please enter the year: ");
            scanf("%d", &y);

            while(d<1 || d>31 || m<1 || m>12 || y<2000 || y>2023 || (d>28 && m==2) || ((m == 4 || m == 6 || m==9 || m == 11) && d ==31)) //Αμυντικος προγραμματισμος για τις ημερομηνιες.
            {
            printf("Incorrect date. Please try again.\nPlease enter the day.");
            scanf("%d", &d);
            printf("Please enter the month: ");
            scanf("%d", &m);
            printf("Please enter the year: ");
            scanf("%d", &y);
            }

            snprintf(search, 15, "%d/%d/%d", d,m,y);
            deleteNode(root,search);
            break;

        default:
            printf("Incorrect choice. Please try again.");
            break;
    }
    printf("\nEnter your choice: ");
    scanf("%d",&choice);
    }
    printf("\nExiting program...");
    return 0;
}

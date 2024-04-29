#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
int hash; // Μεταβλητη για την αλυσιδα που ανηκει το καθε struct
};

struct Node { //Κομβος για τις λιστες στα buckets
    struct Data data;
    struct Node* next;
};

int hash(char* str);
void calculatehash(struct Data d[]);
void insert(struct Node* hash_table[], struct Data d);
void printdata(struct Node* hash_table[]);
void readfile(struct Data pinakas[]);
void search(struct Node* hash_table[], char* date);
void changevalue(struct Node* hashtable[], char* date, long int updatedvalue);
void deleteentry(struct Node* hashtable[], char* date);


void insert(struct Node* hash_table[], struct Data d) { //Συναρτηση για τις εισαγωγες σε αλυσιδες.
    int index = d.hash;
    struct Node* insertnode = (struct Node*)malloc(sizeof(struct Node)); //Δεσμευση μνημης για την καθε αλυσιδα.
    insertnode->data = d; //Τα δεδομενα ειναι το struct data.
    insertnode->next = hash_table[index]; //Δεικτης για την επομενη τιμη.
    hash_table[index] = insertnode;
}

void calculatehash(struct Data d[]) //Υπολογισμος του hash key που θα ανηκει καθε entry του data.
{
    int i = 0;
    while (i<size)
    {
        d[i].hash = hash(d[i].date);
        i = i + 1;
    }
}

int hash(char* str) { //Υπολογιζει το αθροισμα των χαρακτηρων ασκι. Με αλλα λογια η συναρτηση κερματισμου.
    int sum = 0;
    for (int i = 0; str[i]; i++) {
        sum = sum + str[i];
    }
    return sum % 11;
}

void search(struct Node* hashtable[], char* date) { //Συναρτηση για την ευρεση συγκεκριμενης ημερομηνιας.

    int search;
    search = hash(date); //Η ημερομηνια που δεχεται η συναρτηση σαν ορισμα περναει απο την συναρτηση κερματισμου για να βρουμε την σωστη αλυσιδα.

    struct Node* searchnode;
    searchnode = hashtable[search]; //Η αλυσιδα που ψαχνουμε.

    while (searchnode) { //Oσο υπαρχουν στοιχεια στην αλυσιδα.
        if (strcmp(searchnode->data.date, date) == 0) { //Αν βρεθει η ημερομηνια.
            printf("Value for date %s is %ld.\n", date, searchnode->data.value);
            return;
        }
        searchnode = searchnode->next;//Επομενο στοιχειο του bucket.
    }

    printf("No data found for date %s.\n", date);//Αν εξαντληθουν ολα τα στοιχεια της αλυσιδας και δεν βρεθει η ημερομηνια.
}


void changevalue(struct Node* hashtable[], char* date, long int updatedvalue) { //Συναρτηση για την αλλαγη ενος value.

    int search;
    search = hash(date); //Η ημερομηνια που δεχεται η συναρτηση σαν ορισμα περναει απο την συναρτηση κερματισμου για να βρουμε την σωστη αλυσιδα.

    struct Node* searchnode;
    searchnode = hashtable[search]; //Η αλυσιδα που ψαχνουμε.

    while (searchnode) { //Oσο υπαρχουν στοιχεια στην αλυσιδα.
        if (strcmp(searchnode->data.date, date) == 0) { //Αν βρεθει η ημερομηνια.
            searchnode->data.value = updatedvalue; //Αλλαγη value.
            printf("The updated value for date %s is %ld.\n", date, searchnode->data.value);
            return;
        }
        searchnode = searchnode->next; //Επομενο στοιχειο του bucket.
    }

    printf("The date %s was not found.\n", date); //Αν εξαντληθουν ολα τα στοιχεια του bucket και δεν βρεθει η ημερομηνια.
}

void deleteentry(struct Node* hashtable[], char* date) { //Συναρτηση για διαγραφη ενος entry.

    int search;
    search = hash(date); //Η ημερομηνια που δεχεται η συναρτηση σαν ορισμα περναει απο την συναρτηση κερματισμου για να βρουμε την σωστη αλυσιδα.

    struct Node* searchnode;
    searchnode = hashtable[search]; //Η αλυσιδα που ψαχνουμε.

    struct Node* prev = NULL;

    while (searchnode) { //Oσο υπαρχουν στοιχεια στην αλυσιδα.
        if (strcmp(searchnode->data.date, date) == 0) { //Αν βρεθει η ημερομηνια.
            if (prev) {
                prev->next = searchnode->next; // Unlink του current κομβου απο την λιστα.
            } else {
                hashtable[search] = searchnode->next; // Ο current node ειναι η αρχη της λιστας.
            }
            free(searchnode); // Αποδεσμευση της μνημης του node που διαγραψαμε.
            printf("Entry for date %s has been deleted.\n", date);
            return;
        }
        prev = searchnode; //Εντολες για να μεταφερθουμε στον επομενο κομβο της λιστας.
        searchnode = searchnode->next;
    }
    printf("The date %s was not found\n", date);
}



void readfile(struct Data pinakas[])
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

void printdata(struct Node* hash_table[]) { //Προσθετη συναρτηση για την εκτυπωση των buckets.
    for (int i = 0; i < 11; i++) {
        printf("Chain %d:\n", i);
        struct Node* node = hash_table[i];
        while (node) {
            printf("%s %d %s %s %s %s %s %s %ld %ld \n",
                node->data.direction, node->data.year, node->data.date, node->data.weekday,
                node->data.country, node->data.commodity, node->data.transport, node->data.measure,
                node->data.value, node->data.cumulative);
            node = node->next;
        }
    }
}

int main()
{
    int choice,d,m,y,value;
    char searchdate[15];
    struct Data pinakas[size];
    struct Node* hashtable[11] = {NULL};

    readfile(pinakas);

    calculatehash(pinakas);

    for(int i=0; i < size; i ++)
        insert(hashtable, pinakas[i]);


    printf("-------------------Menu--------------------\n");//Το menu επιλλογων.
    printf("(1)Search a value of a specific date.\n");
    printf("(2)Change the value of a specific date.\n");
    printf("(3)Delete an entry for a specific date.\n");
    printf("(4)Display chains.\n"); //Προσθετη επιλογη για την εκτυπωση των chains.
    printf("(5)Exit program.\n");
    printf("Enter your choice: ");
    scanf("%d",&choice);

    while(choice !=5)
    {
    switch(choice)
    {
        case 1:
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

            snprintf(searchdate, 15, "%d/%d/%d", d,m,y);
            search(hashtable,searchdate);
            break;

        case 2:
            printf("\nPlease enter the date you want to update the value for.\n");
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

            snprintf(searchdate, 15, "%d/%d/%d", d,m,y);
            printf("Enter the desired value: ");
            scanf("%d", &value);
            changevalue(hashtable,searchdate,value);
            break;

        case 3:
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

            snprintf(searchdate, 15, "%d/%d/%d", d,m,y);
            deleteentry(hashtable,searchdate);
            break;

        case 4:
            printdata(hashtable);
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

    return 0;
}

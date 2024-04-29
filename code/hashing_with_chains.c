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
int hash; // ��������� ��� ��� ������� ��� ������ �� ���� struct
};

struct Node { //������ ��� ��� ������ ��� buckets
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


void insert(struct Node* hash_table[], struct Data d) { //��������� ��� ��� ��������� �� ��������.
    int index = d.hash;
    struct Node* insertnode = (struct Node*)malloc(sizeof(struct Node)); //�������� ������ ��� ��� ���� �������.
    insertnode->data = d; //�� �������� ����� �� struct data.
    insertnode->next = hash_table[index]; //������� ��� ��� ������� ����.
    hash_table[index] = insertnode;
}

void calculatehash(struct Data d[]) //����������� ��� hash key ��� �� ������ ���� entry ��� data.
{
    int i = 0;
    while (i<size)
    {
        d[i].hash = hash(d[i].date);
        i = i + 1;
    }
}

int hash(char* str) { //���������� �� �������� ��� ���������� ����. �� ���� ����� � ��������� �����������.
    int sum = 0;
    for (int i = 0; str[i]; i++) {
        sum = sum + str[i];
    }
    return sum % 11;
}

void search(struct Node* hashtable[], char* date) { //��������� ��� ��� ������ ������������� �����������.

    int search;
    search = hash(date); //� ���������� ��� ������� � ��������� ��� ������ ������� ��� ��� ��������� ����������� ��� �� ������ ��� ����� �������.

    struct Node* searchnode;
    searchnode = hashtable[search]; //� ������� ��� ��������.

    while (searchnode) { //O�� �������� �������� ���� �������.
        if (strcmp(searchnode->data.date, date) == 0) { //�� ������ � ����������.
            printf("Value for date %s is %ld.\n", date, searchnode->data.value);
            return;
        }
        searchnode = searchnode->next;//������� �������� ��� bucket.
    }

    printf("No data found for date %s.\n", date);//�� ����������� ��� �� �������� ��� �������� ��� ��� ������ � ����������.
}


void changevalue(struct Node* hashtable[], char* date, long int updatedvalue) { //��������� ��� ��� ������ ���� value.

    int search;
    search = hash(date); //� ���������� ��� ������� � ��������� ��� ������ ������� ��� ��� ��������� ����������� ��� �� ������ ��� ����� �������.

    struct Node* searchnode;
    searchnode = hashtable[search]; //� ������� ��� ��������.

    while (searchnode) { //O�� �������� �������� ���� �������.
        if (strcmp(searchnode->data.date, date) == 0) { //�� ������ � ����������.
            searchnode->data.value = updatedvalue; //������ value.
            printf("The updated value for date %s is %ld.\n", date, searchnode->data.value);
            return;
        }
        searchnode = searchnode->next; //������� �������� ��� bucket.
    }

    printf("The date %s was not found.\n", date); //�� ����������� ��� �� �������� ��� bucket ��� ��� ������ � ����������.
}

void deleteentry(struct Node* hashtable[], char* date) { //��������� ��� �������� ���� entry.

    int search;
    search = hash(date); //� ���������� ��� ������� � ��������� ��� ������ ������� ��� ��� ��������� ����������� ��� �� ������ ��� ����� �������.

    struct Node* searchnode;
    searchnode = hashtable[search]; //� ������� ��� ��������.

    struct Node* prev = NULL;

    while (searchnode) { //O�� �������� �������� ���� �������.
        if (strcmp(searchnode->data.date, date) == 0) { //�� ������ � ����������.
            if (prev) {
                prev->next = searchnode->next; // Unlink ��� current ������ ��� ��� �����.
            } else {
                hashtable[search] = searchnode->next; // � current node ����� � ���� ��� ������.
            }
            free(searchnode); // ����������� ��� ������ ��� node ��� ����������.
            printf("Entry for date %s has been deleted.\n", date);
            return;
        }
        prev = searchnode; //������� ��� �� ������������ ���� ������� ����� ��� ������.
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

void printdata(struct Node* hash_table[]) { //�������� ��������� ��� ��� �������� ��� buckets.
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


    printf("-------------------Menu--------------------\n");//�� menu ���������.
    printf("(1)Search a value of a specific date.\n");
    printf("(2)Change the value of a specific date.\n");
    printf("(3)Delete an entry for a specific date.\n");
    printf("(4)Display chains.\n"); //�������� ������� ��� ��� �������� ��� chains.
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

            while(d<1 || d>31 || m<1 || m>12 || y<2000 || y>2023 || (d>28 && m==2) || ((m == 4 || m == 6 || m==9 || m == 11) && d ==31)) //��������� ��������������� ��� ��� �����������.
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

            while(d<1 || d>31 || m<1 || m>12 || y<2000 || y>2023 || (d>28 && m==2) || ((m == 4 || m == 6 || m==9 || m == 11) && d ==31)) //��������� ��������������� ��� ��� �����������.
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

            while(d<1 || d>31 || m<1 || m>12 || y<2000 || y>2023 || (d>28 && m==2) || ((m == 4 || m == 6 || m==9 || m == 11) && d ==31)) //��������� ��������������� ��� ��� �����������.
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

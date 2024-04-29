#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
long int value;
long int cumulative;
} data;


void printdata(struct Data d[]);

void heapify(struct Data d[], int N, int i);

void heapSort(struct Data d[], int N);

void swap(struct Data d1[], struct Data d2[]);

int comparedates(char date1[], char date2[]);

void readfile();

void heapSort(struct Data d[], int N)
{
    int i;

    for (i = N/2-1; i >= 0; i--)// Δημιουργια μεγιστου σωρου
        heapify(d, N, i); // Heapify για να παρω το μεγιστο στοιχεο στην ριζα


    for (i = N-1; i >= 0; i--) {// Heap sort

        swap(&d[0], &d[i]);
        heapify(d, i, 0); // Heapify για να παρω το μεγιστο στοιχεο στην ριζα ξανα
    }
}

void heapify(struct Data d[], int N, int i)
{
    // Αρχικοποιηση root ως μεγιστο
    int max;
    max  = i;

    int left;
    int right;

    left  = 2*i + 1; //αριστερο παιδι
    right = 2*i + 2; //δεξι παιδι

    if (left < N && (d[left].cumulative > d[max].cumulative ))  // Αν το αριστερο παιδι ειναι μεγαλυτερο απο την ριζα
        max = left;

    else
    {
            if (right < N && (d[right].cumulative > d[max].cumulative )) // Αν το δεξι παιδι ειναι μεγαλυτερο απο την ριζα
            max = right;

            else
                {
                    if ( left < N && d[left].cumulative == d[max].cumulative && comparedates(d[left].date, d[max].date) < 0) //Για κοινα cumulative ελεγχονται τα dates.
                    max = left;

                    if (right < N && d[right].cumulative == d[max].cumulative && comparedates(d[right].date, d[max].date) < 0) //Για κοινα cumulative ελεγχονται τα dates.
                    max = right;
                }
    }


    if (max != i) {// Εναλλαγη και heapify αν η ριζα δεν ειναι η μεγαλυτερη.

            swap(&d[i], &d[max]);
            heapify(d, N, max);
        }
}

void swap(struct Data d1[], struct Data d2[]) //Swap για τα structs.
{
    struct Data temp;
    temp = *d1;
    *d1 = *d2;
    *d2 = temp;
}

int comparedates(char date1[],char date2[])
{

    int d1,m1,y1,d2,m2,y2;

    // Μετατροπη των strings σε integers για να μπορω να συγκρινω τις ημερομηνίες. Σημείωση: Η strcmp δεν δουλέυει για ημερομηνίες
    sscanf(date1, "%d/%d/%d", &d1, &m1, &y1);
    sscanf(date2, "%d/%d/%d", &d2, &m2, &y2);

    if (y1 > y2) {
        return 1;
    } else if (y1 < y2) {
        return -1;
    } else if (m1 > m2) { // Αν y1 = y2 τότε κοιτάω τους μήνες
        return 1;
    } else if (m1 < m2) {
        return -1;
    } else if (d1 > d2) { // Αν m1 = m2 τότε κοιτάω τις ημέρες
        return 1;
    } else if (d1 < d2) {
        return -1;
    } else {
        return 0;
    }

}


void readfile(struct Data pinakas[])
{
int i=0;
    FILE *fp ;
    char *string;
    const char s[2] = "\t"; //Χρησιμοποιω το tab ως delimiter για να διαβασω ξεχωριστα το κάθε διαφορετικό πλαίσιο της κάθε γραμμής

    fp = fopen("F:\\C_project\\testfiletxt.txt", "r"); //Σημείωση. To pathway πρέπει να αλλαχτεί για διαφορετικό σύστημα.

if (fp == NULL)
    {
        printf("Error: could not open file");
        exit(0);
    }


    const unsigned MAX_LENGTH = 512;
    char buffer[MAX_LENGTH];    // Διαβασμα γραμμης με μεγιστο 512 bytes

    fgets(buffer, MAX_LENGTH, fp); // Την πρωτη φορα ο buffer παιρνει τις επικεφαλιδες.
    printf("%s", buffer);

    while (fgets(buffer, MAX_LENGTH, fp) && i < size)
    {

    string = strtok(buffer, s); //Πρώτη λέξη του buffer
    strcpy(pinakas[i].direction,string); //Αποθήκευση direction

    string = strtok(NULL, s);   //Δεύτερη λέξη του buffer
    pinakas[i].year = atoi(string); //Αποθηκευση year

    string = strtok(NULL, s); //Τρίτη λέξη του buffer
    strcpy(pinakas[i].date,string); //Ομοίως για όλα τα παρακάτω.

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

    i = i + 1; //Αλλαγή γραμμής.
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

int main()
{

    struct Data pinakas[size]; //Ορισμός struct

    readfile(pinakas); //Διάβασμα αρχείου και αποθηκευεση στο struct

    printdata(pinakas); //Εκτύπωση μη ταξινομημένου struct

    heapSort(pinakas, size); //Ταξινομηση

    printf("\n\n\nThe sorted array is: \n\n\n");

    printdata(pinakas); //Εκτύπωση ταξινομημένου struct

    return 0;
}

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
long int value;
long int cumulative;
} data;


int interpolsearch(struct Data d[], int start, int end, char search[], long int *x1, long int *x2);

void printdata(struct Data d[]);

int datetonumber(char date[]);

void readfile();

int datetonumber(char date[]) //Συναρτηση για την μετατροπη των ημερομινιων σε αριθμους. Περισσοτερα σχολια στην αναφορα.
{
    int number,d,m,y;
    sscanf(date, "%d/%d/%d", &d, &m, &y);
    number = 500*y + 40*m + d;
    return number;
}


int interpolsearch(struct Data d[], int start, int end, char search[], long int *x1, long int *x2)
{

    int pos = start + (((double)(end - start) / (datetonumber(d[end].date) - datetonumber(d[start].date)))
                       * (datetonumber(search) - datetonumber(d[start].date))); //ορισμος pos συμφωνα με τον τυπο του interpolation search.

    if (start > end || pos<0 || pos > size) { //Αν τελος < αρχη ή pos εκτός ορίων του πίνακα, τοτε δεν βρεθηκε η ημερομηνια και επιστροφη 0.
        return 0;
    }

    if (datetonumber(d[pos].date) == datetonumber(search)) { //Αν βρεθηκε η ημερονία επιστροφή 1.
        *x1 = d[pos].value;
        *x2 = d[pos].cumulative;
        return 1;
    } else if (datetonumber(d[pos].date) > datetonumber(search)) { //Αν η δοσμενη ημερομηνια ειναι μικροτερη τοτε interpolation search από την αρχη μεχρι pos-1.
        return interpolsearch(d, start, pos-1, search, x1, x2);
    } else {
        return interpolsearch(d, pos+1, end, search,x1,x2); //Αν η δοσμενη ημερομηνια ειναι μεγαλυτερη τοτε interpolation search από το pos + 1 μεχρι το τελος.
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

void swap(struct Data d1[], struct Data d2[])
{
    struct Data temp;
    temp = *d1;
    *d1 = *d2;
    *d2 = temp;
}

int partition(struct Data d[], int left, int right) { //Quicksort με βαση της ημερομηνιες.

  int pivot = right;
  int i = left - 1;

  for (int j = left; j <= right - 1; j++) {
    if (comparedates(d[j].date, d[pivot].date) < 0) {
      i++;
      swap(&d[i], &d[j]);
    }
  }
  swap(&d[i + 1], &d[pivot]);
  return i + 1;
}

void quickSort(struct Data d[], int left, int right) {// Συναρτηση για να χρησιμοποιηση τον Quick Sort
    int pi;
    if (left < right) {
        pi = partition(d, left, right);
        quickSort(d, left, pi - 1);
        quickSort(d, pi + 1, right);
    }
}

int main()
{
    int n;

    struct Data pinakas[size]; //Ορισμός struct

    readfile(pinakas); //Διάβασμα αρχείου και αποθηκευεση στο struct

    n = sizeof(pinakas)/sizeof(pinakas[0]); // Ορισμος n συμφωνα με τον ορισμο του Quick sort

    quickSort(pinakas, 0, n-1); //Ταξινομηση με ημερομηνίες.

    printdata(pinakas);  //Εκτύπωση μη ταξινομημένου struct

    long int x1,x2; //Τα x1 και x2 θα χρησιμοποιουνται σαν ορισματα στην συναρτηση της δυαδικης αναζητησης
                    //και θα είναι με αναφορα ωστε να αποθηκευτουν οι επιθυμητες τιμες σε αυτα.
    int d,m,y,z;

    printf("Enter a date in the following format.(example: 10/10/2010). \nFirst enter the day:");
    scanf("%d", &d);
    printf("please enter the month: ");
    scanf("%d", &m);
    printf("please enter the year: ");
    scanf("%d", &y);

    while(d<1 || d>31 || m<1 || m>12 || y<2000 || y>2023 || (d>28 && m==2) || ((m == 4 || m == 6 || m==9 || m == 11) && d ==31)) //Αμυντικος προγραμματισμος για τις ημερομηνιες.
    {
    printf("incorrect date. Please try again. Enter day.");
    scanf("%d", &d);
    printf("please enter the month: ");
    scanf("%d", &m);
    printf("please enter the year: ");
    scanf("%d", &y);
    }

    char date[20];
    snprintf(date, 20, "%d/%d/%d", d,m,y); //Εννοποιηση των integers των d,m,y σε ενα string για την ημερομηνια.
    printf("The date you entered is: %s \n", date);

    z = interpolsearch(pinakas, 0, size-1, date, &x1, &x2); //Καλεσμα της interpolation search.


    if (z==1)
        printf("Date found. The value and cumulative respectively are: %ld $ and %ld $\n", x1,x2);
    if (z==0)
        printf("Date not found. \n");


    return 0;
}

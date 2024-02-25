#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#define size 365
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


int binterpolsearch(struct Data d[], int start, int end, char search[], long int *x1, long int *x2);

void printdata(struct Data d[]);

int datetonumber(char date[]);

void readfile();

int datetonumber(char date[]) //Συναρτηση για την μετατροπη των ημερομινιων σε αριθμους. Περισσοτερα σχολια στην αναφορα.
{
    int number,d,m,y;
    sscanf(date, "%d/%d/%d", &d, &m, &y);
    number = 420*y + 32*m + d;
    return number;
}


int binterpolsearch(struct Data d[], int start, int end, char search[], long int *x1, long int *x2) //Η παραλαγή του Bis σύμφωνα με το βιβλίο του κ. Τσακαλίδη.
{
    int left,right,size2,next,i,k; //Ορισμός τιμών σύμφωνα με τον αλγόριθμο.
    left = 0;
    right = size-1;
    size2 = right - left + 1;

    int key = datetonumber(search); //Key =  η ημερομηνία που ψάχνουμε.

    next = floor(size2 * (key - datetonumber(d[left].date))/(datetonumber(d[right].date) - datetonumber(d[left].date))) - 1; // Ορισμός του next σύμφωνα με τον bis.
    //printf ("next = %d\n", next);

    if (next < 0 || next > size) //Αν ειμαστε εκτος οριων
            return 0;

    while ( key != datetonumber(d[next].date)) //Όσο δεν έχει βρεθεί η ημερομηνία.
    {

        i = 0;
        size2 = right - left + 1; //Τύπος για το μέγεθος.

            if (key >= datetonumber(d[next].date)){  //Αν η ημερομηνία που δώσαμε είναι μεγαλύτερη της ημερομηνίας του next.
                while (key > datetonumber(d[next + i * (int)sqrt(size2) - 1].date)) //'Οσο ισχύει η ανισότητα, αυξάνουμε το i.
                 {
                    i = i + 1;
                 }
                right = i*sqrt(size2); //Αναννέωση των right και left.
                left = (i-1)* sqrt(size2);

            }

            else if  (key <= datetonumber(d[next].date)) //Αν η ημερομηνία που δώσαμε είναι μικρότερη της ημερομηνίας του next.
            {
                while (key > datetonumber(d[next - i * (int)sqrt(size2) + 1].date)) //'Οσο ισχύει η ανισότητα, αυξάνουμε το i.
                    {
                    i = i + 1;
                    }

                right = (i-1)*sqrt(size2); //Αναννέωση των right και left.
                left = i*sqrt(size2);
            }

            next = floor((right - left)*(key - datetonumber(d[left].date))/(datetonumber(d[right].date) - datetonumber(d[left].date))) - 1; //Υπολογίζουμε το νέο κλειδί next.
            //printf("Next = %d\n", next);
    }

    if(key == datetonumber(d[next].date)) //Αν βρεθεί η ημερομηνία, ενημερώνουμε τους δείκτες για το value και cumulative.
    {
        *x1 = d[next].value;
        *x2 = d[next].cumulative;
        return 1;
    }
    else
        return 0;
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

    n = sizeof(pinakas)/sizeof(pinakas[0]); // Ορισμος n συμφωνα με τον ορισμο του Quick sort

    readfile(pinakas); //Διάβασμα αρχείου και αποθηκευεση στο struct

    quickSort(pinakas, 0, n-1); //Ταξινομηση με βάση τις ημερομηνίες.

    printdata(pinakas); //Εκτύπωση ταξινομημένου struct

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

    z = binterpolsearch(pinakas, 0, size-1, date, &x1, &x2);//Κλήση της binterpolsearch.

    if (z==1)
        printf("Date found. The value and cumulative respectively are: %ld $ and %ld $\n", x1,x2);
    if (z ==0)
        printf("Date not found");

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long int size = 1000;

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

int comparedates(char date1[], char date2[]);

void merge(struct Data arr[], int l, int m, int r);

void mergesort(struct Data arr[], int l, int r);

void readfile();

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

void merge(struct Data d[], int l, int m, int r)
{
    int i, j, k; //Δείκτες για τους πίνακες
    int n1 = m-l+1;
    int n2 = r-m;


    struct Data Left[n1], Right[n2]; //Δημιουργία προσωρινών πινάκων

    for (i = 0; i < n1; i++)    //Αντιγραφη δεδομενων στους προσωιρνους πινακες.
        Left[i] = d[l+i];

    for (j = 0; j < n2; j++)
        Right[j] = d[m +1+j];

    // Συγχωνευση των προσωρινων πινακων
    i = 0; // δείκτης για τον πρωτο πινακα
    j = 0; // δείκτης για τον δευτερο πινακα
    k = l; // δείκτης για τον συγχονευμενο πινακα

    while (i < n1 && j < n2) {
        if (Left[i].value < Right[j].value) {
            d[k] = Left[i];
            i++;
        }
        else if (Left[i].value > Right[j].value) {
            d[k] = Right[j];
            j++;
        }
        else { // Αν τα values είναι ιδιες, συγκρινε ημερομηνιες
            if (comparedates(Left[i].date, Right[j].date) < 0) {
                d[k] = Left[i];
                i = i + 1;
            }
            else {
                d[k] = Right[j];
                j = j + 1;
            }
        }
        k = k + 1;
    }


    while (i < n1) { // αντιγραφη των στοιχειων του L[] που απομενουν, αν υπαρχουν
        d[k] = Left[i];
        i = i + 1;
        k = k + 1;
    }

    while (j < n2) { // αντιγραφη των στοιχειων του R[] που απομενουν, αν υπαρχουν
        d[k] = Right[j];
        j = j + 1;
        k = k + 1;
    }
}

void mergesort(struct Data d[], int left, int right)
{
    if (left<right) {
        int m =left+(right-left)/2;

        // Διαίρεση των πινάκων μέχρι να φτάσουν μήκους 1
        mergesort(d, left, m);
        mergesort(d, m + 1, right);

        // συγχωνευση των ταξινομημενων μισων
        merge(d, left, m, right);
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

    mergesort(pinakas, 0, size-1); //Ταξινομηση

    printf("\n\n\nThe sorted array is: \n\n\n");
    printdata(pinakas);  //Εκτύπωση ταξινομημένου struct

    return 0;
}

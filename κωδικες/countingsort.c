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

void swap(struct Data d1[], struct Data d2[]);

int comparedates(char date1[], char date2[]);

void countingsort(struct Data d[], int size);


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

void countingsort(struct Data d[], int size) {

    int i;
    long int max;
    long int* C;
    int index;

    max = d[0].value;

    for (int i = 1; i < size; i++) {    //Εύρεση Μεγίστου
        if (d[i].value > max) {
            max = d[i].value;
        }
    }


    C = (long int*)calloc(max+1, sizeof(long int));// Δέσμευση μνήμης για τον πίανακα C δυναμικά
    struct Data* array2 = (struct Data*)malloc(size * sizeof(struct Data)); // Δημιουργία ενός δεύτερου προσωρινού πίνακα για να αποθηκέυσει τα ταξινομημένα δεδομένα

    for (i = 0; i < size; i++) {    // Αποθήκευση των εμφανίσεων του κάθε αριθμού
        C[d[i].value]++;
    }


    for (i = 1; i <= max; i++) {    // Υπολογισμός των πλήρη μφανίσεων του κάθε αριθμού όπως ορίζει ο coutningsort
        C[i] += C[i-1];
    }


    for (i = size-1; i >= 0; i--) { // Ταξινόμηση των στοιχείων
        index = d[i].value;
        array2[C[index]-1] = d[i];
        C[index]--;
    }

    struct Data temp; // Ενα προσωρινο struct για την μεταθεσεις

    for (int i = 0; i < size-1; i++) { //Ταξινόμηση αναλογα των ημερομιων για ίδιες τιμες values.
        if (array2[i].value == array2[i+1].value && comparedates(array2[i].date, array2[i+1].date) > 0) {
            temp = array2[i];
            array2[i] = array2[i+1];
            array2[i+1] = temp;
            i = 0; // Επαναήψη του βρογχου σε περιπτωση εναλλαγης. Περισσοτερα σχολια στην αναφορα
        }
    }


    for (int i = 0; i < size; i++) {// Αντιγραφη του ταξινομημενου πινακα στον αρχικο.Copy the sorted data back into the original array
        d[i] = array2[i];
    }

    // Ελευθερωση μνημης.
    free(C);
    free(array2);
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
    //printf("%s", buffer);

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

void printdata(struct Data pinakas[]) //Εκτύπωση του struct
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

    readfile(pinakas);  //Διάβασμα αρχείου και αποθηκευεση στο struct

    printdata(pinakas); //Εκτύπωση μη ταξινομημένου struct

    countingsort(pinakas, size); //Ταξινομηση

    printf("\n\n\nThe sorted array is: \n\n\n");
    printdata(pinakas); //Εκτύπωση ταξινομημένου struct

    return 0;
}

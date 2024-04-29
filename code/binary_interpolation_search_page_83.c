#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
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


int binterpolsearch(struct Data d[], int start, int end, char search[], long int *x1, long int *x2);

void printdata(struct Data d[]);

int datetonumber(char date[]);

void readfile();

int datetonumber(char date[]) //��������� ��� ��� ��������� ��� ����������� �� ��������. ����������� ������ ���� �������.
{
    int number,d,m,y;
    sscanf(date, "%d/%d/%d", &d, &m, &y);
    number = 500*y + 40*m + d;
    return number;
}


int binterpolsearch(struct Data d[], int start, int end, char search[], long int *x1, long int *x2) //� �������� ��� Bis ������� �� �� ������ ��� �. ���������.
{
    int left,right,size2,next,i,k; //������� ����� ������� �� ��� ���������.
    left = 0;
    right = size-1;
    size2 = right - left + 1;

    int key = datetonumber(search); //Key =  � ���������� ��� ��������.

    next = floor(size2 * (key - datetonumber(d[left].date))/(datetonumber(d[right].date) - datetonumber(d[left].date))) ; // ������� ��� next ������� �� ��� bis.

    if (next < 0 || next > size) //�� ������� ����� �����
            return 0;

    //������� ��� ���������� ��������� �� ��� ���� �������� ���������.
            if (next > size - 10)
            {
                for (k = (size - 10); k < size; k++)
                {
                    if (key == datetonumber(d[k].date))
                        {
                            *x1 = d[k].value;
                            *x2 = d[k].cumulative;
                            return 1;
                        }
                }
                return 0;
            }

    while ( key != datetonumber(d[next].date)) //��� ��� ���� ������ � ����������.
    {

        i = 0;
        size2 = right - left + 1; //����� ��� �� �������.

            if (size2 <= 3) //�� size < 3 ��������� ���������.
            {
            for (k = left; k <= right; k++)
                {
                    if (key == datetonumber(d[k].date)) //�� ������ � ���������� ������������ ���� �������.
                        {
                            *x1 = d[k].value;
                            *x2 = d[k].cumulative;
                            return 1;
                        }
                }
                return 0;
            }

            if (key >= datetonumber(d[next].date)){  //�� � ���������� ��� ������ ����� ���������� ��� ����������� ��� next.
                while (key > datetonumber(d[next + i * (int)sqrt(size2) - 1].date)) //'��� ������ � ���������, ��������� �� i.
                 {
                    i = i + 1;
                 }
                right = next + i*sqrt(size2); //��������� ��� right ��� left.
                left = next + (i-1)* sqrt(size2);

            }

            else if  (key < datetonumber(d[next].date)) //�� � ���������� ��� ������ ����� ��������� ��� ����������� ��� next.
            {
                while (key < datetonumber(d[next - i * (int)sqrt(size2) + 1].date)) //'��� ������ � ���������, ��������� �� i.
                    {
                    i = i + 1;
                    }

                right = next - (i-1)*sqrt(size2); //��������� ��� right ��� left.
                left = next - i*sqrt(size2);
            }

            next = left + ceil((right - left + 1)*(key - datetonumber(d[left].date))/(datetonumber(d[right].date) - datetonumber(d[left].date))) - 1; //������������ �� ��� ������ next.
    }

    if(key == datetonumber(d[next].date)) //�� ������ � ����������, ������������ ���� ������� ��� �� value ��� cumulative.
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
    const char s[2] = "\t"; //����������� �� tab �� delimiter ��� �� ������� ��������� �� ���� ����������� ������� ��� ���� �������

    fp = fopen("F:\\C_project\\testfiletxt.txt", "r"); //��������. To pathway ������ �� �������� ��� ����������� �������.

if (fp == NULL)
    {
        printf("Error: could not open file");
        exit(0);
    }


    const unsigned MAX_LENGTH = 512;
    char buffer[MAX_LENGTH];    // �������� ������� �� ������� 512 bytes

    fgets(buffer, MAX_LENGTH, fp); // ��� ����� ���� � buffer ������� ��� ������������.
    //printf("%s", buffer);

    while (fgets(buffer, MAX_LENGTH, fp) && i < size)
    {

    string = strtok(buffer, s); //����� ���� ��� buffer
    strcpy(pinakas[i].direction,string); //���������� direction

    string = strtok(NULL, s);   //������� ���� ��� buffer
    pinakas[i].year = atoi(string); //���������� year

    string = strtok(NULL, s); //����� ���� ��� buffer
    strcpy(pinakas[i].date,string); //������ ��� ��� �� ��������.

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

    i = i + 1; //������ �������.
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

    // ��������� ��� strings �� integers ��� �� ����� �� �������� ��� �����������. ��������: � strcmp ��� �������� ��� �����������
    sscanf(date1, "%d/%d/%d", &d1, &m1, &y1);
    sscanf(date2, "%d/%d/%d", &d2, &m2, &y2);

    if (y1 > y2) {
        return 1;
    } else if (y1 < y2) {
        return -1;
    } else if (m1 > m2) { // �� y1 = y2 ���� ������ ���� �����
        return 1;
    } else if (m1 < m2) {
        return -1;
    } else if (d1 > d2) { // �� m1 = m2 ���� ������ ��� ������
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

int partition(struct Data d[], int left, int right) { //Quicksort �� ���� ��� �����������.

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

void quickSort(struct Data d[], int left, int right) {// ��������� ��� �� ������������� ��� Quick Sort
    int pi;
    if (left < right) {
        pi = partition(d, left, right);
        quickSort(d, left, pi - 1);
        quickSort(d, pi + 1, right);
    }
}

int main()
{
    int n,z;

    struct Data pinakas[size]; //������� struct

    n = sizeof(pinakas)/sizeof(pinakas[0]); // ������� n ������� �� ��� ������ ��� Quick sort

    readfile(pinakas); //�������� ������� ��� ����������� ��� struct

    quickSort(pinakas, 0, n-1); //���������� �� ���� ��� �����������.

    printdata(pinakas); //�������� ������������� struct

    long int x1,x2; //�� x1 ��� x2 �� ���������������� ��� �������� ���� ��������� ��� �������� ����������
                    //��� �� ����� �� ������� ���� �� ������������ �� ���������� ����� �� ����.
    int d,m,y;

    printf("Enter a date in the following format.(example: 10/10/2010). \nFirst enter the day:");
    scanf("%d", &d);
    printf("please enter the month: ");
    scanf("%d", &m);
    printf("please enter the year: ");
    scanf("%d", &y);

    while(d<1 || d>31 || m<1 || m>12 || y<2000 || y>2023 || (d>28 && m==2) || ((m == 4 || m == 6 || m==9 || m == 11) && d ==31)) //��������� ��������������� ��� ��� �����������.
    {
    printf("incorrect date. Please try again. Enter day.");
    scanf("%d", &d);
    printf("please enter the month: ");
    scanf("%d", &m);
    printf("please enter the year: ");
    scanf("%d", &y);
    }

    char date[20];
    snprintf(date, 20, "%d/%d/%d", d,m,y);
    printf("The date you entered is: %s \n", date);

    z = binterpolsearch(pinakas, 0, size-1, date, &x1, &x2);


    if (z==0)
        printf("Date not found. \n");
    if (z==1)
        printf("Date found. The value and cumulative respectively are: %ld $ and %ld $\n", x1,x2);

    return 0;
}

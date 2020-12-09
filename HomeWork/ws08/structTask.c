#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

typedef struct Rectangle{
    unsigned int xcoord;
    unsigned int ycoord;
    unsigned short width;
    unsigned short height;
}rectangle;
typedef struct Point{
    unsigned int xcoord;
    unsigned int ycoord;
    char name [10];
}point ;

bool equals_point(point *point1, point *point2){
    if((point1->xcoord == point2->xcoord) && (point1->ycoord == point2->ycoord)){
        return true;
    }else return false;
}
bool equals_rect( rectangle *rec1, rectangle *rec2 ){
    if(memcmp(rec1,rec2,sizeof(rectangle)) == 0){
        return true;
    }else return false;
}
bool inbetween(int num1, int num2){
if(num1>num2 && num2 <num1){
    return true;
}else return false;
}
bool contains(rectangle *rec, point *point){
    if(inbetween(rec->xcoord , point->xcoord) && inbetween(rec->ycoord, point->ycoord)){
        return true;
    }else return false;
}
int generate(int number,int seed){
    int lower = 0, upper = 100;
    srand(seed);
    int num=(number % (upper - lower + 1))+lower;
    return num;
}
int main  (int argc, char** argv)
{
   
    point *points = (point*)malloc(sizeof(point)*1000);
    //generate a random location
    int randx = generate(rand(),time(0));
    int randy = generate(rand(),time(0));
    //generate a rectangle of 50x50 
    rectangle *rec1=(rectangle*)malloc(sizeof(rectangle));;
    rec1->height = 50,
    rec1->width = 50,
    rec1->xcoord = randx,
    rec1->ycoord = randy;

    printf("Generated a rectangle 50x50\n");
    printf("%u\n",rec1->height);
    printf("%u\n",rec1->width);
    printf("%u\n",rec1->xcoord);
    printf("%u\n",rec1->ycoord);
    printf("----------------\n");

    //count number of rectangles
    int rectangleCount = 0;
    int const numOfPoints = 1000;
    //Generate 1,000 points
    for (int i = 0; i < numOfPoints; i++)
    {
        int x = generate(rand(),i+rand());
        int y = generate(rand(),i+rand());
        point *p= (point*)malloc(sizeof(point));
        char name [20]= "point";
        char str[20];
        
        sprintf(str,"%d",i);
        strcat(name,str);
        strcpy(p->name, name);
        p->xcoord = x;
        p->ycoord = y;
        
        *(points+i) = *p;
        printf("%s",name);
        printf("|%d |",x);
        printf("%d \n",y);
        //check if it is in a rectangle
        bool areEqual = contains(rec1,p);
        if(areEqual){
            rectangleCount+=1;
        }
        
        free(p);
    }
    int percentage= (float)rectangleCount/numOfPoints * 100.0;
    printf("number of points in rectangle = %d\n",rectangleCount);
    printf("number of points in percentage = %d%%",percentage);
    free(points);
    free(rec1);
    return 0;
}

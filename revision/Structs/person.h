#ifndef PERSONS_INCLUDED
#define PERSONS_INCLUDED

#include <stdio.h>

struct Persons {
   char  name[50];
   char  surname[50];
   char  address[100];
   int   age;
};

void print_person( struct Persons *p ) {
    printf("%s is %d yrs old\n", p->name, p->age);
}

#endif
#include <string.h>
#include <stdlib.h>
#include "person.h"

int main(int argc, char** argv) {
    struct Persons *p1 = malloc(sizeof(struct Persons));

    if (p1 == NULL) {
        return -1;
    }
    else {
        strcpy(p1->name, "John");
        p1->age = 36;

        print_person(p1);

        free(p1);
        return 0;
    }
}
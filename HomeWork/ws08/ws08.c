#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef struct Persons
{
    char name[50];
    char surname[50];
    char address[100];
    int  yob;
    struct Persons* next;
} person;

/* Head of the list. NULL means empty list (start)*/
person *list_head = NULL;

/* Retrieves the list head record or NULL if list is empty */
person* get_list_head() {
    return list_head;
}
/* Calculate the total number of persons in the list */
int count_list_size() {
    int total = 0;
    person* head = get_list_head();

    if(head == NULL){
        return total;
    }
    else{
        while(head != NULL){
            total++;
            head = head->next;
        }
    }
    return total;
}
/* Set first element of the list */
void set_list_head(person* new_head) {
    list_head = new_head;
}

//     _               _      
//    | |__   __ _ ___(_) ___ 
//    | '_ \ / _` / __| |/ __|
//    | |_) | (_| \__ \ | (__ 
//    |_.__/ \__,_|___/_|\___|

/* Finds the last allocation entry in list, or returns NULL if list is empty */
person* find_last() {
    person* current = get_list_head();
    bool endOfList = false;
    if(current ==NULL){
        return NULL;
    }else{
        while(endOfList == false){
            if(current->next == NULL){
                endOfList = true;
                return current;
            }else current = current->next;
        }
    }
    return current;
}

/* Add a person to the end of the list */
void add_person_at_end(person* new_person) {
    person* current = new_person;
    
    if(get_list_head() == NULL){
        set_list_head(current);
    }else{
        person *tail=find_last();
        tail->next = current;
    }
}

/* Prints a string representation of a person */
void print_person(person* p) {
    printf("Name = %s, Surname = %s, Address = %s, YOB = %d\n",p->name,p->surname,p->address,p->yob);
}

/* Returns a pointer to the person at the specified index or NULL if nothing is there */
person* get_person_at_index(int index) {
    person* head = get_list_head();
    for(int i=0; i < count_list_size(); i++){
        if(i==index) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

/* Remove a person from list and free memory. Return true if successful */
bool remove_person(person* p) {
    person* temp;
    temp = p->next;
    p->next = temp->next;
    free(temp);
    return true;
}

/* Remove the person at index from list and free memory. Return true if successful */
bool remove_person_at_index(int index) {
    if(remove_person(get_person_at_index(index-1))){
        return true;
    }
    return false;
}

//Prints the List
void printList(){
    person* p = get_list_head();
    for(size_t i=0;i<count_list_size();i++){
        print_person(p);
        p = p->next;
    }
}

/* Frees all the persons in the list */
void free_list() {
   person* head = get_list_head(),*temp;
    while(head != NULL){
        temp = head;
        free(temp);
        head = head->next;
    }
}



//               _                               _ 
//      __ _  __| |_   ____ _ _ __   ___ ___  __| |
//     / _` |/ _` \ \ / / _` | '_ \ / __/ _ \/ _` |
//    | (_| | (_| |\ V / (_| | | | | (_|  __/ (_| |
//     \__,_|\__,_| \_/ \__,_|_| |_|\___\___|\__,_|

/* Add a person to the list at a specific index */
void add_person_at_index(person* new_person, int index) {
    person* previous = get_person_at_index(index-1);
   // person* temp = NULL;
    size_t length = count_list_size();
    // size_t count = 0;
    if(index <= length-1){// check if index is valid

        if(index == count_list_size()-1){ // then its in the end
            add_person_at_end(new_person);
        }else
        {
            new_person->next = previous->next;
            previous->next = new_person;
        }
    }
}

/* Search the list for a particular name, return NULL if not found */
person* search_by_name(const char* name) {
    person* head = get_list_head();

    while(head!=NULL){
        if(strcmp(head->name,name) == 0 ){
            return head;
        }
        head = head->next;
    }
    return NULL;
}

//     _            _   _             
//    | |_ ___  ___| |_(_)_ __   __ _ 
//    | __/ _ \/ __| __| | '_ \ / _` |
//    | ||  __/\__ \ |_| | | | | (_| |
//     \__\___||___/\__|_|_| |_|\__, |
//                              |___/ 

/* Generate an amount of identical persons - for testing purposes */
void generate_persons(int amount) {
    
    for (int i=0; i < amount; i++)
    {
        person* p = (person*)malloc(sizeof(person));
        sprintf (p->name, "John %d", i);
        strcpy(p->surname, "Doe");
        strcpy(p->address, "unknown");
        p->yob = 2000+i;
        add_person_at_end(p);
        printf("Index %d address is = %p\n",i,&p);
        print_person(p);
        printf("\n");
    }
}

int main (int argc, char *argv[]) {
    
//confirm that the list is empty when we start
    assert (count_list_size() == 0);

//generate 10 persons and check that the count is 10
    generate_persons(10);
    assert (count_list_size() == 10);

//confirm that there is a person at index 0 and 9
    assert (get_person_at_index(0) != NULL);
    assert (get_person_at_index(9) != NULL);

//get the last person (index 9) and confirm yob
    person *last = get_person_at_index(9);
    assert (last->yob == 2009);

//search list by name
    person* fetched = search_by_name("John 2");
    assert (strcmp(fetched->name,"John 2") == 0);

//Add Person @ index
    person* p = (person*)malloc(sizeof(person));
    sprintf (p->name, "Johnny");
    strcpy(p->surname, "Doe");
    strcpy(p->address, "unknown");
    p->yob = 2018;
    add_person_at_index(p,8);
    printList();
    printf("\n");
    assert(get_person_at_index(8)!=NULL);

//Remove Person
   assert(count_list_size() == 11);
   remove_person(get_person_at_index(7));
   assert(strcmp(get_person_at_index(8)->name, "Johnny")!=0);
   assert(count_list_size() == 10);
   printList();

//remove last person and confirm it was successful
    assert (remove_person_at_index(9) == true);
    assert (count_list_size() == 9);

//free list and confirm count is zero
    free_list();
    printList();
    assert (count_list_size() == 0);
    
    
    return 0;
}

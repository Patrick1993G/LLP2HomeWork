
#include <time.h>
typedef struct SENSORS
{
    int MOISTURE;
    int PH;
    int SUNLIGHT;
    char STATS[200];
    struct SENSORS* next;
} sensor;

int generate(int number,int lower, int upper){
    srand(time(0));
    int num=(number % (upper - lower + 1))+lower;
    return num;
}

/* Head of the list. NULL means empty list (start)*/
sensor *list_head = NULL;

/* Retrieves the list head record or NULL if list is empty */
sensor* get_list_head() {
    return list_head;
}
/* Calculate the total number of SENSORS in the list */
int count_list_size() {
    int total = 0;
    sensor* head = get_list_head();

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
void set_list_head(sensor* new_head) {
    list_head = new_head;
}

/* Finds the last allocation entry in list, or returns NULL if list is empty */
sensor* find_last() {
    sensor* current = get_list_head();
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

/* Add a sensor to the end of the list */
void add_sensor_at_end(sensor* new_sensor) {
    sensor* current = new_sensor;
    
    if(get_list_head() == NULL){
        set_list_head(current);
    }else{
        sensor *tail=find_last();
        tail->next = current;
    }
}

// /* Prints a string representation of a sensor */
// void print_person(sensor* s) {
//     printf("Name = %s, Surname = %s, Address = %s, YOB = %d\n",s->name,s->surname,s->address,s->yob);
// }

/* Returns a pointer to the sensor at the specified index or NULL if nothing is there */
sensor* get_sensor_at_index(int index) {
    sensor* head = get_list_head();
    for(int i=0; i < count_list_size(); i++){
        if(i==index) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

/* Remove a sensor from list and free memory. Return true if successful */
bool remove_sensor(sensor* s) {
    sensor* temp;
    temp = s->next;
    s->next = temp->next;
    free(temp);
    return true;
}

/* Remove the sensor at index from list and free memory. Return true if successful */
bool remove_sensor_at_index(int index) {
    if(remove_sensor(get_sensor_at_index(index-1))){
        return true;
    }
    return false;
}

// //Prints the List
// void printList(){
//     sensor* s = get_list_head();
//     for(size_t i=0;i<count_list_size();i++){
//         print_person(s);
//         s = s->next;
//     }
// }

/* Frees all the SENSORS in the list */
void free_list() {
   sensor* head = get_list_head();
    while(head != NULL){
        sensor* temp = head;
        head = head->next;
        free(temp);
    }
    set_list_head(NULL);
}

/* Add a sensor to the list at a specific index */
void add_sensor_at_index(sensor* new_sensor, int index) {
    sensor* previous = get_sensor_at_index(index-1);
    
    size_t length = count_list_size();
    
    if(index <= length-1){// check if index is valid

        if(index == count_list_size()-1){ // then its in the end
            add_sensor_at_end(new_sensor);
        }else
        {
            new_sensor->next = previous->next;
            previous->next = new_sensor;
        }
    }
}

// /* Search the list for a particular name, return NULL if not found */
// sensor* search_by_name(const char* name) {
//     sensor* head = get_list_head();

//     while(head!=NULL){
//         if(strcmp(head->name,name) == 0 ){
//             return head;
//         }
//         head = head->next;
//     }
//     return NULL;
// }
#ifndef COLLECTION_H_INCLUDED
#define COLLECTION_H_INCLUDED

#include <time.h>
typedef struct SENSORS
{
    unsigned long ID;
    unsigned int MOISTURE;
    unsigned int PH;
    unsigned int SUNLIGHT;
   // struct SENSORS* next;
} sensor;

typedef struct SENSORDATA
{
    unsigned int MOISTURE;
    float PH;
    unsigned int SUNLIGHT;
} sensorData;


int generate(int lower, int upper)
{
    srand((unsigned)time(NULL));
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

float generateFloat(float lower, float upper)
{
    float num = ((float)rand() / RAND_MAX) * (float)(upper);
    return num;
}

sensorData generateData()
{
    sensorData data;
    data.MOISTURE = generate(1, 10);
    data.PH = generateFloat(1.0f, 10.0f);
    data.SUNLIGHT = generate(0, 2000);
    return data;
}
sensor *initialiseCollection()
{
    sensor *recSensData = (sensor *)malloc(sizeof(sensor));
    if(recSensData != NULL){
        recSensData->ID =0;
        recSensData->MOISTURE = 0;
        recSensData->PH = 0;
        recSensData->SUNLIGHT = 0;
       // recSensData->next=NULL;
    }
    
    return recSensData;
}


// /* Head of the list. NULL means empty list (start)*/
// sensor *list_head = NULL;

// /* Retrieves the list head record or NULL if list is empty */
// sensor* get_list_head() {
//     return list_head;
// }
// /* Calculate the total number of SENSORS in the list */
// int count_list_size() {
//     int total = 0;
//     sensor* head = get_list_head();

//     if(head == NULL){
//         return total;
//     }
//     else{
//         while(head != NULL){
//             total++;
//             head = head->next;
//         }
//     }
//     return total;
// }
// /* Set first element of the list */
// void set_list_head(sensor* new_head) {
//     list_head = new_head;
// }

// /* Finds the last allocation entry in list, or returns NULL if list is empty */
// sensor* find_last() {
//     sensor* current = get_list_head();
//     bool endOfList = false;
//     if(current ==NULL){
//         return NULL;
//     }else{
//         while(endOfList == false){
//             if(current->next == NULL){
//                 endOfList = true;
//                 return current;
//             }else current = current->next;
//         }
//     }
//     return current;
// }

// /* Add a sensor to the end of the list */
// void add_sensor_at_end(sensor* new_sensor) {
//     sensor* current = new_sensor;
    
//     if(get_list_head() == NULL){
//         set_list_head(current);
//     }else{
//         sensor *tail=find_last();
//         tail->next = current;
//     }
// }

// /* Prints a string representation of stats */
// // void getStats(sensor* s) {
     
// //     printf("PH = %d, MOISTURE = %d, SUNLIGHT = %d \n",s->PH,s->MOISTURE,s->SUNLIGHT);
// // }
// //Prints the List
// // void printList(){
// //     sensor* s = get_list_head();
// //     for(size_t i=0;i<count_list_size();i++){
// //         getStats(s);
// //         s = s->next;
// //     }
// // }
// /* Returns a pointer to the sensor at the specified index or NULL if nothing is there */
// sensor* get_sensor_at_index(int index) {
//     sensor* head = get_list_head();
//     for(int i=0; i < count_list_size(); i++){
//         if(i==index) {
//             return head;
//         }
//         head = head->next;
//     }
//     return NULL;
// }

// /* Remove a sensor from list and free memory. Return true if successful */
// bool remove_sensor(sensor* s) {
//     sensor* temp;
//     temp = s->next;
//     s->next = temp->next;
//     free(temp);
//     return true;
// }

// /* Remove the sensor at index from list and free memory. Return true if successful */
// // bool remove_sensor_at_index(int index) {
// //     if(remove_sensor(get_sensor_at_index(index-1))){
// //         return true;
// //     }
// //     return false;
// // }

// /* Frees all the SENSORS in the list */
// void free_list() {
//    sensor* head = get_list_head();
//     while(head != NULL){
//         sensor* temp = head;
//         head = head->next;
//         free(temp);
//     }
//     set_list_head(NULL);
// }

// /* Add a sensor to the list at a specific index */
// // void add_sensor_at_index(sensor* new_sensor, int index) {
// //     sensor* previous = get_sensor_at_index(index-1);
    
// //     size_t length = count_list_size();
    
// //     if(index <= length-1){// check if index is valid

// //         if(index == count_list_size()-1){ // then its in the end
// //             add_sensor_at_end(new_sensor);
// //         }else
// //         {
// //             new_sensor->next = previous->next;
// //             previous->next = new_sensor;
// //         }
// //     }
// // }

#endif
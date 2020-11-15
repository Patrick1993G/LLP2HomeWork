#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
int count(char text[]){
    int count=1;
    for (size_t i = 0; i < strlen(text); i++)
    {
        if(*(text+i)== ' '){
            count++;
        }else{
            continue;
        }
    }
    
    return count;
}
int countLetters(char text[]){
  int count=0;
    for (size_t i = 0; i < strlen(text); i++)
    {
        if(*(text+i)!= ' ' && *(text+i)!= '\n'){
            count++;
        }else{
            continue;
        }
    }
    
    return count;
}

bool should_count_chars(int argc, char* argv[]){
    if(*argv[1] == 'c'){
        return true;
    }else{
        return false;
    }
}

bool is_space(char ch){
    if(ch == ' '){
        return true;
    }else{
        return false;
    }
}
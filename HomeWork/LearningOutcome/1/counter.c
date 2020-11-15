


#include "lo1.h"

int main(int argc, char const *argv[])
{
    char*String = (char*)malloc(sizeof(char)*99);
    printf("Enter text: ");
    scanf("%[^\n]",String);
    int words=count(String);
    #if defined(c)
        int letters=countLetters(String);
        printf("%d words (%d characters)",words,letters);
    #else 
        printf("%d words",words);  
    #endif // MACRO
    
    return -1;
}
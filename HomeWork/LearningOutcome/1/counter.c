


#include "lo1.h"

int main(int argc, char const *argv[])
{
    char*String = (char*)malloc(sizeof(char)*99);
    int words=0;
    #if defined(EXTENDED)
    do   
        {
            printf("Enter text: ");
            scanf("%[^\n]",String);
            words+=count(String);
        }while(strcmp(String,""))
    #endif

    #if defined (NORMAL)
        printf("Enter text: ");
        scanf("%[^\n]",String);
        words+=count(String);
    #endif
        #if defined(c)
            
            int letters=countLetters(String);
            printf("%d words (%d characters)",words,letters);
        #else
        printf("%d words", words);  

        #endif // MACRO
    
    return -1;
}
#include<stdio.h>


int func4(int v1, int v2, int v3){
    int eax = v3 - v2;
    int ecx = ((unsigned int)eax)>>31;
    eax = (eax + ecx) >> 1;
    ecx = eax + v2;
    if(ecx > v1){
        eax = func4(v1, v2, ecx - 1);
        eax *= 2;
    }
    else{
        eax = 0;
        if(ecx < v1){
            eax = func4(v1, ecx+1, v3);
            eax = 2*eax + 1;
        }
    } 
    return eax;
}

int main(){

    // phase_5
    char str1[] = "flyers";
    char str2[] = "maduiersnfotvbyl" ;
    for(char *p1 = str1; *p1 != '\0'; ++p1)
        for(char *p2 = str2; *p2 != '\0'; ++p2){
            int index;

            if(*p1 == *p2){
                index = p2 - str2;
                printf("index = %d: ", index);
                for(char ch = 'a'; ch <= 'z'; ++ch){
                    if((ch&0x0F) == index)    printf("%c ", ch);
                }
                for(char ch = 'A'; ch <= 'Z'; ++ch){
                    if((ch&0x0F) == index)    printf("%c ", ch);
                } 
                printf("\n");
            }
        }
    
    // phase_4
    int input;
    while(scanf("%d", &input)){
        printf("output: %d\n", func4(input, 0, 14));
    }
    return 0;
}
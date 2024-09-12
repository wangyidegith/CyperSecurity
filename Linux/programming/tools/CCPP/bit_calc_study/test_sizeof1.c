#include <stdio.h>                                                           
                                                                             
int main() {                                                                 
    size_t size = sizeof(1) * 8;
                                                                             
    printf("Number 1 is represented using %lu bits on this system.\n", size);
                                                                             
    return 0;                                                                
}                                                                            

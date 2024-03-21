
#include <stdio.h>
#include "../module/inc/makelib.h"


int main(){

    int a,b;
    scanf("%d %d",&a, &b);

    int c = add(a,b);
    printf("add method c = %d", c);

    int d = mul(a,b);
    printf("mul method d = %d", d);

    return 0;
}
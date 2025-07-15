
#include "b.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    double a = 81.0;
    double b = 0.0;

    printf("input a value is :%f\n", a);
    b = cal_sqrt(a);
    printf("call lib sqrt result:%f\n", b);
    
    return 0;
}

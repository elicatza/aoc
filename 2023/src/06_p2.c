#include <stdio.h>
#include <math.h>

#include "./core.h"


int main(void)
{
    f64 time = 40829166;
    f64 dist = 277133813491063;

    f64 max = (-time - sqrt(time*time - (4 * -1 * -dist))) / 2 * -1;
    f64 min = (-time + sqrt(time*time - (4 * -1 * -dist))) / 2 * -1;

    printf("Solution: %.0f\n", ceil(max) - ceil(min));
    return 0;
}

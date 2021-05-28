#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main() {
    int i = 0, points_in = 0;
    double x,y, points, points_seed;
    clock_t timer;

    srand(time(0));//Random seed

    printf("How many points will be sorted: 10^");
    scanf("%lf", &points_seed);

    points = pow(10, points_seed);
    
    timer = clock(); //start time count
    for( i = 0; i < points; i++) {//Loop where the point will be sorted
        x = (double)rand() * ( 1.0 - 0.0 ) / (double)RAND_MAX + 0.0;//x coordinate
        y = (double)rand() * ( 1.0 - 0.0 ) / (double)RAND_MAX + 0.0;//y coordinate
        
        if(x * x + y * y < 1) {//The point is in the circle
            points_in++;
        }
    }

    printf("%d\n", points_in);//Numbers of points inside the circle

    double pi = 4.0 * points_in/points;//Pi
    timer = clock() - timer; // finish time count
    printf("%f\n", pi);
	printf("Time duration: %lf milliseconds.\n", ((double)timer)/((CLOCKS_PER_SEC/1000)));

    return 0;
}
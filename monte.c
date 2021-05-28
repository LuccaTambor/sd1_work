#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define BILLION  1000000000.0

int main() {
    int i = 0, points_in = 0;
    double x,y, points, points_seed;
    struct timespec start, end;

    srand(time(0));//Random seed

    printf("How many points will be sorted: 10^");
    scanf("%lf", &points_seed);

    points = pow(10, points_seed);
    clock_gettime(CLOCK_REALTIME, &start);
    for( i = 0; i < points; i++) {//Loop where the point will be sorted
        x = (double)rand() * ( 1.0 - 0.0 ) / (double)RAND_MAX + 0.0;//x coordinate
        y = (double)rand() * ( 1.0 - 0.0 ) / (double)RAND_MAX + 0.0;//y coordinate
        
        if(x * x + y * y < 1) {//The point is in the circle
            points_in++;
        }
    }

    //printf("%d\n", points_in);//Numbers of points inside the circle
    clock_gettime(CLOCK_REALTIME, &end);
	long double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/BILLION;
    double pi = 4.0 * points_in/points;//Pi
    printf("%f\n", pi);
    printf("Time duration: %Lf seconds.\n", time_spent);

    return 0;
}
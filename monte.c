#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int num = 0, i = 0, acertos = 0;
    double x,y;

    srand(time(0));//Gerando seed de aleatoriedade

    num = (rand() % (5000000 - 4550000 + 1)) + 4550000;//número de pontos que serão posicionados

    //printf("%d\n", num);//Número de pontos
    
    for( i = 0; i < num; i++) {//Loop onde os pontos serão colocados no quadrante
        x = (double)rand() * ( 1.0 - 0.0 ) / (double)RAND_MAX + 0.0;//Coordenada x do ponto
        y = (double)rand() * ( 1.0 - 0.0 ) / (double)RAND_MAX + 0.0;//Coordenada y do ponto
        
        if(x * x + y * y < 1) {//O ponto se econtra dentro da circunferência
            acertos++;
        }
    }

    printf("%d\n", acertos);//Número de pontos dentro da circunferência

    double pi = 4.0 * acertos/num;//Calculanddo pi

    printf("%f\n", pi);

    return 0;
}
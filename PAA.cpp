#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iomanip>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <math.h>
#include <vector>
#include "sys/time.h"

using namespace std;
#define INF 0x3f3f3f3f

// ALUNO: PEDRO HENRIQUE REIS RODRIGUES
// MATRÍCULA: 668443
// PROFESSOR: SILVIO GUIMARAES
// TRABALHO: CLOSEST PAIR OF POINTS 
// Complexidade O(n log n) em que n representa o número de pontos a serem analisados

// Métodos utilizados: DIVISÃO E CONQUISTA e FORÇA BRUTA

typedef struct Ponto{
    int x;
    int y;
} Ponto;

int compararX (const void *x, const void *y) {
    Ponto *a = (Ponto*)x;
    Ponto *b = (Ponto*)y;
    return (a->x != b->x) ? (a->x - b->x) : (a->y - b->y);
}

int compararY (const void *x, const void *y) {
    Ponto *a = (Ponto*)x;
    Ponto *b = (Ponto*)y;
    return (a->y != b->y) ? (a->y - b->y) : (a->y - b->y);
}

float distancia(Ponto X, Ponto Y){
    return sqrt((pow(X.x - Y.x,2)+pow(X.y-Y.y,2)));
}
/*
    Algoritmo de força bruta, simplesmente passa por cada ponto e analisa
    com o próximo ponto de forma sequencial, sempre armazenando a minima
    distancia, e retornando no final
*/
float forca_bruta(Ponto P[],int N){

    float min = INF;
    for(int i = 0 ; i < N ; i++){
        for(int j= i+1; j< N; j++){
            if(distancia(P[i],P[j]) < min){
                min = distancia(P[i],P[j]);
            }
        }
    }
    return min;
}// O(n^2)

/*
    Método de divisão e conquista, será realizado várias divisões
    até que N seja igual ou menor que 3, sendo assim faremos a força
    bruta para encontrar a menor distancia, dividindo assim o problema maior
    em problemas menores, e no final somaremos os resultado e forneceremos
    a combinação ideal, retornando a distância minima entre os pontos
    no plano
*/
float divisao_conquista(Ponto P[], int N){

    if (N<=3){
        return forca_bruta(P,N);
    }

    int meio = N/2;
    Ponto Pm = P[meio];

    float distE = divisao_conquista(P,meio); // metodo de divisao do lado para esquerda
    float distD = divisao_conquista(P+meio,N-meio); // metodo de divisao do lado para direita
    float dist;
    // armazena a menor distancia em dist
    if(distE <= distD){
        dist = distE;
    }
    else{
        dist = distD;
    }
    
    //metodo para encontrar pontos proximos
    //as fronteiras (strips)

    Ponto strip[N];
    int j = 0;

    for(int i = 0; i< N; i++){
        if(abs(P[i].x - Pm.x) < dist){ // adiciona apenas os pontos que estão dentro do parametro da distancia minima
            strip[j]=P[i];
            j++;
        }
    }

    float min = dist;
    qsort(strip,j,sizeof(Ponto),compararY);

    // resolução do retângulo de 8 lados
    // aparentemente parece ser O(n^2), porém é O(n)
    // a ideia principal é melhor explicada no arquivo teX
    for(int i = 0; i< j;i++){
        for(int k = i+1; k< j && (strip[k].y-strip[i].y)<min ; k++){
            if(distancia(strip[i],strip[k])<min){
                min = distancia(strip[i],strip[k]);
            }
        }
    }
    
    return min;

} // O(n log^2 n)

/*
    Preparação de variáveis para ser utilizada no método de divisão
    e conquista.
*/
float div_conquista(Ponto P[], int N){

    qsort(P,N,sizeof(Ponto), compararX);
    return divisao_conquista(P,N);

}


/*
    Método de divisão e conquista, será realizado várias divisões
    até que N seja igual ou menor que 3, sendo assim faremos a força
    bruta para encontrar a menor distancia, dividindo assim o problema maior
    em problemas menores, e no final somaremos os resultado e forneceremos
    a combinação ideal, retornando a distância minima entre os pontos
    no plano
*/
float divisao_conquista_apri(Ponto Px[], Ponto Py[], int N){

    if(N <= 3){
        return forca_bruta(Px,N);
    }

    int meio = N/2;
    
    Ponto Pm = Px[meio];
    Ponto Pye[meio]; // Pontos na esquerda, com os pontos ordenados por y.
    Ponto Pyd[N-meio]; // Pontos na direita, com os pontos ordenados por y.
    int aux = 0, aux1 = 0; // auxiliares para indexar as subarrays da esquerda e direita
    for(int i = 0 ; i < N ; i++){
        if(Py[i].x < Pm.x || (Py[i].x == Pm.x && Py[i].y == Pm.y) && aux < meio){
            Pye[aux++] = Py[i];
        } 
        else{
            Pyd[aux1++]= Py[i];
        }
    }
    float distE = divisao_conquista_apri(Px,Pye,meio); // metodo de divisao do lado para esquerda
    float distD = divisao_conquista_apri(Px + meio, Pyd, N-meio); // metodo de divisao do lado para direita
    float dist;
    // armazena a menor distancia em dist
    if(distE <= distD){
        dist = distE;
    }
    else{
        dist = distD;
    }

    //metodo para encontrar pontos proximos
    //as fronteiras (strips)

    Ponto strip[N];
    int j = 0;
    for(int i = 0 ; i< N ; i++){
        if(abs(Py[i].x - Pm.x)< dist){ // adiciona apenas os pontos que estão dentro do parametro da distancia minima
            strip[j] = Py[i];
            j++;
        }
    }

    // diferentemente do metodo padrao, veja que nao foi necessario reordenar a strip, já que o Py já está ordenado

    // resolução do retângulo de 8 lados
    // aparentemente parece ser O(n^2), porém é O(n)
    // a ideia principal é melhor explicada no arquivo teX

    float min = dist;
    for(int i = 0 ; i < j; i++){
        for(int k = i+1; k < j && (strip[k].y - strip[i].y) <min ; k++){
            if(distancia(strip[i],strip[k])<min){
                min = distancia(strip[i],strip[k]);
            }
        }
    }
    return min;
} // O(n log n)


/*
    Preparação de variáveis para ser utilizada no método de divisão
    e conquista aprimorado.
*/
float div_conquista_apri(Ponto P[], int N){
    Ponto Px[N];
    Ponto Py[N];
    for(int i = 0; i< N ; i++){
        Px[i] = P[i];
        Py[i] = P[i];
    }
    qsort(Px,N,sizeof(Ponto), compararX);
    qsort(Py,N,sizeof(Ponto),compararY);
    return divisao_conquista_apri(Px,Py,N);
}

int main(){
    int N;
    // leitura dos pontos no arquivo pontos.txt
    FILE *fp;
    fp = fopen("pontos.txt","r");
    fscanf(fp,"%d\n",&N);// primeira linha, numero de pontos
    printf("%d",N);
    Ponto P[N];
    for(int i = 0 ; i< N; i++){
        fscanf(fp,"%d %d\n",&P[i].x,&P[i].y); // seguintes linhas, X e Y de cada ponto
    }
    fclose(fp);

    struct timeval start,end;
    double tempo;
    int choice;
    do{
        // interface para resoluçao do trabalho
        printf("\n\n\n\nResolver por:\n[1] O(n^2) - Forca Bruta\n[2] O(nlog^2n) - Divisao e Conquista\n[3] O(nlogn) - Divisao e Conquista otimizado\n[0] Encerrar o programa\n");
        scanf("%d",&choice);
        switch (choice){
            case 0:
                break;
            case 1:
                gettimeofday(&start,NULL); // calculo de tempo
                printf("Distancia: %f",forca_bruta(P,N));
                gettimeofday(&end, NULL); // calculo de tempo
                
                tempo = (end.tv_sec - start.tv_sec)*1e6;
                tempo = (tempo + (end.tv_usec-start.tv_usec))*1e-6;
                printf("\nTempo de execucao: %lf segundos",tempo);
                break;
            case 2:
                gettimeofday(&start,NULL); // calculo de tempo
                printf("Distancia: %f",div_conquista(P,N));
                gettimeofday(&end, NULL); // calculo de tempo
                
                tempo = (end.tv_sec - start.tv_sec)*1e6;
                tempo = (tempo + (end.tv_usec-start.tv_usec))*1e-6;
                printf("\nTempo de execucao: %lf segundos",tempo);
                break;
            case 3:
                gettimeofday(&start,NULL); // calculo de tempo
                printf("Distancia: %f",div_conquista_apri(P,N));
                gettimeofday(&end, NULL); // calculo de tempo
                
                tempo = (end.tv_sec - start.tv_sec)*1e6;
                tempo = (tempo + (end.tv_usec-start.tv_usec))*1e-6;
                printf("\nTempo de execucao: %lf segundos",tempo);
                break;
            default:
                printf("Erro na inicializacao");
        }
        
    }while(choice != 0);
    
    return 0;

}
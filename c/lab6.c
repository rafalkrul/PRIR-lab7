#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>
#define WJAZD 1
#define WYJAZD 3
#define POSTOJ 2



int stan = 1;
int tag = 1999;
int tablica[2];
int tablica2[2];
int postoj = 5;
int lmiejsc = 15;
int lmiejscMAX = 15;
MPI_Status status;
int num_proc;
int rank;    

void wyslij(int stan, int nr){
    tablica[0] = stan;
    tablica[1] = nr;
    MPI_Send(&tablica,2,MPI_INT,0,tag,MPI_COMM_WORLD);
    sleep(1);
}

void parking(num_proc){
    int nra;
    int stats;
    int pocz;
    int lsamochodow = num_proc-1;
    int loop = 1;
    printf("Parking otwarto! \n");
    while(lsamochodow != 0){
    MPI_Recv(&tablica2,2,MPI_INT,MPI_ANY_SOURCE,tag,MPI_COMM_WORLD, &status);
    stats = tablica2[0];
    nra = tablica2[1];

    if(stats == 1){
        printf("Samochod %d wjezdza na parking \n", nra);
        stats = 2;
        MPI_Send(&stats,1,MPI_INT,nra,tag,MPI_COMM_WORLD);        
        sleep(1);
    }
    if(stats == 2){
        MPI_Send(&stats,1,MPI_INT,nra,tag,MPI_COMM_WORLD);
    }
    if(stats == 3){
        sleep(1);
        printf("Samochod %d wyjezdza z parkingu \n", nra);
        MPI_Send(&stats,1,MPI_INT,nra,tag,MPI_COMM_WORLD);
        lsamochodow--;
         }
    }

    printf("Wszystkie samochody opuściły parking\n");
    
    
}

void samochod(){
    int loop = 1;

    while(loop == 1){
    if(stan == 1){
        wyslij(stan,rank);
        stan = POSTOJ;
    
    }
    else if(stan == 2){
        postoj-= rand()%2;
        if(postoj <= 0){
            stan = WYJAZD;
            wyslij(stan,rank);
            
        }else{
            stan = POSTOJ;
            wyslij(stan,rank);
            
        }        
    }else if(stan == 3){
        loop = 2;
        
    }
        
}
}


int main(int argc,char **argv){
    
    // Inicjuje MPI + pobieram ranking i liczbe procesów
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    srand(time(NULL));
if(rank == 0){
    parking(num_proc);
    
}else{
    samochod();
    
}
    MPI_Finalize();

    return 0;

}

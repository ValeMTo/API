/*
Progetto API Anno 2021
Valeria Amato
*/

/*Definizione tipo*/

typedef struct {
  unsigned short name;
  unsigned long sum_path; /*Check the size*/
} graph_t;


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define LEN 13 /*AggiungiGrafo sono 13 caratteri*/
#define GRAFO "AggiungiGrafo"
#define TOPK "TopK"


/* Prototipi */
unsigned long sum_path(unsigned short);
short is_best(graph_t *,unsigned short, unsigned short, unsigned long);
unsigned short trova_min_from(unsigned short, unsigned long *, unsigned long, unsigned short);
unsigned short trova_min(unsigned short, unsigned long *);

int main( int argc, char * argv[])
{
  unsigned short d, k;
  char comando[LEN+1];
  graph_t * best_graph;
  unsigned short pos, i;
  unsigned long size_path;

  scanf("%hu,%hu", &d, &k);
  printf("d=%hu, k=%hu\n", d, k);
  pos=0;
  if((best_graph = malloc(k*sizeof(graph_t)))){
    for(i=0; i<k; i++) /*Inizializzo vettore dei k migliori*/
      best_graph[i].name=-1; /*-1 valore di non validità*/
    scanf("%s\n", comando);
    while(!feof(stdin)){
      printf("Sono nel while:%s\n", comando); /*ELIMINA*/
      if(strcmp(comando, GRAFO) == 0){ /*Se le stringhe sono uguali*/
        //strcpy(comando, "");
        printf("Sono nell'if\n"); /*ELIMINA*/
        pos = pos+1;
        size_path = sum_path(d); /*Calcola la bontà del grafo*/
        printf("sono qui\n"); /*ELIMINA*/
        is_best(best_graph,k,pos,size_path);  /*Se opportuno, inserisce il grafo nei k migliori*/
        printf("finish is_best\n" ); /*ELIMINA*/
      }else if(strcmp(comando, TOPK)==0){
        //strcpy(comando, "");
        pos = 0;
        printf("TOPK\n"); /*ELIMINA*/
        for(i=0; i<k && best_graph[i].name!= -1; i++)
          printf("%hu ", best_graph[i].name);
        for(i=0; i<k; i++)  /*Svuoto l'array dei k migliori grafi*/
          best_graph[i].name=-1;
      }
      scanf("%s", comando);
    }
  } else
    printf("Allocazione non riuscita di %hu elementi", k);
  free(best_graph);
  printf("\n");
  return 0;
}

short is_best(graph_t * array, unsigned short len, unsigned short pos, unsigned long path)
{
  unsigned short i, indice_max;
  unsigned long max;

  max = array[0].sum_path;
  indice_max=0;
  for(i=0; i<len; i++){
    if(array[i].name == -1){
      array[i].name = pos;
      array[i].sum_path = path;
      break;
    }else{
      if(array[i].sum_path > max){
        max = array[i].sum_path;
        indice_max  = i;
      }
    }
  }

  if(path < max){
    array[indice_max].sum_path = max;
    array[indice_max].name = pos;
    return 1;
  }
  return 0;
}

unsigned long sum_path(unsigned short nodi) /*Fare una branch - IDEA: spostare il puntatore sull'input e prendere ciò che serve*/
{
  unsigned long dijkstra[nodi], min, somma;
  unsigned int matrice[nodi-1][nodi];
  unsigned short i,j, used, indice_min;


  if(nodi>1){
  /*Non ho bisogno di riutilizzare i percorsi da zero*/
    for(j=0; j<nodi-1; j++){
      scanf("%lu,", &dijkstra[j]);
    }
    scanf("%lu", &dijkstra[j]);

    for(i=1; i<nodi; i++){
      for(j=0; j<nodi-1; j++){
        scanf("%u,", &matrice[i][j]);
      }
      scanf("%u", &matrice[i][j]);
    }

    used= nodi-1;
    indice_min = trova_min(nodi, dijkstra);
    if(indice_min != -1){
      min = dijkstra[indice_min];
      while(used>0){
        for(i=1; i<nodi; i++){
          if((min + matrice[indice_min][i]<dijkstra[i] && matrice[indice_min][i] != 0) || dijkstra[i]==0)
            dijkstra[i] = min + matrice[indice_min][i];
        }
        indice_min = trova_min_from(nodi, dijkstra, min, indice_min);
        printf("DOPO DI TROVA MIN_from %hu\n", indice_min); /*ELIMINA*/
        used--;
      }
    }else
      return 0;

    somma = 0;
    for(i=1; i<nodi; i++)
      somma = somma + dijkstra[i];
    printf("Somma %lu", somma);
    return somma;

  }else
    return 0;
}

/*Trova il minimo a partire da un certo numero e dall'indice i=1 e restituisce l'indice del minimo*/
/*Nel contesto "GraphRanker" la soglia corrisponde alla parte già percorsa di dijkstra*/
/*soglia_indice serve per non riprendere lo stesso elemento*/
unsigned short trova_min_from(unsigned short len, unsigned long * array, unsigned long soglia, unsigned short soglia_indice)
{
  unsigned short i, indice_min;
  unsigned long min;

  for(i=0; i<len; i++){
    printf("%lu ", array[i]);
  }
  printf("\n");
  printf("soglia:= %lu\n", soglia);
  printf("soglia_indice:= %hu\n", soglia_indice);
  i=0;
  indice_min=-1;/*Verrà sempre sovrascritto perché l'array non sarà mai tutto nullo - Statement per rimuovere warning*/
  min = -1; /*Verrà sempre sovrascritto o non utilizzato - Statement per rimuovere warning*/
  while(i<len){
    if((((array[i]==soglia && i>soglia_indice) || array[i]>soglia)) && array[i] != 0){
      indice_min=i;
      min=array[i];
      break; /*È possibile usare break?*/
    }
    i++;
  }
  for(i=2; i<len; i++){
    if(array[i]<min && array[i] !=0){ /*Scelgo sempre il valore sopra la soglia con indice più piccolo*/
      if((array[i]==soglia && i>soglia_indice) || array[i]>soglia){
        indice_min=i;
        min=array[i];
      }
    }
  }
  printf("INDICE_MIN:= %hu\n", indice_min); /*ELIMINA*/
  return indice_min;

}
/*Trova il minimo a partire dall'indice i=1 e restituisce l'indice del minimo*/
unsigned short trova_min(unsigned short len, unsigned long * array)
{
  unsigned short i, indice_min;
  unsigned long min;

  /*Se resta -1, tutti gli elementi sono a zero, zero non raggiungibile*/
  indice_min = -1;
  for(i=1; i<len; i++){
    if(array[i] != 0){
      min = array[i];
      indice_min = i;
      break;
    }
  }
  i++; /*Aggiungo e sottraggo per eliminare il warning*/
  for(i=i-1; i<len; i++){
    if(array[i]<min && array[i] !=0){
      indice_min=i;
      min=array[i];
    }
  }
  return indice_min;
}

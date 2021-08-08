/*
Progetto API Anno 2021
Valeria Amato

Versione 2
*/

/*Definizione tipo*/

typedef struct {
  int name;
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
int trova_min_from(unsigned short, unsigned long *, unsigned long, unsigned short);
int trova_min(unsigned short, unsigned long *);
int compara(graph_t *, graph_t *);

int main( int argc, char * argv[])
{
  unsigned short d, k;
  char comando[LEN+1];
  graph_t * best_graph;
  unsigned short pos, i;
  unsigned long size_path;
  int return_value;

  return_value = scanf("%hu %hu", &d, &k);
  if(return_value != 0){
    pos=0;
    if((best_graph = malloc(k*sizeof(graph_t)))){
      for(i=0; i<k; i++) /*Inizializzo vettore dei k migliori*/
        best_graph[i].name=-1; /*-1 valore di non validità*/
      return_value = scanf("%s\n", comando);
      while(!feof(stdin)){
        if(return_value != 0){
          //printf("WHILE: %s %hu\n", comando, pos);
          if(strcmp(comando, GRAFO) == 0){ /*Se le stringhe sono uguali*/
            size_path = sum_path(d); /*Calcola la bontà del grafo*/
            //printf("sizepath finito\n");
            is_best(best_graph,k,pos,size_path);  /*Se opportuno, inserisce il grafo nei k migliori*/
            pos = pos+1;
            // for(i=0; i<k; i++)
            //   if(best_graph[i].name!= -1)
            //     printf("graph_name:%d ---- points: %lu\n", best_graph[i].name, best_graph[i].sum_path);or(i=0; i<k; i++)
          }else if(strcmp(comando, TOPK)==0){
            qsort(best_graph, k, sizeof(graph_t), compara);
            /*
            for(i=0; i<k && best_graph[i].name!= -1; i++)
              printf("%d ", best_graph[i].name);
            */
            for(i=0; i<k ; i++)
              if(best_graph[i].name!= -1){
                printf("%d ", best_graph[i].name);
              }
          }
          return_value = scanf("%s", comando);
        }else 
          printf("Scanf fallita");
      }
    } else
      printf("Allocazione non riuscita di %hu elementi", k);
    free(best_graph);
  }else
    printf("Scanf fallita");
  printf("\n");
  return 0;
}

short is_best(graph_t * array, unsigned short len, unsigned short pos, unsigned long path)
{
  unsigned short i, indice_max;
  unsigned long max;
  _Bool flag=0;

  if(pos<len){
    for(i=0; i<len; i++){
      if(array[i].name == -1){
        array[i].name = pos;
        array[i].sum_path = path;
        flag = 1;
        break;
        }
      }
    }else{
      max = array[0].sum_path;
      indice_max=0;
      for(i=0; i<len; i++){
        if((array[i].sum_path > max)||((array[i].sum_path == max) &&(array[i].name>indice_max))){
            max = array[i].sum_path;
            indice_max = i;
        }
      }
      //printf("Massimo:= %lu\n", max);
      //printf("Indice_max := %hu\n", indice_max);
      if(path <= max && flag==0){
        array[indice_max].sum_path = path;
        array[indice_max].name = pos;
        return 1;
      }
    }
    return 0;

}

unsigned long sum_path(unsigned short nodi) /*Fare una branch - IDEA: spostare il puntatore sull'input e prendere ciò che serve*/
{
  unsigned long dijkstra[nodi], min, somma;
  unsigned int matrice[nodi][nodi];
  unsigned short i,j, used;
  int indice_min;
  int return_value;

  if(nodi>1){
    for(i=0; i<nodi; i++){
      for(j=0; j<nodi-1; j++){
        return_value = scanf("%u,", &matrice[i][j]);
      }
      return_value = scanf("%u", &matrice[i][j]);
    }
    for(i=0; i<nodi; i++){
      matrice[i][i]=0;
      dijkstra[i] = matrice[0][i];
    }

    // for(i=0; i<nodi; i++){
    //   for(j=0; j<nodi-1; j++){
    //     printf("%u,",matrice[i][j]);
    //   }
    //   printf("%u\n", matrice[i][j]);
    // }

    if(return_value != 0){
      used=nodi;
      indice_min = trova_min(nodi, dijkstra);
      //printf("trovamin concluso in sum_path %d \n", indice_min);
      if(indice_min != -1){
        //min = dijkstra[indice_min];
        while(used>0 && indice_min !=-1){
          min = dijkstra[indice_min];
          //printf("Stampa dijkstra!!!!!!!!!!!!!!!!!\n");
          // for(i=0; i<nodi; i++)
          //   printf("%lu ", dijkstra[i]);
          // printf("\n");
          for(i=1; i<nodi; i++){
            if((min + matrice[indice_min][i]<dijkstra[i] || dijkstra[i]==0) && matrice[indice_min][i] != 0)
              //printf("sto per sostituire %lu + %u = %lu\n",min, matrice[indice_min][i], min + matrice[indice_min][i] );
              dijkstra[i] = min + matrice[indice_min][i];
          }
          indice_min = trova_min_from(nodi, dijkstra, min, indice_min);
          used--;
        }
      }else
        return 0;

      somma = 0;
      for(i=0; i<nodi; i++)
        somma = somma + dijkstra[i];
      //printf("Somma:= %lu\n", somma);
      return somma;
    }else{
      //printf("Una Scanf è fallita\n");
      return 0;
    }

  }else
    return 0;
}

/*Trova il minimo a partire da un certo numero e dall'indice i=1 e restituisce l'indice del minimo*/
/*Nel contesto "GraphRanker" la soglia corrisponde alla parte già percorsa di dijkstra*/
/*soglia_indice serve per non riprendere lo stesso elemento*/
int trova_min_from(unsigned short len, unsigned long * array, unsigned long soglia, unsigned short soglia_indice)
{
  unsigned short i;
  int indice_min;
  unsigned long min;

  i=0;
  indice_min=-1;
  min = -1; /*Verrà sempre sovrascritto o non utilizzato - Statement per rimuovere warning*/
  while(i<len){
    if((((array[i]==soglia && i>soglia_indice) || array[i]>soglia)) && array[i] != 0){
      indice_min=i;
      min=array[i];
      break; /*È possibile usare break?*/
    }
    i++;
  }
  i++;
  for(i=i-1; i<len; i++){
    if(array[i]<min && array[i] !=0){ /*Scelgo sempre il valore sopra la soglia con indice più piccolo*/
      if((array[i]==soglia && i>soglia_indice) || array[i]>soglia){
        indice_min=i;
        min=array[i];
      }
    }
  }
  //printf("*****************************************indice_min:=%d\n", indice_min);
  return indice_min;

}
/*Trova il minimo a partire dall'indice i=1 e restituisce l'indice del minimo*/
int trova_min(unsigned short len, unsigned long * array)
{
  unsigned short i;
  int indice_min;
  unsigned long min;

  /*Se resta -1, tutti gli elementi sono a zero, zero non raggiungibile*/
  //printf("Trova_min appena iniziato\n");
  indice_min = -1;
  for(i=0;i<len; i++){
    if(array[i] > 0){
      min = array[i];
      indice_min = i;
      break;
    }
  }
  i++; /*Aggiungo e sottraggo per eliminare il warning*/
  for(i=i-1; i<len; i++){
    if(array[i]<min && array[i] >0){
      indice_min=i;
      min=array[i];
    }
  }
  //printf("Trova_min concluso: return %d\n", indice_min);
  return indice_min;
}

/*ORDINAMENTO*/
int compara(graph_t * a, graph_t * b)
{
  return a->name - b->name;
}

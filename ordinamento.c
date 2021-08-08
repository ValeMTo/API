
/*Algoritmo di ordinamento*/

typedef struct {
  unsigned short name;
  unsigned long sum_path; /*Check the size*/
} graph_t;

#define LEN 20

void print_array(graph_t* , unsigned);
void quick_sort(graph_t*, unsigned short, unsigned short);
unsigned short partiziona(graph_t*, unsigned short, unsigned short);

int main()
{
  graph_t array[LEN];

  quick_sort(array, 0, LEN);

  print_array(array, LEN);

  return 0;
}

void print_array(graph_t * array , unsigned short len)
{
  unsigned short i;

  for(i=0; i<len; i++){
    printf("%hu ", array[i].name);
  }
  printf("\n");
}

unsigned short partiziona(graph_t* array, unsigned short min, unsigned short max)
{
  unsigned short med = array[max].name;
  int i = min-1;
  int j;
  graph_t tmp;

  for(j=min; j<=max-1; j++){
    if(array[j].name < med){
      i++;
      tmp.name = array[i].name;
      tmp.sum_path = array[i].sum_path;
      array[i].name = array[j].name;
      array[i].sum_path = array[j].sum_path;
      array[j].name = tmp.name;
      array[j].sum_path = tmp.sum_path;
    }
  }
  tmp.name = array[i+1].name;
  tmp.sum_path = array[i+1].sum_path;
  array[i+1].name = array[max].name;
  array[i+1].sum_path = array[max].sum_path;
  array[max].name = tmp.name;
  array[max].sum_path = tmp.sum_path;

  return (i+1);

}

void quick_sort(graph_t* array, unsigned short min, unsigned short max)
{
  unsigned short part;
  if(min<max){
    part = partiziona(array, min, max);
    quick_sort(array, min, part -1);
    quick_sort(array, part+1, max)
  }
}

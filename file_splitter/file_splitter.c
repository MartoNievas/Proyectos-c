#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#define BLOCK_SIZE 1024 // En bytes

/*Toma como parametro un puntero a file y el numero de veces que lo voy a partir*/
void split(FILE* file, int num,char* base_name)
{
  if (!num) {
    printf("Error: It cannot be divided into 0 parts\n");
  }
    
  int part = 0;
  int bytes_read;
  fseek(file,0,SEEK_END);
  int size = ftell(file);
  rewind(file);
  
  //Ahora vamos con las particiones
  int partition_size = size / num;
  int remaining = size % num;
  
  printf("Size of file: %d Bytes \n",size);
  printf("Size of partitions: %d Bytes\n", partition_size);
  printf("Remaining partitions size: %d Bytes \n",remaining);

  unsigned char* buffer = malloc(partition_size);
  if (!buffer) {
    printf("Error: Memory cannot be reserved \n");
    free(buffer);
  }  
  

  while ((bytes_read = fread(buffer,1,partition_size,file)) > 0) 
  {
    char name[64];
    snprintf(name, sizeof(name), "%s.parte%03d",base_name,part++);
    
    FILE* out = fopen(name,"w");

    if(!out) {
      printf("Error: Memory cannot be reserved\n");
      free(buffer);
    }
    
    //Ahora escribimos 
    fwrite(buffer,1,bytes_read,out);
    fclose(out);
  }
  free(buffer);
}


void join(int argc,char* argv[])
{
  int i;
  char* filepart_path;
  FILE* filepart;
  unsigned char* buffer = malloc(BLOCK_SIZE);
  int bytes_read;


  //Ahora abrimos el archivo donde se va a guardar todo
  char* filejoin_path = argv[argc-2];
  FILE* filejoin = fopen(filejoin_path,"w");
  
  for (i = 1;i < argc - 2; i++) {
    filepart_path = argv[i]; //Aqui tenemos la i-esima parte del archivo spliteado
     
    filepart = fopen(filepart_path,"r");
    while((bytes_read = fread(buffer,1,BLOCK_SIZE,filepart)) > 0) 
    {
        fwrite(buffer,1,bytes_read,filejoin);
    }
    fclose(filepart);
  }
  fclose(filejoin);
  free(buffer);
}

int main(int argc, char *argv[])
{
  
  if (argc < 3) 
  {
    printf("Error: Arguments are missing\n");
    return -1;
  }

  char* flag = argv[argc-1]; //El ultimo es la flag


  if (strcmp(flag,"-s") == 0) 
  {
    char* fpath = argv[1];
    int num = atoi(argv[2]);
    FILE* file = fopen(fpath,"r");
    split(file,num,basename(fpath));
    fclose(file);
    return 0;
  }

  if (strcmp(flag,"-j")==0 && argc >= 5) 
  {
    join(argc,argv);
    printf("This is join mod\n");
  }

  
  return 0;
}

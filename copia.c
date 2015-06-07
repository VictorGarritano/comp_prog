#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#define TAM 128

int myopen (const char *pathname, int flags);
ssize_t myread(int fd, void *buf);			// I/O retorna int: numeros de bytes lidos; 0 se EOF, -1 como erro (ver pags.865,866)
ssize_t mywrite(int fd, const void *buf, size_t count); // retorna numero de bytes escritos se OK, -1 como erro
int myclose(int fd);					// 

int myopen (const char *pathname, int flags) {
  return open(pathname, flags);
 __asm__ ("movl $5, %%eax\n\t" // Open
  "movl %[name], %%ebx\n\t" // argumento 1: nome
  "movl %[flags], %%ecx\n\t" // argumento 2: flags
  "int $0x80\n\t" // System call
: // saida
:[name]"r"(pathname), [flags]"r"(flags) // lista de entrada
:"%ebx", "%eax", "%ecx");
}

ssize_t myread(int fd, void *buf) {
  return read(fd, buf, TAM);
}

ssize_t mywrite(int fd, const void *buf, size_t count) {
  return write(fd, buf, count);
}

int myclose(int fd) {
  return close(fd);
}  
                                                                              
int main (int argc, char** argv) {  	// argc=# de args,argv[0]=copia.c, argv[1]=arq.entrada                                            
  int arq;                                                                    
  char buf[TAM];                                                              
  ssize_t lidos;                                                              

  if (argc != 2) {                                                            
    fprintf(stderr,"forma correta: %s <nomearquivo>\n", argv[0]);       
    return 1;                                                                 
  }                                                                           

  arq = myopen (argv[1], O_RDONLY);                                           
  if (arq<0) { perror("abertura de arquivo"); return 1;}                      

  while ((lidos = myread (arq, buf)) > 0)                                
    if ((mywrite (STDOUT_FILENO, buf, lidos) != lidos))                       
      { perror("escrita:"); return 1;}                                        

  myclose (arq);                                                              
  return 0;                                                                   
}  
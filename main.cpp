#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <ctype.h>
#include <conio.h>
//descritor possibilita armazenar detalhes sobre a lista e, no minimo acessar as extremidades da mesma
//editora simp encadeada com desc
//livros duplamente encadeada
//autor sem ordenar nada simp encadeada
//info autor simp encadeada aparte //nao apagar nunca

struct tpEditora {
    char editora[30];
    tpLivros *livros;
    tpEditora *proximo;
};

struct tpDescritor {
    tpEditora *inicio, *fim;
    int qtde;
};

struct tpLivros {
    char titulo[30];
    int ano,paginas;
    tpListaAutores *autores;
    tpLivros *anterior, *proximo;
};

struct tpAutor {
    char nome[30],sobrenome[30];
    tpAutor *proximo;
};

struct tpListaAutores {
    //informacao
    tpAutor *autor;
    tpListaAutores *proximo;
};
//nao alterar structs
struct tpLivroBinario {
    char autores[100],tituloLivro[50],editora[50];
    int ano,paginas;
};

void arquivoTextoParaBinario () {
    tpLivroBinario reg;
    FILE *ponteiroTxt = fopen("livros.txt","r");
    FILE *ponteiroBinario = fopen("livrosBinario.dat","rb+");

    fscanf(ponteiroTxt,"%[^|];%s;%[^|]%s;[^|];%s;%d,%d\n");
    while (!feof(ponteiroTxt)) {
        fscanf(ponteiroTxt,"%[^|];%s;%[^|]%s;[^|];%s;%d,%d\n");
        fread();
        fwrite();
    }    
    fscanf(ponteiroTxt,"%[^|];%s;%[^|]%s;[^|];%s;%d,%d\n");
    fread();
    fwrite();

    fclose(ponteiroBinario);
    fclose(ponteiroTxt);

}

void inicializarDescritor (tpDescritor &descritor) {    
    descritor.inicio = descritor.fim=NULL;
    descritor.qtde=0;
}

int main () {
    
    tpDescritor descritor;
    inicializarDescritor(descritor);
    
    return 0;
}
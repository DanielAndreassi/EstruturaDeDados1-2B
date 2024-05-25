#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <ctype.h>
#include <conio.h>
//descritor possibilita armazenar detalhes sobre a lista e, no minimo acessar as extremidades da mesma
//editora simp encadeada com desc ordenado sempre
//livros duplamente encadeada
//autor sem ordenar nada simp encadeada
//info autor simp encadeada aparte //nao apagar nunca

struct tpAutor {
    char nome[30],sobrenome[30];
    tpAutor *proximo;
};
struct tpListaAutores {
    //informacao
    tpAutor *endereco;
    tpListaAutores *proximo;
};
struct tpLivros {
    char titulo[30];
    int ano,paginas;
    tpListaAutores *autores;
    tpLivros *anterior, *proximo;
};
struct tpEditora {
    char editora[30];
    tpLivros *livros;
    tpEditora *proximo;
};

struct tpDescritor {
    tpEditora *inicio, *fim;
    int qtde;
};
//nao alterar structs
struct tpLivroBinario {
    char autores[100],tituloLivro[50],editora[50];
    int ano,paginas;
};

void arquivoTextoParaBinario () {
    tpLivroBinario reg;

    FILE *ponteiroTxt = fopen("livros.txt", "r");
    FILE *ponteiroBinario = fopen("livrosBinario.dat", "wb+");

    if (!ponteiroTxt || !ponteiroBinario) {
        fprintf(stderr, "Erro ao abrir arquivos.\n");
    } else {
        fseek(ponteiroBinario, 0, SEEK_END); 
        if (ftell(ponteiroBinario) == 0) { 
            rewind(ponteiroBinario); 
            
            fscanf(ponteiroTxt, "%[^|]|%[^|]|%[^|]|%d|%d\n",reg.autores, reg.tituloLivro, reg.editora, &reg.ano, &reg.paginas);
            while (!feof(ponteiroTxt)) {
                fscanf(ponteiroTxt, "%[^|]|%[^|]|%[^|]|%d|%d\n",reg.autores, reg.tituloLivro, reg.editora, &reg.ano, &reg.paginas);
                fwrite(&reg, sizeof(tpLivroBinario), 1, ponteiroBinario);
            }
        } else {
            fprintf(stderr, "O arquivo binário já contém dados.\n");
        }
        fclose(ponteiroBinario);
        fclose(ponteiroTxt);
    }
}

void inicializarDescritor (tpDescritor &descritor) {    
    descritor.inicio = descritor.fim=NULL;
    descritor.qtde=0;
}

void inicializarEstrutura (tpAutor autor, tpListaAutores listaAutor,tpLivros livros, tpEditora editora) {
    
}

tpEditora *novaCaixaEditora (tpLivroBinario arquivo, tpAutor *autor,tpListaAutores *listaAutores) {
    tpEditora *novo = new tpEditora;
    tpAutor *aux;

    strcpy(novo->editora,arquivo.editora);
    novo->livros = novaCaixaLivro(arquivo,autor,listaAutores);
    novo->proximo = NULL;
    return novo;
}

tpListaAutores *novaCaixaAutores(tpListaAutores *listaAutores,tpAutor *autor,char nome[50],char sobrenome[50]) {
    tpAutor *novoAutor = new tpAutor;
    tpListaAutores *NovalistaAutores = new tpListaAutores;
    tpListaAutores *aux;
    
    novoAutor->proximo=NULL;
    strcpy(novoAutor->nome,nome);
    strcpy(novoAutor->sobrenome,sobrenome);
    
    if(listaAutores->endereco == NULL) {
        listaAutores->endereco = novoAutor;
    }
    else {
        aux=listaAutores;
        while (aux->proximo != NULL) {
            aux=aux->proximo;
        }

    }
}

tpLivros *novaCaixaLivro (tpLivroBinario arq,tpAutor *autor,tpListaAutores *listaAutores) {
    tpLivros *livros = new tpLivros;
    tpAutor *aux,*novoAutor;
    char nome[50],sobrenome[50];
    int i=0,tl;

    livros->anterior = NULL;
    livros->proximo = NULL;
    strcpy(livros->titulo,arq.tituloLivro);
    livros->ano=arq.ano;
    livros->paginas= arq.paginas;
    livros->autores=NULL;
    
    while(i<strlen(arq.autores)) {
        for(tl = 0;i<strlen(arq.autores) && arq.autores[i] != ','; i++) {
            sobrenome[tl]=arq.autores[i];
        }
        sobrenome[tl] = '\0';
    
        for(tl = 0,i++; i<strlen(arq.autores) && arq.autores[i] != ';';i++) {
            nome[tl] = arq.autores[i];
        }
        nome[tl++] = '\0';
        i++;
        
        tpListaAutores *listaAutor = new tpListaAutores;
        
    }
    
}


int main () {
    
    tpAutor *autor;
    tpListaAutores *listaAutores;
    tpLivros *livros;
    tpEditora *editora;
    tpDescritor descritor;

    inicializarDescritor(descritor);
    arquivoTextoParaBinario();
    
    return 0;
}

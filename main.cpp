#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <locale.h>
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
    char autores[200],tituloLivro[50],editora[50];
    int ano,paginas;
};

void arquivoTextoParaBinario () {
    tpLivroBinario reg;

    FILE *ponteiroTxt = fopen("livros.txt", "r");
    FILE *ponteiroBinario = fopen("livrosBinario.dat", "wb+");

    if (!ponteiroTxt || !ponteiroBinario) {
        fprintf(stderr, "Erro ao abrir arquivos.\n");
    } else {
        if (ftell(ponteiroBinario) == 0) { 
            
            fscanf(ponteiroTxt, "%[^|]|%[^|]|%[^|]|%d|%d\n",&reg.autores, &reg.tituloLivro, &reg.editora, &reg.ano, &reg.paginas);
            while (!feof(ponteiroTxt)) {
                
                fwrite(&reg, sizeof(tpLivroBinario), 1, ponteiroBinario);
                fscanf(ponteiroTxt, "%[^|]|%[^|]|%[^|]|%d|%d\n",&reg.autores, &reg.tituloLivro, &reg.editora, &reg.ano, &reg.paginas);
            }
            fwrite(&reg, sizeof(tpLivroBinario), 1, ponteiroBinario);
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

tpAutor *inicializarEstruturaAutores () {
    FILE *ponteiro = fopen("livrosBinario.dat","rb+");
    tpLivroBinario aux;
    tpAutor *autorAuxiliar,*autor=NULL,*novoAutor;
    char nome[50],sobrenome[50];
    int tlSobrenome,tlNome,i=0;

    if(!ponteiro) {
        printf("Erro ao abrir o arquivo");
    }
    else {
        fread(&aux,sizeof(tpLivroBinario),1,ponteiro);

        while(!feof(ponteiro)) {
            i=0;
            while(i<strlen(aux.autores)) {
                for(tlSobrenome=0; i < strlen(aux.autores) && aux.autores[i] != ',' ;i++) {
                    sobrenome[tlSobrenome] = aux.autores[i];
                    tlSobrenome++;
                }
                for(i++,tlNome=0; i < strlen(aux.autores) && aux.autores[i] != ';';i++) {
                    nome[tlNome] = aux.autores[i];
                    tlNome++;
                }
                sobrenome[tlSobrenome] = '\0';
                nome[tlNome] = '\0';

                if(autor == NULL) {
                    autor = new tpAutor;
                    autor->proximo=NULL;
                    strcpy(autor->sobrenome,sobrenome);
                    strcpy(autor->nome,nome);
                }
                else {
                    if(strcmp(autor->nome,nome) != 0 && strcmp(autor->sobrenome,sobrenome) != 0) {
                        autorAuxiliar = autor;
                        while(autorAuxiliar->proximo != NULL && strcmp(autorAuxiliar->proximo->nome,nome) != 0 && strcmp(autorAuxiliar->proximo->sobrenome,sobrenome) != 0) {
                            autorAuxiliar = autorAuxiliar->proximo;
                        }
                        if(autorAuxiliar->proximo == NULL) {
                            novoAutor = new tpAutor;
                            novoAutor->proximo = NULL;
                            strcpy(novoAutor->sobrenome,sobrenome);
                            strcpy(novoAutor->nome,nome);
                            autorAuxiliar->proximo=novoAutor;
                        }
                    }
                }            
            }
            fread(&aux,sizeof(tpLivroBinario),1,ponteiro);
        }
    }
    
    return autor;
}

tpLivros inicializarEstruturaLivros (tpLivros *livros,tpLivroBinario arquivo) {
    tpLivros *livro = new tpLivros;
    int i=0;
    char nome[50],sobrenome[50];

    livros->anterior=NULL;
    livros->proximo = NULL;
    livros->ano = arquivo.ano;
    livros->paginas = arquivo.paginas;
    livro->autores = NULL;
    strcpy(livro->titulo,arquivo.tituloLivro);





}

void exibir(){
	FILE * ponteiro = fopen("livrosBinario.dat","rb");
	tpLivroBinario arquivo;
	if (!ponteiro)
		printf("Erro ao abrir livros.dat");
	else{
		fread(&arquivo,sizeof(tpLivroBinario),1,ponteiro);
		while(!feof(ponteiro)){
			printf("%s|%s|%s|%d|%d\n",arquivo.autores,arquivo.tituloLivro,arquivo.editora,arquivo.ano,arquivo.paginas);
			fread(&arquivo,sizeof(tpLivroBinario),1,ponteiro);
		}
		fclose(ponteiro);
	}
}

tpEditora *novaCaixaEditora (tpLivroBinario arquivo, tpAutor *autor,tpListaAutores *listaAutores) {
    tpEditora *novo = new tpEditora;
    tpAutor *aux;

    strcpy(novo->editora,arquivo.editora);
    // novo->livros = novaCaixaLivro(arquivo,autor,listaAutores);
    novo->proximo = NULL;
    return novo;
}


void ExibeAutores(tpAutor *autor){
	printf("Nome: %s %s\n",autor->nome,autor->sobrenome);
	if(autor->proximo)
		ExibeAutores(autor->proximo);
}

int main () {
    
    arquivoTextoParaBinario();
    exibir();
    // setlocale(LC_ALL, "Portuguese");
    // tpAutor *autor;
    // autor= inicializarEstruturaAutores();
    // ExibeAutores(autor);
    // tpListaAutores *listaAutores;
    // tpLivros *livros;
    // tpEditora *editora;
    // tpDescritor descritor;


    // inicializarDescritor(descritor);
    // exibir();
    
    
    return 0;
}

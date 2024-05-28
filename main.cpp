#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <locale.h>
#include <ctype.h>
#include <conio.h>
// descritor possibilita armazenar detalhes sobre a lista e, no minimo acessar as extremidades da mesma
// editora simp encadeada com desc ordenado sempre
// livros duplamente encadeada
// autor sem ordenar nada simp encadeada
// info autor simp encadeada aparte //nao apagar nunca

struct tpAutor
{
    char nome[30], sobrenome[30];
    tpAutor *proximo;
};
struct tpListaAutores
{
    // informacao
    tpAutor *endereco;
    tpListaAutores *proximo;
};
struct tpLivros
{
    char titulo[30];
    int ano, paginas;
    tpListaAutores *autores;
    tpLivros *anterior, *proximo;
};
struct tpEditora
{
    char editora[100];
    tpLivros *livros;
    tpEditora *proximo;
};
struct tpDescritor
{
    tpEditora *inicio, *fim;
    int qtde;
};
// nao alterar structs
struct tpLivroBinario
{
    char autores[200], tituloLivro[50], editora[50];
    int ano, paginas;
};

// Funções
// void arquivoTextoParaBinario();
// void inicializarDescritor( tpDescritor *descritor);
//  tpAutor *inicializarEstruturaAutores();
//  tpLivros *novaCaixaLivro( tpLivroBinario arquivo);
// void exibir();
//  tpEditora *novaCaixaEditora(char editora[100]);
//  tpAutor *buscaAutor( tpAutor *autor, char nome[100], char sobrenome[100]);
//  tpListaAutores *novaCaixaListaAutores();
// void separarAutores(char *stringAutores, char vetorAutores[][100], int *tl);
// void recuperarSobrenome(char *stringAutores, char *sobrenome);
// void recuperarPrimeiroNome(char *stringAutores, char *primeiro_nome);
//  tpEditora *incializarEstruraPrincipal( tpAutor *autor, tpDescritor *descritor, tpEditora *editora);
// void ExibeAutores( tpAutor *autor);
// void exibeEditora( tpEditora *editora);

void arquivoTextoParaBinario()
{
    tpLivroBinario reg;

    FILE *ponteiroTxt = fopen("livros.txt", "r");
    FILE *ponteiroBinario = fopen("livrosBinario.dat", "wb+");

    if (!ponteiroTxt || !ponteiroBinario)
    {
        fprintf(stderr, "Erro ao abrir arquivos.\n");
    }
    else
    {
        if (ftell(ponteiroBinario) == 0)
        {

            fscanf(ponteiroTxt, "%[^|]|%[^|]|%[^|]|%d|%d\n", &reg.autores, &reg.tituloLivro, &reg.editora, &reg.ano, &reg.paginas);
            while (!feof(ponteiroTxt))
            {

                fwrite(&reg, sizeof(tpLivroBinario), 1, ponteiroBinario);
                fscanf(ponteiroTxt, "%[^|]|%[^|]|%[^|]|%d|%d\n", &reg.autores, &reg.tituloLivro, &reg.editora, &reg.ano, &reg.paginas);
            }
            fwrite(&reg, sizeof(tpLivroBinario), 1, ponteiroBinario);
        }
        else
        {
            fprintf(stderr, "O arquivo binário já contém dados.\n");
        }
        fclose(ponteiroBinario);
        fclose(ponteiroTxt);
    }
}

void inicializarDescritor(tpDescritor &descritor)
{
    descritor.inicio = descritor.fim = NULL;
    descritor.qtde = 0;
}

tpAutor *inicializarEstruturaAutores()
{
    FILE *ponteiro = fopen("livrosBinario.dat", "rb+");
    tpLivroBinario aux;
    tpAutor *autorAuxiliar, *autor = NULL, *novoAutor;
    char nome[50], sobrenome[50];
    int tlSobrenome, tlNome, i = 0;

    if (!ponteiro)
    {
        printf("Erro ao abrir o arquivo");
    }
    else
    {
        fread(&aux, sizeof(tpLivroBinario), 1, ponteiro);

        while (!feof(ponteiro))
        {
            i = 0;
            while (i < strlen(aux.autores))
            {
                for (tlSobrenome = 0; i < strlen(aux.autores) && aux.autores[i] != ','; i++)
                {
                    sobrenome[tlSobrenome] = aux.autores[i];
                    tlSobrenome++;
                }
                for (i++, tlNome = 0; i < strlen(aux.autores) && aux.autores[i] != ';'; i++)
                {
                    nome[tlNome] = aux.autores[i];
                    tlNome++;
                }
                sobrenome[tlSobrenome] = '\0';
                nome[tlNome] = '\0';

                if (autor == NULL)
                {
                    autor = new tpAutor;
                    autor->proximo = NULL;
                    strcpy(autor->sobrenome, sobrenome);
                    strcpy(autor->nome, nome);
                }
                else
                {
                    if (strcmp(autor->nome, nome) != 0 && strcmp(autor->sobrenome, sobrenome) != 0)
                    {
                        autorAuxiliar = autor;
                        while (autorAuxiliar->proximo != NULL && strcmp(autorAuxiliar->proximo->nome, nome) != 0 && strcmp(autorAuxiliar->proximo->sobrenome, sobrenome) != 0)
                        {
                            autorAuxiliar = autorAuxiliar->proximo;
                        }
                        if (autorAuxiliar->proximo == NULL)
                        {
                            novoAutor = new tpAutor;
                            novoAutor->proximo = NULL;
                            strcpy(novoAutor->sobrenome, sobrenome);
                            strcpy(novoAutor->nome, nome);
                            autorAuxiliar->proximo = novoAutor;
                        }
                    }
                }
            }
            fread(&aux, sizeof(tpLivroBinario), 1, ponteiro);
        }
    }

    return autor;
}

tpLivros *novaCaixaLivro(tpLivroBinario arquivo)
{
    tpLivros *livros = new tpLivros;

    livros->anterior = NULL;
    livros->proximo = NULL;
    livros->ano = arquivo.ano;
    livros->paginas = arquivo.paginas;
    livros->autores = NULL;
    strcpy(livros->titulo, arquivo.tituloLivro);
    printf("VAI CRIAR LIVRO\n");
    return livros;
}

void exibir()
{
    FILE *ponteiro = fopen("livrosBinario.dat", "rb");
    tpLivroBinario arquivo;
    if (!ponteiro)
        printf("Erro ao abrir livros.dat");
    else
    {
        fread(&arquivo, sizeof(tpLivroBinario), 1, ponteiro);
        while (!feof(ponteiro))
        {
            printf("%s|%s|%s|%d|%d\n", arquivo.autores, arquivo.tituloLivro, arquivo.editora, arquivo.ano, arquivo.paginas);
            fread(&arquivo, sizeof(tpLivroBinario), 1, ponteiro);
        }
        fclose(ponteiro);
    }
}

tpEditora *novaCaixaEditora(char editora[100])
{
    tpEditora *novo = new tpEditora;

    strcpy(novo->editora, editora);
    novo->proximo = NULL;
    novo->livros = NULL;
    printf("VAI CRIAR EDITORA\n");
    return novo;
}

tpAutor *buscaAutor(tpAutor *autor, char nome[100], char sobrenome[100])
{
    tpAutor *aux = autor;

    while (aux->proximo != NULL)
    {
        if (strcmp(aux->nome, nome) != 0 && strcmp(aux->sobrenome, sobrenome) != 0)
        {
            aux = aux->proximo;
        }
        else
        {
            return aux;
        }
    }

    if (strcmp(aux->nome, nome) == 0 && strcmp(aux->sobrenome, sobrenome) == 0)
    {
        return aux;
    }
    else
    {
        return NULL;
    }
}

tpListaAutores *novaCaixaListaAutores()
{
    // ESTOU RECEBENDO ERRO AQUI
    printf("NATES DE CRIAR NOVA CAIXA AYTIRWS\n");
    tpListaAutores *novoAutor = new tpListaAutores;
    novoAutor->endereco = NULL;
    novoAutor->proximo = NULL;
    printf("DEPOISSSSSSSSSSSSS DE CRIAR NOVA CAIXA AYTIRWS\n");
    return novoAutor;
}

void separarAutores(char *stringAutores, char vetorAutores[][100], int *tl)
{
    char *token;
    *tl = 0;
    token = strtok(stringAutores, ";");
    while (token != NULL)
    {
        while (*token == ' ')
            token++;
        strcpy(vetorAutores[*tl], token);
        (*tl)++;
        token = strtok(NULL, ";");
    }
}

void recuperarSobrenome(char *stringAutores, char *sobrenome)
{
    char *posVirgula = strchr(stringAutores, ',');
    if (posVirgula != NULL)
    {
        strncpy(sobrenome, stringAutores, posVirgula - stringAutores);
        sobrenome[posVirgula - stringAutores] = '\0';
    }
    else
    {
        strcpy(sobrenome, stringAutores);
    }
}

void recuperarPrimeiroNome(char *stringAutores, char *primeiro_nome)
{
    char *posVirgula = strchr(stringAutores, ',');
    if (posVirgula != NULL)
    {
        posVirgula++;
        while (*posVirgula == ' ')
            posVirgula++;
        strcpy(primeiro_nome, posVirgula);
    }
    else
    {
        strcpy(primeiro_nome, "");
    }
}

// char arrayAutores[10][100];
//         int tl;
//         separarAutores(aux.autores, arrayAutores, &tl);
//         for (int i = 0; i < tl; i++)
//         {
//             char nome[100];
//             recuperarPrimeiroNome(arrayAutores[i], nome);
//             char sobrenome[100];
//             recuperarSobrenome(arrayAutores[i], sobrenome);
//          }

void incializarEstruraPrincipal(tpAutor *autor, tpDescritor &descritor, tpEditora *editora)
{

    tpLivroBinario aux;
    tpLivros *novoLivro;
    tpEditora *novaEditora;
    tpListaAutores *aux2, *aux3;
    tpAutor *autorBusca;
    int tlSobrenome, i, tlNome;
    char nome[100], sobrenome[100];
    FILE *ponteiro = fopen("livrosBinario.dat", "rb+");
    rewind(ponteiro);

    fread(&aux, sizeof(tpLivroBinario), 1, ponteiro);
    while (!feof(ponteiro))
    {
        if (descritor.qtde == 0)
        {
            printf("QUANTIDADE 0\n");
            novaEditora = novaCaixaEditora(aux.editora);
            novoLivro = novaCaixaLivro(aux);
            strcpy(novoLivro->titulo, aux.tituloLivro);
            novoLivro->ano = aux.ano;
            novoLivro->paginas = aux.paginas;
            novaEditora->livros = novoLivro;
            char arrayAutores[10][100];
            int tl;
            separarAutores(aux.autores, arrayAutores, &tl);
            printf("CHEGOU AQUI.COM\n");
            tpListaAutores *novoAutorEndereco = new tpListaAutores;
            printf("CHEGOU AQUI.COM.BR\n");
            novoAutorEndereco->endereco = NULL;
            novoAutorEndereco->proximo = NULL;
            novoLivro->autores = novoAutorEndereco;
            for (int i = 0; i < tl; i++)
            {
                if (novoAutorEndereco->endereco == NULL)
                {

                    recuperarPrimeiroNome(arrayAutores[i], nome);
                    recuperarSobrenome(arrayAutores[i], sobrenome);
                    autorBusca = buscaAutor(autor, nome, sobrenome);

                    if (autorBusca != NULL)
                    {
                        novoAutorEndereco->endereco = autorBusca;
                    }
                }
                else
                {
                    aux2 = novaCaixaListaAutores();
                    aux3 = novoAutorEndereco;
                    while (aux3->proximo != NULL)
                    {
                        aux3 = aux3->proximo;
                    }

                    recuperarPrimeiroNome(arrayAutores[i], nome);
                    recuperarSobrenome(arrayAutores[i], sobrenome);

                    autorBusca = buscaAutor(autor, nome, sobrenome);

                    if (autorBusca != NULL)
                    {
                        aux2->endereco = autorBusca;
                        aux3->proximo = aux2;
                    }
                }
            }

            descritor.qtde++;
            descritor.fim = novaEditora;
            descritor.inicio = novaEditora;
        }
        else
        {
            printf("QUANTIDADE MAIOR QUE 0\n");
            tpEditora *editoraAuxiliar = descritor.inicio;
            while (strcmp(editoraAuxiliar->editora, aux.editora) != 0)
            {
                editoraAuxiliar = editoraAuxiliar->proximo;
            }
            if (strcmp(editoraAuxiliar->editora, aux.editora) == 0)
            {
                // é mais um livro da mesma editora que é a editoraAuxiliar
                novoLivro = novaCaixaLivro(aux);
                strcpy(novoLivro->titulo, aux.tituloLivro);
                novoLivro->ano = aux.ano;
                novoLivro->paginas = aux.paginas;
                editoraAuxiliar->livros->proximo = novoLivro;
                char arrayAutores[10][100];
                int tl;
                separarAutores(aux.autores, arrayAutores, &tl);
                tpListaAutores *novoAutorEndereco = new tpListaAutores;
                novoAutorEndereco->endereco = NULL;
                novoAutorEndereco->proximo = NULL;
                novoLivro->autores = novoAutorEndereco;

                for (int i = 0; i < tl; i++)
                {
                    if (novoAutorEndereco->endereco == NULL)
                    {

                        recuperarPrimeiroNome(arrayAutores[i], nome);
                        recuperarSobrenome(arrayAutores[i], sobrenome);
                        autorBusca = buscaAutor(autor, nome, sobrenome);

                        if (autorBusca != NULL)
                        {
                            novoAutorEndereco->endereco = autorBusca;
                        }
                    }
                    else
                    {
                        aux2 = novaCaixaListaAutores();
                        aux3 = novoAutorEndereco;
                        while (aux3->proximo != NULL)
                        {
                            aux3 = aux3->proximo;
                        }

                        recuperarPrimeiroNome(arrayAutores[i], nome);
                        recuperarSobrenome(arrayAutores[i], sobrenome);

                        autorBusca = buscaAutor(autor, nome, sobrenome);

                        if (autorBusca != NULL)
                        {
                            aux2->endereco = autorBusca;
                            aux3->proximo = aux2;
                        }
                    }
                }
            }
            else
            {
                // ** INSERIR ORDENADO **
                if (strcmp(descritor.inicio->editora, aux.editora) > 0)
                {
                    // aux.editora vem antes do inicio
                    novaEditora = novaCaixaEditora(aux.editora);
                    novoLivro = novaCaixaLivro(aux);
                    strcpy(novoLivro->titulo, aux.tituloLivro);
                    novoLivro->ano = aux.ano;
                    novoLivro->paginas = aux.paginas;
                    novaEditora->livros = novoLivro;
                    char arrayAutores[10][100];
                    int tl;
                    separarAutores(aux.autores, arrayAutores, &tl);
                    // novoAutorEndereco = novaCaixaListaAutores();
                    printf("CHEGOU AQUI.COM\n");
                    tpListaAutores *novoAutorEndereco = new tpListaAutores;
                    novoAutorEndereco->endereco = NULL;
                    novoAutorEndereco->proximo = NULL;
                    printf("CHEGOU AQUI.COM.BR\n");
                    novoLivro->autores = novoAutorEndereco;

                    for (int i = 0; i < tl; i++)
                    {
                        if (novoAutorEndereco->endereco == NULL)
                        {

                            recuperarPrimeiroNome(arrayAutores[i], nome);
                            recuperarSobrenome(arrayAutores[i], sobrenome);
                            autorBusca = buscaAutor(autor, nome, sobrenome);

                            if (autorBusca != NULL)
                            {
                                novoAutorEndereco->endereco = autorBusca;
                            }
                        }
                        else
                        {
                            aux2 = novaCaixaListaAutores();
                            aux3 = novoAutorEndereco;
                            while (aux3->proximo != NULL)
                            {
                                aux3 = aux3->proximo;
                            }

                            recuperarPrimeiroNome(arrayAutores[i], nome);
                            recuperarSobrenome(arrayAutores[i], sobrenome);

                            autorBusca = buscaAutor(autor, nome, sobrenome);

                            if (autorBusca != NULL)
                            {
                                aux2->endereco = autorBusca;
                                aux3->proximo = aux2;
                            }
                        }
                    }
                    novaEditora->proximo = descritor.inicio;
                    descritor.inicio = novaEditora;
                    descritor.qtde++;
                }
                else if (strcmp(descritor.fim->editora, aux.editora) < 0)
                {
                    // aux.editora vem depois do fim
                    novaEditora = novaCaixaEditora(aux.editora);
                    novoLivro = novaCaixaLivro(aux);
                    strcpy(novoLivro->titulo, aux.tituloLivro);
                    novoLivro->ano = aux.ano;
                    novoLivro->paginas = aux.paginas;
                    novaEditora->livros = novoLivro;
                    char arrayAutores[10][100];
                    int tl;
                    separarAutores(aux.autores, arrayAutores, &tl);
                    tpListaAutores *novoAutorEndereco = new tpListaAutores;
                    novoAutorEndereco->endereco = NULL;
                    novoAutorEndereco->proximo = NULL;
                    novoLivro->autores = novoAutorEndereco;

                    for (int i = 0; i < tl; i++)
                    {
                        if (novoAutorEndereco->endereco == NULL)
                        {

                            recuperarPrimeiroNome(arrayAutores[i], nome);
                            recuperarSobrenome(arrayAutores[i], sobrenome);
                            autorBusca = buscaAutor(autor, nome, sobrenome);

                            if (autorBusca != NULL)
                            {
                                novoAutorEndereco->endereco = autorBusca;
                            }
                        }
                        else
                        {
                            aux2 = novaCaixaListaAutores();
                            aux3 = novoAutorEndereco;
                            while (aux3->proximo != NULL)
                            {
                                aux3 = aux3->proximo;
                            }

                            recuperarPrimeiroNome(arrayAutores[i], nome);
                            recuperarSobrenome(arrayAutores[i], sobrenome);

                            autorBusca = buscaAutor(autor, nome, sobrenome);

                            if (autorBusca != NULL)
                            {
                                aux2->endereco = autorBusca;
                                aux3->proximo = aux2;
                            }
                        }
                    }

                    descritor.fim->proximo = novaEditora;
                    descritor.fim = novaEditora;
                    descritor.qtde++;
                }
                else
                {
                    tpEditora *editoraAuxiliar1 = descritor.inicio;
                    tpEditora *anterior = descritor.inicio;
                    while (strcmp(editoraAuxiliar1->editora, aux.editora) < 0)
                    {
                        anterior = editoraAuxiliar1;
                        editoraAuxiliar1 = editoraAuxiliar1->proximo;
                    }
                    // aqui na teoria temos em "anterior" a editora anterior à aquela a ser colocada na estrutura
                    novaEditora = novaCaixaEditora(aux.editora);
                    novoLivro = novaCaixaLivro(aux);
                    strcpy(novoLivro->titulo, aux.tituloLivro);
                    novoLivro->ano = aux.ano;
                    novoLivro->paginas = aux.paginas;
                    novaEditora->livros = novoLivro;
                    char arrayAutores[10][100];
                    int tl;
                    separarAutores(aux.autores, arrayAutores, &tl);
                    tpListaAutores *novoAutorEndereco = new tpListaAutores;
                    novoAutorEndereco->endereco = NULL;
                    novoAutorEndereco->proximo = NULL;
                    novoLivro->autores = novoAutorEndereco;

                    for (int i = 0; i < tl; i++)
                    {
                        if (novoAutorEndereco->endereco == NULL)
                        {

                            recuperarPrimeiroNome(arrayAutores[i], nome);
                            recuperarSobrenome(arrayAutores[i], sobrenome);
                            autorBusca = buscaAutor(autor, nome, sobrenome);

                            if (autorBusca != NULL)
                            {
                                novoAutorEndereco->endereco = autorBusca;
                            }
                        }
                        else
                        {
                            aux2 = novaCaixaListaAutores();
                            aux3 = novoAutorEndereco;
                            while (aux3->proximo != NULL)
                            {
                                aux3 = aux3->proximo;
                            }

                            recuperarPrimeiroNome(arrayAutores[i], nome);
                            recuperarSobrenome(arrayAutores[i], sobrenome);

                            autorBusca = buscaAutor(autor, nome, sobrenome);

                            if (autorBusca != NULL)
                            {
                                aux2->endereco = autorBusca;
                                aux3->proximo = aux2;
                            }
                        }
                    }
                    novaEditora->proximo = anterior->proximo;
                    anterior->proximo = novaEditora;
                    descritor.qtde++;
                }
            }
        }
        fread(&aux, sizeof(tpLivroBinario), 1, ponteiro);
    }
    fclose(ponteiro);
}

void ExibeAutores(tpAutor *autor)
{
    printf("Nome: %s %s\n", autor->nome, autor->sobrenome);
    if (autor->proximo)
        ExibeAutores(autor->proximo);
}

void exibeEditora(tpEditora *editora)
{
    printf("\nNome: %s", editora->editora);
    if (editora->proximo)
        exibeEditora(editora->proximo);
}

int main()
{

    tpDescritor descritor;
    inicializarDescritor(descritor);
    // arquivoTextoParaBinario();
    // exibir();

    tpAutor *autor = NULL;
    autor = inicializarEstruturaAutores();
    tpEditora *editora = NULL;
<<<<<<< HEAD
    editora = incializarEstruraPrincipal(autor,descritor,editora);
    exibeEditora(editora);
=======
    incializarEstruraPrincipal(autor, descritor, editora);
    // exibeEditora(editora);
    printf("\n\nEDITORA INCIO %s\t EDITORA FIM %s\tQUANTIDADE %d\n", descritor.inicio->editora, descritor.fim->editora, descritor.qtde);
>>>>>>> 733d4c946a583b385a506950ec29881a85448f6e

    // autor = inicializarEstruturaAutores();
    printf("\n\n");
    // ExibeAutores(autor);
    // tpListaAutores *listaAutores;
    // tpLivros *livros;

    // exibir();

    return 0;
}

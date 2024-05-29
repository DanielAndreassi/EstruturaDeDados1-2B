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
    char nome[50], sobrenome[50];
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
    char titulo[70];
    int ano, paginas;
    tpListaAutores *autores;
    tpLivros *anterior, *proximo;
};
struct tpEditora
{
    char editora[50];
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
    char autores[100], tituloLivro[100], editora[30];
    int ano, paginas;
};

// Funções
// void arquivoTextoParaBinario();
// void inicializarDescritor( tpDescritor *descritor);
//  tpAutor *inicializarEstruturaAutores();
//  tpLivros *novoLivro( tpLivroBinario arquivo);
// void exibir();
//  tpEditora *novaEditora(char editora[100]);
//  tpAutor *buscaAutor( tpAutor *autor, char nome[100], char sobrenome[100]);
//  tpListaAutores *novaCaixaListaAutores();
void separarAutores(char *stringAutores, char vetorAutores[][100], int *tl);
void recuperarSobrenome(char *stringAutores, char *sobrenome);
void recuperarPrimeiroNome(char *stringAutores, char *primeiro_nome);
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
        fclose(ponteiro);
    }

    return autor;
}

tpLivros *novoLivro(tpLivroBinario arquivo, tpAutor *autor)
{
    tpLivros *livros = new tpLivros;
    tpAutor *autorAuxiliarBusca;
    char nome[50];
    char sobrenome[50];
    int tl_sobrenome, tl_nome;

    livros->anterior = NULL;
    livros->proximo = NULL;
    livros->ano = arquivo.ano;
    livros->paginas = arquivo.paginas;
    livros->autores = NULL;
    strcpy(livros->titulo, arquivo.tituloLivro);

    int index = 0;
    tpListaAutores *ponteiroAutorAuxiliar, *novoAutor;
    while (index < strlen(arquivo.autores))
    {
        for (tl_sobrenome = 0; index < strlen(arquivo.autores) && arquivo.autores[index] != ','; index++)
            sobrenome[tl_sobrenome++] = arquivo.autores[index];
        sobrenome[tl_sobrenome] = '\0';
        index++;
        for (tl_nome = 0; index < strlen(arquivo.autores) && arquivo.autores[index] != ';'; index++)
            nome[tl_nome++] = arquivo.autores[index];
        nome[tl_nome++] = '\0';
        index++;

        if (stricmp(autor->nome, nome), stricmp(autor->sobrenome, sobrenome) != 0)
        {
            autorAuxiliarBusca = autor;
            while (stricmp(autorAuxiliarBusca->proximo->nome, nome) != 0 && stricmp(autorAuxiliarBusca->proximo->sobrenome, sobrenome) != 0)
            {
                autorAuxiliarBusca = autorAuxiliarBusca->proximo;
            }
            if (!livros->autores)
            {
                livros->autores = new tpListaAutores;
                livros->autores->endereco = autorAuxiliarBusca->proximo;
                livros->autores->proximo = NULL;
            }
            else
            {
                ponteiroAutorAuxiliar = livros->autores;

                while (ponteiroAutorAuxiliar->proximo)
                {
                    ponteiroAutorAuxiliar = ponteiroAutorAuxiliar->proximo;
                }
                novoAutor = new tpListaAutores;
                novoAutor->proximo = NULL;
                novoAutor->endereco = autorAuxiliarBusca->proximo;

                ponteiroAutorAuxiliar->proximo = novoAutor;
            }
        }
        else
        {
            if (!livros->autores)
            {
                livros->autores = new tpListaAutores;
                livros->autores->proximo = NULL;
                livros->autores->endereco = autor;
            }
            else
            {
                ponteiroAutorAuxiliar = livros->autores;
                while (ponteiroAutorAuxiliar->proximo)
                {
                    ponteiroAutorAuxiliar = ponteiroAutorAuxiliar->proximo;
                }
                novoAutor = new tpListaAutores;
                novoAutor->endereco = autor;
                novoAutor->proximo = NULL;
                ponteiroAutorAuxiliar->proximo = novoAutor;
            }
        }
    }
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

tpEditora *novaEditora(tpAutor *autor, tpLivroBinario arquivo)
{
    tpEditora *novo = new tpEditora;
    tpAutor *auxliarAutor;

    strcpy(novo->editora, arquivo.editora);
    novo->proximo = NULL;
    novo->livros = novoLivro(arquivo, autor);
    return novo;
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

void inicializarEstruturaPrincipal(tpAutor *autor, tpDescritor &descritor)
{
    FILE *ponteiroArquivo = fopen("livrosBinario.dat", "rb+");
    tpEditora *editoraNova, *editoraAuxiliar;
    tpLivros *livroNovo, *livroAuxiliarBusca;
    tpLivroBinario arquivo;

    if (ponteiroArquivo == NULL)
    {
        printf("Erro na abertura do arquivo livros.dat na hora de inicializar estrutura!\n");
    }
    else
    {
        rewind(ponteiroArquivo);
        fread(&arquivo, sizeof(tpLivroBinario), 1, ponteiroArquivo);

        while (!feof(ponteiroArquivo))
        {

            if (descritor.qtde == 0)
            {
                descritor.inicio = descritor.fim = novaEditora(autor, arquivo);
                descritor.qtde++;
            }
            else
            {
                if (strcmp(arquivo.editora, descritor.inicio->editora) == 0)
                {
                    if (descritor.inicio->livros == NULL)
                    {
                        descritor.inicio->livros = novoLivro(arquivo, autor);
                    }
                    else
                    {
                        livroNovo = novoLivro(arquivo, autor);
                        livroAuxiliarBusca = descritor.inicio->livros;

                        while (livroAuxiliarBusca->proximo != NULL)
                        {
                            livroAuxiliarBusca = livroAuxiliarBusca->proximo;
                        }
                        livroAuxiliarBusca->proximo = livroNovo;
                        livroNovo->anterior = livroAuxiliarBusca;
                    }
                }
                else
                {
                    if (strcmp(arquivo.editora, descritor.inicio->editora) < 0)
                    {
                        editoraNova = novaEditora(autor, arquivo);
                        editoraNova->proximo = descritor.inicio;
                        descritor.inicio = editoraNova;
                        descritor.qtde++;
                    }
                    else
                    {
                        editoraAuxiliar = descritor.inicio;
                        while (editoraAuxiliar->proximo != NULL && strcmp(arquivo.editora, editoraAuxiliar->proximo->editora) > 0)
                        {
                            editoraAuxiliar = editoraAuxiliar->proximo;
                        }
                        if (strcmp(arquivo.editora, editoraAuxiliar->proximo->editora) == 0)
                        {

                            if (editoraAuxiliar->proximo->livros == NULL)
                                editoraAuxiliar->proximo->livros = novoLivro(arquivo, autor);

                            else
                            {
                                livroNovo = novoLivro(arquivo, autor);
                                livroAuxiliarBusca = editoraAuxiliar->proximo->livros;
                                while (livroAuxiliarBusca->proximo != NULL)
                                {
                                    livroAuxiliarBusca = livroAuxiliarBusca->proximo;
                                }

                                livroAuxiliarBusca->proximo = livroNovo;
                                livroNovo->anterior = livroAuxiliarBusca;
                            }
                        }
                        else
                        {
                            if (editoraAuxiliar->proximo == NULL)
                            {
                                descritor.fim->proximo = editoraNova = novaEditora(autor, arquivo);
                            }
                            else
                            {
                                editoraNova = novaEditora(autor, arquivo);
                                editoraNova->proximo = editoraAuxiliar->proximo;
                                editoraAuxiliar->proximo = editoraNova;
                            }
                            descritor.qtde++;
                        }
                    }
                }
            }
            fread(&arquivo, sizeof(tpLivroBinario), 1, ponteiroArquivo);
        }
    }
    fclose(ponteiroArquivo);
}

void exibeAutores(tpAutor *autor)
{
    printf("Nome: %s %s\n", autor->nome, autor->sobrenome);
    if (autor->proximo)
        exibeAutores(autor->proximo);
}

void exibeListaAutor(tpListaAutores *lista)
{
    printf("\n%s\t%s\t\n", lista->endereco->nome, lista->endereco->sobrenome);
    if (lista->proximo)
    {
        exibeListaAutor(lista->proximo);
    }
}

void exibirLivro(tpLivros *livros)
{
    printf("\n%s\n", livros->titulo);
    exibeListaAutor(livros->autores);
    if (livros->proximo)
    {
        exibirLivro(livros->proximo);
    }
}

void exibeEditora(tpEditora *editora)
{
    printf("\nNome: %s", editora->editora);
    exibirLivro(editora->livros);
    if (editora->proximo)
        exibeEditora(editora->proximo);
}

void excluirLivroArquivoBin(char livroAExcluir[70])
{
    FILE *ponteiroArquivo = fopen("livrosBinario.dat", "rb+");
    FILE *ponteiroArquivoTemp = fopen("temp.dat", "wb+");
    tpLivroBinario arquivo;
    if (ponteiroArquivo == NULL || ponteiroArquivoTemp == NULL)
    {
        printf("Erro na abertura do arquivo livros.dat na hora de inicializar estrutura!\n");
    }
    else
    {
        rewind(ponteiroArquivo);
        fread(&arquivo, sizeof(tpLivroBinario), 1, ponteiroArquivo);
        while (!feof(ponteiroArquivo))
        {
            if (strcmp(arquivo.tituloLivro, livroAExcluir) != 0)
            {
                fwrite(&arquivo, sizeof(tpLivroBinario), 1, ponteiroArquivoTemp);
            }
            fread(&arquivo, sizeof(tpLivroBinario), 1, ponteiroArquivo);
        }
        fclose(ponteiroArquivo);
        fclose(ponteiroArquivoTemp);
        remove("livrosBinario.dat");
        rename("temp.dat", "livrosBinario.dat");
    }
}

void excluiRecursivo(tpListaAutores *autor)
{
    if (autor->proximo)
    {
        excluiRecursivo(autor->proximo);
    }
    delete autor;
}

void excluirLivroEstrutura(tpDescritor &descritor, char livroAExcluir[70])
{
    if (descritor.qtde == 0)
    {
        printf("\nEstrutura não inicializada!\n");
    }
    else
    {
        tpEditora *editoraAux = descritor.inicio;

        while (editoraAux->proximo != NULL)
        {
            tpLivros *livrosAux = editoraAux->livros;
            while (livrosAux->proximo != NULL && strcmp(livrosAux->titulo, livroAExcluir) != 0)
            {
                livrosAux = livrosAux->proximo;
            }
            if (strcmp(livrosAux->titulo, livroAExcluir) != 0)
            {
                printf("\nLivro nao encontrado na editora %s\n", editoraAux->editora);
            }
            else
            {
                printf("\nLivro encontrado na editora %s!\n", editoraAux->editora);
                excluiRecursivo(livrosAux->autores);
                if (livrosAux->proximo)
                {
                    livrosAux->proximo->anterior = livrosAux->anterior;
                }
                if (livrosAux->anterior)
                {
                    livrosAux->anterior->proximo = livrosAux->proximo;
                }
                else
                {
                    editoraAux->livros = livrosAux->proximo;
                }
                delete livrosAux;
                printf("\nLivro excluido!\n");
            }

            editoraAux = editoraAux->proximo;
        }
    }
}

void excluirLivro(char livroAExcluir[70], tpDescritor &descritor)
{
    excluirLivroArquivoBin(livroAExcluir);
    excluirLivroEstrutura(descritor, livroAExcluir);
}

char menu()
{
    printf("[A] Gerar arquivo binario");
    printf("[B] Exibir arquivo binario");
    printf("[C] Construir estrutura");
    printf("[D] Gerar arquivo binário");
    printf("[E] Gerar arquivo binário");
    printf("[F] Gerar arquivo binário");
    printf("[G] Gerar arquivo binário");
    printf("[H] Gerar arquivo binário");
    return toupper(getche());
}

int main()
{

    tpDescritor descritor;
    inicializarDescritor(descritor);
    tpAutor *autor;
    arquivoTextoParaBinario();
    autor = inicializarEstruturaAutores();
    inicializarEstruturaPrincipal(autor, descritor);
    exibeEditora(descritor.inicio);
    excluirLivro("Estruturas de dados usando C", descritor);

    // exibir();

    exibeEditora(descritor.inicio);
    // exibeAutores(autor);

    return 0;
}

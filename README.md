# Tlist(POSIX)

Implementação C para listas encadeadas e arrays dinâmicos, com suporte a multiplos tipos, gerenciamento seguro de memória e documentação Doxygen.

Esta Biblioteca fornece estruturas de listas e arrays flexiveis que podem armazenar diferentes tipos de dados, incluindo `int`, `float`, `double`, `char*`(string) e ponteiros genéricos(`void*`). A biblioteca copia automaticamente valores para tipos primitivos e strings; para ponteiros genéricos(tipo `T`/`void*`) a lista armazena o ponteiro do valor — Veja a seção "[Gerenciamento de memória](#memoria)" para mais detalhes.

    Status do projeto: Ativo ✅

### Indice
- [Funcionalidades](#funcionalidades)
- [Requisitos]()
- [Instalação]()
- [Compilação]()
- [Uso Rápido]()
    - [Gerenciamento de Memória](#gerenciamento-de-memóriaimportante)
    - [Exemplos]()
- [Documentação(Doxygen)](#documentacao)
- [Licença]()
- [Contatos]()

## Funcionalidades
- Tipagem dinâmica: listas/arrays para `int`, `float`, `double`, `char*` e ponteiros genéricos(`void*`/`T`).
- Gerenciamento de memória para tipos primitivos e strings(valores copiados)
- Operações suportadas:
    - Listas Encadeadas:
        - `new_list`: cria uma nova lista vazia.
        - `list_print`: imprime os elementos da lista.
        - `list_length`: retorna a quantidade de elementos da lista.
        - `list_free`: libera a memória alocada pela lista.
        - `list_push`: adiciona o elemento ao final da lista.
        - `list_pop`: remove e retorna o primeiro elemento da lista.
        - `list_get`: retorna um ponteiro para uma cópia do elemento na posição especificada.
        - `list_set`: define o elemento na posição especificada.
        - `list_delete`: remove o elemento na posição especificada.
        - `list_insert`: insere um elemento na posição especificada.
        - `list_pick`: remove e retorna o elemento na posição especificada.
        - `list_foreach`: executa uma função para cada elemento da lista.
    - Arrays Dinâmicos:
        - `new_array`: cria um novo array vazio.
        - `array_clone`: cria uma cópia do array.
        - `array_print`: imprime os elementos do array.
        - `array_length`: retorna a quantidade de elementos do array.
        - `array_free`: libera a memória alocada pelo array.
        - `array_get`: retorna um ponteiro para o elemento na posição especificada.
        - `array_set`: define o elemento na posição especificada.
        - `array_clear`: remove todos os elementos do array e define como NULL.
        - `array_resize`: altera o tamanho do array.
        - `array_push_back`: adiciona o elemento ao final do array.
        - `array_toString`: define a função de callback com a formatção da string a ser retornada.
        - `array_clearFunctio`: Define a função usada para liberar a memória utilizada por tipos personalizados.

## Requisitos
- CMake
- Compilador C(POSIX)
- (Opcional) Doxygen para gerar documentação da API.
- (Opcional - Apenas MinGW) VCPKG para fácil instalação no windows.

## Compilação
1. Clone o repositório do projeto

    ```bash
    git clone https://github.com/Slot148/Tlist
    cd Tlist
    ```
2. Crie o diretorio de build e compile:

    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```
3. O arquivo `.a` deve ser gerado em `build/lib` conforme CMakeLists. Os `.h` estão em `include/tlist`. (A partir daqui, fica a escolha do usuário adiciona-la ao path do projeto ou do sistema)
    - Para compilar um programa que usa a biblioteca:
        ```bash
        gcc meu_arquivo.c -Iinclude -Llib -ltlist -o meu_programa
        ```
        Observação: Caso instalado via vcpkg verifique [VCPKG]().

## Uso Rápido
1. Inclua o header:

    ```c
    #include "Tlist.h"
    ```

2. Crie a lista(exemplo com inteiros):
    ```c
    int main(){
        List list = new_list(INT);
        list_push(list, 1);
        list_push(list, 2);
        list_push(list, 3);
        list_push(list, 4);
        list_print(list);
        list_free(list);
    }
    ```


## Gerenciamento de Memória(Importante)

---
<div align="center">
    <h1>Work in progress 🚧</h1>
    <h1 style="font-size: 80px">🛠️</h1>
    <h3>Documentation and features are currently being updated. Stay tuned!</h3>
</div>

---
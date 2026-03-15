# Tlist(POSIX)

Implementação C para listas encadeadas e arrays dinâmicos, com suporte a multiplos tipos, gerenciamento seguro de memória e documentação Doxygen.

Esta Biblioteca fornece estruturas de listas e arrays flexiveis que podem armazenar diferentes tipos de dados, incluindo `int`, `float`, `double`, `char*`(string) e ponteiros genéricos(`void*`). A biblioteca copia automaticamente valores para tipos primitivos e strings; para ponteiros genéricos(tipo `T`/`void*`) a lista armazena o ponteiro do valor — Veja a seção "[Gerenciamento de memória](#memoria)" para mais detalhes.

    Status do projeto: Ativo ✅

### Indice
- [Funcionalidades](#funcionalidades)
- [Requisitos](#requisitos)
- [Instalação](#instalação)
- [Uso Rápido](#uso-rápido)
    - [Gerenciamento de Memória](#gerenciamento-de-memóriaimportante)
    - [Exemplos](#exemplo)
- [Licença](./LICENSE)
- [Contatos]()

## Funcionalidades
- Tipagem dinâmica: listas/arrays para `int`, `float`, `double`, `char*` e ponteiros genéricos(`void*`/`T`).
- Gerenciamento de memória para tipos primitivos e strings(valores copiados).
- Fácil gerenciamento de memória através de callbacks para tipos personalizados.

## Requisitos
- CMake
- Compilador C(POSIX)
- (Opcional) Doxygen para gerar documentação da API.

## Instalação
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

É essencial estar atento a objetos proprios adicionados a lista, ela ja abstrai muito do gerenciamento impossibilitando liberar os ponteiros da lista sem que fornela uma função de callback que faça a limpeza de cada objeto.

1. Sempre use a função apropriada para liberar a lista:
    ```c
    List list = new_list();
    list_free(list);
    ```
    Ela garante que a memoria seja liberada corretamente de acordo com o tipo fornecido.
    Evite usar `free()` fornecido pela `<stdlib.h>`.

2. Sempre que usar tipos personalizados forneça um callback:
    ```c
    list_set_clearFunction(list, function_callback);
    ```
    Ela garante que a função de limpeza funcione corretamente para tipos personalizados, na verdade ela sequer funcionará caso não forneça a função. Contudo, mantenha a atenção a variedade de tipos inseridos na lista, ela não espera encontrar dois tipos diferentes de dado, mas se optar por fazer, garanta que a callback consiga lidar com todo dado que encontrar.

3. A lista não guarda valores, apenas ponteiros, então é importante se atentar ao ciclo de vida de um dado, de preferência a dados alocados ao heap para evitar problemas como dangling pointer, double free ou use after free(que pode causar um Segmentation fault).
    ```c
    typedef struct Car{int car_id}Car;

    void function(List l){
        Car* car = malloc(sizeof(Car)); <- ponteiro para estrutura
        car->car_id = 101;
        list_push(l, car);
    }                                   <- seria deletado aqui(stack)

    int main(){
        List l = new_list(T);
        function(l);
        list_print(l);                  <- heap permanece válido
    }
        
    ```

## Exemplo
Uma breve demonstração do funcionamento da lista:
```c
#include "Tlist.h"
#include <stdlib.h>

typedef struct Car{
    int model;
    int year;
}Car;

Car* new_car(int year, int model){
    Car* carro = (Car*) malloc (sizeof(Car));
    carro->year = year;
    carro->model = model;
    return car;
}

void print_car(void* car){
    Car* c = (Car*) car;
    printf("{model: %d, year: %d}", c->model, c->year);
}
void free_car(void* car){
    Car* c = (Car*)car;
    free(c);
}
void upgrade_car(void* car){
    Car* c = (Car*)car;
    c->year++;
}

int main(){
    List list = new_list(T);

    list_set_clearFunction(free_car);
    list_set_toString(print_car);

    Car* car1 = new_car(12, 1212);
    Car* car2 = new_car(11, 1010);

    list_push(list, car1);
    list_push(list, car2);

    list_foreach(list, upgrade_car);
    list_print(list);
    list_free(list);

    return 0;
}

```
---
<div align="center">
    <h1>Work in progress 🚧</h1>
    <h1 style="font-size: 80px">🛠️</h1>
    <h3>Documentation and features are currently being updated. Stay tuned!</h3>
</div>

---
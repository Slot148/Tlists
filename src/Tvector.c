#include "../include/tlist/_Tlist.h"
#include "../include/tlist/Tlist.h"

ArrayList new_array(Type type, int capacity){
    ArrayList list = (ArrayList)malloc(sizeof(struct ArrayList));
    if(list == NULL) {
        fprintf(stderr, "Error in new_array(): Failed to allocate memory for the new list.\n");
        exit(EXIT_FAILURE);
    }
    list->_array = (void **)malloc(capacity * sizeof(void *));
        if (list->_array == NULL) {
        fprintf(stderr, "Error in new_array(): Failed to allocate memory for the array.\n");
        free(list);
        exit(EXIT_FAILURE);
    }
    list->_capacity = capacity;
    list->_length = 0;
    list->_type = type;
    list->to_string = NULL;
    list->clear_function = NULL;

    switch(type){
        case INT: list->_size = sizeof(int); break;
        case STRING: list->_size = sizeof(char *); break;
        case DOUBLE: list->_size = sizeof(double); break;
        case FLOAT: list->_size = sizeof(float); break;
        case T: list->_size = sizeof(void *); break;
    }

    for(int i = 0; i < capacity; i++){
        list->_array[i] = NULL;
    }

    return list;
}

int array_length(ArrayList list){
    return list->_length;
}

void array_set(ArrayList list, int index, ...){
    if (list == NULL) {
        fprintf(stderr, "Error in array_set(): The provided list instance is NULL.\n");
        return;
    }
    if (index < 0 || index >= list->_capacity) {
        fprintf(stderr, "Error in array_set(): Index %d is out of bounds. Valid range is 0 to %d.\n", index, list->_capacity - 1);
        return;
    }
    va_list args;
    va_start(args, index);

    if (list->_array[index] != NULL && list->_type != T) {
        free(list->_array[index]);
    }

    switch (list->_type){
        case INT:{
            list->_array[index] = malloc(sizeof(int));
            *(int*)list->_array[index] = va_arg(args, int);
            break;
        }
        case STRING:{
            char* str = va_arg(args, char *);
            list->_array[index] = malloc(strlen(str) + 1);
            strcpy((char *)list->_array[index], str);
            break;
        }
        case DOUBLE:{
            list->_array[index] = malloc(sizeof(double));
            *(double*)list->_array[index] = va_arg(args, double);
            break;
        }
        case FLOAT:{
            list->_array[index] = malloc(sizeof(float));
            *(float*)list->_array[index] = (float)va_arg(args, double);
            break;
        }
        default:{
            list->_array[index] = va_arg(args, void *);
            break;
        }
    }

    if (index >= list->_length) {
        list->_length = index + 1;
    }
    
    va_end(args);
} 

void array_print(ArrayList list) {
    printf("[");
    for (int i = 0; i < list->_length; i++) {
        if (list->_array[i] != NULL) { 
            if(list->to_string == NULL){
                switch (list->_type) {
                    case INT: printf("%d", *(int *)list->_array[i]); break;
                    case STRING: printf("\"%s\"", (char *)list->_array[i]); break;
                    case DOUBLE: printf("%.2f", *(double *)list->_array[i]); break;
                    case FLOAT: printf("%.2f", *(float *)list->_array[i]); break;
                    case T: printf("%p", list->_array[i]); break;
                }
            }else{
                list->to_string(list->_array[i]);
            }
            
        } else {
            printf("NULL");
        }   
        if (i < list->_length - 1) printf(", ");
    }
    printf("]\n");
}

void array_set_toString(ArrayList list, void(*function)(void*)){
    list->to_string = function;
}
void array_set_clearFunction(ArrayList list ,void(*function)(void*)){
    list->clear_function = function;
}

void array_free(ArrayList list){
    if (list == NULL) return;
    
    if(list->clear_function != NULL){
        for (int i = 0; i < list->_length; i++){
            if (list->_array[i] != NULL) {
                list->clear_function(list->_array[i]);
            }
        }
    }else{
        for (int i = 0; i < list->_length; i++) {
            if (list->_array[i] != NULL && list->_type != T){
                free(list->_array[i]);
            }else{
                fprintf(stderr, "Warning in array_free(): No clear function provided for custom type at index %d. Skipping free operation.\n", i);
            }
        }
    }
    
    free(list->_array);
    free(list);
}

void* array_get(ArrayList list, int index){
    if (list == NULL) {
        fprintf(stderr, "Error in array_get(): The provided list instance is NULL.\n");
        return NULL;
    }
    if (index < 0 || index >= list->_length) {
        fprintf(stderr, "Error in array_get(): Index %d is out of bounds. Valid range is 0 to %d.\n", index, list->_length - 1);
        return NULL;
    }
    return list->_array[index];
}

void array_push(ArrayList list, ...){
    if (list == NULL) {
        fprintf(stderr, "Error in array_push_back(): The provided list instance is NULL.\n");
        return;
    }
    if(list->_length == list->_capacity) {
        fprintf(stderr, "Error in array_push_back(): Out of index.\n");
        return;
    }

    va_list args;
    va_start(args, list);
    int index = list->_length;

    switch (list->_type){
        case INT:{
            list->_array[index] = malloc(sizeof(int));
            *(int*)list->_array[index] = va_arg(args, int);
            break;
        }
        case STRING:{
            char* str = va_arg(args, char *);
            list->_array[index] = malloc(strlen(str) + 1);
            strcpy((char *)list->_array[index], str);
            break;
        }
        case DOUBLE:{
            list->_array[index] = malloc(sizeof(double));
            *(double*)list->_array[index] = va_arg(args, double);
            break;
        }
        case FLOAT:{
            list->_array[index] = malloc(sizeof(float));
            *(float*)list->_array[index] = (float)va_arg(args, double);
            break;
        }
        default:{
            list->_array[index] = va_arg(args, void *);
            break;
        }
    }
    list->_length++;
    va_end(args);
}

void array_resize(ArrayList list, int new_size){
    if (list == NULL) {
        fprintf(stderr, "Error in array_resize(): The provided list instance is NULL.\n");
        return;
    }
    list->_array = (void**)realloc(list->_array, new_size * sizeof(void*));
    if (new_size > list->_capacity) {
        for (int i = list->_capacity; i < new_size; i++) list->_array[i] = NULL;
    }
    list->_capacity = new_size;
    if (list->_length > new_size) list->_length = new_size;
}

void array_clear(ArrayList list){
    if (list == NULL) {
        fprintf(stderr, "Error in array_clear(): The provided list instance is NULL.\n");
        return;
    }
    for(int i = 0; i < list->_length; i++){
        if(list->_array[i] != NULL && list->_type != T){
            free(list->_array[i]);
            list->_array[i] = NULL;
        }else if(list->_array[i] != NULL && list->clear_function != NULL){
            list->clear_function(list->_array[i]);
            list->_array[i] = NULL;
        }else{
            fprintf(stderr, "Warning in array_clear(): No clear function provided for custom type at index %d. Skipping clear operation.\n", i);
        }
    }
    list->_length = 0;    
}

ArrayList array_duplicate(ArrayList list){
    if (list == NULL) {
        fprintf(stderr, "Error in array_clone(): The provided list instance is NULL.\n");
        return NULL;
    }
    ArrayList new = new_array(list->_type, list->_capacity);
    if(list->to_string != NULL) new->to_string = list->to_string;
    if(list->clear_function != NULL) new->clear_function = list->clear_function;
        
    for(int i = 0; i < list->_length; i++){
        switch (list->_type){
            case INT:{
                array_set(new, i, *(int*)list->_array[i]);
                break;
            }
            case STRING:{
                array_set(new, i, (char*)list->_array[i]);
                break;
            }
            case DOUBLE:{
                array_set(new, i, *(double*)list->_array[i]);
                break;
            }
            case FLOAT:{
                array_set(new, i, *(float*)list->_array[i]);
                break;
            }
            default:{
                array_set(new, i, list->_array[i]);
                break;
            }
        }
    }
    return new;
}

void array_foreach(ArrayList list, void(*function)(void*)){
    if(list == NULL){
        fprintf(stderr, "Error in array_clone(): The provided list instance is NULL.\n");
        return;
    }
    for(int i = 0; i < list->_length; i++){
        function(array_get(list, (int)i));
    }
}
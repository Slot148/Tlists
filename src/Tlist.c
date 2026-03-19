#include "Tlist.h"
#include "../include/tlist/_Tlist.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// typedef struct _PointerCollection{
//     void** _pointers;
//     int _pointers_num;
// }_PointerCollection;

// static _PointerCollection *_collection;

// void _init_pointers_collection(void){
//     _collection = malloc(sizeof(struct _PointerCollection));
//     _collection->_pointers = malloc(sizeof(void*) * 40);
//     _collection->_pointers_num = 0;
// }

List new_list(Type type){
    List this = (List)malloc(sizeof(struct List));
    if(this == NULL){
        CATCH_STATUS(FATAL_ERROR, "Failed to allocate memory for the new list.");
        return NULL;
    }
    this->_head = NULL;
    this->_tail = NULL;
    this->_type = type;
    this->_length = 0;
    this->clear_function = NULL;
    this->to_string = NULL;

    switch(type){
        case INT: this->_size = sizeof(int); break;
        case STRING: this->_size = sizeof(char *); break;
        case DOUBLE:this->_size = sizeof(double);break;
        case FLOAT:this->_size = sizeof(float);break;
        case T:this->_size = sizeof(void *);break;
    }
    return this;
}

Node _new_node(void *val, size_t size, Type type){
    Node node = (Node)malloc(sizeof(struct Node));    
    if(node == NULL) {
        CATCH_STATUS(ERROR, "Failed to allocate memory for the new node.");
        return NULL;
    }
    if (type == STRING) {
        if (val == NULL) {
            CATCH_STATUS(ERROR, "Cannot create a STRING node from a NULL pointer.");
            return NULL;
        }
        node->_val = malloc(strlen((char *)val) + 1);
        if (node->_val == NULL) {
            CATCH_STATUS(ERROR, "Cannot create a STRING node from a NULL pointer.");
            return NULL;
        }
        strcpy((char *)node->_val, (char *)val);
    }
    else if (type == T) {
        node->_val = val;
    }
    else {
        node->_val = malloc(size);
        if (node->_val == NULL) {
            CATCH_STATUS(ERROR, "Failed to allocate memory for the node's value.");
            return NULL;
        }
        memcpy(node->_val, val, size);
    }   
    node->_nextNode = NULL;
    return node;
}

void list_dPrint(List this){
    if (this == NULL) {
        CATCH_STATUS(ERROR, "The provided list instance is NULL.");
        return;
    }
    printf("[");
    
    for (Node current = this->_head; current != NULL; current = current->_nextNode){
        switch (this->_type){
            case INT: printf("%d", *(int *)current->_val); break;
            case STRING: printf("\"%s\"", (char *)current->_val); break;
            case DOUBLE: printf("%.2f", *(double *)current->_val); break;
            case FLOAT: printf("%.2f", *(float *)current->_val); break;
            case T: printf("%p", current->_val); break;
        }
        if (current->_nextNode != NULL){
            printf(", ");
        }
    }
    
    printf("]");
    printf("\n");
}
    
void list_free(List this){
    if (this == NULL) {
        CATCH_STATUS(ERROR, "The provided list instance is NULL.");
        return;
    }
    Node current = this->_head;
    while (current != NULL){
        Node temp = current;
        current = temp->_nextNode;
        if(this->clear_function != NULL){
            this->clear_function(temp->_val);
        }else if(this->_type != T){
            free(temp->_val);
        }else{
            CATCH_STATUS(ERROR, "No clear function provided for custom type. Skipping free operation.");
            return;
        }
        
        free(temp);
    }
    this->_head = NULL;
    this->_tail = NULL;
    this->_length = 0;
}

void _under_push(List this, Node node){
    if (this->_head == NULL) {
        this->_head = node;
        this->_tail = node;
    } else {
        this->_tail->_nextNode = node;
        this->_tail = node;
    }
    this->_length++; 
}

void list_push(List this, ...){
    if (this == NULL){
        CATCH_STATUS(ERROR, "The provided list instance is NULL.");
        return;
    } 
    
    va_list args;
    va_start(args, this);
    switch (this->_type){
        case INT:{
            int val = va_arg(args, int);
            _under_push(this, _new_node(&val, this->_size, this->_type));
            break;
        }
        case STRING:{
            char *str = va_arg(args, char *);
            _under_push(this, _new_node(str, this->_size, this->_type));
            break;
        }
        case DOUBLE:{
            double dbl = va_arg(args, double);
            _under_push(this, _new_node(&dbl, this->_size, this->_type));
            break;
        }
        case FLOAT:{
            float flt = (float)va_arg(args, double);
            _under_push(this, _new_node(&flt, this->_size, this->_type));
            break;
        }
        default:{
            void *unkown = va_arg(args, void *);
            _under_push(this, _new_node(unkown, this->_size, this->_type));
            break;
        }
    }
    va_end(args);
}

int list_length(List this){
    if (this == NULL) {
        CATCH_STATUS(ERROR, "The provided list instance is NULL.");
        return 1;
    }
    return this->_length;
}

void *_clone_value(Node node, Type type){
    Node current = node;
    switch (type){
        case INT:{
            void *val = malloc(sizeof(int));
            memcpy(val, current->_val, sizeof(int));
            return val;
            break;
        }
        case FLOAT:{
            void *val = malloc(sizeof(float));
            memcpy(val, current->_val, sizeof(float));
            return val;
            break;
        }
        case DOUBLE:{
            void *val = malloc(sizeof(double));
            memcpy(val, current->_val, sizeof(double));
            return val;
            break;
        }
        case STRING:{
            //TODO: verificar se o valor esta sendo copiado corretamente
            void *val = malloc(strlen((char *)current->_val) + 1);
            strcpy((char *)val, (char *)current->_val);
            return val;
            break;
        }
        case T:{
            //TODO: avisar na dacumentação que o get para tipo T retorna um ponteiro para o valor real e não uma cópia(antigamente não dava para dar get em T)
            //TODO: talvez implementar uma callback para cópia
            CATCH_STATUS(WARNING, "The returned value is a pointer to the original element");
            return node->_val;
        };
    }
    return NULL;
}

void *list_pop(List this){
    if (this == NULL) {
        CATCH_STATUS(ERROR, "The provided list instance is NULL.");
        return NULL;
    }
    if (this->_head == NULL){
        CATCH_STATUS(ERROR, "The provided list do not contain any value.");
        return NULL;
    }else {
        Node current = this->_head;
        this->_head = current->_nextNode;
        void* val;
        if(this->_type == T){
            val = current->_val;
        }else{
            val = _clone_value(current, this->_type);
            free(current->_val);
        }
        free(current);
        this->_length--;
        if (this->_head == NULL) {
            this->_tail = NULL;
        }
        return val;
    }
}

void *list_get(List this, int index){
    if (this == NULL) {
        CATCH_STATUS(ERROR, "The provided list instance is NULL.");
        return NULL;
    }
    if (index < 0) {
        CATCH_STATUS(ERROR, "Index is invalid");
        return NULL;
    }

    int x = 0;
    Node current = this->_head;
    while (current != NULL){
        if (x == index){
            return _clone_value(current, this->_type);
        }
        current = current->_nextNode;
        x++;
    }
    CATCH_STATUS(ERROR, "Index is out of bounds for list");
    return NULL;
}

void list_set(List this, int index, ...){
    if (this == NULL) {
        CATCH_STATUS(ERROR, "The provided list instance is NULL.");
        return;
    }
    if (index < 0) {
        CATCH_STATUS(ERROR, "Index is invalid.");
        return;
    }
    
    va_list args;
    va_start(args, index);
    Node current = this->_head;
    int x = 0;
    while (current != NULL){
        if (x == index) {
            switch (this->_type){
                case INT:{
                    int val = va_arg(args, int);
                    memcpy(current->_val, &val, this->_size);
                    break;
                }
                case FLOAT:{
                    float flt = (float)va_arg(args, double);
                    memcpy(current->_val, &flt, this->_size);
                    break;
                }
                case DOUBLE:{
                    double dbl = va_arg(args, double);
                    memcpy(current->_val, &dbl, this->_size);
                    break;
                }
                case STRING:{
                    char *chr = va_arg(args, char *);
                    free(current->_val);
                    current->_val = malloc(strlen(chr) + 1);
                    if (current->_val == NULL) {
                        CATCH_STATUS(ERROR, "Failed to allocate memory for the new string value.");
                        return;
                    }
                    strcpy((char *)current->_val, chr);
                    break;
                }
                case T:{
                    void *nil = va_arg(args, void *);
                    current->_val = nil;
                    break;
                }
            }
            va_end(args);
            return;
        }
        current = current->_nextNode;
        x++;
    }
    CATCH_STATUS(ERROR, "Index is out of bounds for list of size.");
    va_end(args);
}

void list_delete(List this, int index){
    if (this == NULL) {
        CATCH_STATUS(ERROR, "The provided list instance is NULL.");
        return;
    }
    if (index < 0) {
        CATCH_STATUS(ERROR, " Index is invalid.");
        return;
    }

    if (index == 0) {
        Node temp = this->_head;
        this->_head = temp->_nextNode;
        if (this->_head == NULL) this->_tail = NULL;
        if(this->_type != T) free(temp->_val);
        free(temp);
        this->_length--;
        return;
    }
    Node current = this->_head;
    int x = 0;
    while (current != NULL){
        if (index - 1 == x && index != 0){
            if (current->_nextNode == NULL) break;
            Node temp = current->_nextNode;
            current->_nextNode = temp->_nextNode;
            if (temp == this->_tail) {
                this->_tail = current;
            }
            if(this->_type != T) free(temp->_val);
            free(temp);
            this->_length--;
            return;
        }
        current = current->_nextNode;
        x++;
    }
    CATCH_STATUS(ERROR, "Index is out of bounds for list of size");
}

void _under_insert(List this, int index, Node node){
    if (index == 0){
        node->_nextNode = this->_head;
        this->_head = node;
        this->_length++;
        if (this->_tail == NULL) {
            this->_tail = node;
        }
        return;
    }

    Node current = this->_head;
    int x = 0;
    while (current != NULL){
        if (x == index - 1){
            Node temp = current->_nextNode;
            current->_nextNode = node;
            node->_nextNode = temp;
            this->_length++;
            return;
        }
        current = current->_nextNode;
        x++;
    }
}

void list_insert(List this, int index, ...){
    if (this == NULL) {
        CATCH_STATUS(ERROR, "The provided list instance is NULL.");
        return;
    }

    int list_len = list_length(this);
    if (index < 0 || index > list_len) {
        CATCH_STATUS(ERROR, "Index is out of bounds");
        return;
    }

    va_list args;
    va_start(args, index);

    switch (this->_type){
        case INT:{
            int val = va_arg(args, int);
            _under_insert(this, index, _new_node(&val, this->_size, this->_type));
            break;
        }
        case STRING:{
            char *str = va_arg(args, char *);
            _under_insert(this, index, _new_node(str, this->_size, this->_type));
            break;
        }
        case DOUBLE:{
            double dbl = va_arg(args, double);
            _under_insert(this, index, _new_node(&dbl, this->_size, this->_type));
            break;
        }
        case FLOAT:{
            float flt = (float)va_arg(args, double);
            _under_insert(this, index, _new_node(&flt, this->_size, this->_type));
            break;
        }
        default:{
            void *unkown = va_arg(args, void *);
            _under_insert(this, index, _new_node(unkown, this->_size, this->_type));
            break;
        }
    }

    va_end(args);
}

void *list_pick(List this, int index){
    if (this == NULL) {
        CATCH_STATUS(ERROR, "The provided list instance is NULL");
        return NULL;
    }
    if (index < 0) {
        CATCH_STATUS(ERROR, "Index is invalid.");
        return NULL;
    }

    if(index == 0){
        return list_pop(this);
    }

    Node current = this->_head;
    int x = 0;
    while (current != NULL){
        if (x == index - 1 && current->_nextNode != NULL){
            Node temp = current->_nextNode;
            current->_nextNode = temp->_nextNode;
            if (temp == this->_tail) {
                this->_tail = current;
            }
            void* n;
            if(this->_type == T){
                n = temp->_val;
            }else{
                n = _clone_value(temp, this->_type);
                free(temp->_val);
            }
            free(temp);
            this->_length--;
            return n;
        }
        current = current->_nextNode;
        x++;
    }

    fprintf(stderr, "Error in pick(): Index %d is out of bounds for list of size %d.\n", index, x);
    CATCH_STATUS(ERROR, "Index is out of bounds for list of size.");
    return NULL;
}

void list_foreach(List this, void(*function)(void*)){
    if (this == NULL) {
        CATCH_STATUS(ERROR, "The provided list instance is NULL.");
        return;
    }
    for(Node current = this->_head; current != NULL; current = current->_nextNode){
        function(current->_val);
    }
}

List list_duplicate(const List this){
    if (this == NULL) {
        CATCH_STATUS(ERROR, "The provided list instance is NULL.");
        return NULL;
    }
    List list = new_list(this->_type);
    TIterator iterator = new_iterator(this);
    
    if(this->to_string != NULL) list->to_string = this->to_string;
    if(this->clear_function != NULL) list->clear_function = this->clear_function;
    
    while(iterator_has_next(iterator)){
        void* val = iterator_next(iterator);
        switch (this->_type){
            //TODO: revisar se isso não esta copiando o ponteiro inves do valor
            case INT: list_push(list, *(int *)val); break;
            case FLOAT: list_push(list, *(float *)val); break;
            case DOUBLE: list_push(list, *(double *)val); break;
            case STRING: list_push(list, (char *)val); break;
            case T: list_push(list, val); break;
        }
    }
    iterator_free(iterator);
    return list;
}

void list_set_toString(List list, char*(*function)(void*)){
    if (list == NULL) {
        CATCH_STATUS(ERROR, "The provided list instance is NULL.");
        return;
    }
    list->to_string = function;
    return;
}       

void list_set_clearFunction(List list, void(*function)(void*)){
    if (list == NULL) {
        CATCH_STATUS(ERROR, "The provided list instance is NULL.");
        return;
    }
    list->clear_function = function;
    return;
}

char* _append_format(Type type, void* val){
    char* buffer = NULL;
    switch(type){
        case INT:{
            int len = snprintf(NULL, 0, "%d", *(int*)val) + 1;
            buffer = malloc(len);
            if(buffer)sprintf(buffer, "%d", *(int*)val);
            break;
        }
        case STRING:{
            buffer = malloc(strlen(val) + 1);
            strcpy(buffer, (char*)val);
            break;
        }
        case T:{
            int len = snprintf(NULL, 0, "%p", val) + 1;
            buffer = malloc(len);
            if(buffer)sprintf(buffer, "%p", val);
            break;
        }
        default:{
            buffer = malloc(1);
            if (buffer) buffer[0] = '\0';
            break;
        }
    }
    return buffer;
}

size_t _string_list_size(List list){
    if (list == NULL) {
        CATCH_STATUS(ERROR, "The provided list instance is NULL.");
        return 0;
    }
    size_t size = 0;
    for(Node current = list->_head; current != NULL; current = current->_nextNode){
        switch(list->_type){
            case INT:{
                size += snprintf(NULL, 0, "%d", *(int*)current->_val);
                break;
            }
            case DOUBLE:{
                size += snprintf(NULL, 0, "%f", *(double*)current->_val);
                break;
            }
            case FLOAT:{
                size += snprintf(NULL, 0, "%f", *(float*)current->_val);
                break;
            }
            case STRING:{
                size += strlen((char*)current->_val);
                break;
            }
            case T:{
                size += snprintf(NULL, 0, "%p", current->_val);
                break;
            }
        }
        if(current->_nextNode){
            size += 2;
        }
    }
    return size;
}

char* _string_list_concat(List list){
    if (list == NULL) {
        CATCH_STATUS(ERROR, "The provided list instance is NULL.");
        return NULL;
    }
    
    if(!list){
        CATCH_STATUS(ERROR, "The provided list instance is NULL.\n");
        return NULL;
    }
    char* str = malloc(_string_list_size(list));
    if(!str){
        CATCH_STATUS(ERROR, "Fail to memory allocate for string");
        return NULL;
    }
    
    char* ptr = str;
    *ptr++ = '[';
    
    for(Node current = list->_head; current != NULL; current = current->_nextNode){
        switch(list->_type){
            case INT:{
                ptr += sprintf(ptr, "%d", *(int*)current->_val);                
                break;
            }
            case DOUBLE:{
                ptr += sprintf(ptr, "%f", *(double*)current->_val);                
                break;
            }
            case FLOAT:{
                ptr += sprintf(ptr, "%f", *(float*)current->_val);                
                break;
            }
            case STRING:{
                ptr += sprintf(ptr, "%s", (char*)current->_val);                
                break;
            }
            case T:{
                ptr += sprintf(ptr, "%p", current->_val);                
                break;
            }
        }
        if(current->_nextNode){
            ptr += sprintf(ptr, "%s", ", ");
        }
    }
    *ptr++ = ']';
    *ptr = '\0';
    return str;
}


List _string_list_converter(List list){
    if (list == NULL) {
        CATCH_STATUS(ERROR, "The provided list instance is NULL.");
        return NULL;
    }

    List l = new_list(STRING);
    for(Node current = list->_head; current != NULL; current = current->_nextNode){
        list_push(l, list->to_string(current->_val));
    }
    return l;
}

char* list_toString(List list){
    if (list == NULL) {
        CATCH_STATUS(ERROR, "The provided list instance is NULL.");
        return NULL;
    }

    if (list->to_string) {
        return _string_list_concat(_string_list_converter(list));
    }
    return _string_list_concat(list);
}

void list_print(List list){
    if (list == NULL) {
        CATCH_STATUS(ERROR, "The provided list instance is NULL.");
        return;
    }
    char* str = list_toString(list);
    fprintf(stdout, "%s\n", str);
    free(str);
    return;
}
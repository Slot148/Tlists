#include "../include/tlist/TlistPrivate.h"
#include "../include/tlist/Tlist.h"

List new_list(Type type){
    List this = (List)malloc(sizeof(struct List));
    if(this == NULL) CATCH_STATUS(FATAL_ERROR, "Failed to allocate memory for the new list.");
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
    if(node == NULL) CATCH_STATUS(ERROR, "Failed to allocate memory for the new node.");
    if (type == STRING) {
        if (val == NULL) {
            fprintf(stderr, "Error in _new_node(): Cannot create a STRING node from a NULL pointer.\n");
            exit(EXIT_FAILURE);
        }
        node->_val = malloc(strlen((char *)val) + 1);
        if (node->_val == NULL) {
            fprintf(stderr, "Error in _new_node(): Failed to allocate memory for the node's string value.\n");
            exit(EXIT_FAILURE);
        }
        strcpy((char *)node->_val, (char *)val);
    }
    else if (type == T) {
        node->_val = val;
    }
    else {
        node->_val = malloc(size);
        if (node->_val == NULL) {
            fprintf(stderr, "Error in _new_node(): Failed to allocate memory for the node's value.\n");
            exit(EXIT_FAILURE);
        }
        memcpy(node->_val, val, size);
    }   
    node->_nextNode = NULL;
    return node;
}

void list_print(List this){
    if (this == NULL) {
        fprintf(stderr, "Error in print(): The provided list instance is NULL.\n");
        return;
    }
    printf("[");
        if(this->to_string == NULL){
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
        }else{
            for (Node current = this->_head; current != NULL; current = current->_nextNode){
                this->to_string(current->_val);    
                if (current->_nextNode != NULL){
                    printf(", ");
                }
            }
        }
    printf("]");
    printf("\n");
}

void list_free(List this){
    if (this == NULL) {
        fprintf(stderr, "Error in destroyList(): The provided list instance is NULL.\n");
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
            fprintf(stderr, "Warning in list_free(): No clear function provided for custom type. Skipping free operation.\n");
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
    if (this == NULL) CATCH_STATUS(ERROR, "Error in push(): The provided list instance is NULL.");
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
        fprintf(stderr, "Error in len(): The provided list instance is NULL.\n");
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
            void *val = malloc(strlen((char *)current->_val) + 1);
            strcpy((char *)val, (char *)current->_val);
            return val;
            break;
        }
        case T:{
            fprintf(stderr, "Error: _clone_value should not be called for type T\n");
            return NULL;
        };
    }
    return NULL;
}

void *list_pop(List this){
    if (this == NULL) {
        fprintf(stderr, "Error in pop(): The provided list instance is NULL.\n");
        return NULL;
    }
    if (this->_head == NULL){
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
        fprintf(stderr, "Error in get(): The provided list instance is NULL.\n");
        return NULL;
    }
    if (index < 0) {
        fprintf(stderr, "Error in get(): Index %d is negative and invalid.\n", index);
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
    fprintf(stderr, "Error in get(): Index %d is out of bounds for list of size %d.\n", index, x);
    return NULL;
}

void list_set(List this, int index, ...){
    if (this == NULL) {
        fprintf(stderr, "Error in set(): The provided list instance is NULL.\n");
        return;
    }
    if (index < 0) {
        fprintf(stderr, "Error in set(): Index %d is negative and invalid.\n", index);
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
                        fprintf(stderr, "Error in set(): Failed to allocate memory for the new string value.\n");
                        exit(EXIT_FAILURE);
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
    fprintf(stderr, "Error in set(): Index %d is out of bounds for list of size %d.\n", index, x);
    va_end(args);
}

void list_delete(List this, int index){
    if (this == NULL) {
        fprintf(stderr, "Error in delete(): The provided list instance is NULL.\n");
        return;
    }
    if (index < 0) {
        fprintf(stderr, "Error in delete(): Index %d is negative and invalid.\n", index);
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
    fprintf(stderr, "Error in delete(): Index %d is out of bounds for list of size %d.\n", index, x);
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
        fprintf(stderr, "Error in insert(): The provided list instance is NULL.\n");
        return;
    }

    int list_len = list_length(this);
    if (index < 0 || index > list_len) {
        fprintf(stderr, "Error in insert(): Index %d is out of bounds. Valid range is 0 to %d.\n", index, list_len);
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
        fprintf(stderr, "Error in pick(): The provided list instance is NULL.\n");
        return NULL;
    }
    if (index < 0) {
        fprintf(stderr, "Error in pick(): Index %d is negative and invalid.\n", index);
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
    return NULL;
}

void list_foreach(List this, void(*function)(void*)){
    if (this == NULL) {
        fprintf(stderr, "Error in foreach(): The provided list instance is NULL.\n");
        return;
    }
    for(Node current = this->_head; current != NULL; current = current->_nextNode){
        function(current->_val);
    }
}

List list_duplicate(const List this){
    if (this == NULL) {
        fprintf(stderr, "Error in duplicate(): The provided list instance is NULL.\n");
        return NULL;
    }
    List list = new_list(this->_type);
    TIterator iterator = new_iterator(this);
    
    if(this->to_string != NULL) list->to_string = this->to_string;
    if(this->clear_function != NULL) list->clear_function = this->clear_function;

    while(iterator_has_next(iterator)){
        void* val = iterator_next(iterator);
        switch (this->_type){
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

void list_set_toString(List list, void(*function)(void*)){
    list->to_string = function;
    return;
}       

void list_set_clearFunction(List list, void(*function)(void*)){
    list->clear_function = function;
    return;
}
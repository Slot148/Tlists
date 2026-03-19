#include "../include/tlist/_Tlist.h"
#include "../include/tlist/Tlist.h"

TIterator new_iterator(const List list){
    TIterator iterator = malloc(sizeof(struct TIterator));
    if(iterator == NULL) {
        fprintf(stderr, "Error in newIterator(): Failed to allocate memory for the new iterator.\n");
        exit(EXIT_FAILURE);
    }
    if (list == NULL) {
        fprintf(stderr, "Error in newIterator(): The provided list instance is NULL.\n");
        exit(EXIT_FAILURE);
    }
    iterator->_list = list;
    iterator->_current = list->_head;
    iterator->_index = 0;
    return iterator;
}

void* iterator_next(TIterator iterator){
    if (iterator == NULL || iterator->_current == NULL) {
        fprintf(stderr, "Error in next(): No more elements to iterate or invalid iterator.\n");
        return NULL;
    }
    void* val = iterator->_current->_val;
    iterator->_index++;
    iterator->_current = iterator->_current->_nextNode;
    return val;
}

int iterator_has_next(TIterator iterator){
    if (iterator == NULL) {
        return false;
    }
    return iterator->_current != NULL;
}

void iterator_free(TIterator iterator){
    free(iterator);
}

void iterator_reset(TIterator iterator){
    iterator->_current = iterator->_list->_head;
    iterator->_index = 0;
}

void _catch_status(Status status_code, const char* message, const char* file, int line, const char* function){
    static const char* status[] = {
        [OK] = "OK", 
        [WARNING] = "WARNING",
        [ERROR] = "ERROR",
        [FATAL_ERROR] = "FATAL ERROR"
    };
    FILE* stream = (status_code == ERROR || status_code == WARNING) ? stderr : stdout;
    fprintf(stream, "(%s)[%s:%d in %s()] %s\n", status[status_code], file, line, function, message);
    if(status_code == ERROR) return;
    if(status_code == FATAL_ERROR) exit(EXIT_FAILURE);
}
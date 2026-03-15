#ifndef T_LIST_PRIVATE
#define T_LIST_PRIVATE

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "Tlist.h"

typedef enum Status{OK, WARNING, ERROR, FATAL_ERROR}Status;
struct Node{
    void *_val;      
    Node _nextNode;  
};

struct TNode{
    void* _value;
    TNode _left;
    TNode _right;
};

struct TIterator{
    Node _current;   
    List _list;      
    int _index;     
};

struct List{
    Node _head;    
    Node _tail;     
    Type _type;
    size_t _size;
    int _length;
    void(*to_string)(void*);
    void(*clear_function)(void*);
};

struct Tree{
    TNode _root;
    Type _type;
    size_t _size;
    int _length;
    int(*condition)(void*, void*);
    void(*to_string)(void*);
    void(*clear_function)(void*);
};

struct ArrayList{
    void** _array;
    int _capacity;
    Type _type;
    size_t _size;
    int _length;
    void(*to_string)(void*);
    void(*clear_function)(void*);
};

/** @private */
Node _new_node(void *val, size_t size, Type type);
TNode _new_tnode(void* val, Type type, size_t size);
void _under_push(List this, Node node);
void _under_tpush(Tree tree, TNode new_node);
void _under_insert(List this, int index, Node node);
void* _clone_value(Node node, Type type);
void _catch_status(Status status_code, const char* message, const char* file, int line, const char* function);

#define CATCH_STATUS(status_code, message) _catch_status(status_code, message, __FILE_NAME__, __LINE__, __func__)

#endif
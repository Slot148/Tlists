#include "../include/tlist/_Tlist.h"
#include "../include/tlist/Tlist.h"

Tree new_tree(Type type){
    Tree tree = malloc(sizeof(struct Tree));
    if(tree == NULL) CATCH_STATUS(ERROR, "Failed to allocate memory for the new tree.");

    tree->_length = 0;
    tree->_type = type;
    tree->_root = NULL;
    tree->to_string = NULL;
    tree->clear_function = NULL;
    tree->condition_function = NULL;

    switch(type){
        case INT: tree->_size = sizeof(int); break;
        case STRING: tree->_size = sizeof(char *); break;
        case DOUBLE:tree->_size = sizeof(double);break;
        case FLOAT:tree->_size = sizeof(float);break;
        case T:tree->_size = sizeof(void *);break;
    }
    return tree;
}

TNode _new_tnode(void* val, Type type, size_t size){
    TNode node = malloc(sizeof(struct TNode));
    if(node == NULL)CATCH_STATUS(ERROR, "Failed to allocate memory for the new tree node.");
    if(type == STRING){
        if(val == NULL)CATCH_STATUS(ERROR, "Cannot create a STRING node from a NULL pointer.");
        node->_value = malloc(strlen((char*)val)+1);
        if(node->_value == NULL)CATCH_STATUS(ERROR, "Error in _new_node(): Failed to allocate memory for the node's string value.");
        strcpy((char*)node->_value,(char*)val);
    }else if (type == T) {
        node->_value = val;
    }else{
        node->_value = malloc(size);
        if(val == NULL)CATCH_STATUS(ERROR, "Cannot create a STRING node from a NULL pointer.");
        memcpy(node->_value, val, size);
    }
    node->_left = NULL;
    node->_right = NULL;
    return node;
}

void _add_newNode(TNode node, TNode new_node, int(*function)(void*, void*)){
    TNode current = node;

    if(function(current->_value, new_node->_value)){
        if(current->_left == NULL){
            current->_left = new_node;
            return;
        }else{
            _add_newNode(current->_left, new_node, function);
        }
    }else{
        if(current->_right == NULL){
            current->_right = new_node;
            return;
        }else {
            _add_newNode(current->_right, new_node, function);
        }
    }

}

void _under_tpush(Tree tree, TNode new_node){
    if(tree->_root == NULL){
        tree->_root = new_node;
        return;
    }
    _add_newNode(tree->_root, new_node, tree->condition_function);
}

void tree_push(Tree tree, ...){
    if(tree == NULL)CATCH_STATUS(ERROR, "The provided tree instance is NULL.");
    if(tree->condition_function == NULL)CATCH_STATUS(ERROR, "THe condition function is not provided.");
    
    size_t size = tree->_size;

    va_list args;
    va_start(args, tree);

    switch(tree->_type){
        case INT:{
            int val = va_arg(args, int);
            _under_tpush(tree, _new_tnode(&val, INT, size));
            break;
        }
        case FLOAT:{
            int val = (float)va_arg(args, double);
            _under_tpush(tree, _new_tnode(&val, FLOAT, size));
            break;
        }
        case DOUBLE:{
            double val = va_arg(args, double);
            _under_tpush(tree, _new_tnode(&val, DOUBLE, size));
            break;
        }
        case STRING:{
            char* val = va_arg(args, char*);
            _under_tpush(tree, _new_tnode(val, STRING, size));
            break;
        }
        default:{
            void* val = va_arg(args, void*);
            _under_tpush(tree, _new_tnode(val, T, size));
            break;
        }
    }
    va_end(args);
}

void tree_set_checkCondition(Tree tree, Condition condition){
    tree->condition_function = condition;
}

void tree_set_toString(Tree tree, Callback to_string){
    tree->to_string = to_string;
}

void tree_set_cleanFunction(Tree tree , Callback clear_function){
    tree->clear_function = clear_function;
}
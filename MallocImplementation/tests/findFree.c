// Unit test to test find_free function

#include <stdlib.h>

int main(int argc, char *argv[]){
    struct blcok *head = NULL;
    struct block *last = NULL;
    head = grow_heap(last, 10);
    assert(head->size == 10); // returns correct pointer
    assert(last->next->size == head->size); // updates value of lsat
    while (last) last = last->next;
    head = grow_heap(last, 20);
    assert(head->size == 20);
    assert(last->next->size, head->size);
    return EXIT_SUCCESS;
}

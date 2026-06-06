#include "../Shared/print.h"

#define HEAP_SIZE 262144

unsigned char heap[HEAP_SIZE];
static int heap_used = 0;

typedef struct {
    unsigned long long *data;
    int size; //cantidad de elementos usados
    int capacity; //espacio total disponible
} ClemArrayDynamic;

typedef struct {
    int size;
    int free;
} BlockHeader;

void *clem_malloc(int size)
{
    int pos = 0;

    while(pos < heap_used)
    {
        BlockHeader *header = (BlockHeader*)&heap[pos];

        if(header->free && header->size >= size)
        {
            unsigned long int remaining = header->size - size;
            
            if(remaining > sizeof(BlockHeader)) {
                BlockHeader *new_header =(BlockHeader*)
                (
                    (unsigned char*)(header + 1)
                    + size
                );

                new_header->size =
                remaining - sizeof(BlockHeader);
                new_header->free = 1;

                header->size = size;
            }

            header->free = 0;

            return (void*)(header + 1);
        }
        pos +=
            sizeof(BlockHeader)
            + header->size;
    }

    int total_size = sizeof(BlockHeader) + size;

    if(heap_used + total_size > HEAP_SIZE)
    {
        return 0;
    }

    BlockHeader *header = (BlockHeader*)&heap[heap_used];

    header->size = size;
    header->free = 0;

    heap_used += total_size;

    return (void*)(header + 1);

}

void coalesce(){
    int pos = 0;

    while(pos < heap_used)
    {
        BlockHeader *current =
            (BlockHeader*)&heap[pos];

        int next_pos =
            pos
            + sizeof(BlockHeader)
            + current->size;

        if(next_pos >= heap_used)
        {
            break;
        }

        BlockHeader *next =
            (BlockHeader*)&heap[next_pos];

        if(current->free &&
           next->free)
        {
            current->size +=
                sizeof(BlockHeader)
                + next->size;

            continue;
        }

        pos = next_pos;
    }
}

void shrink_heap()
{
    int pos = 0;

    BlockHeader *last = 0;
    int last_pos = 0;

    while(pos < heap_used)
    {
        BlockHeader *header =
            (BlockHeader*)&heap[pos];

        last = header;
        last_pos = pos;

        pos +=
            sizeof(BlockHeader)
            + header->size;
    }

    if(last && last->free)
    {
        heap_used = last_pos;
    }
}

void clem_free(void *ptr)
{
    if(ptr == 0)
    {
        return;
    }

    BlockHeader *header = ((BlockHeader*)ptr) - 1;

    header->free = 1;

    coalesce();

    shrink_heap();
}

void clem_free_all()      // rápida
{
    heap_used = 0;
}

void clem_free_all_slow() // segura
{
    for(int i = 0; i < heap_used; i++)
    {
        heap[i] = 0xDD;
    }

    heap_used = 0;
}

int clem_block_size(void *ptr)
{
    BlockHeader *header = ((BlockHeader *)ptr) - 1;

    return header->size;
}

unsigned long long clem_get(ClemArrayDynamic *arr, int index){
    if(index < 0 || index >= arr->size){
        return 0; // fuera de rango (valor por defecto)
    }

    return arr->data[index];
}

void *clem_realloc(void *ptr, int new_size)
{
    if(ptr == 0)
    {
        return clem_malloc(new_size);
    }

    int old_size = clem_block_size(ptr);

    if(old_size >= new_size)
    {
        return ptr;
    }

    void *new_ptr = clem_malloc(new_size);

    if(new_ptr == 0)
    {
        return 0;
    }

    unsigned char *src = ptr;
    unsigned char *dst = new_ptr;

    for(int i = 0; i < old_size; i++)
    {
        dst[i] = src[i];
    }

    clem_free(ptr);

    return new_ptr;
}

int clem_push(ClemArrayDynamic *arr, unsigned long long value){
    
    if(arr->size >= arr->capacity)
    {
        int new_capacity = arr->capacity * 2;

        unsigned long long *new_data =
            clem_realloc(
                arr->data,
                new_capacity * sizeof(unsigned long long)
            );

        if(new_data == 0)
        {
            return 0;
        }

        arr->data = new_data;
        arr->capacity = new_capacity;
    }

    arr->data[arr->size++] = value;

    return 1;
}

void dump_heap()
{
    int pos = 0;

    while(pos < heap_used)
    {
        BlockHeader *header =
            (BlockHeader*)&heap[pos];

        clem_print(PRINT_STRING,
                   "\n----- BLOCK -----");

        clem_print(PRINT_STRING,
                   "\nOffset:");
        clem_print(PRINT_INT,
                   &pos);

        clem_print(PRINT_STRING,
                   "\nSize:");
        clem_print(PRINT_INT,
                   &header->size);

        clem_print(PRINT_STRING,
                   "\nFree:");
        clem_print(PRINT_INT,
                   &header->free);

        pos +=
            sizeof(BlockHeader)
            + header->size;
    }
}

void dump_heap_per_byte()
{
    clem_print(PRINT_STRING, "\n\n=== DUMP HEAP PER BYTE ===");

    for(int i = 0; i < heap_used; i++)
    {
        unsigned long long value = heap[i];

        clem_print(PRINT_STRING,"\nByte:");
        clem_print(PRINT_INT,&i);

        clem_print(PRINT_STRING," = ");
        clem_print(PRINT_INT,&value);
    }
}

ClemArrayDynamic clem_darray_init()
{
    ClemArrayDynamic arr;

    arr.capacity = 1;
    arr.size = 0;
    arr.data = clem_malloc(sizeof(unsigned long long));

    return arr;
}

int main(){

    ClemArrayDynamic myArray = clem_darray_init();

    clem_push(&myArray, 1);
    clem_push(&myArray, 'G');
    unsigned long long value_in_0 = clem_get(&myArray, 0); 
    unsigned long long value_in_1 = clem_get(&myArray, 1);    

    clem_print(PRINT_STRING, "\nValor pos 0: ");
    clem_print(PRINT_INT,&value_in_0);
    clem_print(PRINT_STRING, "\nValor pos 1: ");
    clem_print(PRINT_STRING,&value_in_1);

    dump_heap_per_byte();
    int *x = clem_malloc(4);
    dump_heap_per_byte();
    char *str = clem_malloc(10);
    dump_heap_per_byte();
    clem_print(PRINT_STRING, "\n");
    clem_print(PRINT_INT,&x);
    clem_print(PRINT_STRING, "\n");
    clem_print(PRINT_INT,&str);

    dump_heap();

    int *test_free = clem_malloc(sizeof(int));
    *test_free = 42;
    dump_heap_per_byte();
    clem_print(PRINT_STRING, "\n");
    clem_print(PRINT_INT,&test_free);

    dump_heap();

    clem_free(test_free);

    clem_print(PRINT_STRING, "\n\nDUMP HEAD after free\n");
    dump_heap();

    char *too_big_not_replace_test_free = clem_malloc(10);
    *too_big_not_replace_test_free = 'B';
    dump_heap_per_byte();
    clem_print(PRINT_STRING, "\n");
    clem_print(PRINT_INT,&too_big_not_replace_test_free);

    dump_heap();

    int *replace_test_free = clem_malloc(sizeof(int));
    *replace_test_free = 12;
    dump_heap_per_byte();
    clem_print(PRINT_STRING, "\n");
    clem_print(PRINT_INT,&replace_test_free);

    dump_heap();

    void *test_big_split_realocattion = clem_malloc(100);
    clem_free(test_big_split_realocattion);
    void *test_takes_50_not_100 = clem_malloc(50);
    clem_print(PRINT_STRING, "\n");
    clem_print(PRINT_INT,&test_takes_50_not_100);

    dump_heap();

    void *test_unir_bloques1 = clem_malloc(20);
    clem_print(PRINT_STRING, "\n\nPRIMER BLOQUE A UNIR\n");
    dump_heap();
    void *test_unir_bloques2 = clem_malloc(20);
    clem_print(PRINT_STRING, "\n\nSEGUNDO BLOQUE A UNIR\n");
    dump_heap();

    clem_free(test_unir_bloques1);
    clem_free(test_unir_bloques2);

    clem_print(PRINT_STRING, "\n\nBLOQUE FINAL LIBRE BORRADO\n");
    clem_print(PRINT_STRING, "\n\nBLOQUE EN OFFSET 118 NO EXISTE\n");


    clem_print(PRINT_STRING, "\n\nTEST REALOCACION MYARRAY\n");
    ClemArrayDynamic myArrayDynamic = clem_darray_init();

    clem_push(&myArrayDynamic, 2);
    clem_push(&myArrayDynamic, 'H');

    unsigned long long value_in_0_d = clem_get(&myArrayDynamic, 0); 
    unsigned long long value_in_1_d = clem_get(&myArrayDynamic, 1);    

    clem_print(PRINT_STRING, "\nValor pos 0: ");
    clem_print(PRINT_INT,&value_in_0_d);
    clem_print(PRINT_STRING, "\nValor pos 1: ");
    clem_print(PRINT_STRING,&value_in_1_d);

    clem_push(&myArrayDynamic, 3);
    clem_push(&myArrayDynamic, 'J');

    unsigned long long value_in_2_d = clem_get(&myArrayDynamic, 2); 
    unsigned long long value_in_3_d = clem_get(&myArrayDynamic, 3);    

    clem_print(PRINT_STRING, "\nValor pos 2: ");
    clem_print(PRINT_INT,&value_in_2_d);
    clem_print(PRINT_STRING, "\nValor pos 3: ");
    clem_print(PRINT_STRING,&value_in_3_d);

    dump_heap_per_byte();
    clem_free_all_slow();

    clem_print(PRINT_STRING, "\nTEST: ");
    ClemArrayDynamic testArray = clem_darray_init();

    for (int i = 0; i < 5000; i++){
        clem_push(&testArray, i);
    }

    int no_error = 1;
    for (int i = 0; i < 5000; i++){
        unsigned long long t1 = clem_get(&testArray, i);
        if (t1 != (unsigned long long)i){
            clem_print(PRINT_STRING, "\nERROR EN TEST\n");
            clem_print(PRINT_INT, &t1);
            clem_print(PRINT_STRING, " != ");
            clem_print(PRINT_INT, &i);
            no_error = 0;
        }
    }
    if (no_error){
        clem_print(PRINT_STRING, "\nNo hubo error en el test\n");
    }

    clem_free_all_slow();
    clem_print(PRINT_STRING, "\nFREE HEAP ENTERO");
    dump_heap_per_byte();

}






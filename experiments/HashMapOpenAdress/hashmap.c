#include "../Shared/print.h"

#define CAPACITY 16

typedef enum
{
    EMPTY,
    OCCUPIED,
    DELETED
} BucketState;

// key == 0 significa que no esta usado
struct Bucket
{
    char* key;
    int value;
    BucketState state;
};

struct Bucket table[CAPACITY];


unsigned int hash(char* str)
{
    unsigned int h = 2166136261u;

    while(*str)
    {
        h ^= (unsigned char)(*str);
        h *= 16777619u;
        str++;
    }

    return h;
}

int streq(char* a, char* b)
{
    while(*a && *b)
    {
        if(*a != *b)
            return 0;

        a++;
        b++;
    }

    return *a == *b;
}

void put(char* key, int value)
{
    int index = hash(key) % CAPACITY;

    int visited = 0;
    int first_deleted = -1;

    while(table[index].state != EMPTY)
    {
        if(table[index].state == OCCUPIED &&
           streq(table[index].key, key))
        {
            table[index].value = value;
            return;
        }

        if(table[index].state == DELETED &&
           first_deleted == -1)
        {
            first_deleted = index;
        }

        index++;

        if(index >= CAPACITY)
            index = 0;

        visited++;

        if(visited >= CAPACITY)
            break;
    }

    if(first_deleted != -1)
    {
        index = first_deleted;
    }

    table[index].key = key;
    table[index].value = value;
    table[index].state = OCCUPIED;
}

int get(char* key)
{
    int index = hash(key) % CAPACITY;

    int visited = 0;

    while(table[index].state != EMPTY)
    {
        if(table[index].state == OCCUPIED &&
           streq(table[index].key, key))
        {
            return table[index].value;
        }

        index++;

        if(index >= CAPACITY)
            index = 0;

        visited++;

        if(visited >= CAPACITY)
            break;
    }

    return -1;
}

int delete(char* key)
{
    int index = hash(key) % CAPACITY;

    int visited = 0;

    while(table[index].state != EMPTY)
    {
        if(table[index].state == OCCUPIED &&
           streq(table[index].key, key))
        {
            table[index].state = DELETED;
            return 1;
        }

        index++;

        if(index >= CAPACITY)
            index = 0;

        visited++;

        if(visited >= CAPACITY)
            break;
    }

    return 0;
}

void print_table()
{
    int i;

    for(i = 0; i < CAPACITY; i++)
    {
        clem_print(PRINT_STRING, "\n[");

        clem_print(PRINT_INT, &i);

        clem_print(PRINT_STRING, "] ");

        if(table[i].state == EMPTY)
        {
            clem_print(PRINT_STRING, "EMPTY");
        }
        else if(table[i].state == DELETED)
        {
            clem_print(PRINT_STRING, "DELETED");
        }
        else
        {
            clem_print(PRINT_STRING, table[i].key);
        }
    }

    clem_print(PRINT_STRING, "\n");
}

int main()
{
    put("apple", 10);
    put("banana", 20);
    put("orange", 30);

    clem_print(PRINT_STRING, "apple = ");
    clem_print(PRINT_INT, &(int){get("apple")});
    clem_print(PRINT_STRING, "\n");

    clem_print(PRINT_STRING, "banana = ");
    clem_print(PRINT_INT, &(int){get("banana")});
    clem_print(PRINT_STRING, "\n");

    print_table();

    clem_print(PRINT_STRING, "\nDeleting banana\n");

    delete("banana");

    print_table();

    clem_print(PRINT_STRING, "\nbanana = ");
    clem_print(PRINT_INT, &(int){get("banana")});
    clem_print(PRINT_STRING, "\n");

    put("banana", 999);

    clem_print(PRINT_STRING, "\nbanana reinserted = ");
    clem_print(PRINT_INT, &(int){get("banana")});
    clem_print(PRINT_STRING, "\n");

    print_table();

    return 0;
}

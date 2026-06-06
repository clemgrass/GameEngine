#ifndef PRINT_H
#define PRINT_H

typedef enum {
    PRINT_STRING,
    PRINT_INT
} PrintType;

void clem_print(PrintType type, void *value);

#endif
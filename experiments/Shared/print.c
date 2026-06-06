#include "print.h"

//System call x86 open, write, close
long syscall(long number, ...);
#define SYS_write 1


void clem_print(PrintType type, void *value) {
    switch (type) {
        case PRINT_STRING: {
            char *str = (char *)value;

            int len = 0;
            while (str[len] != '\0')
                len++;

            syscall(SYS_write, 1, str, len);
            break;
        }

        case PRINT_INT: {
            int n = *(int *)value;

            char buf[12];
            int i = 0;

            if (n == 0) {
                syscall(SYS_write, 1, "0", 1);
                break;
            }

            if (n < 0) {
                syscall(SYS_write, 1, "-", 1);
                n = -n;
            }

            while (n > 0) {
                buf[i++] = '0' + (n % 10);
                n /= 10;
            }

            while (i--) {
                syscall(SYS_write, 1, &buf[i], 1);
            }

            break;
        }
    
    }
}
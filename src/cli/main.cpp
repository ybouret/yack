#include <cstdio>
#include "yack/system/compiler.h"

int main(int , char* argv[]) {
    fprintf(stdout,"%s: %lu\n", argv[0], static_cast<unsigned long>(__cplusplus) );
    fprintf(stdout,"<%u.%u.%u>\n", YACK_MAJOR, YACK_MINOR, YACK_PATCH);
    return 0;
}

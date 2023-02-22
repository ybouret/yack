#include <cstdio>

int main(int , char* argv[]) {
    fprintf(stdout,"%s: %lu\n", argv[0], static_cast<unsigned long>(__cplusplus) );
    return 0;
}

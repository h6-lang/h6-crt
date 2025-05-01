#include "rt.h"
#include <assert.h>
#include <stdlib.h>

void syscallback(h6_rt_t* rt, uint32_t id, void* userptr)
{
    switch (id)
    {
    case 0: {
        int32_t byte = h6_heap_arr_pop_num(rt->stack);
        int32_t stream = h6_heap_arr_pop_num(rt->stack);
        assert(stream == 1);
        fputc(byte, stdout);
    } break;

    case 1: {
        int32_t stream = h6_heap_arr_pop_num(rt->stack);
        assert(stream == 1);
        int32_t byte = fgetc(stdin);
        h6_heap_arr_push_num(rt->stack, byte);
    } break;

    default:
        assert(0 && "unknwon syscall");
        break;
    }
}

int main(int argc, char** argv)
{
    FILE* fp = fopen(argv[1], "rb");
    assert(fp);
    fseek(fp, 0, SEEK_END);
    size_t len = ftell(fp);
    rewind(fp);
    char* bytecode = malloc(len);
    fread(bytecode, 1, len, fp);
    fclose(fp);

    h6_rt_t rt = h6_mk_rt(bytecode, syscallback, NULL);
    h6_run_bytecode(&rt, bytecode);

    if ( h6_heap_arr_len(rt.stack) > 0 )
    {
        printf("BOT\n");
        for (size_t i = 0; i < h6_heap_arr_len(rt.stack); i ++) {
            h6_op* v = h6_heap_arr_get_op(rt.stack, i);

            putc(' ', stdout);
            putc(' ', stdout);
            h6_op_print(stdout, v);
            putc('\n', stdout);
        }
        printf("TOP\n");
    }
}

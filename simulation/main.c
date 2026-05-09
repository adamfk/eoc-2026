#include "V2/Control.h"
#include "SimHal.h"
#include <stdio.h>

static char read_char_from_stdin_line(void);

int main(void)
{
    printf("\n");
    printf("USAGE:\n");
    printf("    'i' <enter>: send an INC event\n");
    printf("    'd' <enter>: send an DIM event\n");
    printf("    'o' <enter>: send an OFF event\n");
    printf("\n");

    V2_Control_setup();
    SimHal_print_outputs();

    while (1)
    {
        char c = read_char_from_stdin_line();

        switch (c)
        {
            case 'i': V2_Control_inc_press(); break;
            case 'd': V2_Control_dim_press(); break;
            case 'o': V2_Control_off_press(); break;
        }

        SimHal_print_outputs();
    }

    return 0;
}

// blocks while waiting for input
static char read_char_from_stdin_line(void)
{
    static char s_buf[100];
    char* c_ptr = fgets(s_buf, sizeof(s_buf), stdin);

    if (c_ptr == NULL)
    {
        return '\0';
    }

    return *c_ptr;
}

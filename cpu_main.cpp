#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "cpu_data.h"
#include "cpu_input.h"
#include "../secure_stack/stack.h"


int main()
{
    Stack_t stack = {};

    if (stackCtor(&stack, 8) != SUCCESS)
        return 1;

    Program program = {};

    input(&program, "output");     

    for (size_t index = 0; index < program.size; index++) {
        bool break_flag = false;
        int temp1 = 0, temp2 = 0;
        switch ((Instruction)program.data[index]) {
            case PUSH: 
                       stackPush(&stack, program.data[++index]);
                       break;
            case ADD:  
                       stackPop(&stack, &temp1);
                       stackPop(&stack, &temp2);
                       stackPush(&stack, temp2 + temp1);
                       break;
            case SUB: 
                       stackPop(&stack, &temp1);
                       stackPop(&stack, &temp2);
                       stackPush(&stack, temp2 - temp1);
                       break;
            case DIV:
                       stackPop(&stack, &temp1);
                       stackPop(&stack, &temp2);
                       stackPush(&stack, temp2 / temp1);
                       break;
            case MUL: 
                       stackPop(&stack, &temp1);
                       stackPop(&stack, &temp2);
                       stackPush(&stack, temp2 * temp1);
                       break;
            case SQRT:
                       stackPop(&stack, &temp1);
                       stackPush(&stack, (int)sqrt(temp1));
                       break;
            case OUT:
                       stackPop(&stack, &temp1);
                       printf("%d\n", temp1);
                       break;
            case HLT:
                       break_flag = true;
                       break;
            default:
                       printf("Error instruction\n");
                       break_flag = true;
                       break;
        }

        if (break_flag)
            break;
    }

    free(program.data);
    stackDtor(&stack);

    return 0;
}

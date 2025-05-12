#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char    key[100], tmp[4];
    int     index = 2, t_index = 1;
    char    transformed_input[100];

    printf("Please enter key: ");
    int scan_ret = scanf("%23s", &key);
    
    if (scan_ret != 1 || key[0] != '4' || key[1] != '2')
        return (printf("Nope.\n"));
    fflush(stdin);

    transformed_input[0] = '*';
    while (key[index])
    {
        if (!(strlen(key) >= index + 3))
            break;
        tmp[0] = key[index++];
        tmp[1] = key[index++];
        tmp[2] = key[index++];
        tmp[3] = '\0';
        transformed_input[t_index++] = (char) atoi(tmp);

    }
    transformed_input[t_index] = '\0';

    int ret = strcmp(transformed_input, "********");
    if (ret)
        return (printf("Nope.\n"));
    return (printf("Good job.\n"));
}

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const float pi = 3.141592;

void token(char* a)
{
    float x, y, rad, square, perimetr;
    char del[] = "( ,)";
    char* tok = strtok(a, del);
    x = atof(strtok(NULL, del));
    y = atof(strtok(NULL, del));
    rad = atof(strtok(NULL, del));
    square = pi * rad * rad;
    perimetr = 2 * pi * rad;
    printf("x = %.3f\ty = %.3f\trad = %.3f\n", x, y, rad);
    printf("square = %.3f\tperimetr = %.3f\n", square, perimetr);
}

void check_word(char* a, char* b, int* error, int* ind_open_bracket)
{
    for (int i = 0; i < 7; i++) {
        if (a[i] != b[i] && i < 6) {
            printf("Error at column %d: expected 'circle'\n", i);
            *error = 1;
            break;
        }
        *ind_open_bracket = i;
    }
}

void find_close_bracket(char* a, int* length, int* ind_close_bracket)
{
    for (int i = 0; i < *length && a[i] != '\n'; i++) {
        if (a[i] == ')') {
            *ind_close_bracket = i;
        } else {
            *ind_close_bracket = *length - 1;
        }
    }
}

// check first number
void check_first_num(
        char* a, int* ind_open_bracket, int* ind_first_num_elm, int* error)
{
    for (int i = *ind_open_bracket + 1; a[i] != ' '; i++) {
        if (*error == 0) {
            if (a[i] == ',') {
                *error = 1;
                printf("Error at column %d: expected '<space>' and "
                       "'<double>'\n",
                       i);
                break;
            }
            if (isdigit(a[i]) == 0 && a[i] != '.' && a[i] != '-'
                && a[i] != ' ') {
                *error = 1;
                printf("Error at column %d: expected '<double>'\n", i);
                break;
            }
            *ind_first_num_elm = i;
        } else {
            break;
        }
    }
}

// check second number
void check_second_num(
        char* a, int* ind_first_num_elm, int* ind_second_num_elm, int* error)
{
    for (int i = *ind_first_num_elm + 2; a[i] != ','; i++) {
        if (*error == 0) {
            if (a[i] == ')') {
                *error = 1;
                printf("Error at column %d: expected ',' and '<double>'\n", i);
                break;
            }
            if (isdigit(a[i]) == 0 && a[i] != '.' && a[i] != '-') {
                *error = 1;
                printf("Error at column %d: expected '<double>' or ',' "
                       "token\n",
                       i);
                break;
            }
            *ind_second_num_elm = i;
        } else {
            break;
        }
    }
}

// check last number
void check_third_num(
        char* a,
        int* ind_second_num_elm,
        int* ind_last_num_elm,
        int* error,
        int* ind_close_bracket)
{
    for (int i = *ind_second_num_elm + 3; i < *ind_close_bracket; i++) {
        if (*error == 0) {
            if ((isdigit(a[i]) == 0 && a[i] != '.') || a[i] == '0') {
                if (a[i] == ')' || a[i] == '(' || a[i] == ' ') {
                    break;
                }
                *error = 1;
                printf("Error at column %d: expected '<double>'\n", i);
                break;
            }
            *ind_last_num_elm = i;
        } else {
            break;
        }
    }
}

// check ')' symbol
void check_close_bracket(
        char* a,
        int* ind_last_num_elm,
        int* length,
        int* ind_close_bracket,
        int* error)
{
    for (int i = *ind_last_num_elm + 1; i < *length; i++) {
        if (*error == 0) {
            if (a[i] != ')') {
                *error = 1;
                printf("Error at column %d: expected ')'\n", i);
                break;
            } else {
                *ind_close_bracket = i;
                break;
            }
        } else {
            break;
        }
    }
}

// check unexpected tokens
void check_unexpected_token(
        char* a, int* ind_close_bracket, int* length, int* error)
{
    for (int i = *ind_close_bracket + 1; i < *length; i++) {
        if (*error == 0) {
            if (a[i] == '\n') {
                break;
            }

            if (a[i] != ' ') {
                *error = 1;
                printf("Error at column %d: unexpected token\n", i);
                break;
            }
        } else {
            break;
        }
    }
}

int main()
{
    FILE* file1;
    FILE* file;

    file1 = fopen("geometry.txt", "r");

    if (!file1) {
        printf("Error: cannot open file. Check name of file\n");
        return 0;
    }

    int ind_open_bracket = 0, ind_close_bracket = 0, ind_last_num_elm = 0,
        ind_first_num_elm = 0, ind_second_num_elm = 0;
    int length = 0, count = 0, element = 0, error = 0;

    while (1) {
        element = fgetc(file1);
        if (element == EOF) {
            if (feof(file1) != 0) {
                break;
            }
        }
        count++;
    }
    length = count;
    fclose(file1);

    char a[length], b[6] = "circle";
    file = fopen("geometry.txt", "r");
    while (fgets(a, length + 1, file)) {
        printf("%s", a);

        // check 'circle and finding index of '(' symbol
        check_word(a, b, &error, &ind_open_bracket);

        // printf("error = %d\tind_open_bracket = %d\n", error,
        // ind_open_bracket);

        // find index of ')' token
        find_close_bracket(a, &length, &ind_close_bracket);
        // printf("ind_close_bracket = %d\n", ind_close_bracket);

        // check first number
        check_first_num(a, &ind_open_bracket, &ind_first_num_elm, &error);
        // printf("ind_first_num_elm = %d\n", ind_first_num_elm);

        // check second number
        check_second_num(a, &ind_first_num_elm, &ind_second_num_elm, &error);
        // printf("ind_second_num_elm = %d\n", ind_second_num_elm);

        // check last number
        check_third_num(
                a,
                &ind_second_num_elm,
                &ind_last_num_elm,
                &error,
                &ind_close_bracket);
        // printf("ind_last_num_elm = %d\n", ind_last_num_elm);

        // check ')' symbol
        check_close_bracket(
                a, &ind_last_num_elm, &length, &ind_close_bracket, &error);
        // printf("ind_close_bracket = %d\n", ind_close_bracket);

        // check unexpected tokens
        check_unexpected_token(a, &ind_close_bracket, &length, &error);

        if (error == 0) {
            printf("No Errors!\n");
        }
        token(a);
        error = 0;
        printf("\n");
    }
    return 0;
}
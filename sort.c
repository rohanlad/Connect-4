#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct line_array
{
    char **arr_of_lines;
    int num_lines;
};

int char_compare(const void *x, const void *y)
{
    const char *ix = *(const char **)x;
    const char *iy = *(const char **)y;
    return (strcmp(ix, iy));
}

int char_compare_reverse(const void *x, const void *y)
{
    const char *ix = *(const char **)x;
    const char *iy = *(const char **)y;
    return (-1) * (strcmp(ix, iy));
}

int int_compare(const void *x, const void *y)
{
    const int *ix = (const int *)x;
    const int *iy = (const int *)y;
    return (*ix - *iy);
}

int int_compare_reverse(const void *x, const void *y)
{
    const int *ix = (const int *)x;
    const int *iy = (const int *)y;
    return (-1) * (*ix - *iy);
}

struct line_array read_in_lines(FILE *infile, struct line_array lary)
{
    int index = 0;
    char *string = NULL;
    int c = fgetc(infile);
    while (c != EOF)
    {
        index = 0;
        while (c != '\n' && c != EOF)
        {
            string = realloc(string, (index + 2));
            if (string == NULL)
            {
                printf("Error! Memory not allocated.");
                exit(1);
            }
            string[index] = c;
            index = index + 1;
            c = fgetc(infile);
        }
        if (string != NULL)
        {
            string[index] = '\0';
        }
        else
        {
            c = fgetc(infile);
            continue;
        }
        lary.arr_of_lines = realloc(lary.arr_of_lines, ((lary.num_lines + 1) * sizeof(*lary.arr_of_lines)));
        if (lary.arr_of_lines == NULL)
        {
            printf("Error! Memory not allocated.");
            exit(1);
        }
        lary.arr_of_lines[lary.num_lines] = string;
        lary.num_lines = lary.num_lines + 1;
        string = NULL;
        c = fgetc(infile);
    }
    return lary;
}

int main(int argc, char **argv)
{
    (void)argc;
    struct line_array lary;
    lary.arr_of_lines = NULL;
    lary.num_lines = 0;
    int int_array[lary.num_lines];
    int option_r = 0;
    int option_n = 0;
    int read_stdin = 1;
    char *file_to_write_to = NULL;
    while ((++argv)[0])
    {
        if (argv[0][0] == '-')
        {
            switch (argv[0][1])
            {
            default:
                printf("Error: The option -%c is invalid.\n", argv[0][1]);
                exit(1);
            case 'h':
                if (argv[0][2])
                {
                    printf("Error: The option %s is invalid.\n", argv[0]);
                    exit(1);
                }
                else
                {
                    printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", "\nThe sort utility sorts",
                           " text and binary files by lines. A line is a record separated from the",
                           " subsequent record by a newline (default) or NUL character (-z option).",
                           " A record can contain any printable or unprintable characters.  Comparisons",
                           " are based on one or more sort keys extracted from each line of input,",
                           " and are performed lexicographically, according to",
                           " the specified command-line options that can tune the actual sorting behavior.",
                           " By default, if keys are not given, sort uses entire lines for comparison.\n\n",
                           "The command line options are as follows:\n\n",
                           "-h Outputs this usage information\n",
                           "-o [output] Print the output to the [output] file instead of the standard output.\n",
                           "-r Sort in reverse order \n",
                           "-n Sort fields numerically by arithmetic value.  Fields are supposed to have optional",
                           " blanks in the beginning, an optional minus sign, zero or more digits",
                           " (including decimal point and possible thousand separators).\n\n",
                           "The following operand is available:\n\n",
                           "[file] The pathname of a file to be sorted, merged, or checked.  If no file",
                           " operands are specified, the standard input is used. Multiple files can",
                           " be provided in this way.\n\n",
                           "Exit Status: The sort utility shall exit with one of the following values:\n\n",
                           "0 Successfully sorted the input files\n",
                           "1 An error occurred.\n\n",
                           "NOTES: I think I have been able to implement the entirety of this part of the coursework.\n\n");
                    return 0;
                }
            case 'o':
                if (argv[0][2])
                {
                    printf("Error: The option %s is invalid.\n", argv[0]);
                    exit(1);
                }
                else
                {
                    file_to_write_to = (argv + 1)[0];
                    break;
                }
            case 'r':
                if (argv[0][2])
                {
                    printf("Error: The option %s is invalid.\n", argv[0]);
                    exit(1);
                }
                else
                {
                    option_r = 1;
                    break;
                }
            case 'n':
                if (argv[0][2])
                {
                    printf("Error: The option %s is invalid.\n", argv[0]);
                    exit(1);
                }
                else
                {
                    option_n = 1;
                    break;
                }
            }
        }
        else if ((argv - 1)[0][0] == '-' && (argv - 1)[0][1] == 'o')
        {
            continue;
        }
        else
        {
            FILE *infile;
            infile = fopen(argv[0], "r");
            if (infile == NULL)
            {
                printf("Error opening file %s. File may not exist.\n", argv[0]);
                exit(1);
            }
            lary = read_in_lines(infile, lary);
            fclose(infile);
            read_stdin = 0;
        }
    }

    if (read_stdin)
    {
        lary = read_in_lines(stdin, lary);
    }

    if (option_n)
    {
        for (int k = 0; k < lary.num_lines; k++)
        {
            int m = 0;
            sscanf(lary.arr_of_lines[k], "%d", &m);
            int_array[k] = m;
        }
        qsort(int_array, lary.num_lines, sizeof(int), (option_r ? int_compare_reverse : int_compare));
    }
    else
    {
        qsort(lary.arr_of_lines, lary.num_lines, sizeof(char *), (option_r ? char_compare_reverse : char_compare));
    }

    FILE *outfile;
    if (file_to_write_to != NULL)
    {
        outfile = fopen(file_to_write_to, "w");
    }
    else
    {
        outfile = stdout;
    }
    if (outfile == NULL)
    {
        printf("Error opening the file to write to!");
        exit(1);
    }
    if (option_n)
    {
        for (int j = 0; j < lary.num_lines; j++)
        {
            fprintf(outfile, "%d\n", int_array[j]);
        }
    }
    else
    {
        for (int j = 0; j < lary.num_lines; j++)
        {
            fprintf(outfile, "%s\n", lary.arr_of_lines[j]);
        }
    }
    fclose(outfile);
    for (int j = 0; j < lary.num_lines; j++)
    {
        free(lary.arr_of_lines[j]);
    }
    free(lary.arr_of_lines);
    return 0;
}
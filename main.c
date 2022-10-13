#include "read.h"
void errorPrint(int ret);

void errorPrint(int ret)
{
    printf(" This is ERROR %d, you can find out more about errors in makros.h\n\n", ret);
    switch (ret) {
        case SUCCESS:
            printf("Everything is OK\n");
            break;
        case ERROR_LEN:
            printf("There's a very long string.\n");
            printf("You can modify a makros called LENGTH in makros.h:17.\n");
            break;
        case ERROR_OPEN:
            printf("Cannot open file!\n");
            break;
        case ERROR_READ:
            printf("Cannot read!\n");
            break;
        case ERROR_MALLOC:
            printf("Not enough memory for malloc!\n");
            printf("Try reducing amount of files or argument sets\n");
            break;
        case ERROR_READFILE_ARGUMENT1:
            printf("Last argument set needs a closing parenthesis '}'\n");
            printf("(and perhaps a few arguments)\n");
            break;
        case ERROR_READTYPE:
            printf("There's a not-expected string in file.\n");
            printf("It might be a type or a comment.\n");
            break;
        case ERROR_READPARAMS:
            printf("There's a mistake in specifying ranges/constants/strings/files\n");
            break;
        case ERROR_ANALYZE:
            printf("There's a not-expected string in file\n");
            printf("Use '#' to comment it\n");
            break;
        case ERROR_TIMEOUT_SYNTAX:
            printf("After \"!timeout \" you should write time in these ways:\n");
            printf("123s for 123 seconds\n4m for 4 minutes\n5h for 5 hours\n7d for a week\n");
            printf("(number should be positive)\n");
            break;
        case ERROR_OUT_SYNTAX:
            printf("After \"!out \" you should write a non-empty string without spaces\n");
            break;
        case ERROR_NUMBER_OF_TESTS:
            printf("After specifying arguments for an argument set you should provide a number of tests\n");
            printf("You should use this syntax:\n");
            printf("{\n\t...\n} 1234\n");
            break;
        default:
            printf("This error is unknown. Have a good day!\n");
    }

    printf("\n");
}

int main(int argc, char ** argv)
{
    int ret;
    boolean cleverTestIndicator = FALSE;
    char * timeout = 0;
    char * out = 0;
    ArgumentSet * Set = 0;
    ArgumentSet * Set_;
    Argument * Arg;
    int i, j;

    if (argc==1)
    {
        printf("Instructions\n");
        return SUCCESS;
    }

    for (i=1; i<argc; i++)
    {
        printf("\t\tFILE: %s\n", argv[i]);
        ret = readFile(argv[i], &cleverTestIndicator, &timeout, &out, &Set);

        if (!Set)
        {
            printf("There are no argument sets in %s!\n", argv[i]);
            continue;
        }


        if (ret!=SUCCESS)
        {
            printf("ERROR occurred while reading from %s\n", argv[i]);
            errorPrint(ret);
            break;
        }

        if (cleverTestIndicator) printf(" ct: %d\n", cleverTestIndicator);
        if (timeout) printf(" timeout: %s\n", timeout);
        if (out) printf(" out: %s\n", out);
        printf("\n");

        for (Set_ = Set, j=1; Set_; Set_=Set_->next, j++)
        {
            printf("Set#%d:\n", j);
            printf("\tnumber of tests: %d\n", Set_->numberOfTests);
            for (Arg=Set_->argumentList; Arg; Arg=Arg->next) printf("\t%s\n", Arg->string);
            printf("\n");
        }

        ret = writeScript(cleverTestIndicator, timeout, out, Set);
        if (ret!=SUCCESS)
        {
            printf("ERROR occurred while writing script from %s\n", argv[i]);
            errorPrint(ret);
        }

        if (out)
        {
            free(out);
            out=0;
        }
        if (timeout)
        {
            free(timeout);
            timeout=0;
        }
    }

    if (out) free(out);
    if (timeout) free(timeout);
    if (Set) deleteArgumentSetList(Set);

    printf("Tests are generated!\n");
    return SUCCESS;
}

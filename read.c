//
// Created by powehi on 6/24/22.
//

#include "read.h"


int readFile(const char * filename, boolean * cleverTestIndicator, char ** timeout, char ** out, ArgumentSet ** Head)
{
    ArgumentSet * FirstSet = 0;
    ArgumentSet * CurrentSet = 0;
    ArgumentSet * NextSet;
    Argument * CurrentArg;
    Argument * NextArg;
    boolean cleverTestInd = FALSE;

    int i, j, n;
    int startOfBuf;

    int startOfOut;
    int startOfTime;
    int number = 0;
    int ret;
    char * ptr;
    char buf[LENGTH];
    char word[LENGTH];

    FILE * fp;

    fp=fopen(filename, "r");
    if (!fp) return ERROR_OPEN;

    while (fgets(buf, LENGTH, fp))
    {
        i=0;
        while (buf[i] && buf[i]==' ') i++;
        if (!buf[i] || buf[i]=='\n') continue;
        startOfBuf = i;

        if (buf[startOfBuf]=='!')
        {
            strcpy(word, "ct");
            for (i=startOfBuf+1, j=0; buf[i] && word[j]; i++, j++)
            {
                if (buf[i]!=word[j]) break;
            }
            if (!(buf[i] && word[j]))
            {
                cleverTestInd = TRUE;
                continue;
            }

            strcpy(word, "timeout ");
            for (i=startOfBuf+1, j=0; buf[i] && word[j]; i++, j++)
            {
                if (buf[i]!=word[j]) break;
            }
            if (!word[j] && buf[i])
            {
                while (buf[i] && buf[i]==' ') i++;
                if (!buf[i] || buf[i]=='\n')
                {
                    fclose(fp);
                    return ERROR_TIMEOUT_SYNTAX;
                }
                startOfTime = i;
                if (buf[startOfTime]=='0')  // требуется положительное число
                {
                    fclose(fp);
                    return ERROR_TIMEOUT_SYNTAX;
                }

                while (buf[i] && buf[i]<='9' && buf[i]>='0') i++;
                if (!buf[i] || buf[i]=='\n')
                {
                    ptr = buf + startOfTime;
                    buf[i]=0;

                    if (!(*timeout = (char *)malloc(sizeof(char)*(i-startOfTime+1))))
                    {
                        fclose(fp);
                        return ERROR_MALLOC;
                    }
                    strcpy(*timeout, ptr);
                    continue;
                }
                else if (buf[i]=='s' || buf[i]=='m' || buf[i]=='h' || buf[i]=='d')
                {
                    ptr = buf + startOfTime;

                    if (!(*timeout = (char *)malloc(sizeof(char)*(i-startOfTime+2))))
                    {
                        fclose(fp);
                        return ERROR_MALLOC;
                    }
                    buf[i+1]=0;
                    strcpy(*timeout, ptr);
                    continue;
                }
                else
                {
                    fclose(fp);
                    return ERROR_TIMEOUT_SYNTAX;
                }

            }


            strcpy(word, "out ");
            for (i=startOfBuf+1, j=0; buf[i] && word[j]; i++, j++)
            {
                if (buf[i]!=word[j]) break;
            }
            if (!word[j] && buf[i])
            {
                while (buf[i] && buf[i]==' ') i++;
                if (!buf[i])
                {
                    fclose(fp);
                    return ERROR_OUT_SYNTAX;
                }
                startOfOut = i;
                while (buf[i] && buf[i]!=' ' && buf[i]!='\n') i++;
                buf[i]=0;

                if (!(*out = (char *)malloc(sizeof(char)*(i-startOfOut+1))))
                {
                    fclose(fp);
                    return ERROR_MALLOC;
                }
                strcpy(*out, buf + startOfOut);
                continue;
            }

        }
        else if (buf[startOfBuf]=='#')
        {
            continue;
        }
        else if (buf[startOfBuf]=='{')
        {
            if (!CurrentSet)
            {
                FirstSet = (ArgumentSet *)malloc(sizeof(ArgumentSet));
                if (!FirstSet)
                {
                    fclose(fp);
                    return ERROR_MALLOC;
                }
                FirstSet->next = 0;
                FirstSet->argumentList = 0;
                NextSet = FirstSet;
            }
            else
            {
                NextSet = (ArgumentSet *)malloc(sizeof(ArgumentSet));
                if (!NextSet)
                {
                    deleteArgumentSetList(FirstSet);
                    fclose(fp);
                    return ERROR_MALLOC;
                }
                CurrentSet->next = NextSet;
                NextSet->argumentList = 0;
                NextSet->next = 0;
            }

            if (!fgets(buf, LENGTH, fp))
            {
                fclose(fp);
                deleteArgumentSetList(FirstSet);
                return ERROR_READFILE_ARGUMENT1;
            }
            ret = analyzeAndChangeString(buf);
            while (ret==COMMENT)
            {
                if (!fgets(buf, LENGTH, fp))
                {
                    fclose(fp);
                    deleteArgumentSetList(FirstSet);
                    return ERROR_READFILE_ARGUMENT1;
                }
            }

            CurrentArg = (Argument *)malloc(sizeof(Argument));
            if (!CurrentArg)
            {
                fclose(fp);
                deleteArgumentSetList(FirstSet);
                return ERROR_MALLOC;
            }

            if (ret!=SUCCESS)
            {
                if (buf[0]=='}')
                {
                    i=1;
                    while (buf[i] && buf[i] == ' ') i++;
                    if (!buf[i] || buf[i]=='#')
                    {
                        number = 0;
                        break;
                    }

                    j=i;
                    if (buf[j]<='0' || buf[j]>'9')
                    {
                        fclose(fp);
                        free(CurrentArg);
                        deleteArgumentSetList(FirstSet);
                        return ERROR_NUMBER_OF_TESTS;
                    }
                    j++;
                    while (buf[j]>='0' && buf[j]<='9') j++;

                    for (n=1, number=0, j-- ; j>=i; j--, n*=10)
                    {
                        number += n*((int)buf[j] - (int)'0');
                    }
                    NextSet->argumentList = CurrentArg;
                    CurrentArg->next = 0;
                    CurrentArg = 0;
                    NextSet->numberOfTests = number;
                    continue;
                }

                return ret;

                /*if (ret==ERROR_READPARAMS)
                {
                    printf("Cannot read parameters for an argument.\n");
                    fclose(fp);
                    free(CurrentArg);
                    deleteArgumentSetList(FirstSet);
                    return ERROR_READ;
                }
                if (ret==ERROR_READTYPE)
                {
                    printf("Cannot read type of an argument.\n");
                    fclose(fp);
                    free(CurrentArg);
                    deleteArgumentSetList(FirstSet);
                    return ERROR_READ;
                }*/

            }

            CurrentArg->string = (char *)malloc(sizeof(char)*(strlen(buf)+1));
            if (!(CurrentArg->string))
            {
                fclose(fp);
                free(CurrentArg);
                deleteArgumentSetList(FirstSet);
                return ERROR_MALLOC;
            }

            strcpy(CurrentArg->string, buf);
            NextSet->argumentList = CurrentArg;
            CurrentArg->next = 0;

            while (fgets(buf, LENGTH, fp))
            {
                ret = analyzeAndChangeString(buf);
                if (ret!=SUCCESS)
                {
                    if (ret==COMMENT) continue;
                    if (buf[0]=='}')
                    {
                        i=1;
                        while (buf[i] && buf[i] == ' ') i++;
                        if (!buf[i] || buf[i]=='#')
                        {
                            number = 0;
                            break;
                        }
                        else
                        {
                            j=i;
                            if (buf[j]<='0' || buf[j]>'9')
                            {
                                fclose(fp);
                                deleteArgumentSetList(FirstSet);
                                return ERROR_NUMBER_OF_TESTS;
                            }
                            j++;
                            while (buf[j]>='0' && buf[j]<='9') j++;

                            for (n=1, number=0, j-- ; j>=i; j--, n*=10)
                            {
                                number += n*((int)buf[j] - (int)'0');
                            }
                            break;
                        }
                    }
                    else
                    {
                        fclose(fp);
                        deleteArgumentSetList(FirstSet);
                        return ret;
                    }
                }

                NextArg = (Argument *)malloc(sizeof(Argument));
                if (!NextArg)
                {
                    fclose(fp);
                    deleteArgumentSetList(FirstSet);
                    return ERROR_MALLOC;
                }

                NextArg->string = (char *)malloc(sizeof(char)*(strlen(buf)+1));
                if (!(NextArg->string))
                {
                    fclose(fp);
                    free(NextArg);
                    deleteArgumentSetList(*Head);
                    return ERROR_MALLOC;
                }
                strcpy(NextArg->string, buf);
                CurrentArg->next = NextArg;
                NextArg->next = 0;

                CurrentArg = NextArg;
            }


            NextSet->numberOfTests = number;
            CurrentSet = NextSet;
        }
        else
        {
            fclose(fp);
            return ERROR_READ;
        }

    }

    *cleverTestIndicator = cleverTestInd;
    *Head = FirstSet;


    return SUCCESS;

}

int analyzeAndChangeString(char * buf)
{
    int i, j, startOfTheWord, endOfTheWord;
    int type = NONE;

    while (buf[i] && buf[i]==' ') i++;
    if (buf[i] && (buf[i]=='\n' || buf[i]=='#')) return COMMENT;

    for (; buf[i]; i++)
    {
        if (buf[i]=='\n')
        {
            buf[i] = 0;
            break;
        }
    }

    i=0;
    while (buf[i] && buf[i]==' ') i++;
    if (!buf[i]) return ERROR_LEN;
    startOfTheWord = i;

    if (buf[startOfTheWord]=='#') return COMMENT;


    if (buf[i]=='i')
    {
        if (buf[++i] && buf[i] == 'n' && buf[++i] && buf[i] == 't') {
            endOfTheWord = ++i;
            type = INT;
        }
        else return ERROR_READTYPE;
    }
    else if (buf[i]=='d')
    {
        if (buf[++i] && buf[i] == 'o' && buf[++i] && buf[i] == 'u' && buf[++i] && buf[i] == 'b' && buf[++i] && buf[i] == 'l' && buf[++i] && buf[i] == 'e') {
            endOfTheWord = ++i;
            type = DOUBLE;
        }
        else return ERROR_READTYPE;
    }
    else if (buf[i]=='c')
    {
        if (buf[++i] && buf[i] == 'h' && buf[++i] && buf[i] == 'a' && buf[++i] && buf[i] == 'r') {
            endOfTheWord = ++i;
            type = CHAR;
        }
        else return ERROR_READTYPE;
    }
    else if (buf[i]=='s')
    {
        if (buf[++i] && buf[i] == 't' && buf[++i] && buf[i] == 'r' && buf[++i] && buf[i] == 'i' && buf[++i] && buf[i] == 'n' && buf[++i] && buf[i] == 'g') {
            endOfTheWord = ++i;
            type = STRING;
        }
        else return ERROR_READTYPE;
    }
    else if (buf[i]=='f')
    {
        if (buf[++i] && buf[i] == 'i' && buf[++i] && buf[i] == 'l' && buf[++i] && buf[i] == 'e') {
            endOfTheWord = ++i;
            type = FILENAME;
        }
        else return ERROR_READTYPE;
    }
    else if (buf[i]==0 || buf[i]=='#') return COMMENT;
    else return ERROR_READTYPE;

    j=0;
    buf[j++] = type; // type + (int)'0' ??
    buf[j++] = ' ';

    if (buf[endOfTheWord]==0 || buf[startOfTheWord]=='#')
    {
        for ( ; buf[j]; j++) buf[j]=0;
        return SUCCESS;
    }

    while (buf[i] && buf[i]==' ') i++;

    startOfTheWord = i;

    if (buf[startOfTheWord]==0 || buf[startOfTheWord]=='#')
    {
        for ( ; buf[j]; j++) buf[j]=0;
        return SUCCESS;
    }
    if (type==INT)
    {
        if (buf[startOfTheWord]=='[')
        {
            i++;
            if (!buf[i]) return ERROR_READPARAMS;

            if (buf[i]=='-')
            {
                buf[j++] = buf[i++];
                if (!buf[i]) return ERROR_READPARAMS;
                if (buf[i]<='0' || buf[i]>'9') return ERROR_READPARAMS;
                buf[j++] = buf[i++];
                while (buf[i] && buf[i]>='0' && buf[i]<='9')
                {
                    buf[j++] = buf[i++];
                }
            }
            else if (buf[i]=='0')
            {
                buf[j++] = buf[i++];
                if (!buf[i]) return ERROR_READPARAMS;
                /*if (buf[i]!=' ' || buf[i]!=';' || buf[i]!=',') return ERROR_READPARAMS;*/
            }
            else if (buf[i]>'0' && buf[i]<='9')
            {
                buf[j++] = buf[i++];
                if (!buf[i]) return ERROR_READPARAMS;
                while (buf[i] && buf[i]>='0' && buf[i]<='9')
                {
                    buf[j++] = buf[i++];
                }

            }
            else return ERROR_READPARAMS;

            while (buf[i] && buf[i]==' ') i++;
            if (!buf[i]) return ERROR_READPARAMS;

            if (!(buf[i]==';' || buf[i]==',')) return ERROR_READPARAMS;

            i++;
            buf[j++]=' ';

            while (buf[i] && buf[i]==' ') i++;
            if (!buf[i]) return ERROR_READPARAMS;

            if (buf[i]=='-')
            {
                buf[j++] = buf[i++];
                if (!buf[i]) return ERROR_READPARAMS;
                if (buf[i]<='0' || buf[i]>'9') return ERROR_READPARAMS;
                buf[j++] = buf[i++];
                while (buf[i] && buf[i]>='0' && buf[i]<='9')
                {
                    buf[j++] = buf[i++];
                }
            }
            else if (buf[i]=='0')
            {
                buf[j++] = buf[i++];
                if (!buf[i]) return ERROR_READPARAMS;
            }
            else if (buf[i]>'0' && buf[i]<='9')
            {
                buf[j++] = buf[i++];
                if (!buf[i]) return ERROR_READPARAMS;
                while (buf[i] && buf[i]>='0' && buf[i]<='9')
                {
                    buf[j++] = buf[i++];
                }
            }
            else return ERROR_READPARAMS;

            while (buf[i] && buf[i]==' ') i++;
            if (!buf[i]) return ERROR_READPARAMS;
            if (buf[i]!=']') return ERROR_READPARAMS;

            for (; buf[j]; j++) buf[j] = 0;

            return SUCCESS;

        }
        else if (buf[startOfTheWord]=='c')
        {
            if (buf[++i] && buf[i] == 'o' && buf[++i] && buf[i] == 'n' && buf[++i] && buf[i] == 's' && buf[++i] && buf[i] == 't')
            {
                endOfTheWord = ++i;
                while (buf[i] && buf[i]==' ') i++;

                if (!buf[i]) return ERROR_READPARAMS;

                startOfTheWord = i;

                if (buf[i]=='-')
                {
                    buf[j++] = buf[i++];
                    if (!buf[i]) return ERROR_READPARAMS;
                    if (buf[i]<='0' || buf[i]>'9') return ERROR_READPARAMS;
                    buf[j++] = buf[i++];
                    while (buf[i] && buf[i]>='0' && buf[i]<='9')
                    {
                        buf[j++] = buf[i++];
                    }
                }
                else if (buf[i]=='0')
                {
                    buf[j++] = buf[i++];
                    //if (!buf[i]) return ERROR_READPARAMS;
                }
                else if (buf[i]>'0' && buf[i]<='9')
                {
                    buf[j++] = buf[i++];
                    if (!buf[i])
                    {
                        for (; buf[j]; j++) buf[j] = 0;
                        return SUCCESS;
                    }
                    while (buf[i] && buf[i]>='0' && buf[i]<='9')
                    {
                        buf[j++] = buf[i++];
                    }
                }
                else return ERROR_READPARAMS;

                for (; buf[j]; j++) buf[j] = 0;

                return SUCCESS;

                /*endOfTheWord = i;

                for (i=endOfTheWord-1, n=1, number=0; i>=startOfTheWord; i--, n*=10)
                {
                    number += n*(buf[i]-(int)'0');
                }*/
            }
            else return ERROR_READPARAMS;
        }
        else return ERROR_READPARAMS;
    }
    else if (type==CHAR)
    {
        if (buf[startOfTheWord]=='[')
        {
            i++;
            while (buf[i] && buf[i]==' ') i++;
            if (!buf[i]) return ERROR_READPARAMS;

            if (buf[i]=='\'')
            {
                i++;
                if (!buf[i]) return ERROR_READPARAMS;
                buf[j++] = buf[i++];
                if (!buf[i] || buf[i++]!='\'') return ERROR_READPARAMS;
            }
            else buf[j++] = buf[i++];

            while (buf[i] && buf[i]==' ') i++;
            if (!buf[i]) return ERROR_READPARAMS;

            if (!(buf[i]==';' || buf[i]==',')) return ERROR_READPARAMS;

            i++;
            buf[j++]=' ';
            while (buf[i] && buf[i]==' ') i++;
            if (!buf[i]) return ERROR_READPARAMS;

            if (buf[i]=='\'')
            {
                i++;
                if (!buf[i]) return ERROR_READPARAMS;
                buf[j++] = buf[i++];
                if (!buf[i] || buf[i++]!='\'') return ERROR_READPARAMS;
            }
            else buf[j++] = buf[i];

            for (; buf[j]; j++) buf[j] = 0;     //нет проверки на закрывающую скобку

            return SUCCESS;

        }
        else if (buf[startOfTheWord]=='c')
        {
            if (buf[++i] && buf[i] == 'o' && buf[++i] && buf[i] == 'n' && buf[++i] && buf[i] == 's' && buf[++i] && buf[i] == 't')
            {
                endOfTheWord = ++i;
                while (buf[i] && buf[i]==' ') i++;

                if (!buf[i]) return ERROR_READPARAMS;

                startOfTheWord = i;

                if (buf[i]=='\'')
                {
                    i++;
                    if (!buf[i]) return ERROR_READPARAMS;
                    buf[j++] = buf[i++];
                    if (!buf[i] || buf[i++]!='\'') return ERROR_READPARAMS;
                }
                else buf[j++] = buf[i];

                for (; buf[j]; j++) buf[j] = 0;

                return SUCCESS;

            }
            else return ERROR_READPARAMS;
        }
        else return ERROR_READPARAMS;
    }
    else if (type==DOUBLE)
    {
        if (buf[startOfTheWord]=='c')
        {
            if (buf[++i] && buf[i] == 'o' && buf[++i] && buf[i] == 'n' && buf[++i] && buf[i] == 's' && buf[++i] && buf[i] == 't')
            {
                endOfTheWord = ++i;
                while (buf[i] && buf[i]==' ') i++;

                if (!buf[i]) return ERROR_READPARAMS;

                while (buf[i] && buf[i]!=' ' && buf[i]!='#')
                {
                    buf[j++] = buf[i++];
                }
                for ( ; buf[j]; j++) buf[j] = 0;

                return SUCCESS;
            }
            else return ERROR_READPARAMS;
        }
    }
    else
    {
        for ( ; buf[i]; i++)
        {
            if (buf[i]==' ' && buf[i-1]==' ') continue;
            if (buf[i]=='#') break;
            buf[j++]=buf[i];
        }
        for ( ; buf[j]; j++) buf[j] = 0;

        return SUCCESS;
    }


    return ERROR_ANALYZE;
}
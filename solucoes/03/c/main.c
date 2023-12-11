#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_LINE_SIZE 1024

regex_t symbols;

void compile_regex()
{
    const char *pattern = "[^0-9.\t\n\r]";
    int error = regcomp(&symbols, pattern, REG_EXTENDED);
    if (error)
    {
        char msgbuf[100];
        regerror(error, &symbols, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex error compiling '%s': %s\n", pattern, msgbuf);
        exit(1);
    }
}

void partOne(char **lines, int lineAmount)
{
    compile_regex();
    int sum = 0;
    for (int i = 0; i < lineAmount; i++)
    {
        for (int j = 0; j < strlen(lines[i]); j++)
        {
            if (lines[i][j] <= '9' && lines[i][j] >= '0')
            {
                int numStart = j;
                j++;
                while (lines[i][j] <= '9' && lines[i][j] >= '0')
                {
                    j++;
                }
                int numEnd = j - 1;
                int breakFlag = 0;
                for (int k = i - 1; k <= i + 1; k++)
                {
                    if (k < 0 || k >= lineAmount || lines[k] == NULL)
                        continue;
                    for (int m = numStart - 1; m <= numEnd + 1; m++)
                    {
                        if (m < 0 || m >= strlen(lines[k]))
                            continue;
                        if (k != i || m < numStart || m > numEnd)
                        {
                            char symbol[2];
                            symbol[0] = lines[k][m];
                            symbol[1] = '\0';
                            if (regexec(&symbols, symbol, 0, NULL, 0) == 0)
                            {
                                int numSize = numEnd - numStart + 1;
                                char *partNumber = malloc((numSize + 1) * sizeof(char));
                                strncpy(partNumber, &lines[i][numStart], numSize);
                                partNumber[numSize] = '\0';
                                printf("Part Number: %s\n", partNumber);
                                int number = atoi(partNumber);
                                printf("Number: %d\n", number);
                                sum += number;
                                free(partNumber);
                                breakFlag = 1;
                                break;
                            }
                        }
                    }
                    if (breakFlag)
                        break;
                }
            }
        }
    }

    regfree(&symbols);

    printf("Part one: %d", sum);
}

void partTwo(char **lines, int lineAmount)
{
    int lineLength = strlen(lines[0]);
    int sum = 0;
    for (int i = 0; i < lineAmount; i++)
    {
        for (int j = 0; j < lineLength; j++)
        {
            if (lines[i][j] == '*')
            {
                int numCount = 0;
                int column1 = -1;
                int column2 = -1;
                int line1 = -1;
                int line2 = -1;
                int breakFlag = 0;
                for (int k = i - 1; k <= i + 1; k++)
                {
                    if (k < 0 || k >= lineAmount || lines[k] == NULL)
                        continue;
                    for (int m = j - 1; m <= j + 1; m++)
                    {
                        if (m < 0 || m >= lineLength)
                            continue;
                        if (lines[k][m] >= '0' && lines[k][m] <= '9')
                        {
                            if (numCount == 0)
                            {
                                line1 = k;
                                column1 = m;
                                do
                                {
                                    m++;
                                } while (m < lineLength && lines[k][m] >= '0' && lines[k][m] <= '9');
                            }
                            else if (numCount == 1)
                            {
                                line2 = k;
                                column2 = m;
                            }
                            numCount++;
                            if (numCount == 2)
                            {
                                int numStart = column1;
                                while (numStart > 0 && lines[line1][numStart - 1] >= '0' && lines[line1][numStart - 1] <= '9')
                                {
                                    numStart--;
                                }
                                int numEnd = column1;
                                while (numEnd < lineLength && lines[line1][numEnd + 1] >= '0' && lines[line1][numEnd + 1] <= '9')
                                {
                                    numEnd++;
                                }
                                int numSize = numEnd - numStart + 1;
                                char *firstNumS = malloc((numSize + 1) * sizeof(char));
                                for (int numIndex = numStart; numIndex <= numEnd; numIndex++)
                                {
                                    firstNumS[numIndex - numStart] = lines[line1][numIndex];
                                }
                                firstNumS[numSize] = '\0';
                                int firstNum = atoi(firstNumS);
                                free(firstNumS);
                                numStart = column2;
                                while (numStart > 0 && lines[line2][numStart - 1] >= '0' && lines[line2][numStart - 1] <= '9')
                                {
                                    numStart--;
                                }
                                numEnd = column2;
                                while (numEnd < lineLength && lines[line2][numEnd + 1] >= '0' && lines[line2][numEnd + 1] <= '9')
                                {
                                    numEnd++;
                                }
                                numSize = numEnd - numStart + 1;
                                char *secondNumS = malloc((numSize + 1) * sizeof(char));
                                for (int numIndex = numStart; numIndex <= numEnd; numIndex++)
                                {
                                    secondNumS[numIndex - numStart] = lines[line2][numIndex];
                                }
                                secondNumS[numSize] = '\0';
                                int secondNum = atoi(secondNumS);
                                free(secondNumS);
                                int result = firstNum * secondNum;
                                sum += result;
                                breakFlag = 1;
                                break;
                            }
                        }
                    }
                    if (breakFlag)
                        break;
                }
            }
        }
    }
    printf("Part two: %d", sum);
}

int main()
{
    char **lines = malloc(MAX_LINE_SIZE * sizeof(char *));
    char line[MAX_LINE_SIZE];
    int i = 0;

    FILE *file = fopen("input.txt", "r");
    if (file == NULL)
    {
        printf("Failed to open file\n");
        return 1;
    }

    while (fgets(line, MAX_LINE_SIZE, file))
    {
        lines[i] = malloc(strlen(line) + 1);
        strcpy(lines[i], line);
        i++;
    }

    fclose(file);

    partOne(lines, i);
    partTwo(lines, i);

    for (int j = 0; j < i; j++)
    {
        free(lines[j]);
    }
    free(lines);

    return 0;
}
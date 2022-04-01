#include "userInput.h"

int getInt(const char* question)
{
    printf(question);
    int answer;
    int count;
    count = scanf("%d", &answer);
    bool valid = false;

    if (count != 1)
    {
        printf("Invalid data type.\nPlease enter an int...\n");
        fflush(stdin);
    }
    else
    {
        printf("Your int is: %d\n", answer);
        valid = true;
    }
    if (!valid)
    {
        answer = getInt(question);
    }
    return answer;
}

float getFloat(const char* question)
{
    printf(question);
    float answer;
    int count;
    count = scanf("%f", &answer);
    bool valid = false;

    if (count != 1)
    {
        printf("Invalid data type.\nPlease enter a float...\n");
        fflush(stdin);
    }
    else
    {
        printf("Your float is: %f\n", answer);
        valid = true;
    }
    if (!valid)
    {
        answer = getFloat(question);
    }
    return answer;
}

char getChar(const char* question)
{
    char c;
    printf(question);
    scanf(" %c", &c);
    fflush(stdin);
    return c;
}

char getEnter(const char* question)
{
    char enter = 0;
    printf(question);
    while (enter != '\r' && enter != '\n')
    {
        enter = getchar();
    }
    fflush(stdin);
    return enter;
}

// Will return a string the size of the int passed in
const char* getString(int size, const char* question)
{
    size++;
    static char str[2];
    printf(question);
    //    scanf("%s", str);
    fgets(str, size, stdin);
    return str;
}


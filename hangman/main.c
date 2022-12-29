/* TIC TAC TOE GAME DEVELOPED BY EHSAN SHAFIEE
GITHUB REPO : https://github.com/esh83/HangMan_C */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAXNUM 100
#define LIVES 10

void menu();
void exit_game();
void print_guide();
void start_game();
void restart(int, int);
void fill_random(int, int, int *);
int number_of_words = 1;
char words[MAXNUM][50];
int main()
{
    srand(time(NULL));
    FILE *words_file = fopen("words.txt", "r");
    char current_char;
    while ((current_char = fgetc(words_file)) != EOF)
    {
        if (current_char == '\n')
            number_of_words++;
    }
    rewind(words_file);

    char current_word[50];
    int counter = 0;
    while (fgets(current_word, 49, words_file))
    {
        strcpy(words[counter], current_word);
        counter++;
    }
    menu();
    return 0;
}

void menu()
{
    printf("Menu :\ntype 1 to start the game \ntype 2 for the guide \ntype 3 to exit\n");
    void (*startup_funcs[3])() = {start_game, print_guide, exit_game};
    int itm;
    scanf("%d", &itm);
    fflush(stdin);
    while (itm < 1 || itm > 3)
    {
        printf("invalid input!! enter again : \n");
        scanf("%d", &itm);
        fflush(stdin);
    }
    startup_funcs[itm - 1]();
}
void exit_game()
{
    exit(0);
}
void print_guide()
{
    printf("\nhow to play hangman game : \nsoon...\n");
    printf("press enter to back to menu\n");
    char x;
    while (getchar() != '\n')
        ;
    scanf("%c", &x);
    menu();
}
void restart(int win, int i)
{
    if (win)
        printf("\nYOU WON THE GAME !! :)\n");
    else
        printf("\nSORRY YOU LOSE !! :( THE WORD WAS \"%s\"\n", words[i]);

    printf("press enter to continue ...\n");
    char x;
    scanf("%c", &x);
    menu();
}
void fill_random(int x, int len, int *gusses_status)
{
    int show_count = 0;
    while (show_count < x)
    {
        int x = rand() % len;
        if (gusses_status[x] == 1)
            continue;
        gusses_status[x] = 1;
        show_count++;
    }
}
void start_game()
{

    int random_index = rand() % number_of_words;
    int len = strlen(words[random_index]);
    if (words[random_index][len - 1] == '\n')
    {
        words[random_index][len - 1] = '\0';
        len--;
    }

    int lives = LIVES;
    char entered_str[5];
    enum
    {
        QUIT,
        LOSE,
        WIN
    };
    int exit_status = 0;
    int gusses_status[50] = {0};
    int show_indexs[5];
    int show_count = 0;
    if (len <= 3)
    {
        fill_random(1, len, gusses_status);
    }
    else if (len <= 6)
    {
        fill_random(2, len, gusses_status);
    }
    else if (len <= 10)
    {
        fill_random(3, len, gusses_status);
    }
    else
    {
        fill_random(4, len, gusses_status);
    }
    printf("\nwelcome to the game\ntype -1 to quit the game\n\n");
    for (int i = 0; i < len; i++)
    {
        if (gusses_status[i])
            printf("%c ", words[random_index][i]);
        else
        {
            printf("_ ");
        }
    }
    printf("\n\nenter your guess letter:\n");
    while (fgets(entered_str, 4, stdin))
    {
        if (entered_str[0] == '\n')
            continue;
        if (strncmp(entered_str, "-1", 2) == 0)
        {
            exit_status = QUIT;
            break;
        }
        if (strlen(entered_str) != 2)
        {
            printf("invalid charecter, enter again :\n");
            fflush(stdin);
            continue;
        }
        int is_wrong = 1;
        for (int i = 0; i < len; i++)
        {
            if (words[random_index][i] == entered_str[0])
            {
                gusses_status[i] = 1;
                is_wrong = 0;
            }
        }
        printf("-------------------------------\n");
        if (is_wrong)
        {
            lives--;
            if (lives <= 0)
            {
                exit_status = LOSE;
                break;
            }

            printf("wrong guess :(\n\n");
        }
        else
        {
            printf("correct guess :)\n\n");
        }
        int is_all_correct = 1;
        for (int i = 0; i < len; i++)
        {
            if (gusses_status[i])
                printf("%c ", words[random_index][i]);
            else
            {
                printf("_ ");
                is_all_correct = 0;
            }
        }
        if (is_all_correct)
        {
            exit_status = WIN;
            break;
        }
        printf("\n\nremaining lives : %d\nenter a guess letter:\n", lives);
    }
    switch (exit_status)
    {
    case QUIT:
        menu();
        break;
    case LOSE:
        restart(0, random_index);
        break;
    case WIN:
        restart(1, random_index);
        break;
    }
}

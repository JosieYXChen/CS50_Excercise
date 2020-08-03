// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table;
const unsigned int N = 27 * 27 * 27 + 27 * 27 + 27;

// Hash table
node *table[N];

int index_calculator(const char *word)
{
    //match the first three letters of each word to a number;
    int i = 0;
    int asc[LENGTH + 1];
    while (word[i])
    {
        if (isalpha(word[i]))
        {
            asc[i] = tolower(word[i]) - 97;
        }
        else
        {
            asc[i] = 26;
        }
        i++;
    }

    int index = 0;
    if (i == 1)
    {
        index = asc[0];
    }
    else if (i == 2)
    {
        index = asc[0] * 27 + asc[1] + 27;
    }
    else if (i > 2)
    {
        index = asc[0] * 27 * 27 + asc[1] * 27 + asc[2] + 27 * 27 + 27;
    }
    return index;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // make a copy of word in lower case
    char lower_cpy[LENGTH + 1];
    memset(lower_cpy, 0, sizeof lower_cpy);
    for (int i = 0; word[i]; i++)
    {
        lower_cpy[i] = tolower(word[i]);
    }

    int index = index_calculator(lower_cpy);
    if (table[index] != NULL)
    {
        for (node *tmp = table[index]; tmp != NULL; tmp = tmp->next)
        {
            if (strcmp(tmp->word, lower_cpy) == 0)
            {
                return true;
            }
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int index = index_calculator(word);

    //turn the new word into a node structure
    node *new_word = malloc(sizeof(node));
    if (new_word == NULL)
    {
        return 1;
    }
    strcpy(new_word->word, word);

    // make a linked list
    if (table[index] == NULL)
    {
        table[index] = new_word;
    }
    else
    {
        new_word->next = table[index];
        table[index] = new_word;
    }
    return 0;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //open the dictionary
    FILE *dic_copy = fopen(dictionary, "r");
    if (dic_copy != NULL)
    {
        char word[LENGTH + 2];
        while (fgets(word, sizeof word, dic_copy))
        {
            int index = 0;
            char parsed[LENGTH + 2];
            memset(parsed, 0, sizeof parsed);
            while (isalpha(word[index]) || word[index] == 39)
            {
                parsed[index] = word[index];
                index++;
            }
            if (index <= 45)
            {
                hash(parsed);
            }
        }
        fclose(dic_copy);
        return true;
    }
    return false;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    int count = 0;
    for (int i = 0; i < N; i++)
    {
       if (table[i] != NULL)
       {
            for (node *tmp = table[i]; tmp != NULL; tmp = tmp->next)
            {
                count++;
            }
        }
    }
    return count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            while (table[i] != NULL)
            {
                node *tmp = table[i]->next;
                free(table[i]);
                table[i] = tmp;
            }
        }
    }

    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            return false;
        }
    }
    return true;
}

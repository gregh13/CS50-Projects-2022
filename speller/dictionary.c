// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>

#include "dictionary.h"




// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

void add_node(node *root, node *new_node);
int check_word(node *root, const char *word);

int WORDS_IN_DICTIONARY = 0;

// TODO: Choose number of buckets in hash table
const unsigned int N = 19682;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int dict_index = hash(word);
    //int match = check_word(table[dict_index], word);
    //printf("CHECK: %i: %s\n", dict_index, table[dict_index]->word);

    if (table[dict_index] == NULL)
    {
        //printf("NULL VALLL");
        return false;
    }
    else if (strcasecmp(table[dict_index]->word, word) == 0)
    {
        //printf("MATCH!");
        return true;
    }
    else
    {
        //printf("1st: Saved_word: %s, check word: %s\n", table[dict_index]->word, word);
        node *temp = table[dict_index];
        while (temp != NULL)
        {
            //printf("2nd: Saved_word: %s, check word: %s\n", temp->word, word);
            if (strcasecmp(temp->word, word) == 0)
            {
                return true;
            }
            temp = temp->next;
        }
        //printf("ELSE");
        return false;
    }
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int num = 0;
    int w = strlen(word);
    // Word is one letter, goes to index 0 through 25
    if (w == 1)
    {
        num += tolower(word[0]) - 'a';
    }
    // Word is two letters, goes from index 26 through 727 (since 2nd letter could be apostrophe)
    else if (w == 2)
    {
        num += 26;
        num += (tolower(word[0]) - 'a') * 27;
        if (isalpha(word[1]))
        {
            num += (tolower(word[1]) - 'a');
        }
        else
        {
            // Only non-alphabetic character is an apostrophe (single quote)
            // 39 (single quote ascii) - 13 = 26, the index after 'z')
            num += (word[1] - 13);
        }
    }
    // Word is three letters or more, goes from index 728 to 19681
    else if (w > 2)
    {
        num += 728;
        num += (tolower(word[0]) - 'a') * 729;
        if (isalpha(word[1]))
        {
            num += (tolower(word[1]) - 'a') * 27;
        }
        else
        {
            // Only non-alphabetic character possible is an apostrophe (single quote)
            // 39 (single quote ascii) - 13 = 26, the index after 'z')
            num += (word[1] - 13) * 27;
        }
        if (isalpha(word[2]))
        {
            num += (tolower(word[2]) - 'a');
        }
        else
        {
            // Only non-alphabetic character is an apostrophe (single quote)
            // 39 (single quote ascii) - 13 = 26, the index after 'z')
            num += (word[2] - 13);
        }
    }
    return num;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    node *n = malloc(sizeof(node));
    if (n == NULL)
    {
        printf("Not enough memory for next node.\n");
        return false;
    }
    char c;
    int index = 0, num_words = 0;
    char word[LENGTH + 1];
    unsigned int hash_val;

    FILE *dict_file = fopen(dictionary, "r");
    if (dict_file == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return 1;
    }

    while (fread(&c, sizeof(char), 1, dict_file))
    {
        if (c != '\n')
        {
            word[index] = c;
            index++;
        }
        // Reached end of word
        else
        {
            word[index] = '\0';
            strcpy(n->word, word);
            n->next = NULL;
            hash_val = hash(word);
            if (table[hash_val] == NULL)
            {
                table[hash_val] = n;
            }
            else
            {
                node *tmp = table[hash_val];
                while (tmp->next != NULL)
                {
                    tmp = tmp->next;
                }
                tmp->next = n;
            }
            //add_node(table[hash_val], n);
            num_words++;
            index = 0;
            n = malloc(sizeof(node));
        }
    }
    WORDS_IN_DICTIONARY = num_words;
    free(n);
    fclose(dict_file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    int dic_size = WORDS_IN_DICTIONARY;
    return dic_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < 19682; i++)
    {
        while (table[i] != NULL)
        {
            node *temp = table[i]->next;
            free(table[i]);
            table[i] = temp;
        }
    }
    return true;
}

// Used in load function, adds new node/word from dictionary to the table of words.
void add_node(node *root, node *new_node)
{
    if (root == NULL)
    {
        root = new_node;
        return;
    }
    add_node(root->next, new_node);
}

int check_word(node *root, const char *word)
{
    if (root == NULL)
    {
        printf("NULL ROOT");
        return 0;
    }
    else if (strcasecmp(root->word, word) == 0)
    {
        printf("It's a match");
        return 1;
    }
    else
    {
        printf("GOT HERE");
        check_word(root->next, word);

        // Not sure about this return
        return 2;
    }
}
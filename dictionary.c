/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include "dictionary.h"
#include <string.h>

// global variables
int no_of_words = 0;


    // create a node data structure for a trie
    typedef struct node
    {
        bool real_word;
        struct node* children[27];
    }
    node;
    
    // root node
    node* root;

// functions that will be used within my unload function    
bool HasChild(node* curNode);
void LetsUnload(node* curNode);

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // node pointer
    node* chkptr = root;
    
    // local variable
    int len = strlen(word);
    
    // handle min and max length
    if (len < MIN || len > LENGTH)
    {
        return false;
    }
    
    for (int i = 0; i < len; i++)
    {
        // procedure lvl variable
        int letr = word[i];
        
        // set char value between 1 - 26
        if ((letr > 64) && (letr < 91))
        {
            letr -= 65;
        }
        else if((letr > 96) && (letr < 123))
        {
            letr -= 97;
        }
        // handle apostrophe
        else if(letr == 39)
        {
            letr = 26;
        }
        else
        {
            return false;
        }
        
        // if pointer is null, word not in used dictionary
        if (chkptr->children[letr] == NULL)
        {
            return false;
        }
        else
        {
            // move to next pointer
            chkptr = chkptr->children[letr];
        }
    }
    
    // if user input matches dict word, return true
    if (chkptr->real_word == true)
    {
        return true;
    }
    
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    /** 
    * using a trie data structure
    */
    
    // my node pointers
    root = calloc(1, sizeof(node));
    root->real_word = false;
    node* ptr = root;
    
    // local variable
    int step = 0;

    // open the hard-coded file
    FILE* words = fopen(dictionary, "r");
    
    // return false if it cannot be read
    if (words == NULL)
    {
        return false;
    }
    
    // read the contents
    while (!feof(words))
    {
        // procedure lvl variables
        int the_end = 0;
        
        // iteration through each word until it hits end of line
        for (int letr = fgetc(words); (((letr != '\'') && 
        (letr != '\n')) || (letr == 39)); letr = fgetc(words))
        {
            the_end = letr;

            // set char value between 1 - 26
            if ((letr > 64) && (letr < 91))
            {
                letr -= 65;
            }
            if ((letr > 96) && (letr < 123))
            {
                letr -= 97;
            }
            // apostrophe exception
            if (letr == 39)
            {
                letr = 26;
            }

            // if end of dictionary
            if (the_end < 0)
            {
                step = 1;
                break;
            }

            // check if pointer to array index is null
            if (ptr->children[letr] == NULL)
            {
                ptr->children[letr] = calloc(1, sizeof(node));
                ptr = ptr->children[letr];
            }
            else
            {
                // move to next pointer
                ptr = ptr->children[letr];
            }
        }

        // end of word procedures
        if (step != 1)
        {
            // demarcate real_word as true
            ptr->real_word = true;
            
            // reset to root
            ptr = root;
            
            // count number of words in dictionary
            no_of_words++;
        }
        else
        {
            // end of dictionary
            break;
        }
    }
    // close file pointer
    fclose(words);
    
    // return true once dictionary is loaded
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // number of words in dictionary
    return no_of_words;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // call function to free nodes recursively
    LetsUnload(root);
    return true;
}

// function to check if a node has a child node
bool HasChild(node* curNode)
{
    for (int i = 0; i < 27; i++)
    {
        // if it has a child node, return true
        if (curNode->children[i] != NULL)
        {
            return true;
        }
    }
    // else false
    return false;
}

// function that frees nodes recursively
void LetsUnload(node* curNode)
{
    // check if node has a child
    if (HasChild(curNode))
    {
        for (int i = 0; i < 27; i++)
        {
            // if child is't null then process it
            if (curNode->children[i] != NULL)
            {
                // function calls itself
                LetsUnload(curNode->children[i]);
            }
        }
        // free node
        free(curNode);
    }
    else
    {
        // free node
        free(curNode);
    }
}
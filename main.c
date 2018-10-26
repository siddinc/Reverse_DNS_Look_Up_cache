#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>
#include <malloc.h>
#include "dataset.h"

// There are atmost 11 different chars in a valid IP address 0-9 for numbers 0-9 and 10 for dot
#define CHARS 11

// Maximum length of a valid IP address
#define MAX 50

// A utility function to find index of child for a given character 'c'
int getIndex(char c) { return (c == '.')? 10: (c - '0'); }

// Trie Node
struct trieNode
{
    bool isLeaf;
    char *URL;
    struct trieNode *child[CHARS];
};

// Function to create a new trie node
struct trieNode *newTrieNode(void)
{
    struct trieNode *newNode;
    newNode = (struct trieNode *)malloc(sizeof(struct trieNode));
    newNode->isLeaf = false;
    newNode->URL = NULL;
    for (int i=0; i<CHARS; i++)
    {
        newNode->child[i] = NULL;
    }
    return newNode;
}

// This method inserts an IP address and the corresponding
// domain name in the trie. The last node in Trie contains the URL
void insert(struct trieNode *root, char *ipAdd, char *URL)
{
    // Length of the IP address
    int len = strlen(ipAdd);
    struct trieNode *pCrawl = root;

    // Traversing over the length of the IP address
    for (int level=0; level<len; level++)
    {
        // Get index of child node from current character
        // in ipAdd[].  Index must be from 0 to 10 where
        // 0 to 9 is used for digits and 10 for dot
        int index = getIndex(ipAdd[level]);

        // Create a new child if not exist already
        if (!pCrawl->child[index])
        {
            pCrawl->child[index] = newTrieNode();
        }

        // Move to the child
        pCrawl = pCrawl->child[index];
    }

    //Below needs to be carried out for the last node
    //Save the corresponding URL of the IP address in the
    //last node of trie
    pCrawl->isLeaf = true;
    pCrawl->URL = (char*) malloc(sizeof(char) * (strlen(URL) + 1));
    strcpy(pCrawl->URL, URL);
}

// This function returns URL if given IP address is present in DNS cache.
// Else returns NULL
char  *searchDNSCache(struct trieNode *root, char *ipSearched)
{
    // Root node of trie
    struct trieNode *pCrawl = root;
    int  len = strlen(ipSearched);

    // Traversal over the length of IP address.
    for (int level=0; level<len; level++)
    {
        int index = getIndex(ipSearched[level]);
        if (!pCrawl->child[index])
        {
            return NULL;
        }
        pCrawl = pCrawl->child[index];
    }

    // If we find the last node for a given IP address, print the URL
    if (pCrawl!=NULL && pCrawl->isLeaf)
    {
        return pCrawl->URL;
    }
    return NULL;
}

//Driver function.
void main()
{
    int run = 1, choice, i;
    int n = sizeof(ipAdd)/sizeof(ipAdd[0]); //size of the ipAdd array
    char ip[MAX];
    struct trieNode *root = newTrieNode();

    // Inserts all the IP addresses and their corresponding
    // domain name after IP address validation in the Trie
    for (int i=0; i<n; i++)
    {
        insert(root,ipAdd[i],URL[i]);
    }

    // If reverse DNS look up succeeds, print the domain
    // name along with DNS resolved.

    while(run)
    {
        printf("*******Main menu*******\n\n");
        printf("1. Resolve an IP address\n2. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
            {
                fflush(stdin);
                printf("Enter the IP address to be resolved: ");
                gets(ip);
                char *res_url = searchDNSCache(root, ip);
                if (res_url != NULL)
                {
                    printf("\nReverse DNS look up resolved in cache!\n\n%s --> %s\n\n", ip, res_url);
                }

                else
                {
                    printf("\nReverse DNS look up not resolved in cache!\n\n");
                }
                break;
            }

            case 2:
            {
                run = 0;
                break;
            }

            default:
            {
                printf("Invalid choice!");
                break;
            }
        }
    }
}

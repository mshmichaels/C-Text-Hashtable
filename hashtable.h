/*
 * CS 5800
 * Summer 2024
 * Miriam Michaels
 * Assignment 8: Hash for text (Problem 1)
 * hashtable.h
 * To compile: gcc try_hashtable.c hashtable.c -o testhash
 */

#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>





/**
 * Singly linked list node struct. Contains string as data attribute and the
 * number of times that string occurs as the occurrences attribute.
 * 
 * Attributes:
 *    char * data -- pointer to the data to be hashed
 *    int occurrences -- number of times the string has occurred
 *    node * next -- next node in the singly linked list
*/
typedef struct node {
  char * data;
  int occurrences;
  struct node * next;
} node_t;


/**
 * Pointer to a function that takes in a char * and an integer and returns an
 * integer. 
 * 
 * Parameters:
 *    char * entry -- pointer to the data to be hashed to find the index
 *    int no_of_buckets -- the number of buckets in the hashtable
 * Returns:
 *    int index -- index of the entry in the hashtable
*/
typedef int (*hashfunction_type)(char *,int);

/**
 * Hash function defined for the hash table. Adds 33, works better for smaller
 * M.
 * 
 * Parameters:
 *    char * entry -- pointer to the data to be hashed to find the index
 *    int no_of_buckets -- the number of buckets in the hashtable
 * Returns:
 *    int index -- index of the entry in the hashtable
*/
int hash_33(char * entry, int no_of_buckets);

/**
 * Hash function defined for the hash table. Adds 31, works better for bigger 
 * M.
 * 
 * Parameters:
 *    char * entry -- pointer to the data to be hashed to find the index
 *    int no_of_buckets -- the number of buckets in the hashtable
 * Returns:
 *    int index -- index of the entry in the hashtable
*/
int hash_31(char * entry, int no_of_buckets);

/**
 * Hashtable struct. Takes in the number of buckets and what hashfunction to use
 * at creation. 
 * 
 * Attributes:
 *    node_t ** buckets -- pointer to an array containing list head pointers
 *    int no_of_buckets -- size of array
 *    hashfunction_type hash_function -- hash function to use
 *    int * collisions -- table listing number of keys in buckets
*/
typedef struct hashtable {
  node_t ** buckets;
  int no_of_buckets; 

  hashfunction_type hash_function;
  int * collisions;
} hashtable_t;


/**
 * Reserves the appropriate memory for a hashtable struct as well as for the
 * underlying array based on an input number of buckets. It initializes all the
 * bucket node pointers to null. 
 * 
 * Parameters:
 *    int no_of_buckets -- the number of buckets in the hashtable
 * Returns:
 *    hashtable * table -- pointer to the created hashtable
*/
hashtable_t *create_hashtable(int buckets, hashfunction_type hashfunction);


/**
 * Takes in a hashtable and an entry to find. Uses the hash function to find 
 * the appropriate bucket to search. If the entry is in the bucket, returns 
 * the bucket index. Otherwise, returns -1. 
 * 
 * Parameters:
 *    hashtable * table -- pointer to the hashtable
 *    char * entry -- pointer to data to be added to the hashtable
 * Returns:
 *    int -- bucket if hashtable contains entry, -1 if not
*/
int hash_find(hashtable_t *table,char *entry);


/**
 * Deletes a hashtable entirely. Starts by moving through each bucket, and if 
 * the bucket is not NULL, frees every entry and then every node that holds the
 * entries. Once all the buckets are emptied, frees the bucket array memory, 
 * then the hashtable struct itself. 
 * Note: buckets are not set to NULL and no_of_buckets is not set to 0. This is
 * because when the whole table is freed, these values are also freed. If 
 * no_of_buckets is accessed, garbage data will be returned.
 * Parameters:
 *    hashtable * table -- pointer to the hashtable
*/
void delete_hashtable(hashtable_t *table);

/**
 * Deletes a key entirely from input hashtable. 
 * 
 * Parameters:
 *    hashtable * table -- pointer to the hashtable
 *    char * entry -- pointer to key to be removed from the hashtable
*/
void hash_delete_key(hashtable_t * table, char * entry);

/**
 * Need a function that reads in a file, tokenizes it on punctuation and white 
 * space, and then adds each token to the hashtable. 
 */
void read_and_hash(hashtable_t * table, char * filename);


/**
 * Writes the words and the number of times each word occurs to a CSV file.
 * 
 * Parameters: 
 *    hashtable * table -- pointer to the hashtable
 *    char * filename -- name of file to write to
 * Returns: 
 *    none, writes to file
 */
void list_all_keys(hashtable_t * table, char * filename);


/**
 * Writes the length of each chain in the table to a file.
 * Parameters: 
 *    hashtable * table -- pointer to the hashtable
 *    char * filename -- name of file to write to
 * Returns: 
 *    none, writes to file
 */
void output_collisions(hashtable_t * table, char * filename);


/**
 * Prints the contents of a particular bucket to the terminal. 
 * Parameters: 
 *    hashtable * table -- pointer to the hashtable
 *    char * filename -- name of file to write to
 * Returns: 
 *    none, writes to file
 */
void large_bucket(hashtable_t * table, int bucket, char * filename);

/**
 * Finds the largest 10% of buckets and prints them, their lengths, and their
 * entries. 
 * 
 * Parameters: 
 *    hashtable * table -- pointer to the hashtable
 */
void top_ten(hashtable_t * table);


/**
 * Finds the input kth order statistic and partitions the input list around it.
 * In this case, partitions both the collisions list and the actual buckets 
 * themselves, so their entries correspond and can be printed. 
 * 
 * Parameters: 
 *      int * A -- pointer to an array of integers
 *      node_t ** buckets -- pointer to an array of node_ts
 *      int left -- lower bound of the range
 *      int right -- upper bound of the range
 * Returns:
 *      int index -- index at which there are k smaller elements earlier
*/
int quick_select(int * A, node_t ** buckets, int left, int right, int k);


/**
 * Randomly selects a number between left and right inclusive.
 * 
 * Parameters: 
 *      int left -- lower bound of the range
 *      int right -- upper bound of the range
 * Returns:
 *      int pivot -- randomly picked number between left and right inclusive
*/
int pick_pivot(int left, int right);

/**
 * Partitions an array around a pivot point such that all the elements to the 
 * left of the pivot are smaller than the pivot's element and all the elements
 * to the right of the pivot are larger than the pivot index's element.
 * 
 * Parameters: 
 *      int * -- array of integers
 *      int left -- the smallest index in the subarray
 *      int right -- the largest index in the subarray
 *      int pivot -- index to pivot around
 * Returns:
 *      int pivot -- the new, sorted location of the pivot element
*/
int lomuto_partition(int * A, node_t ** buckets, int left, int right, int pivot_index);


/**
 * Swaps the elements at two indices of an array by using a temporary value to
 * store one element.
 * 
 * Parameters: 
 *      int * -- array of integers
 *      int first -- the first index to be swapped
 *      int second -- the index to be swapped with
*/
void swap(int * A, int first, int second);


/**
 * Swaps the elements at two indices of an array by using a temporary value to
 * store one element.
 * 
 * Parameters: 
 *      node_t ** -- array of node_ts
 *      int first -- the first index to be swapped
 *      int second -- the index to be swapped with
*/
void swap_node(node_t ** A, int first, int second);

/**
 * Increases the count of a word in the hashtable. 
 * 
 * Parameters: 
 *      hashtable * table -- pointer to the hashtable
 *      char * entry -- entry to increase count
*/
void hash_increase(hashtable_t * table, char * entry);
#endif

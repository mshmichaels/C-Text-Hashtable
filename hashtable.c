/*
 * CS 5800
 * Summer 2024
 * Miriam Michaels
 * Assignment 8: Hash for text (Problem 1)
 * hashtable.c
 * To compile: gcc tests_hashtable.c hashtable.c -o testhash
 */

#include "hashtable.h"

int hash_33(char * entry, int no_of_buckets){
  unsigned int index = 0;
  int length = strlen(entry);

  if (strcmp(entry, "") == 0){ // if the entry is an empty string
    return 0;
  }

  for (int i = 0; i < length; i ++) {
    index = index + ((int)entry[i] * (pow(i + 1, 3) + 33));
  }
  index = index % no_of_buckets;
  return index;
}

int hash_31(char * entry, int no_of_buckets){
  unsigned int index = 0;
  int length = strlen(entry);

  if (strcmp(entry, "") == 0){ // if the entry is an empty string
    return 0;
  }

  for (int i = 0; i < length; i ++) {
    index = index + ((int)entry[i] * (pow(i + 1, 3) + 31));
  }
  index = index % no_of_buckets;
  return index;
}


hashtable_t * create_hashtable(int no_of_buckets, hashfunction_type hash_function) {
  hashtable_t * table; // initializing pointer to hashtable

  // reserving appropriate memory for the hashtable
  table = (hashtable_t *) malloc (sizeof(hashtable_t));
  table -> buckets = (node_t **) malloc (no_of_buckets * sizeof(node_t *));
  table -> collisions = (int *) malloc (no_of_buckets * sizeof(int));

  table -> no_of_buckets = no_of_buckets; // setting number of buckets
  if (no_of_buckets < 1) { // can't have a 0 or negative number of buckets
    table -> no_of_buckets = 1;
  }
  
  // iterating through hashtable and setting the node pointers to NULL
  for (int i = 0; i < table -> no_of_buckets; i ++) {
    table -> buckets[i] = NULL;
    table -> collisions[i] = 0; // collisions table initializes to 0
  }

  table -> hash_function = hash_function; // setting hash function
  return table;
}


void hash_increase(hashtable_t * table, char * entry) {
  int index;
  node_t * current_node;
  
  // use hash function to find index of entry
  index = table -> hash_function(entry, table -> no_of_buckets);
  
  // check if entry is in hashtable, if so: increment its occurrences
  current_node = table -> buckets[index]; 
  while (current_node != NULL) {
    if (strcmp(entry, current_node -> data) == 0) { // compares the strings
      current_node -> occurrences ++;
      return;
    }
    current_node = current_node -> next;
  }
}

void hash_delete_key(hashtable_t * table, char * word) {
  char * entry;
  entry = strdup(word);
  // everything hashed in the table is lower case for accurate counts
  for (int i = 0; i < strlen(entry); i ++) {
    entry[i] = tolower(entry[i]);
  }
  
  int index = hash_find(table, entry);
  if (index > -1){
    node_t * current_node;
    node_t * prev;

    // find entry in hashtable, remove it
    current_node = table -> buckets[index]; 
    prev = NULL; 

    while (current_node != NULL) {
      if (strcmp(entry, current_node -> data) == 0) { // compares the strings
        if (prev == NULL) { // node was first in chain
          table -> buckets[index] = current_node -> next;
        }
        else {
          prev -> next = current_node -> next;
        }
        free(current_node);
        return;
      }
      prev = current_node;
      current_node = current_node -> next;
    }
  }
}


void hash_insert(hashtable_t * table, char * entry, int value) {
  int index;
  node_t * current_node;
  
  // use hash function to find index of entry
  index = table -> hash_function(entry, table -> no_of_buckets);

  // if entry was not found in hashtable already, adds it
  node_t * oldhead;
  oldhead = table -> buckets[index];

  // reserves memory for the size of the new node to be added
  node_t * newnode = (node_t *)malloc(sizeof(node_t));
  
  // reserves memory for and duplicates entry into node data
  newnode -> data = strdup(entry);
  newnode -> occurrences = value;

  // if the bucket is currently empty
  if (table -> buckets[index] == NULL) {
    newnode -> next = NULL; // means tail next will be null
  }
  else {
    newnode -> next = oldhead; // points to new next value
  }
  table -> buckets[index] = newnode;
  table -> collisions[index] = table -> collisions[index] + 1;
}


void list_all_keys(hashtable_t * table, char * filename){
  FILE * fpointer;
  fpointer = fopen(filename, "w+");
  fprintf(fpointer, "Keys,Counts\n");

  
  // iterates through all indices of table
  for (int i = 0; i < table -> no_of_buckets; i ++) {
    node_t * current = table -> buckets[i];

    // iterates through all nodes in chain, adds values to file
    while (current != NULL) {
      fprintf(fpointer, "%s,%d\n", current -> data, current -> occurrences);
      current = current -> next;
    }
  }
  fclose(fpointer);
}


void output_collisions(hashtable_t * table, char * filename) {
  FILE * fpointer;
  fpointer = fopen(filename, "w+");
  fprintf(fpointer, "Collisions\n");

  // iterates through list of collisions, adds values to file
  for (int i = 0; i < table -> no_of_buckets; i ++) {
    fprintf(fpointer, "%d\n", table -> collisions[i]);
  }
  fclose(fpointer);
}


void large_bucket(hashtable_t * table, int bucket, char * filename) {
  node_t * current;
  current = table -> buckets[bucket];
  printf("%d elements in in bucket %d:\n", table -> collisions[bucket], bucket);
  
  while (current != NULL) {
    printf("%s\n", current -> data);
    current = current -> next;
  }

}

void read_and_hash(hashtable_t * table, char * filename){
  FILE * fpointer;
  fpointer = fopen(filename, "r");
  int max = 1000;
  char line[max];
  const char * delim = "[]/_.,*!\"()?;:- \t\r\n";

  while (fgets(line, max - 1, fpointer)) {
    char * word;
    word = strtok(line, delim);

    // adding each word in the line to the hashtable
    while (word != NULL) {
      // making each word lower case for accurate word counts
      for (int i = 0; i < strlen(word); i ++) {
        word[i] = tolower(word[i]);
      }
      
      if (strcmp(word, "") != 0) {
        if (hash_find(table, word) == -1) {
          hash_insert(table, word, 1);
        }
        else {
          hash_increase(table, word);
        }
      }
      word = strtok(NULL, delim); // moving to next word in line
    }

  }
  fclose(fpointer);
}


int hash_find(hashtable_t *table,char *entry) {
  int index;
  node_t * current_node;
  
  // use hash function to find index of entry
  index = table -> hash_function(entry, table -> no_of_buckets);
  
  // traverse bucket at index to find entry
  current_node = table -> buckets[index]; 
  while (current_node != NULL) {
    if (strcmp(entry, current_node -> data) == 0) { // compares the strings
      return index;
    }
    current_node = current_node -> next;
  }

  return -1;
}


void delete_hashtable(hashtable_t * table){
  node_t * current_node;
  node_t * previous;
  
  // iterating through every bucket
  for (int i = 0; i < (table -> no_of_buckets); i ++) {
    if ((table-> buckets[i]) != NULL) {
      current_node = table -> buckets[i];
      previous = NULL;

      // freeing every node in the bucket
      while (current_node != NULL) {
        previous = current_node;
        current_node = current_node -> next;
        free(previous -> data); // frees the string IN the node
        free(previous); // frees the node itself
      }
      // setting each bucket head to NULL is pointless when array will be freed
    }
  }
  free(table -> buckets); // frees underlying array
  free(table -> collisions); // frees underlying array
  // setting buckets to NULL is pointless when entire structure will be freed
  
  free(table); // frees hashtable structure
}


void top_ten(hashtable_t * table){
  int ten_percent;
  ten_percent = (table -> no_of_buckets / 10) * 9;
  int index;
  index = quick_select(table -> collisions, table ->buckets, 0, table -> no_of_buckets - 1, ten_percent);
  for (int i = table -> no_of_buckets - 1; i >= index; i--) {
    large_bucket(table, i, "yoohoo.txt");
    printf("----------------------\n");
  }

}


int quick_select(int * A, node_t ** buckets, int left, int right, int k) {
  if (left == k) {
    return left;
  }
  int p = pick_pivot(left, right);
  p = lomuto_partition(A, buckets, left, right, p);
  int i = p - left + 1;
  // printf("i: %d, k: %d\n", i, k);
  if (i == k) {
    return p;
  }
  if (k < i) {
    // printf("left: %d, p-1: %d, k: %d\n", left, p-1, k);
    return quick_select(A, buckets, left, p - 1, k);
  }
  else {
    // printf("p+1: %d, right: %d, k-i: %d\n", p+1, right, k-i);
    return quick_select(A, buckets, p + 1, right, k - i);
  }
}


int pick_pivot(int left, int right){
    srand(200); //set the seed of the random number generator
    // left (lower end index) plus random integer mod # of indices
    return left + rand() % (right - left + 1);
}

int lomuto_partition(int * A, node_t ** buckets, int left, int right, int pivot_index){
    int pivot = A[pivot_index]; // pivot is element at pivot_index
    swap(A, pivot_index, right); // swapping to far right end
    swap_node(buckets, pivot_index, right);

    int i = left - 1; // starting before array
    // iterating left to right
    for (int j = left; j <= right - 1; j ++) {
        if (A[j] <= pivot) { // if element less than pivot
            i = i + 1; // i increments
            swap(A, i, j); 
            swap_node(buckets, i, j); 
        }
    }

    swap(A, i + 1, right); // pivot element ends up at current i + 1 index
    swap_node(buckets, i+1, right);

    return i + 1;    
}

void swap(int * A, int first, int second){
    int temp = A[first];
    A[first] = A[second];
    A[second] = temp;
}

void swap_node(node_t ** A, int first, int second){
    node_t * temp = A[first];
    A[first] = A[second];
    A[second] = temp;
}
/*
 * CS 5800
 * Summer 2024
 * Miriam Michaels
 * Assignment 8: Hash for text (Problem 1)
 * hashtable.c
 * To compile: gcc tests_hashtable.c hashtable.c -o testhash
 */

#include "hashtable.h"

int main() {
  char * input_text = "alice.txt";

  // hashtable with m=30
  hashtable_t * table_small = create_hashtable(30, hash_33);
  read_and_hash(table_small, input_text);
  output_collisions(table_small, "outputcoll30.csv");
  delete_hashtable(table_small);

  // hashtable with m=300
  hashtable_t * table_medium = create_hashtable(300, hash_33);
  read_and_hash(table_medium, input_text);
  output_collisions(table_medium, "outputcoll300.csv");
  list_all_keys(table_medium, "keys.csv");
  
  delete_hashtable(table_medium);

  // hashtable with m=1000
  hashtable_t * table_large = create_hashtable(1000, hash_31);
  read_and_hash(table_large, input_text);
  output_collisions(table_large, "outputcoll1000.csv");
  // top_ten(table_large);
  delete_hashtable(table_large);

  // demonstrates that the delete key function works as expected
  char * test_input = "test_upper.txt";
  hashtable_t * table = create_hashtable(12, hash_33);
  read_and_hash(table, test_input);
  list_all_keys(table, "keysbeforedelete.csv");
  hash_delete_key(table, "this"); // deleting a word that IS in the table
  list_all_keys(table, "keysafterdelete.csv");
  hash_delete_key(table, "keynotinfile"); // deleting a word not in the table
  list_all_keys(table, "keysafterseconddelete.csv");
  hash_increase(table, "bop");
  list_all_keys(table, "keysafterincrease.csv");
  delete_hashtable(table);

}

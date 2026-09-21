#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash_table.h"
#include "hash_table_iterator.h"
#include "hash_table_private.h"

#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"

/// @brief Process a single word, updating its frequency
/// @param word the word to process
/// @param ht a hash table containing the frequencies of the words found so far
void process_word(elem_t word, ioopm_hash_table_t *ht)
{
  elem_t freq = int_elem(0);

  // If the word already exsists in the ht, increase freq else add the word and init freq to 1
  if (ioopm_hash_table_lookup(ht, word, &freq))
  {
    freq.i += 1;
    ioopm_hash_table_insert(ht, word, freq);
  } else
  {
    ioopm_hash_table_insert(ht, string_elem(strdup(word.s)), int_elem(1));
  }
}

/// @brief Process a single file, updating the frequencies of its words
/// @param filename the name of the file to process
/// @param ht a hash table containing the frequencies of the words found so far
void process_file(const char *filename, ioopm_hash_table_t *ht)
{
  FILE *f = fopen(filename, "r");

  // Is file empty?
  if (f == NULL)
  {
    perror(filename);
    return;
  }

  char *buf = NULL;
  size_t len = 0;

  // Iterate through each word and add it to the ht
  while (getline(&buf, &len, f) != -1)
  {
    for (char *word = strtok(buf, Delimiters);
         word && *word;
         word = strtok(NULL, Delimiters))
    {
      process_word(string_elem(word), ht);
    }
  }

  free(buf);
  fclose(f);
}

/// @brief A word together with its frequency
struct freq_word
{
  char *word;
  size_t freq;
};

/// @brief Compare the frequency of two freq_words through pointers to them
/// @param p1 the first freq_word
/// @param p2 the second freq_word
/// @return a number @n@:
///     @n@ > 0  if @p1@'s frequency is higher than @p2@'s
///     @n@ < 0  if @p1@'s frequency is lower than @p2@'s
///     @n@ == 0 if @p1@'s frequency is equal to @p2@'s
static int cmp_freq_words(const void *p1, const void *p2)
{
  const struct freq_word *w1 = p1;
  const struct freq_word *w2 = p2;

  return w1->freq - w2->freq;
}

/// @brief Like @cmp_freq_words@ but with the comparison result reversed
static int cmp_freq_words_reverse(const void *p1, const void *p2)
{
  return -cmp_freq_words(p1, p2);
}

/// @brief Sort an array of @freq_word@s in descending frequency order
/// @param words the array to be sorted
/// @param no_words the number of elements in the array
void sort_freq_words(struct freq_word words[], size_t no_words)
{
  qsort(words, no_words, sizeof(struct freq_word), cmp_freq_words_reverse);
}



int main(int argc, char *argv[])
{
  // Check arguments
  if (argc < 2)
  {
    printf("Usage: %s file1 ... filen", argv[0]);
    return 1;
  }

  // Create hash table
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq);

  // Iterate over all arguments and process their files
  for (int i = 1; i < argc; ++i)
  {
    // stores all words from a file in a ht
    // The key is the word
    // The value is the freq of the word
    process_file(argv[i], ht);
  }

  // Create an array where each index contains a word and its frequency 
  size_t size = ioopm_hash_table_size(ht);
  struct freq_word freq_words[size];

  // Iterate over hash table to dump its words and
  // frequencies into the array above
  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  int i = 0;
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    size_t value = ioopm_hash_table_iterator_current_positive_value(it); 
    elem_t word = ioopm_hash_table_iterator_current_key(it); 
    
    // Place the value and word in the created array
    // The value is the freq of the word
    freq_words[i].freq = value; 
    freq_words[i].word = word.s; 
    
    i++;
    ioopm_hash_table_iterator_advance(it);
  }
  
  sort_freq_words(freq_words, size);

  // Print the result of the sorted array
  for (size_t i = 0; i < size; ++i)
  {
    printf("%s: %zd\n", freq_words[i].word, freq_words[i].freq);

    // Free the allocaded memory after print
    free(freq_words[i].word); 
  }

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
}

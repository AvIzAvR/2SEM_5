typedef struct CacheNode {
    char* key;
    char* value;
    struct CacheNode* prev;
    struct CacheNode* next;
} CacheNode;

typedef struct CacheList {
    CacheNode* head;
    CacheNode* tail;
} CacheList;

typedef struct Cache {
    int size;
    int capacity;
    CacheNode** table;
    CacheList* list;
} Cache;


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define DNS 100
#define MAX_WORD_LENGTH 256
#define MAX_CACHE_SIZE 3
#define HASH_SIZE 1024

#ifndef LAB5_HEADER_H
#define LAB5_HEADER_H

char* search_word(FILE* fp, Cache* cache);
int compare_word(char* word, char* str);
void free_words(char **words, int count);
char* find_previous_word(char** words, int count, char* target_word);
int is_valid_ip_address(char *str);
char* check_input(FILE* fp, char* dns);
void remove_word(char** words, int* count, int index);
char* find_result_from_cache(Cache* cache, FILE* fp, char* str, int* add_new);
void add_to_cache(Cache* cache, const char* key, const char* value);

void delete_cache(Cache* cache);
Cache* create_cache(int capacity);
void print_cache(Cache* cache);
void find_ip(Cache* cache);
void print_all_dns();
void add_adress();
void add_cname();


#endif //LAB5_HEADER_H

#include "header.h"

unsigned int get_hash(char* key) {
    unsigned int hash = 0;
    int i;
    for (i = 0; i < strlen(key); i++) {
        hash = (hash * 47 + key[i]) % HASH_SIZE;
    }
    return hash;
}

Cache* create_cache(int capacity) {
    Cache* cache = (Cache*) malloc(sizeof(Cache));
    cache->size = 0;
    cache->capacity = capacity;
    cache->table = (CacheNode**) calloc(HASH_SIZE, sizeof(CacheNode*));
    cache->list = (CacheList*) malloc(sizeof(CacheList));
    cache->list->head = NULL;
    cache->list->tail = NULL;
    return cache;
}

void delete_cache(Cache* cache) {
    CacheNode* node = cache->list->head;
    while (node != NULL) {
        CacheNode* next = node->next;
        free(node->key);
        free(node->value);
        free(node);
        node = next;
    }
    free(cache->list);
    free(cache->table);
    free(cache);
}



void add_to_cache(Cache* cache, const char* key, const char* value) {
    unsigned int hash = get_hash(key);
    CacheNode* node = cache->table[hash];
    CacheNode* prev = NULL;
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            free(node->value);
            node->value = strdup(value);
            if (prev != NULL) {
                prev->next = node->next;
                if (node->next != NULL) {
                    node->next->prev = prev;
                }
                node->prev = NULL;
                node->next = cache->table[hash];
                cache->table[hash]->prev = node;
                cache->table[hash] = node;
            }
            // Update the position in the list
            if (cache->list->head != node) {
                if (node->next != NULL) {
                    node->next->prev = node->prev;
                } else {
                    cache->list->tail = node->prev;
                }
                node->prev->next = node->next;
                node->prev = NULL;
                node->next = cache->list->head;
                cache->list->head->prev = node;
                cache->list->head = node;
            }
            return;
        }
        prev = node;
        node = node->next;
    }

    node = (CacheNode*) malloc(sizeof(CacheNode));
    node->key = strdup(key);
    node->value = strdup(value);
    node->prev = NULL;
    node->next = cache->table[hash];
    if (node->next != NULL) {
        node->next->prev = node;
    }
    cache->table[hash] = node;
    cache->size++;
    // Add the node to the front of the list
    if (cache->list->head != NULL) {
        node->next = cache->list->head;
        cache->list->head->prev = node;
        cache->list->head = node;
    } else {
        cache->list->head = node;
        cache->list->tail = node;
    }

    if (cache->size > cache->capacity) {
        CacheNode* lru_node = cache->list->tail;
        cache->list->tail = lru_node->prev;
        if (cache->list->tail != NULL) {
            cache->list->tail->next = NULL;
        } else {
            cache->list->head = NULL;
        }

        unsigned int lru_hash = get_hash(lru_node->key);
        if (cache->table[lru_hash] == lru_node) {
            cache->table[lru_hash] = NULL;
        }


        free(lru_node->key);
        free(lru_node->value);
        free(lru_node);

        cache->size--;
    }
}

char* get_from_cache(Cache* cache, char* key) {
    unsigned int hash = get_hash(key);
    CacheNode *node = cache->table[hash];
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            if (cache->list->head != node) {
                if (node->next != NULL) {
                    node->next->prev = node->prev;
                } else {
                    cache->list->tail = node->prev;
                }
                node->prev->next = node->next;
                node->prev = NULL;
                node->next = cache->list->head;
                cache->list->head->prev = node;
                cache->list->head = node;
            }
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

char* find_result_from_cache(Cache* cache, FILE* fp, char* str, int* add_new) {
    char* result = get_from_cache(cache, str);
    if (result != NULL) {
        if (cache->size == 1)
            (*add_new)++;
        return result;
    }
    rewind(fp);
    char current_word[MAX_WORD_LENGTH];
    char previous_word[MAX_WORD_LENGTH];
    int found = 0;

    while (fscanf(fp, "%s", current_word) == 1) {
        if (isdigit(str[0])) {
            if (found) {
                result = strdup(previous_word);
                break;
            }

            if (compare_word(current_word, str)) {
                found = 1;
                continue;
            }

            strcpy(previous_word, current_word);
        }

        if (isalpha(str[0])) {
            if (found) {
                if (isalpha(current_word[0])) {
                    result = find_result_from_cache(cache, fp, current_word, add_new);
                    break;
                }
                result = strdup(current_word);
                break;
            }

            if (compare_word(current_word, str)) {
                found = 1;
                continue;
            }
        }
    }
    if (compare_word(current_word, str)) {
        result = strdup(previous_word);
    }


    return result;
}



void print_cache(Cache* cache) {
    CacheNode* node = cache->list->head;
    int num = 1;
    printf("Cache:\n");
    while (node != NULL) {
        printf("%d: %s: %s\n", num, node->key, node->value);
        node = node->next;
        num++;
    }
    if(num == 1)
        printf("The cache is empty =(\n");
}

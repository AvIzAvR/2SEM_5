#include "header.h"

int compare_word(char* word, char* str) {
    return strcmp(word, str) == 0;
}


void remove_word(char** words, int* count, int index) {
    if (index < 0 || index >= *count) {
        printf("Invalid index.\n");
        return;
    }
    int i;
    for (i = index; i < *count-1; i++) {
        words[i] = words[i+1];
    }
    words[*count-1] = NULL;
    (*count)--;
}

int is_valid_ip_address(char *ip_address) {
    int octet;
    int num_octets = 0;
    int num_chars = 0;
    int num_digits = 0;

    while (*ip_address != '\0') {
        if (*ip_address == '.') {
            if (num_chars == 0 || num_digits == 0 || octet > 255) {
                return 0;
            }
            num_octets++;
            octet = 0;
            num_chars = 0;
            num_digits = 0;
        }
        else if (*ip_address >= '0' && *ip_address <= '9') {
            octet = octet * 10 + (*ip_address - '0');
            num_chars++;
            num_digits++;
            if (num_digits > 3) {
                return 0;
            }
        }
        else {
            return 0;
        }
        ip_address++;
    }
    if (num_octets != 3 || num_chars == 0 || num_digits == 0 || octet > 255) {
        return 0;
    }

    return 1;
}

char* check_input(FILE* fp, char* dns) {
    scanf(" %99s", dns);
    while (1) {
        int valid_ip = is_valid_ip_address(dns);
        int word_found = 0;
        char word[MAX_WORD_LENGTH];
        rewind(fp);
        while(fscanf(fp, "%s", word) == 1)
            if (compare_word(dns, word)) {
                word_found = 1;
                break;
            }
        if (valid_ip && !word_found) {
            return dns;
        }
        else if(!valid_ip) {
            printf("Invalid input. Please enter a valid IP address:\n");
        }
        else if(word_found)
            printf("Invalid input. IP address found in the file:\n");
        scanf("%99s", dns);
    }
}


char* search_word(FILE* fp, Cache* cache) {
    char str[MAX_WORD_LENGTH];
    scanf("%99s", str);
    int add_new = 0;
    char* result = find_result_from_cache(cache, fp, str, &add_new);
    if (result != NULL)
        add_to_cache(cache, str, result);
    return result;
}



void free_words(char **words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
}

char* find_previous_word(char** words, int count, char* target_word) {
    int i = 0;
    char* result = NULL;
    result = (char*)calloc(MAX_WORD_LENGTH, sizeof(char));
    int fl = 0;
    for (int z = 0; z < count; z++) {
        if (strcmp(words[z], target_word) == 0)
            fl++;
    }
    while(fl == 0) {
        printf("IP not found. Enter the correct IP:\n");
        rewind(stdin);
        scanf("%99s", target_word);
        for (int z = 0; z < count; z++) {
            if (strcmp(words[z], target_word) == 0)
                fl++;
        }
    }
    while (i < count) {
        if (strcmp(words[i], target_word) == 0) {
            if (i > 0) {
                strcat(result, words[i-1]);
                strcat(result, "\n");
                strcpy(target_word, words[i-1]);
                remove_word(words, &count, i);
                remove_word(words, &count, i-1);
                i = 0;
            } else {
                i++;
            }
        } else {
            i++;
        }
    }
    return result;
}

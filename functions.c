#include "header.h"

void find_ip(Cache* cache) {
    printf("Write DNS or IP adress: ");
    FILE *f_dns = fopen("../dns.txt", "r");
    if (f_dns == NULL)
        printf("\nInvalid open dns.txt");
    char* result = search_word(f_dns, cache);
    if (result != NULL) {
        printf("Result: %s\n", result);
    } else {
        printf("No match found\n");
    }
    fclose(f_dns);
};

void print_all_dns() {
    printf("Write IP address: ");
    char *dns = (char *) calloc(DNS, sizeof(char));
    if (dns == NULL) {
        printf("\nMemory allocation error");
        exit(1);
    }
    scanf("%99s", dns);
    int add_new = 0;
    while (!is_valid_ip_address(dns)) {
        printf("Invalid IP address\n");
        rewind(stdin);
        scanf("%99s", dns);
    }
    FILE *f_dns = fopen("../dns.txt", "r");
    if (f_dns == NULL) {
        printf("\nInvalid open dns.txt");
        free(dns);
        exit(1);
    }
    char current_word[MAX_WORD_LENGTH];
    char previous_word[MAX_WORD_LENGTH];
    int found = 0;
    while (fscanf(f_dns, "%s", current_word) == 1) {
        if (compare_word(current_word, dns)) {
            found = 1;
            continue;
        }
        if (found == 1) {
            printf("%s\n", previous_word);
            dns = strdup(previous_word);
            found = 0;
        }

        strncpy(previous_word, current_word, MAX_WORD_LENGTH);
        }

    if (compare_word(current_word, dns)) {
        printf("%s\n", previous_word);
    }

    free(dns);
    fclose(f_dns);
}


void add_adress() {
    FILE *f_dns = fopen("../dns.txt", "a+");
    if (f_dns == NULL) {
        printf("\nInvalid open dns.txt");
        exit(1);
    }
    char *dns = (char *) calloc(DNS, sizeof(char));
    if (dns == NULL) {
        printf("\nMemory allocation error");
        exit(1);
    }
    char* result = (char*)calloc(DNS, sizeof(char));
    printf("Write DNS address: ");
    scanf("%99s", dns);
    char word[MAX_WORD_LENGTH];
    while(fscanf(f_dns, "%s", word) == 1) {
        if(compare_word(dns, word)) {
            printf("DNS repetition found\n");
            rewind(stdin);
            rewind(f_dns);
            scanf("%99s", dns);
        }
    }
    strcat(result, "\n");
    strcat(result, dns);
    strcat(result, " ");
    rewind(stdin);
    printf("Write IP address: ");
    dns = check_input(f_dns, dns);
    strcat(result, dns);

    fprintf(f_dns, "%s", result);
    free(dns);
    fclose(f_dns);
}


void add_cname() {
    FILE *f_dns = fopen("../dns.txt", "a+");
    if (f_dns == NULL) {
        printf("\nInvalid open dns.txt");
        exit(1);
    }
    char *dns = (char *) calloc(DNS, sizeof(char));
    if (dns == NULL) {
        printf("\nMemory allocation error");
        exit(1);
    }
    char* result = (char*)calloc(DNS, sizeof(char));
    printf("Write DNS address: ");
    scanf("%99s", dns);
    char word[MAX_WORD_LENGTH];
    while(fscanf(f_dns, "%s", word) == 1) {
        if(compare_word(dns, word)) {
            printf("DNS repetition found\n");
            rewind(stdin);
            rewind(f_dns);
            scanf("%99s", dns);
        }
    }
    strcat(result, "\n");
    strcat(result, dns);
    strcat(result, " ");
    rewind(stdin);
    printf("Write CNAME address: ");
    scanf("%99s", dns);
    int fl = 0;
    rewind(f_dns);
    while(fl == 0) {
        while(fscanf(f_dns, "%s", word) == 1) {
            if(compare_word(dns, word)) {
                fl++;
            }
        }
        if(fl == 0) {
            printf("CNAME repetition not found\n");
            rewind(stdin);
            rewind(f_dns);
            scanf("%99s", dns);
        }
    }
    strcat(result, dns);
    fprintf(f_dns, "%s", result);

    free(dns);
    fclose(f_dns);
}
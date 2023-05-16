#include "header.h"


int main() {
    Cache* cache = create_cache(MAX_CACHE_SIZE);
    while (1) {
        int choise = 0;
        printf("Select an action:\n\n1 - Print all DNS\n2 - Add new address\n3 - Add new CNAME\n4 - Find IP address or DNS\n5 - Show Cache\n6 - Delete Cache\n7 - EXIT\n");
        while(!scanf("%d", &choise) || choise > 7 || choise < 1) {
            rewind(stdin);
            printf("Wrong choice!\n");
        }
        switch (choise) {
            case 1: {
                print_all_dns();
                break;
            }
            case 2: {
                add_adress();
                break;
            }
            case 3: {
                add_cname();
                break;
            }
            case 4: {
                find_ip(cache);
                break;
            }
            case 5: {
                print_cache(cache);
                break;
            }
            case 6: {
                delete_cache(cache);
                cache = create_cache(MAX_CACHE_SIZE);
                break;
            }
            case 7: {
                exit(0);
            }
        }
        rewind(stdin);
        choise = 0;
    }
}



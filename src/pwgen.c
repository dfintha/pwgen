#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "messages.h"

typedef void * (*memset_ptr)(void *, int, size_t);

typedef struct {
    size_t length;
    size_t amount;
    bool has_specials;
    bool has_numbers;
    bool has_lowercase;
    bool no_warning;
    bool show_help;
    char *excluded;
} env_t;

static void process_params(int argc, char **argv, env_t *flags);
static bool is_valid(char c, const env_t *flags);

int main(int argc, char **argv) {
    env_t environment = {
        .length = 16,
        .amount = 1,
        .has_specials = true,
        .has_numbers = true,
        .has_lowercase = true,
        .no_warning = false,
        .show_help = false,
        .excluded = NULL
    };

    process_params(--argc, ++argv, &environment);

    if (environment.show_help) {
        puts(info_help_message);
        return 0;
    }

    if (!environment.no_warning) {
        if (environment.length < 16)
            fputs(warn_short_pw, stderr);

        if (!environment.has_numbers)
            fputs(warn_no_numbers, stderr);

        if (!environment.has_lowercase)
            fputs(warn_no_lowercase, stderr);
    }

    int urandom = open("/dev/urandom", O_RDONLY);
    if (urandom == -1) {
        fputs(err_fd_urandom, stderr);
        return 1;
    }


    char* buffer = malloc(environment.length + 1);
    if (buffer == NULL) {
        fputs(err_memory_alloc, stderr);
        close(urandom);
        return 1;
    }

    buffer[environment.length] = '\0';
    for (size_t a = 0; a < environment.amount; ++a) {
        for (size_t l = 0; l < environment.length; ++l) {
            char current;
            do {
                read(urandom, &current, 1);
                current = current % 255;
            } while (!is_valid(current, &environment));
            buffer[l] = current;
        }

        puts(buffer);
    }

    close(urandom);
    volatile memset_ptr memset_noopt = memset;
    memset_noopt(buffer, 0, environment.length);
    if (environment.excluded != NULL)
        free(environment.excluded);
    free(buffer);
    return 0;
}

static void process_params(int argc, char **argv, env_t *flags) {
    if (argv == NULL)
        return;

    for (int i = 0; i < argc; ++i) {
        if (argv[i] == NULL || argv[i][0] != '-')
            continue;

        switch (argv[i][1]) {
            case 'n':
            case 'l':
                {
                    size_t * const env = (argv[i][1] == 'n') ? &flags->amount
                                                             : &flags->length;
                    const int len = strlen(argv[i]);
                    for (int j = 2; j < len; ++j) {
                        argv[i][j - 2] = argv[i][j];
                    }
                    argv[i][len - 2] = '\0';

                    *env = (size_t) atoi(argv[i]);
                }
                break;

            case 'E':
                {
                    if (argv[i][2] != '=')
                        continue;

                    const char *exc = argv[i] + 3;
                    if (flags->excluded == NULL) {
                        size_t len = strlen(exc) + 1;
                        flags->excluded = (char *) malloc(len);
                        strcpy(flags->excluded, exc);
                    } else {
                        char *old = flags->excluded;
                        size_t len = strlen(exc) + 
                                     strlen(flags->excluded) + 1;
                        flags->excluded = (char *) malloc(len);
                        strcpy(flags->excluded, old);
                        free(old);
                        strcat(flags->excluded, exc);
                    }
                }
                break;

            default:
                {
                    for (size_t n = 1; n < strlen(argv[i]); ++n) {
                        #define handle_flag(ch, name, val) \
                            if (argv[i][n] == ch) flags->name = val

                        handle_flag('L', has_lowercase, false);
                        handle_flag('N', has_numbers, false);
                        handle_flag('S', has_specials, false);
                        handle_flag('W', no_warning, true);

                        handle_flag('H', show_help, true);
                        handle_flag('h', show_help, true);

                        #undef handle_flag
                    }
                }
                break;
        }
    }
}

static bool is_valid(char c, const env_t *flags) {
    const int cc = c;

    if (!isprint(cc) || !isgraph(cc) || iscntrl(cc))
        return false;

    if (!flags->has_numbers && isdigit(cc))
        return false;

    if (!flags->has_lowercase && isalpha(cc) && islower(cc))
        return false;

    if (!flags->has_specials && !isalnum(cc))
        return false;

    if (flags->excluded != NULL) {
        for (unsigned i = 0; i < strlen(flags->excluded); ++i) {
            if (c == flags->excluded[i])
                return false;
        }
    }

    return true;
}


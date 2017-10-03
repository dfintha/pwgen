#include <ctype.h>      // character class checks
#include <fcntl.h>      // open
#include <stdbool.h>    // bool
#include <stdio.h>      // fprintf, printf
#include <stdlib.h>     // free, malloc
#include <string.h>     // memset
#include <unistd.h>     // close, read

typedef void * (*memset_ptr)(void *, int, size_t);

typedef struct {
    size_t length;
    size_t amount;
    bool has_specials;
    bool has_numbers;
    bool has_lowercase;
    bool no_warning;
    bool show_help;
} env_t;

static void process_params(int argc, char **argv, env_t *flags);
static bool is_valid(char c, const env_t *flags);
static void show_help(void);

int main(int argc, char **argv) {
    env_t environment = {
        .length = 16,
        .amount = 1,
        .has_specials = true,
        .has_numbers = true,
        .has_lowercase = true,
        .no_warning = false,
        .show_help = false
    };

    process_params(--argc, ++argv, &environment);

    if (environment.show_help) {
        show_help();
        return 0;
    }

    if (!environment.no_warning) {
        if (environment.length < 16) {
            fprintf(stderr, "security warning: passwords under 16 characters "
                            "long are not considered secure, consider "
                            "generating longer passwords\n");
        }

        if (!environment.has_numbers) {
            fprintf(stderr, "security warning: not including numbers into "
                            "passwords makes them considerably easier to "
                            "crack, consider using numbers in passwords\n");
        }

        if (!environment.has_lowercase) {
            fprintf(stderr, "security warning: using case-insensitive "
                            "passwords makes them considerably easier to "
                            "crack, consider using lowercase letters, too\n");
        }
    }

    int urandom = open("/dev/urandom", O_RDONLY);
    if (urandom == -1) {
        fprintf(stderr, "error: can't get file descriptor to urandom\n");
        return 1;
    }


    char* buffer = malloc(environment.length + 1);
    if (buffer == NULL) {
        fprintf(stderr, "error: could not allocate memory on the heap\n");
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

        printf("%s\n", buffer);
    }

    close(urandom);
    volatile memset_ptr memset_noopt = memset;
    memset_noopt(buffer, 0, environment.length);
    free(buffer);
    return 0;
}

static void process_params(int argc, char **argv, env_t *flags) {
    if (argv == NULL) {
        return;
    }

    for (int i = 0; i < argc; ++i) {
        if (argv[i] == NULL || argv[i][0] != '-') {
            continue;
        }

        switch (argv[i][1]) {
            case 'n':
            case 'l':
                {
                    size_t *where = (argv[i][1] == 'n') ? &flags->amount
                                                        : &flags->length;
                    const int len = strlen(argv[i]);
                    for (int j = 2; j < len; ++j) {
                        argv[i][j - 2] = argv[i][j];
                    }
                    argv[i][len - 2] = '\0';

                    *where = (size_t) atoi(argv[i]);
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

                        // --help triggers this
                        handle_flag('h', show_help, true);

                        #undef handle_flag
                    }
                }
                break;
        }
    }
}

static void show_help(void) {
    printf("usage: pwgen [OPTIONS]\n"
           "Generate passwords with specified complexity.\n"
           "\n"
           "  -nN       generate N passwords\n"
           "  -lN       generated passwords are N characters long\n"
           "  -L        exclude lowercase characters\n"
           "  -N        exclude numbers\n"
           "  -S        exclude special characters\n"
           "  -W        disable warnings for weak passwords\n"
           "  -H, -h    show this help and don't generate any passwords\n"
           "\n"
           "Warnings are issued for weak passwords, if the specified length\n"
           "is smaller than 16 characters, or if lowercase characters or \n"
           "numbers are excluded from the passwords.\n"
           "\n"
           "By default, one password is generated, which is 16 characters\n"
           "long, and includes all possible character types.\n");
}

static bool is_valid(char c, const env_t *flags) {
    if (!isprint(c) || !isgraph(c) || iscntrl(c))
        return false;

    if (!flags->has_numbers && isdigit(c))
        return false;

    if (!flags->has_lowercase && isalpha(c) && islower(c))
        return false;

    if (!flags->has_specials && !isalnum(c)) 
        return false;

    return true;
}

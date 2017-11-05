#if !defined(PWGEN_MESSAGES)
#define PWGEN_MESSAGES

static const char warn_short_pw[] = 
    "warning: passwords shorter than 16 characters are considered insecure\n";

static const char warn_no_numbers[] = 
    "warning: passwords without numbers are considered insecure\n";

static const char warn_no_lowercase[] = 
    "warning: passwords without lowercase letters are considered insecure\n";

static const char err_fd_urandom[] = 
    "error: can't get file descriptor for urandom device\n";

static const char err_memory_alloc[] = 
    "error: can't allocate memory on the heap\n";

static const char info_help_message[] =
    "usage: pwgen [OPTIONS]\n"
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
    "is smaller than 16 characters, or if lowercase characters and/or \n"
    "numbers are excluded from the passwords.\n"
    "\n"
    "By default, one password is generated, which is 16 characters\n"
    "long, and includes all possible character types.";

#endif


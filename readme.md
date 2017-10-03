# pwgen

This repository contains a very basic password generator, utilizing the urandom
feature of UNIX-like operating systems. The program emits a warning if the user
attempts to create insecure passwords, but this functionality can be disabled.

By default, it creates one password, which is 16 characters long, and contains
numbers, upper- and lowercase letters, and special characters alike.

For more information about the program's command-line arguments issue the
__pwgen --help__ command.


Project 1
=========

To compile: make all

Usage of runCommand: ./runCommand <name> <args>

Usage of shell: ./shell
				./shell < <file>

Shell will search the user's PATH for any commands where a full path is not entered. To exit the shell, use ^C, as the 'exit' command has not yet been implemented.

As dictated in the project specifications, the shell will allocate arrays large enough for 32 distinct arguments, assuming each argument has a parameter associated with it, and the command name is not considered an argument. If this condition is violated, an error is printed and the input is discarded.

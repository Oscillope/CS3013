Project 1
=========

To compile: make all

Usage of runCommand: ./runCommand <name> <args>

Usage of shell: ./shell
				./shell < <file>

Shell will search the user's PATH for any commands where a full path is not entered. To exit the shell, type `exit`.

As dictated in the project specifications, the shell will allocate arrays large enough for 32 distinct arguments, assuming each argument has a parameter associated with it, and the command name is not considered an argument. If this condition is violated, an error is printed and the input is discarded.

Usage of shell2: ./shell2
				 ./shell2 < <file>
				 
Shell2 will keep background jobs with an & after the command. Job information is kept using an array of pointers to job structs. The job struct contains the job number, PID, start and end times, and a pointer to a character array containing the name of the command. After executing any foreground commands, shell2 will wait3 using the WNOHANG option to check if any jobs have been completed. If they have, it will iterate through the array to locate the completed job based on its PID, and print relevant information about the job. When a new background job is created, shell2 will expand the array of jobs and increment the job counter. When it detects that a job has been completed, that job's struct will be freed and the job counter will be decremented.

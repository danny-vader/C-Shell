# **Operating Systems and Networks, Monsoon 2022**
## **Assignment 3**
## **Gaurav Bansal | 2021101022 | Fedora 35**
---
***

## Compilation and Exectution Instructions
---
> In the directory containing the program files, run the following commands on the terminal to compile and create the executable `shell` and run the exectuable file obtained. 

```sh
$ make all
$ ./shell
```
## Shell Features
---

> The shell is essentially built using C programs arranged in a modular fashion to provide functionality for commands directly implemented in a linux terminal. It is capable of running all possible system commands, user commands and inbuilt commands. For some commands, the program files have been written in C, through which they are implemented, whereas for some, a simple fork is being used to run the system command in background or in foreground as required. Below, we find the purpose of each file, post that we find the possible commands that the shell can run. The shell is also capable of implementing input-output redirected command as well as piped commands, alongwith auto-completion while entering the commands. The shell can also run signals like `Ctrl + C`, `Ctrl + Z` and `Ctrl + D`

**Note** - _The shell format of display, exactly matches to that of an actual linux terminal, in this case of fedora 35, as it was being built on a fedora machine_
## Code Files
---
| File | Details |
| -----| ------------|
| background.c, background.h | Implements the execution of system commands in the background, which include all those commands for which an implementation hasn't been made seperately in the shell |
| bg.c, bg.h | Implements the user-defined `bg` command |
| cd.c, cd.h | Implements the inbuilt `cd` command |
| discover.c, discover.h | Implements the user-defined `discover` command |
| echo.c, echo.h | Implements the inbuilt `echo` command |
| execute.c, execute.h | Implements the exeuction of all commands, by calling them through their respective functions passing the required arguments, also calling the input-output redirection function |
| fg.c, fg.h | Implements the user-defined `fg` command |
| foreground.c, foreground.h | Implements the execution of system commands in the foreground, which include all those commands for which an implementation hasn't been made seperately in the shell |
| headers.c, headers.h | Includes all the external inbuilt header files into the program, also declaring all global variables, struct and macros |
| history.c, history.h | Implements the `history` command, also keeping track of the recent history through a history file and array |
| init.c, init.h | Initialises the shell by initialising parameters such as the cwd, pwd, home directory, shell pid, history file, username, system name and the background process linked list |
| jobs.c, jobs.h | Implements the execution of the user-defined `jobs` command and routines for the linked list of background processes |
| ls.c, ls.h | Implements the execution of the inbuilt `ls` command |
| main.c | The main function of the shell, which is responsible for calling appropriate functions for initialising, ignoring signals, execution, piping, tokenizations, reading the input, auto-completion, and handling child process signals for background process |
| makefile | Implements the compilation of the shell program, by compiling all the files using a single `make all` command into an executable file `shell` |
| misc.c, misc.h | Implements side routines like colour changes of the output colour onto the terminal |
| pinfo.c, pinfo.h | Implements the user-defined `pinfo` command, which prints process-related information about a process or the shell process by reading deatils from the `/proc` file using the `pid` of the process |
| pipe.c, pipe.h | Implements piping in the shell by tokenizing on the `pipe` symbol and calling the exeuction for each of the commands within the `pipe`|
| prompt.c, prompt.h | Implements the printing of the prompt of the terminal, which contains the username, the system-name and the current working directory, considering the relative paths from the home directory |
| pwd.c, pwd.h | Implements the execution of the `pwd` command |
| redirection.c, redirection.h| Implements input output redirection in the shell taking place using  `<` , `>` , `>>` symbols like in an actual shell |
| sig.c, sig.h | Implements the user-defined `sig` command |
| tokenize.c, tokenize.h | Implements tokenization of the input command, firstly taking `;` as the delimiter (to identify processes to be run in the sequence) and then taking `&` as the delimiter (to identify background processes) |
## Commands
---
> The commands implemented by the shell, alongside necessary assumptions related to each command is as follows : -

1. `bg`
    * It is an error for the `bg` command to not have exactly one command-line argument
    * It is an error for the `bg` command if the argument passed is not a positive integer, that is a valid job number
    * It is an error for the `bg` command if no background job corresponding to the given job number exists.
2. `cd`
    * It is an error for the `cd` command to have more than one command-line argument
    * It is an error for the `cd` command, if the target directory doesn't exist
    * `cd` supports flags such as `'.'`, `'..'`, `'~'`, `'-'`

2. `pwd`
    * The `pwd` command outputs the full path of the current working directory, the absolute path, not in refernce with `~`.

3. `echo`
    * The `echo` command outputs the arguments irrespective of the spaces given, just by seperating strings

4. `ls`
    * It outputs the contents of a file, if it exits in different colours
    * If it doesn't exist, it throws an error 

5. `sig`
    * `sig` command throws an error if exactly two command line arguments aren't passed with the command, and also if these arguments aren't positive integers, that is, valid job and signal numbers
    * `sig` command throws an error if no background job corresponding to the job number exists

5. `pinfo`
    * `pinfo` prints the process - related info of the shell program, if no argument is given
    * `pinfo` prints the process - related info of the given pid, if an argument in the form of a pid of a process is given
    * It is an error for the `pinfo` command to have more than one command-line argument
    * The process-related info, consists of the pid, the status code, the virtual memory and the executable path of the process
    * The status code is followed by a '+' if the proces is running in the foreground

6. `discover`
    * The `discover` command is a slightly more efficient and advanced version of the `find` command in a linux shell, which searches for a file in a directory hierarchy
    * The `discover` command followed by a directory, to print all the contents, prints the name of the directory at the top
    * It is an error if the file to be searched for is not found in the required target directory

7. `history`
    * This command stores the history, for a maximum of 20 commands, by creating a file `history.txt` in the same folder as the other program files
    * This file is being created, destroyed and again created upon the entry of a new command, to update the `history.txt` file 

8. `system commands`
    * The system commands other than the one mentioned above can be runned both in foreground and in background
    * If the process runs in the background, the completion message would come immediately, and on presseing an enter, the new prompt comes.
    * Errors have been handled at the requisite areas
    * On ending a background process, complete command is shown

9. `signals`
    * `Ctrl + C` only ends one process that is currently running in the foreground
    * `Ctrl + D` works when no process is running in the foreground
## Assumptions
---
> Apart from the assumptions taken for the functioning of all the commands, some general assumptions have also been taken, stated as follows :-

* The maximum size of a single command, directory, path to a directory is taken to be 1000 using the macro defined as `#define MAX_SIZE 1000` 
* Error handling for standard C functions is being done using `perror` whereas for some functions, it is done by printing off the error message onto `stderr`
* The maximum commands stored in the history are 20
* The total time taken by all system commands as foreground processes entered in a single command is being printed on the prompt if it is greater than 1s
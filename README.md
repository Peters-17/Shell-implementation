# Objectives
1.	To gain yet more familiarity with programming in C and in Linux
2.	To learn how processes are handled with fork(), exec(), and wait()
3.	To gain exposure to some of the functionality in shells
4.	To understand how redirection of stdout works
## Score: 120/120
## Overview

Implement a command line interpreter, or shell, on top of Unix. The shell should operate in this basic way: when you type in a command (in response to its prompt), the shell creates a child process that executes the command you entered and then prompts for more user input when it has finished.  More specifically, shells are typically implemented as a simple loop that waits for input and fork()s a new child process to execute the command; the child process then exec()s the specified command while the parent process wait()s for the child to finish before continuing with the next iteration of the loop.
The shell you implement will be similar to, but much simpler, than the one you run every day in Unix. You can find out which shell you are running by typing echo $SHELL at a prompt. You may then wish to look at the man pages for sh or the shell you are running to learn more about all of the functionality that can be present. For this project, you do not need to implement as much functionality as is in most shells. 
Besides the most basic function of executing commands, your shell (mysh) must provide the following two features: interactive vs. batch mode and output redirection. If you are interested in Systems, we also encourage you to work on the aliasing feature. It is not mandatory to implement the aliasing feature.  

## Features
### 2.1) Modes: Interactive and Batch
Your shell can be run in two modes: interactive and batch.   The mode is determined when your shell is started.  If your shell is started with no arguments (i.e., ./mysh) , it will run in interactive mode; if your shell is given the name of a file (e.g., ./mysh batch-file), it runs in batch mode.
In interactive mode, you will display a prompt (the string mysh> , note the space AFTER the > character) to stdout) and the user of the shell will type in a command at the prompt.
In batch mode, your shell is started by specifying a batch file on its command line; the batch file contains the list of commands (each on its own line) that should be executed. In batch mode, you should not display a prompt. In batch mode, you should echo each line you read from the batch file back to the user (stdout) before executing it; this will help you when you debug your shells (and us when we test your shell -- most of which will be of batch mode). If the line is empty or only composed of whitespace, you should still echo it; if it is over the 512-character limit then echo at least the first 512 characters (but you may echo more if you want).
In both interactive and batch mode, your shell terminates when it sees the exit command on a line or reaches the end of the input stream (i.e., the end of the batch file or the user types 'Ctrl-D').  
### 2.2) Redirection
Many times, a shell user prefers to send the output of a program to a file rather than to the screen. Usually, a shell redirects standout output to a file with the > character; your shell should include this feature.

### 2.3) Aliases
Many shells also contain functionality for aliases.   To see the aliases that are currently active in your Linux shell, you can type alias.    Basically, an alias is just a short-cut so that the user can type in something simple and have something more complex (or more safe) be executed.  
For example, you could set up:
mysh> alias ls /bin/ls
so that within this shell session, the user can simply type ls and the executable /bin/ls will be run.
Note that alias is an example of a "built-in" command. A built-in command means that the shell interprets this command directly; the shell does not exec() the built-in command and run it as a separate process; instead, the built-in command impacts how the shell itself runs.  
There are three ways that alias can be invoked in your shell. 
If the user types the word alias, followed by a single word (the alias-name), followed by a replacement string(s), you should set up an alias between the alias-name and the value (e.g. alias ll /bin/ls -l -a). (Special cases: If the alias-name was already being used, just replace the old value with the new value.
If the user simply types alias, your shell should display all the aliases that have been set up with one per line (first the alias-name, then a single space, and then the corresponding replacement value, with each token separated by exactly one space).
If the user types alias followed by a word, if the word matches a current alias-name, print the alias-name and corresponding replacement value, with each token separated by exactly one space; if the word does not match a current alias-name, just continue.  
The user can also unalias alias-names; if the user types unalias <alias-name> you should remove the alias from your list. If <alias-name> does not exist as an alias, just continue. If the user does not specify <alias-name> or there are too many arguments to unalias then print unalias: Incorrect number of arguments.\n and continue.
You should be able to handle an arbitrary number of aliases.
You do not need to worry about aliases to other aliases, aliases that involve redirection, or redirection of aliases. There are only three words that cannot be used as alias-names: alias, unalias, and exit.   For example, if the user types alias alias some-string, alias unalias some-string, or alias exit some-string, your shell should print to stderr alias: Too dangerous to alias that.\n and continue.
To actually use an alias, the user types the alias just as they would type any other command:
  
mysh> alias ll /bin/ls -l
  
mysh> ll
  
Running an alias with additional arguments (e.g. ll -a where ll is an alias-name) is undefined behavior. We will not test this. All alias calls will consist of only the alias-name.

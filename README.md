# Unix shell like “bash” called “nush” 

## A shell operates as follows:
1. Accept an optional script file as the first command line argument.
2. If there’s no script, show a prompt. Your prompt should be “nush$ “. Command input is accepted on the same line after the prompt.
3. Read one line of commands from either stdin or the script.
4. Execute that line of commands.
5. Repeat until EOF or an “exit” command.

## Command lines are made up of:
1. Programs to execute.
2. Built-in commands.
3. Command line arguments to programs / builtins.
4. Operators.

## Programs to Execute
nat@masaru:hw05$ ./nush 
nush$ head -n 2 /usr/share/dict/words
A
A's
nush$ 

## Given a program to execute, like “head” above, the shell will do three things:
1. fork(), to create a new process.
2. in the child process, exec(), to execute the requested program, passing through the three command line arguments
3. in the parent process, wait() or waitpid(), to allow the program to finish before executing another command

## Built-inCommands
nush$ pwd
/home/nat/Teaching/3650/hw05
nush$ cd tests
nush$ pwd
/home/nat/Teaching/3650/hw05/tests
nush$ exit
nat@masaru:hw05$

## Some commands aren’t programs to execute, they’re things that the shell should handle itself. Implemented two of these:
1. The “cd” command changes the current directory. This means calling chdir() to change the state of your shell process.
2. The “exit” command exits the shell.

In bash, lots of commands that are available as programs are actually built-in for efficiency, like pwd, echo, etc. The shell should just run these as programs.

## Operators
nush$ sort tests/sample.txt | tail -n 3
remorselessly
revolutionized
underplays
nush$ 

Operators either modify the behavior of one command or chain together multiple commands into a single command line.

## The shell should support seven operators:
1. Redirect input: sort < foo.txt
2. Redirect output: sort foo.txt > output.txt
3. Pipe: sort foo.txt | uniq
4. Background: sleep 10 &
5. And: true && echo one
6. Or: true || echo one
7. Semicolon: echo one; echo two

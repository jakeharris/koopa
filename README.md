koopa
=====

A robust shell program for Intro to OS.

Authors: Jake Harris (javakatdesign@gmail.com), Clay Miller (clay@smockle.com), Patrick Fleming (jpf0005@auburn.edu).

Table of Contents:
=====
1. [Project Summary] [5]
2. [To-Do List] [6]
3. [Assignment Handout] [7]

[5]: #project-summary "Project Summary"
[6]: #to-do-list "To-Do List"
[7]: #assignment-handout "Assignment Handout"



Project Summary
=====
koopa is an exercise in fully understanding shell programs like /bin/bash. As such, it should be as robust and deep as possible, but still learnable and reproducible in less than a week by a newbie in OS development (especially since we only have that long to write it!).

To-Do List
=====
- Split string, delimited by spaces, into substrings, and store those in a data structure of a length determined at run-time
- Create a "job" data structure that runs one command and passes in all the relevant parameters
- Using the split string, create a series of jobs, connectable via the & operator.
- Grab the return value of a command and put it somewhere.
- Reorder jobs based on the location and implications of the piping and redirect operators.

Coding Standards
====
- Use two spaces instead of tabs.
- Use C-style comments (eg. /* */)
- Open curly braces do not get their own lines.

Assignment Handout
=====
The design spec from the handout:

**Objective**

Your assignment is to write a shell: a program that starts up other programs. Your shell, called robust-shell, will read input lines from standard input, parse them into a command name and arguments, and then start a new process running that command. When you start your shell, it will provide a prompt and then wait for a command line of the form:

    foobar -l this and that

This starts a program stored in a file called "foobar" with four arguments: "-l", "this", "and", and "that".

**Program Details**

Your shell will loop (until end-of-file) waiting for a line from standard input.

Each line has a command and zero or more arguments, and is of the form:

    cmd arg1 ... argk

Each of these items is separated by one or more blank characters (spaces or tabs). There is one command per input line. Your shell will [fork][1] a child process and then overlay itself ([exec][2]) the command in the file named by "cmd". The shell parent process will default to waiting for the child process. Optionally, at the end of any command input, can be a "&" character, which means that the parent process does NOT wait for the child process to complete before prompting the next command.

The command line is processed (by the shell) into a list of character strings, one for each argument (including the command name). These arguments are passed as parameters to the exec command (you'll probably want to use [execvp][3]).

Make sure that your program can handle input lines that have very long command names and arguments. You must also be able to deal gracefully with command lines that are arbitrarily long; you must be able to gracefully reject them.

You will want to make sure that you break your program up into modules, such that each module represents a sensible type abstraction.

[1]: http://linux.die.net/man/2/fork/		"fork"
[2]: http://linux.die.net/man/3/exec/		"exec"
[3]: http://linux.die.net/man/3/execvp/ "execvp"

**Testing Your Program**

First, test that your program can start up simple programs and pass parameters to them. Then test it by running standard Linux utilities like [ls][4].

Your program should not crash, no matter how weird the input. For example, suppose that the input has zero-value characters (bytes) or lines that have 1,000,000 characters? As a result, you may not be able to use the input routines that first occur to you.

To test the robustness of your program, you should run it against files that ocntain a lot of nonsense characters, so it will cause real programs not to be executed. But your program should not crash when processing them, and some sort of reasonable error message should be displayed.

A test list of all the commands and parameters will be given to you. Your submission must include the results of running these commands in your robust-shell.

[4]: http://linux.die.net/man/1/ls/		"ls"

**Deliverables**

You will work in groups of two or three and turn in a single copy of your program through Canvas submission, clearly labeled with the name and logins of all authors. You will turn in your programs, including all .c and .h files and your Makefile, and output (use script) from your program using the test list of all commands and parameters that will be supplied to you. Also include a README file indicating which lab machines you compiled and ran your program upon.

NOTE: You must eventually run your programs correctly on the tux Linux systems provided by the AU College of Engineering. You can access these machines from the Davis 123 lab or from anywhere on the Internet using the secureCRT remote login facility.

**Extra Credit**

In addition to the above work, your shell should also handle the standard input and output redirect on any command (the ">" and "<" operations).

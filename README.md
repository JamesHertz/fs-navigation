# FS-NAVEGATION
 I really like to use alias to move between my directory tree, but adding an alias isn't as comfortable as I wanted it to be. So I decided that would allow me to move between my directories fast and in a very confortable way. I also plan to add new functionalities to what a simple alias can do. 

## TODO:

- [X] write a simple deployment method as the one I am using now
- [X] write the other part of the installation and deployment method :)
- [X] add a remove command
- [ ] add that special inner dir record insert

# how to does it work?

It's very easy to use. After following the [instalation guide](#installing) you can run fs in you terminal and it will display the following menu:

```
fs-commands: 
    * set <name> <path> - set's <name> path to <path>
    * get <name>        - moves to the path of <name> if such exists
    * rm <name>         - removes entry which name is <name> if such exists
    * <name>            - alias for get <name>
    * list              - list all record entries stored
    * help              - prints help message
```

As you see there are 4 main commands:
- get \[record-name]
- set \[record-name] \[record-path]
- rm \[record-name]
- list 

Records can be tought of tuples that has a unique name and a path. When we run 

## get command
```
fs get <a-record-name>
``` 
It will move us to the path that is associated with such name or print an error.

We can run just :
```
fs <a-record-name>
```
Instead of fs get. It will work for all the record names that are not equal to one of the commands name.

## set command
To set a record or to change a path of a record, we run 
```
fs set <a-record-name> <a-record-path>
```
then **fs** (the name of the program c: ) will check if the path is is valid (exists and it's a directory). If it's not, it will report and an error and not perform the operation.

**Note:** the path given to fs can be relative to the current directory of a full path (starting with /).

## rm command
We can also remove records by running:
```
fs rm <record-name-1> <record-name-2> ... <record-name-n>
```

# Base file
The file where the records are stored by default it's ~/.fs-nav. But you can change that by setting the environment variable FS_BASE_FILE.

# Installing

To install this projects you will need to do alterations in the install.mk file. In the first lines of this file you will find this:
```
# TODO: change the two variables below for your situation
FS_BASE_DIR :=
RC_FILE     :=
```
They are two variables that you will need to set values for. The first one is the folder where you want to put the project files. The second one is the rc/profile file of your terminal (for bash it's ~/.bashrc and for zsh it's ~/.zprofile if yours is none of this just google it).

After doing such changes you should run the following command in the terminal:
```
make -f install.mk
```
**Warning:**  if you want to place the project a folder where you need special permitions to create folders you will need to use **sudo** before the command above. And if you are using linux you should not use ~ in the path name.

# Requirements

This tool is made of a script file and some C code. In order to install it you will need to have installed: *makefile* and *a C compiler*. And in order to run it I you need a shell interpreter like bash. (I used zsh develop and test this project)

It wasn't tested on windows so I offer no guarantees. But I can say that it works fine for unix based systems.
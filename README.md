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

**Note:** the path given to fs can be relative to the current directory of a full path (starting with /). If no path is given it will take the current directory as the record path.

## rm command
We can also remove records by running:
```
fs rm <record-name-1> <record-name-2> ... <record-name-n>
```

# Base file
The file where the records are stored by default it's ~/.fs-nav. But you can change that by setting the environment variable FS_BASE_FILE.

# Installing

This proccess was over simplified very recently.
Basically you will use the fs-install.sh script. The usage of it is the following:
```
usage: fs-install.sh <base-dir> <rc-file (optional)> "
```
Where base-dir is the directory where fs-navegation will be installed (I used /opt but you choose yours). This means will create a folder and inside this one it will have the files fs-navegation needs to run. And rc-file is the run command file or profile file or your shell. (eg. if it's bash the rc-file is ~/.bashrc if its zsh it's ~/.zprofile, any other just google c:)

If you don't specify the rc-file in the end will pop-up this message to you:
```
** commands for your rc-file **
You need to copy the following to your rc-file:

BASE_DIR=<a-dir-name-c:>
export FS_EXE=$BASE_DIR/bin/fs-exe
source $BASE_DIR/script.sh
```

You will need to paste the two last lines of it into your rc-file, otherwise fs-navegation won't work.

If we had this already working and the only thing you want to do is to update it. You only need to run:
```
./fs-install.sh
```
The only thing that needs to be done is to replace your older files with news. No change needs to be done in your rc-file so you should not specify it.

If you want to install it on a new directory just re-run the **usage** described above, and be sure to remove the lines fs-install.sh added (or that you added) to your rc-file, and of course delete the files in the previous folder.

# Requirements

This tool is made of a script file and some C code. In order to install it you will need to have installed: *makefile* and *a C compiler*. And in order to run it I you need a shell interpreter like bash. (I used zsh develop and test this project)

It wasn't tested on windows so I offer no guarantees. But I can say that it works fine for unix based systems.

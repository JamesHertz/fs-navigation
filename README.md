# FS-NAVIGATION 

I really like to use alias to move between my directory tree, but adding an alias isn't as comfortable as I wanted it to be. So I decided that would allow me to move between my directories fast and in a very confortable way. I also plan to add new functionalities to what a simple alias can do. 

## TODO: 
- [X] write a simple deployment method as the one I am using now 
- [X] write the other part of the installation and deployment method :) 
- [X] add a remove command 
- [ ] add that special inner dir record insert 
- [ ] think of a new way to store records 
    - Use protocol buffer make a bridge between C and C++ 
- [ ] Add a recursive set to a folder \<folder\> where it's sub folder are added with names \<folder\>/\<sub-folder\> 
- [ ] Increase the space locality of fs-navigation 


# IDEAS: 
- Turn this into a real alias. 
- Instead of records pointing to files they will now point to commands. 
- That can be one of three types: 
    - General pupose (just like alias) 
    - Directories (works just like the current version of fs) 
    - Files (opens them using the open command) 

# how to does it work? It's very easy to use. 

After following the [instalation guide](#installing) you can run fs in you terminal and it will display the following menu:
```
fs-commands:
    * set <name> [ <path> ] - set's <name> path to <path>
    * get <name>            - moves to the path of <name> if such exists
    * rm  <name>            - removes record which name is <name> if such exists
    * mv  <name> <new-name> - changed record name from <name> to <new-name>
    * <name>                - alias for get <name>
    * list                  - list all record entries stored
    * help                  - prints help message
```

As you see the main commands are:
- get \[record-name]
- set \[record-name] \[record-path]
- rm \[record-name]
- mv \[record-name] \[new-record-name]
- list 

Records are tuples (something that holds 2 values) in which the name is unique.

## get command
```
fs get <a-record-name>
``` 
It will move us to the path that is associated with such name or print an error.

We could just run :
```
fs <a-record-name>
```
instead of fs get, It will work for all the record names that are not equal to one of the commands name.

## set command
To set a record or to change a path of a record, we run 
```
fs set <a-record-name> <a-record-path>
```
then **fs** (the name of the program c: ) will check if the path is is valid (exists and it's a directory). If it's not, it will report and an error and not perform the operation.

**Note:** the path given to fs can be relative to the current directory of a full path (starting with /) or we could specify no path and it will take the current directory as the record path.

## rm command
We can also remove records by running:
```
fs rm <record-name-1> <record-name-2> ... <record-name-n>
```
If one of the \<record-name-i\> doest exists it will abort the operation and it won't save the changes to the disk which means none record will be removed.

## mv command
To change the name associated with a record path we can run:
```
fs mv <old-record-name> <new-record-name>
```
If \<new-record-name\> is an name of one record such record will be removed (this behaviour could be discussed on a issue).

# Base file
The file where the records are stored by default it's ~/.fs-nav. But you can change that by setting the environment variable FS_BASE_FILE.

# Installing

You can install by run the following command:
```
sudo ./fs-install.sh /opt
```
This will install fs-navigation in **/opt** folder, more about it below.

## Understand install script

The usage of the script is the following:
```
usage: fs-install.sh <base-dir> <rc-file (optional)> "
```
Where **base-dir** is the directory where fs-navigation will be installed (I used /opt but you choose yours). This means will create a folder and inside this one it will have the files fs-navigation needs to run. And **rc-file** is the run command file or profile file of your shell. (eg. if it's bash the rc-file is ~/.bashrc if its zsh it's ~/.zshrc, any other just google c:)

If you don't specify the rc-file in the end will pop-up this message to you:
```
** commands for your rc-file **
You need to copy the following to your rc-file:

BASE_DIR=<a-dir-name-c:>
export FS_EXE=$BASE_DIR/bin/fs-exe
source $BASE_DIR/script.sh
```

You will need to paste the two last lines of it into your rc-file, otherwise fs-navigation won't work.

If we had this already working and the only thing you want to do is to update it. You only need to run:
```
./fs-install.sh
```
The only thing that needs to be done is to replace your older files with news. No change needs to be done in your rc-file so you should not specify it.

If you want to install it on a new directory just re-run the **usage** described above, and be sure to remove the lines fs-install.sh added (or that you added) to your rc-file, and of course delete the files in the previous folder.

# Requirements

This tool is made of a script file and some C code. In order to install it you will need to have installed: *makefile* and *a C compiler* (I will eventually make the binaries avaible so there is no need to compile). And in order to run it I you need a shell interpreter like bash. (I used zsh develop and test this project)

It wasn't tested on windows so I offer no guarantees. But I can say that it works fine for unix based systems.

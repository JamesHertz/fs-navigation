#!/usr/bin/env bash

function install_fs(){
# TODO: check if I need super user to install the file
# TODO: if we are not in the file with the make file error
    dir=$( make install || echo 'null' )
    commands="" ...

    fs_dir=$FS_BASE_DIR/fs-nav
    exe_dir=$fs_dir/bin

    for dir in ("$fs_dir" "$exe_dir") ; do 
        # make the dir if needed
         [ -n $dir ] && ! [ -d $dir ] && mkdir $dir
    done

    

    if [ dir = 'null' ];
        echo "error"
        exit 1
    fi

    if [ -n $2 ] ; then
        echo $commands >> $RC_FILE
        echo "installed reopen your terminal and you can run the program"
    else
        echo $commands
        echo "You need to copy this to your rc-file"
        echo "Once done you restart your terminal and run the program"
    fi


}

FS_BASE_DIR=
RC_FILE=

MY_VARS=.myvars.sh
[[ -f $MY_VARS ]] && source $MY_VARS

if [ $# -gt 0 ]; then
    FS_BASE_DIR=$1
    [ $# -gt 1 ] && RC_FILE=$2
fi

# check for all the errors
if [ -z $FS_BASE_DIR ]; then
    echo "missing the base-dir (directory for installing the program)"
    echo "call: fs-install.sh <base-dir>"
    exit 0
fi

install_fs()

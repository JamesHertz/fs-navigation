#!/usr/bin/env bash

function add_dir(){
    local dir1=$1
    local dir2=$2

    if [[ $dir1 =~ ^/ ]]; then 
        echo "$dir1$dir2"
    else
        echo "$dir1/$dir2"
    fi
}

function get_full_name(){
    local dirname=$1
    if [[ -d $dirname ]] ; then
        local old=$PWD
        cd $dirname
        echo $PWD
        cd $old
    else
        echo "$dirname"
    fi
}

function install_fs(){
    # TODO: check if I need super user to install the file

    local output=$( make install 2> /dev/null || echo 'null' )

    if [ "$output" == 'null' ] ; then
        echo "Error running building project."
        echo "Be sure you are in the right directory"
        exit 1
    fi

    # get the source exe and script location
    local _values=($output)
    local src_script=${_values[0]}
    local src_exe=${_values[1]}

	# get the target dirs names
    local target_base_dir=$FS_BASE_DIR/fs-nav
    local target_bin_dir=$target_base_dir/bin

	local fs_exe=bin/fs-exe
	local fs_script=script.sh

    # local other="\n\n# added by fs-install script\n"
    local commands="FS_DIR=$taget_base_dir\nexport FS_EXE=\$FS_DIR/$fs_exe\nsource \$FS_DIR/$fs_script"

    cp $src_exe $FS_BASE_DIR/$fs_exe
    cp $src_script $FS_BASE_DIR/$fs_script


    # TODO: prenvent this thing from breaking
    ! [ -d  $target_base_dir ] && mkdir $target_base_dir
    ! [ -d  $target_bin_dir ] && mkdir $target_bin_dir

   if [[ -n $RC_FILE ]] ; then
       echo -e "\n\n#Added by fs-install\n$commands" >> $RC_FILE
       echo "fs-navegation installed!!"
       echo "Restart your terminal and you can run the program."
   else
      echo "You need to copy the followin to your rc-file:"
      echo -e "\n$commands\n"
      echo "Once done you can restart your terminal and run the program"
   fi
}

FS_BASE_DIR=
RC_FILE=

MY_VARS=#.myvars.sh
[[ -f $MY_VARS ]] && source $MY_VARS

if [ $# -gt 0 ] ; then
    FS_BASE_DIR=$(get_full_name $1)
    [ $# -gt 1 ] && RC_FILE=$2
fi

# change error message and change the plane where you will use the
# get_full_dir_name
if [ -z $FS_BASE_DIR ]; then
    echo "missing the base-dir (directory for installing the program)"
    echo "call: fs-install.sh <base-dir>"
    exit 1
elif ! [ -d $FS_BASE_DIR ] ; then
    echo "Error no such directory: $FS_BASE_DIR"
    exit 1
fi


echo "FS_BASE_DIR : $FS_BASE_DIR"
#install_fs

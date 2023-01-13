#!/usr/bin/env bash

function error(){
	echo "ERROR:"

	for msg in "$@" ; do
		echo -e " | $msg"
	done

	exit 1
}

# pre: the dirname ($1) should be a valid directory 
function get_full_name(){
    local dirname=$1
    local old=$PWD
    cd $dirname
    echo $PWD
    cd $old
}

function add_dir(){
    local dir1=$1 
    local dir2=$2

    if [[ $dir1 =~ /$ ]]; then 
        echo "$dir1$dir2"
    else
        echo "$dir1/$dir2"
    fi
}

# surpress the sterr msg for a command
function surpress(){
	$@ 2> /dev/null
}

function create_dirs(){
	for dir in "$@"; do
		if ! [ -d $dir ]; then
			mkdir -p "$dir"  || return 1
			echo "dir: $dir created!!"
		fi
	done
	return 0
}

function install_fs(){

	# 2> /dev/null -> for surpressing the stderr messages
    local output=$( make install 2&> 1)

    if [ -z "$output" ] ; then
        error "Unable to buildig the project. Be sure you are in the right directory" "> $output"
    fi

    # get the source exe and script location
    local _values=($output)
    local src_script=${_values[0]}
    local src_exe=${_values[1]}

	# get the target dirs names
    local target_base_dir=$(add_dir $FS_BASE_DIR $FS_DIR_NAME )
    local target_bin_dir=$target_base_dir/bin

	# the target name files
	local fs_exe=$target_base_dir/fs-exe
	local fs_script=$target_base_dir/script.sh

	if ! create_dirs $target_base_dir $target_bin_dir ; then

		error  "Unable to create one or both of the fs-navegation diretories:" \
			   "   -> $target_base_dir" \
			   "   -> $target_bin_dir" \
			   "you may need to use sudo or there's a file with the same name"
	fi


	echo "coping $src_exe to $fs_exe"
    cp $src_exe $fs_exe

	echo "coping $src_target to $fs_target"
    cp $src_script $fs_script

	echo "files copied!!"


	if [[ -z $FS_EXE ]]; then

		# get the full dir name
		target_base_dir=$(get_full_name $target_base_dir)

    	local commands=$( \
			echo "BASE_DIR=$( get_full_name $FS_BASE_DIR)";
			echo "export FS_EXE=\$BASE_DIR/$fs_exe";
			echo "source \$BASE_DIR/$fs_script"
		)

		if [[ -n $RC_FILE ]] ; then
			echo -e "\n\n#Added by fs-install\n$commands" >> $RC_FILE
			# TODO: adding to rc-file :)
			echo "Restart your terminal and you can run the program."
		else
			echo "You need to copy the following to your rc-file:"
			echo -e "\n$commands\n"
			echo "Once done you can restart your terminal and run the program"
		fi
	fi

	echo -e  "\n*** fs-navegation installed!! ***"
}


# var used to save error ouput
errno=
FS_BASE_DIR=
RC_FILE=

FS_DIR_NAME=fs-nav

MY_VARS=#.myvars.sh
[[ -f $MY_VARS ]] && source $MY_VARS

if [ $# -gt 0 ] ; then
    FS_BASE_DIR=$1
    [ $# -gt 1 ] && RC_FILE=$2 # TODO: check if we can write in the rc_file
fi

if [ -z $FS_BASE_DIR ]; then
# TODO: if FS_EXE is defined get the dir from there
# TODO: look at the message below
# change error message and change the plane where you will use the
    error \
	"You have to specify at least the base-dir (directory where to install the program)" \
    "Call this way: [ fs-install.sh <base-dir> <rc-file (optional)> ]"
elif ! [ -d $FS_BASE_DIR ] ; then
    error "no such directory: $FS_BASE_DIR"
fi


install_fs

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

    if [[ $dir1 =~ ^/ ]]; then 
        echo "$dir1$dir2"
    else
        echo "$dir1/$dir2"
    fi
}

function create_dirs(){
	for dir in "$@"; do
		if ! [ -d $dir ]; then
			mkdir -p "$dir" || return 1
			echo "dir: $dir created!!"
		fi
	done
	return 0
}

function install_fs(){

	# 2> /dev/null -> for surpressing the stderr messages
    local output=$( make install 2> /dev/null || echo 'null' )

    if [ "$output" == 'null' ] ; then
        error "running building project. Be sure you are in the right directory"
    fi

    # get the source exe and script location
    local _values=($output)
    local src_script=${_values[0]}
    local src_exe=${_values[1]}

	# the target name files
	local fs_exe=bin/fs-exe
	local fs_script=script.sh

	# get the target dirs names
    local target_base_dir=$( get_full_name $FS_BASE_DIR )/fs-nav
    local target_bin_dir=$target_base_dir/$(dirname $fs_exe) # just to get the bin :)

    # local other="\n\n# added by fs-install script\n"
    local commands=\
	"FS_DIR=$taget_base_dir\nexport FS_EXE=\$FS_DIR/$fs_exe\nsource \$FS_DIR/$fs_script"

	create_dirs $target_base_dir $target_bin_dir
#    ! [ -d  $target_base_dir ] && mkdir $target_base_dir
#    ! [ -d  $target_bin_dir ] && mkdir $target_bin_dir
#
	echo "err_code: $?"
	if ! [ $? -eq 0 ]; then

    	# TODO: better error messages
		error  "creating fs-navegation directories" \
			   "Wasn't able to create on or both of the diretores:" \
			   "   -> $target_base_dir" \
			   "   -> $target_bin_dir" \
			   "you may need to use sudo or there's a file with the same name"
	fi

    cp $src_exe $fs_base_dir/$fs_exe
    cp $src_script $fs_base_dir/$fs_script

	if [[ -z $FS_EXE ]]; then
		if [[ -n $RC_FILE ]] ; then
			echo -e "\n\n#Added by fs-install\n$commands" >> $RC_FILE
			echo "fs-navegation installed!!"
			echo "Restart your terminal and you can run the program."
		else
			echo "You need to copy the followin to your rc-file:"
			echo -e "\n$commands\n"
			echo "Once done you can restart your terminal and run the program"
		fi
	fi

	echo -e "\n-------------------------"
	echo    "fs navegation installed!!"
	echo    "-------------------------"
}

FS_BASE_DIR=
RC_FILE=

MY_VARS=#.myvars.sh
[[ -f $MY_VARS ]] && source $MY_VARS

if [ $# -gt 0 ] ; then
    FS_BASE_DIR=$1
    [ $# -gt 1 ] && RC_FILE=$2
fi

# change error message and change the plane where you will use the
# get_full_dir_name
if [ -z $FS_BASE_DIR ]; then
# TODO: if FS_EXE is defined get the dir from there
# TODO: look at the message below
    error \
	"You have to specify at least the base-dir (directory where to install the program)" \
    "Call this way: [ fs-install.sh <base-dir> <rc-file (optional)> ]"
elif ! [ -d $FS_BASE_DIR ] ; then
    error "no such directory: $FS_BASE_DIR"
fi


install_fs

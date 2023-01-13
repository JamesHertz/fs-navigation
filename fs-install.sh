#!/usr/bin/env bash

# colors taken from the net
red='\033[0;31m'
green='\033[0;32m'
yellow='\033[0;33m'
blue='\033[0;34m'
magenta='\033[0;35m'
cyan='\033[0;36m'
clear='\033[0m'

# background colors
bg_red='\033[0;41m'
bg_green='\033[0;42m'
bg_yellow='\033[0;43m'
bg_blue='\033[0;44m'
bg_magenta='\033[0;45m'
bg_cyan='\033[0;46m'

# important variables
FS_BASE_DIR=
FS_DIR_NAME=fs-nav
RC_FILE=
# var used to save error ouput
err=


function error(){
	echo -e "${red}ERROR:"

	for msg in "$@" ; do
		echo -e " | $msg"
	done

	if [ -n "$err" ]; then
		echo -ne $blue
		echo "COMANDLINE-ERROR: $err"
	fi


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

function create_dirs(){
	for dir in "$@"; do
		if ! [ -d $dir ]; then
			err=$( mkdir "$dir" 2>&1 ) || return 1
			echo "dir: $dir created!!"
		fi
	done
	return 0
}

function print_init_info(){
	echo "------------------------"
	echo -e "${cyan}** install info **${clear}"
	echo -e "${blue}base-dir:${clear} ${green}$FS_BASE_DIR${clear}"
	echo -ne "${blue}rc-file :${clear} "

	# TODO: look what if a RC_FILE is provided
	if [ -z $FS_EXE ] ; then
	   [  -n "$RC_FILE" ] && echo -e "${green}$RC_FILE$" || echo -e "${red}<none>"   
	   echo -e ${clear}
	else
		echo "---"
	fi
	echo -e "------------------------\n"
}

function install_fs(){

	# prints info about the base-dir and the rc-file
	print_init_info

	# TODO: check if the RC_FILE is valid and if we can write on it

    if ! err=$(make install 2>&1) ; then
        error "Unable to buildig the project. Be sure you are in the right directory" 
    fi

    # get the source exe and script location
    local _values=($err)
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

	
	echo -e "${cyan}** coping files **${clear}"

	echo -e "coping ${yellow}$src_exe${clear} to ${green}$fs_exe${clear}"
    cp $src_exe $fs_exe

	echo -e "coping ${yellow}$src_script${clear} to ${green}$fs_script${clear}"
    cp $src_script $fs_script

	echo -e "${green}*** files copied!! ***${clear}\n"


	if [[ -z $FS_EXE ]]; then

		# get the full dir name
		target_base_dir=$(get_full_name $target_base_dir)

    	local commands=$( \
			echo "BASE_DIR=$( get_full_name $FS_BASE_DIR)";
			echo "export FS_EXE=\$BASE_DIR/$fs_exe";
			echo "source \$BASE_DIR/$fs_script"
		)

		echo -e "${cyan}** commands for your rc-file **${clear}"

		if [[ -n $RC_FILE ]] ; then
			commands="\n\n#Added by fs-install\n$commands"
			echo -e "appending: ${bg_blue}$commands${clear}\n\nto your ${blue}rc-file${clear}"
			echo -e $commands >> $RC_FILE
			echo "Restart your terminal and you can run the program."
		else
			echo "You need to copy the following to your rc-file:"
			echo -e "${bg_cyan}\n$commands${clear}\n"
			echo "Once done you can restart your terminal and run the program"
		fi
		echo -e "${green}*** fs-navegation installed!! ***${clear}"

	else
		echo -e "${green}*** fs-navegation updated!! ***${clear}"
	fi

}


function main(){
	echo -e "${blue}--- fs-install running ---${clear}"

	if [ -n "$FS_EXE" ]; then
		local aux=$FS_EXE
		for ((i=0; i < 3; ++i)); do # going back three times :)
			aux=$(dirname $aux)
		done
		FS_BASE_DIR=$aux
	fi

	if [ $# -gt 0 ] ; then
		FS_BASE_DIR=$1
		[ $# -gt 1 ] && RC_FILE=$2 # TODO: check if we can write in the rc_file
	fi

	if [ -z "$FS_BASE_DIR" ]; then
	# TODO: if FS_EXE is defined get the dir from there
	# TODO: look at the message below
	# change error message and change the plane where you will use the
		error \
		"You have to specify at least the base-dir (directory where to install the program)" \
		"Call this way: [ fs-install.sh <base-dir> <rc-file (optional)> ]"
	elif ! [ -d "$FS_BASE_DIR" ] ; then
		error "no such directory: $FS_BASE_DIR"
	fi

	install_fs
}


main "$@"

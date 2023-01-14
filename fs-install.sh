#!/usr/bin/env bash

# colors taken from the net
red='\033[0;31m'
green='\033[0;32m'
yellow='\033[0;33m'
blue='\033[0;34m'
magenta='\033[0;35m'
cyan='\033[0;36m'
clear='\033[0m'

# background colors also taken from the net
bg_red='\033[0;41m'
bg_green='\033[0;42m'
bg_yellow='\033[0;43m'
bg_blue='\033[0;44m'
bg_magenta='\033[0;45m'
bg_cyan='\033[0;46m'

# important variables:
FS_DIR_NAME=fs-nav # name of the fs-navegation folder in the FS-BASE-DIR folder
# folder where fs-navegation will be instaled
FS_BASE_DIR=
# name of the terminal rc-file (run command file)
RC_FILE=
# var used to save error ouput, just like errno ;)
err=

# prints an error message and exits the program
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

# concatenates the name of a subdir to another dir
function concat_dir(){
    local dir1=$1 
    local dir2=$2

    if [[ $dir1 =~ /$ ]]; then 
        echo "$dir1$dir2"
    else
        echo "$dir1/$dir2"
    fi
}

# creates dirs passed as arguments if they don't exits
# returns: 1 if couldn't create one of the dirs or 0 if everying went well
function create_dirs(){
	for dir in "$@"; do
		if ! [ -d $dir ]; then
			err=$( mkdir "$dir" 2>&1 ) || return 1
			echo "dir: $dir created!!"
		fi
	done
	return 0
}

# prints information about the base-dir and rc-file
function print_init_info(){
	echo "------------------------"
	echo -e "${cyan}** install info **${clear}"
	echo -e "${blue}base-dir:${clear} ${green}$FS_BASE_DIR${clear}"
	echo -ne "${blue}rc-file :${clear} "

	if [ -z $RC_FILE ]; then
	   [ -n $FS_EXE ] && echo "---" || echo -e "${red}<none>" 
	else
		echo -e "${green}$RC_FILE"
	fi

	echo -e "${clear}------------------------\n"
}

# checks the script has wirte access in the diretory/file
# passed as argument
function check_write_acess(){
	local dir=$1	

	if ! [ -w "$dir" ]; then  
		error "Doesn't have write access to: $dir" \
              "You need to use ${blue}sudo${red} before the command."
	fi
}


# copies files to their respective place
# and reports an error whenever such happens
function install_fs(){


	# checks for write access to FS_BASE_DIR and RC_FILE
	check_write_acess "$FS_BASE_DIR"
	if [ -n "$RC_FILE" ] ; then
	   [ -d "$RC_FILE" ] && error "The rc-file should be a file not a directory"
		check_write_acess "$RC_FILE"
	fi

	# prints info about the base-dir and the rc-file
	print_init_info
	
    if ! err=$(make install 2>&1) ; then
        error "Unable to buildig the project. Be sure you are in the right directory" 
    fi

    # get the source exe and script location
    local _values=($err)
    local src_script=${_values[0]}
    local src_exe=${_values[1]}

	# get the target dirs names
    local target_base_dir=$(concat_dir $FS_BASE_DIR $FS_DIR_NAME )
    local target_bin_dir=$target_base_dir/bin

	# the target name files
	local fs_exe=$target_base_dir/fs-exe
	local fs_script=$target_base_dir/script.sh

	if ! create_dirs $target_base_dir $target_bin_dir ; then

		error  "Unable to create one or both of the fs-navegation diretories:" \
			   "   -> $target_base_dir" \
			   "   -> $target_bin_dir" \
			   "There might be some files with the same name as these dirs." 
			   
	fi

	
	echo -e "${cyan}** coping files **${clear}"

	echo -e "coping ${yellow}$src_exe${clear} to ${green}$fs_exe${clear}"
    cp $src_exe $fs_exe

	echo -e "coping ${yellow}$src_script${clear} to ${green}$fs_script${clear}"
    cp $src_script $fs_script

	echo -e "${green}*** files copied!! ***${clear}\n"
	
	if [[ -z "$FS_EXE" ]] || [[ -n "$RC_FILE" ]]; then

		# get the full dir name
		target_base_dir=$(get_full_name $target_base_dir)

		local commands=$( \
			echo -n "BASE_DIR=$( get_full_name $FS_BASE_DIR)\n";
			echo -n "export FS_EXE=\$BASE_DIR/$fs_exe\n";
			echo -n "source \$BASE_DIR/$fs_script\n"
		)	

		echo -e "${cyan}** commands for your rc-file **${clear}"

		if [[ -n "$RC_FILE" ]] ; then
			commands="\n#Added by fs-install\n$commands"
			echo -e "appending: \n${bg_blue}$commands${clear}\nto your ${blue}rc-file${clear}"
			echo -e "\n$commands" >> $RC_FILE
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

# TODO: change to we only pop up the RC-thing if:
# the folder where
# the fs-exe was not set
# the RC_FILE was specified

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
		[ $# -gt 1 ] && RC_FILE=$2
	fi

	if [ -z "$FS_BASE_DIR" ]; then
	# TODO: look at the message below
	# change error message and change the plane where you will use the
		error \
		"You have to specify at least the base-dir (directory to install the program)" \
		"usage: [ fs-install.sh <base-dir> <rc-file (optional)> ]"
	elif ! [ -d "$FS_BASE_DIR" ] ; then
		error "no such directory: $FS_BASE_DIR"
	fi

	install_fs
}


main "$@"

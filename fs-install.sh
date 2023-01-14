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
# name for FS_DIR_NAME folder which in turn will have fs-navegation files 
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

# prints information about the base-dir and rc-file
function print_init_info(){
	echo "------------------------"
	echo -e "${cyan}** install info **${clear}"
	echo -e "${blue}base-dir:${clear} ${green}$FS_BASE_DIR${clear}"
	echo -ne "${blue}rc-file :${clear} "

	if [ -z $RC_FILE ]; then
	   [ -z $FS_EXE ] && echo -e "${red}<none>" || echo "---" 
	else
		echo -e "${green}$RC_FILE"
	fi

	echo -e "${clear}------------------------"
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

	# used to copy files and display beautiful messages :)
	function copy(){
		local src=$1 dst=$2
		echo -e "coping ${yellow}$src${clear} to ${green}$dst${clear}"
		cp $src $dst
	}

	# checks for write access to FS_BASE_DIR and RC_FILE
	check_write_acess "$FS_BASE_DIR"
	if [ -n "$RC_FILE" ] ; then
	   [ -f "$RC_FILE" ] || error "Invalid rc-file." "It doesn't exist or it's a diretory"
	   check_write_acess "$RC_FILE"
	fi

	# prints info about the base-dir and the rc-file
	print_init_info

	# look at this later: https://www.redhat.com/sysadmin/arguments-options-bash-scripts 
	
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
	local fs_exe=bin/fs-exe 
	local fs_script=script.sh 

	echo -e "\n${cyan}** checking dirs **${clear}"

	for dir in  "$target_base_dir" "$target_bin_dir" ; do
		echo -ne "dir: ${blue}$dir${clear} - "
		if ! [ -d $dir ]; then
	   		if ! err=$( mkdir "$dir" 2>&1 ) ; then
				echo -e "${red}error"
				error "Could not create dir: $dir"
			fi
			echo -e "${cyan}created${clear}"
		else
			echo -e "${green}exits${clear}"
		fi
	done


	echo -e "\n${cyan}** coping files **${clear}"

	copy $src_exe $target_base_dir/$fs_exe 
	copy $src_script $target_base_dir/$fs_script

	echo -e "${green}*** files copied!! ***${clear}\n"
	
	if [[ -z "$FS_EXE" ]] || [[ -n "$RC_FILE" ]]; then

		# get the full dir name
		target_base_dir=$(get_full_name $target_base_dir)

		local commands=$( \
			echo -n "BASE_DIR=$( get_full_name $FS_BASE_DIR )/$FS_DIR_NAME\n";
			echo -n "export FS_EXE=\$BASE_DIR/$fs_exe\n";
			echo -n "source \$BASE_DIR/$fs_script"
		)	

		echo -e "${cyan}** commands for your rc-file **${clear}"

		if [[ -n "$RC_FILE" ]] ; then
			commands="\n# Added by fs-install\n$commands"
			echo -e "appending: \n${bg_blue}$commands${clear}\n\nto your ${blue}rc-file${clear}"
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
	# TODO: define /opt as the default FS_BASE_DIR
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

function fs(){

    if  [[ ! -x $FS_EXE ]]; then
        echo "FS_EXE not set properly exiting..."
        return 1
    fi


    case $1 in

        list|rm|set|help|'')
            $FS_EXE $@
        ;;

        get|*)

            local entry=$1 
            [ "$1" = get ] && entry=$2

            dir=$($FS_EXE get $entry)
            if [[ "$dir" =~ ^/ ]] ; then
                # TODO: if path is broken ask to delete
                cd $dir
            else
                echo $dir
                return 1
            fi

            
        ;;
    esac
}

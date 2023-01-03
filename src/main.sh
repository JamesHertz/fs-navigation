# TODO: change this :)
# FOR TESTING REASONS :)
# FS_DB=bin/main #/opt/fs-nav/bin/fs-db

FS_DB=/opt/fs-nav/bin/fs-db

function fs(){

    case $1 in
        ''|help)
            echo "fs-commands: " 
            echo "    * set <name> <path> - set's <name> path to <path>"
            echo "    * get <name>        - moves to the path of <name> if such exists"
            echo "    * rm <name>         - removes entry which name is <name> if such exists"
            echo "    * <name>            - alias for get <name>"
            echo "    * list              - list all record entries stored"
            echo "    * help              - prints help message"
        ;;

        list)
            $FS_DB list
        ;;

        set)
            if [ $# -lt 2 ]; then
                echo "invalid call to set missing: <name> <path>"
                return 1
            fi

            local path
            if [ $# -eq 2 ] ; then
                path=$PWD
            elif [[ $3 =~ ^/ ]]; then
                path=$3
            else
                path=$PWD/$3
            fi

            if ! [[ -e $path && -d $path ]]; then
                echo "invalid path: $path"
                return 1
            fi

            $FS_DB set $2 $path
        ;;

        rm)
           if [ $# -lt 2 ]; then
                echo "Invalid use of rm missing: <entry-name>"
                return 1
            fi

            $FS_DB $@
        ;;

        get|*)

            local entry=$1

            if [ $1 = get ]; then
                if [ $# -lt 2 ]; then
                    echo "Invalid use of get missing: <entry-name>"
                    return 1
                fi
                entry=$2
            fi

            dir=$($FS_DB get $entry)

            if [ $dir = null ]; then
                echo "no path found for entry: $entry"
                return 1
            fi

            cd $dir

        ;;
    esac
}
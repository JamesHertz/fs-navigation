export TEST_DIR=$PWD # for testing reasons :)
FS_DB=$TEST_DIR/bin/main

function fs(){

    case $1 in
        ''|help)
            echo "commands: " 
            echo "    -> set <entry-name> <path> - set's <entry-name> path to <path>"
            echo "    -> get <entry-name> - moves to the path of <entry-name> if such exists"
            echo "    -> help - prints help message"
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
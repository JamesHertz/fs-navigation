FS_DB=bin/main

function fs(){
    case $1 in
        set)
            if [ $# -lt 2 ]; then
                echo "invalid call to set: missing <name> <path>"
                return 1
            fi

            local path
            if [ $# -eq 2 ] ; then
                path=$PWD
            else
                path=$3
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
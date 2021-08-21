function o
    set argc (count $argv)
    if test $argc -eq 0
        cd $HOME
    else if test "$argv" = -
        popd
    else if begin
            test $argc -eq 1; and test -d $argv[1]
        end
        coxide - $argv[1]
        cd $argv[1]
    else
        set -l coxide_result (command coxide $argv[1])
        and cd $coxide_result
    end
end

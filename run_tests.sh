if [ $1 = "-u" ]; then
        echo "Usage $0 [DIR_PATH](example $0 tests/bad_files/conclusions/)" 
else
	if [ -d $1 ]; then
		echo "Running tests.... "
		for f in $(find $1 -name '*.txt'); do echo "\n\n\033[1;100mTEST file: $f\033[0m"; ./expertsystem $f; done
	else
		echo "$1 is not a direcory. Run with -u option to see usage"
	fi
fi

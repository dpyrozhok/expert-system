PATH=$1
substring_error="Error"
substring_succes="FINAL_STATUS"


if [ $1 = "-u" ]; then
	echo "Usage $0 [DIR_PATH](example $0 tests/bad_files/conclusions/)" 
else
echo -e "Running tests.... "
for file in $PATH/*.txt
do
	echo -e "\n\nTEST file: " $file
 	 ./expertsystem $file | { while read my_string; do 
	if [ "${my_string/$substring_error}" != "$my_string" ] ; then
	  echo "${my_string}"
  	elif [ "${my_string/$substring_succes}" != "$my_string" ] ; then
		echo "${my_string}"
	else
		echo $my_string
	fi
	done; }

done
fi


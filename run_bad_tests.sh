PATH=$1
substring_error="Error"
substring_succes="FINAL_STATUS"


if [ $1 = "-u" ]; then
	echo "Usage $0 [DIR_PATH](example $0 tests/bad_files/conclusions/)" 
else
echo -e "\e[5mRunning tests.... \e[25m"
for file in $PATH/*.txt
do
	echo -e "\n\n\e[7mTEST file: " $file "\e[27m"
 	 ./a.out $file | { while read my_string; do 
	if [ "${my_string/$substring_error}" != "$my_string" ] ; then
	  echo -e "\e[31m${my_string}\e[0m"
  	elif [ "${my_string/$substring_succes}" != "$my_string" ] ; then
		echo -e "\e[92m${my_string}\e[0m"
	else
		echo $my_string
	fi
	done; }

done
fi


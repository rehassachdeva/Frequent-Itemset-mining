infile=`awk 'BEGIN{FS=","} { confs[$1]=$2 } END { print confs["input"] }' config.csv`
outfile=`awk 'BEGIN{FS=","} { confs[$1]=$2 } END { print confs["output"] }' config.csv`

declare -A itemCodes
declare -A itemCodesReverse

eval "itemCodes=($(awk 'BEGIN{FS="," 
cnt=0}
 		{
			for(i=1;i<=NF;i++) {
     			if(itemCodes[$i]<1) {
        			cnt++
            		itemCodes[$i]=cnt
        		}
    		}
        }
	END {
		    for(item in itemCodes) 
		    	 print "[\"" item "\"]=" "\"" itemCodes[item]  "\""
		}' $infile) )"

eval "itemCodesReverse=($(awk 'BEGIN{FS="," 
cnt=0}
 		{
			for(i=1;i<=NF;i++) {
     			if(itemCodes[$i]<1) {
        			cnt++
            		itemCodes[$i]=cnt
        		}
    		}
        }
	END {
		    for(item in itemCodes) 
		    	 print "[\"" itemCodes[item] "\"]=" "\"" item  "\""
		}' $infile) )"

OIFS=IFS
IFS=","

while read line
do
	record=($line)
	len=${#record[@]}
	for((i=0;i<$len-1;i++))
	do
		printf '%s,' ${itemCodes[${record[$i]}]}
	done
	printf '%s\n' ${itemCodes[${record[$i]}]}
done < $infile > converted.csv

IFS=OIFS

./apriori

OIFS=IFS
IFS=","

while read line
do
	record=($line)
	len=${#record[@]}
	for((i=0;i<$len-1;i++))
	do
		if [ "${record[$i]}" != "=>" ] && [ "${record[0]}" != "#" ]
		then
			printf '%s,' ${itemCodesReverse[${record[$i]}]}
		elif [ "${record[0]}" != "#" ]
		then
			printf "=>,"
		else
			printf '%s\n' "${record[1]}"
		fi
	done
	if [ "${record[0]}" != "#" ]
	then
		printf '%s\n' ${itemCodesReverse[${record[$i]}]}
	fi
done < tempOutput.csv > $outfile

IFS=OIFS

rm -rf converted.csv
rm -rf tempOutput.csv
rm -rf tempRuleOutput.csv
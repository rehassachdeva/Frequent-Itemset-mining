declare -A dict

# dict+=`awk 'BEGIN{FS=","
# cnt=0
#                 }
#                 { for(i=1;i<=NF;i++) {
#                     if (itemCodes[$i]<1)
#                         {
#                             cnt++
#                             itemCodes[$i]=cnt
#                         }
#                     }

#                 }
#             END {
#             print "("
#             for (item in itemCodes) print "[\"" itemCodes[item] "\"=" "\""item "\"" "],"
#                 print ")"
#             }' $1`

#             awk 'BEGIN{FS=","
#             cnt=0
#         }
#         { for(i=1;i<=NF;i++) {
#             if (itemCodes[$i]<1)
#                 {
#                     cnt++
#                     itemCodes[$i]=cnt
#                 }
#                 printf("%s",itemCodes[$i]);
#                 if(i<NF)
#                     printf(",")
#                 else
#                     printf("\n") 
#                 }

#             }
#         END {}' $1>converted.csv

        outfile=`awk 'BEGIN{FS=","
        cnt=0
    }
    {
        confs[$1]=$2

    }
END { print confs["output"]}' "config.csv"`

dict+=(["1"]="cat" ["2"]="dog")

echo $dict
# awk -v dictt="$dict" 'BEGIN{FS=","
# cnt=0
#                 }
#                 {
#                     if($1=="#")
#                         print $2
#                     else {
#                         for(i=1;i<=NF;i++) {
#                             if($i=="=>")
#                                 printf("=>");
#                             else
#                                 printf("%s",dictt[$i]);
#                             if(i<NF)
#                                 printf(",")
#                             else
#                                 printf("\n") 
#                             }

#                         }

#                     }
#                 END { print confs["output"]}' "temp_output.csv" > out222



#! /bin/sh

counter=1
while IFS= read -r line; do
    substring=$(echo $line | cut -c1-4)
    if [ "$substring" = "<td>" ]; then
        if [ $(( $counter & 1 )) -eq 1 ]; then
            word=$(echo "$line" | sed -e 's/<[^>]*>//g' | tr "\`" "\'" | tr -d '?.' | tr -s " " "\n" | tr -d "\-\n" | tr '[:upper:]' '[:lower:]')
            if [ ! -z "$word" ]; then
                echo "$word"
            fi
        fi
        counter=$((counter+1))
    fi
done < "$1"
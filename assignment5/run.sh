#gcc -std=c11 tr2b.c -o tr2b
#./tr2b "abc" "xyz" < bigfile.txt
#gcc -std=c11 tr2u.c -o tr2u
#./tr2u "abc" "xyz" < bigfile.txt

gcc sfrobu.c -o sfrobu -fsanitize=address -fsanitize=undefined
printf '~BO {_CIA FKPS NEM' | ./sfrobu -f | od -ta
echo "--------------"
cat source/main.cpp |   base64 |   base64 -D | tail -n7
echo "--------------"
cat source/main.cpp |   base64 | ./base64 -D | tail -n7
echo "--------------"
cat source/main.cpp | ./base64 |   base64 -D | tail -n7
echo "--------------"
cat source/main.cpp | ./base64 | ./base64 -D | tail -n7
echo "--------------"

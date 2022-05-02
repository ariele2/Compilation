#Run from this folder
declare -a input_files=("t1.in" "t2.in" "t03-bare-block.in" "t04-calls.in" "t05-while.in" "t06-print.in" "t07-empty.in" 
"t08-function-signatures.in" "t09-stress-math.in" "t10-bytes.in" "t11-while.in" "t12-const.in")

echo "Full Segel tests:"

for i in {1..12}
do
../../hw2 < ${input_files[$i-1]} > my_t$i.out
diff <(sed 's/^[0-9]\+/0/g;/TypeAnnotation -> epsilon/d;s/TypeAnnotation Type/Type/g' my_t$i.out) <(sed 's/^[0-9]\+/0/g;/TypeAnnotation -> epsilon/d;s/TypeAnnotation Type/Type/g' ${input_files[$i-1]}.res) &> /dev/null
if [ $? -ne 0 ]; then
	echo "[FAILED] - Test $i"
else
	echo "[OK]  - Test $i"
fi
done

# echo "Test 1:"
# ../../hw2 < t1.in > my_t1.out
# diff my_t1.out t1.in.res

# echo "Test 2:"
# ../../hw2 < t2.in > my_t2.out
# diff my_t2.out t2.in.res

# echo "Test 3:"
# ../../hw2 < t03-bare-block.in > my_t3.out
# diff my_t3.out t03-bare-block.in.res

# echo "Test 4:"
# ../../hw2 < t04-calls.in > my_t4.out
# diff my_t4.out t04-calls.in.res

# echo "Test 5:"
# ../../hw2 < t05-while.in > my_t5.out
# diff my_t5.out t05-while.in.res

# echo "Test 6:"
# ../../hw2 < t06-print.in > my_t6.out
# diff my_t6.out t06-print.in.res

# echo "Test 7:"
# ../../hw2 < t07-empty.in > my_t7.out
# diff my_t7.out t07-empty.in.res

# echo "Test 8:"
# ../../hw2 < t08-function-signatures.in > my_t8.out
# diff my_t8.out t08-function-signatures.in.res

# echo "Test 9:"
# ../../hw2 < t09-stress-math.in > my_t9.out
# diff my_t9.out t09-stress-math.in.res

# echo "Test 10:"
# ../../hw2 < t10-bytes.in > my_t10.out
# diff my_t10.out t10-bytes.in.res

# echo "Test 11:"
# ../../hw2 < t11-while.in > my_t11.out
# diff my_t11.out t11-while.in.res

# echo "Test 12:"
# ../../hw2 < t12-const.in > my_t12.out
# diff my_t12.out t12-const.in.res
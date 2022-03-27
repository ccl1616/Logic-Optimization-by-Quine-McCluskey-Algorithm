case=$1

./convert_2_blif $case.in
./convert_2_blif $case.out
./eq_check $case.in.blif $case.out.blif $case.out

rm *.blif
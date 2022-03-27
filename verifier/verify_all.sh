for i in {0..7}
do
    case=case0$i
    echo -n "case0$i   |   "

    ./convert_2_blif $case.in
    ./convert_2_blif $case.out
    ./eq_check $case.in.blif $case.out.blif $case.out

done

rm *.blif
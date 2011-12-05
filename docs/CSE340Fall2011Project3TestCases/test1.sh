#/bin/bash
gcc -o a.o syntax.h main.h  main.c

./a.o <./tests/test_assignment_basic1.txt

echo "that was test_assignment_basic1.txt"
echo "###########################################################"

./a.o <./tests/test_assignment_basic2.txt

echo "test_assignment_basic2.txt"
echo "###########################################################"

./a.o <./tests/test_assignment_variables1.txt
echo "test_assignment_variables1.txt"

 echo "###########################################################"

./a.o <./tests/test_assignment_variables2_operators.txt
echo "test_assignment_variables2_operators.txt"

 echo "###########################################################"
 
./a.o <./tests/test_assignment_variables_no_init.txt
echo "test_assignment_variables_no_init.txt"
 
 echo "###########################################################"
  
./a.o <./tests/test_control_i_if_f1.txt
echo "test_control_i_if_f1.txt"
 
 echo "###########################################################"
  
./a.o <./tests/test_control_i_if_f2.txt
echo "test_control_i_if_f2.txt"

echo "###########################################################"
  
./a.o <./tests/test_control_i_if_f3.txt
echo "test_control_i_if_f3.txt"

echo "###########################################################"
  
./a.o <./tests/test_control_i_if_f4.txt
echo "test_control_i_if_f4.txt"

echo "###########################################################"
  
./a.o <./tests/test_control_i_if_f5.txt
echo "test_control_i_if_f5.txt"

echo "###########################################################"

./a.o <./tests/test_control_i_if_f_i_if_f1.txt
echo "test_control_i_if_f5.txt"

echo "###########################################################"

./a.o <./tests/test_control_i_if_f_i_if_f2.txt

echo "test_control_i_if_f_i_if_f2.txt
"

echo "###########################################################"

./a.o <./tests/test_control_if1.txt

echo "test_control_if1.txt
"

echo "###########################################################"


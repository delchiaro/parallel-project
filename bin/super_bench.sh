cd release-O1
echo
echo OOP-release-O1
./ObjectOriented imgBig.jpg -pg 200 -pr 4 -l 5 -b 30   -sd 3 -sd 7 -sd 11 -sd 19   -t 1 -t 2 -t 4 -t 8 -t 16 -t 32  -ctn OOP.csv
echo Struct-release
echo
./Structured     imgBig.jpg -pg 10 -pr 4 -l 5 -b 30   -sd 3 -sd 7 -sd 11 -sd 19  -t 1 -t 2 -t 4 -t 8 -t 16 -t 32  -ctn STRUCT.csv


#cd release
#echo
#echo OOP-release
#./ObjectOriented imgBig.jpg -pg 200 -pr 5 -l 5 -b 50   -sd 3 -sd 7 -sd 11 -sd 19   -t 1 -t 2 -t 4 -t 8 -t 16 -t 32  -ctn OOP.csv
#echo Struct-release
#echo
#./Structured     imgBig.jpg -pg 10 -pr 5 -l 5 -b 50   -sd 3 -sd 7 -sd 11 -sd 19  -t 1 -t 2 -t 4 -t 8 -t 16 -t 32  -ctn STRUCT.csv


#cd ../debug-O1
#echo
#echo OOP-debug-O1
#./ObjectOriented imgBig.jpg -pg 10 -pr 5 -l 5 -b 50   -sd 3 -sd 7 -sd 11 -sd 19   -t 1 -t 2 -t 4 -t 8 -t 16 -t 32  -ctn OOP.csv
#echo Sruct-debug01
#echo
#./Structured     imgBig.jpg -pg 10 -pr 5 -l 5 -b 50   -sd 3 -sd 7 -sd 11 -sd 19  -t 1 -t 2 -t 4 -t 8 -t 16 -t 32  -ctn STRUCT.csv



#cd ../debug-O0
#cd debug-O0
#echo
#echo OOP-debug-O0
#./ObjectOriented imgBig.jpg -pg 10 -pr 5 -l 2 -b 30   -sd 3   -t 32 -t 16 -t 8 -t 4 -t 2 -t 1  -ctn OOP.csv
#./ObjectOriented imgBig.jpg -pg 5  -pr 3 -l 2 -b 20   -sd 7   -t 32 -t 16 -t 8 -t 4 -t 2 -t 1  -cta OOP.csv
#./ObjectOriented imgBig.jpg -pg 3  -pr 1 -l 1 -b 15   -sd 11  -t 32 -t 16 -t 8 -t 4 -t 2 -t 1  -cta OOP.csv
#./ObjectOriented imgBig.jpg -pg 1  -pr 1 -l 1 -b 7    -sd 19  -t 32 -t 16 -t 8 -t 4 -t 2 -t 1  -cta OOP.csv

#echo
#echo Sruct-debug00
#./Structured imgBig.jpg -pg 10 -pr 5 -l 2 -b 30   -sd 3   -t 32 -t 16 -t 8 -t 4 -t 2 -t 1  -ctn STRUCT.csv
#./Structured imgBig.jpg -pg 5  -pr 3 -l 2 -b 20   -sd 7   -t 32 -t 16 -t 8 -t 4 -t 2 -t 1  -cta STRUCT.csv
#./Structured imgBig.jpg -pg 3  -pr 1 -l 1 -b 15   -sd 11  -t 32 -t 16 -t 8 -t 4 -t 2 -t 1  -cta STRUCT.csv
#./Structured imgBig.jpg -pg 1  -pr 1 -l 1 -b 7    -sd 19  -t 32 -t 16 -t 8 -t 4 -t 2 -t 1  -cta STRUCT.csv


cd debug-O0
./ObjectOriented imgBig.jpg -pg 10 -pr 5 -l 3 -b 50   -sd 3 -sd 7 -sd 11   -t 1 -t 2 -t 4 -t 8 -t 16 -t 32  -ctn OOP.csv
./Structured     imgBig.jpg -pg 10 -pr 5 -l 3 -b 50   -sd 3 -sd 7 -sd 11   -t 1 -t 2 -t 4 -t 8 -t 16 -t 32  -ctn STRUCT.csv

cd ../debug-O1
./ObjectOriented imgBig.jpg -pg 10 -pr 5 -l 3 -b 50   -sd 3 -sd 7 -sd 11   -t 1 -t 2 -t 4 -t 8 -t 16 -t 32  -ctn OOP.csv
./Structured     imgBig.jpg -pg 10 -pr 5 -l 3 -b 50   -sd 3 -sd 7 -sd 11   -t 1 -t 2 -t 4 -t 8 -t 16 -t 32  -ctn STRUCT.csv



cd ../debug-O0
./ObjectOriented-forceInline          imgBig.jpg -pg 10 -pr 5 -l 3 -b 50   -sd 3 -sd 7 -sd 11   -t 1 -t 2 -t 4 -t 8 -t 16 -t 32  -ctn forceInline.csv
./ObjectOriented-forceInlineDisabled  imgBig.jpg -pg 10 -pr 5 -l 3 -b 50   -sd 3 -sd 7 -sd 11   -t 1 -t 2 -t 4 -t 8 -t 16 -t 32  -ctn forceInlineDisabled.csv
./ObjectOriented-inlineDisabled       imgBig.jpg -pg 10 -pr 5 -l 3 -b 50   -sd 3 -sd 7 -sd 11   -t 1 -t 2 -t 4 -t 8 -t 16 -t 32  -ctn inlineDisabled.csv

cd ../debug-O1
./ObjectOriented-forceInline          imgBig.jpg -pg 10 -pr 5 -l 3 -b 50   -sd 3 -sd 7 -sd 11   -t 1 -t 2 -t 4 -t 8 -t 16 -t 32  -ctn forceInline.csv
./ObjectOriented-forceInlineDisabled  imgBig.jpg -pg 10 -pr 5 -l 3 -b 50   -sd 3 -sd 7 -sd 11   -t 1 -t 2 -t 4 -t 8 -t 16 -t 32  -ctn forceInlineDisabled.csv
./ObjectOriented-inlineDisabled       imgBig.jpg -pg 10 -pr 5 -l 3 -b 50   -sd 3 -sd 7 -sd 11   -t 1 -t 2 -t 4 -t 8 -t 16 -t 32  -ctn inlineDisabled.csv

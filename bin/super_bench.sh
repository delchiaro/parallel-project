
thread_list="-t 1 -t 2 -t 4 -t 8 -t 16 -t 32 -t 64 -t 128 -t 256 -t 512 -t 1024 -t 2048 -t 4096"



cd release-O1
echo "~ ~ ~ RELEASE -O1 ~ ~ ~"    
    
    echo
    echo
    exe="ObjectOriented"
    echo $exe
    ./$exe imgBig.jpg -prg 5 -pr 5   -l 20 -b 50   -se 3   $thread_list  -csv-n $exe.csv -pa -sm 1000  -v
    ./$exe imgBig.jpg -prg 5   -pr 2   -l 5  -b 50   -se 7   $thread_list  -csv-a $exe.csv -pa -sm 1000  -v
    ./$exe imgBig.jpg -prg 5   -pr 1   -l 2  -b 50   -se 11  $thread_list  -csv-a $exe.csv -pa -sm 1000  -v
    ./$exe imgBig.jpg -prg 5   -pr 1   -l 1  -b 50   -se 19  $thread_list  -csv-a $exe.csv -pa -sm 1000  -v


    echo
    echo
    exe="Structured"
    echo $exe
    ./$exe imgBig.jpg -prg 200 -pr 5   -l 20 -b 50   -se 3   $thread_list  -csv-n $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 2   -l 5  -b 50   -se 7   $thread_list  -csv-a $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 1   -l 2  -b 50   -se 11  $thread_list  -csv-a $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 1   -l 1  -b 50   -se 19  $thread_list  -csv-a $exe.csv -pa -sm 1000

cd ..
cd release-O2
echo "~ ~ ~ RELEASE -O2 ~ ~ ~"    
    
    echo
    echo
    exe="ObjectOriented"
    echo $exe
    ./$exe imgBig.jpg -prg 200 -pr 5   -l 20 -b 50   -se 3   $thread_list  -csv-n $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 2   -l 5  -b 50   -se 7   $thread_list  -csv-a $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 1   -l 2  -b 50   -se 11  $thread_list  -csv-a $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 1   -l 1  -b 50   -se 19  $thread_list  -csv-a $exe.csv -pa -sm 1000


    echo
    echo
    exe="Structured"
    echo $exe
    ./$exe imgBig.jpg -prg 200 -pr 5   -l 20 -b 50   -se 3   $thread_list  -csv-n $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 2   -l 5  -b 50   -se 7   $thread_list  -csv-a $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 1   -l 2  -b 50   -se 11  $thread_list  -csv-a $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 1   -l 1  -b 50   -se 19  $thread_list  -csv-a $exe.csv -pa -sm 1000


    
    
    
    
    
cd ..    
cd debug-O0
echo "~ ~ ~ DEBUG -O0 ~ ~ ~"    
    
    echo
    echo
    exe="ObjectOriented"
    echo $exe
    ./$exe imgBig.jpg -prg 200 -pr 5   -l 20 -b 50   -se 3   $thread_list  -csv-n $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 2   -l 5  -b 50   -se 7   $thread_list  -csv-a $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 1   -l 2  -b 50   -se 11  $thread_list  -csv-a $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 1   -l 1  -b 50   -se 19  $thread_list  -csv-a $exe.csv -pa -sm 1000


    echo
    echo
    exe="Structured"
    echo $exe
    ./$exe imgBig.jpg -prg 200 -pr 5   -l 20 -b 50   -se 3   $thread_list  -csv-n $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 2   -l 5  -b 50   -se 7   $thread_list  -csv-a $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 1   -l 2  -b 50   -se 11  $thread_list  -csv-a $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 1   -l 1  -b 50   -se 19  $thread_list  -csv-a $exe.csv -pa -sm 1000

cd ..    
cd debug-O1
echo "~ ~ ~ DEBUG -O1 ~ ~ ~"    
    
    echo
    echo
    exe="ObjectOriented"
    echo $exe
    ./$exe imgBig.jpg -prg 200 -pr 5   -l 20 -b 50   -se 3   $thread_list  -csv-n $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 2   -l 5  -b 50   -se 7   $thread_list  -csv-a $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 1   -l 2  -b 50   -se 11  $thread_list  -csv-a $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 1   -l 1  -b 50   -se 19  $thread_list  -csv-a $exe.csv -pa -sm 1000


    echo
    echo
    exe="Structured"
    echo $exe
    ./$exe imgBig.jpg -prg 200 -pr 5   -l 20 -b 50   -se 3   $thread_list  -csv-n $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 2   -l 5  -b 50   -se 7   $thread_list  -csv-a $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 1   -l 2  -b 50   -se 11  $thread_list  -csv-a $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 1   -l 1  -b 50   -se 19  $thread_list  -csv-a $exe.csv -pa -sm 1000

    
    
cd ..    
cd debug-O2
echo "~ ~ ~ DEBUG -O2 ~ ~ ~"    
    
    echo
    echo
    exe="ObjectOriented"
    echo $exe
    ./$exe imgBig.jpg -prg 200 -pr 5   -l 20 -b 50   -se 3   $thread_list  -csv-n $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 2   -l 5  -b 50   -se 7   $thread_list  -csv-a $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 1   -l 2  -b 50   -se 11  $thread_list  -csv-a $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 1   -l 1  -b 50   -se 19  $thread_list  -csv-a $exe.csv -pa -sm 1000


    echo
    echo
    exe="Structured"
    echo $exe
    ./$exe imgBig.jpg -prg 200 -pr 5   -l 20 -b 50   -se 3   $thread_list  -csv-n $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 2   -l 5  -b 50   -se 7   $thread_list  -csv-a $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 1   -l 2  -b 50   -se 11  $thread_list  -csv-a $exe.csv -pa -sm 1000
    ./$exe imgBig.jpg -prg 5   -pr 1   -l 1  -b 50   -se 19  $thread_list  -csv-a $exe.csv -pa -sm 1000




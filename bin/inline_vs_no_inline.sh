cd debug-O0
./ObjectOriented-forceInline          imgBig.jpg  -i 100  -t 4 -t 8 -t 16 -t 32  -ctn forceInline.csv
./ObjectOriented-forceInlineDisabled  imgBig.jpg  -i 100  -t 4 -t 8 -t 16 -t 32  -ctn forceInlineDisabled.csv
./ObjectOriented-inlineDisabled       imgBig.jpg  -i 100  -t 4 -t 8 -t 16 -t 32  -ctn inlineDisabled.csv

cd ../debug-O1
./ObjectOriented-forceInline          imgBig.jpg  -i 100  -t 4 -t 8 -t 16 -t 32  -ctn forceInline.csv
./ObjectOriented-forceInlineDisabled  imgBig.jpg  -i 100  -t 4 -t 8 -t 16 -t 32  -ctn forceInlineDisabled.csv
./ObjectOriented-inlineDisabled       imgBig.jpg  -i 100  -t 4 -t 8 -t 16 -t 32  -ctn inlineDisabled.csv

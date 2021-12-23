echo "[JPEG] making subdirs"
mkdir -p build/jpeg
cd build
echo "[JPEG] configuring..."
../configure --enable-shared=no
echo "[JPEG] buidling..."
make -sj4
echo "[JPEG] copy headers..."
cp ../*.h jpeg 
cp ./*.h  jpeg 
echo "[JPEG] copy sources..."
for f in `ar -t .libs/libjpeg.a | grep ^j`;
do
	#echo $f;
	s=`basename $f .o`.c;
	echo $s;
	cp ../$s jpeg 
done
rm -f    jpeg.tgz
tar cfvz jpeg.tgz jpeg



echo "[PNG] making subdirs"
mkdir -p build/png
cd build
echo "[PNG] configuring..."
../configure --enable-shared=no --enable-hardware-optimizations=no --with-binconfigs=no
echo "[PNG] buidling..."
make -sj4
echo "[PNG] copy headers..."
cp ../*.h png 
cp ./*.h png 
echo "[PNG] copy sources..."
for f in `ar -t .libs/libpng16.a | grep png`;
do
	#echo $f;
	s=`basename $f .o`.c;
	echo $s;
	cp ../$s png 
done
rm -f png.tgz
tar cfvz png.tgz png



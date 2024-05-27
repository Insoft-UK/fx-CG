# Update
FXCGSDK=/Applications/CASIO/SDK
PATH=$PATH:$FXCGSDK/bin

echo "\033[91;1;1;1mUpdating...\033[0m"

echo "Compiling...\033[92;1;1;1m fx-CG Library\033[0m"
cd libfxcg
make -j$(nproc)
make install
mv lib/*.a $FXCGSDK/lib/
mv -a toolchain $FXCGSDK/
mv -a include $FXCGSDK/
cd ..

echo "\033[92;1;1;1mDONE!\033[0m"

# Installer
FXCGSDK=/Applications/PrizmSDK
PATH=$PATH:$FXCGSDK/bin

echo "\033[91;1;1;1mInstalling...\033[0m"

echo "\033[93;1;1;1mGetting the Needed Packages.\033[0m"
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
brew install mpfr
brew install mpc
brew install gmp
brew install libpng
brew install ppl
brew install texinfo
brew install cmake
brew install imagemagick

echo "\033[93;1;1;1mSetting up the Build Environment.\033[0m"
PREFIX := $FXCGSDK/bin/sh3eb-elf-

curl -O https://ftp.gnu.org/gnu/gcc/gcc-10.1.0/gcc-10.1.0.tar.gz
tar -xzvf gcc*.tar.gz

echo "Compiling...\033[92;1;1;1m binutils\033[0m"
mkdir ~binutils
cd ~binutils
../binutils*/./configure --target=sh3eb-elf --prefix=$FXCGSDK --disable-nls
make -j$(nproc)
make install
cd ..

echo "Compiling...\033[92;1;1;1m GCC\033[0m"
mkdir ~gcc
cd ~gcc
../gcc*/./configure --target=sh3eb-elf --prefix=$FXCGSDK --disable-nls --enable-languages=c,c++ --without-headers
make -j$(nproc) all-gcc
make install-gcc
make -j$(nproc) all-target-libgcc
make install-target-libgcc
cd ..

echo "Compiling...\033[92;1;1;1m mkg3a\033[0m"
cd mkg3a
cmake .
make
sudo make install
sudo mv /usr/local/share/man/man1/mkg3a.1.gz $FXCGSDK/share/man/man1/
gzip -dv $FXCGSDK/share/man/man1/mkg3a.1.gz
sudo mv /usr/local/bin/mkg3a $FXCGSDK/bin/
sudo mv /usr/local/bin/g3a-updateicon $FXCGSDK/bin/
cd ..

echo "Compiling...\033[92;1;1;1m fx-CG Library\033[0m"
cd libfxcg
make -j$(nproc)
make install
cp lib/*.a $FXCGSDK/lib/
cp -a toolchain $FXCGSDK/
cp -a include $FXCGSDK/
cd ..

echo "\033[92;1;1;1mDONE!\033[0m"

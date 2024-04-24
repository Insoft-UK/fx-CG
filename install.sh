# Install fx-CG SDK
BASEDIR=$(dirname "$0")
cd "$BASEDIR"

cp -rf fx-CG ~/
cp -rf SDK /Applications/CASIO/
cd /Applications/CASIO/SDK/Installer
./install.sh
cd ..
rm -rf Installer

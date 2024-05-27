# Install fx-CG SDK
BASEDIR=$(dirname "$0")
cd "$BASEDIR"

cp -rf Prizm ~/
mkdir /Applications/CASIO/SDK
cp -rf SDK/Installer /Applications/CASIO/SDK
cd /Applications/CASIO/SDK/Installer
./install.sh
cd ..
rm -rf Installer

# Install fx-CG SDK
BASEDIR=$(dirname "$0")
cd "$BASEDIR"

cp -rf fx-CG ~/
cp -rf Prizm /Applications/
cd /Applications/Prizm/Installer
./install.sh
cd ..
rm -rf Installer

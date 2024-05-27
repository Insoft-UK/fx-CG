# Update fx-CG SDK
BASEDIR=$(dirname "$0")
cd "$BASEDIR"

cp -rf SDK/Update /Applications/CASIO/SDK
cp -rf SDK/Installer/libfxcg /Applications/CASIO/SDK/Update
cd /Applications/CASIO/SDK/Update
./update.sh
cd ..
rm -rf Update

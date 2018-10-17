echo "hello"
BASEDIR=$(dirname "$0")
echo $BASEDIR

pushd $BASEDIR

pushd libraries/libBlob
./build.sh "$@"
popd


pushd applications/blobUI
./build.sh "$@"
popd

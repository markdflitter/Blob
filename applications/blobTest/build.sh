if [ ! -d ../../../bld ]; then
  mkdir ../../../bld
fi

if [ ! -d ../../../bld/applications ]; then
  mkdir ../../../bld/applications
fi

if [ ! -d ../../../bld/applications/blobTest ]; then
  mkdir ../../../bld/applications/blobTest
fi

pushd ../../../bld/applications/blobTest

cmake ../../../src/applications/blobTest
make

popd

if [ -f ./blobTest.app ]; then
  rm -rf ./blobTest.app
fi

cp ../../../bld/applications/blobTest/blobTest.app .

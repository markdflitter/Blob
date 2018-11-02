printUsage ()
{
  echo "./build.sh [--clean] [-d | --debug] [--deep-clean] [-t | --test] [-T | --test-only] [-F | -test-filter <test to run>] [-h | --help]"
}

projectType=applications
project=blobUI
copyToSrc=1

clean=
debug=
deepClean=0
test=
testOnly=
testFilter=

while [ "$1" != "" ]; do 
	case $1 in
		--clean)		clean=1	;;
		--deep-clean)		deepClean=1 ;;
		-d|--debug)		debug=1 ;;
		-t|--test)		test=1	;;
		-T|--test-only)		testOnly=1 ;;
		-F|--test-filter)
			shift
			testFilter=$1
			;;
		-h|--help)
			printUsage
			exit
			;;
		*)	printUsage
			exit
			;;
	esac
	shift
done

targetFolder=release
if [ "$debug" = "1" ]; then
	targetFolder=debug
fi

echo "$deepClean"

if [ "$deepClean" = "1" ]; then
        echo "removing -rf ../../../bld/$projectType/$project/$targetFolder"
	rm -rf ../../../bld/$projectType/$project/$targetFolder
fi

if [ ! -d ../../../bld ]; then
  mkdir ../../../bld
fi

if [ ! -d ../../../bld/$projectType ]; then
  mkdir ../../../bld/$projectType
fi

if [ ! -d ../../../bld/$projectType/$project ]; then
  mkdir ../../../bld/$projectType/$project
fi

if [ ! -d ../../../bld/$projectType/$project/$targetFolder ]; then
  mkdir ../../../bld/$projectType/$project/$targetFolder
fi

pushd ../../../bld/$projectType/$project/$targetFolder

buildStatus=0
if [ "$testOnly" != "1" ]; then
	if [ "$debug" = "1" ]; then
		cmake ../../../../src/$projectType/$project -DCMAKE_BUILD_TYPE=Debug
	else
		cmake ../../../../src/$projectType/$project
	fi

	if [ "$clean" = "1" ]; then
		make clean
	fi

	make
	buildStatus=$?
	if [ "$buildStatus" -eq 0 ] && [ "$test" = "1" ]; then
		make run-tests
		buildStatus=$?
	fi
fi

if [ "$test" = "1" ] || [ "$testOnly" = "1" ]; then
	if [ "$buildStatus" -eq 0 ]; then
  		if [ "$testFilter" != "" ]; then
			ctest .. -R "$testFilter" 
		else
			ctest .. 
		fi
	fi
fi

popd

if [ "$copyToSrc" = "1" ]; then
	if [ -f ./blobUI.app ]; then
 		rm -rf ./$project.$targetFolder.app
	fi

	cp ../../../bld/$projectType/$project/$targetFolder/$project.app ./$project.$targetFolder.app
fi


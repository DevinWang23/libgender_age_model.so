#! /bin/bash

dir=`dirname $0`
build_dir="${dir}/build"
rm -rf $build_dir
mkdir $build_dir
pushd $build_dir
answer = 'n'

# read -p "Please select which mode will you to build, Y for exe, default to shared lib, [y/n]?": answer

echo "=================== BUILDING ===================="
case "$answer" in
  Y | y)
    export PREDICTOR__EXECUTABLE=1
    echo "building an executable app, to ./build/gender_age_model ...."
    ;;

  *)
    unset PREDICTOR__EXECUTABLE
    echo "building a shared lib, to ./build/libgender_age_model.so ...."
    ;;
esac
echo "=================== BUILDING ===================="

cmake ..
make -j12
popd

echo "=================== DONE ===================="

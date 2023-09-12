OPENRESTY_ROOT="/Users/kuknitin/Workspace/kuknitin/openresty"
OPENRESTY_IMG_PATH="/Users/kuknitin/Workspace/kuknitin/openresty/bin/package/rpm/out"
DELAY=0.5
BUILD_LOG_ROOT="/Users/kuknitin/Workspace/kuknitin/temp/logs/buildlogs"
VERSION=$1
BUILD_LOG_FILE=${BUILD_LOG_ROOT}/build_${VERSION}.log
BUILD_OUT_FILE_NAME=/lb-openresty-$1.el8-1.aarch64.rpm
BUILD_OUT_FILE="${OPENRESTY_IMG_PATH}/${BUILD_OUT_FILE_NAME}"

echo ${BUILD_LOG_FILE}
echo ${BUILD_OUT_FILE}

# check if that already exist
if [ -e ${BUILD_OUT_FILE} ]
then 
    echo "Build file already exists"
else 
    sudo ${OPENRESTY_ROOT}/bin/package/package.sh --version=$1 --os=ol8 --docker-suffix=test_ol8 --with-system-openssl --arm-build --with-debug-log &> ${BUILD_LOG_FILE}

fi


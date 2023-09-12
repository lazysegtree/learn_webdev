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
    lines=$(wc -l < ${BUILD_LOG_FILE})
    echo $lines

    while [ $lines -lt 4000 ]
    do 
        lines=$(wc -l < ${BUILD_LOG_FILE})
        printf "${lines} \t / 4000 \r"
        sleep 1
    done

    echo '\n'

fi


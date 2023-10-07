ARCH=$(uname -m)
echo $ARCH 

if [ $ARCH == "arm64" ]
then 
    echo "hi arm"
else 
    echo "hi 86"
fi
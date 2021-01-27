copy the depandancy to already created folder package/ 

> $ldd ./ImageViewer | grep "=> /"| awk '{print $3}' | xargs -I '{}' cp -v '{}' package/

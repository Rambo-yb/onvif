#!/bin/bash

export PATH="/home/smb/RV1126_RV1109_LINUX_SDK_V2.2.5.1_20230530/buildroot/output/rockchip_rv1126_rv1109/host/bin/:$PATH"

CUR_DIR=`pwd`
HOST=arm-linux-gnueabihf

GSOAP_TOOL_FILE=$CUR_DIR/gsoap_tool
GSOAP_TOOL_DIR=$CUR_DIR/third_lib/gsoap-2.8
GSOAP_TOOL_INSTALL_DIR=$CUR_DIR/_install/gsoap
INSTALL_DIR=$CUR_DIR/_install

build_gsoap_arm()
{
    cd $CUR_DIR/third_lib
    [ -d $GSOAP_TOOL_INSTALL_DIR ] && rm -r $GSOAP_TOOL_INSTALL_DIR
    [ -d $GSOAP_TOOL_DIR ] && rm -r $GSOAP_TOOL_DIR

    unzip gsoap_2.8.134.zip
    cp $CUR_DIR/wsa5_bck.h $GSOAP_TOOL_DIR/gsoap/import/wsa5.h -raf #解决SOAP_ENV__Fault重定义问题

    cd $GSOAP_TOOL_DIR
    ./configure --prefix=$GSOAP_TOOL_INSTALL_DIR --host=$HOST
    make

    #编译过程中，linux需要运行soapcpp2，将x86的可执行程序拷入，执行完之后恢复
    cp -raf ./gsoap/src/soapcpp2 ./soapcpp2_arm
    cp -raf $GSOAP_TOOL_FILE/soapcpp2 ./gsoap/src/soapcpp2
    touch ./gsoap/src/soapcpp2
    make
    cp -raf ./soapcpp2_arm ./gsoap/src/soapcpp2
    touch ./gsoap/src/soapcpp2

    make install
}

build_gsoap_tools()
{
    cd $CUR_DIR/third_lib
    [ -d $GSOAP_TOOL_INSTALL_DIR ] && rm -r $GSOAP_TOOL_INSTALL_DIR
    [ -d $GSOAP_TOOL_DIR ] && rm -r $GSOAP_TOOL_DIR

    unzip gsoap_2.8.134.zip
    cp $CUR_DIR/wsa5_bck.h $GSOAP_TOOL_DIR/gsoap/import/wsa5.h -raf #解决SOAP_ENV__Fault重定义问题

    cd $GSOAP_TOOL_DIR
    ./configure --prefix=$GSOAP_TOOL_INSTALL_DIR
    make && make install

    if [ ! -d $GSOAP_TOOL_FILE ]; then
        mkdir -p $GSOAP_TOOL_FILE
    fi 
    cd $GSOAP_TOOL_INSTALL_DIR 
    cp -raf bin/wsdl2h $GSOAP_TOOL_FILE
    cp -raf bin/soapcpp2 $GSOAP_TOOL_FILE
    cp -raf $GSOAP_TOOL_DIR/gsoap/typemap.dat $GSOAP_TOOL_FILE
}

build_openssl()
{
    cd $CUR_DIR/third_lib
    [ -d $INSTALL_DIR/openssl ] && rm -r $$INSTALL_DIR/openssl
    [ -d "openssl-1.1.1h" ] && rm -r openssl-1.1.1h

    tar -xvzf openssl-1.1.1h.tar.gz
    cd openssl-1.1.1h
    ./Configure no-asm no-shared no-async linux-generic32 --prefix=$INSTALL_DIR/openssl --cross-compile-prefix=$HOST-
    make && make install
}

build_gsoap_code()
{
    WSDL2H=$GSOAP_TOOL_FILE/wsdl2h
    SOAPCPP2=$GSOAP_TOOL_FILE/soapcpp2
    TYPEMAP=$GSOAP_TOOL_FILE/typemap.dat

    ONVIF_XSD= #http://www.onvif.org/onvif/ver10/schema/onvif.xsd
    REMOTE_DISCOVERY_WSDL=http://www.onvif.org/onvif/ver10/network/wsdl/remotediscovery.wsdl
    DEVICE_MGMT_WSDL=http://www.onvif.org/onvif/ver10/device/wsdl/devicemgmt.wsdl
    EVENTS_WSDL= #http://www.onvif.org/onvif/ver10/events/wsdl/event.wsdl
    DISPLAY_WSDL= #http://www.onvif.org/onvif/ver10/display.wsdl
    DEVICEIO_WSDL= #http://www.onvif.org/onvif/ver10/deviceio.wsdl
    IMAGING_WSDL= #http://www.onvif.org/onvif/ver20/imaging/wsdl/imaging.wsdl
    MEIDA_WSDL_10= #http://www.onvif.org/onvif/ver10/media/wsdl/media.wsdl
    MEIDA_WSDL_20= #http://www.onvif.org/onvif/ver20/media/wsdl/media.wsdl
    PTZ_WSDL= #http://www.onvif.org/onvif/ver20/ptz/wsdl/ptz.wsdl
    RECEIVER_WSDL= #http://www.onvif.org/onvif/ver10/receiver.wsdl
    RECORDING_WSDL= #http://www.onvif.org/onvif/ver10/recording.wsdl
    SEARCH_WSDL= #http://www.onvif.org/onvif/ver10/search.wsdl
    REPLAY_WSDL= #http://www.onvif.org/onvif/ver10/replay.wsdl

    cd $CUR_DIR
    cp -raf typemap.dat gsoap_tool/
    if [ -d $CUR_DIR/gsoap ]; then
        rm -r $CUR_DIR/gsoap
    fi 
    mkdir -p $CUR_DIR/gsoap

    cd $CUR_DIR/gsoap
    $WSDL2H -P -x -c -s -t $TYPEMAP -o gsoap.h \
        $ONVIF_XSD $REMOTE_DISCOVERY_WSDL $DEVICE_MGMT_WSDL $EVENTS_WSDL \
        $DISPLAY_WSDL $DEVICEIO_WSDL $IMAGING_WSDL $MEIDA_WSDL_10 $MEIDA_WSDL_20 \
        $PTZ_WSDL $RECEIVER_WSDL $RECORDING_WSDL $SEARCH_WSDL $REPLAY_WSDL

    sed -i 's/#import "wsa5.h"/#import "wsse.h"\r\n#import "wsa5.h"/g' gsoap.h

    $SOAPCPP2 -2 -x -c gsoap.h -I $GSOAP_TOOL_DIR/gsoap/import -I $GSOAP_TOOL_DIR/gsoap 

    cp -raf $GSOAP_TOOL_DIR/gsoap/dom.c ./
    cp -raf $GSOAP_TOOL_DIR/gsoap/stdsoap2.c ./
    cp -raf $GSOAP_TOOL_DIR/gsoap/stdsoap2.h ./
}

build_main()
{
    cd $CUR_DIR
    if [ -d $CUR_DIR/build ];
    then
        rm -r $CUR_DIR/build
    fi 

    mkdir build
    cd $CUR_DIR/build
    cmake .. -DCMAKE_C_COMPILER=${HOST}-gcc -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}/onvif
    make
    make install
}

[ "$1" == "arm" ] && build_gsoap_arm && exit
[ "$1" == "gsoap" ] && build_gsoap_tools && exit
[ "$1" == "openssl" ] && build_openssl && exit
[ "$1" == "code" ] && build_gsoap_code && exit
[ "$1" == "main" ] && build_main && exit

build_gsoap_tools
build_openssl
build_gsoap_code
build_main
#build_gsoap_arm


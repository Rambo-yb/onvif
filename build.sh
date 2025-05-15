#!/bin/bash

CUR_DIR=`pwd`

help()
{
	echo "./build.sh [chip] [oper]"
	echo "support chip: gk7205v200, rv1126, rk3588"
	echo "support oper:"
	echo "	 all	: Perform all operations"
	echo "	gsoap	: Compile the gsoap tool"
	echo "	openssl	: Compile the third-party library openssl"
	echo "	code	: Generate onvif code"
	echo "	main	: Compile onvif code"
	echo "	pack	: Package the onvif library"

	exit 0
}

if [ "$1" == "gk7205v200" ]; then
    # export PATH="/home/smb/GK7205V200/Software/GKIPCLinuxV100R001C00SPC020/tools/toolchains/arm-gcc6.3-linux-uclibceabi/bin/:$PATH"
    HOST=arm-gcc6.3-linux-uclibceabi
elif [ "$1" == "rv1126" ]; then
    export PATH="/home/smb/RV1126_RV1109_LINUX_SDK_V2.2.5.1_20230530/buildroot/output/rockchip_rv1126_rv1109/host/bin/:$PATH"
    HOST=arm-linux-gnueabihf
elif [ "$1" == "rk3588" ]; then
    # export PATH="/home/smb/compiler/rk3588_host/bin/:$PATH"
    # export PERL5LIB="/home/smb/compiler/rk3588_host/lib/perl5/5.32.1:$PERL5LIB"
    HOST="aarch64-buildroot-linux-gnu"
else
    help
    exit 0
fi

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
    ./configure --prefix=$GSOAP_TOOL_INSTALL_DIR --host=$HOST --disable-c-locale
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
    ./configure --prefix=$GSOAP_TOOL_INSTALL_DIR --disable-c-locale
    sed -i 's/#define HAVE_LOCALE_H 1/#define HAVE_LOCALE_H 0/' config.h
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
	SOURCE_DIR=openssl-1.1.1h
    SOURCE_INSTALL_DIR=$INSTALL_DIR/openssl

    cd $CUR_DIR/third_lib
    [ -d $SOURCE_DIR ] && rm -r $SOURCE_DIR
    [ -d $SOURCE_INSTALL_DIR ] && [ "$1" != "rebuild" ] && exit
    [ -d $SOURCE_INSTALL_DIR ] && rm -r $SOURCE_INSTALL_DIR


    tar -xvzf $SOURCE_DIR.tar.gz
    cd $SOURCE_DIR
    ./Configure no-asm no-async linux-generic32 --prefix=$SOURCE_INSTALL_DIR --cross-compile-prefix=$HOST-
    make && make install

	cd $CUR_DIR/third_lib
    [ -d $SOURCE_DIR ] && rm -r $SOURCE_DIR
}

build_gsoap_code()
{
    WSDL2H=$GSOAP_TOOL_FILE/wsdl2h
    SOAPCPP2=$GSOAP_TOOL_FILE/soapcpp2
    TYPEMAP=$GSOAP_TOOL_FILE/typemap.dat

    ONVIF_XSD=http://www.onvif.org/ver10/schema/onvif.xsd
    REMOTE_DISCOVERY_WSDL=http://www.onvif.org/ver10/network/wsdl/remotediscovery.wsdl
    DEVICE_MGMT_WSDL=http://www.onvif.org/ver10/device/wsdl/devicemgmt.wsdl
    EVENTS_WSDL=http://www.onvif.org/ver10/events/wsdl/event.wsdl
    DISPLAY_WSDL= #http://www.onvif.org/ver10/display.wsdl
    DEVICEIO_WSDL= #http://www.onvif.org/ver10/deviceio.wsdl
    IMAGING_WSDL=http://www.onvif.org/ver20/imaging/wsdl/imaging.wsdl
    MEIDA_WSDL_10=http://www.onvif.org/ver10/media/wsdl/media.wsdl
    MEIDA_WSDL_20= #http://www.onvif.org/ver20/media/wsdl/media.wsdl
    PTZ_WSDL=http://www.onvif.org/ver20/ptz/wsdl/ptz.wsdl
    # RECEIVER_WSDL=http://www.onvif.org/ver10/receiver.wsdl
    # RECORDING_WSDL=http://www.onvif.org/ver10/recording.wsdl
    # SEARCH_WSDL=http://www.onvif.org/ver10/search.wsdl
    # REPLAY_WSDL=http://www.onvif.org/ver10/replay.wsdl
	ANALYTICS_WSDL=https://www.onvif.org/ver20/analytics/wsdl/analytics.wsdl
	# RULES_XSD=https://www.onvif.org/ver20/analytics/rules.xsd

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
        $PTZ_WSDL $RECEIVER_WSDL $RECORDING_WSDL $SEARCH_WSDL $REPLAY_WSDL $ANALYTICS_WSDL

    sed -i 's/#import "wsa5.h"/#import "wsse.h"\r\n#import "wsa5.h"/g' gsoap.h

    $SOAPCPP2 -2 -x -c gsoap.h -I $GSOAP_TOOL_DIR/gsoap/import -I $GSOAP_TOOL_DIR/gsoap 

    cp -raf $GSOAP_TOOL_DIR/gsoap/dom.c ./
    cp -raf $GSOAP_TOOL_DIR/gsoap/stdsoap2.c ./
    cp -raf $GSOAP_TOOL_DIR/gsoap/stdsoap2.h ./
}

build_main()
{
    cd $CUR_DIR
    if [ -d $CUR_DIR/build ]; then
        rm -r $CUR_DIR/build
    fi 
    if [ -d $INSTALL_DIR/onvif ]; then
        rm -r $INSTALL_DIR/onvif
    fi 

    mkdir build
    cd $CUR_DIR/build
    cmake .. -DCMAKE_C_COMPILER=${HOST}-gcc -DCMAKE_CXX_COMPILER=${HOST}-g++ -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}/onvif
    make && make install
    exit 1
}

build_pack()
{
	CUR_DATE=$(date +"%Y%m%d")
    PACK_DIR=onvif_$1_$CUR_DATE
    [ -d $PACK_DIR ] && rm -r $PACK_DIR
    [ -f $PACK_DIR.tar.gz ] && rm -r $PACK_DIR.tar.gz
    
    cd $CUR_DIR
    mkdir $PACK_DIR
    cd $PACK_DIR
    cp $INSTALL_DIR/onvif/* ./ -raf
    cp $INSTALL_DIR/openssl/lib/lib*.so* ./lib -raf

    cd $CUR_DIR
    tar -cvzf $PACK_DIR.tar.gz $PACK_DIR
    rm $PACK_DIR -r
}

[ -z "$2" ] && help

[ "$2" == "help" ] && help && exit
[ "$2" == "gsoap_arm" ] && build_gsoap_arm && exit
[ "$2" == "gsoap" ] && build_gsoap_tools && exit
[ "$2" == "openssl" ] && build_openssl && exit
[ "$2" == "mxml" ] && build_mxml && exit
[ "$2" == "code" ] && build_gsoap_code && exit
[ "$2" == "main" ] && build_main && exit
[ "$2" == "pack" ] && build_pack $1 && exit

if [ "$2" == "all" ]; then
	build_gsoap_tools
	build_openssl
	build_gsoap_code
	build_main
	build_pack $1
else
    help
    exit 0
fi



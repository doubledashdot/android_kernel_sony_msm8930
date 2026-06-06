#!/bin/bash

# Defaults
ARCH=arm
DEFCONFIG=taoshan_defconfig
DEVICE=taoshan
LOCAL_PATH=$(pwd)
KERNEL=zImage
KERNEL_PATH=$LOCAL_PATH/arch/arm/boot/$KERNEL
ZIP_DIR=$LOCAL_PATH/zip/

# Tuneables
CROSS_COMPILE_LOCATION="$(pwd)/toolchain/arm-cortex_a15-linux-gnueabihf-linaro_4.9"
CROSS_COMPILE=$CROSS_COMPILE_LOCATION/bin/arm-eabi-
STRIP=$CROSS_COMPILE_LOCATION/bin/arm-eabi-strip
ZIP_PREFIX=n

# Colors
blue='\033[0;34m'
cyan='\033[0;36m'
yellow='\033[0;33m'
red='\033[0;31m'
nocol='\033[0m'

warn() {
	echo -e $yellow"Warning:"$nocol $@
}

error() {
	echo -e $red"Error:"$nocol $@
}

info() {
	echo -e $blue"Info:"$nocol $@
}

install() {
	echo -e $cyan"Install:"$nocol $@
}

export_variables() {
	export ARCH=$ARCH
	export CROSS_COMPILE=$CROSS_COMPILE
	export KBUILD_BUILD_USER="skye"
	export KBUILD_BUILD_HOST="build"
}

check_environment() {
	if [ -f $KERNEL_PATH ];
	then
		warn "You are making a dirty build"
	else
		info "Making a clean build"
	fi
	if [ -d $ZIP_DIR ];
	then
		info "Template zip directory found, proceeding..."
	else
		error "Template zip directory not found. Aborting!!"
		exit
	fi	
}

build_kernel() {
	make $DEFCONFIG
	make
}

make_zip() {
	cd $ZIP_DIR
	rm -rf $ZIP_DIR/rel
	mkdir $ZIP_DIR/rel
	cp -r $ZIP_DIR/raw $ZIP_DIR/rel
	cp $KERNEL_PATH $ZIP_DIR/rel/raw/tools/zImage
	zip -r rel/raw/$ZIP_PREFIX-$(date +"%Y%m%d")-$DEVICE.zip *
}

wrapper() {
	check_environment
	export_variables
	info "Building kernel.."
	build_kernel
	if [ ! -f $KERNEL_PATH ];
	then error "Build failed! Please fix the errors!" && exit
	fi
	info "Making flashable zip"
	make_zip
	install $LOCAL_PATH/zip/rel/$ZIP_PREFIX-$(date +"%Y%m%d")-$DEVICE.zip
}

wrapper $1

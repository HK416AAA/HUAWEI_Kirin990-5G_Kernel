export ARCH=arm64

export PATH=$PATH:/home/gcc4.9/bin

export CROSS_COMPILE=aarch64-linux-android-

make ARCH=arm64 O=out merge_kirin990_defconfig

make ARCH=arm64 O=out -j16

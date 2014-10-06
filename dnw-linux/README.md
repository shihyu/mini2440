編譯 ko , insmod 
make -C /lib/modules/`uname -r`/build M=`pwd` modules
sudo insmod ./secbulk.ko

sudo ./dnw ~/mini2440/mini2440_android/mini2440T35_android.img
sudo ./dnw ~/mini2440/mini2440_android/zImage
sudo ./dnw ~/mini2440/mini2440_android/supervivi-128M 


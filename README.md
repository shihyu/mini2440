mini2440
========
1. 先到網上搜索一下dnw for linux。下載回來解壓。
目錄如下：
dnw_linux/ 
dnw_linux/secbulk/ 
dnw_linux/secbulk/Makefile 
dnw_linux/secbulk/secbulk.c 
dnw_linux/dnw/ 
dnw_linux/dnw/dnw.c 
其中secbulk.c是PC端USB驅動, dnw.c是寫入工具

2編譯並加載secbulk.c內核模塊
$cd secbulk 
$make -C /lib/modules/`uname -r`/build M=`pwd` modules 

編譯成功後在當前目錄下可以看到secbulk.ko 
alex@alex- desktop:~/project3/qq2440/dnw_linux/dnw_linux/secbulk$ ls 
Makefile Module.symvers secbulk.c secbulk.ko secbulk.mod.c secbulk.mod.o secbulk.o 

用insmod命令加載模塊，這需要root權限
$sudo insmod ./secbulk.ko 

加載後用dmesg命令可以看到模塊已經成功加載
[ 238.007970] secbulk:secbulk loaded 
[ 238.009101] usbcore: registered new interface driver secbulk 

 編譯dnw寫入工具
$cd dnw 
$gcc -o dnw dnw. c 
編譯成功後生成可執行檔dnw 


3 。使用DNW下載
3.1啟動開發板，進入Supervivi，並將開發板和PC用USB電纜連接，此時用dmesg命令可以看到secbulk驅動加載：
[ 283.677772] usb 1-1: new full speed USB device using uhci_hcd and address 2 
[ 284.084835] usb 1-1: configuration #1 chosen from 1 choice 
[ 284.140430] secbulk:secbulk probing... 
[ 284.140482] secbulk:bulk out endpoint found! 

3.2此時可以用supervivi進行USB下載，vivi上步驟請參考用戶手冊，當出現“USB host is connected. Waiting a download.”時，在PC端Linux上用dnw工具寫入要下載的文件
$sudo ./dnw /path/to/what/you/want/ download 
＃此處使用命令寫入，無界面
具體命令見下

寫入環境如下  

root@haozi-desktop:/home/haozi/桌面/dnw_linux/dnw# sudo ./dnw /home/haozi/ruanjian/linux/supervivi -128M 

file name : /home/haozi/ruanjian/linux/supervivi-128M 
file size : 253912 bytes 
Writing data... 
100% 253922 bytes OK 

root@haozi-desktop:/home/haozi/桌面/dnw_linux/dnw# sudo ./dnw /home/haozi/ruanjian/linux/zImage_T35  
file name : /home/haozi/ruanjian/linux/zImage_T35 
file size : 2022348 bytes 
Writing data... 
100% 2022358 bytes OK 

root@haozi-desktop:/home/ haozi/桌面/dnw_linux/dnw# sudo ./dnw /home/haozi/ruanjian/linux/root_qtopia-128M.img  
file name : /home/haozi/ruanjian/linux/root_qtopia-128M.img 
file size : 58487616 bytes 
Writing data ... 
26% 58487626 bytes OK 
(前面的supervivi和內核寫的時候還可以後面的QT文件系統寫入的時候速度超級慢文件大了點) 



＃$sudo insmod ./secbulk.ko為加載模塊進內核
開機的時候不會自動加載，這樣每次都要先加載才可以使用，此時將其加入開機腳本，
使其得到自動加載,編輯/etc/init.d/rc.local在最後加上insmod /所在路徑/secbulk.ko。

gedit /etc/init.d/rc.local 
insmod/home/haozi/桌面/dnw_linux/secbulk/secbulk.ko  


＃調用dnw時只能在當前目錄使用./dnw執行，對此情況，將dnw添加到/ usr/local/sbin中，這樣可以在
任何目錄，使用sudo dnw filename來調用。

# bochs


## booting up softlanding linux
* softlanding linux image: http://www.oldlinux.org/Linux.old/bochs/
* simply booting up the image will have a PANIC:
![image](https://user-images.githubusercontent.com/35479537/221070345-a54cb253-db4b-4de1-97ea-7740d1ac1926.png)


* the issue is that, the Bochs BIOS is 128KB, to let the BIOS end at 0xfffff, the starting address must be 0xe0000 instead of 0xf0000
* the origin `bochsrc.bxrc` file

```
romimage: file=$BXSHARE\BIOS-bochs-latest, address=0xef0000
```
* update `bochsrc.bxrc` to be:
```
romimage: file=$BXSHARE\BIOS-bochs-latest, address=0xee0000
```
![image](https://user-images.githubusercontent.com/35479537/221070960-f44a1e0c-9c4f-49ee-bd88-1d57d66059ba.png)


## references
* https://forum.osdev.org/viewtopic.php?t=22772

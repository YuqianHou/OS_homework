# bigger files for xv6 作业报告

<center>软工2班 3017218092 侯雨茜</center>



###一、准备工作

xv6系统安装成功后，可以直接使用下面这条命令在终端编译并运行

```makefile
make qemu-nox
```



#### 1.1 修改Makefile文件

1. 将`CPUS := 2`修改为`CPUS := 1`
2. 在`QEMUOPTS`前，添加`QEMUEXTRA = -snapshot`

修改后的Makefile文件代码片段如下：

```makefile
ifndef CPUS
#CPUS := 2
CPUS := 1
endif
QEMUEXTRA = -snapshot
QEMUOPTS = -drive file=fs.img,index=1,media=disk,format=raw -drive 
```



#### 1.2 修改param.h文件

将`#define FSSIZE       1000  // size of file system in blocks`

修改为`#define FSSIZE         // size of file system in blocks`

修改后的param.h文件代码片段如下：

```c++
#define NBUF         (MAXOPBLOCKS*3)  // size of disk block cache
//#define FSSIZE       1000  // size of file system in blocks
#define FSSIZE       20000  // size of file system in blocks
```



####1.3 添加big.c文件

1. 将big.c文件放入xv6-public文件目录下

   big.c文件如下：

   ```c++
   #include "types.h"
   #include "stat.h"
   #include "user.h"
   #include "fcntl.h"
   
   int
   main()
   {
     char buf[512];
     int fd, i, sectors;
   
     fd = open("big.file", O_CREATE | O_WRONLY);
     if(fd < 0){
       printf(2, "big: cannot open big.file for writing\n");
       exit();
     }
   
     sectors = 0;
     while(1){
       *(int*)buf = sectors;
       int cc = write(fd, buf, sizeof(buf));
       if(cc <= 0)
         break;
       sectors++;
   	if (sectors % 100 == 0)
   		printf(2, ".");
     }
   
     printf(1, "\nwrote %d sectors\n", sectors);
   
     close(fd);
     fd = open("big.file", O_RDONLY);
     if(fd < 0){
       printf(2, "big: cannot re-open big.file for reading\n");
       exit();
     }
     for(i = 0; i < sectors; i++){
       int cc = read(fd, buf, sizeof(buf));
       if(cc <= 0){
         printf(2, "big: read error at sector %d\n", i);
         exit();
       }
       if(*(int*)buf != i){
         printf(2, "big: read the wrong data (%d) for sector %d\n",
                *(int*)buf, i);
         exit();
       }
     }
   
     printf(1, "done; ok\n"); 
   
     exit();
   }
   ```

2. 在Makefile文件中的UPROGS列表中添加big.c文件

   修改后的Makefile文件如下：

   ```makefile
   OBJS = \
   	bio.o\
   	console.o\
   	exec.o\
   	file.o\
   	fs.o\
   	ide.o\
   	ioapic.o\
   	kalloc.o\
   	kbd.o\
   	lapic.o\
   	log.o\
   	main.o\
   	mp.o\
   	picirq.o\
   	pipe.o\
   	proc.o\
   	sleeplock.o\
   	spinlock.o\
   	string.o\
   	swtch.o\
   	syscall.o\
   	sysfile.o\
   	sysproc.o\
   	trapasm.o\
   	trap.o\
   	uart.o\
   	vectors.o\
   	vm.o\
   
   # Cross-compiling (e.g., on Mac OS X)
   # TOOLPREFIX = i386-jos-elf
   
   # Using native tools (e.g., on X86 Linux)
   #TOOLPREFIX = 
   
   # Try to infer the correct TOOLPREFIX if not set
   ifndef TOOLPREFIX
   TOOLPREFIX := $(shell if i386-jos-elf-objdump -i 2>&1 | grep '^elf32-i386$$' >/dev/null 2>&1; \
   	then echo 'i386-jos-elf-'; \
   	elif objdump -i 2>&1 | grep 'elf32-i386' >/dev/null 2>&1; \
   	then echo ''; \
   	else echo "***" 1>&2; \
   	echo "*** Error: Couldn't find an i386-*-elf version of GCC/binutils." 1>&2; \
   	echo "*** Is the directory with i386-jos-elf-gcc in your PATH?" 1>&2; \
   	echo "*** If your i386-*-elf toolchain is installed with a command" 1>&2; \
   	echo "*** prefix other than 'i386-jos-elf-', set your TOOLPREFIX" 1>&2; \
   	echo "*** environment variable to that prefix and run 'make' again." 1>&2; \
   	echo "*** To turn off this error, run 'gmake TOOLPREFIX= ...'." 1>&2; \
   	echo "***" 1>&2; exit 1; fi)
   endif
   
   # If the makefile can't find QEMU, specify its path here
    QEMU = qemu-system-i386
   
   # Try to infer the correct QEMU
   ifndef QEMU
   QEMU = $(shell if which qemu > /dev/null; \
   	then echo qemu; exit; \
   	elif which qemu-system-i386 > /dev/null; \
   	then echo qemu-system-i386; exit; \
   	elif which qemu-system-x86_64 > /dev/null; \
   	then echo qemu-system-x86_64; exit; \
   	else \
   	qemu=/Applications/Q.app/Contents/MacOS/i386-softmmu.app/Contents/MacOS/i386-softmmu; \
   	if test -x $$qemu; then echo $$qemu; exit; fi; fi; \
   	echo "***" 1>&2; \
   	echo "*** Error: Couldn't find a working QEMU executable." 1>&2; \
   	echo "*** Is the directory containing the qemu binary in your PATH" 1>&2; \
   	echo "*** or have you tried setting the QEMU variable in Makefile?" 1>&2; \
   	echo "***" 1>&2; exit 1)
   endif
   
   CC = $(TOOLPREFIX)gcc
   AS = $(TOOLPREFIX)gas
   LD = $(TOOLPREFIX)ld
   OBJCOPY = $(TOOLPREFIX)objcopy
   OBJDUMP = $(TOOLPREFIX)objdump
   CFLAGS = -fno-pic -static -fno-builtin -fno-strict-aliasing -O2 -Wall -MD -ggdb -m32 -Werror -fno-omit-frame-pointer
   CFLAGS += $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)
   ASFLAGS = -m32 -gdwarf-2 -Wa,-divide
   # FreeBSD ld wants ``elf_i386_fbsd''
   LDFLAGS += -m $(shell $(LD) -V | grep elf_i386 2>/dev/null | head -n 1)
   
   # Disable PIE when possible (for Ubuntu 16.10 toolchain)
   ifneq ($(shell $(CC) -dumpspecs 2>/dev/null | grep -e '[^f]no-pie'),)
   CFLAGS += -fno-pie -no-pie
   endif
   ifneq ($(shell $(CC) -dumpspecs 2>/dev/null | grep -e '[^f]nopie'),)
   CFLAGS += -fno-pie -nopie
   endif
   
   xv6.img: bootblock kernel
   	dd if=/dev/zero of=xv6.img count=10000
   	dd if=bootblock of=xv6.img conv=notrunc
   	dd if=kernel of=xv6.img seek=1 conv=notrunc
   
   xv6memfs.img: bootblock kernelmemfs
   	dd if=/dev/zero of=xv6memfs.img count=10000
   	dd if=bootblock of=xv6memfs.img conv=notrunc
   	dd if=kernelmemfs of=xv6memfs.img seek=1 conv=notrunc
   
   bootblock: bootasm.S bootmain.c
   	$(CC) $(CFLAGS) -fno-pic -O -nostdinc -I. -c bootmain.c
   	$(CC) $(CFLAGS) -fno-pic -nostdinc -I. -c bootasm.S
   	$(LD) $(LDFLAGS) -N -e start -Ttext 0x7C00 -o bootblock.o bootasm.o bootmain.o
   	$(OBJDUMP) -S bootblock.o > bootblock.asm
   	$(OBJCOPY) -S -O binary -j .text bootblock.o bootblock
   	./sign.pl bootblock
   
   entryother: entryother.S
   	$(CC) $(CFLAGS) -fno-pic -nostdinc -I. -c entryother.S
   	$(LD) $(LDFLAGS) -N -e start -Ttext 0x7000 -o bootblockother.o entryother.o
   	$(OBJCOPY) -S -O binary -j .text bootblockother.o entryother
   	$(OBJDUMP) -S bootblockother.o > entryother.asm
   
   initcode: initcode.S
   	$(CC) $(CFLAGS) -nostdinc -I. -c initcode.S
   	$(LD) $(LDFLAGS) -N -e start -Ttext 0 -o initcode.out initcode.o
   	$(OBJCOPY) -S -O binary initcode.out initcode
   	$(OBJDUMP) -S initcode.o > initcode.asm
   
   kernel: $(OBJS) entry.o entryother initcode kernel.ld
   	$(LD) $(LDFLAGS) -T kernel.ld -o kernel entry.o $(OBJS) -b binary initcode entryother
   	$(OBJDUMP) -S kernel > kernel.asm
   	$(OBJDUMP) -t kernel | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > kernel.sym
   
   # kernelmemfs is a copy of kernel that maintains the
   # disk image in memory instead of writing to a disk.
   # This is not so useful for testing persistent storage or
   # exploring disk buffering implementations, but it is
   # great for testing the kernel on real hardware without
   # needing a scratch disk.
   MEMFSOBJS = $(filter-out ide.o,$(OBJS)) memide.o
   kernelmemfs: $(MEMFSOBJS) entry.o entryother initcode kernel.ld fs.img
   	$(LD) $(LDFLAGS) -T kernel.ld -o kernelmemfs entry.o  $(MEMFSOBJS) -b binary initcode entryother fs.img
   	$(OBJDUMP) -S kernelmemfs > kernelmemfs.asm
   	$(OBJDUMP) -t kernelmemfs | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > kernelmemfs.sym
   
   tags: $(OBJS) entryother.S _init
   	etags *.S *.c
   
   vectors.S: vectors.pl
   	./vectors.pl > vectors.S
   
   ULIB = ulib.o usys.o printf.o umalloc.o
   
   _%: %.o $(ULIB)
   	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o $@ $^
   	$(OBJDUMP) -S $@ > $*.asm
   	$(OBJDUMP) -t $@ | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $*.sym
   
   _forktest: forktest.o $(ULIB)
   	# forktest has less library code linked in - needs to be small
   	# in order to be able to max out the proc table.
   	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _forktest forktest.o ulib.o usys.o
   	$(OBJDUMP) -S _forktest > forktest.asm
   
   mkfs: mkfs.c fs.h
   	gcc -Werror -Wall -o mkfs mkfs.c
   
   # Prevent deletion of intermediate files, e.g. cat.o, after first build, so
   # that disk image changes after first build are persistent until clean.  More
   # details:
   # http://www.gnu.org/software/make/manual/html_node/Chained-Rules.html
   .PRECIOUS: %.o
   
   UPROGS=\
   	_cat\
   	_echo\
   	_forktest\
   	_grep\
   	_init\
   	_kill\
   	_ln\
   	_ls\
   	_mkdir\
   	_rm\
   	_sh\
   	_stressfs\
   	_usertests\
   	_wc\
   	_zombie\
   	_big\
   
   fs.img: mkfs README $(UPROGS)
   	./mkfs fs.img README $(UPROGS)
   
   -include *.d
   
   clean: 
   	rm -f *.tex *.dvi *.idx *.aux *.log *.ind *.ilg \
   	*.o *.d *.asm *.sym vectors.S bootblock entryother \
   	initcode initcode.out kernel xv6.img fs.img kernelmemfs \
   	xv6memfs.img mkfs .gdbinit \
   	$(UPROGS)
   
   # make a printout
   FILES = $(shell grep -v '^\#' runoff.list)
   PRINT = runoff.list runoff.spec README toc.hdr toc.ftr $(FILES)
   
   xv6.pdf: $(PRINT)
   	./runoff
   	ls -l xv6.pdf
   
   print: xv6.pdf
   
   # run in emulators
   
   bochs : fs.img xv6.img
   	if [ ! -e .bochsrc ]; then ln -s dot-bochsrc .bochsrc; fi
   	bochs -q
   
   # try to generate a unique GDB port
   GDBPORT = $(shell expr `id -u` % 5000 + 25000)
   # QEMU's gdb stub command line changed in 0.11
   QEMUGDB = $(shell if $(QEMU) -help | grep -q '^-gdb'; \
   	then echo "-gdb tcp::$(GDBPORT)"; \
   	else echo "-s -p $(GDBPORT)"; fi)
   ifndef CPUS
   #CPUS := 2
   CPUS := 1
   endif
   QEMUEXTRA = -snapshot
   QEMUOPTS = -drive file=fs.img,index=1,media=disk,format=raw -drive file=xv6.img,index=0,media=disk,format=raw -smp $(CPUS) -m 512 $(QEMUEXTRA)
   
   qemu: fs.img xv6.img
   	$(QEMU) -serial mon:stdio $(QEMUOPTS)
   
   qemu-memfs: xv6memfs.img
   	$(QEMU) -drive file=xv6memfs.img,index=0,media=disk,format=raw -smp $(CPUS) -m 256
   
   qemu-nox: fs.img xv6.img
   	$(QEMU) -nographic $(QEMUOPTS)
   
   .gdbinit: .gdbinit.tmpl
   	sed "s/localhost:1234/localhost:$(GDBPORT)/" < $^ > $@
   
   qemu-gdb: fs.img xv6.img .gdbinit
   	@echo "*** Now run 'gdb'." 1>&2
   	$(QEMU) -serial mon:stdio $(QEMUOPTS) -S $(QEMUGDB)
   
   qemu-nox-gdb: fs.img xv6.img .gdbinit
   	@echo "*** Now run 'gdb'." 1>&2
   	$(QEMU) -nographic $(QEMUOPTS) -S $(QEMUGDB)
   
   # CUT HERE
   # prepare dist for students
   # after running make dist, probably want to
   # rename it to rev0 or rev1 or so on and then
   # check in that version.
   
   EXTRA=\
   	mkfs.c ulib.c user.h cat.c echo.c forktest.c grep.c kill.c\
   	ln.c ls.c mkdir.c rm.c stressfs.c usertests.c wc.c zombie.c\
   	printf.c umalloc.c\
   	README dot-bochsrc *.pl toc.* runoff runoff1 runoff.list\
   	.gdbinit.tmpl gdbutil\
   
   dist:
   	rm -rf dist
   	mkdir dist
   	for i in $(FILES); \
   	do \
   		grep -v PAGEBREAK $$i >dist/$$i; \
   	done
   	sed '/CUT HERE/,$$d' Makefile >dist/Makefile
   	echo >dist/runoff.spec
   	cp $(EXTRA) dist
   
   dist-test:
   	rm -rf dist
   	make dist
   	rm -rf dist-test
   	mkdir dist-test
   	cp dist/* dist-test
   	cd dist-test; $(MAKE) print
   	cd dist-test; $(MAKE) bochs || true
   	cd dist-test; $(MAKE) qemu
   
   # update this rule (change rev#) when it is time to
   # make a new revision.
   tar:
   	rm -rf /tmp/xv6
   	mkdir -p /tmp/xv6
   	cp dist/* dist/.gdbinit.tmpl /tmp/xv6
   	(cd /tmp; tar cf - xv6) | gzip >xv6-rev10.tar.gz  # the next one will be 10 (9/17)
   
   .PHONY: dist-test dist
   ```



####1.4 重新运行xv6

1. 在xv6-public目录下输入`make qumu-nox`，使xv6在qemu中运行
2. 输入命令big，会显示有140个sectors：

![image-20191008140319848](/Users/yuqianhou/Library/Application%20Support/typora-user-images/image-20191008140319848.png)

![image-20191008140441503](/Users/yuqianhou/Library/Application%20Support/typora-user-images/image-20191008140441503.png)



### 二、问题需求分析

####2.1 xv6中的inode分析

由下图可知，一个inode有12个direct pointers分别指向12个disk blocks，还有一个indirect pointer指向另一个indirect block。这个indirect block有`BSIZE / sizeof(uint) = 128`个指针指向disk blocks。因此，一个inode可以指向`12 +128 = 140`个数据块。也就是运行了`big`命令后输出的140个sectors。

![20161127134303831](/Users/yuqianhou/Documents/Software_Engineering/操作系统/作业/bigger files for xv6/20161127134303831.png)



####2.2 问题描述

修改`bmap()`函数，以便除了direct blocks和singly-indirect block之外，还实现doubly-indirect block。只有使用11个direct blocks，而不是12个，才能为新的doubly-indirect block腾出空间。无权更改磁盘inode的大小。ip-> addrs []的前11个元素应该是direct blocks；第十二个应该是一个singly-indirect block（就像当前的一样）；第13个应该是新的doubly-indirect block。

您无需修改xv6即可处理带有doubly-indirect blocks的文件删除。

如果一切顺利，`big`的执行结果可以写入16523个sectors，这需要几十秒钟才能完成。

![inode](/Users/yuqianhou/Documents/Software_Engineering/操作系统/作业/bigger files for xv6/inode.png)



### 三、修改xv6代码的过程

####3.1 修改fs.h文件

由于需要使用11个direct blocks，为新的doubly-indirect block腾出空间，因此需要对fs.h文件中的参数做一些修改：

1. 将`#define NDIRECT 12`修改为`#define NDIRECT 11`

2. 将`#define MAXFILE (NDIRECT + NINDIRECT)`

   修改为`#define MAXFILE (NDIRECT + NINDIRECT + NINDIRECT * NINDIRECT)`

3. 将`uint addrs[NDIRECT+1];   // Data block addresses`

   修改为`uint addrs[NDIRECT+2];   // Data block addresses`

修改后的fs.h文件如下：

```c++
// On-disk file system format.
// Both the kernel and user programs use this header file.


#define ROOTINO 1  // root i-number
#define BSIZE 512  // block size

// Disk layout:
// [ boot block | super block | log | inode blocks |
//                                          free bit map | data blocks]
//
// mkfs computes the super block and builds an initial file system. The
// super block describes the disk layout:
struct superblock {
  uint size;         // Size of file system image (blocks)
  uint nblocks;      // Number of data blocks
  uint ninodes;      // Number of inodes.
  uint nlog;         // Number of log blocks
  uint logstart;     // Block number of first log block
  uint inodestart;   // Block number of first inode block
  uint bmapstart;    // Block number of first free map block
};

//#define NDIRECT 12
#define NDIRECT 11
#define NINDIRECT (BSIZE / sizeof(uint))
//#define MAXFILE (NDIRECT + NINDIRECT)
#define MAXFILE (NDIRECT + NINDIRECT + NINDIRECT * NINDIRECT)

// On-disk inode structure
struct dinode {
  short type;           // File type
  short major;          // Major device number (T_DEV only)
  short minor;          // Minor device number (T_DEV only)
  short nlink;          // Number of links to inode in file system
  uint size;            // Size of file (bytes)
  //uint addrs[NDIRECT+1];   // Data block addresses
    uint addrs[NDIRECT+2];   // Data block addresses
};

// Inodes per block.
#define IPB           (BSIZE / sizeof(struct dinode))

// Block containing inode i
#define IBLOCK(i, sb)     ((i) / IPB + sb.inodestart)

// Bitmap bits per block
#define BPB           (BSIZE*8)

// Block of free map containing bit for block b
#define BBLOCK(b, sb) (b/BPB + sb.bmapstart)

// Directory is a file containing a sequence of dirent structures.
#define DIRSIZ 14

struct dirent {
  ushort inum;
  char name[DIRSIZ];
};
```



#### 3.2 修改fs.c文件

需要将原来的`12+128`改成`11+128+128*128`：

修改后的bmap()函数如下：

```c++
static uint
bmap(struct inode *ip, uint bn)
{
  uint addr, *a, *indirect, *doubleIndirect, indirectIdx, doubleIndirectIdx;
  struct buf *bp, *bp2;

  if(bn < NDIRECT){
    if((addr = ip->addrs[bn]) == 0)
      ip->addrs[bn] = addr = balloc(ip->dev);
    return addr;
  }
  bn -= NDIRECT;

  if(bn < NINDIRECT){
    // Load indirect block, allocating if necessary.
    if((addr = ip->addrs[NDIRECT]) == 0)
      ip->addrs[NDIRECT] = addr = balloc(ip->dev);
    bp = bread(ip->dev, addr);
    a = (uint*)bp->data;
    if((addr = a[bn]) == 0){
      a[bn] = addr = balloc(ip->dev);
      log_write(bp);
    }
    brelse(bp);
    return addr;
  }
    bn -= NINDIRECT;
    if (bn < NINDIRECT * NINDIRECT) {
        // Load first indirect block, allocating if necessary.
        if ((addr = ip->addrs[NDIRECT + 1]) == 0) {
            ip->addrs[NDIRECT + 1] = addr = balloc(ip->dev);
        }
        bp = bread(ip->dev, addr);
        indirect = (uint *)bp->data;
        indirectIdx = bn / NINDIRECT;
        
        if ((addr = indirect[indirectIdx]) == 0) {
            addr = indirect[indirectIdx] = balloc(ip->dev);
            log_write(bp);
        }
        
        bp2 = bread(ip->dev, addr);
        doubleIndirect = (uint *)bp2->data;
        doubleIndirectIdx = bn % NINDIRECT;
        
        if ((addr = doubleIndirect[doubleIndirectIdx]) == 0) {
            addr = doubleIndirect[doubleIndirectIdx] = balloc(ip->dev);
            log_write(bp2);
        }
        
        brelse(bp2);
        brelse(bp);
        return addr;
    }

  panic("bmap: out of range");
}
```



### 四、测试结果

重新编译运行xv6，重新运行big命令：

![image-20191008144533926](/Users/yuqianhou/Library/Application Support/typora-user-images/image-20191008144533926.png)

由上图可知，big命令运行后过了几十秒，sectors从140变成了16523（`= 11(singly-indirect blocks) + 128(direct blocks) + 128*128(doubly-indirect blocks) `）。
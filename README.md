# 2025_DKU_OS


This is a place for studying Operaing System in Dankook University.

### Professor / Assistant
- Class 1: Jongmoo Choi / Yeojin Oh (yeojinoh@dankook.ac.kr)
- Class 3: Gunhee Choi / Boseung Kim (bskim1102@dankook.ac.kr) 
- 2025 DKU Operating System Course Information [(Link)](http://embedded.dankook.ac.kr/~choijm/course/course.html#OS)

## Download Environment
### Windows
- Virtual Machine Platform: [VirtualBox 7.1.6](https://www.virtualbox.org/wiki/Downloads)
- Windows Subsystem for Linux Install Guide: [WSL](https://docs.microsoft.com/ko-KR/windows/wsl/install-win10#step-4---download-the-linux-kernel-update-package)

### MacOS
- Virtual Machine Platform: [UTM 4.6](https://mac.getutm.app/)

### Operating System (Guest OS)
- Ubuntu 22.04.5 LTS (For [Windows](https://releases.ubuntu.com/jammy/), For [MacOS](https://cdimage.ubuntu.com/releases/jammy/release/))


## Lab0
Lab0 contains information about installing a virtual machine and Ubuntu. Set the environment according to the documentation. The documentation for lab0 is at the link below.
- [Lab0 Document]([DKU_OS_LAB0]%20Linux%20Install%20Manual.pdf)

## Lab1
Implement scheduling techniques for CPU virtualization and compare their performance. 
- [Lab1 Document](https://github.com/DKU-EmbeddedSystem-Lab/2025_DKU_OS/blob/main/%5BDKU_OS_LAB1%5D%20CPU%20Virtualization.pdf)
- Class 1
  - ~~[Google Form](https://forms.gle/XGvaHxAuXT39XUkR6)~~
  - ~~Deadline: **2025.04.22 23:59**~~
- Class 3
  - ~~[Google Form](https://forms.gle/HXSmrauZpNmPu1co7)~~
  - ~~Deadline: **2025.05.02 23:59**~~
 
**02025/04/08 Issue** - Makefile [link](https://github.com/DKU-EmbeddedSystem-Lab/2025_DKU_OS/issues/1)

**2025/04/19 Issue** - test.cpp [link](https://github.com/DKU-EmbeddedSystem-Lab/2025_DKU_OS/issues/2)

## Lab2
Implement concurrent data structure and compare their performance. 
- [Lab2 Document](https://github.com/DKU-EmbeddedSystem-Lab/2025_DKU_OS/blob/main/%5BDKU_OS_LAB2%5D%20Concurrency.pdf)
- Class 1
  - [Google Form](https://forms.gle/ce1nCpRVUF8eDsjZA)
  - Deadline: **2025.05.20 23:59**
- Class 3
  - [Google Form](https://forms.gle/m3TUns69gZUXk3RT7)
  - Deadline: **2025.05.21 23:59**

### Getting Start

```
git clone https://github.com/DKU-EmbeddedSystem-Lab/2025_DKU_OS.git // clone repository
cd 2025_DKU_OS                                                      // enter directory
./install_package.sh                                                // install required packages
cd lab1                                                             // enter directory lab1 
./run.sh                                                            // make & excution program 
```



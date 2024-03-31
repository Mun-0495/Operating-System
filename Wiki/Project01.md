# Project01 - Make gpid Systemcall

## 첫 번째 프로젝트입니다. 크게 4 부분으로 나뉘고, 각각의 단계에 대해서 기술해놓았습니다.

***

* Design
  * 명세에서 요구하는 조건에 대한 구현 계획을 서술하고 있습니다.
* Implement
  * 새롭게 구현하거나 수정한 부분이 기존과 어떻게 다른지, 해당 코드의 목적이 무엇인지에 대해 구체적으로 서술하고 있습니다.
* Result
  * 컴파일 및 실행과정과, 명세의 요구부분이 정상적으로 동작하는 실행결과를 첨부하고, 동작 과정에 대해 설명하고 있습니다.
* Trouble shooting
  * 과제 수행 과정에서 겪은 문제와, 해당 문제와 해결 과정을 서술하고 있습니다.
 
***

### Design

* 우리는 gpid 시스템 콜을 구현할 것입니다.
* gpid 시스템 콜은 현재 프로세스의 부모의 부모, 즉 조부모의 pid를 호출하는 시스템 콜입니다.
* 이 시스템콜을 구현하기 전에, 먼저 xv6의 시스템 콜엔 어떤게 있는지 살펴봅시다.
* xv6의 파일 내부는 다음과 같이 이루어져 있습니다.
  * ```shell
     $ ls
  * ![ls] (/Users/gyungtaemun/Desktop/Operating-System/img/Project1/xv6_ls.jpg)
* 자, 이제 xv6의 파일 내부에서 syscall.h를 열어봅시다. 이 파일안엔 xv6 내부에서 쓸 수 있는 시스템콜이 내장되어 있을 겁니다.
  * ```shell
     $ vim syscall.h
  * ![vim syscall] (/Users/gyungtaemun/Desktop/Operating-System/img/Project1/xv6_syscall.jpg)
  * 확인해보니, 현재 프로세스 아이디를 불러올 수 있는 getpid()라는 시스템콜이 이미 구현되어있는걸 확인할 수 있습니다.
  * getpid()함수를 통해 gpid를 구현할 수 있을테니, 저희는 이전에 깔았던 cscope를 활용하여 getpid()가 어디서 호출되어 있고, 어떻게 구현되어 있는지 확인해 봅시다.
* vim의 Command모드에서 **:cs find c getpid** 를 입력해 getpid()함수가 어디서 호출되어있는지 확인해보고, **:cs find s getpid** 를 입력해 getpid가 어디서 심볼로 확인되어있는지 확인해 봅시다.
  * ![getpid1] (/Users/gyungtaemun/Desktop/Operating-System/img/Project1/xv6_cscope_getpid.jpg)
  * 확인해보니, getpid는 user.h안에서 함수를 호출하고 있고, user.h는 다양한 곳에서 호출되고 있습니다. 자 일단, syscall.h안에 getpid가 있으니, syscall.h를 include 하고 함수를 찾아봅시다
  * ![getpid2] (/Users/gyungtaemun/Desktop/Operating-System/img/Project1/xv6_cscope_syscall.jpg)
  * syscall.c에서 syscall.h를 호출하고 있군요. 자, 그럼 syscall.c를 살펴봅시다.
  * ![getpid3] (/Users/gyungtaemun/Desktop/Operating-System/img/Project1/xv6_vim_syscall.jpg)

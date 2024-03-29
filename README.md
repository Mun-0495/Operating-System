# Operating-System
* 3-1 운영체제 과제 제출용

<hr1>

## xv6 Install
* xv-6를 설치하는 방법은 다음과 같습니다.
* 저는 mac-m1 환경을 이용하고 있으므로 Docker를 이용한 설치방법을 안내하겠습니다.
  * 먼저 Docker를 설치해줍니다.
  * https://docs.docker.com/desktop/install/mac-install
  * 다음 링크로 들어가 본인 환경에 맞는 Docker를 설치해주시면 됩니다.
  * Docker Desktop을 설치한 이후, 실행하여 환경설정에 들어갑니다
  * Resouces->Advanced로 들어가 본인 mac 사양에 맞춰 설정합니다.
  * 터미널 실행 후 도커 버전을 확인합니다
  * ```shell
    $ docker --version

* 지금부턴 xv6를 실행해보겠습니다. 먼저 xv6 파일을 다운받아줍니다.
  * https://drive.google.com/file/d/1sEqKz-LOMOUe2c7LtHclIjDbmyGv8Han/view?usp=drive_link
  * 다음 사이트로 들어가 .tar파일을 다운받아 줍니다.
  * 원하는 디렉토리에 .tar파일을 옮겨줍니다
  * ```shell
    $ mv ./os_xv6.tar /User/gyungtaemun/Desktop/xv6
* 옮긴 뒤, 자기가 옮긴 디렉토리로 들어갑니다.
  * ```shell
    $ docker load -i os_xv6.tar
    $ docker images
  * 순서대로 입력하고, 성공적으로 이미지가 업로드 되었다면 다음과 같은 이미지 파일이 뜨게 될겁니다.
  * (이미지 파일 업로드 예정)
* 


<hr1>

## CODE
### 실제로 제출되는 코드가 존재합니다.
> * xv6-public
> * 이 폴더안에 코드로 구현되어있습니다.
> * 자세히 보려면 폴더 안으로 들어가서 확인해주시길 바랍니다.
> * https://github.com/Mun-0495/Operating-System/tree/main/xv6-public

<hr1>

## WIKI
### 각 프로젝트에 대한 위키는 여기에 저장되어 있습니다.
### 자세한 내용은 위키를 참조하시면 되겠습니다.

* 프로젝트는 총 4개입니다
  * Project01
  * Project02
  * Project03
  * Project04

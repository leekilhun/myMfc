// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"


const int MAX_BUF = 4096;

//enum DATA_TYPE {  };

enum class DATA_TYPE {
    _NICK, _TEXT, _IMAGE_NAME, _IMAGE, _FILE, _FILE_NAME, _UNKNOWN
};

const unsigned int UM_CONNECT_CLIENT = WM_USER + 1;
const unsigned int UM_DISCONNECT_CLIENT = WM_USER + 2;
const unsigned int UM_RECV_NICK = WM_USER + 3;
const unsigned int UM_RECV_TEXT = WM_USER + 4;
const unsigned int UM_RECV_IMAGE = WM_USER + 5;
const unsigned int UM_RECV_FILE = WM_USER + 6;



#endif //PCH_H

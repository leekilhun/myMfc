#pragma once
#include <stdint.h>
#include <queue>
#include <chrono>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;


namespace buffer
{

    template <typename T>
    class _que
    {
        T* m_Buff;
        uint32_t m_Head, m_Tail;
        uint32_t m_Length;

    public:
        _que(uint32_t length = 4096) :
            m_Head(0), m_Tail(0), m_Length(length)
            , m_Buff(new T[length]) {

        }
        virtual ~_que() {
            if (m_Buff != nullptr)
                delete[] m_Buff;
            m_Buff = nullptr;
            //Init();
        }

        inline void Init() {
            m_Head = m_Tail = 0;
        }

        inline bool Write(T* p_data, uint32_t length) {
            bool ret = true;
            uint32_t next_in;
            for (int i = 0; i < (int)length; i++) {
                next_in = (m_Head + 1) % m_Length;
                if (next_in != m_Tail) {
                    m_Buff[m_Head] = p_data[i];
                    m_Head = next_in;
                }
                else {
                    ret = false;
                    break;
                }
            }
            return ret;
        }

        inline bool Read(T* p_data, uint32_t length) {
            bool ret = true;
            for (int i = 0; i < (int)length; i++) {
                p_data[i] = m_Buff[m_Tail];
                if (m_Tail != m_Head) {
                    m_Tail = (m_Tail + 1) % m_Length;
                }
                else {
                    ret = false;
                    break;
                }
            }
            return ret;
        }

        inline int Available() {
            return (m_Length + (m_Head - m_Tail)) % m_Length;
        }

        inline bool Put(T data) {
            bool ret = true;
            uint32_t next_in;
            next_in = (m_Head + 1) % m_Length;
            if (next_in != m_Tail) {
                m_Buff[m_Head] = data;
                m_Head = next_in;
            }
            else
                ret = false;

            return ret;
        }

        inline bool Get(T* p_data) {
            bool ret = true;
            *(p_data) = m_Buff[m_Tail];
            if (m_Tail != m_Head) {
                m_Tail = (m_Tail + 1) % m_Length;
            }
            else
                ret = false;

            return ret;
        }

        inline T Pop(uint32_t addr) {
            if (addr > (m_Length - 1))
                return m_Buff[m_Tail];
            return m_Buff[addr];
        }



        inline void Pop(uint32_t addr, T* p_data) {
            if (addr > (m_Length - 1))
                return;
            *(p_data) = m_Buff[addr];
        }

        inline uint32_t GetSize() {
            return ((m_Head - m_Tail) % m_Length);
        }


        inline T operator[](uint32_t addr) {
            return Pop[addr];
        }

        inline void Flush() {
            m_Head = m_Tail = 0;
        }

    };

    template <typename T>
    class _vector
    {
        T* m_Data;
        uint32_t m_Capacity;
        uint32_t m_Length;

    public:
        _vector(uint32_t capa = 1) :
            m_Data(new T[capa]), m_Capacity(capa), m_Length(0) { }

        virtual ~_vector() {
            if (m_Data != nullptr) {
                delete[] m_Data;
            }
        }

    private:
    public:
        inline void push_back(T data) {
            if (m_Capacity <= m_Length) {
                T* temp = new T[m_Capacity * 2]{};
                for (int i = 0; i < m_Length; i++) {
                    temp[i] = data[i % m_Capacity];
                }
                delete[] m_Data;
                m_Data = temp;
                m_Capacity *= 2;
            }
            m_Data[m_Length] = data;
            m_Length++;
        }

        inline T operator[](uint32_t i) { return m_Data[i]; }

        inline void remove(uint32_t x) {
            for (int i = (int)x + 1; i < m_Length; i++) {
                m_Data[i - 1] = data[i];
            }
            m_Length--;
        }

        inline int size() { return (int)m_Length; }

        inline bool insert(T& element, uint32_t addr) {
            if (m_Length < (add - 1))
                return false;
            m_Length++;
            for (int i = (int)m_Length; i > (int)addr; i--) {
                m_Data[i] = m_Data[(i - 1)];
            }
            m_Data[addr] = element;
            return true;
        }

    };

    template <typename T>
    class _stack
    {
        struct Node
        {
            Node* prev;
            T s;
            Node(Node* prev, T s) :prev(prev), s(s) {}
        };
        Node* m_Current;
        Node m_Start;
    public:
        _stack() :m_Start(0, 0) {
            m_Current = &m_Start;
        }

        virtual ~_stack() {
            while (m_Current != &m_Start) {
                Node* prev = m_Current;
                m_Current = m_Current->prev;
                delete prev;
            }
        }

    private:
    public:
        inline void push(T s) {
            Node* n = new Node(m_Current, s);
            m_Current = n;
        }

        inline T pop() {
            T ret = {};
            if (m_Current == &m_Start) return ret;
            ret = m_Current->s;
            Node* prev = m_Current;
            m_Current = m_Current->prev;
            delete prev;
            return ret;
        }

        inline T peek() { return m_Current->s; }

        inline bool is_empty() {
            if (m_Current == &m_Start) return true;
            return false;
        }
    };
}



namespace conv
{
    inline int DwToInt(byte* bytes) {
        int a = bytes[0] & 0xFF;
        a |= ((bytes[1] << 8) & 0xFF00);
        a |= ((bytes[2] << 16) & 0xFF0000);
        a |= ((bytes[3] << 24) & 0xFF000000);

        return a;
    }
}

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include<io.h>
#include<direct.h>
#include<list>

namespace file
{
    const errno_t FILE_ERR_SUCCESS = 0;
    const errno_t FILE_ERR_NO_FILE_IN_DIR = -1;

    /// <summary>
    /// 현재 디렉토리 정보를 버퍼에 가져온다
    /// </summary>
    /// <param name="p_buff">버퍼 주소</param>
    /// <param name="len">버퍼 사이즈</param>
    inline void  CurrDir(char* p_buff, int len) {
        char* ret = _getcwd(p_buff, len);
    }

    /// <summary>
    /// 디렉토리를 생성
    /// </summary>
    /// <param name="dir_name">디렉토리 정보</param>
    /// <returns></returns>
    inline int  MakeDir(const char* dir_name) {
        return _mkdir(dir_name);
    }

    /// <summary>
    /// 디렉토리를 삭제
    /// </summary>
    /// <param name="dir_name">디렉토리 정보</param>
    /// <returns></returns>
    inline int  DeleteDir(const char* dir_name) {
        return _rmdir(dir_name);
    }

    inline bool  isFile(_finddata_t fd) {
        if (fd.attrib & _A_SUBDIR)
            return false;
        return true;
    }

    inline bool  isDir(_finddata_t fd) {
        if (fd.attrib & _A_SUBDIR)
            return true;
        return false;
    }

    inline bool  isDirExists(const char* dir) {
        return (_access_s(dir, 0) == 0 ? true : false);
    }


    inline bool  isFileExists(const char* file) {
        return (_access_s(file, 0) == 0 ? true : false);
    }

    /// <summary>
    /// 하위 디렉토리 포함 파일 수를 리턴한다.
    /// </summary>
    /// <param name="dir_str"></param>
    /// <returns></returns>
    inline int  SearchDir(const char* dir_str) {
        std::string dir_name = dir_str;
        std::string path = dir_name + "\\*.*";
        int ret = 0;
        //path.append("\\*.*");
        struct _finddata_t fd;//디렉토리 내 파일 및 폴더 정보 저장 객체
        intptr_t handle;
        list<_finddata_t> fd_list;//디렉토리, 파일 정보 객체 리스트

        if ((handle = _findfirst(path.c_str(), &fd)) == -1L) //fd 구조체 초기화.
        {
            //파일이나 디렉토리가 없을 경우.
            TRACE("No file in directory!");
            goto returnOut;
        }

        do //폴더 탐색 반복 시작
        {
            bool is_file = isFile(fd);//현재 객체 종류 확인(파일 or 디렉토리)
            if (is_file == false && fd.name[0] != '.') {
                //디렉토리일 때의 로직        
                ret += SearchDir((dir_name + "\\" + fd.name).c_str());//재귀적 호출
            }
            else if (is_file == true && fd.name[0] != '.') {
                //파일일 때의 로직       
                fd_list.push_back(fd);
            }

        } while (_findnext(handle, &fd) == 0);
        ret += (int)fd_list.size();
    returnOut:
        _findclose(handle);
        return ret;
    }


    inline int  GetFileCount(const char* dir_str, bool inc_dir = false) {
        std::string dir_name = dir_str;
        std::string path = dir_name + "\\*.*";
        int ret = 0;
        struct _finddata_t fd;//디렉토리 내 파일 및 폴더 정보 저장 객체
        intptr_t handle;
        if ((handle = _findfirst(path.c_str(), &fd)) == -1L) {
            //파일이나 디렉토리가 없을 경우.
            TRACE("No file in directory!");
            goto returnOut;
        }

        do //폴더 탐색 반복 시작
        {
            bool is_file = isFile(fd);//현재 객체 종류 확인(파일 or 디렉토리)
            if (is_file == false && fd.name[0] != '.') {
                //디렉토리일 때의 로직        
                if (inc_dir) {
                    ret += GetFileCount((dir_name + "\\" + fd.name).c_str(), inc_dir); //재귀적 호출
                }
            }
            else if (is_file == true && fd.name[0] != '.') {
                //파일일 때의 로직  
                ret++;
            }

        } while (_findnext(handle, &fd) == 0);
    returnOut:
        _findclose(handle);

        return ret;
    }

    /// <summary>
    /// 최하위 디렉토리 인지 확인
    /// </summary>
    /// <param name="dir_str"></param>
    /// <returns></returns>
    inline bool  isLeafDir(const char* dir_str) {
        std::string dir_name = dir_str;
        std::string path = dir_name + "\\*.*";
        struct _finddata_t fd;//디렉토리 내 파일 및 폴더 정보 저장 객체
        intptr_t handle;
        bool ret = false;

        //fd 구조체 초기화.
        if ((handle = _findfirst(path.c_str(), &fd)) == -1L) {
            //파일이나 디렉토리가 없을 경우.
            TRACE("No file in directory!");
            ret = true;
            goto returnOut;
        }

        do //폴더 탐색 반복 시작
        {
            bool is_dir = isDir(fd);
            if (is_dir && fd.name[0] != '.') {
                goto returnOut;
            }
        } while (_findnext(handle, &fd) == 0);
        ret = true;
    returnOut:
        _findclose(handle);
        return ret;
    }


    inline int  DeleteFilesInDir(const char* dir_str, bool inc_dir = false) {
        std::string dir_name = dir_str;
        std::string path = dir_name + "\\*.*";
        int ret = 0;
        struct _finddata_t fd;//디렉토리 내 파일 및 폴더 정보 저장 객체
        intptr_t handle;
        if ((handle = _findfirst(path.c_str(), &fd)) == -1L) {
            //파일이나 디렉토리가 없을 경우.
            TRACE("No file in directory!");
            goto returnOut;
        }

        do //폴더 탐색 반복 시작
        {
            bool is_file = isFile(fd);//현재 객체 종류 확인(파일 or 디렉토리)
            if (is_file == false && fd.name[0] != '.') {
                //디렉토리일 때의 로직        
                if (inc_dir) {
                    ret += DeleteFilesInDir((dir_name + "\\" + fd.name).c_str(), inc_dir); //재귀적 호출
                }
            }
            else if (is_file == true && fd.name[0] != '.') {
                //파일일 때의 로직  
                remove((dir_name + "\\" + fd.name).c_str());
                ret++;
            }

        } while (_findnext(handle, &fd) == 0);
    returnOut:
        _findclose(handle);

        return ret;
    }


    // copy files in directory
    // 1. src dir 파일이 있지 확인/ src 디렉토리 없다면
    // 2. dst dir 없다면 생성하고
    // 3. src file과 동일한 dst file이 있으면 비교해 같으면 생략
    // 4. 하위 디렉토리 동일하게 처리


    /// <summary>
    ///  src 디렉토리를 dest 디렉토리롤 복사 
    /// </summary>
    /// <param name="dst_dir">복사할 위치</param>
    /// <param name="src_dir">복사될 위치</param>
    /// <param name="over_write">덮어쓰기 여부</param>
    /// <param name="inc_dir">하위 폴더 적용 여부</param>
    /// <returns></returns>
    inline errno_t  CopyFilesInDir_binary(const char* dst_dir, const char* src_dir, bool over_write = false, bool inc_dir = false) {
        std::string _src_path = src_dir;
        _src_path += "\\*.*";
        struct _finddata_t fd;
        intptr_t handle;
        FILE* src_file = {}, * dst_file = {};
        errno_t err = FILE_ERR_SUCCESS;

        std::string _dst_path = dst_dir;

        if ((handle = _findfirst(_src_path.c_str(), &fd)) == -1L) {
            //소스 디렉토리에 파일이나 디렉토리가 없을 경우.
            TRACE("No file in directory!");
            err = FILE_ERR_NO_FILE_IN_DIR;
            goto returnOut;
        }

        _src_path = src_dir;
        do //폴더 탐색 반복 시작
        {
            bool is_file = isFile(fd);//현재 객체 종류 확인(파일 or 디렉토리)

            if (fd.name[0] == '.')
                continue;

            if (is_file) {
                // 파일 카피 
                if (FILE_ERR_SUCCESS == (fopen_s(&src_file, (_src_path + "\\" + fd.name).c_str(), "rb"))) {
                    // 파일의 마지막 위치로 이동한다. 
                    fseek(src_file, 0, SEEK_END);
                    // 마지막 위치에서 위치 값을 얻어서 파일의 크기를 계산한다.
                    long pos = ftell(src_file);
                    // 파일의 처음 위치로 복귀한다.
                    char* buff = new char[size_t(pos) + 1];
                    fseek(src_file, 0, SEEK_SET);
                    // 파일의 크기만큼 데이터를 읽는다.
                    fread(buff, pos, 1, src_file);
                    fclose(src_file);

                    if (isDirExists(_dst_path.c_str()) == false) {
                        MakeDir((_dst_path /*+ "\\" + fd.name*/).c_str());
                    }

                    // 같은 파일이 있으면
                    if (isFileExists((_dst_path + "\\" + fd.name).c_str())) {
                        if (over_write) {
                            if (FILE_ERR_SUCCESS == (fopen_s(&dst_file, (_dst_path + "\\" + fd.name).c_str(), "wb"))) {
                                // 대상 파일 만들기
                                fwrite((const void*)buff, sizeof(char), size_t(pos) + 1, dst_file);
                                fclose(dst_file);
                            }
                        }
                    }
                    else {
                        if (FILE_ERR_SUCCESS == (fopen_s(&dst_file, (_dst_path + "\\" + fd.name).c_str(), "wb"))) {
                            // 대상 파일 만들기
                            //fprintf(dst_file, buff);
                            fwrite((const void*)buff, sizeof(char), size_t(pos) + 1, dst_file);
                            fclose(dst_file);
                        }
                    }

                    delete[] buff;
                    buff = nullptr;
                }
            }
            else {
                if (inc_dir) {
                    if (isDirExists(_dst_path.c_str()) == false) {
                        MakeDir((_dst_path /*+ "\\" + fd.name*/).c_str());
                    }

                    CopyFilesInDir_binary((_dst_path + "\\" + fd.name).c_str()
                        , (_src_path + "\\" + fd.name).c_str()
                        , over_write
                        , inc_dir);
                }
            }

        } while (_findnext(handle, &fd) == 0);

    returnOut:
        _findclose(handle);

        return err;
    }

}


namespace timer
{
    class _us
    {
        chrono::system_clock::time_point start_time;
        bool is_start;
    public:
        _us() :start_time({}), is_start(false) {
        }

        virtual ~_us() {
        }

    private:
    public:
        inline void Start() {
            start_time = chrono::system_clock::now();
            is_start = true;
        }

        inline chrono::system_clock::time_point End() {
            return chrono::system_clock::now();
        }

        inline void Stop() {
            is_start = false;
        }

        inline bool IsStarted() {
            return is_start;
        }

        inline uint64_t Elaps(bool is_reset = false) {
            if (!is_start) //not
                return 0;
            chrono::microseconds micro = chrono::duration_cast<chrono::microseconds>(End() - start_time);
            if (is_reset)
                Start();
            return micro.count();
        }

        inline bool LessThan(uint32_t us) {
            if (Elaps() < us)
            {
                Start();
                return true;
            }
            else
                return false;
        }

        inline bool MoreThan(uint32_t us) {
            if (Elaps() > us)
            {
                Start();
                return true;
            }
            else
                return false;
        }
    };



    class _ms
    {
        chrono::system_clock::time_point start_time;
        bool is_start;
    public:
        _ms() :start_time({}), is_start(false) {
        }

        virtual ~_ms() {
        }

    private:
    public:
        inline void Start() {
            start_time = chrono::system_clock::now();
            is_start = true;
        }

        inline chrono::system_clock::time_point End() {
            return chrono::system_clock::now();
        }

        inline void Stop() {
            is_start = false;
        }

        inline bool IsStarted() {
            return is_start;
        }

        inline uint64_t Elaps(bool is_reset = false) {
            if (!is_start) //not
                return 0;
            chrono::milliseconds mill = chrono::duration_cast<chrono::milliseconds>(End() - start_time);
            if (is_reset)
                Start();
            return mill.count();
        }

        /// <summary>
        /// 미만 확인
        /// </summary>
        /// <param name="ms"></param>
        /// <returns>초과 false</returns>
        inline bool LessThan(uint32_t ms) {
            if (Elaps() < ms)
            {
                Start();
                return true;
            }
            else
                return false;
        }

        /// <summary>
        /// 초과 확인
        /// </summary>
        /// <param name="ms"></param>
        /// <returns>초과  true</returns>
        inline bool MoreThan(uint32_t ms) {
            if (Elaps() > ms)
            {
                Start();
                return true;
            }
            else
                return false;
        }
    };

}

namespace util
{
    /// <summary>
    /// 프로세스 시간 측정 (ms)
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    inline uint32_t millis(void) {
        double ret;
        LARGE_INTEGER freq, counter;

        QueryPerformanceCounter(&counter);
        QueryPerformanceFrequency(&freq);

        ret = ((double)counter.QuadPart / (double)freq.QuadPart) * 1000.0;

        return (uint32_t)ret;
    }

    /// <summary>
    /// 프로세스 정밀 시간 측정
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    inline uint32_t micros(void) {
        LARGE_INTEGER freq, counter;
        double ret;

        QueryPerformanceCounter(&counter);
        QueryPerformanceFrequency(&freq);

        ret = ((double)counter.QuadPart / (double)freq.QuadPart) * 1000000.0;

        return (uint32_t)ret;
    }

    /// <summary>
    /// 프로세스 지연
    /// </summary>
    /// <param name="ms">ms</param>
    inline void delay(uint32_t ms) {
        uint32_t pre_time;

        pre_time = millis();
        while (1) {
            if (millis() - pre_time >= ms)
                break;
        }
    }

}



#include <time.h>
#include <sys/timeb.h>
//#include <math.h>

namespace trans
{

#if 0
    char* argv[10] = {};
    char p[100] = "I am a boy";
    char* tok, * next_ptr = NULL;

    uint8_t argc = 0;
    for (tok = strtok_s(p, " ", &next_ptr); tok; tok = strtok_s(NULL, " ", &next_ptr))
    {
        argv[argc++] = tok;
    }
    /*tok = strtok_s(p, " ", &next_ptr);
    while (tok != NULL)
    {
      argc++;
      printf("%s\n", tok);
      tok = strtok_s(NULL, " ", &next_ptr);
    }*/

#endif
    inline uint8_t SplitArgs(char* arg_str, char** argv, char* delim_chars = "/ -:,", int max = 4096) {
        uint8_t argc = 0;
        char* tok = nullptr;
        char* next_ptr = nullptr;

        for (tok = strtok_s(arg_str, delim_chars, &next_ptr); tok; tok = strtok_s(NULL, delim_chars, &next_ptr))
        {
            argv[argc++] = tok;
            if (argc >= max)
                return argc - 1;
        }
        return argc;
    }


    inline void NowStr(char* p_str, int max_len) {

        SYSTEMTIME st;
        GetLocalTime(&st);

        sprintf_s(p_str, max_len, "[%04d/%02d/%02d] %02d:%02d:%02d (%02dmsec)\n"
            , st.wYear
            , st.wMonth
            , st.wDay
            , st.wHour
            , st.wMinute
            , st.wSecond
            , st.wMilliseconds);

    }


    inline void DateFormStr(char* p_str, int length, uint8_t type = 0) {
        time_t timetClock;
        struct tm tmNewTime;

        // Get time in seconds 
        time(&timetClock);
        // Convert time to struct  tm form 
        localtime_s(&tmNewTime, &timetClock);
        switch (type)
        {
        case 1:
        {
            sprintf_s(p_str, length, "%04d-%02d-%02d %02d:%02d:%02d "
                , tmNewTime.tm_year + 1900
                , tmNewTime.tm_mon + 1
                , tmNewTime.tm_mday
                , tmNewTime.tm_hour
                , tmNewTime.tm_min
                , tmNewTime.tm_sec
            );
        }
        break;
        default:
        {
            sprintf_s(p_str, length, "[%04d-%02d-%02d] [%02d:%02d:%02d] "
                , tmNewTime.tm_year + 1900
                , tmNewTime.tm_mon + 1
                , tmNewTime.tm_mday
                , tmNewTime.tm_hour
                , tmNewTime.tm_min
                , tmNewTime.tm_sec
            );
        }
        break;
        }
    }


    //
    // Char → CString
    //
    inline CString CharToCString(char* str) {
        CString cStr;
#if defined(UNICODE) || defined(_UNICODE)
        int nLen = (int)strlen(str) + 1;
        TCHAR* tszTemp = nullptr;
        tszTemp = new TCHAR[nLen];
        memset(tszTemp, 0x00, nLen * sizeof(TCHAR));
        MultiByteToWideChar(
            CP_ACP,
            0,
            str,
            -1,
            tszTemp,
            nLen
        );

        cStr.Format(_T("%s"), tszTemp);
        if (tszTemp)
        {
            delete[] tszTemp;
            tszTemp = NULL;
        }
#else
        cStr.Format("%s", str);
#endif
        return cStr;
    }



    /// <summary>
      /// Char 문자열을 Tchar문자열로 변환
      /// </summary>
      /// <param name="str">Char 문자열</param>
      /// <param name="ret_str">Tchar문자열로 반환주소</param>
      /// <returns>문자 수 </returns>
    inline int CharToLPTSTR(LPSTR str, LPTSTR ret_str, int length) {
        if (strlen(str) == 0)
            return 0;
        if (_tcslen(ret_str) != 0)
            memset(ret_str, 0x00, length);   /* _tcslen(ret_str) * sizeof(TCHAR));*/
        int ret = 0;

#if defined(UNICODE) || defined(_UNICODE)
        //int nLen = (int)_tcslen(ret_str) * (int)sizeof(TCHAR);
        ret = MultiByteToWideChar(
            CP_ACP,
            MB_PRECOMPOSED,
            str,
            (int)strlen(str),
            ret_str,
            length
        );
#else
        int nLen = strlen(str) + 1;
        _tcscpy(ret_str, str);
        ret = nLen;
#endif
        return ret;
    }


    //
    // TCHAR → CString
    //
    inline CString TcharToCString(TCHAR* str) {
        CString cStr;
        cStr.Format(_T("%s"), str);
        return cStr;
    }


    /// <summary>
    /// TCHAR 문자열 → Char 문자열
    /// </summary>
    /// <param name="str"></param>
    /// <param name="ret_str"></param>
    /// <returns></returns>
    inline int TcharToLPSTR(LPTSTR str, LPSTR ret_str, int length) {
        int ret = 0;
        if (_tcslen(str) == 0)
            return 0;
        if (length != 0)
            memset(ret_str, 0x00, length * sizeof(CHAR));

#if defined(UNICODE) || defined(_UNICODE)
        ret = WideCharToMultiByte(
            CP_ACP,
            0,
            str,
            -1,
            ret_str,
            length + 1,
            NULL,
            NULL
        );
#else
        strcpy(ret_str, str);

#endif
        return ret;
    }

}
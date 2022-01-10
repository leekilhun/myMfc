#pragma once
#include "pch.h"
#include <functional>
#include <direct.h> 
#include "virtualRS.h"

#define	ERR_DATA_FORMAT			-1
#define	ERR_NULL_POINTER		-2
#define	ERR_DEVICE_DRIVER		-3
#define	ERR_RECEIVED_DATA		-4
#define ERR_COMMUNICATION		-5
#define ERR_TX_TIMEOUT			-6
#define ERR_RX_TIMEOUT			-7


const	int	ERR_IO_SUCCESS = 0;
const	int	ERR_IO_UPDATE_FAIL = 1;
const	int	ERR_IO_MODULE_FAIL = 2;
const	int	ERR_IO_DRIVER_FAIL = 3;
const	int	ERR_IO_INVALID_ADDRESS = 4;

const int	INPUT_ADDRESS       = 0;
const int	OUTPUT_ADDRESS      = 1;


const int	INPUT_MAX_ADDRESS   = 1000;
const int	OUTPUT_MAX_ADDRESS  = 1000;

const int	MAX_DEVICE = 312;

#define IO_VR_MAX_ROWS 2000
#define IO_ADDR_HEAD_INPUT 1
#define IO_ADDR_HEAD_OUTPUT 3

#define IO_ADDR_BASE_INPUT 10000
#define IO_ADDR_INPUT 0
#define IO_ADDR_MAX_INPUT 1000

#define IO_ADDR_BASE_OUTPUT 30000
#define IO_ADDR_OUTPUT 1
#define IO_ADDR_MAX_OUTPUT 1000

#define IO_ADDR_TOTAL_MAX_CNT  IO_ADDR_BASE_OUTPUT + IO_ADDR_MAX_OUTPUT

#define IO_TITLE_MAX_CHART  28




class Table_IO : public virtualRS
{
  /****************************************************
   *	data
   ****************************************************/
public:
  typedef int(*Callback_t)(void* obj);//typedef int(*cb_t)(void);
  /*struct Data_t {
    int head;
    int value;
  };*/

private:
  int CountInput;
  int CountOutput;
public:
  TCHAR* FileInfo;
  TCHAR* Name;
  TCHAR* Path;
  CWinThread* Thread;			// Thread Handle
  bool TrdLife;
  Callback_t CallFunc;
  //std::function<int()> Func;
  void* CallFuncObj;

  //Table_IO::Data_t IN_TITLE[IO_VR_MAX_ROWS];
  TCHAR* IN_TITLE[IO_ADDR_MAX_INPUT];
  TCHAR* OUT_TITLE[IO_ADDR_MAX_OUTPUT];
  /****************************************************
   *	Constructor
   ****************************************************/
public:
  /*Table_IO(LPCTSTR file_info = L"./io.dat") : virtualRS(IO_VR_MAX_ROWS)
    , FileInfo(nullptr), Name(nullptr), Path(nullptr) 
    , CountInput(0), CountOutput(0), Thread(nullptr), TrdLife(false)
    , CallFunc(nullptr), CallFuncObj(nullptr){
    int size = (int)_tcslen(file_info) + 1;
    FileInfo = new TCHAR[size];
    memset(FileInfo, 0x00, size * sizeof(TCHAR));
    _tcsncat_s(FileInfo, size, file_info, 256);
  }*/

  Table_IO(LPCTSTR file_info = L"./io.dat") : virtualRS(IO_VR_MAX_ROWS)
    , FileInfo(nullptr), Name(nullptr), Path(nullptr)
    , CountInput(0), CountOutput(0), Thread(nullptr), TrdLife(false)
    , CallFunc(nullptr), CallFuncObj(nullptr) {
    int size = (int)_tcslen(file_info) + 1;
    FileInfo = new TCHAR[size];
    memset(FileInfo, 0x00, size * sizeof(TCHAR));
    _tcsncat_s(FileInfo, size, file_info, 256);
  };
  virtual ~Table_IO() {
    if (Thread != nullptr) {
      delete Thread;
    }
    Thread = nullptr;


    if (FileInfo != nullptr) {
      delete FileInfo;
    }
    if (Name != nullptr) {
      delete Name;
    }
    if (Path != nullptr) {
      delete Path;
    }    
    FileInfo = nullptr;
    Name = nullptr;
    Path = nullptr;

    for (int i = 0; i < IO_ADDR_MAX_OUTPUT; i++)
    {
      if (OUT_TITLE[i] != nullptr)
      {
        delete OUT_TITLE[i];
      }
      OUT_TITLE[i] = nullptr;
    }
    for (int i = 0; i < IO_ADDR_MAX_INPUT; i++)
    {
      if (IN_TITLE[i] != nullptr)
      {
        delete IN_TITLE[i];
      }
      IN_TITLE[i] = nullptr;
    }
    
  };

#if 0
  Table_IO(const Table_IO& origin) : virtualRS(IO_VR_MAX_ROWS)
    , FileInfo(nullptr), Name(nullptr), Path(nullptr) , CountInput(0), CountOutput(0) {
    int size = (int)_tcslen(origin.FileInfo) + 1;
    this->FileInfo = new TCHAR[size];
    memset(this->FileInfo, 0x00, size * sizeof(TCHAR));
    _tcsncat_s(this->FileInfo, size, origin.FileInfo, 256);

    size = (int)_tcslen(origin.Name) + 1;
    this->Name = new TCHAR[size];
    memset(this->Name, 0x00, size * sizeof(TCHAR));
    _tcsncat_s(this->Name, size, origin.Name, 256);

    size = (int)_tcslen(origin.Path) + 1;
    this->Path = new TCHAR[size];
    memset(this->Path, 0x00, size * sizeof(TCHAR));
    _tcsncat_s(this->Path, size, origin.Path, 256);

    this->CountInput = origin.CountInput;
    this->CountOutput = origin.CountOutput;
    //this->Init();
    //this->str = create(buffer_length);
    //this->buffer_length = origin.buffer_length;
    //this->str = SetString(origin.str);
  }
#endif

  /****************************************************
   *	func
   ****************************************************/

private:
  void threadStop(void) {
    DWORD exitcode;
    TrdLife = false;
    //Sleep(100);
    if (Thread != nullptr)   {
      exitcode = ::WaitForSingleObject(Thread->m_hThread, 3000);
      if (exitcode == WAIT_TIMEOUT) {
#pragma warning(suppress: 6258)
        ::TerminateThread(Thread->m_hThread, exitcode);
      }
      Thread = nullptr;
    }
  }

  void threadRun(void) {
    if (TrdLife)  {
      threadStop();
      Sleep(10);
    }
    TrdLife = true;
    Thread = AfxBeginThread(threadFunction, this);
  }

  static UINT	threadFunction(LPVOID pParam) {
    Table_IO* pThis = (Table_IO*)pParam;
    while (pThis->TrdLife)  {
      pThis->threadJob();
      Sleep(10);
    }
    return 0;
  }

  void threadJob(void) {
    //this->ioReadFile();
    if (this->exRead() == 0) {
      if (CallFunc)
      {
        (*CallFunc)(CallFuncObj);
      }
    }
    if (TrdLife) {
    threadStop();
      Sleep(10);
    }
  }
  void doRunStep(void) {
  }
  
private:
  string getFileInfo() {
    char temp[1024] = { 0, };
    vrTcharToLPSTR(this->FileInfo, &temp[0], 1024);
    string file_loc = temp;
    return file_loc;
  }

  inline int constrain(int amt, int low, int high) {
    return ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)));
  }

  inline bool between(int amt, int low, int high) {
    bool ret = false;
    if (amt >= low)
    {
      if (amt <= high)
      {
        ret = true;
      }
    }
    return ret;
  }


public: 

  int ioReadFileTrd() {
    threadRun();
    return 0;
  }

  void ioSetCallback(void* obj, Callback_t cb) {
    //Func = bind(cb);
    CallFunc = cb;
    CallFuncObj = obj;
  }

  int ioGetRSCount(){
    return virtualRS::GetEndCursor();
  }

  int ioGetInputCount() const{
    return CountInput;
  }
  int ioGetOutputCount() const{
    return CountOutput;
  }

  int ioWriteFile() {
    string str_table = write_form();

    /* 폴더가 있는지 확인해서 없으면 생성 할지, 말지*/

    std::ofstream out(this->getFileInfo());
    out << str_table;
    return 0;
  }



  int exRead() {

    enum class parsingState {
      none,
      section,
      item,
      value,
    };

#if 1
    performanceTimer timer, timer_parsing, timer_reg;
    timer.StartTimer();
    TRACE("IIO exRead Start : [%7.3f] \n\r", timer.GetElaspTime());
    timer.StartTimer();

    char buffer[_MAX_PATH];
    //FILE* InFile;
    int numclosed;
    int ch;
    parsingState  state = parsingState::none;

    char type_str[IO_TITLE_MAX_CHART] = { 0, };
    char name_str[IO_TITLE_MAX_CHART] = { 0, };
    char addr_str[IO_TITLE_MAX_CHART] = { 0, };

    CString	FormattedMessage;
    CString	Type;
    CString	Name;
    CString	Address;
    static int	inputAddressCount = 0;
    static int	outputAddressCount = 0;

    memset(IN_TITLE, 0x00, sizeof(IN_TITLE));
    memset(OUT_TITLE, 0x00, sizeof(OUT_TITLE));


    /* Get the current working directory: */
    if (_getcwd(buffer, _MAX_PATH) == NULL)
      perror("_getcwd error");
    else
      printf("%s\n", buffer);

    // IO Map File Name
    //strFileName.Format("%s\\Info\\%s", buffer, FileName);

    FILE* p_file = nullptr;
    char file_info[256];
    vrTcharToLPSTR(this->FileInfo, &file_info[0], 256);
    /* Open for read (will fail if file "data" does not exist) */
    fopen_s(&p_file, /*"D:\\_MyPGM\\cylinder\\cylinder\\IO.dat"*/file_info, "r");
    if (p_file == nullptr)
    {
      AfxMessageBox(L"The file 'data' was not opened!");
      return -1;
    }
    else
      printf("The file 'data' was opened\n");

    TRACE("IIO exRead Open file : [%7.3f] \n\r", timer.GetElaspTime());
    timer.StartTimer();

    memset(buffer, 0x00, sizeof(buffer));

    while (!feof(p_file))
    {

      //	fscanf(InFile,"%s\n",buffer);
      
      //fread( buffer, sizeof( char ), 80, InFile );

      ch = fgetc(p_file);
      int i = 0;
      //for (int i = 0; (ch != '\n') & (feof(p_file) == 0); i++)
      if (feof(p_file)) //파일의 끝을 지나서 읽으려고 시도 하는 경우 feof 함수는 0이 아닌 값을 반환
        break;
      if (ch == '\n')
        goto gotoNextLine;

      while(true)
      {
        bool is_endoffile = feof(p_file) != 0;
        bool is_continue = (ch != '\n') & !(is_endoffile);
        if (!is_continue)
          break;

        if (i > 0)
        {
          switch (ch)
          {
          case '\0': goto gotoNextLine; break;
          case '\n': goto gotoNextLine; break;
          case ' ':
          {
            if (buffer[i - 1] == ' ')
              goto gotoNextLine;
          }
          break;
          case '#':
          {
            if (buffer[i - 1] == '#')
              goto gotoNextLine;
          }
          break;
          case '/':
          {
            if (buffer[i - 1] == '/')
              goto gotoNextLine;
          }
          break;
          default:
          {
            if (ch == '\t')
              goto gotoNextChar;

            if (state == parsingState::item)
            {
              int i = 1;
              addr_str[0] = buffer[0];
              bool is_pass = false;
              while (true)
              {
                if (ch == '\n' || is_pass)
                  break;
                bool is_num = between(ch, 48, 57);
                if (is_num)
                {
                  addr_str[i] = ch;
                }
                else
                  is_pass = true;
                
                ch = fgetc(p_file);
                i++;
              }

              if (strlen(buffer) < 1)
                goto gotoNextLine;
              bool is_input = strncmp(type_str, "INPUT", 5) == 0;
              if (is_input)
              {
                int size = (int)strlen(name_str)+1;
                IN_TITLE[CountInput] = new TCHAR[size];
                memset(IN_TITLE[CountInput], 0x00, size * sizeof(TCHAR));

                vrCharToLPTSTR(name_str, IN_TITLE[CountInput], size);
                CountInput++;
              }
              else 
              {
                int size = (int)strlen(name_str)+1;
                OUT_TITLE[CountOutput] = new TCHAR[size];
                memset(OUT_TITLE[CountOutput], 0x00, size * sizeof(TCHAR));

                vrCharToLPTSTR(name_str, OUT_TITLE[CountOutput], size);
                CountOutput++;

              } //if (Type.Compare(L"INPUT") == 0)


              memset(type_str, 0x00, sizeof(type_str));
              memset(name_str, 0x00, sizeof(name_str));
              memset(addr_str, 0x00, sizeof(addr_str));

              memset(buffer, 0x00, sizeof(buffer));

              state = parsingState::none;
              goto gotoNextLine;
            }

            if (ch == ',')
            {
              if (strlen(buffer) > 0)
              {
                switch (state)
                {
                case parsingState::none:
                {
                  int copy_size =(int)(IO_TITLE_MAX_CHART > (strlen(buffer)) ? (strlen(buffer) + 1) : IO_TITLE_MAX_CHART -1);
                  strncpy_s(&type_str[0], IO_TITLE_MAX_CHART, &buffer[0], copy_size);
                  //strcpy_s(&type_str[0], strlen(buffer)+1, &buffer[0]);
                  state = parsingState::section;
                  memset(buffer, 0x00, sizeof(buffer));
                  i = 0;
                  goto gotoNextChar;
                }
                break;
                case parsingState::section:
                {
                  int copy_size = (int)(IO_TITLE_MAX_CHART > (strlen(buffer)) ? (strlen(buffer) + 1) : IO_TITLE_MAX_CHART -1);
                  strncpy_s(&name_str[0], IO_TITLE_MAX_CHART, &buffer[0], copy_size);
                  //strcpy_s(&name_str[0], strlen(buffer)+1, &buffer[0]);
                  state = parsingState::item;
                  memset(buffer, 0x00, sizeof(buffer));
                  i = 0;
                  goto gotoNextChar;
                }
                break;
                case parsingState::item:
                {
                  
                }
                break;
                case parsingState::value:;
                  break;
                } //switch (currState)
              }
            }
          }
          } //switch (ch)
        }
        else
        {
          // 줄 첫머리 글자 확인
          switch (ch)
          {
          case '\0':  goto gotoNextLine; break;
          case '\n':  goto gotoNextLine; break;
          case ' ':   goto gotoNextChar; break;
          case '\t':  goto gotoNextChar; break;
          } //switch (ch)
        }      
        buffer[i] = (char)ch;
        i++;
      gotoNextChar:
        ch = fgetc(p_file);
        
      } // for (int i = 0; (ch != '\n') & (feof(p_file) == 0); i++)

#if 0
      FormattedMessage = buffer;
      FormattedMessage.TrimLeft();
      FormattedMessage.TrimRight();

      int n = FormattedMessage.Find(';', 0);
      if (n != -1)
        continue;
      else
      {
        int iType = FormattedMessage.Find(',', 0);
        Type = FormattedMessage.Left(iType);
        Type.TrimLeft();
        Type.TrimRight();

        int iName = FormattedMessage.Find(',', iType + 1);

        Name = FormattedMessage.Mid(iType + 1, iName - iType - 1);
        Name.TrimLeft();
        Name.TrimRight();


        Address = FormattedMessage.Mid(iName + 1);
        Address.TrimLeft();
        Address.TrimRight();

        if (Type.Compare(L"INPUT") == 0)
        {
          int size = Name.GetLength() + 1;
          IN_TITLE[CountInput] = new TCHAR[size];
          memset(IN_TITLE[CountInput], 0x00, size * sizeof(TCHAR));
          _tcscpy_s(IN_TITLE[CountInput], size, Name.GetBuffer());
          Name.ReleaseBuffer();
          CountInput++;
        }
        else if (Type.Compare(L"OUTPUT") == 0)
        {
          int size = Name.GetLength() + 1;
          OUT_TITLE[CountOutput] = new TCHAR[size];
          memset(OUT_TITLE[CountOutput], 0x00, size * sizeof(TCHAR));
          _tcscpy_s(OUT_TITLE[CountOutput], size, Name.GetBuffer());
          Name.ReleaseBuffer();
          CountOutput++;
        } //if (Type.Compare(L"INPUT") == 0)
      }//if (n != -1)
#endif

    gotoNextLine:
      while (true)
      {
        if (ch == '\n')
          break;
        ch = fgetc(p_file);
      }
      state = parsingState::none;
      memset(buffer, 0x00, sizeof(buffer));
    }
  
    TRACE("IIO exRead parsing end : [%7.3f] \n\r", timer.GetElaspTime());
    /* Close stream */
    if (fclose(p_file))
      printf("The file 'data' was not closed\n");

    /* All other files are closed: */
    numclosed = _fcloseall();
    printf("Number of files closed by _fcloseall: %u\n", numclosed);
#endif // 0

    return 0;
  }
  int ioReadFile() {
    enum class parsingState {
      none,
      section,
      item,
      value,
    };

    performanceTimer timer,timer_parsing,timer_reg;
    timer.StartTimer();
    TRACE("IIO Time Start : 0 [%7.3f] \n\r", timer.GetElaspTime());
    timer.StartTimer();

    std::ifstream in(this->getFileInfo());
    TRACE("IIO Time ifstream in : 0 [%7.3f] \n\r", timer.GetElaspTime());
    timer.StartTimer();
    std::string s = "";
    std::string str[3];
    parsingState currState = parsingState::none;
    char buf[256] = { 0, };
    char strbuf[8] = { 0, };
    char* ptr = nullptr;
    int next_idx;
    int curr_idx = 0;
    bool is_input_sec = false;
    if (!in.is_open()) {
      AfxMessageBox(L"파일을 찾을 수 없습니다!");
    }
    
    int count = 0;

    double time_max = .0;
    double time_min = .0;
    double time_sum = .0;
    double time_avr = .0;
    double time_cost = .0;
    double time_reg = .0;
    timer_parsing.StartTimer();
    while (in)
    {
      ptr = &buf[0];
      in.getline(ptr, 256);
      if (strnlen_s(ptr, 256) < 1)
        continue;
      s = "";
      next_idx = 1;
      curr_idx = 0;
      //bool is_front = true;
      //bool is_value = false;

      if (currState == parsingState::value) {
        timer_reg.StartTimer();
        //TRACE("IIO reg Start :  [%7.6f] \n\r", timer_reg.GetElaspTime());

        is_input_sec = (str[COLS_CAST(section)].compare("INPUT") == 0);
        if (is_input_sec) {
          int size = (int)str[COLS_CAST(item)].length() + 1;
          IN_TITLE[CountInput] = new TCHAR[size];
          memset(IN_TITLE[CountInput], 0x00, size * sizeof(TCHAR));
          char tmp[IO_TITLE_MAX_CHART] = { 0, };
          if (size < IO_TITLE_MAX_CHART == false)
            size = IO_TITLE_MAX_CHART;
          for (int i = 0; i < size; i++)
            tmp[i] = str[COLS_CAST(item)][i];
          vrCharToLPTSTR(tmp, IN_TITLE[CountInput], size);
          CountInput++;
        }
        else {
          //IN_TITLE[CountOutput++] = new TCHAR[];
          int size = (int)str[COLS_CAST(item)].length() + 1;
          OUT_TITLE[CountOutput] = new TCHAR[size];
          memset(OUT_TITLE[CountOutput], 0x00, size * sizeof(TCHAR));
          char tmp[IO_TITLE_MAX_CHART] = { 0, };
          if (size < IO_TITLE_MAX_CHART == false)
            size = IO_TITLE_MAX_CHART;
          for (int i = 0; i < size; i++)
            tmp[i] = str[COLS_CAST(item)][i];
          vrCharToLPTSTR(tmp, OUT_TITLE[CountOutput], size);
          CountOutput++;
        }

        memset(&strbuf[0], 0x00, 8);
        //strbuf[8];


        int cursor_no = regRecord(&str[0], 3);
        time_reg = timer_reg.GetElaspTime();
        //TRACE("IIO reg End :  [%7.6f] \n\r", time_reg);
        timer_parsing.StartTimer();

        str[COLS_CAST(item)] = "";
        str[COLS_CAST(value)] = "";
      }
      currState = parsingState::none;
      while (ptr[curr_idx] != '\0')
      {
#if 1
        memcpy(&strbuf[0], ptr, 8);      
        
        switch (strbuf[0])
        {
        case '\0':
        {
          goto gotoBreak;
        }
        case ' ':
        {
          if (strbuf[1] == ' ')
            goto gotoBreak;
        }
        break;
        case '#':
        {
          if (strbuf[1] == '#')
            goto gotoBreak;
        }
        break;
        case '/':
        {
          if (strbuf[1] == '/')
            goto gotoBreak;
        }
        break;
        default:
        {
          for (int i = 0; i < 8; i++)
          {
            if (strbuf[i] == '\0')
              goto gotoBreak;
            if (strbuf[i] == ',')
            {
              switch (currState)
              {
              case parsingState::none: ;
              break;
              case parsingState::section: 
              {
                currState = parsingState::item;
                str[COLS_CAST(item)] = "";
              }
              break;
              case parsingState::item:
              {
                currState = parsingState::value;
                str[COLS_CAST(value)] = "";
              }
              break;
              case parsingState::value:;
              break;
              } //switch (currState)


              next_idx++;
              //ptr++;
              curr_idx++;
              i++;
            }
            
            if (i < 7) 
            {
              switch (strbuf[i])
              {
              case '\0':
              {
                goto gotoBreak;
              }
              case ' ':
              {
                if (strbuf[i + 1] == ' ')
                  goto gotoBreak;
              }
              break;
              case '#':
              {
                if (strbuf[i + 1] == '#')
                  goto gotoBreak;
              }
              break;
              case '/':
              {
                if (strbuf[i + 1] == '/')
                  goto gotoBreak;
              }
              break;
              }//switch (strbuf[i])
            }
            



            if (strbuf[i] != '\t')
            {
              switch (currState)
              {
              case parsingState::none:
              {
                str[COLS_CAST(section)] = "";
                str[COLS_CAST(section)].push_back(strbuf[i]);
                currState = parsingState::section; //
              }
              break;
              case parsingState::section:
              {
                str[COLS_CAST(section)].push_back(strbuf[i]);
              }
              break;
              case parsingState::item:
              {
                str[COLS_CAST(item)].push_back(strbuf[i]);
              }
              break;
              case parsingState::value:
              {
                str[COLS_CAST(value)].push_back(strbuf[i]);
              }
              break;
              }//switch (currState)
            }
            next_idx++;
            curr_idx++;
            //ptr++;
          } //for (int i = 0; i < 8; i++)
        }
        break;
        }//switch (strbuf[0])
#endif
        
        if (ptr[curr_idx] == '#' && buf[next_idx] == '#') // '##'- 줄 생략
          break;

        if (ptr[curr_idx] == '/' && buf[next_idx] == '/') // '//' 줄 생략
          break;

        if (ptr[curr_idx] == ' ' && buf[next_idx] == ' ')
          break;
        
        if (ptr[curr_idx] == ' ') // 빈칸 삭제
          goto gotoPass;;

        if (ptr[curr_idx] == '\t') // '\t' 탭 삭제
          goto gotoPass;


        switch (currState)
        {
        case parsingState::none:
        {
          str[COLS_CAST(section)] = "";
          str[COLS_CAST(section)].push_back(ptr[curr_idx]);

          currState = parsingState::section; //
          next_idx++;
          curr_idx++;
          continue;
        }
        break;
        case parsingState::section:
        {
          if (ptr[curr_idx] == ',') {
            currState = parsingState::item;
            str[COLS_CAST(item)] = "";   
            next_idx++;
            curr_idx++;
            continue;
          }
          else {
            str[COLS_CAST(section)].push_back(ptr[curr_idx]);
          }

        }
        break;
        case parsingState::item:
        {
          if (ptr[curr_idx] == ',') {
            currState = parsingState::value;
            str[COLS_CAST(value)] = "";
            next_idx++;
            curr_idx++;
            continue;
          }
          else {
            str[COLS_CAST(item)].push_back(ptr[curr_idx]);
          }

        }
        break;
        case parsingState::value:
        {
          str[COLS_CAST(value)].push_back(ptr[curr_idx]);
        }
        break;
        } //switch (currState)

      gotoPass:
        next_idx++;
        curr_idx++;
      }
    gotoBreak:
      count++;

      time_cost = timer_parsing.GetElaspTime();
      time_cost > time_max ? time_max = time_cost : time_max = time_max;
      time_cost < time_min ? time_min = time_cost : time_min = time_min;
      time_sum += time_cost;
      timer_parsing.StartTimer();
    }//while(in)
    
    if (currState == parsingState::value) {

      is_input_sec = (str[COLS_CAST(section)].compare("INPUT") == 0);
      if (is_input_sec)
      {
        int size = (int)str[COLS_CAST(item)].length() + 1;
        IN_TITLE[CountInput] = new TCHAR[size];
        memset(IN_TITLE[CountInput], 0x00, size * sizeof(TCHAR));
        char tmp[IO_TITLE_MAX_CHART] = { 0, };
        if (size < IO_TITLE_MAX_CHART == false)
          size = IO_TITLE_MAX_CHART;
        for (int i = 0; i < size; i++)
          tmp[i] = str[COLS_CAST(item)][i];
        vrCharToLPTSTR(tmp, IN_TITLE[CountInput], size);
        CountInput++;
      }
      else
      {
        //IN_TITLE[CountOutput++] = new TCHAR[];
        int size = (int)str[COLS_CAST(item)].length() + 1;
        OUT_TITLE[CountOutput] = new TCHAR[size];
        memset(OUT_TITLE[CountOutput], 0x00, size * sizeof(TCHAR));
        char tmp[IO_TITLE_MAX_CHART] = { 0, };
        if (size < IO_TITLE_MAX_CHART == false)
          size = IO_TITLE_MAX_CHART;
        for (int i = 0; i < size; i++)
          tmp[i] = str[COLS_CAST(item)][i];
        vrCharToLPTSTR(tmp, OUT_TITLE[CountOutput], size);
        CountOutput++;
      }

      int cursor_no = regRecord(&str[0], 3);
      str[COLS_CAST(item)] = "";
      str[COLS_CAST(value)] = "";
    }

    TRACE("IIO Time ifstream end : 0 [%7.3f] \n\r", timer.GetElaspTime());
    TRACE("time max[%7.3f], time min [%7.3f], time avg [%7.3f] \n\r", time_max,time_min, time_sum/ count);
    timer.StartTimer();
    in.close();
    string strTable = write_form();
    TRACE("IIO Time ifstream write form : 0 [%7.3f] \n\r", timer.GetElaspTime());
    //timer.StartTimer();
  return count;
  }

  inline CString ioGetInItem(int value) {
    CString  ret;
    int cursor = GetCursorLoc("INPUT", value);
    if (cursor < 0)
      return CString();

    GetItem(cursor, ret.GetBuffer(256), 256);
    ret.ReleaseBuffer();
    return ret;
  }

  inline void ioGetInItem(int value, TCHAR* str, int length) {
    CString  ret;
    int cursor = GetCursorLoc("INPUT", value);
    if (cursor < 0)
      return ;

    GetItem(cursor, str, length);
  }


  inline CString ioGetOutItem(int value) {
    CString  ret;
    int cursor = GetCursorLoc("OUTPUT", value);
    GetItem(cursor, ret.GetBuffer(256), 256);
    ret.ReleaseBuffer();
    return ret;
  }

  inline void ioGetOutItem(int value, TCHAR* str, int length) {
    CString  ret;
    int cursor = GetCursorLoc("OUTPUT", value);
    if (cursor < 0)
      return;

    GetItem(cursor, str, length);
  }

};










#if 0
class Table_IO 
{
public:
  Table_IO(LPCTSTR name) :
    Name(nullptr), Address(0), Index(0), Type(0){
    int size = (int)_tcslen(name) + 1;
    Name = new TCHAR[size];
    memset(Name, 0x00, size * sizeof(TCHAR));
    _tcsncat_s(Name, size, name, 256);
  };
  virtual ~Table_IO() { 
    if (Name != nullptr) {
      delete Name;
    }
    Name = nullptr;
  };

public:
  TCHAR*	Name;
  int		Address;
  int		Index;
  int		Type;
};
#endif

class IIO
{

public:
  virtual ~IIO() {}  // 가상 소멸자의 선언

public:
 /****************************************************
	*	func
	****************************************************/
  
  /// <summary>
  /// 
  /// </summary>
  /// <param name="input"></param>
  /// <param name="output"></param>
  /// <returns></returns>
  virtual Table_IO*	GetIOTable() = 0;

  /// <summary>
  /// 
  /// </summary>
  /// <param name="file_name"></param>
  /// <returns></returns>
  virtual int LoadIOMap() = 0;

  /// <summary>
  /// Board와의 Communication을 위한 Driver를 Open하며, Board를 초기화하고 통신 대기 상태가 되게 한다.
  /// </summary>
  /// <returns>0 = Success, 그외 = Error Number</returns>
  virtual int Initialize() = 0;
  
  /// <summary>
  /// 
  /// </summary>
  virtual void RunIOThread() = 0;


  /// <summary>
  /// Board와의 Communication을 종료하고 Device Driver를 Close한다
  /// </summary>
  /// <returns></returns>
  virtual int Terminate() = 0;


  /// <summary>
  /// I/O Device의 Digital Status (Bit) 를 읽어들여 리턴한다.
  /// </summary>
  /// <param name="addr"></param>
  /// <returns>TRUE : ON , FALSE : OFF </returns>
  virtual BOOL IsOn(unsigned short addr) = 0;

  /// <summary>
  /// I/O Device의 Digital Status (Bit) 를 읽어들여 리턴한다.
  /// </summary>
  /// <param name="addr">IO Address</param>
  /// <returns>TRUE : OFF , FALSE : ON</returns>
  virtual BOOL IsOff(unsigned short addr) = 0;

    /**
     * Output Device에 On Command (Bit = 1) 를 보낸다.
     * @precondition 이 함수를 실행하기 전에 initialize 함수가 미리 실행되었어야 한다.
     * @param usIOAddr : IO Address
     * @return 0 = Success, 그외 = Error Number
     */

  /// <summary>
  /// Output Device에 On Command (Bit = 1) 를 보낸다.
  /// </summary>
  /// <param name="addr">IO Address</param>
  /// <returns>0 = Success, 그외 = Error Number</returns>
  virtual int OutputOn(unsigned short addr) = 0;

  
  /// <summary>
  /// Output Device에 Off Command (Bit = 0) 를 보낸다.
  /// </summary>
  /// <param name="addr">IO Address</param>
  /// <returns>0 = Success, 그외 = Error Number</returns>
  virtual int OutputOff(unsigned short addr) = 0;


  /// <summary>
  /// Output Device의 Digital Status를 Toggle한다
  /// </summary>
  /// <param name="addr">IO Address</param>
  /// <returns> 0 = Success, 그외 = Error Number</returns>
  virtual int OutputToggle(unsigned short addr) = 0;


  //virtual int GetByte(unsigned short addr, BYTE& pcValue) = 0;

  //virtual int PutByte(unsigned short addr, BYTE pcValue) = 0;

  //virtual int GetWord(unsigned short addr, WORD& pwValue) = 0;

  //virtual int PutWord(unsigned short addr, WORD pwValue) = 0;

  //virtual int GetDWord(unsigned short addr, DWORD& pdwValue) = 0;

  //virtual int PutDWordt(unsigned short addr, DWORD pdwValue) = 0;

  //virtual int Getbufferdata(int idCnt, int addr) = 0;

  //virtual void Setbufferdata(int idCnt, int addr, int value) = 0;
};


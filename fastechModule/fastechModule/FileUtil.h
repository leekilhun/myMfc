#pragma once

#include <vector>


// Error Code Define
#define E_FU_FILE_NOT_FOUND	-1
#define E_FU_FILE_COPY_FAIL	-2
#define E_FU_NOFILE_IN_DIR	-3


/**
 * File 삭제,복사, 갯수 구하기 등의 Uitlity들을 제공하는 Class이다.
 */


class MFileUtil
{

public:

	/**
		* MFileUtil의 생성자
		*/
	MFileUtil() {  }			// 생성자

	/**
		* MFileUtil의 소멸자
		*/
	virtual ~MFileUtil() {}	// 소멸자

	/**
		* 지정한 Path가 디렉토리를 포함하지 않은 최하위 디렉토리인지 확인한다.
		*
		* @Param		pDir : 확인할 디렉토리 Path
		* @return	    TRUE : 다른 디렉토리 포함 하지 않음, FALSE : 다른 디렉토리 포함
		*/
	BOOL	IsLeafDir(LPCTSTR pDir);

	/**
		* 디렉토리내 파일들을 삭제한다.
		* 디렉토리내 하위디렉토리는 삭제하지 않는다.
		* Progress Bar를 뛰어서 진행 경과를 표시한다.
		*
		* @Param		pDir : 삭제할 디렉토리 Path
		* @return		TRUE/FALSE
		*/
	int		DeleteFilesInDir(LPCTSTR pDir);

	/**
		* 디렉토리에 존재하는 파일의 갯수를 구한다.
		*
		* @Param		pDir      : 디렉토리명
		* @return		파일의 갯수
		*/
	int		GetFileCountInDir(LPCTSTR pDir);

	/**
		* 디렉토리내 하위 디렉토리의 갯수를 구한다.
		*
		* @Param		pDir : 삭제할 디렉토리 Path
		* @return		하위 디렉토리의 갯수
		*/
	int		GetDirCountInDir(LPCTSTR pDir);

	/**
		* 디렉토리의 파일들을 복사한다.
		*
		* @Param		pDest      : 목적지 디렉토리
		* @Param		pSrc       : 원본 디렉토리
		* @Param		bOverWrite : OverWrite 여부 (TRUE/FALSSE)
		* @return		하위 디렉토리의 갯수
		*/
	int		CopyFilesInDir(LPCTSTR pDest, LPCTSTR pSrc, BOOL bOverWrite);


	/**
		* 디렉토리에 파일이 존재 하는지 검사한다.
		*
		* @Param		pFile      : 검색하고자 하는 파일명 (Path 포함)
		* @return		TRUE / FALSE
		*/
	BOOL	IsFileExist(LPCTSTR pFile);

	/**
		* 디렉토리내 디렉토리의 리스트를 차즌다.
		*
		* @Param		pDir      : 디렉토리명
		* @Param		strDirname[] : string의 배열
		* @Param		iCount		: 갯수
		* @return		SUCCESS / ERROR CODE
		*/
	int		MFileUtil::GetDirListInDir(LPCTSTR pDir, CString strDirName[], int iCount);


	/**
		* 디렉토리내 디렉토리의 리스트를 차즌다.
		*
		* @Param		pDir      : 디렉토리명
		* @Param		strDirname[] : string의 배열
		* @Param		iCount		: 갯수
		* @return		SUCCESS / ERROR CODE
		*/
	int		MFileUtil::GetDirListInDir(LPCTSTR pDir, CString strDirName[], int* iCount);

	int GetDirListInDir(LPCTSTR pDir, std::vector<CString>& strDirName);

	/**
		* 존재하지 않는 디렉토리의 경우 새로 생성한다.
		*
		* @author		정종철
		* @param		pDir : Path (파일 포함 또는 미포함)
		* @param		bIsIncludeFile : pDir에 파일을 포함 하는가 ?, Default TRUE
		* @return		TRUE : 성공,  FALSE : 실패
		* @see
		* @version
		*/
	BOOL MFileUtil::MakeDir(LPCTSTR pDir, BOOL bIsIncludeFile = TRUE);


	BOOL MakeDirectoryEx(LPCTSTR make_path /*CString strMakePath*/);


	/**
	* 디렉토리와 파일 모두 삭제한다.
	*
	* @param		strPath : Path
	* @return		TRUE : 성공,  FALSE : 실패
	*/
	BOOL DeleteDirWithFile(LPCTSTR path /*CString strPath*/);
};
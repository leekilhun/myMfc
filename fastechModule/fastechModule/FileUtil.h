#pragma once

#include <vector>


// Error Code Define
#define E_FU_FILE_NOT_FOUND	-1
#define E_FU_FILE_COPY_FAIL	-2
#define E_FU_NOFILE_IN_DIR	-3


/**
 * File ����,����, ���� ���ϱ� ���� Uitlity���� �����ϴ� Class�̴�.
 */


class MFileUtil
{

public:

	/**
		* MFileUtil�� ������
		*/
	MFileUtil() {  }			// ������

	/**
		* MFileUtil�� �Ҹ���
		*/
	virtual ~MFileUtil() {}	// �Ҹ���

	/**
		* ������ Path�� ���丮�� �������� ���� ������ ���丮���� Ȯ���Ѵ�.
		*
		* @Param		pDir : Ȯ���� ���丮 Path
		* @return	    TRUE : �ٸ� ���丮 ���� ���� ����, FALSE : �ٸ� ���丮 ����
		*/
	BOOL	IsLeafDir(LPCTSTR pDir);

	/**
		* ���丮�� ���ϵ��� �����Ѵ�.
		* ���丮�� �������丮�� �������� �ʴ´�.
		* Progress Bar�� �پ ���� ����� ǥ���Ѵ�.
		*
		* @Param		pDir : ������ ���丮 Path
		* @return		TRUE/FALSE
		*/
	int		DeleteFilesInDir(LPCTSTR pDir);

	/**
		* ���丮�� �����ϴ� ������ ������ ���Ѵ�.
		*
		* @Param		pDir      : ���丮��
		* @return		������ ����
		*/
	int		GetFileCountInDir(LPCTSTR pDir);

	/**
		* ���丮�� ���� ���丮�� ������ ���Ѵ�.
		*
		* @Param		pDir : ������ ���丮 Path
		* @return		���� ���丮�� ����
		*/
	int		GetDirCountInDir(LPCTSTR pDir);

	/**
		* ���丮�� ���ϵ��� �����Ѵ�.
		*
		* @Param		pDest      : ������ ���丮
		* @Param		pSrc       : ���� ���丮
		* @Param		bOverWrite : OverWrite ���� (TRUE/FALSSE)
		* @return		���� ���丮�� ����
		*/
	int		CopyFilesInDir(LPCTSTR pDest, LPCTSTR pSrc, BOOL bOverWrite);


	/**
		* ���丮�� ������ ���� �ϴ��� �˻��Ѵ�.
		*
		* @Param		pFile      : �˻��ϰ��� �ϴ� ���ϸ� (Path ����)
		* @return		TRUE / FALSE
		*/
	BOOL	IsFileExist(LPCTSTR pFile);

	/**
		* ���丮�� ���丮�� ����Ʈ�� �����.
		*
		* @Param		pDir      : ���丮��
		* @Param		strDirname[] : string�� �迭
		* @Param		iCount		: ����
		* @return		SUCCESS / ERROR CODE
		*/
	int		MFileUtil::GetDirListInDir(LPCTSTR pDir, CString strDirName[], int iCount);


	/**
		* ���丮�� ���丮�� ����Ʈ�� �����.
		*
		* @Param		pDir      : ���丮��
		* @Param		strDirname[] : string�� �迭
		* @Param		iCount		: ����
		* @return		SUCCESS / ERROR CODE
		*/
	int		MFileUtil::GetDirListInDir(LPCTSTR pDir, CString strDirName[], int* iCount);

	int GetDirListInDir(LPCTSTR pDir, std::vector<CString>& strDirName);

	/**
		* �������� �ʴ� ���丮�� ��� ���� �����Ѵ�.
		*
		* @author		����ö
		* @param		pDir : Path (���� ���� �Ǵ� ������)
		* @param		bIsIncludeFile : pDir�� ������ ���� �ϴ°� ?, Default TRUE
		* @return		TRUE : ����,  FALSE : ����
		* @see
		* @version
		*/
	BOOL MFileUtil::MakeDir(LPCTSTR pDir, BOOL bIsIncludeFile = TRUE);


	BOOL MakeDirectoryEx(LPCTSTR make_path /*CString strMakePath*/);


	/**
	* ���丮�� ���� ��� �����Ѵ�.
	*
	* @param		strPath : Path
	* @return		TRUE : ����,  FALSE : ����
	*/
	BOOL DeleteDirWithFile(LPCTSTR path /*CString strPath*/);
};
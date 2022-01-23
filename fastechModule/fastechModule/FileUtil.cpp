#include "pch.h"
#include "FileUtil.h"


/**
	 * MFileUtil.cpp : File 처리를 위한 인터페이스 Class를 구현한 프로그램이다.
	 *
	 * @author		정종철
	 * @version		1.0
	 * @see
	 */

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


BOOL MFileUtil::IsLeafDir(LPCTSTR pDir)
{
	CFileFind	fn;
	CString		strFile;
	int			nextfile;

	strFile.Format(_T("%s\\*.*"), pDir);		// *.*로 검색한다.

	// Open A File Serach
	if (!fn.FindFile(strFile, 0)) return FALSE;

	// 디렉토리에서 해당 파일들을 찾기 시작한다.

	nextfile = fn.FindNextFile();	// 다음 파일을 찾는다.
	while (1)
	{
		// '.'나 '..'이 아닌 디렉토리를 포함하면 FALSE를 리턴한다. Leaf디렉토리가 아님
		if (fn.IsDirectory())
			if (!fn.IsDots()) return FALSE;
		if (!nextfile) break;	// 마지막에 도달했드면 루프를 빠진다.
		nextfile = fn.FindNextFile();	// 다음을 검색한다.
	}
	return TRUE;
}


int MFileUtil::DeleteFilesInDir(LPCTSTR pDir)
{

	CFileFind	fn;
	CString		strFile;
	int			filecnt = 0;

	// Directory내 파일 갯수를 구한다.
	if ((filecnt = GetFileCountInDir(pDir)) <= 0) return TRUE;

	// Show Progress Bar
	/*CMyProgressWnd ProgWnd(NULL, _T("디렉토리에서 파일 삭제....."));
	ProgWnd.SetRange(0, filecnt);
	ProgWnd.SetText(_T("File을 지우고 있습니다...\n")
		_T("잠시만 기다려 주세요..."));
	ProgWnd.PeekAndPump();*/

	// Path내 모들 파일을 검색하기 위한 파일명을 만든다.
	strFile.Format(_T("%s\\*.*"), pDir);
	if (!fn.FindFile(strFile, 0)) return FALSE;
	filecnt = fn.FindNextFile();
	while (1)
	{
		// 디렉토리가 아닌 파일들을 삭제한다.
		if (!fn.IsDirectory())
		{
			strFile = fn.GetFilePath();
			CFile::Remove(strFile);
			//ProgWnd.StepIt();
		}

		if (!filecnt) break;
		filecnt = fn.FindNextFile();
	}

	return TRUE;
}


int MFileUtil::GetFileCountInDir(LPCTSTR pDir)
{
	CFileFind	ff;
	int			cnt = 0, nt;
	CString		strFile;

	strFile.Format(_T("%s\\*.*"), pDir);			// 디렉토리명을 설정한다.
	if (!ff.FindFile(strFile, 0)) return 0; // FileFind Open
	nt = ff.FindNextFile();					// 파일을 검색하기 시작한다.
	while (1)
	{
		if (!ff.IsDirectory()) cnt++;		// 디렉토리가 아니면 파일 갯수 증가
		if (!nt) break;					// 마지막일 경우
		nt = ff.FindNextFile();				// 다음 파일을 찾는다
	}

	return cnt;
}


int MFileUtil::GetDirCountInDir(LPCTSTR pDir)
{
	CFileFind	ff;
	int			cnt = 0, nt;
	CString		strFind;

	strFind.Format(_T("%s\\*.*"), pDir);

	if (!ff.FindFile(strFind, 0)) return 0;

	nt = ff.FindNextFile();
	while (1)
	{
		if (ff.IsDirectory())
		{
			if (!ff.IsDots()) cnt++;
		}
		if (!nt) break;
		nt = ff.FindNextFile();
	}

	return cnt;
}


int MFileUtil::CopyFilesInDir(LPCTSTR pDestDir, LPCTSTR pSrcDir, BOOL bOverWrite)
{
	int			cnt = 0;
	CFileFind	ff;
	CString		strSearchFile;	// File 검색을 위한 String
	CString		strSrcFile;		// 소스 파일 (Path 포함)
	CString		strDestFile;	// 목적지 파일 (Path 포함)

	cnt = GetFileCountInDir(pSrcDir);
	if (!cnt)	return (E_FU_NOFILE_IN_DIR);

	if (bOverWrite) DeleteFilesInDir(pDestDir);

	/*CMyProgressWnd ProgWnd(NULL, _T("파일 복사....."));
	ProgWnd.SetRange(0, cnt);
	ProgWnd.SetText(_T("File을 Copy하고 있습니다..\n")
		_T("잠시만 기다려 주세요..."));
	ProgWnd.PeekAndPump();*/

	strSearchFile.Format(_T("%s\\*.*"), pSrcDir);
	if (!ff.FindFile(strSearchFile, 0)) return (E_FU_FILE_NOT_FOUND);
	cnt = ff.FindNextFile();
	while (1)
	{
		if (!ff.IsDirectory())
		{
			strSrcFile = ff.GetFilePath();
			strDestFile.Format(_T("%s\\"), pDestDir);
			strDestFile += ff.GetFileName();
			if (!CopyFile(strSrcFile, strDestFile, FALSE))
			{
				return (E_FU_FILE_COPY_FAIL);
			}
			//ProgWnd.StepIt();
		}
		if (!cnt) break;
		cnt = ff.FindNextFile();
	}

	return 0;
}

BOOL MFileUtil::IsFileExist(LPCTSTR pFile)
{
	CFileFind fn;

	if (!fn.FindFile(pFile, 0)) return FALSE;
	else return TRUE;
}

int MFileUtil::GetDirListInDir(LPCTSTR pDir, CString strDirName[], int count)
{
	CFileFind	ff;

	int			iCount = 0;	// 리턴할 갯수,  Arguement Count 이하의 값
	int			nt;			// 남은 파일의 갯수
	CString		strFind;	// 파일 검색을 위한 String

	// 파일을 검색하여 strDirName에 하나씩 집어 넣는다
	strFind.Format(_T("%s\\*.*"), pDir);
	if (!ff.FindFile(strFind, 0)) return 0;
	nt = ff.FindNextFile();
	while (1)
	{
		if (ff.IsDirectory())
		{
			if (!ff.IsDots())
			{
				// Argument로 입력된 한계를 넘는지 체크한다.
				if (iCount > (count - 1)) break;

				strDirName[iCount] = ff.GetFileName();
				iCount++;
			}
		}
		if (!nt) break;
		nt = ff.FindNextFile();
	}
	return iCount;	// 디렉토리 갯수를 리턴한다.
}

int MFileUtil::GetDirListInDir(LPCTSTR pDir, CString strDirName[], int* count)
{
	CFileFind	ff;

	int			iMaxCount = 256;	// 리턴할 갯수,  Arguement Count 이하의 값
	int			nt;			// 남은 파일의 갯수
	CString		strFind;	// 파일 검색을 위한 String
	int			iCount = 0;

	// 파일을 검색하여 strDirName에 하나씩 집어 넣는다
	strFind.Format(_T("%s\\*.*"), pDir);
	if (!ff.FindFile(strFind, 0)) return 0;
	nt = ff.FindNextFile();
	while (1)
	{
		if (ff.IsDirectory())
		{
			if (!ff.IsDots())
			{
				// Argument로 입력된 한계를 넘는지 체크한다.
				if (iCount > (iMaxCount - 1)) break;

				strDirName[iCount] = ff.GetFileName();
				iCount++;
			}
		}
		if (!nt) break;
		nt = ff.FindNextFile();
	}

	*count = iCount;
	return 0;	// 디렉토리 갯수를 리턴한다.
}


int MFileUtil::GetDirListInDir(LPCTSTR pDir, std::vector<CString>& strDirName)
{
	CFileFind	ff;
	int			nt;			// 남은 파일의 갯수
	CString		strFind;	// 파일 검색을 위한 String

							// 파일을 검색하여 strDirName에 하나씩 집어 넣는다
	strFind.Format(_T("%s\\*.*"), pDir);

	strDirName.clear();
	if (!ff.FindFile(strFind, 0))
		return 0;

	nt = ff.FindNextFile();
	while (1)
	{
		if (ff.IsDirectory() && !ff.IsDots())
			strDirName.push_back(ff.GetFileName());

		if (!nt)
			break;

		nt = ff.FindNextFile();
	}
	return int(strDirName.size());	// 디렉토리 갯수를 리턴한다.
}

BOOL MFileUtil::MakeDir(LPCTSTR pDir, BOOL bIsIncludeFile)
{
	UNUSED_ALWAYS(bIsIncludeFile);

	CFileFind	fn;
	CString		Dir;
	CString		SubDir;
	CFile		file;
	CFileException	e;
	int			iIndex, vIndex;
	BOOL		bFirst = TRUE;
	BOOL		bRet = TRUE;

	Dir = pDir;
	iIndex = 0;
	vIndex = 0;

	while (TRUE)
	{
		iIndex = Dir.Find(_T("\\"), vIndex + 1);
		if (iIndex == -1)
		{
			iIndex = Dir.Find(_T("/"), vIndex + 1);
			if (iIndex == -1)	break;
			else vIndex = iIndex;
		}
		else
			vIndex = iIndex;

		SubDir = Dir.Left(iIndex);
		if (bFirst && fn.FindFile(SubDir, 0))
		{
			bFirst = FALSE;

			bRet = fn.FindNextFile();
			if (bRet == FALSE)
				return FALSE;

			if (!fn.IsDirectory())
			{
				return FALSE;
			}
		}
		else
		{
			//			if(CreateDirectory(SubDir,NULL)==0) return FALSE;
			CreateDirectory(SubDir, NULL);
		}
	}

#if 0
	if (!bIsIncludeFile)
	{
		if (fn.FindFile(Dir, 0))
		{
			if (!fn.FindNextFile())
				return FALSE;
			if (fn.IsDirectory()) return TRUE;
			else return FALSE;
		}
		else
		{
			if (CreateDirectory(Dir, NULL) == 0) return FALSE;
			CreateDirectory(SubDir, NULL);
		}
	}
#endif
	return TRUE;
}

BOOL MFileUtil::MakeDirectoryEx(LPCTSTR make_path)
{
	CString strDirName;
	CString strMakePath = make_path;
	CFileFind ffind;

	if (ffind.FindFile(strMakePath) == TRUE)
		return TRUE;

	for (int i = 0; i < strMakePath.GetLength(); i++)
	{
		if (strMakePath.GetAt(i) == _T('\\') || strMakePath.GetAt(i) == _T('/'))
		{
			if (i > 0 && strMakePath.GetAt(i - 1) != _T(':'))
			{
				strDirName = strMakePath.Left(i);
				if (ffind.FindFile(strDirName) == FALSE)
					CreateDirectory(strDirName, NULL);
			}
		}
	}

	CreateDirectory(strMakePath, NULL);

	return TRUE;
}

BOOL MFileUtil::DeleteDirWithFile(LPCTSTR path)
{
	CString		strPath = path;
	CString		strFile;
	CFileFind	ffind;
	int			filecnt = 0;

	// Path내 모들 파일을 검색하기 위한 파일명을 만든다.
	strFile.Format(_T("%s\\*.*"), path);

	if (!ffind.FindFile(strFile, 0))
		return FALSE;

	filecnt = ffind.FindNextFile();
	while (1)
	{
		// 디렉토리가 아닌 파일들을 삭제한다.
		if (!ffind.IsDirectory())
		{
			strFile = ffind.GetFilePath();
			CFile::Remove(strFile);
		}

		if (!filecnt)
			break;
		filecnt = ffind.FindNextFile();
	}

	RemoveDirectory(strPath);

	return TRUE;
}

#include "pch.h"
#include "FileUtil.h"


/**
	 * MFileUtil.cpp : File ó���� ���� �������̽� Class�� ������ ���α׷��̴�.
	 *
	 * @author		����ö
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

	strFile.Format(_T("%s\\*.*"), pDir);		// *.*�� �˻��Ѵ�.

	// Open A File Serach
	if (!fn.FindFile(strFile, 0)) return FALSE;

	// ���丮���� �ش� ���ϵ��� ã�� �����Ѵ�.

	nextfile = fn.FindNextFile();	// ���� ������ ã�´�.
	while (1)
	{
		// '.'�� '..'�� �ƴ� ���丮�� �����ϸ� FALSE�� �����Ѵ�. Leaf���丮�� �ƴ�
		if (fn.IsDirectory())
			if (!fn.IsDots()) return FALSE;
		if (!nextfile) break;	// �������� �����ߵ�� ������ ������.
		nextfile = fn.FindNextFile();	// ������ �˻��Ѵ�.
	}
	return TRUE;
}


int MFileUtil::DeleteFilesInDir(LPCTSTR pDir)
{

	CFileFind	fn;
	CString		strFile;
	int			filecnt = 0;

	// Directory�� ���� ������ ���Ѵ�.
	if ((filecnt = GetFileCountInDir(pDir)) <= 0) return TRUE;

	// Show Progress Bar
	/*CMyProgressWnd ProgWnd(NULL, _T("���丮���� ���� ����....."));
	ProgWnd.SetRange(0, filecnt);
	ProgWnd.SetText(_T("File�� ����� �ֽ��ϴ�...\n")
		_T("��ø� ��ٷ� �ּ���..."));
	ProgWnd.PeekAndPump();*/

	// Path�� ��� ������ �˻��ϱ� ���� ���ϸ��� �����.
	strFile.Format(_T("%s\\*.*"), pDir);
	if (!fn.FindFile(strFile, 0)) return FALSE;
	filecnt = fn.FindNextFile();
	while (1)
	{
		// ���丮�� �ƴ� ���ϵ��� �����Ѵ�.
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

	strFile.Format(_T("%s\\*.*"), pDir);			// ���丮���� �����Ѵ�.
	if (!ff.FindFile(strFile, 0)) return 0; // FileFind Open
	nt = ff.FindNextFile();					// ������ �˻��ϱ� �����Ѵ�.
	while (1)
	{
		if (!ff.IsDirectory()) cnt++;		// ���丮�� �ƴϸ� ���� ���� ����
		if (!nt) break;					// �������� ���
		nt = ff.FindNextFile();				// ���� ������ ã�´�
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
	CString		strSearchFile;	// File �˻��� ���� String
	CString		strSrcFile;		// �ҽ� ���� (Path ����)
	CString		strDestFile;	// ������ ���� (Path ����)

	cnt = GetFileCountInDir(pSrcDir);
	if (!cnt)	return (E_FU_NOFILE_IN_DIR);

	if (bOverWrite) DeleteFilesInDir(pDestDir);

	/*CMyProgressWnd ProgWnd(NULL, _T("���� ����....."));
	ProgWnd.SetRange(0, cnt);
	ProgWnd.SetText(_T("File�� Copy�ϰ� �ֽ��ϴ�..\n")
		_T("��ø� ��ٷ� �ּ���..."));
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

	int			iCount = 0;	// ������ ����,  Arguement Count ������ ��
	int			nt;			// ���� ������ ����
	CString		strFind;	// ���� �˻��� ���� String

	// ������ �˻��Ͽ� strDirName�� �ϳ��� ���� �ִ´�
	strFind.Format(_T("%s\\*.*"), pDir);
	if (!ff.FindFile(strFind, 0)) return 0;
	nt = ff.FindNextFile();
	while (1)
	{
		if (ff.IsDirectory())
		{
			if (!ff.IsDots())
			{
				// Argument�� �Էµ� �Ѱ踦 �Ѵ��� üũ�Ѵ�.
				if (iCount > (count - 1)) break;

				strDirName[iCount] = ff.GetFileName();
				iCount++;
			}
		}
		if (!nt) break;
		nt = ff.FindNextFile();
	}
	return iCount;	// ���丮 ������ �����Ѵ�.
}

int MFileUtil::GetDirListInDir(LPCTSTR pDir, CString strDirName[], int* count)
{
	CFileFind	ff;

	int			iMaxCount = 256;	// ������ ����,  Arguement Count ������ ��
	int			nt;			// ���� ������ ����
	CString		strFind;	// ���� �˻��� ���� String
	int			iCount = 0;

	// ������ �˻��Ͽ� strDirName�� �ϳ��� ���� �ִ´�
	strFind.Format(_T("%s\\*.*"), pDir);
	if (!ff.FindFile(strFind, 0)) return 0;
	nt = ff.FindNextFile();
	while (1)
	{
		if (ff.IsDirectory())
		{
			if (!ff.IsDots())
			{
				// Argument�� �Էµ� �Ѱ踦 �Ѵ��� üũ�Ѵ�.
				if (iCount > (iMaxCount - 1)) break;

				strDirName[iCount] = ff.GetFileName();
				iCount++;
			}
		}
		if (!nt) break;
		nt = ff.FindNextFile();
	}

	*count = iCount;
	return 0;	// ���丮 ������ �����Ѵ�.
}


int MFileUtil::GetDirListInDir(LPCTSTR pDir, std::vector<CString>& strDirName)
{
	CFileFind	ff;
	int			nt;			// ���� ������ ����
	CString		strFind;	// ���� �˻��� ���� String

							// ������ �˻��Ͽ� strDirName�� �ϳ��� ���� �ִ´�
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
	return int(strDirName.size());	// ���丮 ������ �����Ѵ�.
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

	// Path�� ��� ������ �˻��ϱ� ���� ���ϸ��� �����.
	strFile.Format(_T("%s\\*.*"), path);

	if (!ffind.FindFile(strFile, 0))
		return FALSE;

	filecnt = ffind.FindNextFile();
	while (1)
	{
		// ���丮�� �ƴ� ���ϵ��� �����Ѵ�.
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

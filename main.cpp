// ExcelTest.cpp : 定义控制台应用程序的入口点。
//




#include "stdafx.h"
//#include "XLRange.h"
#include "MSExcel.h"
#include "afxwin.h"
#include "OLEAuto.h"
#include "ExcelDlg.h"
#include "XLRange.h"
#include "resource.h"
#include "LR1PG.h"
#include <conio.h>

CMSExcel m_msExcel;
char letter[26]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
class MyWindow : public CFrameWnd
{
	
public:
	MyWindow()
	{
	    Create(NULL,_T("Parser"));

		AllocConsole();
		freopen("CONIN$", "r+t", stdin); // 重定向 STDIN
        freopen("CONOUT$", "w+t", stdout); // 重定向STDOUT 

		LR1PG::load_productions ();



		CFileDialog cfd(true,_T("*.xls*"),0,4|2,_T("Microsoft Excel Files|*.xls*"));
		if(cfd.DoModal()==IDCANCEL) return;
		if(SUCCEEDED(m_msExcel.OpenExcelBook(cfd.GetPathName())))
		{
			CString szCell,value;
			string svalue;
			LR1PG::element  elem;
			int col=2,row=1;
			for(;;col++,row=1)
			{
				szCell=get_cell_loaction(row,col);
			    m_msExcel.GetExcelValue(szCell,value);
				if(value.IsEmpty())
					break;
				USES_CONVERSION;
		        svalue= T2CA((LPCTSTR)value);

				cout<<svalue<<endl;

				if(LR1PG::var_list.find(svalue)==LR1PG::var_list.end())//T
				{
					elem.index=LR1PG::ter_list[svalue];
					elem.isVar=0;
				}
				else//V
				{
					elem.index=LR1PG::var_list[svalue];
					elem.isVar=1;
				}
				for(row=2;;row++)
				{
					szCell=get_cell_loaction(row,col);
			        m_msExcel.GetExcelValue(szCell,value);
					USES_CONVERSION;
		            svalue= T2CA((LPCTSTR)value);
					if(value.IsEmpty())
					  break;
					LR1PG::action act(svalue);
					LR1PG::LR_table.set(row-2,elem,act);
				}
			}
		}
		else
		{
			cout<<"打开excel文件:"<<cfd.GetPathName()<<"失败"<<endl;
		}	



		cout<<endl;
	}



	CString get_cell_loaction(int row,int col)
	{//将数字坐标转换成excel中cell的定位方式，如A1
		CString cell;	
		CString cstr;
		col-=1;
		int a=col%26;
		cell=letter[a];
		while(col/=26)
		{
		   a=col%26;
		   cell=letter[a-1]+cell;
		}
		cstr.Format(_T("%d"),row);
		cell=cell+cstr;
		return cell;
	}

};



class MyApp : public CWinApp
{
public:
	BOOL InitInstance()
	{
		MyWindow *MywindowObject = new MyWindow();
		m_pMainWnd = MywindowObject;
		MywindowObject->ShowWindow(SW_SHOWNORMAL);
		//MywindowObject->ShowWindow(SW_HIDE);
	
		return TRUE;
	}
	
}App;




int main()
{
	


	App.InitInstance();

	
	return 0;

}









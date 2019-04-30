#pragma once
#include "main.h"

class CMainWnd : public SHostWnd
{
public:
    CMainWnd() 
        : SHostWnd(_T("LAYOUT:XML_MAINWND"))//这里定义主界面需要使用的布局文件
    {
        m_bLayoutInited=FALSE;
    }

    void OnClose()
    {
        PostMessage(WM_QUIT);
    }
    void OnMaximize()
    {
        SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE);
    }
    void OnRestore()
    {
        SendMessage(WM_SYSCOMMAND,SC_RESTORE);
    }
    void OnMinimize()
    {
        SendMessage(WM_SYSCOMMAND,SC_MINIMIZE);
    }

    void OnSize(UINT nType, CSize size)
    {
        SetMsgHandled(FALSE);
        if(!m_bLayoutInited) return;
        if(nType==SIZE_MAXIMIZED)
        {
            FindChildByName(_T("btn_restore"))->SetVisible(TRUE);
            FindChildByName(_T("btn_max"))->SetVisible(FALSE);
        }else if(nType==SIZE_RESTORED)
        {
            FindChildByName(_T("btn_restore"))->SetVisible(FALSE);
            FindChildByName(_T("btn_max"))->SetVisible(TRUE);
        }
    }
    void OnBtnMsgBox()
    {
        SMessageBox(NULL,_T("this is a message box"),_T("haha"),MB_OK|MB_ICONEXCLAMATION);
        SMessageBox(NULL,_T("this message box includes two buttons"),_T("haha"),MB_YESNO|MB_ICONQUESTION);
        SMessageBox(NULL,_T("this message box includes three buttons"),NULL,MB_ABORTRETRYIGNORE);
    }
    
    BOOL OnInitDialog( HWND hWnd, LPARAM lParam )
    {
        m_bLayoutInited=TRUE;

        return 0;
    }
protected:
    //按钮事件处理映射表
    EVENT_MAP_BEGIN()
        EVENT_NAME_COMMAND(L"btn_close",OnClose)
        EVENT_NAME_COMMAND(L"btn_min",OnMinimize)
        EVENT_NAME_COMMAND(L"btn_max",OnMaximize)
        EVENT_NAME_COMMAND(L"btn_restore",OnRestore)
        EVENT_NAME_COMMAND(L"btn_msgbox",OnBtnMsgBox)
    EVENT_MAP_END()    

    //窗口消息处理映射表
    BEGIN_MSG_MAP_EX(CMainWnd)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_SIZE(OnSize)
        CHAIN_MSG_MAP(SHostWnd)//注意将没有处理的消息交给基类处理
        REFLECT_NOTIFICATIONS_EX()
    END_MSG_MAP()
private:
    BOOL            m_bLayoutInited;
};


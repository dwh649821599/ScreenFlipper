// screen_flipper.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <Windows.h>
#include <WinUser.h>
#include <vector>
#include "get_window_handler.h"
//显示器信息结构体
typedef struct _tagMyMonitorInfo_t
{
    HMONITOR hMonitor;
    MONITORINFOEX info;

    _tagMyMonitorInfo_t()
    {
        ZeroMemory(this, sizeof(*this));
    }
}MYMONITORINFO, * LPMYMONITORINFO;
std::vector<MYMONITORINFO> m_MyMonitorInfo;

//EnumDisplayMonitors 回调函数
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor,
    LPRECT lprcMonitor, LPARAM dwData)
{
    MYMONITORINFO mInfo;
    mInfo.hMonitor = hMonitor;
    mInfo.info.cbSize = sizeof(mInfo.info);
    GetMonitorInfo(hMonitor, &mInfo.info);
    m_MyMonitorInfo.push_back(mInfo);

    return TRUE;
}

int MoveWindow2Monitor(
    int iMonitorIndex, //显示器索引0,1,2,etc
    HWND hWnd, //窗口指针
    int x = 0, int y = 0, //窗口左上角坐标偏移
    int cx = 0, int cy = 0 //窗口尺寸 (0时不改变)
)
{
    int iRet = 0;

    do
    {
        //窗口句柄有效？
        if (!IsWindow(hWnd))
        {
            printf("Invalid window handle");
            iRet = -1;
            break;
        }

        //枚举显示器
        m_MyMonitorInfo.clear();
        if (!EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, NULL))
        {
            printf("EnumDisplayMonitors failed %u\n", GetLastError());
            iRet = -2;
            break;
        }

        //显示器索引是否正常？
        if (iMonitorIndex >= /*GetSystemMetrics(SM_CMONITORS)*/
            m_MyMonitorInfo.size())
        {
            printf("Out of Monitor ranges\n");
            iRet = -3;
            break;
        }

        //工作区域坐标 
        RECT rcWork = m_MyMonitorInfo[iMonitorIndex].info.rcWork;

        //左上角坐标
        x += rcWork.left;
        y += rcWork.top;
        //尺寸
        RECT rcWnd;
        GetWindowRect(hWnd, &rcWnd);
        if (cx == 0)
            cx = rcWnd.right - rcWnd.left;
        if (cy == 0)
            cy = rcWnd.bottom - rcWnd.top;

        //位置限制
        {
        }

        //移动窗口
        if (!SetWindowPos(hWnd, NULL, x, y, cx, cy,
            SWP_FRAMECHANGED | SWP_NOZORDER))
        {
            printf("SetWindowPos failed %u", GetLastError());
            iRet = -4;
            break;
        }

        iRet = TRUE;
    } while (0);

    return iRet;
}

int main() {
    HWND handler = FindWindowByProcessName(L"Clash.exe");
    MoveWindow2Monitor(1, handler);
}
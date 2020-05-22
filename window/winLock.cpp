// Creating a lock
class CMyCriticalSection
{
public:
	CMyCriticalSection()
	{
		//printf("init\n");
		InitializeCriticalSection(&m_cSection);
	}

	void Lock()
	{
		//printf("Lock\n");
		//同一个线程多次锁无效
		EnterCriticalSection(&m_cSection);
	}

	void UnLock()
	{
		//printf("UnLock\n");
		LeaveCriticalSection(&m_cSection);
	}


	//利用析构函数删除临界区对象
	virtual ~CMyCriticalSection()
	{
		//printf("delete\n");
		DeleteCriticalSection(&m_cSection);
	}
private:
	CRITICAL_SECTION m_cSection;
};


class CAutoLock
{
public:
	//利用构造函数上锁，即进去临界区
	CAutoLock(CMyCriticalSection *p)
	{
		pSection = p;
		pSection->Lock();
	}

	//利用析构函数解锁，即离开临界区
	virtual ~CAutoLock()
	{
		pSection->UnLock();
	}
private:
	CMyCriticalSection *    pSection;
};


CMyCriticalSection              criticalSection;      //临界区

int a = 1;

DWORD WINAPI Fun(LPVOID lpParamter)
{
	string strPrint((const char*)lpParamter);
	int iRunTime = 0;
	//执行100次跳出
	while (++iRunTime<2)
	{
		{
			CAutoLock m(&criticalSection);
			cout << "[" << iRunTime << "]:" << a++ << endl;
		}
		Sleep(1); //若去掉此句 可能导致其他线程无法进入临界区,因为 cLock在这之前析构，离开临界区

	}
	return 0;
}

int main()
{
	//创建五个子线程
	string str1 = "A";
	string str2 = "B";
	string str3 = "C";
	string str4 = "D";
	string str5 = "E";

	HANDLE hThread1 = CreateThread(NULL, 0, Fun, (void*)str1.c_str(), 0, NULL);
	HANDLE hThread2 = CreateThread(NULL, 0, Fun, (void*)str2.c_str(), 0, NULL);
	HANDLE hThread3 = CreateThread(NULL, 0, Fun, (void*)str3.c_str(), 0, NULL);
	HANDLE hThread4 = CreateThread(NULL, 0, Fun, (void*)str4.c_str(), 0, NULL);
	HANDLE hThread5 = CreateThread(NULL, 0, Fun, (void*)str5.c_str(), 0, NULL);

	//关闭线程
	CloseHandle(hThread1);
	CloseHandle(hThread2);
	CloseHandle(hThread3);
	CloseHandle(hThread4);
	CloseHandle(hThread5);

	getchar();
	//  system("pause");
	return 0;
}



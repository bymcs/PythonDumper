#include "PythonDumper.h"


HMODULE mod;

bool getCurrentPath(HMODULE hMod, char* dllPath, int size)
{
	int len = GetModuleFileNameA(hMod, dllPath, size);
	if (!len) {
		return false;
	}
	for (int i = len; i >= 0; --i) {
		if (dllPath[i] == '\\')
			return true;;
		dllPath[i] = 0;
	}

	return false;
}

void printModules() {
	PythonModuleMap moduleMap;
	if (!PythonDumper::getPythonModulesAndFunctions(&moduleMap)) {
		printf("Some Error Occurred\n");
		return;
	}

	for (auto mod : moduleMap) {
		printf("\n----------\nModule %s\n----------\n\n", mod.first.c_str());
		for (auto func : mod.second) {
			printf("%s : %#x\n", func.first.c_str(), func.second);
		}
	}
}


void menu() {
	printf("Type the number associeted with the type of dump.\n");
	printf("1 - To File\n");
	printf("2 - On Screen\n");

	int answer = 0;
	scanf("%d", &answer);
	system("cls");
	if (answer == 1) {
		char dllPath[MAX_PATH] = { 0 };
		getCurrentPath(mod, dllPath, MAX_PATH);
		std::string path(dllPath);
		path.append("dump.txt");
		freopen(path.c_str(), "wb", stdout);
		printModules();
		freopen("CONOUT$", "wb", stdout);
		printf("Dump written to %s\n", path.c_str());
		system("pause");
		return;
	}
	if (answer == 2) {
		printModules();
		return;
	}
	system("cls");
	menu();
}

void Leave() {
	printf("Closed!!");
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);
	FreeConsole();
	FreeLibraryAndExitThread(mod, 0);
}


void SetupConsole()
{
	AllocConsole();
	freopen("CONOUT$", "wb", stdout);
	freopen("CONOUT$", "wb", stderr);
	freopen("CONIN$", "rb", stdin);
	SetConsoleTitle(L"Python Dumper");
}

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	SetupConsole();
	menu();
	Leave();
	return true;
}


BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		mod = (HMODULE)hModule;
		CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
		break;
	case DLL_THREAD_ATTACH: break;
	case DLL_THREAD_DETACH: break;
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

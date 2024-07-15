#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>

#define MAX_PATH_LENGTH 260
#define MAX_SECTION_LENGTH 256
#define MAX_FIELD_LENGTH 256
#define MAX_VALUE_LENGTH 256

void ModifyIniField(const char* iniPath, const char* section, const char* field, const char* value) {
	if (!WritePrivateProfileStringA(section, field, value, iniPath)) {
		char errorMsg[256];
		sprintf(errorMsg, "无法修改或添加.ini文件中的字段内容。错误码：%lu", GetLastError());
		perror(errorMsg);
		exit(EXIT_FAILURE);
	}
}

void ShowHelp() {
	printf("使用方法：\n");
	printf("ModifyIni.exe /inifile \"ini文件路径\" /section \"段落名称\" /field \"字段名称\" /value \"值\"\n\n");
	printf("本程序由“何伟贵-873578156”编写。\n");
}

bool FileExists(const char* filePath) {
	DWORD attributes = GetFileAttributesA(filePath);
	return (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
}

int main(int argc, char* argv[]) {

	char iniFile[MAX_PATH_LENGTH] = "";
	char section[MAX_SECTION_LENGTH] = "";
	char field[MAX_FIELD_LENGTH] = "";
	char value[MAX_VALUE_LENGTH] = "";

	bool valueFound = false;

	int i;  /* 因为编译器是C90，在循环外部声明变量 */

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "/inifile") == 0 && i + 1 < argc)
			strncpy(iniFile, argv[++i], sizeof(iniFile) - 1);
		else if (strcmp(argv[i], "/section") == 0 && i + 1 < argc)
			strncpy(section, argv[++i], sizeof(section) - 1);
		else if (strcmp(argv[i], "/field") == 0 && i + 1 < argc)
			strncpy(field, argv[++i], sizeof(field) - 1);
		else if (strcmp(argv[i], "/value") == 0 && i + 1 < argc) {
			strncpy(value, argv[++i], sizeof(value) - 1);
			valueFound = true;
		} else if (strcmp(argv[i], "/?") == 0) {
			ShowHelp();
			return 0;
		}
	}

	if (iniFile[0] == '\0' || section[0] == '\0' || field[0] == '\0') {
		ShowHelp();
		return 0;
	}

	if (!valueFound)
		value[0] = '\0';

	if (!FileExists(iniFile)) {
		fprintf(stderr, "错误：指定的.ini文件不存在。\n");
		return -1;
	}

	ModifyIniField(iniFile, section, field, value);

	printf("已成功修改.ini文件中的字段内容。\n");

	return 0;
}


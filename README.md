# Qt-Inihelper
由于QSetting的读写方式感觉比较麻烦,且对中文支持不太友好,所以折腾了这个qt平台读写ini的帮助类

## 示例用法
```c++
	QString filePath = AppDirPath() + "/" + "测试.ini";
	FileHelper::RemoveFile(filePath);
	for (int i = 0; i < 100; i++)
	{
		QString section = QString("测试节点%1").arg(i);
		QString key = QString("测试键%1").arg(i);
		QString value = QString("测试值%1").arg(i);
		IniHelper::Write(section, key, value, filePath);
	}

	for (int i = 0; i < 100; i++)
	{
		QString section = QString("测试节点%1").arg(i);
		QString key = QString("测试键%1").arg(i);
		QString readValue = IniHelper::Read(section, key, filePath);
		qDebug() << "Read value:" << readValue;
	}

	QString testfile = AppDirPath() + "/" + "test.txt";
	QStringList sections = IniHelper::ReadSections(testfile);
	QStringList keys = IniHelper::ReadKeys(sections.at(0), testfile);
	INIDictionary lists = IniHelper::ReadKeyValues(sections.at(0), testfile);
	IniHelper::RenameSection(sections.at(0), "测试修改新节点", testfile);
	IniHelper::RemoveKey("测试修改新节点", keys.at(keys.count() - 1), testfile);
	IniHelper::ModifyKey("测试修改新节点", keys.at(0), "测试新节点", "新节点的键值", testfile);
	IniHelper::RemoveSection("测试修改新节点", testfile);

	QXINI ini(filePath);
	ini["测试中文节点"]["测试中文键"] = 1;
	int value = ini["测试中文节点"]["测试中文键"];
	double value1 = ini["测试中文节点"]["测试中文键"];
	std::string value2 = ini["测试中文节点"]["测试中文键"];
	QString value3 = ini["测试中文节点"]["测试中文键"];
	ulong value4 = ini["测试中文节点"]["测试中文键"];

	unsigned short value5 = ini["测试中文节点"]["测试中文键"];
	unsigned int  value6 = ini["测试中文节点"]["测试中文键"];
	long value7 = ini["测试中文节点"]["测试中文键"];
	unsigned long value8 = ini["测试中文节点"]["测试中文键"];
	long long  value9 = ini["测试中文节点"]["测试中文键"];
	unsigned long long  value10 = ini["测试中文节点"]["测试中文键"];
	float  value11 = ini["测试中文节点"]["测试中文键"];

	short value12 = ini["测试中文节点"]["测试中文键"];
	char* value13 = ini["测试中文节点"]["测试中文键"];
	const char* value14 = ini["测试中文节点"]["测试中文键"];
	bool value15 = ini["测试中文节点"]["测试中文键"];

	ini["测试中文节点"]["测试中文键"] = value;
	ini["测试中文节点"]["测试中文键1"] = value1;
	ini["测试中文节点"]["测试中文键2"] = value2;
	ini["测试中文节点"]["测试中文键3"] = value3;
	ini["测试中文节点"]["测试中文键4"] = value4;
	ini["测试中文节点"]["测试中文键5"] = value5;
	ini["测试中文节点"]["测试中文键6"] = value6;
	ini["测试中文节点"]["测试中文键7"] = value7;
	ini["测试中文节点"]["测试中文键8"] = value8;
	ini["测试中文节点"]["测试中文键9"] = value9;
	ini["测试中文节点"]["测试中文键10"] = value10;
	ini["测试中文节点"]["测试中文键11"] = value11;
	ini["测试中文节点"]["测试中文键12"] = value12;
	ini["测试中文节点"]["测试中文键13"] = value13;
	ini["测试中文节点"]["测试中文键14"] = value14;
	ini["测试中文节点"]["测试中文键15"] = value15;
```

#pragma once

# Author: [ATFmxyl]
# Date: [2023.05.03]
# Description: [Qt平台操作ini文件的帮助类]


#include <QtCore>
#include <QList>
#include <QStringList>
#include <QDebug>

#include <iostream>
#include <string>

#include <Windows.h>

//ini读写键值对用字典
class INIDictionary
{
public:

	QList<QStringList> dict;

	INIDictionary()
	{
		dict.clear();
	}

	//将指定的键和值添加到
	void Add(const QVariant& key, const QVariant& value)
	{
		if(!ContainsKey(key))
		{
			QList<QString> str;
			str.append(key.toString());
			str.append(value.toString());
			dict.append(str);
		}
	}

	void RemoveAt(const QVariant& key)
	{
		for (int i = 0; i < dict.count(); i++)
		{
			if (dict.at(i).at(0) == key)
			{
				dict.removeAt(i);
			}
		}
	}

	void RemoveAt(const int key)
	{
		if (key < dict.count())
			dict.removeAt(key);
	}

	QVariant at(const int pos)
	{
		if (pos < dict.count())
			return dict.at(pos).at(1);
		else
			return QVariant();
	}

	QVariant TryGetValue(const QVariant& key)
	{
		for (auto item : dict)
		{
			if (item.at(0) == key.toString())
			{
				return item.at(1);
			}
		}
		return QVariant();
	}

	bool ContainsKey(const QVariant& key)
	{
		for (auto item : dict)
		{
			if (item.at(0) == key.toString())
			{
				return true;
			}
		}
		return false;
	}

	bool ContainsValue(const QVariant& value)
	{
		for (auto item : dict)
		{
			if (item.at(1) == value.toString())
			{
				return true;
			}
		}
		return false;
	}

	bool Clear()
	{
		dict.clear();
	}

	int Count()
	{
		return dict.count();
	}

	QStringList keys()
	{
		QStringList info;
		for (auto item : dict)
		{
			info.append(item.at(0));
		}
		return info;
	}

	QStringList Values()
	{
		QStringList info;
		for (auto item : dict)
		{
			info.append(item.at(1));
		}
		return info;
	}

	// 重载 [] 运算符
	QVariant operator[](const QVariant& key)
	{
		for (auto& item : dict)
		{
			if (item[0] == key.toString())
			{
				return QVariant(item[1]);
			}
		}
		return QVariant();
	}
};

class IniHelper
{
private:
	inline const static int maxsize = 8192;
public:

	//键值对模型
	struct KeyValue
	{
		//索引
		int Index;
		//键
		QString Key;
		//值
		QString Value;

		//构造函数
		KeyValue(int index = 0, const QString& key = "", const QString& value = "")
			: Index(index), Key(key), Value(value)
		{}

		//清空方法
		void Clear()
		{
			Index = 0;
			Key.clear();
			Value.clear();
		}

		//重载=赋值运算符
		KeyValue& operator=(const KeyValue& input)
		{
			//判断是否是自我赋值
			if (this != &input)
			{
				Index = input.Index;
				Key = input.Key;
				Value = input.Value;
			}
			return *this;
		}
	};

	//读取对应的值
	static QString Read(const QString& section, const QString& key, const QString& filePath)
	{
		// 动态分配缓冲区
		wchar_t* buffer = new wchar_t[maxsize];
		memset(buffer, 0, sizeof(wchar_t) * maxsize);
		GetPrivateProfileString((LPCWSTR)section.utf16(), (LPCWSTR)key.utf16(), L"", buffer, maxsize, (LPCWSTR)filePath.utf16());

		QString value = QString::fromWCharArray(buffer);
		// 释放动态分配的内存
		delete[] buffer;
		return value;
	}

	//写入对应的值
	static bool Write(const QString& section, const QString& key, const QString& value, const QString& filePath)
	{
		return WritePrivateProfileString((LPCWSTR)section.utf16(), (LPCWSTR)key.utf16(), (LPCWSTR)value.utf16(), (LPCWSTR)filePath.utf16());
	}

	//修改节点名称
	static bool RenameSection(const QString& oldSectionName, const QString& newSectionName, const QString& filePath)
	{
		INIDictionary keyValuePairs = ReadKeyValues(oldSectionName, filePath);
		for (auto item : keyValuePairs.dict)
		{
			Write(newSectionName, item.at(0), item.at(1), filePath);
		}
		return RemoveSection(oldSectionName, filePath);
	}

	//删除某个节点
	static bool RemoveSection(const QString& section, const QString& filePath)
	{
		return WritePrivateProfileString((LPCWSTR)section.utf16(), NULL, NULL, (LPCWSTR)filePath.utf16());
	}

	//删除某个节点下的某个Key
	static bool RemoveKey(const QString& section, const QString& key, const QString& filePath)
	{
		return WritePrivateProfileString((LPCWSTR)section.utf16(), (LPCWSTR)key.utf16(), NULL, (LPCWSTR)filePath.utf16());
	}

	//获取所有Section节点
	static QStringList ReadSections(const QString& filePath)
	{
		// 动态分配缓冲区
		wchar_t* buffer = new wchar_t[maxsize];
		memset(buffer, 0, sizeof(wchar_t) * maxsize);
		QStringList sectionNames;
		DWORD bufferSize = GetPrivateProfileSectionNames(buffer, maxsize, (LPCWSTR)filePath.utf16());
		if (bufferSize > 0)
		{
			// 分离并解析节点名称
			int startPos = 0;
			for (DWORD i = 0; i < bufferSize; i++)
			{
				if (buffer[i] == L'\0')
				{
					QString sectionName = QString::fromWCharArray(&buffer[startPos]);
					startPos = i + 1;

					// 如果sectionName不为空，则添加到列表中
					if (!sectionName.isEmpty())
						sectionNames.append(sectionName);
				}
			}
		}
		// 释放动态分配的内存
		delete[] buffer;
		return sectionNames;
	}

	//获取Section节点下所有Key
	static QStringList ReadKeys(const QString& sectionName, const QString& fileName)
	{
		// 动态分配缓冲区
		wchar_t* buffer = new wchar_t[maxsize];
		memset(buffer, 0, sizeof(wchar_t) * maxsize);
		QStringList keys;
		DWORD bufferSize = GetPrivateProfileSection(sectionName.toStdWString().c_str(), buffer, maxsize, fileName.toStdWString().c_str());
		if (bufferSize > 0)
		{
			// 分离并解析节点名称
			int startPos = 0;
			for (DWORD i = 0; i < bufferSize; i++)
			{
				if (buffer[i] == L'\0')
				{
					QStringList info = QString::fromWCharArray(&buffer[startPos]).split('=', Qt::SkipEmptyParts);
					if (info.size() == 2)
					{
						if (!info.at(0).isEmpty())
							keys.append(info.at(0));
						startPos = i + 1;
					}
				}
			}
		}
		// 释放动态分配的内存
		delete[] buffer;
		return keys;
	}

	//获取Section节点下所有Key,Value值
	static INIDictionary ReadKeyValues(QString sectionName, QString filePath)
	{
		INIDictionary keyValuePairs;
		QList<QString> keys = ReadKeys(sectionName, filePath);
		for (auto key : keys)
		{
			if (!keyValuePairs.ContainsKey(key))
			{
				keyValuePairs.Add(key, Read(sectionName, key, filePath));
			}
		}
		return keyValuePairs;
	}

	//获取Section节点下所有Key,Value值
	static QList<KeyValue> ReadKeyValue(const QString& sectionName, const QString& filePath)
	{
		QList<KeyValue> keyValues;
		QStringList keys = ReadKeys(sectionName, filePath);
		int index = 0;
		for (auto key : keys)
		{
			keyValues.append(KeyValue(index, key, Read(sectionName, key, filePath)));
			index++;
		}
		return keyValues;
	}

	//修改Key
	static void ModifyKey(const QString& section, const QString& oldKey, const QString& newKey, const QString& value, const QString& filePath)
	{
		QList<KeyValue> keyValues = ReadKeyValue(section, filePath);
		int count = std::count_if(keyValues.begin(), keyValues.end(), [oldKey](const KeyValue& kv) { return kv.Key == oldKey; });
		if (count == 0)
		{
			Write(section, newKey, value, filePath);
			return;
		}

		// 1. 找到第一对 key == oldKey 的键值对，并获取其索引 index
		int index = -1;
		for (int i = 0; i < keyValues.size(); i++)
		{
			if (keyValues.at(i).Key == oldKey)
			{
				index = keyValues.at(i).Index;
				break;
			}
		}

		// 2. 删除 index 之后的所有 key-value 对
		for (int i = keyValues.size() - 1; i >= 0; i--)
		{
			if (keyValues.at(i).Index >= index)
			{
				RemoveKey(section, keyValues.at(i).Key, filePath);
			}
		}

		// 3. 根据 index，向配置文件中写入新的 key-value 对，并将后续的 key-value 继续写入到配置文件中
		for (int i = 0; i < keyValues.size(); i++)
		{
			if (keyValues.at(i).Index == index)
			{
				Write(section, newKey, value, filePath);
			}
			else if (keyValues.at(i).Index > index)
			{
				Write(section, keyValues.at(i).Key, keyValues.at(i).Value, filePath);
			}
		}
	}
};

//重载操作ini读写
//可以支持 QXINI int(path); ini[Section][Key] = Value;的方式写入数据  以及 Qt支持的基础数据类型 Value = ini[Section][Key]; 读取数据
class QXINI
{
private:
	QString m_filePath;
public:
	QXINI(const QString& filePath) : m_filePath(filePath) {}

	//// Close the INI file.
	~QXINI() {}
	
	//读取所有节点
	QStringList ReadSections()
	{
		return ::IniHelper::ReadSections(m_filePath);
	}

	//读取所有键
	QStringList ReadKeys(const QString& sectionName)
	{
		return ::IniHelper::ReadKeys(sectionName,m_filePath);
	}

	class Section
	{
	public:
		Section(const QString& sectionName, const QString& filePath) : m_sectionName(sectionName), m_filePath(filePath) {}

		class Key
		{
		public:
			Key(const QString& keyName, const Section& section): m_keyName(keyName), m_section(section){}

			//支持std标准字符串
			operator std::string() const
			{
				return IniHelper::Read(m_section.m_sectionName, m_keyName, m_section.m_filePath).toStdString();
			}

			operator char* () const
			{
				QString str = IniHelper::Read(m_section.m_sectionName, m_keyName, m_section.m_filePath);
				if (!str.isEmpty())
				{
					std::string result = str.toStdString();
					char* buffer = strdup(result.c_str());
					return buffer;
				}
				// 返回空指针
				return nullptr;
			}

			template<typename T>
			operator T() const
			{
				return QVariant(IniHelper::Read(m_section.m_sectionName, m_keyName, m_section.m_filePath)).value<T>();
			}
			
			template<typename T>
			T operator()(const T& defaultValue) const
			{
				QVariant value = IniHelper::Read(m_section.m_sectionName, m_keyName, m_section.m_filePath);
				if (value.toString().isEmpty())
				{
						return defaultValue;
				}
				return value.value<T>();
			}
			
			Key& operator=(std::string value)
			{
				IniHelper::Write(m_section.m_sectionName, m_keyName, QString::fromStdString(value), m_section.m_filePath);
				return *this;
			}

			Key& operator=(ulong value)
			{
				IniHelper::Write(m_section.m_sectionName, m_keyName, QString::number(value), m_section.m_filePath);
				return *this;
			}

			Key& operator=(const char* value)
			{
				IniHelper::Write(m_section.m_sectionName, m_keyName, QString::fromStdWString(ToWString(value)), m_section.m_filePath);
				return *this;
			}

			template<typename T>
			Key& operator=(const T& value)
			{
				IniHelper::Write(m_section.m_sectionName, m_keyName, QVariant(value).toString(), m_section.m_filePath);
				return *this;
			}

		private:
			QString m_keyName;
			const Section& m_section;

			std::wstring ToWString(const char* str) const
			{
				int length = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
				std::wstring result(length, L'\0');
				MultiByteToWideChar(CP_UTF8, 0, str, -1, &result[0], length);
				return result;
			}
		};

		Key operator[](const QString& keyName) const
		{
			return Key(keyName, *this);
		}

	private:
		QString m_sectionName;
		QString m_filePath;
	};

	Section operator[](const QString& sectionName) const
	{
		return Section(sectionName, m_filePath);
	}
};

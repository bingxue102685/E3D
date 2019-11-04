#pragma once
#include "ECommon.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdarg.h>
#include <Windows.h>
#include <ctime>

namespace E3D
{
	//ȫ�ֵ�log�ļ���
	extern std::ofstream *GLogStream;

	//��ʼ��logϵͳ
	bool InitLog(const EString& fileName);

	//��¼��Ϣ��ʹ��ǰ��Ҫ����InitLog��ʼ��Log��������CloseLog�ر�Log�ر�
	void Log(const EChar* string, ...);

	//�ر�Logϵͳ
	void CloseLog();

	//��Χ�ڵ������
	//ʹ��ǰ��Ҫ�ֶ�����
	inline EInt RandomInt(EInt from = 0, EInt to = 10)
	{
		EInt ran = rand() % (to - from + 1) + from;
		return ran;
	}

	//����תΪ�ַ���
	inline EString IntToString(EFloat num) 
	{
		std::stringstream ss;
		ss << num;
		EString re;
		ss >> re;
		return re;
	}

	//�ַ���תΪ����
	inline EInt StringToInt(const EString &str)
	{
		return atoi(str.c_str());
	}

	//�ַ���תΪ������
	inline EFloat StringToFloat(const EString &str)
	{
		return (EFloat)atof(str.c_str());
	}

	//ת�ɿ��ַ���
	inline EWString ToEWString(const EString &str)
	{
		int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), (int)str.size(), NULL, 0);
		WCHAR *tString = new WCHAR[wcsLen + 1];

		::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), (int)str.size(), tString, wcsLen);

		//������'\0'
		tString[wcsLen] = '\0';

		return EWString(tString);
	}

	//��������������Ƿ����
	inline EBool EqualFloat(EFloat left, EFloat right)
	{
		return abs(left - right) <= EPSILON_E6;
	}

	//��·�����ȡ�ļ��������ֻ���ļ������򷵻�
	inline EString GetNameFromPath(const EString &path)
	{
		//ֻҪƥ�䵽�������ַ����������ַ��ͷ�����λ��
		std::size_t beg = path.find_last_of("\\/");
		std::size_t end = path.find_last_of(".");

		if (beg == EString::npos)
		{
			beg = -1;
		}

		return path.substr(beg + 1, end - beg - 1);
	}

	//���������ַ������˵Ŀհ��ַ�ɾ��
	inline EString Trim(const EString &msg)
	{
		/*
		\fƥ��һ����ҳ����leaf
		\nƥ��һ�����з���next
		\rƥ��һ���س�����enter
		\tƥ��һ���Ʊ����table
		\vƥ��һ����ֱ�Ʊ����vertical
		*/

		const static EString SPACE_CHAR = "\t\f\v\n\r";
		std::size_t	beg = msg.find_first_not_of(SPACE_CHAR);

		if (beg > msg.length())
			return EString();

		EString result = msg.substr(beg);
		std::size_t end = result.find_last_not_of(SPACE_CHAR);
		if (end != EString::npos)
			end++;

		return result.substr(0, end);
	}

	inline EFloat Clamp(EFloat value, EFloat min, EFloat max)
	{
		if (value < min)
		{
			return min;
		}

		if (value > max)
		{
			return max;
		}
		return value;
	}
}
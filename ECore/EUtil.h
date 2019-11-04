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
	//全局的log文件流
	extern std::ofstream *GLogStream;

	//初始化log系统
	bool InitLog(const EString& fileName);

	//记录信息，使用前需要调用InitLog初始化Log，并调用CloseLog关闭Log关闭
	void Log(const EChar* string, ...);

	//关闭Log系统
	void CloseLog();

	//范围内的随机数
	//使用前需要手动播种
	inline EInt RandomInt(EInt from = 0, EInt to = 10)
	{
		EInt ran = rand() % (to - from + 1) + from;
		return ran;
	}

	//整数转为字符串
	inline EString IntToString(EFloat num) 
	{
		std::stringstream ss;
		ss << num;
		EString re;
		ss >> re;
		return re;
	}

	//字符串转为整数
	inline EInt StringToInt(const EString &str)
	{
		return atoi(str.c_str());
	}

	//字符串转为浮点型
	inline EFloat StringToFloat(const EString &str)
	{
		return (EFloat)atof(str.c_str());
	}

	//转成宽字符串
	inline EWString ToEWString(const EString &str)
	{
		int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), (int)str.size(), NULL, 0);
		WCHAR *tString = new WCHAR[wcsLen + 1];

		::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), (int)str.size(), tString, wcsLen);

		//最后加上'\0'
		tString[wcsLen] = '\0';

		return EWString(tString);
	}

	//检测两个浮点数是否相等
	inline EBool EqualFloat(EFloat left, EFloat right)
	{
		return abs(left - right) <= EPSILON_E6;
	}

	//从路径里截取文件名，如果只有文件名，则返回
	inline EString GetNameFromPath(const EString &path)
	{
		//只要匹配到参数中字符串的任意字符就返回其位置
		std::size_t beg = path.find_last_of("\\/");
		std::size_t end = path.find_last_of(".");

		if (beg == EString::npos)
		{
			beg = -1;
		}

		return path.substr(beg + 1, end - beg - 1);
	}

	//将给定的字符串两端的空白字符删除
	inline EString Trim(const EString &msg)
	{
		/*
		\f匹配一个换页符，leaf
		\n匹配一个换行符，next
		\r匹配一个回车符，enter
		\t匹配一个制表符，table
		\v匹配一个垂直制表符，vertical
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
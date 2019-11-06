#pragma once

#include "ECommon.h"
#include "EOgreMeshInfo.h"
#include "ECore.h"
#include <fstream>

namespace E3D
{
	//从stream中读取字符串
	//size表示要读取字符串的个数，如果size == -1,则读取一行
	EString ReadString(std::ifstream &in, EInt size = -1);
	//读取下一个标识符  对应OGRMESHID
	EUShort ReadChunk(std::ifstream &in);

	//读取一个bool
	EBool ReadBool(std::ifstream &in);
	//读取一个unsigned int
	EUInt ReadInt(std::ifstream &in);
	//读取一个unsigned short
	EUShort ReadShort(std::ifstream &in);

	template<typename T>
	std::vector<T> ReadArray(std::ifstream &in, EInt size)
	{
		std::vector<T> v(size, 0);
		in.read((char*)(&v[0]), sizeof(T) *size);
		return v;
	}

	//加载一个Ogre .mesh格式的二进制文件，函数是想了对Ogre .mesh文件静态模型的正确读取
	//可以获取模型的顶点索引和顶点数据等模型数据，并返回一个EObject对象，如果模型有多个 submesh 则可以通过EObject.nextObject来获取下一个submesh的指针
	EObject4D* LoadOgreMesh(const EString &meshName, const EVector4D &worldPos = EVector4D::ZERO, const EVector4D &scale = EVector4D(1, 1, 1));
}
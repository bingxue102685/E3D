#include "EMaterial.h"
#include "EUtil.h"
#include "EMeshUtil.h"

namespace E3D
{
	EString ReadString(std::ifstream &in, EInt size = -1)
	{
		EString ss;
		if (size == -1)
		{
			EChar c;
			while (in.read(&c, sizeof(EChar)) && c != '\n' && c != '\r')
			{
				ss += c;
			}
		}
		else
		{
			EChar c[256];
			in.read(c, sizeof(EChar) * size);
			ss.append(c);
		}
		return ss;
	}

	EUShort ReadChunk(std::ifstream &in)
	{
		EUShort id;
		in.read((EChar*)&id, sizeof(EUShort));

		EUInt length;
		in.read((EChar*)&length, sizeof(EUInt));

		return id;
	}

	EBool ReadBool(std::ifstream &in)
	{
		EBool re = false;
		in.read((EChar*)&re, sizeof(EBool));
		return re;
	}

	EUInt ReadInt(std::ifstream &in)
	{
		EUInt re;
		in.read((EChar*)&re, sizeof(EUInt));
		return re;
	}

	EUShort ReadShort(std::ifstream &in)
	{
		EUShort re;
		in.read((EChar*)&re, sizeof(EUShort));
		return re;
	}

	EObject4D* LoadOgreMesh(const EString &meshName, const EVector4D &worldPos = EVector4D::ZERO, const EVector4D &scale = EVector4D(1, 1, 1))
	{
		std::ifstream in;
		in.open(GetPath(meshName).c_str(), std::ios_base::binary | std::ios_base::in);

		if (in.bad())
		{
			in.close();
			return NULL;
		}

		EString name = GetNameFromPath(meshName);
		//加载材质
		ParseMaterialScript(name + ".material");

		EObject4D* head = NULL;
		{
			//网格头信息
			EUShort HEADER = ReadShort(in);
			//网格版本号
			EString Version = ReadString(in);
			//网格标记
			EUShort MESH = ReadChunk(in);
			//蒙皮骨骼
			EBool SkeletallyAnimation = ReadBool(in);

			//子网格标记
			EUShort SUBMESH = ReadChunk(in);
			EObject4D *curObject = NULL;

			while (!in.eof() && (SUBMESH == M_SUBMESH))
			{
				std::vector<EUInt> indexArray;
				std::vector<EFloat> vertexArray;
				//对于有构造函数的类，不论有没有括号，都用构造函数进行初始化；
				//如果没有构造函数，则不加括号的new只分配内存空间，不进行内存的初始化，而加了括号的new会在分配内存的同时初始化为0。
				if (head == NULL)
				{
					head = new EObject4D;
					curObject = head;
				}
				else
				{
					curObject->nextObject = new EObject4D;
					curObject = curObject->nextObject;
				}

				curObject->name = name;
				curObject->state = EOBJECT_STATE_ACTIVE | EOBJECT_STATE_VISIBLE;
				curObject->worldPostion = worldPos;
				curObject->scale = scale;
				//材质信息
				EString Material = ReadString(in);
				curObject->materialName = Material;

				//顶点是否共享
				EBool useSharedVertices = ReadBool(in);
				//顶点数量
				EUInt indexCount = ReadInt(in);
				//顶点模型多边形的个数
				curObject->polyonNumber = indexCount / 3;
				curObject->polyonList.reserve(curObject->polyonNumber);
				//索引是否是32位的
				EBool indexIs32Bit = ReadBool(in);
				if (indexIs32Bit)
				{
					//存储面的顶点索引
					std::vector<EUInt> faceVertexIndices = ReadArray<EUInt>(in, indexCount);
					//读取模型顶点索引
					indexArray.assign(faceVertexIndices.begin(), faceVertexIndices.end());
				}
				else
				{
					//存储面的顶点索引
					std::vector<EUShort> faceVertexIndices = ReadArray<EUShort>(in, indexCount);
					//读取模型顶点索引
					indexArray.assign(faceVertexIndices.begin(), faceVertexIndices.end());
				}
				//网格几何信息
				EUShort GEOMETRY = ReadChunk(in);

				//顶点数量
				EUInt vertexCount = ReadInt(in);
				curObject->vertexNumber = vertexCount;
				curObject->localList.reserve(vertexCount);
				curObject->transformList.reserve(vertexCount);

				//网格几何顶点声明
				EUShort GEOMETRY_VERTEX_DECLARATION = ReadChunk(in);
				//网格几何顶点元素
				EUShort GEOMETRY_VERTEX_ELEMENT = ReadChunk(in);
				while (!in.eof() && GEOMETRY_VERTEX_ELEMENT == M_GEOMETRY_VERTEX_ELEMENT)
				{
					//绑定源缓冲区
					EUShort source = ReadShort(in);
					//顶点类型
					EUShort type = ReadShort(in);
					//顶点语义
					EUShort semantic = ReadShort(in);
					//在缓存中的偏移值
					EUShort offset = ReadShort(in);
					//颜色和纹理坐标的索引值
					EUShort index = ReadShort(in);

					GEOMETRY_VERTEX_ELEMENT = ReadChunk(in);
				}
				//顶点缓存
				EUShort GEOMETRY_VERTEX_BUFFER = GEOMETRY_VERTEX_BUFFER;
				//把索引绑定到缓存中
				EUShort bingIndex = ReadShort(in);
				//每个顶点的大小必须与索引声明保持一致
				EUShort vertexSize = ReadShort(in);

				//结合顶点缓存数据
				EUShort GEOMETRY_VERTEX_BUFFER_DATA = ReadChunk(in);

				EInt vetexNum = vertexSize / sizeof(EFloat);
				//数据缓存大小
				EInt bufferSize = vertexCount * vertexCount;
				//读取顶点数据
				vertexArray = ReadArray<EFloat>(in, bufferSize);
				for (EInt i = 0; i < (EInt)vertexArray.size(); i += vetexNum)
				{
					EVertex4D vex;

					vex.x = vertexArray[i];
					vex.y = vertexArray[i + 1];
					vex.z = vertexArray[i + 2];

					vex.u = vertexArray[i + 6];
					vex.v = vertexArray[i + 7];

					curObject->localList.push_back(vex);
					curObject->transformList.push_back(vex);
				}

				for (EInt i = 0; i < curObject->polyonNumber; i++)
				{
					EInt index = i * 3;
					EPolyon4D poly;
					poly.state = EPOLY_STATE_ACTIVE;
					poly.attribute = EPOLY_ATTR_VERTEX_NORMAL | EPOLY_ATTR_VERTEX_POSITION | EPOLY_ATTR_VERTEX_UV;
					poly.verIndex[0] = indexArray[index];
					poly.verIndex[1] = indexArray[index + 1];
					poly.verIndex[2] = indexArray[index + 2];

					//这里将变换后的顶点指针交给每个多边形，是为了防止变换后变换之后点索引依旧是最初的顶点坐标
					poly.verList = &curObject->transformList;
					curObject->polyonList.push_back(poly);
				}

				//子网格位置
				EUShort SUBMESH_OPERATION = ReadChunk(in);
				//操作类型
				EUShort operationType = ReadShort(in);

				SUBMESH = ReadChunk(in);
			}
			//网格包围盒
			EUShort MESH_BOUNDS = SUBMESH;

			if (MESH_BOUNDS == M_MESH_BOUNDS)
			{
				//最小包围盒和最大包围盒及计算半径
				std::vector<EFloat> bounds = ReadArray<EFloat>(in, 7);
				head->minBoundingBox = EVector4D(bounds[0], bounds[1], bounds[2]);
				head->maxBoundingBox = EVector4D(bounds[3], bounds[4], bounds[5]);
				head->avgRadius = (head->maxBoundingBox - head->minBoundingBox).length() / 2;

				EFloat minR = head->minBoundingBox.length();
				EFloat maxR = head->maxBoundingBox.length();
				head->maxRadius = maxR > minR ? maxR : minR;

				//子网格名字列表
				EUShort SUBMESH_NAME_TABLE = ReadChunk(in);
				if (SUBMESH_NAME_TABLE == M_SUBMESH_NAME_TABLE)
				{
					//子网格名字列表元素
					EUShort SUBMESH_NAME_TABLE_ELEMENT = ReadChunk(in);
					while (!in.eof() && SUBMESH_NAME_TABLE_ELEMENT == M_SUBMESH_NAME_TABLE_ELEMENT)
					{
						//名字和索引
						EShort index = ReadShort(in);
						EString name = ReadString(in);
						SUBMESH_NAME_TABLE_ELEMENT = ReadChunk(in);
					}
				}
			}
		}
		in.close();
		return head;
	}
}
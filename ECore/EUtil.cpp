#include "EUtil.h"

namespace E3D
{
	std::ofstream *GLogStream = NULL;

	bool InitLog(const EString &fileName)
	{
		if (GLogStream == NULL) 
		{
			GLogStream = new std::ofstream();
			GLogStream->open(fileName.c_str());
			if (GLogStream->good())
				return true;

			return false;
		}
		return true;
	}

	void Log(const EChar *format, ...)
	{
		if (GLogStream == NULL)
			return;

		if (!format || !GLogStream)
			return;

		EChar buffer[256];

		//�ɱ�����б�
		va_list arglist;	
		//������ջ�ϵĿɱ�Ĳ�����ӡ����ַ���
		va_start(arglist, format);
		vsprintf(buffer, format, arglist);
		va_end(arglist);

		EString info(buffer);
		//�������Ϣ
		*GLogStream << info << std::endl;
		GLogStream->flush();
	}

	void CloseLog()
	{
		GLogStream->close();
		SafeDelete(GLogStream);
	}
}
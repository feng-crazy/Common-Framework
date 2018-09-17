/*****************************************************************************
	  COPYRIGHT: 2009 Edan Instruments, Inc. All rights reserved
	PROJECTNAME: Mx
		VERSION: 1.0
	   FILENAME: XmlFileManager.h
		CREATED: 2009.11.30
		 AUTHOR: GuanHui
	DESCRIPTION:
		Mx软件使用的配置文件管理类的声明部分.
		这个类主要是用来管理配置文件的,实现配置文件的备份,删除,拷贝等功能.
*******************************************************************************/
#ifndef XMLFILEMANAGER_H
#define XMLFILEMANAGER_H
#include "MFrame/Singleton.h"
#include <string>
using namespace std;


class XmlFileManager
{
	public:
		bool copy_from_backup_file(const string &dest_fname);
		bool copy_file(const string &dst_file_name, const string &src_fname);
		bool delete_file(const string &dst_file_name);
		~XmlFileManager();
	private:
		XmlFileManager();

	DECLARE_SINGLETON_CLASS(XmlFileManager);
};

typedef Singleton<XmlFileManager> XmlFileManagerSingleton;

#endif // XMLFILEMANAGER_H

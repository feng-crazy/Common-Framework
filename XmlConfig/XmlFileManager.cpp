/*****************************************************************************
	  COPYRIGHT: 2009 Edan Instruments, Inc. All rights reserved
	PROJECTNAME: Mx
		VERSION: 1.0
	   FILENAME: XmlFileManager.h
		CREATED: 2009.11.30
		 AUTHOR: GuanHui
	DESCRIPTION:
	Mx软件使用的配置文件管理类.通过调用system系统调用来对系统配置文件
	进行管理,目前所做的主要工作主要是第一次开机的时候从/etc/edan/
	目录中拷贝我们所需要的文件到/home/edan/etc/文件夹下面去.
	当配置文件出错的时候,也可以从备份里拷贝一份配置到/home/edan/etc/
	这个文件夹下面来.
	MODIFICATION:GuanHui.2009.12.15
		增加配置文件的复制与删除功能.
		增加这个功能的目的是每次XmlEditor保存配置文件
		前对此配置文件作临时备份,XmlEditor保存完毕之后就又删除temp文件.
		因此,若下一次开机的时发现存在temp文件存在,可认为上一次XmlEditor保存失败。
		就利用这个temp文件来恢复配置文件.类似于word的备份工作原理.
*******************************************************************************/
#include "XmlFileManager.h"
#include <iostream>
#include <fstream>
#include <iostream>

extern "C"
{
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
}

const string SRC_BACKUP_PATH = "/etc/edan/";
#define READ_BUFF_SIZE 2048

/******************************************************************************
AUTHOR: GuanHui. 2009.11.30
DESCRIPTION: 从备份的文件夹中拷贝一份配置到/home/edan/etc/去.

PARAMETERS:dest_fname:/home/edan/etc/文件夹下的某个目标文件绝对路径名.

RETURN:
	true:厂家默认配置文件拷贝成功
	false:厂家默认配置文件拷贝失败
******************************************************************************/
bool XmlFileManager::copy_from_backup_file(const string &dest_fname)
{
	string src_path;
	string dest_path = dest_fname;
	string::size_type index = dest_fname.find_last_of('/');
	if(index == string::npos)
	{
		return false;

	}

	//get the source file name path.
	//如果是MxConfig.xml需要拷贝,则用/etc/edan/中MxConfigAdult.xml作为拷贝源
	if(dest_fname.substr(index + 1) == "MxConfig.xml")
	{
		src_path = SRC_BACKUP_PATH + "MxConfigAdult.xml";
	}
	else
	{
		src_path = SRC_BACKUP_PATH + dest_fname.substr(index+1);
	}


	//get the Shell command string.
	//生成系统调用的shell命令.
	string cmd_str = "cp " + src_path + " " + dest_path;

	warning("no config file or file error:%s\n", dest_path.c_str());
	warning("copy from backup directory:\n%s ===> %s\n",
		src_path.c_str(), dest_path.c_str());

	int ret;
	//copy file.
	//通过system来调用shell.
	ret = system(cmd_str.c_str());

	if(ret == 0)
	{
		//更改拷贝后文件的属性
		string chmod_str = "chmod 664 "+dest_path;//rw+rw+r
		system(chmod_str.c_str());
		return true;
	}
	else
	{
		warning("\ncopy from backfile error: %s\n", strerror(errno));
		return false;
	}

}


/******************************************************************************
AUTHOR: GuanHui, 2009.12.15
DESCRIPTION:
	文件拷贝函数.该函数调用C的fread, fwrite的方法实现对文件的拷贝.
	其实也可以用如下的C++ fstream方法简便的实现文件的复制:
	dest_file_stm << src_file_stm.rdbuf();但这样做效率相对要低一些.
PARAMETERS:
	dst_file_name:要拷贝为的目标文件绝对路径
	src_file_name:作为拷贝源文件的绝对路径
RETURN:
	true:拷贝成功
	false:拷贝失败
******************************************************************************/
bool XmlFileManager::copy_file(const string &dst_file_name,
		const string &src_file_name)
{
	FILE *src_file, *dest_file;
	src_file = fopen(src_file_name.c_str(), "rb");
	if(src_file == NULL)
	{
		warning("create src file error: %s ---%s\n", src_file_name.c_str(),
				strerror(errno));
		return false;
	}

	dest_file = fopen(dst_file_name.c_str(), "wb");
	if(dest_file == NULL)
	{
		warning("create dest file error: %s ---%s\n", dst_file_name.c_str(),
				strerror(errno));
		fclose(src_file);
		return false;
	}

	//通过中间缓存的方式分批将文件内容拷贝入新文件中.
	char temp_buf[READ_BUFF_SIZE];
	int read_num;
	int write_num;

	//如果src_file还未到文件末尾,则表示可以继续读写文件.
	while( feof(src_file) == 0 )
	{
		//读出read_num个字节的内容.同时将文件指针往后移动.
		read_num = fread(temp_buf, sizeof(char), READ_BUFF_SIZE, src_file);
		if(read_num > 0)
		{
			write_num = 0;

			//为保证完全写入而做的保护措施.
			while(write_num < read_num)
			{

				write_num += fwrite(temp_buf + write_num, sizeof(char),
						read_num - write_num, dest_file);
			}
		}
	}

	//fflush(dest_file_stm);
	//fclose内部会调用fflush的过程将文件内容刷到磁盘上.
	fclose(src_file);
	fclose(dest_file);

	return true;
}



/******************************************************************************
AUTHOR: GuanHui. 2009.12.15
DESCRIPTION:
	删除某个文件.
PARAMETERS:
	src_file_name:需要删除的文件的绝对路径名.
RETURN:
		true:删除成功.
		false:删除失败.
******************************************************************************/
bool XmlFileManager::delete_file(const string &dst_file_name)
{
	int ret;
	ret = remove(dst_file_name.c_str());
	if(ret == 0)
	{
		return true;
	}
	else
	{
		warning("remove backup file error: %s %s\n", dst_file_name.c_str(),
				strerror(errno));
		return false;
	}
}


/******************************************************************************
AUTHOR: GuanHui. 2009.11.30
DESCRIPTION:Constructor

PARAMETERS:

RETURN:
******************************************************************************/
XmlFileManager::XmlFileManager()
{

}


/******************************************************************************
AUTHOR: GuanHui. 2009.11.30
DESCRIPTION: deconstructor

PARAMETERS:

RETURN:
******************************************************************************/
XmlFileManager::~XmlFileManager()
{

}

/******************************************************************************
	  COPYRIGHT: 2008 Edan Instruments, Inc. All rights reserved
	PROJECTNAME: Mx
	    VERSION: 1.0
	   FILENAME: SyncDisk.h
	    CREATED: 2011.12.19
	     AUTHOR: 张虎
	DESCRIPTION: 保存配置文件到磁盘。
******************************************************************************/
#ifndef _SYNC_DISK_H_
#define _SYNC_DISK_H_

#include <set>
#include "MFrame/MThread.h"


class SyncDiskIFace
{
public:
	virtual int save_file(void) = 0;

	SyncDiskIFace();
	virtual ~SyncDiskIFace();
};


class SyncDisk : public MThread
{
	public:
		bool register_sync_iface(SyncDiskIFace &disk_iface);

		SyncDisk();
		~SyncDisk();

	protected:
		void setup_thread(void);
		void thread_run(void);

	private:
		std::set<SyncDiskIFace*> _disk_iface;
};

//将其制作成单件类，这样所有磁盘同步都只创建一个线程。
#define syncdisk (Singleton<SyncDisk>::instance())

#endif


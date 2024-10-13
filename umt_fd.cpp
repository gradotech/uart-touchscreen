#include "umt.h"
#include "umt_fd.h"

#include <fcntl.h>
#include <unistd.h>

#include <cassert>

FDManager::FDManager(const char *file, int flags)
{
	UMT_LOG(UMT_DBG_LOG, "Opening file %s\n", file);

	fd = open(file, flags);
	assert(fd > -1);

	UMT_LOG(UMT_DBG_LOG, "Opened file %s, fd %d\n", file, fd);
}

FDManager::~FDManager()
{
	UMT_LOG(UMT_DBG_LOG, "Closing fd %d\n", fd);

	close(fd);
	fd = -1;
}

int FDManager::getFd()
{
	return fd;
}

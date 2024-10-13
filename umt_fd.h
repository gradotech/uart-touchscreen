#ifndef __UMT_FD_H__
#define __UMT_FD_H__

class FDManager
{
public:
	FDManager(const char *file, int flags);
	~FDManager();

	int getFd();

private:
	int fd;
};

#endif /* __UMT_FD_H__ */

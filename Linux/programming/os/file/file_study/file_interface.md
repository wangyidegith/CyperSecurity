rm : 
	remove(unlink and rmdir) 
	unlink(no dir) 
	rmdir(only empty dir) 
	unlinkat(complex)



dir : 
	#include <dirent.h>
	DIR *opendir(const char *name);   // DIR is a handle
	struct dirent *readdir(DIR *dirp);   // struct dirent has d_name char[] that is current item name
	int closedir(DIR *dirp);



max_filename_length : 
	long max_filename_length = pathconf("/", _PC_NAME_MAX);



stat : 
	struct stat item_stat;
	if (stat(file_path, &item_stat)) {
        	return -1;
	}
	if (S_ISDIR(item_stat.st_mode)) {
		// process dir
	}



打开文件 : 4个
int open(const char *pathname, int flags);   // flags default is append

int open(const char *pathname, int flags, mode_t mode);   // // flags default is append
O_RDONLY：只读方式打开文件。
O_WRONLY：只写方式打开文件。
O_RDWR：读写方式打开文件。
O_CREAT：如果文件不存在，则创建一个新文件。需要与其他标志位一起使用。
O_TRUNC：如果文件存在且以写入方式打开，则将文件截断为0字节。
O_APPEND：以追加方式打开文件，每次写入时都在文件末尾添加内容。
O_EXCL：与 O_CREAT 一起使用，如果文件已经存在，则打开失败。
O_NONBLOCK：以非阻塞方式打开文件。
O_SYNC：每次写入都要等待物理 I/O 操作完成。

int creat(const char *pathname, mode_t mode);   // flags is trunc

FILE *fopen(const char *pathname, const char *mode);   // except r and r+(not existed, open fail), flags default is w and w+ is trunc, a and a+ is append

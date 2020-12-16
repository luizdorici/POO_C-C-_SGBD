#ifndef _FILESYSTEMENTRY_H_
#define _FILESYSTEMENTRY_H_

#include "string.h"

class FileSystemEntry {
	private:
		String *name;

	public:
		FileSystemEntry() {
			this->name = NULL;
		}

		FileSystemEntry(char *name) {
			this->name = new String(name);
		}

		FileSystemEntry(String *name) {
			this->name = name;
		}

		virtual ~FileSystemEntry(){
			if (this->name != NULL)
				delete this->name;
		}

		String *getName() { return this->name; }

		bool setPermission(bool owner_read, 
				bool owner_write, 
				bool owner_execute) {

			mode_t mode = 0;

			// bitwise operator
			mode |= owner_read ? S_IRUSR : 0;
			mode |= owner_write ? S_IWUSR : 0;
			mode |= owner_execute ? S_IXUSR : 0;

			if (!chmod(this->name->toCharArray(), mode))
				return true;
			return false;
		}

		bool setPermission(bool owner_read, 
				bool owner_write, 
				bool owner_execute,
				bool group_read, 
				bool group_write, 
				bool group_execute) {

			mode_t mode = 0;

			// bitwise operator
			mode |= owner_read ? S_IRUSR : 0;
			mode |= owner_write ? S_IWUSR : 0;
			mode |= owner_execute ? S_IXUSR : 0;

			mode |= group_read ? S_IRGRP : 0;
			mode |= group_write ? S_IWGRP : 0;
			mode |= group_execute ? S_IXGRP : 0;

			if (!chmod(this->name->toCharArray(), mode))
				return true;
			return false;

		}

		bool setPermission(bool owner_read, 
				bool owner_write, 
				bool owner_execute,
				bool group_read, 
				bool group_write, 
				bool group_execute,
				bool others_read, 
				bool others_write, 
				bool others_execute) {

			mode_t mode = 0;

			// bitwise operator
			mode |= owner_read ? S_IRUSR : 0;
			mode |= owner_write ? S_IWUSR : 0;
			mode |= owner_execute ? S_IXUSR : 0;

			mode |= group_read ? S_IRGRP : 0;
			mode |= group_write ? S_IWGRP : 0;
			mode |= group_execute ? S_IXGRP : 0;

			mode |= others_read ? S_IROTH : 0;
			mode |= others_write ? S_IWOTH : 0;
			mode |= others_execute ? S_IXOTH : 0;

			if (!chmod(this->name->toCharArray(), mode))
				return true;
			return false;
		}
};

#endif

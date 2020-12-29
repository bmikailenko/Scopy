// Ben Mikailenko

// scopy.c


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

// returns 1 if file exists
int exists(const char *fname) {
	if( access( fname, F_OK ) != -1 ) {
		return 1;
	} else {
		return 0;
	}
}

// returns 1 if file is a link
int isLink(const char *fname) {
  struct stat buf;
	int x;
	x = lstat(fname, &buf);
  if (S_ISLNK(buf.st_mode)) return 1;
  return 0;
}

// returns 1 if file/acl/scopy are owned by same person
int isOwner(const char *fname, const char *aclfname) {

	// get uid of file
	struct stat buf;
	int x;
	x = stat(fname, &buf);

	// get uid of file.acl
	struct stat aclbuf;
	int y;
	y = stat(aclfname, &aclbuf);

	// get uid of scopym
	struct stat scopybuf;
	int z;
	z = stat("scopy", &scopybuf);

	// if file and file.acl dont have the same owner, return false
	if (buf.st_uid != aclbuf.st_uid) return 0;

	// if file and scopy dont have the same owner, return false
	if (buf.st_uid != scopybuf.st_uid) return 0;

	// return true
	return 1;
}

// returns 1 if file dosen't have group access
int noGroupAccess(const char *fname) {

	struct stat aclbuf;
	int y;
	y = stat(fname, &aclbuf);

	if (aclbuf.st_mode & S_IRWXG) return 0;
	if (aclbuf.st_mode & S_IRGRP) return 0;
	if (aclbuf.st_mode & S_IWGRP) return 0;
	if (aclbuf.st_mode & S_IXGRP) return 0;

	// return true
	return 1;
}

// returns 1 if file is in valid acl file format
int isValid(const char *fname) {
	// variables
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	// open the file
	fp = fopen(fname, "r");

	// go through each line
	while ((read = getline(&line, &len, fp)) != -1) {

		int lineHasSpace = 0;

		// for each character in a line
		for (size_t i = 0; i < len; i++){

			// if there are more than two spaces, return false
			if(*(line+i) == 32) lineHasSpace++;
			if(lineHasSpace > 1) return 0;

			if(*(line+i) == 10) break;
		}

		// if there aren't any spaces, return false as well
		if(lineHasSpace == 0) return 0;
	}

	// close the file
	fclose(fp);

	return 1;
}

// returns 1 if username is in the file
int isAllowed(const char *fname) {

	char *buf;
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	// get username
	buf = (char *)malloc(10*sizeof(char));
	buf = getlogin();

	// open the file
	fp = fopen(fname, "r");

	// go through each line
	while ((read = getline(&line, &len, fp)) != -1) {

		// username pointer
		int x = 0;

		// for each character in a line
		for (size_t i = 0; i < len; i++){

			// if reached end of name, next line
			if(*(line+i) == 32) break;

			// successfully matched a letter, or reset username pointer
			if(buf[x] == *(line+i)) x++;
			else x = 0;

			// sucessfully matched whole username
			if(buf[x] == 0) return 1;

		}
	}

	// close the file
	fclose(fp);

	return 0;
}

void printError() {
	printf("Access Denied\n");
	return;
}

int main(int argc, char *argv[]) {

	// concat argv[1] with .acl
	char aclStr[50], acl[50];
  strcpy(acl, argv[1]);
  strcpy(aclStr,  ".acl");
  strcat(acl, aclStr);

	// tests

	// if acl doesn't exist, exit
	if (exists(acl) == 0) {
		printError();
		return 0;
	}

	// if acl is a link, exit
	if (isLink(acl) == 1) {
		printError();
		return 0;
	}

	// if file is a link, exit
	if (isLink(argv[1]) == 1) {
		printError();
		return 0;
	}

	// if acl file is invalid, exit
	if (isValid(acl) == 0) {
		printError();
		return 0;
	}

	// if files don't share one owner, exit
	if (isOwner(argv[1], acl) == 0) {
		printError();
		return 0;
	}

	// if acl file allows group access, exit
	if (noGroupAccess(acl) == 0) {
		printError();
		return 0;
	}

	// if user is in the acl
	if (isAllowed(acl) == 0) {
		printError();
		return 0;
	}

	// open the file
	FILE *in, *out;

	in = fopen(argv[1], "r");
	out = fopen(argv[2], "w");

	char c;

	c = fgetc(in);

	while(c != EOF) {
		fputc(c, out);
		c = fgetc(in);
	}

	printf("successfully copied file\n");

	fclose(in);
	fclose(out);

	return 0;
}

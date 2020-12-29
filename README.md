Ben Mikailenko
scopy

files:
	scopy.c: the c code
	Makefile: the makefile
	README.txt: the readme

to configure:

	run "make"

to run:

	./scopy "name of file" "name of new file"

About this program:

	this program is a secure way of copying files between users. A user wishing to make a file avalible to other users simple makes a seperate file with an ".acl" extention. 

ex: file.txt.acl

Then, the user may enter a username followed by a space followed by a r/w/b permission for the users they allow to access the file. 

Then, place a copy of scopy (MUST BE OWNED) into a user shared directory. 

The user wishing to download the file will run:

	"directory"/scopy "directory"/file "directory"/newfile
	where "directory" is replaced with the directory of the files. 

scopy will then check to make sure the files are valid, unmodified, have the correct permissions, and that the .acl file contains the user name of the user wishing to copy the file. 

If the checks are good, the file will copy to the newfile directory. 


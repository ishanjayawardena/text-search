text-search
===========

A simple text search engine for Linux based systems.

PLEASE READ THIS FILE BEFORE YOU DO ANYTHING.

How to install the program
--------------------------
	* Chage the directory to 'src'.
	  
	* To make the executable, type
	  make + <Enter>
	  
	* To install the executable to the default installation directory, type
	  make install + <Enter>
	  
	  The default installation directory is '/usr/local/bin'.
	  If you want to install the executable into a different directory, just
	  change the value of the variable named INSTDIR to your desired path before
	  you invoke 'make install' command. This INSTDIR is found in the makefile.
	  
	  e.g. if you want to install to your /usr/bin directory, change the line in
	  the make file INSTDIR = /usr/local/bin into INSTDIR = /usr/bin
	  
	  NOTE: To execute the 'make install' command, sometimes you may require
	  		root privileges. If so, please consult your administrator.
	  		
	* to clean all the .o files after the compilation, type
	  make clean + <Enter>
	  
	* if you need to remove the executable together with the .o files, type
	  make cleanall + <Enter>.
	  
How to run the program
----------------------
	* you can always try out the program just after making it.
	  i.e. after you type 'make'+<Enter>, if the program compiles and
	  if the executable is made without any error, you can test it by typing
	  ./search + <Enter> (see 'invoking' subsection below).
	  
	* if you installed the executable to one of the standard paths of your terminal
	  you can run it by just typing 
	  search + <Enter> (see 'invoking' subsection below)
	  (the default path in the makefile is a standard path.)
	  
	invoking the program:
	---------------------
		NOTE: I assume that you have installed the program in to the default path.
		
		+ The name of the executable is 'search'
		+ Lets first look at how to search the working directory.
		
		+ to invoke 'search' just type
		  search + <Enter>
		
		+ when you run it with no options, it searches the text files in the current
		  working directory and stores all the words in those files to a data struc
		  -ture.( If any file or any subdirectory cannot be opened, 'search' just 
		  skips it and continues to work with the rest.) 
			
		+ Then it prints a prompt 'query>> '
	    
	    + Now you can give your query of words to search.
	      e.g 'query >> for '
	    	this will find and print the absolute paths of all the files
	    	those contain the word 'for'.
	    	
	    + If you want to do a logical 'and' or 'or' search for any number of words
	      greater than 1, just type
	    	'for | while' to do a logical or search on words 'for' and 'while'.
	    		then, 'search' will print all the files that contain the word
	    		either 'for' or 'while'.
	    	
	    	'for & while' to do a logical and search on words 'for' and 'while'.
	    		then, 'search' will print all the files that contain both the
	    		words 'for' and 'while'.
	    		
	    	if no result is found, 'search' will display a message.
	    	
	    + if you want to search the text files in /usr/include/ directory,
	      just give the path as an option when you invoke 'search'.
	      
	      search -d /usr/include + <Enter>
	      
	      -d option is used to provide a root directory structure to 'search'
	      from where to start searching.
	      If 'search' fails to open the root directory given in -d option, 
	      it will print an error message and die.
	      
	    + to exit the program, type '.' and press <Enter>.
	    	
	

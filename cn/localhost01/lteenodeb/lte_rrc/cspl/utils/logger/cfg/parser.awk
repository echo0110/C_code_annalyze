###################################################################
###  Copyright (C) 2006 Aricent Inc . All Rights Reserved ##
###################################################################
###################################################################
#
#	awk script for extracting the
#	 info
#
#	Author	  :	Guninder Preet Singh	
#
###################################################################


BEGIN { FS =  "[\t| ]+" } 


BEGIN	{

 
		TEMP_FILE = "api.c"
		STR = "%d	%s"
		STR1 = "%s"
		system("rm api.c");
		system("touch api.c")
		CFG_FILE = sprintf("%s%s",ENVIRON["CFG_FILE"],".cfg");
		MOD_SECTION_NAME_BEGIN = sprintf("[%s]",ENVIRON["CFG_FILE"]);
		MOD_SECTION_NAME_END = sprintf("[/%s]",ENVIRON["CFG_FILE"]);
		
		printf "%s","#include <stdio.h>\n" >> TEMP_FILE;
		printf "%s","#include " >> TEMP_FILE;
		printf"%c", 34 >>TEMP_FILE;
		printf "%s","api.h" >> TEMP_FILE;
		printf"%c\n", 34 >>TEMP_FILE;

		printf "%s\n","main()" >> TEMP_FILE;
		printf "%c\n", "{" >>TEMP_FILE;

		printf "%s%c\n","FILE		*fp=NULL",59 >> TEMP_FILE;
		printf "fp=stdout;\n" >> TEMP_FILE
		printf "fprintf( fp, " >> TEMP_FILE;
		printf "%c%s%c)%s\n", 34,MOD_SECTION_NAME_BEGIN,34,";" >> TEMP_FILE;
		printf "fprintf( fp," >> TEMP_FILE;
		printf "%c%c%s%c)%s\n", 34,92,"n",34,";" >> TEMP_FILE;

		i = 0;
		while ( getline)
		{

			if ( $1 == "#define" )
			{
				if ( match($2,"_") == 1)
				{
					continue;
				}
				printf "fprintf( fp, " >> TEMP_FILE;
				printf"%c", 34 >>TEMP_FILE;
				printf "%s", STR >> TEMP_FILE;
				printf "%c%s",92,"n" >> TEMP_FILE;
				printf"%c", 34 >>TEMP_FILE;
				printf ", %s, ",$2 >> TEMP_FILE;	
				printf"%c", 34 >>TEMP_FILE;
				printf "%s",$2 >> TEMP_FILE;	
				printf"%c", 34 >>TEMP_FILE;
				printf ")%c\n",59 >> TEMP_FILE;
			}

		}

		printf "fprintf( fp, " >> TEMP_FILE;
		printf "%c%s%c)%s\n", 34,MOD_SECTION_NAME_END,34,";" >> TEMP_FILE;
		printf "fprintf( fp," >> TEMP_FILE;
		printf "%c%c%s%c)%s\n", 34,92,"n",34,";" >> TEMP_FILE;
		printf "fprintf( fp," >> TEMP_FILE;
		printf "%c%c%s%c)%s\n", 34,92,"n",34,";" >> TEMP_FILE;

		printf "%c\n", "}" >>TEMP_FILE;
		close(TEMP_FILE);
	}

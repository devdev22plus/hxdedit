#include <iostream>
#include <stdio.h>
#include <string.h>
#include <map>
#include <iomanip>

#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

int main(int argc, const char * argv[])
{
	// example how to use call program with arguments scacheserver.exe cacheserver.xml cacheserve2.xml
	//
	//
	/* std::cout << "argc:" << argc << std::endl; */
	/* for(int i = 0 ; i < argc ; ++i) */
	/* { */
	/* 	std::cout << argv[i] << std::endl; */
	/* } */

	if ( argc != 5 )
	{
		std::cout << "please enter argument [input file name] [output file name] [find string] [replace string]" << std::endl;
		return 0;
	}

	const char * szInputFile = argv[1];
	const char * szOutputFile = argv[2];
	const char * szFind = argv[3];
	const char * szReplace = argv[4];

	if ( strcmp(szReplace, "none") != 0 )
	{
		/* if ( strlen(szFind) != strlen(szReplace) ) */
		if ( strlen(szFind) < strlen(szReplace) )
		{
			/* std::cout << "find and replace length not same" << std::endl; */
			std::cout << "can't replace " << szReplace << " because length is over find" << std::endl;
			return 0;
		}
	}

	/* const char * szInputFile = "CacheServer.exe"; */
	/* const char * szOutputFile = "cacheserver2.exe"; */

	FILE *pFile = fopen(szInputFile, "rb");
	if (pFile)
	{
		std::cout << "Start searching and replace hex..." << std::endl;

		fseek(pFile, 0 , SEEK_END);
		auto fileSize = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);

		char * szContent = new char[fileSize];
		auto size = fread( szContent, sizeof(char) , fileSize, pFile );

		fclose(pFile);

		if ( size > 0 )
		{
			auto funcFind = [=](const char *szFind) -> std::map<int, const char*> {
				std::map<int, const char*> mapFound;

				/* const char * szFind = ".xml"; */
				/* const char * szFind = "cacheserver.xml"; */
				int nFindLen = strlen(szFind);
				
				
				for(int i = 0 ; i < fileSize ; ++i)
				{
					bool bFound = true;
					for(int n = 0 ; n < nFindLen ; ++n)
					{
						char *szContentTemp = strdup( &szContent[i+n] ); // make copy
						if ( tolower(*szContentTemp) != szFind[ n ] ) // set to lower before check
						{
							bFound = false;
							free(szContentTemp);
							break;
						}

						free(szContentTemp);
					}

					if ( bFound )
					{
						int nStartInd = 0;
						while(true)
						{
							nStartInd++;

							if (szContent[i - nStartInd] == '\0')
							{
								break;
							}
						}

						int nFoundIndex = i - (nStartInd - 1);
						const char * szFoundContent = &szContent[nFoundIndex];
						/* std::cout << "found "  << (void*)nFoundIndex << ": " << szFoundContent << std::endl; */

						mapFound.insert( std::make_pair(nFoundIndex, szFoundContent) );
					}
				}


				return mapFound;
			};


			auto mapFound = funcFind(szFind);
			std::map<int, const char*>::iterator iter, iter_end;

			iter = mapFound.begin();
			iter_end = mapFound.end();
			for(; iter != iter_end ; ++iter)
			{
				auto nFoundIndex = iter->first;
				auto szFoundContent = iter->second;
				/* const char *szReplace = "cacheserve2.xml"; */

				/* std::cout << "found "  << (void*)nFoundIndex << ": " << szFoundContent << std::endl; */


				// do replace here
				std::cout << "replace " << szFoundContent << " -> " << szReplace << std::endl;

				if ( strcmp(szReplace, "none") == 0 )
				{
					szContent[nFoundIndex] = '\0';
				}
				else
				{
					for(int i = 0 ; i < (int)strlen(szReplace) ; ++i)
					{
						szContent[nFoundIndex + i] = szReplace[i];
					}
				}

				break;
			}


			FILE *pFileOutput = fopen(szOutputFile, "wb");
			if ( pFileOutput )
			{
				for(int i = 0 ; i < fileSize ; ++i)
				{
					fwrite(&szContent[i], sizeof(char), 1, pFileOutput);
					/* std::cout << " " << std::setfill('0') << std::setw(2) << std::hex << (0xff & (unsigned int)szContent[i]); */

					/* if (i > 130) */
					/* 	break; */
				}
				fclose(pFileOutput);

				std::cout << std::endl;

				std::cout << "Write file success." << std::endl;
			}
			else std::cout << "Can't open file for write at " << szOutputFile << std::endl;
		}



		free(szContent);

		std::cout << "Program End." << std::endl;
	}
	else std::cout << "Not found input file." << std::endl;

	return 0;
}


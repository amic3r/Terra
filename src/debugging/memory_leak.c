
#include "stdafx.h"

#include "memory_leak.h"

#if defined(_MSC_VER) && defined(_DEBUG)
void ParseMemoryLeakFile(char *_inputFilename, char *_outputFilename)
{
	// Start up
	BTree <char *, int> combined;
	BTree <char *, int> frequency;
	int unrecognised = 0;

	// Open the file and start parsing
	ifstream memoryfile(_inputFilename);

	while(!memoryfile.eof()) {
		char thisline [256];
		memoryfile.getline(thisline, 256);

		if(!strncmp(thisline, " Data:", 6) == 0 && strchr(thisline, ':')) {
			// Get the size
			char *lastcomma = strrchr(thisline, ',');
			char *ssize = lastcomma+2;
			int size;
			char unused [32];
			sscanf(ssize, "%d %s", &size, unused);

			// Get the source file name
			char *sourcelocation = thisline;
			char *colon = strrchr(thisline, ':');
			*(colon-1) = '\x0';
			char *lowercasesourcelocation = LowerCaseString(sourcelocation);

			// Put the result into our BTree
			int btree = combined.find(lowercasesourcelocation);
			if(btree)			combined.replace(lowercasesourcelocation, btree + size);
			else				combined.insert(lowercasesourcelocation, size);

			int freq = frequency.find(lowercasesourcelocation);
			if(freq)			frequency.replace(lowercasesourcelocation, freq + 1);
			else 				frequency.insert(lowercasesourcelocation, 1);

			delete lowercasesourcelocation;
		} else {
			char *lastcomma = strrchr(thisline, ',');
			if(lastcomma) {
				char *ssize = lastcomma+2;
				int size;
				char unused [32];
				sscanf(ssize, "%d %s", &size, unused);

				unrecognised += size;
			}
		}
	}

	memoryfile.close();

	// Sort the results into a list
	DArray <int> *sizes = combined.ConvertToDArray();
	DArray <char *> *sources = combined.ConvertIndexToDArray();
	LList <char *> sorted;
	int totalsize = 0;

	for(int i = 0; i < sources->Size(); ++i) {
		char *newsource = sources->GetData(i);
		int newsize = sizes->GetData(i);
		totalsize += newsize;
		bool inserted = false;

		for(int j = 0; j < sorted.Size(); ++j) {
			char *existingsource = sorted.GetData(j);
			int existingsize = combined.GetData(existingsource);

			if(newsize <= existingsize) {
				sorted.insert_at(newsource, j);
				inserted = true;
				break;
			}
		}
	if(!inserted)			sorted.insert(newsource);
	}

	// Open the output file
	FILE *output = fopen(_outputFilename, "wt");

	// Print out our sorted list
	fprintf(output, "Total recognised memory leaks   : %d Kbytes\n", int(totalsize/1024));
	fprintf(output, "Total unrecognised memory leaks : %d Kbytes\n\n", int(unrecognised/1024));

	for(int k = sorted.Size() - 1; k >= 0; --k) {
		char *source = sorted.GetData(k);
		int size = combined.GetData(source);
		int freq = frequency.GetData(source);

		if(size > 2048)		fprintf(output, "%-95s(%d Kbytes in %d leaks)\n", source, int(size/1024), freq);
		else				fprintf(output, "%-95s(%d  bytes in %d leaks)\n", source, size, freq);
	}

	// Clear up
	fclose(output);

	delete sources;
	delete sizes;
}

void AppPrintMemoryLeaks(char *_filename)
{
	// Print all raw memory leak data to a temporary file
	char tmpFilename[512];
	sprintf(tmpFilename, "%s.tmp", _filename);

	OFSTRUCT ofstruct;

	HFILE file = OpenFile(tmpFilename, &ofstruct, OF_CREATE);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN,(void *) file);
	_CrtDumpMemoryLeaks();
	_lclose(file);

	// Parse the temp file into a sensible format
	ParseMemoryLeakFile(tmpFilename, _filename);

	// Delete the temporary file
	DeleteThisFile(tmpFilename);
}
#endif

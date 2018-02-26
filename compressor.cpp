/* Compressor LZW by Caroline GALLIEZ */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <sstream>

using namespace std;

class lzwStream : public ifstream
{
	private:
		map<string,unsigned int> dictionary;
		void writeCode(unsigned short code, ostream &output);
		int Position = 0;
		char OldCode = 0;
	public:
		// Constructor
		lzwStream():ifstream() { initializeDictionary(); };
		// Compressor function
		bool CompressToStream(ostream &output);
		// Initialize (or reinitialize) the dictionary
		void initializeDictionary();
};


void lzwStream::initializeDictionary()
{
	// Remove all the elements from the dictionary
	dictionary.clear(); 
	// Fill it with initial values
	for (unsigned int code=0; code<=255; code++) 
	{
		// we use a string because we may have to add to the dictionary a string (for instance "HE")
		string E;(char)code;
		E = (char)code;
		dictionary.insert(pair<string, unsigned int>(E,code));
	}
}

void lzwStream::writeCode(unsigned short code, ostream &output)
{
	// If we have an even number of code
	if (Position & 1)
	{
		OldCode = (OldCode << 4) | (code >> 8); 
		output.write(&OldCode, 1);
		OldCode = code & 0xff;
		output.write(&OldCode, 1);
	}
	else // we have an odd number of code
	{
		OldCode = code & 0xf;	
		code = code >> 4; 
		output.write((char*)&code, 1);
	}
	Position++;
}


// function Compression(text, dictionnary):
// w = ""
// 	WHILE (there is character to read in the text):
//		c = Read(text) // one char
// 		p = concatenate(w,c)
// 		IF (p is in the dictionary):
//			w=p
// 		ELSE:
//			Add p to the dictionnary
//			Write the code of w to the compressed file
//			w=c
// 	END;

bool lzwStream::CompressToStream(ostream &output)
{
	string w = ""; // Initialize an empty string
	string p;
	bool bEven = 0;
	while  (true)
	{
		// Read one code in the Lzw file
		char c;
		this->read((char *)&c, 1);
		if (this->eof()) // If we have reached the end of file we return false
		{
			map<string, unsigned int>::iterator itw;			
			itw = dictionary.find(w); 
			if (itw != dictionary.end())
			{
				// write 
				writeCode(itw->second, output);
			}
			return false; 
		}
		
		p = w + c; // Concatenate w and c
		
		// Find p in the dictionnary
		map<string, unsigned int>::iterator itp;
		itp = dictionary.find(p); 
		if (itp == dictionary.end()) // p is not in the dictionnary
		{
			// add p to the dictionnary
			if (dictionary.size() >= (size_t)(1 << 12)) // if dictionnary is full, we reinitialize it
				initializeDictionary(); 
			dictionary.insert(pair<string, unsigned int>(p, (unsigned)dictionary.size())); 
			// Write the w code to the compressed file
			map<string, unsigned int>::iterator itw;			
			itw = dictionary.find(w); 
			if (itw != dictionary.end()) // If we have found w
			{
			  	// write 
				writeCode(itw->second, output);
			
			}
			w = c;
		}
		else
			w = p;
			
	}
	return true;
}


int main (int argc, char *argv[])
{
	// Verify the command line arguments
	string fileName;
	if (argc!=2)
	{
		cout << "Syntax: compressor filename\n";
		return 0;
	}
	else
		fileName = argv[1];
	// Create the lzm File and the compressed file
	string fileNameCompressed;
	lzwStream fileToCompress;
	ofstream fileCompressed;
	// Open the file to compress
	fileToCompress.open(fileName.c_str(), ios::binary);
	// Open the compressed file
	fileNameCompressed=fileName.substr(0,fileName.find_last_of('.'))+".z";
	fileCompressed.open(fileNameCompressed.c_str(), ios::binary);

	if (!fileToCompress.is_open())
	{
		cout << "Cannot open file " << fileName << "\n";
	}

	// Compress to the file
	fileToCompress.CompressToStream(fileCompressed);	

	// Close the files
	fileToCompress.close();
	fileCompressed.close();
	return 1;
}

/* Decompressor LZW by Caroline GALLIEZ */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <vector>

using namespace std;

class lzwStream : public ifstream
{
	private:
		// The dictionary associates "codes" to "translation bytes"
		typedef vector<unsigned char> directoryEntry;
		map<unsigned int, directoryEntry> dictionary;
		unsigned char oldByte;
		int Position = 0;
	public:
		// Constructor
		lzwStream():ifstream() { initializeDictionary(); };
		// Reads 1 code in the input Lzw file
		bool readCode(unsigned int* code);
		// Decompressor function
		bool DecompressToStream(ostream &output);
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
		directoryEntry E = { (unsigned char)code };
		dictionary.insert(pair<unsigned int, directoryEntry>(code,E));
	}
}

bool lzwStream::DecompressToStream(ostream &output)
{
	directoryEntry lastEntry;
	unsigned int code;
	while  (true)
	{
		// Read one code in the Lzw file
		if (!readCode(&code)) break; 
		
		// Find the code in the dictionary, or use the last entry if not found
		map<unsigned int, directoryEntry>::iterator it;
		it = dictionary.find(code); 
		directoryEntry currentEntry = (it == dictionary.end()) ? lastEntry : it->second;
		// Output the directory entry
		for (unsigned char v:currentEntry)
			output << v;

		// Create the new directory entry
		if (lastEntry.size()>0)  // LastEntry must be initialized !
		{
			// The new directory entry is made of the concatenation of the previous code and the first byte of the current one
			directoryEntry newEntry = lastEntry;
			newEntry.push_back(currentEntry[0]); 
			// Dictionary is full ?
			if (dictionary.size() >= (size_t)(1 << 12)) 
				initializeDictionary(); 
			dictionary.insert(pair<unsigned int, directoryEntry>((unsigned)dictionary.size(),newEntry)); 
		}
			// Save the current directory entry
			lastEntry = currentEntry;
	}
	return true;
}

bool lzwStream::readCode(unsigned int* code)
{
	unsigned char c1;
	// Read 1 byte
	this->read((char *)&c1, 1);
	if (this->eof()) return false; // If we have reached the end of file we return false
	Position++;

	if (Position & 1)  // If the file results in an even number of code (we already added one before)
	{
		this->read((char *)&oldByte, 1);
		if (this->eof()) return false;
		*code = (c1 << 4) | ((oldByte >> 4) & 0xf);
	}

	else
		*code = ((oldByte & 0xf) << 8) | c1;	
	
	return true;
}


int main (int argc, char *argv[])
{
	// Verify the command line arguments
	string fileName;
	if (argc!=2)
	{
		cout << "Syntax: decompressor filename\n";
		return 0;
	}
	else
		fileName = argv[1];
	
	// Create the lzm File and the decompressed file
	string fileNameDecompressed;
	lzwStream fileToDecompress;
	ofstream fileDecompressed;
	// Open the file to decompress
	fileToDecompress.open(fileName.c_str(), ios::binary);
	fileNameDecompressed=fileName.substr(0,fileName.find_last_of('.'))+".txt";
	// Open the decompressed file
	fileDecompressed.open(fileNameDecompressed.c_str(), ios::binary);

	if (!fileToDecompress.is_open())
	{
		cout << "Cannot open file " << fileName << "\n";
	}
	   
	// Decompress to the decompressed file
	fileToDecompress.DecompressToStream(fileDecompressed);	

	// Close the files
	fileToDecompress.close();
	fileDecompressed.close();
	return 1;
}

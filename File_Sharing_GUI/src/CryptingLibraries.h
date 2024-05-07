class BobFileCrypt
{
public:
	int FileReceived;
	BobFileCrypt(char* FilePath, char* aliceIP, short port);
	~BobFileCrypt();


private:

};




class AliceFileCrypt
{
public:
	int FileSent;
	AliceFileCrypt(char* FilePath, char* bobIP, short port);
		~AliceFileCrypt();


private:

};



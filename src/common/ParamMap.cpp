
#include "ParamMap.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

bool ParamMap::loadBinary()
{
	bool res = true;
	
	_values.clear();

	FILE *f = fopen(fileName().c_str(), "rb" );
	if ( f && !feof( f ) ) {
		int n;
		fread(&n, sizeof( n ), 1, f);

		for (int i=0; i<n && !feof(f); i++) {
			string key = readChunk( f );
			string value = readChunk( f );
			setString( key, value );
		}

		//если feof раньше времени - то ошибка
		//TODO

		fclose( f );
	}
	else {
		//ошибка открытия файла
		res = false;
	}
	return res;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

bool ParamMap::saveBinary()
{
	bool res = true;

	FILE *f = fopen(fileName().c_str(), "wb" );
	if ( f ) {
		int n = _values.size();
		fwrite( &n, sizeof( n ), 1, f );

		for (StringStringMap::const_iterator iter = _values.begin();
			iter != _values.end(); iter++) {
			string key = iter->first;
			string value = iter->second;
			writeChunk( f, key );
			writeChunk( f, value );
		}

		fclose( f );
	}
	else {
		//ошибка открытия файла
		res = false;
	}
	return res;

}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

bool ParamMap::loadText()
{
	clear();
  /*	ifstream in;
	in.open(filename);
	string key, value;
	in.getline(key);
	in.getline(value);*/
	return true;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

bool ParamMap::saveText()
{
/*	ofstream out;
	out.open(filename);
	out<<myData;
*/
	return true;

	/*vector<string> keys = getKeys();
	for (StringStringMap::const_iterator iter = _values.begin();
		iter != _values.end(); iter++) {
		string key = iter->first;
		//string value = iter->second;
		keys.push_back( key );
	}
	//keys.sort();
	*/
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

vector<string> ParamMap::keys()
{
	vector<string> _keys;
	for (StringStringMap::const_iterator iter = _values.begin();
		iter != _values.end(); iter++) {
		string key = iter->first;
		_keys.push_back( key );
	}
	return _keys;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


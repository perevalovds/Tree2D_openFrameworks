//---------------------------------------------------------------------------

#ifndef ParamMapH
#define ParamMapH
//---------------------------------------------------------------------------

#include <string>
#include <map>
#include <vector>

#include "maps.h"

#include "converter.h"


using namespace std;

class ParamMap {
public:
	ParamMap() {
	}

	ParamMap( string fileName ) {
		_fileName = fileName;
	}
	~ParamMap() {
	}

	string fileName() { return _fileName; };
	void setFileName( string fileName ) { _fileName = fileName; }

	bool load( bool textMode = false) {
		bool res;
		if (textMode) res = loadText();
		else res = loadBinary();
		return res;
	}
	bool save( bool textMode = false) {
		bool res;
		if (textMode) res = saveText();
		else res = saveBinary();
		return res;
	}

    vector<string> keys(); //unsorted!

	void deleteKey( string key )	{
		_values.erase( key );
	}

	void clear() {
		_values.clear();
	}

	bool keyExists( const string &key ) {
		StringStringMap::iterator p = _values.find( key );
		return ( p != _values.end() );		
	}

	string getString( const string &key, string def = "" ) {
		StringStringMap::iterator p = _values.find( key );
		if (p != _values.end()) {
			return p->second;
		}
		else {
			return def;
		}
	}

	int getInt( const string &key, int def = 0 ) {
		string value = getString( key );
		if ( value != "" ) return Converter::strToInt( value);
					 else return def;
	}

	double getDouble( const string &key, double def = 0 ) {
		string value = getString( key );
		if ( value != "" ) return Converter::strToDouble( value );
					 else return def;
	}
	float getFloat( const string &key, float def = 0 ) {
		return getDouble( key, def );
	}

	bool getBool( const string &key, bool def = false ) {
		string value = getString( key );
		if ( value != "" ) {
			if ( value == "1" ) return true;
							else return false;
		}
		 else return def;
	}

	void setString( const string &key, const string &value ) {
		//_values.insert( StringStringMapValueType( key, value) );
		_values[key] = value;
	}

	void setDouble( const string &key, double dValue )
	{
		setString( key, Converter::doubleToStr( dValue ) );
	}

	void setFloat( const string &key, float value )
	{
		setDouble( key, value );
	}

	void setInt( const string &key, int iValue )
	{
		setString( key, Converter::intToStr( iValue ) );
	}
	void setBool( const string &key, bool bValue )
	{
		string value;
		if (bValue) value = "1";
		else value = "0";
		setString( key, value );
	}




protected:
	StringStringMap _values;
	string _fileName;


protected:
	string readChunk( FILE *f )
	//DUMMY неоптимально
	{
		int size;
		fread( &size, sizeof( size ), 1, f );
		char *data = new char [size];
		fread( data, sizeof(data[0]), size, f );
		string s = "";
		for (int j=0; j<size; j++) {
			s += data[j];
		}
		delete[] data;
		return s;
	}

	void writeChunk( FILE *f, const string &s )
	//DUMMY неоптимально
	{
		int size = s.length();
		fwrite( &size, sizeof( size ), 1, f);
		char *data = new char [size];
		for (int j=0; j<size; j++) {
			data[j] = s[j];
		}
		fwrite( data, sizeof(data[0]), size, f );
		delete[] data;
	}

private:
	bool loadBinary();
	bool saveBinary();

	bool loadText();
	bool saveText();


};






#endif

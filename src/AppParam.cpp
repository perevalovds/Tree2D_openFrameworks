#include "AppParam.h"
#include "ofxIniSettings.h"
#include "ofxXmlSettings.h"
#include "parsing.h"


AppParam shared_param;



//--------------------------------------------------------------------------------------
void AppParam::load()
{
    if (!_loaded) {
        _loaded = true;
        reload();
    }
 
}
//--------------------------------------------------------------------------------------
void AppParam::reload()
{
	xmlFileName = "params-dynamic.xml";

	_cache.clear();

	//ini
	_ini.clear();
	_ini.load( ofToDataPath( "params.ini" ) );

	//xml
	_xml.loadFile( xmlFileName );


    _loaded = true;

}

//--------------------------------------------------------------------------------------
void AppParam::save()
{
	_xml.saveFile( xmlFileName );	
}


//--------------------------------------------------------------------------------------
//берет значение с кэшированием
string AppParam::getString( const string &name, const string &defValue )
{
    load();
	if ( _cache.keyExists( name ) ) {
		return _cache.getString( name );
	}
	else {
		string value = _ini.get( name, defValue );	//сначала статичный параметр
		value = _xml.getValue( name, value );		//потом динамичный
		_cache.setString( name, value );
		return value;
	}
}

//--------------------------------------------------------------------------------------
void AppParam::saveString( const string &name, const string &value )
{
    load();

	_cache.setString( name, value );
	_xml.setValue( name, value );
	save();

}

//--------------------------------------------------------------------------------------
float AppParam::getFloat( const string &name, float defValue )
{
	return ofToFloat( getString( name, ofToString( defValue ) ) );
}

//--------------------------------------------------------------------------------------
void AppParam::saveFloat( const string &name, float value, bool saveDisk )
{
	//если есть связка - ставим ту переменую
	StringPointerMap::iterator p = _floatMap.find( name );
	if ( p != _floatMap.end() ) {
		float *pVal = (float*)_floatMap[ name ];		
		if ( pVal ) { 
			*pVal = value;
		}
	}
	else {
	}

	if ( saveDisk ) {
		saveString( name, ofToString( value ) );
	}
}

//--------------------------------------------------------------------------------------
void AppParam::linkedChanged( float *pVar )	//обновились линкованные значения, надо их записать
{
	if ( pVar ) {
		//если есть связка - запоминаем ту переменую
		PointerStringMap::iterator p = _float2Map.find( pVar );
		if ( p != _float2Map.end() ) {
			string name = _float2Map[ pVar ];		
			saveFloat( name, *pVar );
		}
		else {
			cout << "ERROR AppParam::linkedChanged" << endl;
		}
	}
}

//--------------------------------------------------------------------------------------
void AppParam::linkFloat( float *pVar, const string &name, float defValue )	//считать и связать значение
{
	if ( pVar ) {
		float value = getFloat( name, defValue );
		*pVar = value;
		_floatMap[ name ] = pVar;		//запоминаем связку
		_float2Map[ (void*)pVar ] = name;

	}
	else {
		cout << "ERROR AppParam::linkFloat : linking float variable " << name << endl;
	}
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void ParamSetup()
{
    shared_param.load();
}


//--------------------------------------------------------------------------------------
string	ParamString( const string &name, const string &defValue )
{
	return shared_param.getString( name, defValue );
}

//--------------------------------------------------------------------------------------
int		ParamInt( const string &name, int defValue )
{
	return ofToInt( shared_param.getString( name, ofToString( defValue ) ) );

}

//--------------------------------------------------------------------------------------
float	ParamFloat( const string &name, float defValue )
{
	return shared_param.getFloat( name, defValue );
}

//--------------------------------------------------------------------------------------
void	ParamFloatLink( float *pVar, const string &name, float defValue )	//считать и связать значение
{
	shared_param.linkFloat( pVar, name, defValue );
}

//--------------------------------------------------------------------------------------
void	ParamStringSave( const string &name, const string &value )			//запись на диск
{
	shared_param.saveString( name, value );
}

//--------------------------------------------------------------------------------------
void	ParamIntSave( const string &name, int value )				//запись на диск
{
	shared_param.saveString( name, ofToString( value ) );
}

//--------------------------------------------------------------------------------------
void	ParamFloatSave( const string &name, float value, bool saveDisk )			//запись на диск
{
	shared_param.saveFloat( name, value, saveDisk );
}

//--------------------------------------------------------------------------------------
void	ParamLinkedChanged( float *pVar  )		//переменная была изменена, записать на диск
{	
	shared_param.linkedChanged( pVar );
}

//--------------------------------------------------------------------------------------

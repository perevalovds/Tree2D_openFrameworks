#pragma once

//параметры приложения
#include "ofMain.h"
#include "ofxIniSettings.h"
#include "ofxXmlSettings.h"
#include "ParamMap.h"

//быстрая загрузка параметров. Если нужен часто - лучше все же его считать в начале работы программы
void ParamSetup();

string	ParamString( const string &name, const string &defValue = "" );
int		ParamInt( const string &name, int defValue = 0 );
float	ParamFloat( const string &name, float defValue = 0 );

void	ParamFloatLink( float *pVar, const string &name, float value );	//считать и связать значение

void	ParamStringSave( const string &name, const string &value );		//запись на диск
void	ParamIntSave( const string &name, int value );		//запись на диск
void	ParamFloatSave( const string &name, float value, bool saveDisk = true );		//запись на диск
void	ParamLinkedChanged( float *pVar  );		//переменная была изменена, записать на диск

class AppParam
{
public:
    AppParam() { _loaded = false; }
	void save();
    void load();        //грузит только если раньше не загружали
	void reload();
	
	//берет значение с кэшированием
	string getString( const string &name, const string &defValue );
	void saveString( const string &name, const string &value );		//запись на диск

	float getFloat( const string &name, float defValue );
	void saveFloat( const string &name, float value, bool saveDisk = true );
	void linkFloat( float *pVar, const string &name, float defValue );	//считать и связать значение

	void linkedChanged( float *pVar );	//обновились линкованные значения, надо их записать

private:
    bool _loaded;
	ofxIniSettings _ini;
	ofxXmlSettings _xml;
	ParamMap _cache;	//кэш значений

	string xmlFileName;


	typedef map<string, void *, less<string> > StringPointerMap;
	typedef map<void *, string, less<void *> > PointerStringMap;


	StringPointerMap _floatMap;
	PointerStringMap _float2Map;
	
};

extern AppParam shared_param;

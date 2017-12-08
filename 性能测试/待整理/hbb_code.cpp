// hbb_code.cpp : 定义控制台应用程序的入口点。
//

#include "../hbb_code/stdafx.h"

//常用头文件
#if 1

#define WIN32_LEAN_AND_MEAN

#include <algorithm>
#include <array>
#include <cstdlib>
#include <deque>
#include <fstream>

#include "BaseHeader.hpp"
#include "DumpFile.h"

#include <iostream>
#include <list>
#include <map>
#include <memory>

#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#if defined(WIN32) || defined(WIN64)
#include <time.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif
#include <tuple>
#include <set>
#include <sstream>


#include <vector>
#include <utility>

#include <windows.h>
#include "noncopy.h"
#include "linux_and_win.h"

#endif

#if 0

#pragma region //排序

struct IntAndInt
{
	IntAndInt()
	{
		key = 0;
		value =0;
	}
	int key;
	int value;
};

static inline bool _IntAndInt_DownByValue(const IntAndInt & oLeft, const IntAndInt & oRight) 
{
	return oLeft.value > oRight.value;
}

std::vector< IntAndInt  > g_vec_id_and_ability;


int FindPosFromRankList(int value_  )
{
	IntAndInt obj_;
	obj_.value = value_;

	std::vector<IntAndInt>::iterator find_it = std::lower_bound(g_vec_id_and_ability.begin() , g_vec_id_and_ability.end() , obj_ ,_IntAndInt_DownByValue  );
	return find_it - g_vec_id_and_ability.begin();
}

int FindPosFromRankList(int value_ , const int key_)
{
	IntAndInt obj_;
	obj_.value = value_;

	std::vector<IntAndInt>::iterator find_it = std::lower_bound(g_vec_id_and_ability.begin() , g_vec_id_and_ability.end() , obj_ ,_IntAndInt_DownByValue  );
	std::vector<IntAndInt>::iterator temp_it = find_it;
	std::vector<IntAndInt>::iterator end_it = g_vec_id_and_ability.end();
	for(; temp_it != end_it; ++temp_it ){
		if(temp_it->key == key_){
			return temp_it - g_vec_id_and_ability.begin();
		}
	}

	end_it = find_it;
	for(temp_it = g_vec_id_and_ability.begin() ; temp_it != end_it ; ++temp_it){
		if(temp_it->key == key_){
			return temp_it - g_vec_id_and_ability.begin();
		}
	}
	return -1;
}

void UpdateVecByEraseAndInsert(int old_value_ ,int new_value_, const int key_)
{
	int pos_ =  FindPosFromRankList(old_value_ , key_);
	if( (size_t)pos_ < g_vec_id_and_ability.size() ){
		g_vec_id_and_ability.erase(g_vec_id_and_ability.begin() + pos_);
	}

	IntAndInt obj_;
	obj_.key = key_;
	obj_.value = new_value_;
	g_vec_id_and_ability.insert(std::upper_bound(g_vec_id_and_ability.begin(),g_vec_id_and_ability.end(),obj_ ,_IntAndInt_DownByValue ), obj_ );
}

void UpdateVecBySort(int old_value_ ,int new_value_, const int key_)
{
	int pos_ =  FindPosFromRankList(old_value_ , key_);
	bool is_find = false;
	if( (size_t)pos_ < g_vec_id_and_ability.size() ){
		g_vec_id_and_ability[pos_].value = new_value_;
		is_find = true;
	}

	if(is_find == false){
		IntAndInt obj_;
		obj_.key = key_;
		obj_.value = new_value_;
		g_vec_id_and_ability.push_back(obj_);
	}
	std::sort(g_vec_id_and_ability.begin(),g_vec_id_and_ability.end(), _IntAndInt_DownByValue);
}

void UpdateVecByMove(int old_value_ ,int new_value_, const int key_)
{
	int pos_ = FindPosFromRankList(old_value_ , key_);
	if( (size_t)pos_ >= g_vec_id_and_ability.size() ){
		//不存在//即新增
		IntAndInt obj_;
		obj_.value = new_value_;
		obj_.key = key_;
		std::vector<IntAndInt>::iterator it = std::upper_bound(g_vec_id_and_ability.begin() , g_vec_id_and_ability.end() , obj_ ,_IntAndInt_DownByValue  );
		g_vec_id_and_ability.insert(it , obj_);
	}
	else{
		int now_pos_ = pos_;
		int new_pos_ = FindPosFromRankList(new_value_ );
		//确保索引有效
		if((size_t)new_pos_ >= g_vec_id_and_ability.size()){
			return;
		}

		if(now_pos_ == new_pos_){
			g_vec_id_and_ability[now_pos_].value = new_value_;
			return;
		}
		else if(now_pos_ < new_pos_){
			--new_pos_;
			if(now_pos_ == new_pos_){
				g_vec_id_and_ability[now_pos_].value = new_value_;
				return;
			}

			int must_move_buff_len = new_pos_ - now_pos_;
			IntAndInt obj_ = g_vec_id_and_ability[now_pos_];
			obj_.value = new_value_;

			IntAndInt * begin_ptr = &(g_vec_id_and_ability[now_pos_]);
			memmove((char*)begin_ptr  ,(char*)(begin_ptr +1) , sizeof(IntAndInt)*must_move_buff_len  );
			g_vec_id_and_ability[new_pos_] = obj_;
		}
		else{
			int must_move_buff_len =  now_pos_ - new_pos_;
			IntAndInt obj_ = g_vec_id_and_ability[now_pos_];
			obj_.value = new_value_;

			IntAndInt * begin_ptr = &(g_vec_id_and_ability[new_pos_]);
			memmove( (char*)(begin_ptr +1) ,(char*)begin_ptr , sizeof(IntAndInt)*must_move_buff_len  );
			g_vec_id_and_ability[new_pos_] = obj_;
		}
	}
}

void test_fun1()
{
	const int test_times = 1000;
	const int test_vector_size = 100000;
	const int dest_key = 9000;
	const int rand_range = 100000;
	g_vec_id_and_ability.reserve(test_vector_size + 1);

	std::map<int , int> map_;
	for ( int i = 0; i < test_vector_size ; ++i ){

		// hero battle ability
		int nBattleAbility = rand() %rand_range;
		IntAndInt t_pair;
		t_pair.key = i;
		t_pair.value = nBattleAbility;
		g_vec_id_and_ability.push_back(t_pair);
		map_[t_pair.key] = t_pair.value;
	}

	std::sort(g_vec_id_and_ability.begin() , g_vec_id_and_ability.end() , _IntAndInt_DownByValue );

	std::cout<<"容器大小:"<<test_vector_size<<std::endl;
	std::cout<<"运行次数:"<<test_times<<std::endl;

	{
		unsigned long begin_t = GetTickCount();
		for(int i = 0 ; i < test_times ; ++i){
			int nBattleAbility = rand() %rand_range;
			int & o_value = map_[dest_key];
			UpdateVecBySort(o_value ,nBattleAbility, dest_key);
			o_value = nBattleAbility;
		}
		int n1 = (int)(GetTickCount() - begin_t);
		std::cout<<"UpdateVecBySort diff:"<<n1<<"ms"<<std::endl;
	}

	{
		unsigned long begin_t = GetTickCount();
		for(int i = 0 ; i < test_times ; ++i){
			int nBattleAbility = rand() %rand_range;
			int & o_value = map_[dest_key];
			UpdateVecByMove(o_value ,nBattleAbility, dest_key );
			o_value = nBattleAbility;
		}
		int n1 = (int)(GetTickCount() - begin_t);
		std::cout<<"UpdateVecByMove diff:"<<n1<<"ms"<<std::endl;
	}

	{
		unsigned long begin_t = GetTickCount();
		for(int i = 0 ; i < test_times ; ++i){
			int nBattleAbility = rand() %rand_range;
			int & o_value = map_[dest_key];
			UpdateVecByEraseAndInsert(o_value ,nBattleAbility, dest_key);
			o_value = nBattleAbility;
		}
		int n1 = (int)(GetTickCount() - begin_t);
		std::cout<<"UpdateVecByEraseAndInsert diff:"<<n1<<"ms"<<std::endl;
	}
}

#pragma endregion


#pragma region //跳表


#include "../src/skiplist.h"  
#include <iostream>  
  
using namespace std;
int skip_test()
{  
    cout << "test is starting ....." << endl;  
  
    SkipList list;  
  
    //测试插入  
    for(int i = 0; i < 100; ++i){  
        list.Insert(i, i+10);  
        //cout << list.GetCurrentLevel() << endl;  
    }  
    cout << "The number of elements in SkipList is :"   
         << list.size()   
         << endl;  
    if(list.size() != 100){  
        cout << "Insert failure." << endl;  
    }else{  
        cout << "Insert success." << endl;  
    }  
      
    //测试查找  
    bool is_search_success = true;  
    for(int i = 0; i < 100; ++i){  
        int value;  
        if(!(list.Search(i,value) && (value == i+10))){  
            is_search_success = false;  
            break;  
        }  
    }  
    if(is_search_success){  
        cout << "Search success." << endl;  
    }else{  
        cout << "Search failure." << endl;  
    }  
      
    //测试删除  
    bool is_delete_success = true;  
    for(int i = 0; i < 100; ++i){  
        int value;  
        if(!(list.Delete(i,value) && (value == i+10))){  
            is_delete_success = false;  
            break;  
        }  
    }  
    if(is_delete_success){  
        cout << "Delete success." << endl;  
    }else{  
        cout << "Delete failure." << endl;  
    }  
      
    cout << "test is finished ...." << endl;  
    return 0;  
}  
#pragma endregion

#endif

//测试常用函数
#if 1

void waitTo(time_t end_time) {
	while (time(NULL) < end_time) {
		Sleep(1);
	}
}

void write2File(int nAppID , std::string str_) {
	char name_[128];
	sprintf(name_ , "report_dir/%d.txt" , nAppID);
	FILE * f_ = fopen(name_, "w");
	if (f_ == NULL) {
		return;
	}
	fputs(str_.c_str(), f_ );
	fclose(f_); f_ = NULL;
}

#endif

//大小端
#if 0
#include <vector>
#include "base64.h"
#include <sstream> 
#include <iostream>

struct ShortAndShort
{
	ShortAndShort(short n1, short n2) {
		key = n1;
		value = n2;
	}
	short key;
	short value;
};


#include <winsock2.h>
#pragma  comment(lib,"ws2_32.lib")

void test_main(int argc, char* argv[])
{
	ShortAndShort temp_(htons(100), htons(256));
	std::cout << sizeof(ShortAndShort) << std::endl;
	std::string encoded_str = base64_encode((unsigned char const*)(&temp_), sizeof(ShortAndShort));
	std::cout << encoded_str << std::endl;
}

#endif

//base64_test
#if 0
#include <vector>
#include "base64.h"
#include <sstream> 
#include <iostream>


struct IntAndInt2
{
	IntAndInt2()	{
		key = 0;
		value =0;
	}
	int key;
	int value;
};

void base64_test()
{
	const int len_ = 10;
	std::vector< IntAndInt2  > vec_0;
	for ( int i = 0; i < len_ ; ++i ){

		int nBattleAbility = rand() %1000;
		IntAndInt2 t_pair;
		t_pair.key = i;
		t_pair.value = nBattleAbility;
		vec_0.push_back(t_pair);
	}

	std::string encoded_str = base64_encode( (unsigned char const*)(&(vec_0[0])) , sizeof(IntAndInt2) * len_);
	std::string decoded = base64_decode(encoded_str);
	if(decoded.empty() ){
		return ;
	}

	const IntAndInt2 * p_obj = (const IntAndInt2 *)decoded.data();
	const int len_1 = decoded.size() / sizeof(IntAndInt2);
	std::vector< IntAndInt2  > vec_1;
	for(int i = 0 ; i < len_1 ; ++i){
		vec_1.push_back( *p_obj );
		++p_obj;
	}
}

void test_main(int argc, char* argv[])
{
	std::string str_("AAQ=");
	std::string decoded = base64_decode(str_);
	short * p_num = (short*)decoded.c_str();
	int i = 0;
}

#endif

//rapidjson_test_write
#if 0

#include "../rapidjson/include/rapidjson/writer.h"
#include "../rapidjson/include/rapidjson/stringbuffer.h"
#include <iostream>

using namespace rapidjson;
using namespace std;

int rapidjson_main()
{
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    
    writer.StartObject();               // Between StartObject()/EndObject(), 
    writer.Key("hello");                // output a key,
    writer.String("world");             // follow by a value.
    writer.Key("t");
    writer.Bool(true);
    writer.Key("f");
    writer.Bool(false);
    writer.Key("n");
    writer.Null();
    writer.Key("i");
    writer.Uint(123);
    writer.Key("pi");
    writer.Double(3.1416);
    writer.Key("a");
    writer.StartArray();                // Between StartArray()/EndArray(),
    for (unsigned i = 0; i < 4; i++)
        writer.Uint(i);                 // all values are elements of the array.
    writer.EndArray();
    writer.EndObject();

    // {"hello":"world","t":true,"f":false,"n":null,"i":123,"pi":3.1416,"a":[0,1,2,3]}
    cout << s.GetString() << endl;

    return 0;
}
#endif

//rapidjson_test_reader
#if 0 

#include "../rapidjson/include/rapidjson/reader.h"
#include <iostream>

using namespace rapidjson;
using namespace std;

struct MyHandler {
    bool Null() { cout << "Null()" << endl; return true; }
    bool Bool(bool b) { cout << "Bool(" << boolalpha << b << ")" << endl; return true; }
    bool Int(int i) { cout << "Int(" << i << ")" << endl; return true; }
    bool Uint(unsigned u) { cout << "Uint(" << u << ")" << endl; return true; }
    bool Int64(int64_t i) { cout << "Int64(" << i << ")" << endl; return true; }
    bool Uint64(uint64_t u) { cout << "Uint64(" << u << ")" << endl; return true; }
    bool Double(double d) { cout << "Double(" << d << ")" << endl; return true; }
    bool RawNumber(const char* str, SizeType length, bool copy) { 
        cout << "Number(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool String(const char* str, SizeType length, bool copy) { 
        cout << "String(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool StartObject() { cout << "StartObject()" << endl; return true; }
    bool Key(const char* str, SizeType length, bool copy) {
        cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool EndObject(SizeType memberCount) { cout << "EndObject(" << memberCount << ")" << endl; return true; }
    bool StartArray() { cout << "StartArray()" << endl; return true; }
    bool EndArray(SizeType elementCount) { cout << "EndArray(" << elementCount << ")" << endl; return true; }
};

int reader_main() {
    const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";

    MyHandler handler;
    Reader reader;
    StringStream ss(json);

	Json::Value v_;
    reader.Parse(ss, v_);
    //reader.Parse(ss, handler);

    return 0;
}

#endif

//repidjson_test_dom
#if 0

#include "../rapidjson/include/rapidjson/document.h"
#include "../rapidjson/include/rapidjson/writer.h"
#include "../rapidjson/include/rapidjson/stringbuffer.h"
#include <iostream>

using namespace rapidjson;

int rapidjson_dom_main() {
    // 1. Parse a JSON string into DOM.
    const char* json = "{\"project\":\"rapidjson\",\"stars\":10 ,\"array_int\":[1,2,3,4] }";
    Document d;
    d.Parse(json);

    // 2. Modify it by DOM.
    Value& s = d["stars"];
    s.SetInt(s.GetInt() + 1);

	Value& arr_int = d["array_int"];
	for(size_t i = 0 ; i < arr_int.Size() ; ++i){
		std::cout<<arr_int[i].GetInt()<<std::endl;
	}

    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;
    return 0;
}


#endif

//compare json repidjson
#if 0

#include <windows.h>
#include "base64.h"
#include "../rapidjson/include/rapidjson/document.h"
#include "../rapidjson/include/rapidjson/writer.h"
#include "../rapidjson/include/rapidjson/stringbuffer.h"
#include "../protocol/gs_global.pb.h"


#pragma comment(lib,"libprotobufD.lib")

#define INVALID_32BIT_ID  uint32(~0)
#define ARENA_ACT_HERO_SKILL_SIZE			8


#ifndef mymax
#define mymax(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef mymin
#define mymin(a,b)            (((a) < (b)) ? (a) : (b))
#endif

typedef unsigned int uint32;
typedef int                int32;
const int max_team_heros_size = 11;

enum eAttributeIndex
{
	ATTR_HP = 0,         // 生命
	ATTR_ATTACK,         // 攻击
	ATTR_DEFENSE,        // 防御
	ATTR_CRIT,           // 暴击百分比
	ATTR_CRIT_DEFENSE,   // 抗暴百分比
	ATTR_HIT,		     // 命中百分比
	ATTR_DODGE,			 // 闪避百分比
	ATTR_PENETRATION,    // 穿透伤害
	ATTR_PENETRATION_DEF,// 穿透抵抗
	ATTR_REVOVER,        // 生命恢复
	ATTR_TREAT,          // 治疗效果
	ATTR_VAMPIRE,        // 吸血效果
	ATTR_CRITICAL,       // 暴击伤害
	ATTR_ATTACK_SPEED,   // 攻击加速
	ATTR_MOVE_SPEED,     // 移动加速
	ATTR_SKILL_CD,       // 技能冷却
	ATTR_DAMAGE_ADD,     // 伤害百分比
	ATTR_DAMAGE_REDUCE,  // 伤害减免百分比
	ATTR_MAX,
};

struct SHeroSkillInfo
{
	uint32 dwId;     // id
	int    nLevel;   // level
	bool   bActivate;

	SHeroSkillInfo() : 
		dwId(INVALID_32BIT_ID), nLevel(0), bActivate(false)
	{
	}
};

// hero base info
struct SPlayerHeroBaseInfo
{
	uint32 dwId;      // template id
	int    nLevel;    // level
	int    nStarNum;  // star
	int    nQuality;  // quality
	int    nDamaged;  // get damage
	int    nHPPercent;// hp percent

	SPlayerHeroBaseInfo() : dwId(INVALID_32BIT_ID),
		nLevel(0), nStarNum(0), nDamaged(0), nHPPercent(100)
	{
	}
};

struct SPlayerHeroInfo : SPlayerHeroBaseInfo
{
	// hero skills
	int nSkillSize;
	SHeroSkillInfo arSkill[ARENA_ACT_HERO_SKILL_SIZE];


	int32 m_HeroAttr[ATTR_MAX];
	int32 m_HeroPartialAttr[ATTR_MAX];//在pve或pvp战斗中,会改变数值
	int   m_QualityS[max_team_heros_size];
	int   m_Reinforcements;

	void clear_array()
	{
		m_Reinforcements = max_team_heros_size;
		memset(m_HeroAttr , 0 , sizeof(m_HeroAttr ));
		memset(m_HeroPartialAttr , 0 , sizeof(m_HeroPartialAttr ));
		memset(m_QualityS , -1 , sizeof(m_QualityS ));
	}

	SPlayerHeroInfo() : nSkillSize(0)
	{
		clear_array();
	}
};

static inline int DecodeHeros_2(const Json::Value &jsHeros)
{
	enum TempEnum{
		JSON_BASE_DATA_INDEX = 0,
		JSON_SKILL_S_INDEX,
		JSON_HERO_ATTR_S_INDEX,
		JSON_HERO_PARTIAL_ATTR_S_INDEX,
		JSON_QUALITY_S_INDEX,
	};

	int nHeroSize = 3;
	SPlayerHeroInfo arHero[3];

	if (NULL == arHero) {
		return -1;
	}

	if (jsHeros.isObject() && 
		jsHeros.isMember("data") && 
		jsHeros.isMember("num"))
	{
		int ver_num = 0;
		if(jsHeros.isMember("ver_num")){
			ver_num = jsHeros["ver_num"].asInt();
		}


		const Json::Value &jsHerDatas = jsHeros["data"];
		int nRealHeroSize = mymin(jsHeros["num"].asInt(), nHeroSize);

		for (int i = 0; i < nRealHeroSize; i++){
			const Json::Value &jsHero = jsHerDatas[i];
			SPlayerHeroInfo &stHero = arHero[i];

			int hero_size = (int)jsHero.size();
			if(hero_size> JSON_BASE_DATA_INDEX ){
				const Json::Value &json_base_data = jsHero[JSON_BASE_DATA_INDEX];
				if(json_base_data.size() == 7 ){
					int json_i = 0;
					stHero.dwId = json_base_data[json_i++].asUInt();
					stHero.nLevel = json_base_data[json_i++].asInt();
					stHero.nStarNum = json_base_data[json_i++].asInt();
					stHero.nDamaged = json_base_data[json_i++].asInt();
					stHero.nQuality = json_base_data[json_i++].asInt();
					stHero.nHPPercent = json_base_data[json_i++].asInt();
					stHero.m_Reinforcements = json_base_data[json_i++].asInt();
				}
			}

			if(hero_size > JSON_SKILL_S_INDEX ){
				const Json::Value &json_skill_s = jsHero[JSON_SKILL_S_INDEX];
				stHero.nSkillSize = mymin(ARENA_ACT_HERO_SKILL_SIZE, json_skill_s.size());
				for (int j = 0; j < stHero.nSkillSize; j++){
					const Json::Value &json_skill = json_skill_s[j];
					if(json_skill.size() == 2){
						int json_i = 0;
						stHero.arSkill[j].dwId = json_skill[json_i++].asUInt();
						stHero.arSkill[j].nLevel = json_skill[json_i++].asInt();
					}
				}
			}

			if(hero_size > JSON_HERO_ATTR_S_INDEX ){
				const Json::Value &json_hero_attr_s = jsHero[JSON_HERO_ATTR_S_INDEX];
				int nHeroAttrSize = mymin(ATTR_MAX, json_hero_attr_s.size());
				for (int j = 0; j < nHeroAttrSize; j++){
					stHero.m_HeroAttr[j] = json_hero_attr_s[j].asInt();
				}
			}

			if(hero_size > JSON_HERO_PARTIAL_ATTR_S_INDEX ){
				const Json::Value &json_hero_partial_attr_s = jsHero[JSON_HERO_PARTIAL_ATTR_S_INDEX];
				int nHeroPartialAttrSize = mymin(ATTR_MAX, json_hero_partial_attr_s.size());
				for (int j = 0; j < nHeroPartialAttrSize; j++){
					stHero.m_HeroPartialAttr[j] = json_hero_partial_attr_s[j].asInt();
				}
			}

			if(hero_size > JSON_QUALITY_S_INDEX ){
				const Json::Value &json_quality_s = jsHero[JSON_QUALITY_S_INDEX];
				int nQualitySize = mymin(max_team_heros_size, json_quality_s.size());
				for (int j = 0 ; j < nQualitySize; j++){
					stHero.m_QualityS[j] = json_quality_s[j].asInt();
				}
			}
		}
		return nRealHeroSize;
	}

	return 0;
}

static inline int DecodeHeros_3(const rapidjson::Document & jsHeros)
{
	using namespace rapidjson;
	 
	int nHeroSize = 3;
	SPlayerHeroInfo arHero[3];

	if (jsHeros.IsObject() && 
		jsHeros.HasMember("data") )
	{
		const Value &jsHerDatas = jsHeros["data"];
		int nRealHeroSize = mymin(jsHeros["num"].GetInt(), nHeroSize);

		for (int i = 0; i < nRealHeroSize; i++){

			SPlayerHeroInfo &stHero = arHero[i];
			std::string encoded64_str = jsHerDatas[i].GetString();
			std::string proto_buff = base64_decode( encoded64_str );
			SProtoSpace::arena_data_ver_3 arena_data_;
			if( arena_data_.ParseFromString(proto_buff) ){
				if(arena_data_.base_data_size() == 7){
					int proto_i = 0;
					stHero.dwId = arena_data_.base_data(proto_i++);
					stHero.nLevel = arena_data_.base_data(proto_i++);
					stHero.nStarNum = arena_data_.base_data(proto_i++);
					stHero.nDamaged = arena_data_.base_data(proto_i++);
					stHero.nQuality = arena_data_.base_data(proto_i++);
					stHero.nHPPercent = arena_data_.base_data(proto_i++);
					stHero.m_Reinforcements = arena_data_.base_data(proto_i++);
				}

				if(arena_data_.skill_id_s_size() == arena_data_.skill_lv_s_size() ){
					stHero.nSkillSize = mymin(ARENA_ACT_HERO_SKILL_SIZE, arena_data_.skill_lv_s_size());
					for (int j = 0; j < stHero.nSkillSize; j++){
						stHero.arSkill[j].dwId = (uint32)arena_data_.skill_id_s(j);
						stHero.arSkill[j].nLevel = arena_data_.skill_lv_s(j);
					}
				}

				int nHeroAttrSize = mymin(ATTR_MAX, arena_data_.attr_s_size() );
				for (int j = 0; j < nHeroAttrSize; j++){
					stHero.m_HeroAttr[j] = arena_data_.attr_s(j);
				}

				int nHeroPartialAttrSize = mymin(ATTR_MAX, arena_data_.partial_attr_s_size() );
				for (int j = 0; j < nHeroPartialAttrSize; j++){
					stHero.m_HeroPartialAttr[j] = arena_data_.partial_attr_s(j);
				}

				int nQualitySize = mymin(max_team_heros_size, arena_data_.quality_s_size() );
				for (int j = 0 ; j < nQualitySize; j++){
					stHero.m_QualityS[j] = arena_data_.quality_s(j);
				}
			}
		}
		return nRealHeroSize;
	}

	return 0;
}

static inline int DecodeHeros_repid(const rapidjson::Document &d_)
{
	enum TempEnum{
		JSON_BASE_DATA_INDEX = 0,
		JSON_SKILL_S_INDEX,
		JSON_HERO_ATTR_S_INDEX,
		JSON_HERO_PARTIAL_ATTR_S_INDEX,
		JSON_QUALITY_S_INDEX,
	};

	using namespace rapidjson;
	int nHeroSize = 3;
	SPlayerHeroInfo arHero[3];

	if (NULL == arHero) {
		return -1;
	}

	if (d_.IsObject() && d_.HasMember("data")&& d_.HasMember("num"))
	{
		int ver_num = 0;
		if(d_.HasMember("ver_num")){
			ver_num = d_["ver_num"].GetInt();
		}


		const Value &jsHerDatas = d_["data"];
		int nRealHeroSize = mymin(d_["num"].GetInt(), nHeroSize);

		for (int i = 0; i < nRealHeroSize; i++){
			const Value &jsHero = jsHerDatas[i];
			SPlayerHeroInfo &stHero = arHero[i];

			int hero_size = (int)jsHero.Size();
			if(hero_size> JSON_BASE_DATA_INDEX ){
				const Value &json_base_data = jsHero[JSON_BASE_DATA_INDEX];
				if(json_base_data.Size() == 7 ){
					int json_i = 0;
					stHero.dwId = json_base_data[json_i++].GetInt();
					stHero.nLevel = json_base_data[json_i++].GetInt();
					stHero.nStarNum = json_base_data[json_i++].GetInt();
					stHero.nDamaged = json_base_data[json_i++].GetInt();
					stHero.nQuality = json_base_data[json_i++].GetInt();
					stHero.nHPPercent = json_base_data[json_i++].GetInt();
					stHero.m_Reinforcements = json_base_data[json_i++].GetInt();
				}
			}

			if(hero_size > JSON_SKILL_S_INDEX ){
				const Value &json_skill_s = jsHero[JSON_SKILL_S_INDEX];
				stHero.nSkillSize = mymin(ARENA_ACT_HERO_SKILL_SIZE, json_skill_s.Size());
				for (int j = 0; j < stHero.nSkillSize; j++){
					const Value &json_skill = json_skill_s[j];
					if(json_skill.Size() == 2){
						int json_i = 0;
						stHero.arSkill[j].dwId = json_skill[json_i++].GetInt();
						stHero.arSkill[j].nLevel = json_skill[json_i++].GetInt();
					}
				}
			}

			if(hero_size > JSON_HERO_ATTR_S_INDEX ){
				const Value &json_hero_attr_s = jsHero[JSON_HERO_ATTR_S_INDEX];
				int nHeroAttrSize = mymin(ATTR_MAX, json_hero_attr_s.Size());
				for (int j = 0; j < nHeroAttrSize; j++){
					stHero.m_HeroAttr[j] = json_hero_attr_s[j].GetInt();
				}
			}

			if(hero_size > JSON_HERO_PARTIAL_ATTR_S_INDEX ){
				const Value &json_hero_partial_attr_s = jsHero[JSON_HERO_PARTIAL_ATTR_S_INDEX];
				int nHeroPartialAttrSize = mymin(ATTR_MAX, json_hero_partial_attr_s.Size());
				for (int j = 0; j < nHeroPartialAttrSize; j++){
					stHero.m_HeroPartialAttr[j] = json_hero_partial_attr_s[j].GetInt();
				}
			}

			if(hero_size > JSON_QUALITY_S_INDEX ){
				const Value &json_quality_s = jsHero[JSON_QUALITY_S_INDEX];
				int nQualitySize = mymin(max_team_heros_size, json_quality_s.Size() );
				for (int j = 0 ; j < nQualitySize; j++){
					stHero.m_QualityS[j] = json_quality_s[j].GetInt();
				}
			}
		}
		return nRealHeroSize;
	}

	return 0;
}


void repidjson_test(const std::string &str_ )
{
	using namespace rapidjson;
    Document d;
    d.Parse(str_.c_str() );
	DecodeHeros_repid(d);
}

bool ReadFile(std::string path_ , std::string & out_)
{
	//system("start %cd%");
	FILE * hanlde = fopen(path_.c_str() , "rb");
	if(hanlde == NULL){
		return false;
	}
	const int once_read_size = 512;
	int cur_buff_size = 0;
	int once_real_size = 0;
	do{
		out_.resize(cur_buff_size + once_read_size);
		once_real_size = fread( (void *)(out_.data() + cur_buff_size) , 1 , once_read_size , hanlde);
		cur_buff_size +=once_real_size;
	}while (once_real_size == once_read_size);
	out_.resize(cur_buff_size);
	return true;
}

void json_cpp_test(const std::string &str_ )
{
	Json::Value json_;
	if(Json::Reader().parse(str_ , json_) ){
		DecodeHeros_2(json_);
	}
	else{
		std::cout<<"json cpp error"<<std::endl;
	}
}

void base64_test_fun(const std::string &str_)
{
	using namespace rapidjson;
    Document d;
    d.Parse(str_.c_str() );
	DecodeHeros_3(d);
}

void compare_json()
{
	std::string buff_ , buff_base64;
	ReadFile("json_test.txt" , buff_);
	ReadFile("base64_test.txt" , buff_base64);

	const int test_times = 1800;
	int json_test_result = 0 , repid_test_result = 0 , base64_test_result=0;
	std::cout<<"测试次数 "<<test_times<<std::endl;

	DWORD begin_t = 0;

	begin_t = GetTickCount();
	for(int i = 0 ; i < test_times ; ++i){
		json_cpp_test(buff_);
	}
	json_test_result = GetTickCount() - begin_t; 

	begin_t = GetTickCount();
	for(int i = 0 ; i < test_times ; ++i){
		repidjson_test(buff_);
	}
	repid_test_result = GetTickCount() - begin_t; 

	begin_t = GetTickCount();
	for(int i = 0 ; i < test_times ; ++i){
		base64_test_fun(buff_base64);
	}
	base64_test_result = GetTickCount() - begin_t; 

	std::cout<<"json_test_result = "<<json_test_result	<<"ms"<<std::endl;
	std::cout<<"repid_test_result = "<<repid_test_result<<"ms"<<std::endl;
	std::cout<<"base64_test_result = "<<base64_test_result<<"ms"<<std::endl;
}

#endif

//repidjson write
#if 0
#include <windows.h>
#include "../rapidjson/include/rapidjson/writer.h"
#include "../rapidjson/include/rapidjson/stringbuffer.h"
#include "../rapidjson/include/rapidjson/document.h"
#include <iostream>

using namespace rapidjson;



std::string GetJsonStr_jsoncpp()
{
	Json::Value json_;
	json_["hello"] = "world";
	json_["t"] = true;
	{
		Json::Value & json_1 = json_["a"];
		for (unsigned i = 0; i < 4; i++){
			json_1.append(i);
		}
	}

	{
		Json::Value & json_1 = json_["player_s"];
		for (unsigned i = 0; i < 2; i++){
			Json::Value j_2;
			j_2["name"] = "hbb";
			j_2["lv"] = i;
			json_1.append(j_2);
		}
	}

	return Json::FastWriter().write(json_);
}

std::string GetJsonStr_ss()
{

    StringBuffer s;
    Writer<StringBuffer> writer(s);
    
    writer.StartObject();
    writer.Key("hello");writer.String("world");
    writer.Key("t"); writer.Bool(true);
    writer.Key("a");
	{
		Writer<StringBuffer> & arr_js = writer;
		arr_js.StartArray();

		for (unsigned i = 0; i < 4; i++)
			arr_js.Uint(i);

		arr_js.EndArray();
	}

	{
		writer.Key("player_s");writer.StartArray();
		for(int i = 0 ; i < 2 ; ++i){
			writer.StartObject();
			writer.Key("name") ;writer.String("hbb");
			writer.Key("lv") ;writer.Int(i);
			writer.EndObject();
		}
		writer.EndArray();
	}

    writer.EndObject();
	return s.GetString();
}

std::string GetJsonStr_doc()
{
	Document d_(kObjectType);
	Value & json_ = d_;
	json_.AddMember("hello" , "world" , d_.GetAllocator() );
	json_.AddMember("t" , true , d_.GetAllocator() );

	{
		Value v_(kArrayType);
		for (unsigned i = 0; i < 4; i++){
			v_.PushBack(i , d_.GetAllocator() );
		}
		json_.AddMember("a" , v_ , d_.GetAllocator() );
	}

	{
		Value js_arr(kArrayType);
		for (unsigned i = 0; i < 2; i++){
			Value js_obj(kObjectType);
			js_obj.AddMember("name" , "hbb" , d_.GetAllocator() );
			js_obj.AddMember("lv" , i , d_.GetAllocator() );

			js_arr.PushBack(js_obj , d_.GetAllocator() );
		}
		json_.AddMember("player_s" , js_arr , d_.GetAllocator() );
	}

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d_.Accept(writer);

	return buffer.GetString();
}



void test_fun()
{

	const int test_times = 10000;
	DWORD GetJsonStr_jsoncpp_time = 0, GetJsonStr_ss_time = 0 , GetJsonStr_doc_time = 0;

	GetJsonStr_jsoncpp_time = GetTickCount();
	for(int i = 0 ; i < test_times ; ++i){
		GetJsonStr_jsoncpp();
	}
	GetJsonStr_jsoncpp_time = GetTickCount() - GetJsonStr_jsoncpp_time;

	GetJsonStr_ss_time = GetTickCount();
	for(int i = 0 ; i < test_times ; ++i){
		GetJsonStr_ss();
	}
	GetJsonStr_ss_time = GetTickCount() - GetJsonStr_ss_time;

	GetJsonStr_doc_time = GetTickCount();
	for(int i = 0 ; i < test_times ; ++i){
		GetJsonStr_doc();
	}
	GetJsonStr_doc_time = GetTickCount() - GetJsonStr_doc_time;


	std::cout<<"GetJsonStr_jsoncpp_time:"<<GetJsonStr_jsoncpp_time<<std::endl;
	std::cout<<"GetJsonStr_ss_time:"<<GetJsonStr_ss_time<<std::endl;
	std::cout<<"GetJsonStr_doc_time:"<<GetJsonStr_doc_time<<std::endl;


	std::string json_str = GetJsonStr_ss();
	std::cout<<json_str<<std::endl;

	{
		Document d_;
		d_.Parse(json_str.c_str() );
		if(! d_.HasParseError() ){
			const Value & json_ = d_;
			std::cout<<json_["hello"].GetString()<<std::endl;
			std::cout<<json_["t"].GetBool()<<std::endl;

			{
				const Value & json_arr = json_["a"];
				size_t len_ = json_arr.Size();
				for(size_t i = 0 ; i < len_ ; ++i ){
					std::cout<<json_arr[i].GetUint()<<",";
				}
			}

			{
				const Value & json_arr = json_["player_s"];
				size_t len_ = json_arr.Size();
				for(size_t i = 0 ; i < len_ ; ++i ){
					const Value & json_ = json_arr[i];
					std::cout<<json_["name"].GetString()<<std::endl;
					std::cout<<json_["lv"].GetInt()<<std::endl;
				}
			}

			std::cout<<std::endl;
		}
	}
}


#endif

//测试vector删除重复的元素
#if 0
#include <algorithm>
#include <vector>
struct IntAndInt
{
	IntAndInt()
	{
		key = 0;
		value = 0;
	}

	IntAndInt(int key_, int v_) {
		key = key_;
		value = v_;
	}

	int key;
	int value;
};

static inline bool _IntAndInt_DownByValue(const IntAndInt & oLeft, const IntAndInt & oRight)
{
	return oLeft.value > oRight.value;
}

static inline bool _IntAndInt_EqByValue(const IntAndInt & oLeft, const IntAndInt & oRight)
{
	return oLeft.value == oRight.value;
}

void test_fun()
{
	std::vector<IntAndInt> t_vec;
	for (int i = 0; i < 5; ++i) {
		t_vec.push_back(IntAndInt(i , rand() % 3) );
	}
	std::sort(t_vec.begin() , t_vec.end(), _IntAndInt_DownByValue);
	std::vector<IntAndInt>::iterator pos_it = std::unique(t_vec.begin(), t_vec.end(), _IntAndInt_EqByValue);
	t_vec.erase(pos_it, t_vec.end() );
	int i = 0;
}

#endif

//排查定时器bug
#if 0
#include "../timer/minheap_timer_include.h"
#include "../timer/minheap_timer_module.h"
#include "../timer/timemgr.h"
using namespace SMinHeapSpace;


inline uint64 GetSystemTime(bool bLocal = true)
{
#if defined(WIN32) || defined(WIN64)
	struct timeb tp;
	ftime(&tp);
	if (!bLocal) {
		return tp.time * 1000 + tp.millitm;
	}
	else {
		return (tp.time - tp.timezone * 60L) * 1000 + tp.millitm;
	}
#else
	struct timeval stNow;
	struct timezone stZone;
	gettimeofday(&stNow, &stZone);
	if (bLocal) {
		return (stNow.tv_sec - stZone.tz_minuteswest * 60L) * 1000 + stNow.tv_usec / 1000;
	}
	else {
		return stNow.tv_sec * 1000 + stNow.tv_usec / 1000;
	}
#endif
}

class CCountry : public ITimer
{
	enum E_CCountry_TimerId
	{
		E_SubBroadCountryInfo = 1000,
		E_SubBroad_Job0 = 1001,
		E_SubBroad_Job1 = 1002,
		E_SubBroadCountryInfo_And_Rank = 1003,
		E_SubBroadCountry_Rank = 1004,
		//
	};
public:

	CCountry():m_oTimer( *_GetThis() ), m_nCount(0)
	{
	}

	~CCountry()
	{
		m_oTimer.KillAll();
	}

	void Init()
	{
		//int n_1 = 1;
		//for (int i = 0; i < E_SubBroadCountryInfo; ++i) {
		//	int n_len = (rand() % 10 ) + n_1;
		//	m_oTimer.SetTimer(i, (void *)NULL, n_len * 1000 , false);
		//}
		//m_oTimer.SetTimer(E_SubBroadCountryInfo, (void *)NULL , 20 , false);
		//m_oTimer.SetTimer(E_SubBroadCountryInfo_And_Rank, (void *)NULL, 5*60*1000, false);
		m_oTimer.SetTimer(E_SubBroadCountry_Rank, (void *)NULL, 2 * 1000, true);
	}

private:
	//定时器_begin 
	int m_nCount;
	CTimerWapper<CCountry>   m_oTimer;
public:
	CCountry *_GetThis() { return this; }
	virtual void STDCALL OnKillTimer(unsigned int nId)
	{
		m_oTimer.KillTimer(nId);
	}

	virtual void STDCALL OnTimer(unsigned int nId, UData &oData) 
	{
		if (nId == E_SubBroadCountryInfo) {
			int n_len = 10;
			std::cout << "###E_SubBroadCountryInfo  time = "<<n_len << std::endl;
			m_oTimer.SetTimer(nId, (void *)NULL, n_len *1000, false);
		}
		else if (nId == E_SubBroadCountryInfo_And_Rank) {
			int n_len = 7*24*60*60;
			std::cout << "###E_SubBroadCountryInfo_And_Rank  time = " << n_len << std::endl;
			m_oTimer.SetTimer(nId, (void *)NULL, n_len * 1000, false);
		}
		else {
			m_oTimer.KillTimer(nId);
		}
	}
	//定时器_end

};

CCountry g_country;
void test_main(int argc, char* argv[])
{
	//create
	CSysTime::CreateInstance();

	//init
	CSysTime::Instance()->SetMilliSecond(GetSystemTime());
	g_country.Init();



	while (1) {
		Sleep(1000);

		CSysTime::Instance()->SetMilliSecond(GetSystemTime());

		CMinHeapModule::Instance().Run();
	}
}

#endif

//多线程测试
#if 0
#include <ctime>
#include <thread>

#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <sys/timeb.h> 

long GetMillitm()
{
	struct timeb tv;
	ftime(&tv);
	return ((long)tv.time) * 1000 + (long)tv.millitm;
}


const int g_thead_num = 8;
const int gOneConut = 100000;
const int gConut = gOneConut * g_thead_num;
int gCurNum = 0;
clock_t gBegin, gEnd, gDiff;

inline void test_print() {
	for (int i = 0; i < 10000; ++i) {
		int b = i*i*i;
		b += 10;
	}
	gCurNum++;
}

void clear()
{
	gCurNum = 0;
}

void test2()
{
	for (int i = 0; i < gOneConut; ++i) {
		test_print();
	}
}

void test1()
{
	for (int i = 0; i < gConut; ++i) {
		test_print();
	}
}

void test_main(int argc, char* argv[])
{
	std::map<std::string, int> map_;

	{
		gBegin = GetMillitm();
		test1();
		gEnd = GetMillitm();
		gDiff = (gEnd - gBegin);

		system("pause");
		map_["单线程_耗时"] = gDiff;
		clear();
	}

	{
		std::vector<std::thread *> vec_;
		gBegin = GetMillitm();
		for (int i = 0; i < g_thead_num; ++i) {
			vec_.push_back(new std::thread(test2));
		}

		for (int i = 0; i < g_thead_num; ++i) {
			vec_[i]->join();
		}
		gEnd = GetMillitm();
		gDiff = (gEnd - gBegin);

		for (int i = 0; i < g_thead_num; ++i) {
			delete vec_[i];
		}
		system("pause");
		map_["多线程_耗时"] = gDiff;
		clear();
	}

	printf("run times %d  \n", gConut);
	for (std::map<std::string, int>::const_iterator it = map_.begin(); it != map_.end(); ++it) {
		printf("%s %d s %d ms \n", it->first.c_str(), it->second/ 1000, it->second%1000);
	}
}
//windows
//run times 800000
//单线程_耗时 17 s 917 ms
//多线程_耗时 3 s 118 ms


#endif 

//基类的 虚析构
#if 0
class CObj
{
public:
	CObj() {}
	virtual ~CObj() {
		printf(" %s\n", __FUNCTION__);
	}
public:
	bool m_bDel;
};

class hbb :public CObj
{
public:
	~hbb() {
		//如果 基类不用_虚析构_那么 该函数将不被触发
		printf(" %s\n", __FUNCTION__);
	}
};

void test_main(int argc, char* argv[]) {
	CObj *hbb_ = new hbb();
	delete hbb_;
}
#endif

//GUID
#if 0

#define GUID_LEN 64 
void test_main(int argc, char* argv[]) {
	char buffer[GUID_LEN] = { 0 };
	GUID guid;

	if (CoCreateGuid(&guid))
	{
		fprintf(stderr, "create guid error\n");
		return ;
	}
	_snprintf(buffer, sizeof(buffer),
		"%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
	printf("guid: %s\n", buffer);
}
#endif

//多线程测试 sleep
#if 0
#include <thread>
#include <stdio.h>


void base_sleep(const char * str , int n_)
{
	while (1) {
		Sleep(n_ * 1000);
		printf("%s stop %ds\n", str, n_);
	}
}

void test2()
{
	base_sleep(__FUNCTION__, 4);
}

void test1()
{
	base_sleep(__FUNCTION__, 2);
}

void test_main(int argc, char* argv[])
{
	std::thread t1(test1);
	std::thread t2(test2); 
	while (1) {
		Sleep(1);
	}
	t1.join();
	t2.join();

}


#endif 

//测试memmove的效率
#if 0
long GetMillitm()
{
	struct timeb tv;
	ftime(&tv);
	return ((long)tv.time) * 1000 + (long)tv.millitm;
}

struct clientOp {
	int index;
	clock_t	startTime;
};

static inline bool BasicPlayerSortDown(const clientOp* left_, const clientOp* right_)
{
	return left_->startTime < right_->startTime;
}

void test_main(int argc, char* argv[]) {
	clock_t nStart = GetMillitm();
	std::vector<clientOp*> vec_;
	for (size_t i = 0; i < 10*10000; ++i) {
		clientOp * p_ = new clientOp();
		p_->index = i;
		p_->startTime = rand() % 100;
		std::vector<clientOp*>::iterator find_it = std::lower_bound(vec_.begin(), vec_.end(), p_, BasicPlayerSortDown);
		int b = find_it - vec_.begin();
		int nLen = vec_.size();
		vec_.push_back(p_);
		if (b < nLen) {
			memmove(&vec_[b + 1], &vec_[b], (nLen-b) * sizeof(clientOp*));
			vec_[b] = p_;
		}
	}
	int nDiff = GetMillitm() - nStart;
	printf("##### \n");
	printf("%d \n" , nDiff);
}
#endif

//定时器
#if 0	

class CTimer;
class CTimerData
{
public:
	long long m_nUID;
	int m_nID;
	time_t m_nEndTime;
	CTimer * m_pTimer;
};

class CTimer
{
	std::map<long long, time_t> m_map;
public:
	virtual ~CTimer() {
		DelTimer();
	}

	virtual void OnTimer(int id, long long uid) = 0;

	void OnBaseTimer(int id, long long uid)
	{
		m_map.erase(uid);
		OnTimer( id , uid);
	}
	//新增 定时器
	long long AddTimer(int id, time_t time_);
	//删除 定时器
	void DelTimer(long long uid);
	void DelTimer();
};

class CTimerMgr
{
	CTimerMgr() {

	}
public:
	typedef std::set<CTimerData *> TimerSet;
	typedef std::map<time_t, std::set<CTimerData *> > TimerMap;

	static CTimerMgr& Instance()
	{
		static CTimerMgr g_oInstance;
		return g_oInstance;
	}

	CTimerData * GetFront() const 
	{
		if (m_mapData.empty()) {
			return NULL;
		}

		const TimerSet & set_ = m_mapData.begin()->second;
		if (set_.empty()) {
			return NULL;
		}
		return *set_.begin();
	}

	void Pop()
	{
		if (m_mapData.empty()) {
			return ;
		}

		TimerSet & set_ = m_mapData.begin()->second;
		if (set_.empty()) {
			m_mapData.erase(m_mapData.begin());
			return ;
		}
		Destroy(*set_.begin());
		set_.erase(set_.begin() );
	}

	bool DealOne()
	{
		if (m_mapData.empty()) {
			return false;
		}

		bool is_continue = true;

		do{
			CTimerData * pCTimerData = GetFront();
			if (pCTimerData == NULL) {
				break;
			}

			if (pCTimerData->m_nEndTime <= time(NULL)) {
				//调用 回调函数
				if (pCTimerData->m_pTimer) {
					pCTimerData->m_pTimer->OnBaseTimer(pCTimerData->m_nID , pCTimerData->m_nUID);
				}
			}
			else {
				return false;
			}

		} while (0);

		Pop();
		return is_continue;
	}

	bool Run(int count)
	{
		for (int i = 0; i < count; ++i) {
			if (DealOne() == false) {
				return false;
			}
		}
		return true;//任务 繁忙中
	}

	long long AddTimerJob(int id , time_t time_ , CTimer * p_timer)
	{
		static long long uid = 1000;
		CTimerData * pCTimerData = new CTimerData();
		pCTimerData->m_nEndTime = time_;
		pCTimerData->m_nUID = ++uid;
		pCTimerData->m_nID = id;
		pCTimerData->m_pTimer = p_timer;

		m_mapData[pCTimerData->m_nEndTime].insert(pCTimerData);
		return pCTimerData->m_nUID;
	}

	void Close(long long uid , time_t n_time) 
	{
		TimerMap::iterator it_ = m_mapData.find(n_time);
		if (it_ == m_mapData.end() ) {
			printf("%s , %d \n", __FUNCTION__, __LINE__);
			return;
		}

		TimerSet::iterator it_set = it_->second.begin();
		for (; it_set != it_->second.end(); ++it_set) {
			CTimerData * pCTimerData = *it_set;
			if (pCTimerData) {
				if (pCTimerData->m_nUID == uid) {
					Destroy(pCTimerData);
					it_->second.erase(pCTimerData);
					break;
				}
			}
		}
	}

	void Destroy(CTimerData* pCTimerData)
	{
		printf("%s , %d \n", __FUNCTION__, __LINE__);
		if (pCTimerData) {
			printf("删除 任务 id %d \n", pCTimerData->m_nID );
			delete pCTimerData; pCTimerData = NULL;
		}
	}

	void Destroy()
	{
		for (TimerMap::const_iterator it_ = m_mapData.begin();
			it_ != m_mapData.end(); ++it_) {

			for (TimerSet::const_iterator it_set = it_->second.begin(); it_set != it_->second.end(); ++it_set) {
				Destroy(*it_set);
			}
		}
		m_mapData.clear();
	}
	
	TimerMap m_mapData;
};

//新增 定时器
long long CTimer::AddTimer(int id, time_t time_)
{
	long long uid = CTimerMgr::Instance().AddTimerJob(id, time_, this);
	m_map[uid] = time_;
	return uid;
}

//删除 定时器
void CTimer::DelTimer()
{
	for (std::map<long long , time_t>::const_iterator it_ = m_map.begin();
		it_ != m_map.end(); ++it_) {
		CTimerMgr::Instance().Close(it_->first , it_->second);
	}
	m_map.clear();
}


void CTimer::DelTimer(long long uid)
{
	std::map<long long, time_t>::const_iterator it_ = m_map.find(uid);
	if (it_ != m_map.end()) {
		CTimerMgr::Instance().Close(it_->first, it_->second);
		m_map.erase(it_);
	}
}


class TestOne : public CTimer
{
public:
	virtual void OnTimer(int id, long long uid)
	{
		printf("%s , %d \n" , __FUNCTION__, __LINE__);
		printf("执行 任务id %d \n", id);
	}

};

class Player : public CTimer
{
public:
	virtual void OnTimer(int id, long long uid)
	{
		printf("%s , %d \n", __FUNCTION__, __LINE__);
		printf("执行 任务id %d \n", id);
	}
};


void test_main(int argc, char* argv[]) 
{
	Player oPlayer;
	for (int i = 0; i < 100000; ++i) {
		oPlayer.AddTimer(i, rand()%30 + time( NULL));//新增 定时器
	}

	TestOne oTestB;
	long long uid = oTestB.AddTimer(100, 11 + time(NULL));
	oTestB.DelTimer(uid);//删除 指定 定时器

	for (int i = 0; i < 100; ++i) 
	{
		TestOne oTest;
		oTest.AddTimer(12, 10 + time(NULL));
		//类型 析构时, 自动删除定时器
	}

	while (1) {
		CTimerMgr::Instance().Run(10);
		Sleep(100);
	}

}

#endif

//protobuff 反射
#if 0

#include "../protocol/base.pb.h"
#pragma comment(lib,"libprotobufD.lib")

//
//TestRaw* test_raw = new(std::nothrow) TestRaw();
//if (test_raw == NULL) {
//	printf("allocate test raw failed\");
//		return -1;
//}
//const Descriptor* descriptor = test_raw->GetDescriptor();
//const Reflection* reflection = test_raw->GetReflection();
//int field_count = descriptor->field_count();
//printf("field_count:%d\", field_count);
//	const FieldDescriptor* item1 = descriptor->FindFieldByNumber(1);
//if (item1->type() == FieldDescriptor::TYPE_UINT64) {
//	printf("uint64 type\");
//		reflection->SetUInt64(test_raw, item1, 100);
//	printf("item1:%d\", test_raw->item1());
//}

void test_main(int argc, char* argv[])
{
	using ::google::protobuf::Descriptor;
	using ::google::protobuf::Reflection;
	using ::google::protobuf::FieldDescriptor;

	SProtoSpace::client_skill_data one_;
	const Descriptor* descriptor = one_.GetDescriptor();
	const Reflection* reflection = one_.GetReflection();
	int field_count = descriptor->field_count();
	printf("field_count:%d\n", field_count);
	for (int i = 1; i <= field_count; ++i) {
		const FieldDescriptor* item1 = descriptor->FindFieldByNumber(i);
		if (item1->type() == FieldDescriptor::TYPE_INT32) {
			printf("TYPE_INT32 type\n");
			reflection->SetInt32(&one_, item1, 100);
		}
		else if (item1->type() == FieldDescriptor::TYPE_UINT32) {
			reflection->SetUInt32(&one_, item1, 200);
		}
	}
	printf("%s \n", one_.DebugString().c_str());
}



#endif 

//boost 无锁
#if 0



//迭代次数
const int iterations = 10 * 10000;

//对比测试
#if 1

#include <boost/lockfree/spsc_queue.hpp>
#include <iostream>
#include "thread.h"
#include <vector>
using namespace std;

//是否生产完毕标志
bool done = false;

//队列
boost::lockfree::spsc_queue<int, boost::lockfree::capacity<10 * 10000> > spsc_queue;


class Cproducer : public CThreadEx
{
	//生产函数
	virtual void run() {
		for (int i = 0; i != iterations; ++i) {
			//原子计数————多线程不存在计数不上的情况
			int value = ++producer_count;
			//cout << "*";     //观察生产类型： 纯生产还是同时有消费的情况

			//若没有进入队列，则重复推送
			while (!spsc_queue.push(value));
		}
	}

public:
	Cproducer() {
		producer_count = 0;
	}

	//生产数量
	int producer_count;
};

class Cconsumer : public CThreadEx
{
	//消费函数
	virtual void run() {
		int value;
		//当没有生产完毕，则边消费边生产
		while (!done) {
			//只要能弹出元素，就消费
			while (spsc_queue.pop(value)) {
				//cout << ".";     //观察消费类型： 纯消费还是边生产边消费
				++consumer_count;
			}
		}
		//如果生产完毕，则消费
		while (spsc_queue.pop(value))
			++consumer_count;
	}
public:
	Cconsumer() {
		consumer_count = 0;
	}
	//消费数量
	int consumer_count;
};


void test_main(int argc, char* argv[]) {
	if (!spsc_queue.is_lock_free()) {
		cout << "boost::lockfree::queue is not lockfree" << endl;
		return;
	}

	//线程群管理器
	std::vector<Cproducer *> vec_producer;
	std::vector<Cconsumer *> vec_consumer;

	//创建生产者线程
	{
		Cproducer * ptr_ = new Cproducer();
		ptr_->start();
		vec_producer.push_back(ptr_);
	}

	//创建消费者线程
	{
		Cconsumer * ptr_ = new Cconsumer();
		ptr_->start();
		vec_consumer.push_back(ptr_);
	}

	//等待生产者生产完毕
	for (size_t i = 0; i < vec_producer.size(); ++i) {
		while (vec_producer[i]->getStatus() != CThreadEx::EXIT) {
			sleep_msec(1);
		}
	}


	//可以消费标志
	done = true;     //主线程不等生产线程完毕就设置done标记
	cout << endl;
	cout << "done" << endl;    //输出以观察主线程和各子线程的执行顺序

							   //等待消费者结束
	for (size_t i = 0; i < vec_consumer.size(); ++i) {
		while (vec_consumer[i]->getStatus() != CThreadEx::EXIT) {
			sleep_msec(1);
		}
	}
	//输出生产和消费数量
	cout << endl;

	{
		int all_count = 0;
		for (size_t i = 0; i < vec_producer.size(); ++i) {
			all_count += vec_producer[i]->producer_count;
		}
		cout << "produced " << all_count << " objects." << endl;
	}

	{
		int all_count = 0;
		for (size_t i = 0; i < vec_consumer.size(); ++i) {
			all_count += vec_consumer[i]->consumer_count;
		}
		cout << "consumed " << all_count << " objects." << endl;
	}


	//释放资源
	//略
}

#else

#include <iostream>
#include "thread.h"
#include <vector>
#include <deque>
#include "mutex.h"
using namespace std;

//是否生产完毕标志
bool done = false;

//队列
std::deque<int> queue;
CLocker gLock;

class Cproducer : public CThreadEx
{
	//生产函数
	virtual void run() {
		for (int i = 0; i != iterations; ++i) {
			int value = ++producer_count;
			//cout << "*";     //观察生产类型： 纯生产还是同时有消费的情况

			//若没有进入队列，则重复推送
			gLock.Lock();
			queue.push_front(value);
			gLock.Unlock();
		}
	}

public:
	Cproducer() {
		producer_count = 0;
	}

	//生产数量
	int producer_count;
};

class Cconsumer : public CThreadEx
{
	//消费函数
	virtual void run() {
		int value;
		//当没有生产完毕，则边消费边生产
		while (!done) {
			//只要能弹出元素，就消费
			gLock.Lock();
			if (queue.empty()) {
				gLock.Unlock();
				continue;
			}
			value = queue.back();
			queue.pop_back();
			gLock.Unlock();

			//cout << ".";     //观察消费类型： 纯消费还是边生产边消费
			++consumer_count;
		}
		//如果生产完毕，则消费
		while (1) {
			gLock.Lock();
			if (queue.empty()) {
				gLock.Unlock();
				break;
			}
			value = queue.back();
			queue.pop_back();
			gLock.Unlock();

			++consumer_count;
		}
	}
public:
	Cconsumer() {
		consumer_count = 0;
	}
	//消费数量
	int consumer_count;
};


void test_main(int argc, char* argv[]) {

	//线程群管理器
	std::vector<Cproducer *> vec_producer;
	std::vector<Cconsumer *> vec_consumer;

	//创建生产者线程
	{
		Cproducer * ptr_ = new Cproducer();
		ptr_->start();
		vec_producer.push_back(ptr_);
	}

	//创建消费者线程
	{
		Cconsumer * ptr_ = new Cconsumer();
		ptr_->start();
		vec_consumer.push_back(ptr_);
	}

	//等待生产者生产完毕
	for (size_t i = 0; i < vec_producer.size(); ++i) {
		while (vec_producer[i]->getStatus() != CThreadEx::EXIT) {
			sleep_msec(1);
		}
	}


	//可以消费标志
	done = true;     //主线程不等生产线程完毕就设置done标记
	cout << endl;
	cout << "lock done" << endl;    //输出以观察主线程和各子线程的执行顺序

									//等待消费者结束
	for (size_t i = 0; i < vec_consumer.size(); ++i) {
		while (vec_consumer[i]->getStatus() != CThreadEx::EXIT) {
			sleep_msec(1);
		}
	}
	//输出生产和消费数量
	cout << endl;

	{
		int all_count = 0;
		for (size_t i = 0; i < vec_producer.size(); ++i) {
			all_count += vec_producer[i]->producer_count;
		}
		cout << "produced " << all_count << " objects." << endl;
	}

	{
		int all_count = 0;
		for (size_t i = 0; i < vec_consumer.size(); ++i) {
			all_count += vec_consumer[i]->consumer_count;
		}
		cout << "consumed " << all_count << " objects." << endl;
	}


	//释放资源
	//略
}
#endif


#endif

//memory order 测试_全局变量未更新_bug
#if 0


#include "thread.h"
#include <assert.h>


#if defined(WIN32) || defined(WIN64)
#include <atomic>
using namespace std;
#else
#include <boost/atomic.hpp>
using namespace boost;
#endif

int g_exp = 0;
const int g_total = 1;

//对比测试
#if 0

atomic<int> g_lv(0);
class CReadLV :public CThreadEx
{
	virtual void run() {
		while (g_lv.load(memory_order_acquire) != 1);
		set(g_exp);
	}
	void set(int x) { m_nNum = x; }
public:
	CReadLV() {
		m_nNum = 0;
	}
	int m_nNum;
};

class CSetData :public CThreadEx
{
	virtual void run() {
		g_exp = 42;
		g_lv.store(1, memory_order_release);
		//printf("g_lv :%d \n", g_lv);
	}
public:
	CSetData() {
		m_nNum = 0;
	}
	int m_nNum;
};

#else
int g_lv = 0;
class CReadLV :public CThreadEx
{
	virtual void run() {
		while ( g_lv != 1);
		set(g_exp);
	}
	void set(int x) { m_nNum = x; }
public:
	CReadLV() {
		m_nNum = 0;
	}
	int m_nNum;
};

class CSetData :public CThreadEx
{
	virtual void run() {
		g_exp = 42;
		g_lv = 1;
		printf("g_lv :%d \n", g_lv);
	}
public:
	CSetData() {
		m_nNum = 0;
	}
	int m_nNum;
};
#endif




void run_once()
{
	g_lv = 0;
	g_exp = 0;
	CReadLV readObj;
	CSetData setObj;

	readObj.start();
	setObj.start();

	while (readObj.getStatus() != CThreadEx::EXIT) {
		sleep_msec(1);
	}

	while (setObj.getStatus() != CThreadEx::EXIT) {
		sleep_msec(1);
	}

	if (readObj.m_nNum != 42) {
		printf("*");
		//assert(0);
	}
}

void test_main(int argc, char* argv[])
{
	for (int i = 0; i < g_total; ++i) {
		run_once();
	}
}

#endif

//boost 内存池
#if 0

#include <boost/pool/pool.hpp>


class hbbObj
{
public:
	hbbObj(){
		printf("%s \n", __FUNCTION__);
	}

	~hbbObj() {
		printf("%s \n", __FUNCTION__);
	}

	int m1;
	std::string m2;
};

template <class T>
class CPool
{
public:
	CPool():m_pool(sizeof(T)) {}
	T * Create() {
		T * ptr_ = (T *)m_pool.malloc();
		new (ptr_) T();
		return ptr_;
	}

	void Release(T * ptr_) {
		ptr_->~T();
		m_pool.free(ptr_);
	}
private:
	boost::pool<> m_pool;
};

CPool<hbbObj> gPool;

void test_main(int argc, char* argv[]) {
	hbbObj * ptr_ = gPool.Create();
	gPool.Release(ptr_);
}

#endif

//boost asio_server_收发多线程
#if 0

#define BOOST_ERROR_CODE_HEADER_ONLY
#define BOOST_REGEX_NO_LIB
#define BOOST_DATE_TIME_SOURCE
#define BOOST_SYSTEM_NO_LIB

#include <boost/asio.hpp>

#include <boost/pool/pool.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include "thread.h"
#include "base64.h"

using boost::asio::ip::tcp;


template <class T >
class CPool
{
public:
	CPool() :m_pool(sizeof(T)) {}
	T * CreateSession(boost::asio::io_service &io_service) {
		T * ptr_ = (T *)m_pool.malloc();
		new (ptr_) T(io_service);
		return ptr_;
	}

	void Release(T * ptr_) {
		ptr_->~T();
		m_pool.free(ptr_);
	}
private:
	boost::pool<> m_pool;
};


class CSession;//声明
struct SOptSession;//声明
inline void FreeSession(int socket_id);//声明
inline bool Add2MsgQueue(int nSocketID, short nMsgID, const std::string & str_);//声明
inline bool Add2SendQueue(int nSocketID, short nMsgID, const std::string & str_);//声明
inline void CloseSession(int socket_id);//声明
inline CSession * GetSession(int socket_id);//声明
inline bool Add2CloseNetQueue(int nSocketID);//声明

enum E_Session_Status
{
	E_S_None = 0,
	E_S_Begin ,
	//E_S_Reading,
	E_S_Read_Error,
	//E_S_End,
};

struct SOptSession {
	int nSocketID;
	int nMsgID;
	std::string strContent;
};

struct MsgHead {
	short nLen;
	short nMsgID;
};

//全局变量
#define Max_Session (1024 * 10)
CPool<CSession > gPool;
CSession * gSession[Max_Session];
boost::lockfree::spsc_queue<SOptSession, boost::lockfree::capacity<Max_Session * 10> > gMsgQueue;
boost::lockfree::spsc_queue<SOptSession, boost::lockfree::capacity<Max_Session * 10> > gSendQueue;
boost::lockfree::spsc_queue<int, boost::lockfree::capacity<Max_Session * 10> > gCloseNetQueue;


class CSession
{
public:
#define MSG_MAX_  (1024*101)
	CSession(boost::asio::io_service &io_service) : m_oSocket(io_service) {
		m_nStatus = E_S_None;
	}

	void start()
	{
		m_nDataLen = 0;
		m_nStatus = E_S_Begin;
		async_read();
	}
private:
	bool GetMsg() {
		if (m_nDataLen > 2) {
			short len_ = ((short *)m_arrData.data()) [0];
			len_ = ntohs(len_);
			if (len_ + 2 <= m_nDataLen) {

				if (m_nDataLen > 4) {
					short msg_id = ((short *)m_arrData.data())[1];
					msg_id = ntohs(msg_id);
					if (Add2MsgQueue(m_nID, msg_id, std::string(m_arrData.data() + 2 + 2, len_ - 2)) == false) {
						printf("%s:%d thread_id : %d \n", __FUNCTION__, __LINE__, GetCurrentThreadId());
						return false;
					}
				}

				m_nDataLen -= len_ + 2;
				memmove(m_arrData.data() , m_arrData.data() + len_ +2 , m_nDataLen);
				return true;
			}
		}
		return false;
	}

	void async_read()
	{
		int len_ = MSG_MAX_ - m_nDataLen;
		if (len_ <= 0) {
			m_nStatus = E_S_Read_Error;
			FreeSession(m_nID);
			return;
		}

		m_oSocket.async_read_some(boost::asio::buffer(m_arrData.data() + m_nDataLen , len_),
			std::bind(&CSession::read_cb , this, std::placeholders::_1 , std::placeholders::_2)
		);
	}

	void read_cb(const boost::system::error_code &ec , std::size_t length)
	{
		m_nDataLen += length;
		while (GetMsg());

		if (!ec) {	
			async_read();
		}
		else {
			m_nStatus = E_S_Read_Error;
			FreeSession(m_nID);
		}
	}
public:
	void SendMsg(const char * buff_ , std::size_t length)
	{
		//zhou//测试下//同步写快,还是异步写快
		boost::system::error_code ec;
		size_t len = boost::asio::write(m_oSocket , boost::asio::buffer(buff_, length), ec);
	}

public:
	int m_nID;
	E_Session_Status m_nStatus;
	tcp::socket m_oSocket;
private:
	std::array<char, MSG_MAX_> m_arrData;
	int m_nDataLen;
	
};

//网络_线程//可调用
inline bool Add2MsgQueue(int nSocketID , short nMsgID, const std::string & str_)
{
	SOptSession obj_;
	obj_.nSocketID = nSocketID;
	obj_.strContent = str_;
	obj_.nMsgID = nMsgID;
	return gMsgQueue.push(obj_);
}

//主_线程//可调用
inline bool Deal_MsgQueue(int count) {
	for (int i = 0; i < count; ++i) {
		SOptSession obj_;
		if (gMsgQueue.pop(obj_)) {
			//printf("msg_id :%d ,msg:%s \n", obj_.nMsgID, obj_.strContent.c_str() );
			//printf("cur socket_id :%d \n", obj_.nSocketID);
			printf("-");
			if (obj_.nMsgID == 10) {
				if (Add2SendQueue(obj_.nSocketID, 12, std::string("ok")) == false) {
					printf("%s:%d thread_id : %d \n", __FUNCTION__, __LINE__,GetCurrentThreadId());
				}
			}
			else if (obj_.nMsgID == 8) {
				//关闭网络
				if (Add2CloseNetQueue(obj_.nSocketID) == false) {
					printf("%s:%d thread_id : %d \n", __FUNCTION__, __LINE__, GetCurrentThreadId());
				}
			}
		}
		else {
			break;
		}
	}
	return gMsgQueue.empty();
}


//主_线程//可调用
inline bool Add2SendQueue(int nSocketID, short nMsgID, const std::string & str_)
{
	MsgHead oHead;
	oHead.nLen = short(str_.size() + 2);
	oHead.nMsgID = short(nMsgID);
	oHead.nLen = htons(oHead.nLen);
	oHead.nMsgID = htons(oHead.nMsgID);

	SOptSession obj_;
	obj_.nSocketID = nSocketID;
	obj_.strContent.resize(str_.size() + 2 + 2);


	memcpy( ( void *)obj_.strContent.data() , (const void *)&oHead, sizeof(MsgHead) );
	memcpy( (void *)(obj_.strContent.data() + sizeof(MsgHead) ) , (const void *)str_.data() , str_.size() );
	obj_.nMsgID = nMsgID;
	return gSendQueue.push(obj_);
}

//网络_线程//可调用
inline bool Deal_SendQueue(int count) {
	for (int i = 0; i < count; ++i) {
		SOptSession obj_;
		if (gSendQueue.pop(obj_)) {
			CSession *ptr_ = GetSession(obj_.nSocketID);
			if (ptr_) {
				ptr_->SendMsg(obj_.strContent.c_str(), obj_.strContent.size());
			}
		}
		else {
			break;
		}
	}
	return gSendQueue.empty();
}

//主_线程//可调用
inline bool Add2CloseNetQueue(int nSocketID)
{
	return gCloseNetQueue.push(nSocketID);
}

//网络_线程//可调用
inline bool Deal_CloseNetQueue(int count) {
	for (int i = 0; i < count; ++i) {
		int id_;
		if (gCloseNetQueue.pop(id_)) {
			CloseSession(id_);
		}
		else {
			break;
		}
	}
	return gCloseNetQueue.empty();
}



//网络_线程//可调用
inline CSession * CreateSession(boost::asio::io_service &io_service)
{
	for (int i = 0; i < sizeof(gSession) / sizeof(gSession[0]); ++i) {
		if (gSession[i] != NULL) {
			continue;
		}
		gSession[i] = gPool.CreateSession(io_service);

		if (gSession[i] == NULL) {
			return NULL;
		}

		gSession[i]->m_nID = i;
		printf("create socket_id:%d \n" , i);
		return gSession[i];
	}

	return NULL;
}

//网络_线程//可调用
inline CSession * GetSession(int socket_id) {
	if ( (size_t) socket_id < sizeof(gSession) / sizeof(gSession[0]) ) {
		return gSession[socket_id];
	}
	return NULL;
}

//网络_线程//可调用
inline void CloseSession(int socket_id) {
	CSession * ptr_ = GetSession(socket_id);
	if (ptr_) {
		if (ptr_->m_oSocket.is_open() ) {
			ptr_->m_oSocket.shutdown(tcp::socket::shutdown_both);
			ptr_->m_oSocket.close();
		}
		printf("%s thread_id : %d \n", __FUNCTION__, GetCurrentThreadId());
	}
}

//网络_线程//可调用//该函数 私有
inline void FreeSession(int socket_id) {
	CSession * ptr_ = GetSession(socket_id);
	if (ptr_) {
		gPool.Release(ptr_);
		gSession[socket_id] = NULL;
		printf("free socket_id:%d \n", socket_id);
	}
}

//网络_线程//可调用
inline void FreeSession() {
	for (size_t i = 0; i < sizeof(gSession) / sizeof(gSession[0]) ; ++i ) {
		if (gSession[i] == NULL) {
			continue;
		}
		gPool.Release(gSession[i]);
		gSession[i] = NULL;
	}
}

class CServer
{
public:
	CServer(boost::asio::io_service &io_service, short port)
		: m_oAcceptor(io_service, tcp::endpoint(tcp::v4(), port)), m_oIO(io_service)
	{
		async_accept();
	}

private:
	void async_accept()
	{
		CSession * ptr_session = CreateSession(m_oIO);
		if (ptr_session == NULL) {
			printf("error in %s " , __FUNCTION__);
			return;
		}

		m_oAcceptor.async_accept(ptr_session->m_oSocket, std::bind(&CServer::handle_accept, this, ptr_session->m_nID, std::placeholders::_1));
	}

	void handle_accept(int socket_id , const boost::system::error_code &ec)
	{
		if (!ec)
		{
			CSession * ptr_ = GetSession(socket_id);
			if (ptr_) {
				ptr_->start();
			}
		}
		else {
			FreeSession(socket_id);
			printf("error %s \n", ec.message().c_str() );
		}
		async_accept();  // 继续监听客户端连接请求
	}

	tcp::acceptor m_oAcceptor;
	boost::asio::io_service & m_oIO;
};

class COptServer :public CThreadEx
{

	virtual void run() {
		printf("%s thread_id : %d \n", __FUNCTION__, GetCurrentThreadId());
		boost::asio::io_service o_io;
		CServer o_server(o_io, 2017);

		for (; ; ) {//网络_线程
			bool is_empty = false;
			if (o_io.poll_one() == 0) {
					is_empty = true;
			}
			else {
				printf("*");
			}

			bool is_empty2 = Deal_SendQueue(50);
			bool is_empty3 = Deal_CloseNetQueue(50);

			if (is_empty && is_empty2 && is_empty3) {
				sleep_msec(1);//xx毫秒
			}
		}
		//zhou//捕获关闭信号

		//处理_关闭网络_请求//网络_线程
		//释放资源//网络_线程
		o_io.stop();
		FreeSession();
		printf("%s thread_id : %d is end \n", __FUNCTION__, GetCurrentThreadId());
	}
};

void test_main(int argc, char* argv[]) {
	printf("%s thread_id : %d \n", __FUNCTION__, GetCurrentThreadId());
	
	COptServer oNet;
	oNet.start();

	for (; ; ) {
		bool is_empty = Deal_MsgQueue(50);

		if (is_empty) {
			sleep_msec(1);//xx毫秒
		}
	}
	//zhou//捕获关闭信号

}

#endif

//boost asio_server_and_client收发单线程
#if 0

#define BOOST_ERROR_CODE_HEADER_ONLY
#define BOOST_REGEX_NO_LIB
#define BOOST_DATE_TIME_SOURCE
#define BOOST_SYSTEM_NO_LIB

#include <boost/asio.hpp>
#include <boost/pool/pool.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include "thread.h"
#include "base64.h"

using boost::asio::ip::tcp;


template <class T >
class CPool
{
public:
	CPool() :m_pool(sizeof(T)) {}
	T * CreateSession(boost::asio::io_service &io_service) {
		T * ptr_ = (T *)m_pool.malloc();
		new (ptr_) T(io_service);
		return ptr_;
	}

	void Release(T * ptr_) {
		ptr_->~T();
		m_pool.free(ptr_);
	}
private:
	boost::pool<> m_pool;
};

enum E_Session_Status
{
	E_S_None = 0,
	E_S_Begin,
	//E_S_Reading,
	E_S_Read_Error,
	//E_S_End,
};

struct MsgHead {
	short nLen;
	short nMsgID;
};

inline void write_handler(int nModuleID, int nScoketID, const boost::system::error_code& error, std::size_t bytes_transferred)
{
	//建议这个回调用全局的//整个框架有2个回调//一个读回调,一个写回调//它们都在一个线程处理//读回调_会释放类对象//
	//暂无
}

typedef std::vector<int> ToBeDeletedSocketIDs;

class CSession :public CNoncopy
{
public:
#define MSG_MAX_  (1024*101)
	CSession(boost::asio::io_service &io_service) : m_oSocket(io_service) {
		m_nStatus = E_S_None;
		m_pvecFreeSession = NULL;
		m_nModuleID = 0;
	}

	virtual ~CSession() {}

	void start(ToBeDeletedSocketIDs * p_vec, int nModuleID)
	{
		m_pvecFreeSession = p_vec;
		m_nModuleID = nModuleID;
		m_nDataLen = 0;
		m_nStatus = E_S_Begin;
		OnInit();
		async_read();
	}

	void CloseSession() {
		if (m_oSocket.is_open()) {
			m_oSocket.shutdown(tcp::socket::shutdown_both);
			m_oSocket.close();
		}
		printf("%s thread_id : %d \n", __FUNCTION__, GetCurrentThreadId());
	}

	void FreeSession() {
		if (m_pvecFreeSession) {
			(*m_pvecFreeSession).push_back(m_nSocketID);
		}
		OnFreeSession();
	}


	//zhou//开始接收消息
	virtual void OnInit() = 0;
	virtual bool OnDeal_Msg(short nMsgID, const std::string & str_) = 0;
	virtual void OnFreeSession() = 0;
private:
	bool GetMsg() {
		if (m_nDataLen > 2) {
			short len_ = ((short *)m_arrData.data())[0];
			len_ = ntohs(len_);
			if (len_ + 2 <= m_nDataLen) {

				if (m_nDataLen > 4) {
					short msg_id = ((short *)m_arrData.data())[1];
					msg_id = ntohs(msg_id);
					if (OnDeal_Msg(msg_id, std::string(m_arrData.data() + 2 + 2, len_ - 2)) == false) {
						printf("%s:%d thread_id : %d \n", __FUNCTION__, __LINE__, GetCurrentThreadId());
						return false;
					}
				}

				m_nDataLen -= len_ + 2;
				memmove(m_arrData.data(), m_arrData.data() + len_ + 2, m_nDataLen);
				return true;
			}
		}
		return false;
	}

	void async_read()
	{
		int len_ = MSG_MAX_ - m_nDataLen;
		if (len_ <= 0) {
			m_nStatus = E_S_Read_Error;
			FreeSession();
			return;
		}

		m_oSocket.async_read_some(boost::asio::buffer(m_arrData.data() + m_nDataLen, len_),
			std::bind(&CSession::read_cb, this, std::placeholders::_1, std::placeholders::_2)
		);
	}

	void read_cb(const boost::system::error_code &ec, std::size_t length)
	{
		m_nDataLen += length;
		while (GetMsg());
		//当网络close后//read不会异常
		//当网络close后//read能触发回调
		if (!ec) {
			async_read();
		}
		else {
			m_nStatus = E_S_Read_Error;
			FreeSession();
		}
	}
public:
	void BaseSendMsg(const char * buff_, std::size_t length)
	{
#if 1
		m_oSocket.async_write_some(boost::asio::buffer(buff_, length), std::bind(write_handler, m_nModuleID, m_nSocketID, std::placeholders::_1, std::placeholders::_2));
#else
		boost::system::error_code ec;
		size_t len = boost::asio::write(m_oSocket, boost::asio::buffer(buff_, length), ec);
#endif
	}

public:
	int m_nSocketID;
	E_Session_Status m_nStatus;
	tcp::socket m_oSocket;
private:
	std::array<char, MSG_MAX_> m_arrData;
	int m_nDataLen;
	ToBeDeletedSocketIDs * m_pvecFreeSession;
	int m_nModuleID;
};

//server_
#if 0

template<class T>
class CServer :public CNoncopy
{
public:
#define Max_Session (1024 * 10)

	CServer(short port, int nModuleID)
		: m_oAcceptor(m_oIO, tcp::endpoint(tcp::v4(), port))
	{
		m_nModuleID = nModuleID;
		memset(m_arrSession, 0, sizeof(m_arrSession));
		async_accept();
	}

	void DealFreeSessionReq() {
		for (size_t i_ = 0; i_ < m_vecFreeSession.size(); ++i_) {
			const int & one_ = m_vecFreeSession[i_];
			FreeOneSession(one_);
		}
		m_vecFreeSession.clear();
	}

	T * GetSession(int socket_id) {
		if ((size_t)socket_id < sizeof(m_arrSession) / sizeof(m_arrSession[0])) {
			return m_arrSession[socket_id];
		}
		return NULL;
	}

	void Stop() {
		m_oIO.stop();
		FreeAllSession();
	}

private:

	void FreeOneSession(int socket_id) {
		T * ptr_ = GetSession(socket_id);
		if (ptr_) {
			m_oPool.Release(ptr_);
			m_arrSession[socket_id] = NULL;
			printf("free socket_id:%d \n", socket_id);
		}
	}

	void FreeAllSession() {
		for (size_t i = 0; i < sizeof(m_arrSession) / sizeof(m_arrSession[0]); ++i) {
			if (m_arrSession[i] == NULL) {
				continue;
			}
			m_oPool.Release(m_arrSession[i]);
			m_arrSession[i] = NULL;
		}
	}

	T * CreateSession(boost::asio::io_service &io_service)
	{
		for (int i = 0; i < sizeof(m_arrSession) / sizeof(m_arrSession[0]); ++i) {
			if (m_arrSession[i] != NULL) {
				continue;
			}
			m_arrSession[i] = m_oPool.CreateSession(io_service);

			if (m_arrSession[i] == NULL) {
				return NULL;
			}

			m_arrSession[i]->m_nSocketID = i;
			printf("create socket_id:%d \n", i);
			return m_arrSession[i];
		}

		return NULL;
	}

	void async_accept()
	{
		T * ptr_session = CreateSession(m_oIO);
		if (ptr_session == NULL) {
			printf("error in %s ", __FUNCTION__);
			return;
		}

		m_oAcceptor.async_accept(ptr_session->m_oSocket, std::bind(&CServer::handle_accept, this, ptr_session->m_nSocketID, std::placeholders::_1));
	}

	void handle_accept(int socket_id, const boost::system::error_code &ec)
	{
		if (!ec)
		{
			T * ptr_ = GetSession(socket_id);
			if (ptr_) {
				ptr_->start(&m_vecFreeSession, m_nModuleID);
			}
		}
		else {
			FreeOneSession(socket_id);
			printf("error %s \n", ec.message().c_str());
		}
		async_accept();  // 继续监听客户端连接请求
	}

public:
	boost::asio::io_service m_oIO;
private:
	CPool<T > m_oPool;
	T * m_arrSession[Max_Session];
	tcp::acceptor m_oAcceptor;
	ToBeDeletedSocketIDs m_vecFreeSession;//在下一帧对容器进行操作
	int m_nModuleID;
};



//举例================
//全局变量
class CGateSession;
CServer<CGateSession> * P_server = NULL;

class CGateSession :public CSession
{
public:
	CGateSession(boost::asio::io_service &io_service) :CSession(io_service) {}


	//zhou//给 其他session 发消息
	void Send2OtherServer() {
		int nSocketID = 0;
		if (P_server == NULL) {
			return;
		}

		CGateSession *ptr_ = P_server->GetSession(nSocketID);
		if (ptr_ == NULL) {
			return;
		}
		//ptr_->Send_Msg();
	}

	virtual void OnInit()
	{
		//绑定玩家数据
	}

	virtual bool OnDeal_Msg(short nMsgID, const std::string & str_) {
		if (nMsgID == 10) {
			if (Send_Msg(12, std::string("ok")) == false) {
				printf("%s:%d thread_id : %d \n", __FUNCTION__, __LINE__, GetCurrentThreadId());
			}
		}
		else if (nMsgID == 8) {
			//关闭网络//并解除对应的玩家绑定
			CloseSession();
		}
		//zhou//处理协议
		return true;
	}

	bool Send_Msg(short nMsgID, const std::string & str_) {
		MsgHead oHead;
		oHead.nLen = short(str_.size() + 2);
		oHead.nMsgID = short(nMsgID);
		oHead.nLen = htons(oHead.nLen);
		oHead.nMsgID = htons(oHead.nMsgID);

		std::string strContent;
		strContent.resize(str_.size() + 2 + 2);

		memcpy((void *)strContent.data(), (const void *)&oHead, sizeof(MsgHead));
		memcpy((void *)(strContent.data() + sizeof(MsgHead)), (const void *)str_.data(), str_.size());

		BaseSendMsg(strContent.c_str(), strContent.size());
		return true;
	}

	virtual void OnFreeSession() {
		//解绑玩家数据
	}
};


void test_main(int argc, char* argv[]) {

	printf("%s thread_id : %d \n", __FUNCTION__, GetCurrentThreadId());
	P_server = new CServer<CGateSession>(2017, 0);
	if (P_server == NULL) {
		return;
	}

	for (; ; ) {//网络_线程
		bool is_empty = false;
		if (P_server->m_oIO.poll_one() == 0) {
			is_empty = true;
		}
		else {
			printf("*");
		}
		P_server->DealFreeSessionReq();

		if (is_empty) {
			sleep_msec(1);//xx毫秒
		}
	}
	//zhou//捕获关闭信号

	//处理_关闭网络_请求
	//释放资源
	P_server->Stop();

	delete P_server; P_server = NULL;
	printf("%s thread_id : %d is end \n", __FUNCTION__, GetCurrentThreadId());

}


#else


template<class T>
class CClient :public CNoncopy
{
public:
#define Max_Session (1024 * 10)

	CClient(int nModuleID)
	{
		m_nModuleID = nModuleID;
		memset(m_arrSession, 0, sizeof(m_arrSession));
	}

	T * Connect(const std::string str_ip, const std::string str_port)
	{
		T * ptr_session = CreateSession(m_oIO);
		if (ptr_session == NULL) {
			printf("error in %s ", __FUNCTION__);
			return NULL;
		}

		//zhou//再封装一个 异步的方法

		tcp::resolver r(m_oIO);
		tcp::resolver::query q(str_ip.c_str(), str_port.c_str());
		boost::system::error_code ec;
		boost::asio::connect(ptr_session->m_oSocket, r.resolve(q), ec);
		if (!ec) {
			ptr_session->start(&m_vecFreeSession, m_nModuleID);
			return ptr_session;
		}
		else {
			FreeOneSession(ptr_session->m_nSocketID);
			return NULL;
		}
	}

	void DealFreeSessionReq() {
		for (size_t i_ = 0; i_ < m_vecFreeSession.size(); ++i_) {
			const int & one_ = m_vecFreeSession[i_];
			FreeOneSession(one_);
		}
		m_vecFreeSession.clear();
	}

	T * GetSession(int socket_id) {
		if ((size_t)socket_id < sizeof(m_arrSession) / sizeof(m_arrSession[0])) {
			return m_arrSession[socket_id];
		}
		return NULL;
	}

	void Stop() {
		m_oIO.stop();
		FreeAllSession();
	}

private:

	void FreeOneSession(int socket_id) {
		T * ptr_ = GetSession(socket_id);
		if (ptr_) {
			m_oPool.Release(ptr_);
			m_arrSession[socket_id] = NULL;
			printf("free socket_id:%d \n", socket_id);
		}
	}

	void FreeAllSession() {
		for (size_t i = 0; i < sizeof(m_arrSession) / sizeof(m_arrSession[0]); ++i) {
			if (m_arrSession[i] == NULL) {
				continue;
			}
			m_oPool.Release(m_arrSession[i]);
			m_arrSession[i] = NULL;
		}
	}

	T * CreateSession(boost::asio::io_service &io_service)
	{
		for (int i = 0; i < sizeof(m_arrSession) / sizeof(m_arrSession[0]); ++i) {
			if (m_arrSession[i] != NULL) {
				continue;
			}
			m_arrSession[i] = m_oPool.CreateSession(io_service);

			if (m_arrSession[i] == NULL) {
				return NULL;
			}

			m_arrSession[i]->m_nSocketID = i;
			printf("create socket_id:%d \n", i);
			return m_arrSession[i];
		}

		return NULL;
	}

public:
	boost::asio::io_service m_oIO;
private:
	CPool<T > m_oPool;
	T * m_arrSession[Max_Session];
	ToBeDeletedSocketIDs m_vecFreeSession;//在下一帧对容器进行操作
	int m_nModuleID;
};



//举例================
inline time_t GetCurMs();//声明
						 //全局变量
#define Max_Send_Msg (2500)
#define Session_Num (80)
volatile int Total_All_Send_Msg = 0;
class CGateSession;
CClient<CGateSession> * P_client = NULL;

class CGateSession :public CSession
{
public:
	CGateSession(boost::asio::io_service &io_service) :CSession(io_service)
	{
		m_nBeginT = 0;
		m_nEndT = 0;
		m_nSendNum = 0;
	}

public:

	time_t m_nBeginT;
	time_t m_nEndT;
	int m_nSendNum;

	//给 其他session 发消息
	void Send2OtherServer() {
		int nSocketID = 0;
		if (P_client == NULL) {
			return;
		}

		CGateSession *ptr_ = P_client->GetSession(nSocketID);
		if (ptr_ == NULL) {
			return;
		}
		//ptr_->Send_Msg();
	}

	virtual void OnInit()
	{
		//绑定玩家数据
	}

	virtual bool OnDeal_Msg(short nMsgID, const std::string & str_) {
		if (nMsgID == 12) {
			if (Send_Msg(10, std::string(10240 ,'h')) == false) {
				printf("%s:%d thread_id : %d \n", __FUNCTION__, __LINE__, GetCurrentThreadId());
			}
		}
		//处理协议
		return true;
	}

	bool Send_Msg(short nMsgID, const std::string & str_) {
		if (nMsgID == 10) {
			if (m_nSendNum < Max_Send_Msg) {
				++m_nSendNum;
				++Total_All_Send_Msg;
				m_nEndT = GetCurMs();
			}
			else {
				return false;
			}
		}

		MsgHead oHead;
		oHead.nLen = short(str_.size() + 2);
		oHead.nMsgID = short(nMsgID);
		oHead.nLen = htons(oHead.nLen);
		oHead.nMsgID = htons(oHead.nMsgID);

		std::string strContent;
		strContent.resize(str_.size() + 2 + 2);

		memcpy((void *)strContent.data(), (const void *)&oHead, sizeof(MsgHead));
		memcpy((void *)(strContent.data() + sizeof(MsgHead)), (const void *)str_.data(), str_.size());

		BaseSendMsg(strContent.c_str(), strContent.size());
		return true;
	}

	virtual void OnFreeSession() {
		//解绑玩家数据
	}
};


void test_main(int argc, char* argv[]) {

	if (argc < 5) {
		return;
	}
	time_t nBeginTime = _atoi64(argv[1]);
	int nAppID = atoi(argv[2]);
	std::string str_ip(argv[3]);
	std::string str_port(argv[4]);

	printf("%s thread_id : %d \n", __FUNCTION__, GetCurrentThreadId());
	P_client = new CClient<CGateSession>(0);
	if (P_client == NULL) {
		return;
	}
	std::vector<int> vecSocketID;
	std::stringstream oss_;

	for (int i = 0; i < Session_Num; ++i) {
		CGateSession *ptr_ = P_client->Connect(str_ip, str_port);
		if (ptr_) {
			vecSocketID.push_back(ptr_->m_nSocketID);
		}
		else {
			printf("#");
		}
	}

	int nStep = 0;
	for (int run_i = 0; ; ++run_i) {//网络_线程
		bool is_empty = false;
		if (P_client->m_oIO.poll_one() == 0) {
			is_empty = true;
		}
		else {
			//printf("*");
		}
		P_client->DealFreeSessionReq();

		if (time(NULL) < nBeginTime) {
			sleep_msec(1);//xx毫秒
			if (run_i % 1000 == 0) {
				printf("%lld \n ", nBeginTime - time(NULL));
			}
			continue;
		}

		if (nStep == 0) {
			nStep = 1;
			for (std::vector<int>::const_iterator it_ = vecSocketID.begin();
				it_ != vecSocketID.end(); ++it_) {
				CGateSession *ptr_ = P_client->GetSession(*it_);
				if (ptr_) {
					ptr_->m_nBeginT = GetCurMs();
					printf("%lld %lld %d\n", ptr_->m_nEndT, ptr_->m_nBeginT, ptr_->m_nSendNum);
					ptr_->Send_Msg(10, std::string("hello"));
				}
				else {
					printf("#");
				}
			}
		}
		else if (nStep == 1) {
			if (Total_All_Send_Msg >= Max_Send_Msg * Session_Num) {
				nStep++;
			}
		}
		else if (nStep < 4000) {
			nStep++;
		}
		else if (nStep == 4000) {
			nStep++;

			int n_total_time = 0;
			for (std::vector<int>::const_iterator it_ = vecSocketID.begin();
				it_ != vecSocketID.end(); ++it_) {
				CGateSession *ptr_ = P_client->GetSession(*it_);
				if (ptr_) {
					ptr_->Send_Msg(8, std::string("exit"));

					int n1 = ptr_->m_nEndT - ptr_->m_nBeginT;
					n_total_time += n1;
					oss_ << ptr_->m_nEndT << " " << ptr_->m_nBeginT << " " << ptr_->m_nSendNum << " " << n1 << "\n";
				}
				else {
					printf("#");
				}
			}
			oss_ << "job:" << float(n_total_time / Session_Num / 1000.0) << "s\n";
			write2File(nAppID, std::to_string(float(n_total_time / Session_Num / 1000.0)));
		}
		else if (nStep < 5000) {
			nStep++;
		}
		else if (nStep == 5000) {
			break;
		}


		if (is_empty) {
			sleep_msec(1);//xx毫秒
		}
	}

	std::cout << oss_.str() << std::endl;

	//zhou//捕获关闭信号

	//处理_关闭网络_请求
	//释放资源
	P_client->Stop();

	delete P_client; P_client = NULL;
	printf("%s thread_id : %d is end \n", __FUNCTION__, GetCurrentThreadId());

}

#endif


#endif

//可变 函数回调
#if 0

#include <functional>

class chbb
{
public:
	void run(int id1) {
		printf("%s ,%d \n", __FUNCTION__ ,id1);
	}

	void run2(int id1, int id2) {
		printf("%s ,%d ,%d \n", __FUNCTION__ , id1 , id2);
	}

	void run3(std::string str_, int id1) {
		printf("%s ,%s ,%d \n", __FUNCTION__, str_.c_str() ,id1);
	}
};


template<class T>
void do_job(T p_) {
	printf("%s \n", __FUNCTION__);
	p_(5);
}

void test_main(int argc, char* argv[]) {
	chbb obj;
	do_job(std::bind(&chbb::run, &obj, std::placeholders::_1));
	do_job(std::bind(&chbb::run2, &obj, 4, std::placeholders::_1) );
	do_job(std::bind(&chbb::run3, &obj, std::string("ok") , std::placeholders::_1));
}

#endif

//异步查询//主线程 处理 查询结构
#if 0
#include "thread.h"
#include <boost/lockfree/spsc_queue.hpp>

class BaseHandle {
public:
	virtual void OnExecute() = 0;
	virtual void OnResult() = 0;
	virtual void OnRelease() = 0;
};

class AsynDeal: public CThreadEx
{
	bool DealOne() {

		BaseHandle * p_one = NULL;
		if (m_vecIn.pop(p_one) ) {
			if (p_one == NULL) {
				return false;
			}

			p_one->OnExecute();
			m_vecOut.push(p_one);
		}
		return false;
	}

	virtual void run() {
		while (m_bRun) {
			if (DealOne() == false) {
				sleep_msec(1);
			}
		}
	}

public:
	AsynDeal() {
		m_bRun = true;
	}

	virtual void stop() {
		m_bRun = false;
	}

	void AddJob(BaseHandle * p_) {
		m_vecIn.push(p_);
	}

	void runFromMain() {
		BaseHandle * p_one = NULL;
		m_vecOut.pop(p_one);
		if (p_one == NULL) {
			return;
		}
		p_one->OnResult();
		p_one->OnRelease();
	}

	boost::lockfree::spsc_queue<BaseHandle *, boost::lockfree::capacity<10 * 10000> > m_vecIn;
	boost::lockfree::spsc_queue<BaseHandle *, boost::lockfree::capacity<10 * 10000> > m_vecOut;
	bool m_bRun;
};

class HbbJob;
class ZhouJob;
basefun::CPool<HbbJob> g_oHbbJobPool;
basefun::CPool<ZhouJob> g_oZhouJobPool;

volatile bool bMainStatus = true;
const int max_id = 10;

class HbbJob : public BaseHandle
{
public:
	virtual void OnExecute() {
		printf("%s thread_id : %d is end \n", __FUNCTION__, GetCurrentThreadId());
	}
	virtual void OnResult() {
		if (m_nID == max_id - 1) {
			bMainStatus = false;
		}
		printf("%s thread_id : %d is end \n", __FUNCTION__, GetCurrentThreadId());
	}
	virtual void OnRelease() {
		printf("%s thread_id : %d is end \n", __FUNCTION__, GetCurrentThreadId());
		g_oHbbJobPool.Release(this);
	}
	int m_nID;
};


class ZhouJob : public BaseHandle
{
public:
	virtual void OnExecute() {
		printf("%s thread_id : %d is end \n", __FUNCTION__, GetCurrentThreadId());
	}
	virtual void OnResult() {
		if (m_nID == max_id - 1) {
			bMainStatus = false;
		}
		printf("%s thread_id : %d is end \n", __FUNCTION__, GetCurrentThreadId());
	}
	virtual void OnRelease() {
		printf("%s thread_id : %d is end \n", __FUNCTION__, GetCurrentThreadId());
		g_oZhouJobPool.Release(this);
	}
	int m_nID;
};



int test_main(int argc, char* argv[]) {
	AsynDeal oDealer;
	oDealer.start();

	for (int i = 0; i < max_id/2; ++i) {
		HbbJob * pJob = g_oHbbJobPool.Create();
		if (pJob == NULL) {
			return 0;
		}
		pJob->m_nID = i;
		oDealer.AddJob(pJob);
	}

	for (int i = 0; i < max_id; ++i) {
		ZhouJob * pJob = g_oZhouJobPool.Create();
		if (pJob == NULL) {
			return 0;
		}
		pJob->m_nID = i;
		oDealer.AddJob(pJob);
	}

	for (; bMainStatus ;) {
		oDealer.runFromMain();
		sleep_msec(1);
	}

	oDealer.stop();

	return 0;
}


#endif

//共享内存
#if 0

#define BOOST_ALL_NO_LIB
#include <boost/interprocess/shared_memory_object.hpp>  
#include <boost/interprocess/mapped_region.hpp>  
#include <cstring>  
#include <cstdlib>  
#include <string>  
#include <signal.h>

using namespace boost::interprocess;

void clearMem(int v_ = 0)
{
	bool ret = shared_memory_object::remove("MySharedMemory");
	printf("%s , %d , %d \n", __FUNCTION__, __LINE__ , ret);
}

int test_main(int argc, char* argv[]) {
	signal(SIGINT, clearMem);
	if (argc <= 1) {
		printf("%s , %d \n", __FUNCTION__, __LINE__);
		return 0;
	}

	int nType = atoi(argv[1]);

	if (nType == 1) {
		clearMem();

		//Create a shared memory object.  
		shared_memory_object shm(create_only, "MySharedMemory", read_write);

		//Set size  
		shm.truncate(1024);

		//Map the whole shared memory in this process  
		mapped_region region(shm, read_write);

		//Write all the memory to 1  
		std::memset(region.get_address(), 0, region.get_size());
		strcpy( (char *)region.get_address(), "hbb ");

		while (1) {
			sleep_msec(2000);
			printf("%s , %d \n", __FUNCTION__, __LINE__);
		}

	}
	else if (nType == 2){
		//Open already created shared memory object.  
		shared_memory_object shm(open_only, "MySharedMemory", read_only);

		//Map the whole shared memory in this process  
		mapped_region region(shm, read_only);

		//Check that memory was initialized to 1  
		char *mem = static_cast<char*>(region.get_address());
		printf("%s" , mem);
		printf("%s , %d \n", __FUNCTION__, __LINE__);

	}
	else {
		printf("%s , %d \n", __FUNCTION__, __LINE__);
	}

	return 0;
}

#endif

//临时代码_敏感词_屏蔽
#if 0

const int g_len = 10000;
std::vector<char > g_vec;
std::vector<std::string > g_vecSensitiveWords;
std::vector<std::string > g_vecRoleName;

std::map<char, std::vector<std::string> > g_map;

std::string RandName() {
	std::string ret;
	const size_t s_ = g_vec.size();
	for (int i = 0; i < 12; ++i) {
		int vec_i = rand() % s_;
		ret.push_back(g_vec[vec_i]);
	}
	return ret;
}

void init() {
	for (char ch = 'a'; ch <= 'z'; ++ch) {
		g_vec.push_back(ch);
	}

	for (char ch = 'A'; ch <= 'Z'; ++ch) {
		g_vec.push_back(ch);
	}

	//随机n个 名字
	for (int i = 0; i < g_len; i++) {
		std::string str_ = RandName();
		g_vecSensitiveWords.push_back(str_);
		g_map[ str_[0] ].push_back(str_);
	}

	for (size_t i_ = 0; i_ < g_vecSensitiveWords.size(); ++i_) {
		const std::string & one_ = g_vecSensitiveWords[i_];
		std::string str_ = RandName() + one_;
		g_vecRoleName.push_back(str_);
	}

}


bool FindName2(const std::string & strName , const std::vector<std::string> & vecAll) {
	
	for (size_t i_ = 0; i_ < vecAll.size(); ++i_) {
		const std::string & one_ = vecAll[i_];
		if (strstr(strName.c_str(), one_.c_str()) != NULL) {
			return true;
		}
	}
	return false;
}

bool FindName1(const std::string & strName) {
	for (int i = 0; i < strName.size(); ++i) {
		const char ch_ = strName[i];
		std::map<char, std::vector<std::string> >::const_iterator it_ = g_map.find(ch_);
		if (it_ == g_map.end()) {
			continue;
		}
		const std::vector<std::string> & vec_ = it_->second;

		if (FindName2(strName, it_->second) == true) {
			return true;
		}
	}
	return false;
}

int test_main(int argc, char* argv[]) {
	init();
	time_t nCurT = GetCurMs();
	for (size_t i_ = 0; i_ < g_vecRoleName.size(); ++i_) {
		const std::string & find_name = g_vecRoleName[i_];
		if (FindName1(find_name) == false) {
			exit(0);
		}
	}
	printf("FindName1 diff time: %d ms\n", int(GetCurMs() - nCurT));

	nCurT = GetCurMs();
	for (size_t i_ = 0; i_ < g_vecRoleName.size(); ++i_) {
		const std::string & find_name = g_vecRoleName[i_];
		if (FindName2(find_name , g_vecSensitiveWords) == false) {
			exit(0);
		}
	}
	printf("FindName2 diff time: %d ms\n", int(GetCurMs() - nCurT));
	return 0;
}

#endif

//pvp_技能被动_demo
#if 0

#include<conio.h>

enum ESkill {
	E_pu_gong = 0,
	E_pu_gong3 ,
	E_fan_ji,
	E_fan_shang,
	E_chi_xu_s_h,
	E_lian_xu_g_j,
	E_Max,
};

#define Mix_Type(x , y) (x) * 1000 + (y)
#define Mix_Key(x , y) (x) * 1000 + (y)
//配置文件_相关数据
std::set<int> gBuffRule;
void configInit() {
	gBuffRule.insert(Mix_Type(E_pu_gong, E_fan_ji));
	gBuffRule.insert(Mix_Type(E_pu_gong, E_fan_shang));
	gBuffRule.insert(Mix_Type(E_pu_gong, E_chi_xu_s_h));
	gBuffRule.insert(Mix_Type(E_pu_gong, E_lian_xu_g_j));

	//gBuffRule.insert(Mix_Type(E_fan_ji, E_fan_ji) );
	gBuffRule.insert(Mix_Type(E_fan_ji , E_fan_shang) );
	//gBuffRule.insert(Mix_Type(E_fan_ji, E_lian_xu_g_j));

	gBuffRule.insert(Mix_Type(E_lian_xu_g_j, E_fan_ji));
	gBuffRule.insert(Mix_Type(E_lian_xu_g_j, E_fan_shang));
	gBuffRule.insert(Mix_Type(E_lian_xu_g_j, E_chi_xu_s_h));
	//gBuffRule.insert(Mix_Type(E_lian_xu_g_j, E_lian_xu_g_j));
}

//普攻//反击//持续伤害2回合//连续攻击//反伤

class CHero;

class CPvpEvent
{
public:
	int m_nStep;
	ESkill m_nType;
	CHero * m_pAttack;
	CHero * m_pDef;
};

typedef std::map<int, std::list<CPvpEvent> > Event_Type;
typedef std::map<int, std::map<int ,CPvpEvent> > Event_Type2;

class CHero 
{
public:
	CHero(std::string str_ ) {
		m_strName = str_;
		m_nHP = 50;
	}
	std::string m_strName;
	int m_nHP;
	int m_nSpeed;
	void Show() {
		printf("\n	%s , hp:%d \n", m_strName.c_str(), m_nHP);
	}

	void AddEvent1(ESkill type, int nRoundID, int nStep , CHero *pDef, Event_Type2 & map_) {
		//触发被动
		int nRand = rand() % 100;
		if (nRand > 70) {
			ESkill nAddType = E_chi_xu_s_h;
			if (gBuffRule.count(Mix_Type(type , nAddType) ) == 0 ) {
				return;
			}
			CPvpEvent one;
			one.m_nStep = nStep;
			one.m_nType = nAddType;
			one.m_pAttack = this;
			one.m_pDef = pDef;
			//add
			map_[Mix_Key(nRoundID + 1 , 0)].insert(std::make_pair(nAddType, one) );
			map_[Mix_Key(nRoundID + 1, 0)].insert(std::make_pair(nAddType, one) );
		}
		else if (nRand > 40) {
			ESkill nAddType = E_lian_xu_g_j;
			if (gBuffRule.count(Mix_Type(type, nAddType)) == 0) {
				return;
			}
			CPvpEvent one;
			one.m_nStep = nStep;
			one.m_nType = nAddType;
			one.m_pAttack = this;
			one.m_pDef = pDef;
			map_[Mix_Key(nRoundID , nStep + 1)].insert(std::make_pair(nAddType, one));
		}
	}

	void AddEvent2(ESkill type, int nRoundID, int nStep, CHero *pAttack, Event_Type2 & map_) {
		//触发被动
		int nRand = rand() % 100;
		if (nRand > 70) {
			ESkill nAddType = E_fan_ji;
			if (gBuffRule.count(Mix_Type(type, nAddType)) == 0) {
				return;
			}

			CPvpEvent one;
			one.m_nStep = nStep;
			one.m_nType = nAddType;
			one.m_pAttack = this;
			one.m_pDef = pAttack;
			//add
			map_[Mix_Key(nRoundID , nStep + 1)].insert(std::make_pair(nAddType, one) );
		}
		else if (nRand > 40) {
			ESkill nAddType = E_fan_shang;
			if (gBuffRule.count(Mix_Type(type, nAddType)) == 0) {
				return;
			}

			CPvpEvent one;
			one.m_nStep = nStep;
			one.m_nType = nAddType;
			one.m_pAttack = this;
			one.m_pDef = pAttack;
			map_[Mix_Key(nRoundID, nStep)].insert(std::make_pair(nAddType, one));
		}
	}

	void Hit(ESkill type, int nRoundID, int nStep , CHero *pDef, Event_Type & map_) {
		int nCount = 1;
		ESkill nCurType = type;
		if (type == E_pu_gong3) {
			nCount = 3;
			nCurType = E_pu_gong;
		}

		Event_Type2 map2;
		for (int i = 0; i < nCount; i++) {
			AddEvent1(nCurType, nRoundID, nStep, pDef, map2);
			pDef->m_nHP -= 1;
			pDef->AddEvent2(nCurType, nRoundID, nStep, this, map2);
		}
		
		for (Event_Type2::const_iterator it_ = map2.begin(); 
			it_ != map2.end(); ++it_ ) {
			//
			const int nKey = it_->first;
			const std::map<int, CPvpEvent> & sub_map = it_->second;
			for (std::map<int, CPvpEvent>::const_iterator it2_ = sub_map.begin() ;
				it2_ != sub_map.end() ; ++it2_) {
				//
				map_[nKey].push_back(it2_->second);
			}
		}

		std::string strType;
		switch (type)
		{
		case E_pu_gong:	strType = "普攻";	break;
		case E_pu_gong3:	strType = "3段击打";	break;
		case E_fan_ji:	strType = "反击";	break;
		case E_fan_shang:	strType = "反弹伤害";	break;
		case E_chi_xu_s_h:	strType = "流血伤害";	break;
		case E_lian_xu_g_j:	strType = "连续攻击";	break;
		}
		printf("	%s => %s => %s , hp:%d \n", m_strName.c_str(), strType.c_str(), pDef->m_strName.c_str(),  pDef->m_nHP);
	}


};


class CBattle {
public:
	CBattle():oA("红方1" )
		,oB("蓝方" )
		, oA2("红方2" )
	{
		m_nRoundID = 0;
	}
	
	bool DealEvent() {
		if (m_mapEvent.empty() == true) {
			return false;
		}

		if (m_mapEvent.begin()->first > Mix_Key(m_nRoundID  , 999) ) {
			return false;
		}

		std::list<CPvpEvent> & list_ = m_mapEvent.begin()->second;
		if (list_.empty() == true) {
			m_mapEvent.erase(m_mapEvent.begin());
			return true;
		}

		const CPvpEvent oEvent = list_.front();
		list_.pop_front();

		oEvent.m_pAttack->Hit(oEvent.m_nType, m_nRoundID, oEvent.m_nStep, oEvent.m_pDef, m_mapEvent);
		return true;
	}

	bool IsEnd() {
		if (oA.m_nHP <= 0 || oA2.m_nHP <= 0 || oB.m_nHP <= 0) {
			return true;
		}
		return false;
	}

	void DealEvent2End() {
		while (DealEvent() == true) {
			if (IsEnd() == true) {
				return;
			}
		}
	}

	bool Next() {
		m_nRoundID++;
		printf("\n回合_开始 {\n");
		DealEvent2End();
		
		int nStep = 0;
		std::vector<std::pair<CHero *, CHero *> > vec_;
		GetCurHero(vec_);
		for (int i = 0; i < vec_.size(); ++i) {
			printf("\n准备_行动[\n");
			DealEvent2End();
			printf("]=============\n\n");

			printf("开始_行动===[\n");
			CHero * pHero1, *pHero2;
			pHero1 = vec_[i].first;
			pHero2 = vec_[i].second;

			pHero1->Hit(E_pu_gong3, m_nRoundID, ++nStep , pHero2, m_mapEvent);
			if (IsEnd() == true) {
				return false;
			}
			printf("]=============\n\n");

			printf("\n结束_行动===[\n");
			DealEvent2End();
			printf("]=============\n\n");
		}

		printf("\n} 回合_结束\n");
		return true;
	}

	void GetCurHero(std::vector<std::pair<CHero *, CHero *> > & vec_) {
		if (m_nRoundID % 2 == 0) {
			vec_.push_back(std::make_pair(&oA, &oB));
			vec_.push_back(std::make_pair(&oA2, &oB));
		}
		else {
			if (m_nRoundID % 4 == 1) {
				vec_.push_back(std::make_pair(&oB, &oA));
			}
			else {
				vec_.push_back(std::make_pair(&oB, &oA2));
			}
		}
	}

	void GetCurHero(CHero ** pAttack, CHero ** pDef) {
		if (m_nRoundID % 2 == 0) {
			*pAttack = &oA;
			*pDef = &oB;
		}
		else {
			*pDef = &oA;
			*pAttack = &oB;
		}
	}
	Event_Type m_mapEvent;
	int m_nRoundID;
	CHero oA;
	CHero oA2;
	CHero oB;
};



void test_main(int argc, char* argv[])
{
	configInit();
	CBattle oBattle;
	while (1) {
		if ( getche()) {
			system("cls");

			if (oBattle.Next() == false) {
				break;
			}
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		}
	}
	system("cls");
	oBattle.oA.Show();
	oBattle.oA2.Show();
	oBattle.oB.Show();
}

#endif

//颜色_输出
#if 0

void PrintColorText(std::string str_, int nColor = 12)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE); //获取缓冲区句柄
	SetConsoleTextAttribute(hCon, nColor); //设置文本及背景色
	std::cout << str_ << std::endl;
	SetConsoleTextAttribute(hCon, 7); //设置文本及背景色
}
#endif

//哈希_效率优化
#if 1	
//zhou//优化内存

class CHero {
public:
	static void FreeOne(CHero * ptr_) {
		delete ptr_;
	}

	static CHero * CreateOne() {
		return new CHero();
	}

	int GetKey()const { return m_nHeroID; }
	int m_nHeroID;
	int m_nExp;
};

template< class T>
class CHashVec
{
public:
	std::vector<T *> m_vecPtr;
	std::list<int> m_vecIndex;	//优先使用的索引
	size_t m_nLen;
	CHashVec(int size_ = 10):m_nLen(0)
	{
		m_vecPtr.reserve(size_);
	}

	long long GetHashID(int nKey , int index_) {
		long long ret = 0;
		int * pBuff = (int *)&ret;
		pBuff[0] = index_;
		pBuff[1] = nKey;
		return ret;
	}

	size_t Size() const { return m_nLen; }

	long long Add(T * o_) {
		if (o_ == NULL) {
			return -1;
		}

		if (m_vecIndex.empty()) {
			m_vecPtr.push_back(o_);
			return GetHashID(o_->GetKey() , m_nLen++);
		}
		else {
			size_t nPos = m_vecIndex.back();
			m_vecIndex.pop_back();
			if (nPos < m_vecPtr.size()) {
				m_vecPtr[nPos] = o_;
				m_nLen++;

				return GetHashID(o_->GetKey(), nPos);
			}
		}
		return -1;
	}

	bool Del(long long nHashID) {
		int * pBuff = (int *)&nHashID;
		const int index_ = pBuff[0];
		const int nKey = pBuff[1];
		if (index_ < m_vecPtr.size()) {
			if (m_vecPtr[index_] != NULL && m_vecPtr[index_]->GetKey() == nKey) {
				T::FreeOne(m_vecPtr[index_]);
				m_vecPtr[index_] = NULL;
				--m_nLen;
				m_vecIndex.push_back(index_);
				return true;
			}
		}
		return false;
	}


	bool Del(int nKey) {
		//暂无
	}

	T * Find(long long nHashID) {
		int * pBuff = (int *)&nHashID;
		const int index_ = pBuff[0];
		const int nKey = pBuff[1];
		if (index_ < m_vecPtr.size() ) {
			if (m_vecPtr[index_] != NULL && m_vecPtr[index_]->GetKey() == nKey) {
				return m_vecPtr[index_];
			}
		}
		return NULL;
	}

};


#include <unordered_map>

const static int max_ = 1000000;

void test_main(int argc, char* argv[])
{

#if 1
	{
		printf("##test map \n");
		std::map<int, CHero *> map_;
		std::vector<int> vec_key;
		time_t nCurT = GetCurMs();
		for (int i = 0; i < max_; ++i) {
			CHero * ptr_ = CHero::CreateOne();
			if (ptr_ == NULL) {
				printf("err");
			}

			int nKey = max_ + i;
			ptr_->m_nExp = i;
			ptr_->m_nHeroID = nKey;

			map_[nKey] = ptr_;
			vec_key.push_back(nKey);
		}
		printf("map##diff time: %d ms\n", int(GetCurMs() - nCurT));
		for (size_t i_ = 0; i_ < vec_key.size(); ++i_) {
			if (map_.find(vec_key[i_]) == map_.end()) { printf("err"); }
		}
		printf("map##diff time: %d ms\n", int(GetCurMs() - nCurT));
		for (size_t i_ = 0; i_ < vec_key.size(); ++i_) {
			map_.erase(vec_key[i_]);
		}
		printf("map##diff time: %d ms\n", int(GetCurMs() - nCurT));
	}
#endif


#if 1
	{
		printf("##test unordered_map \n");
		std::unordered_map<int, CHero * > hash_map_;
		std::vector<int> vec_key;

		time_t nCurT = GetCurMs();
		for (int i = 0; i < max_; ++i) {
			CHero * ptr_ = CHero::CreateOne();
			if (ptr_ == NULL) {
				printf("err");
			}

			int nKey = max_ + i;
			ptr_->m_nExp = i;
			ptr_->m_nHeroID = nKey;
			//
			hash_map_[nKey] = ptr_;
			vec_key.push_back(nKey);
		}
		printf("unordered_map##diff time: %d ms\n", int(GetCurMs() - nCurT));
		for (size_t i_ = 0; i_ < vec_key.size(); ++i_) {
			if (hash_map_.find(vec_key[i_]) == hash_map_.end()) { printf("err"); }
		}
		printf("unordered_map##diff time: %d ms\n", int(GetCurMs() - nCurT));

		for (size_t i_ = 0; i_ < vec_key.size(); ++i_) {
			hash_map_.erase(vec_key[i_]);
		}
		printf("unordered_map##diff time: %d ms\n", int(GetCurMs() - nCurT));
	}
#endif

#if 1
	{
		printf("##test CHashVec \n");
		CHashVec<CHero> oCHashVec( max_);
		std::vector<long long> vec_ID;
		time_t nCurT = GetCurMs();
		for (int i = 0; i < max_; ++i) {
			CHero * ptr_ = CHero::CreateOne();
			if (ptr_ == NULL) {
				printf("err");
			}

			int nKey = max_ + i;
			ptr_->m_nExp = i;
			ptr_->m_nHeroID = nKey;
			//
			long long nID = oCHashVec.Add(ptr_);
			vec_ID.push_back(nID);
		}
		printf("CHashVec##diff time: %d ms\n", int(GetCurMs() - nCurT));
		for (size_t i_ = 0; i_ < vec_ID.size(); ++i_) {
			if (oCHashVec.Find(vec_ID[i_]) == NULL) { printf("err"); }
		}
		printf("CHashVec##diff time: %d ms\n", int(GetCurMs() - nCurT));
		for (size_t i_ = 0; i_ < vec_ID.size(); ++i_) {
			if (oCHashVec.Del(vec_ID[i_]) == false) { printf("err"); }
		}
		printf("CHashVec##diff time: %d ms\n", int(GetCurMs() - nCurT));

		vec_ID.clear();
		for (int i = 0; i < max_; ++i) {
			CHero * ptr_ = CHero::CreateOne();
			if (ptr_ == NULL) {
				printf("err");
			}

			int nKey = max_ + i;
			ptr_->m_nExp = i;
			ptr_->m_nHeroID = nKey;
			//
			long long nID = oCHashVec.Add(ptr_);
			vec_ID.push_back(nID);
		}
		printf("CHashVec##diff time: %d ms\n", int(GetCurMs() - nCurT));

	}
#endif

	
}

#endif

//字符串_效率优化
#if 0	
void test_main(int argc, char* argv[])
{
	for (int i = 0; i < 1000000; ++i) {
#if 1
		char str_[128] = "";
		strcpy(str_, "hbb_zhou_zhou_zhou_hbbhbb_zhou_zhou_zhou_hbbhbb_zhou_zhou_zhou_hbb");
		int len = strlen(str_);
		strcat(str_ + len, "111");
		len += strlen("111");
		strcat(str_ + len, "111");
		len += strlen("111");
		strcat(str_ + len, "111");
#else
		std::string str_("hbb_zhou_zhou_zhou_hbbhbb_zhou_zhou_zhou_hbbhbb_zhou_zhou_zhou_hbb");
		str_ += "111";
		str_ += "111";
		str_ += "111";
#endif
	}

}

#endif



int _tmain(int argc, char* argv[])
{
	time_t nCurT = GetCurMs();
	test_main(argc, argv);
	printf("all diff time: %d ms\n", int(GetCurMs() - nCurT) );
	//system("pause");
	return 0;
}
















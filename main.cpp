#pragma warning(disable:4996)
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <ctime>

using namespace std;

enum Type { STRING, INT, FLOAT, DOUBLE, CHAR, BOOL }; // 属性的类型
string typeStr[] = {"string", "int", "float", "double", "char", "bool" };
enum RecordWay { cluster, noneCluster }; // 关系的存储方法
string recordWayStr[] = { "cluster", "noneCluster" };
enum RecordTis { sequence, hash, heap }; // 关系的存储组织
string recordTisStr[] = { "sequence", "hash", "heap" };


class Attribute {
public:
	string attriName; // 属性的名，唯一标识符
	Type attriType; // 属性的类型
	int attriSize; // 属性的长度
	bool attriIsStable; // 属性是否定长
	bool attriIsEmpty; // 属性是否可以为空
	// 默认值
	Attribute(){}
	Attribute(string attriName, Type attriType, int attriSize, bool attriIsStable, bool attriIsEmpty) {
		this->attriName = attriName;
		this->attriType = attriType;
		this->attriSize = attriSize;
		this->attriIsStable = attriIsStable;
		this->attriIsEmpty = attriIsEmpty;
	}

	bool operator < (Attribute& otherAttribute) {
		return this->attriName < otherAttribute.attriName;
	}

};

class Metadata {
public:
	string databaseName; // 关系所属数据库名
	long ID; //  关系的编号
	string relaName; // 关系的名字
	string primerayKeyName; // 主键的名字
	map<string, long> foreignKeyName; // <外键的名字，相关关系的id>
	long int recordNum; // 记录的条数
	RecordWay relaStore; // 关系的存储方法
	RecordTis relaTissue; // 关系的存储组织
	string relaLocation; // 关系的存储位置
	// 关系的索引
	map<string, Attribute> attriMap; // 关系中的所有属性
	string createTime; // 关系的创建时间
	string updateTime; // 关系的最近修改时间
	// 关系的授权用户信息

	Metadata() {
		this->foreignKeyName = map<string, long>();
		this->attriMap = map<string, Attribute>();
	}
	~Metadata() {}
	Metadata(string databaseName, long ID, string relaName, string primerayKeyName, map<string, long>& foreignKeyName,
		long int recordNum, RecordWay relaStore, RecordTis relaTissue, string relaLocation, map<string, Attribute>& attriMap,
		string createTime, string updateTime) {
		this->databaseName = databaseName;
		this->ID = ID;
		this->relaName = relaName;
		this->primerayKeyName = primerayKeyName;
		this->foreignKeyName.clear();
		for (auto i = foreignKeyName.begin(); i != foreignKeyName.end(); ++i) {
			this->foreignKeyName.insert(*i);
		}
		this->recordNum = 0;
		this->relaStore = relaStore;
		this->relaTissue = relaTissue;
		this->relaLocation = relaLocation;
		this->attriMap.clear();
		for (auto i = attriMap.begin(); i != attriMap.end(); ++i) {
			this->attriMap.insert(*i);
		}
		this->createTime = createTime;
		this->updateTime = updateTime;
	}

	// 插入数据时，传入插入数据条数和时间
	void insertData(int insertNum, string insertTime) {
		this->recordNum += insertNum;
		this->updateTime = insertTime;
	}

	// 删除数据时，传入删除数据条数和时间
	void deleteData(int deleteNum, string deleteTime) {
		this->recordNum -= deleteNum;
		this->updateTime = deleteTime;
	}

	// 修改数据时，传入修改时间
	void updateData(string updateTime) {
		this->updateTime = updateTime;
	}

	// 删去关系表时
	void dropRelation() {
		this->~Metadata();
	}

	Attribute& searchAttribute(string attriName) {
		if (attriMap.find(attriName) != attriMap.end()) {
		return attriMap[attriName];
		}
		else cout << "不存在该属性!" << endl;
	}

	// 增加属性时
	void insertAttribute(Attribute newAttribute, string updateTime) {
		if (this->attriMap.find(newAttribute.attriName) == this->attriMap.end()) {
			cout << "添加新属性：" << newAttribute.attriName << endl;
			this->updateTime = updateTime;
			this->attriMap.insert({ newAttribute.attriName, newAttribute });
		}
		else cout << "已经存在该属性" << endl;
	}

	// 修改属性时
	void updateAttribute(Attribute attributeAfterModify, string updateTime) {
		if (attriMap.find(attributeAfterModify.attriName) != attriMap.end()) {
		attriMap.insert({ attributeAfterModify.attriName, attributeAfterModify });
		this->updateTime = updateTime;
		}
		else cout << "不存在该属性，无法修改" << endl;
	}

	// 删除属性时
	void deleteAttribute(string attriName, string updateTime) {
		if (attriMap.find(attriName) != attriMap.end()) {
			cout << "删除属性" << attriName << endl;
			attriMap.erase(attriName);
		}
		else cout << "不存在该属性，无需删除" << endl;
	}

	friend ostream& operator << (ostream& os, Metadata& metadata) {
		cout << endl << "---------------------------------------------------" << endl;
		cout << "所属数据库：" << metadata.databaseName << endl;
		cout << "关系表ID：" << metadata.ID << endl;
		cout << "关系表名：" << metadata.relaName << endl;
		cout << "主键名：" << metadata.primerayKeyName << endl;
		cout << "记录条数：" << metadata.recordNum << endl;
		cout << "关系存储方式：" << recordWayStr[metadata.relaStore] << endl;
		cout << "关系存储组织：" << recordTisStr[metadata.relaTissue] << endl;
		cout << "关系存储位置：" << metadata.relaLocation << endl;
		cout << "关系创建时间：" << metadata.createTime;
		cout << "关系最近修改时间：" << metadata.updateTime;
		cout << "\n关系中的属性：" << endl;

		cout << "属性名\t" << "类型\t" << "长度\t" << "定长\t" << "为空\t" << endl;
		auto primaryKey = metadata.attriMap[metadata.primerayKeyName];
		cout << primaryKey.attriName << "\t" << typeStr[primaryKey.attriType] << "\t" << primaryKey.attriSize << "\t"
			<< primaryKey.attriIsStable << "\t" << primaryKey.attriIsEmpty << endl;
		for (auto i = metadata.attriMap.begin(); i != metadata.attriMap.end(); ++i) {
			if((*i).second.attriName != primaryKey.attriName)
				cout << (*i).first << "\t" << (*i).second.attriType << "\t" << (*i).second.attriSize << "\t" 
				<< (*i).second.attriIsStable << "\t" << (*i).second.attriIsEmpty << endl;
		}

		cout << endl << "---------------------------------------------------" << endl;
		return os;
	}
};

map<long, Metadata> allMetaData;

void insertAttribute(long ID, Attribute newAttribute, string updateTime) {
	allMetaData[ID].insertAttribute(newAttribute, updateTime);
}

void deleteAttribute(long ID, string attriName, string updateTime) {
	allMetaData[ID].deleteAttribute(attriName, updateTime);
}

int main() {
	Attribute a1("name", Type::STRING, 16, false, true);
	Attribute a2("age", Type::INT, 16, false, true);
	Attribute a3("gender", Type::CHAR, 8, true, true);
	Attribute a4("salary", Type::FLOAT, 32, false, true);
	map<string, Attribute> attriMap1, attriMap2;
	attriMap1.insert({ a1.attriName, a1 }); attriMap1.insert({ a2.attriName, a2 }); attriMap1.insert({ a3.attriName, a3 });
	attriMap2.insert({ a1.attriName, a1 }); attriMap2.insert({ a2.attriName, a2 }); attriMap2.insert({ a4.attriName, a4 });


	map<string, long> foreignMap1, foreignMap2;
	time_t now = time(0);
	string dt = ctime(&now);
	//string databaseName, long ID, string relaName, string primerayKeyName, map<string, long>& foreignKeyName, long int recordNum, RecordWay relaStore, RecordTis relaTissue, string relaLocation, map<string, Attribute>& attriMap, tm createTime, tm updateTime
	Metadata r1("myDataBase", 0001, "gnederStatistic", "name", foreignMap1, attriMap1.size(), RecordWay::cluster, RecordTis::hash, "localhost", attriMap1, dt, dt);
	Metadata r2("myDataBase", 0002, "salaryStatistic", "name", foreignMap2, attriMap2.size(), RecordWay::noneCluster, RecordTis::heap, "localhost", attriMap2, dt, dt);
	allMetaData.insert({ r1.ID, r1 }); allMetaData.insert({ r2.ID, r2 });

	cout << "打印数据库中所有元数据:" << endl;
	for (auto i = allMetaData.begin(); i != allMetaData.end(); ++i)
		cout << (*i).second << endl;

	_sleep(1 * 1000);

	insertAttribute(r1.ID, a4, dt);
	//r1.deleteAttribute(a2.attriName, dt);
	
	cout << endl << "往gnederStatistic中增加salary属性后：" << endl;
	cout << allMetaData[r1.ID] << endl;;


	deleteAttribute(r1.ID, a2.attriName, dt);
	cout << endl << "往gnederStatistic中删除age属性后：" << endl;
	cout << allMetaData[r1.ID] << endl;;
}



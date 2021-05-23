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
	string relaName; // 属性所属关系的名字
	string attriName; // 属性的名，唯一标识符
	Type attriType; // 属性的类型
	int attriSize; // 属性的长度
	bool attriIsStable; // 属性是否定长
	bool attriIsEmpty; // 属性是否可以为空
	// 默认值
	Attribute(){}
	Attribute(string relaName, string attriName, Type attriType, int attriSize, bool attriIsStable, bool attriIsEmpty) {
		this->relaName = relaName;
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

map<string, map<string, Attribute>> attriMap; // <关系名，<属性名，属性>>

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
	//map<string, Attribute> attriMap; // 关系中的所有属性
	string createTime; // 关系的创建时间
	string updateTime; // 关系的最近修改时间


	Metadata() {
		this->foreignKeyName = map<string, long>();
	}
	~Metadata() {}
	Metadata(string databaseName, long ID, string relaName, string primerayKeyName, map<string, long>& foreignKeyName,
		RecordWay relaStore, RecordTis relaTissue, string relaLocation, string createTime, string updateTime) {
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
		map<string, Attribute>& tempMap = attriMap[this->relaName];
		if (tempMap.find(attriName) != tempMap.end()) {
			return tempMap[attriName];
		}
		else cout << "不存在该属性!" << endl;
	}

	// 增加属性时
	void insertAttribute(Attribute newAttribute, string updateTime) {
		if (newAttribute.relaName != this->relaName) {
			cout << "该属性不在此关系中";
			return;
		}
		if (attriMap.find(this->relaName) == attriMap.end()) attriMap[this->relaName] = map<string, Attribute>();
		map<string, Attribute>& tempMap = attriMap[this->relaName];

		if (tempMap.find(newAttribute.attriName) == tempMap.end()) {
			cout << "关系表" << this->relaName << "添加新属性：" << newAttribute.attriName << endl;
			this->updateTime = updateTime;
			tempMap.insert({ newAttribute.attriName, newAttribute });
		}
		else cout << "已经存在该属性" << endl;
	}

	// 修改属性时
	void updateAttribute(Attribute attributeAfterModify, string updateTime) {
		map<string, Attribute>& tempMap = attriMap[this->relaName];
		if (tempMap.find(attributeAfterModify.attriName) != tempMap.end()) {
			tempMap.insert({ attributeAfterModify.attriName, attributeAfterModify });
		this->updateTime = updateTime;
		}
		else cout << "不存在该属性，无法修改" << endl;
	}

	// 删除属性时
	void deleteAttribute(string attriName, string updateTime) {
		map<string, Attribute>& tempMap = attriMap[this->relaName];
		if (tempMap.find(attriName) != tempMap.end()) {
			cout << "删除属性" << attriName << endl;
			tempMap.erase(attriName);
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

		map<string, Attribute>& tempMap = attriMap[metadata.relaName];

		cout << "属性名\t" << "类型\t" << "长度\t" << "定长\t" << "为空\t" << endl;
		auto primaryKey = tempMap[metadata.primerayKeyName];
		cout << primaryKey.attriName << "\t" << typeStr[primaryKey.attriType] << "\t" << primaryKey.attriSize << "\t"
			<< primaryKey.attriIsStable << "\t" << primaryKey.attriIsEmpty << endl;
		for (auto i = tempMap.begin(); i != tempMap.end(); ++i) {
			if((*i).second.attriName != primaryKey.attriName)
				cout << (*i).first << "\t" << (*i).second.attriType << "\t" << (*i).second.attriSize << "\t" 
				<< (*i).second.attriIsStable << "\t" << (*i).second.attriIsEmpty << endl;
		}

		cout << endl << "---------------------------------------------------" << endl;
		return os;
	}
};

map<long, Metadata> allMetaData; 

int main() {
	Attribute a1("gnederStatistic","name", Type::STRING, 16, false, false);
	Attribute a2("gnederStatistic","age", Type::INT, 16, false, true);
	Attribute a3("salaryStatistic", "ID", Type::INT, 16, true, false);
	Attribute a4("salaryStatistic","gender", Type::CHAR, 8, true, true);
	Attribute a5("salaryStatistic","salary", Type::FLOAT, 32, false, true);

	map<string, long> foreignMap1, foreignMap2;
	time_t now = time(0);
	string dt = ctime(&now);
	//string databaseName, long ID, string relaName, string primerayKeyName, map<string, long>& foreignKeyName,long int recordNum, RecordWay relaStore, RecordTis relaTissue, string relaLocation, string createTime, string updateTime
	Metadata r1("myDataBase", 0001, "gnederStatistic", "name", foreignMap1, RecordWay::cluster, RecordTis::hash, "localhost", dt, dt);
	Metadata r2("myDataBase", 0002, "salaryStatistic", "ID", foreignMap2, RecordWay::noneCluster, RecordTis::heap, "localhost", dt, dt);
	allMetaData.insert({ r1.ID, r1 }); allMetaData.insert({ r2.ID, r2 });

	// 插入数据
	allMetaData[0001].insertAttribute(a1, dt);
	allMetaData[0001].insertAttribute(a2, dt);

	allMetaData[0002].insertAttribute(a3, dt);
	allMetaData[0002].insertAttribute(a4, dt);

	cout << "打印数据库中所有元数据:" << endl;
	for (auto i = allMetaData.begin(); i != allMetaData.end(); ++i)
		cout << (*i).second << endl;

	_sleep(1 * 1000);

	allMetaData[0002].insertAttribute(a5, dt);
	//r1.deleteAttribute(a2.attriName, dt);
	
	cout << endl << "往salaryStatistic中增加salary属性后：" << endl;
	cout << allMetaData[0002] << endl;;


	allMetaData[0002].deleteAttribute(a4.attriName, dt);
	cout << endl << "往salaryStatistic中删除gender属性后：" << endl;
	cout << allMetaData[0002] << endl;;
}



#include "DataXML.h"
#include "DataSQL.h"

int main() {
	const std::string oldXML = "Workshop.xml"; //готовый xml
	const std::string newXML = "Workshop_v2.xml";
	
	DataXML dataxml1;
	dataxml1.read(oldXML);
	dataxml1.write(newXML); //из "Workshop.xml" считывааются данные и заносятся новый файл "Workshop_v2.xml"
	Registries reg1 = dataxml1.registries(); //считанные данные хранятся в реестрах структуры reg1

	const std::string oldDB = "Workshop.db";
	const std::string newDB = "Workshop_v2.db";

	DataSQL datasql1;
	datasql1.setRegistries(reg1); //данные из reg1 переносятся в реестры объекта datasql для работы с БД
	datasql1.write(oldDB); //БД "Workshop.db" заполняется
	reg1 = datasql1.registries(); //здесь происходит перенос старых данных по ссылке (при корректной работе программы значение
								 //reg1 совпадает с значением reg1 после выполнения команды в 11 строке)

	DataSQL datasql2;
	datasql2.read(oldDB); //БД "Workshop.db" считывается,
	Registries reg2 = datasql2.registries(); //ее данные заносятся в reg2,
	datasql2.write(newDB); //и на их основе создается новая БД "Workshop_v2.db"

	const std::string check = "Check.xml";
	DataXML dataxml2;
	dataxml2.setRegistries(reg2);
	dataxml2.write(check); //данными reg2 заполняется проверочный файл "Check.xml"
};
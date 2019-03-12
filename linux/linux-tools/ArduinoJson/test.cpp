#include "ArduinoJson.h"
#include <iostream>
/*
	Document url : https://arduinojson.org/v6/api/
*/
int Deserialization(){
	char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

	DynamicJsonDocument doc(1024);
	deserializeJson(doc, json);

	JsonObject root = doc.as<JsonObject>();
	const char* sensor = root["sensor"];
	long time = root["time"];
	double latitude = root["data"][0];
	double longitude = root["data"][1];
	printf("latitude:%f longitude:%f \n", latitude, longitude);
	return 0;
}
void Serialization(){
	DynamicJsonDocument doc(1024);

	JsonObject root = doc.to<JsonObject>();
	root["sensor"] = "gps";
	root["time"] = 1351824120;

	JsonArray data = root.createNestedArray("data");
	data.add(48.756080);
	data.add(2.302038);

	std::string finalJson;
	serializeJson(doc, finalJson);
	std::cout << finalJson << std::endl;
	// This prints:
	// {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
}
//JsonArray
//JsonObject
//JsonVariant

//JsonBuffer
//RawJson()
void testJsonArray(){

	std::string Serial;

	//JsonArray::add()
	{
		Serial.clear();
		// compute the required size
		const size_t CAPACITY = JSON_ARRAY_SIZE(6);

		// allocate the memory for the document
		StaticJsonDocument<CAPACITY> doc;

		// create an empty array
		JsonArray array = doc.to<JsonArray>(); //清空 JsonDocument并将其转换为指定的类型。

		// add some values
		array.add("hello");
		array.add(42);
		array.add(3.14);
		JsonArray child001 = array.createNestedArray();
		child001.add("this is a child node");
		child001.add(10101);


		// serialize the array and send the result to Serial
		serializeJson(doc, Serial);
		std::cout << Serial << std::endl;
	}



	//JsonArray::begin() / JsonArray::end()
	{
		Serial.clear();
		char json[] = "[\"one\",\"two\",\"three\"]";
		DynamicJsonDocument doc(1024);
		deserializeJson(doc, json);
		JsonArray arr = doc.as<JsonArray>();

		// using C++11 syntax (preferred):
		for (JsonVariant value : arr) {
			std::cout << (value.as<char*>()) << " ";
		}
		std::cout << std::endl;
		// using C++98 syntax (for older compilers):
		for (JsonArray::iterator it = arr.begin(); it != arr.end(); ++it) {
			std::cout << (it->as<char*>()) << " ";
		}
		std::cout << std::endl;
	}


	//JsonArray::createNestedArray()
	{
		Serial.clear();
		StaticJsonDocument<200> doc;
		JsonArray array = doc.to<JsonArray>();
		array.add("hello");
		JsonArray nested = array.createNestedArray();
		nested.add("world");
		serializeJson(array, Serial);
		std::cout << Serial << std::endl;

	}

	//JsonArray::createNestedObject()
	{
		Serial.clear();
		StaticJsonDocument<200> doc;
		JsonArray array = doc.to<JsonArray>();
		JsonObject nested = array.createNestedObject();
		nested["hello"] = "world";
		serializeJson(array, Serial);
		std::cout << Serial << std::endl;
	}

	//JsonArray::getElement()
	{
		Serial.clear();
		StaticJsonDocument<200> doc;
		JsonArray array = doc.to<JsonArray>();
		array.add("hello0");
		array.add("hello1");
		array.add("hello2");
		array.add("hello3");

		serializeJson(array, Serial);
		std::cout << "array.getElement(0);" << array.getElement(0) << std::endl;
		std::cout << "array.getElement(1);" << array.getElement(1) << std::endl;
		std::cout << "array.getElement(2);" << array.getElement(2) << std::endl;
		std::cout << "array.getElement(3);" << array.getElement(3) << std::endl;
		std::cout << "array.getElement(4);" << array.getElement(4) << std::endl;

		std::cout << Serial << std::endl;
	}

	//JsonArray::isNull()
	{
		//parsing success:
		StaticJsonDocument<200> doc;
		deserializeJson(doc, "[1,2]");
		JsonArray array = doc.as<JsonArray>();
		std::cout << "array.isNull():" << (array.isNull()) << std::endl; // false

		// parsing failure:
		deserializeJson(doc, "{1,2}");
		array = doc.as<JsonArray>();
		std::cout << "array.isNull():" << (array.isNull()) << std::endl; // false
	}
	//JsonArray::operator[]
	{
		Serial.clear();
		StaticJsonDocument<200> doc;
		JsonArray array = doc.to<JsonArray>();
		array.add(42);
		int ivalue = array[0];
		array[0] = 666;


		serializeJson(array, Serial);
		std::cout << Serial << std::endl;
		Serial.clear();

		JsonVariant value = array[0];
		JsonVariant value2 = array.getElement(0);

		array[0] = "value";
		array.getElement(0).set("value");

		serializeJson(array, Serial);
		std::cout << Serial << std::endl;
	}
	//JsonArray::remove()
	{
		/*
			导致内存泄漏
			因为JsonDocument包含单调分配器，所以此函数无法释放与已删除元素关联的内存。
			因此，您无法在循环中删除和添加元素，否则JsonDocument将溢出。
		*/
		Serial.clear();
		StaticJsonDocument<200> doc;
		JsonArray array = doc.to<JsonArray>();
		array.add("A");
		array.add("B");
		array.add("C");
		array.remove(0);
		serializeJson(array, Serial);

		std::cout << Serial << std::endl;

	}

	//JsonArray::set()
	{
		Serial.clear();
		StaticJsonDocument<200> doc;
		JsonArray array = doc.to<JsonArray>();
		StaticJsonDocument<200> doc2;
		JsonArray arr2 = doc2.to<JsonArray>();

		arr2.add("A2");
		arr2.add("B2");
		arr2.add("C2");

		array.add("A");
		array.add("B");
		array.add("C");
		array.remove(0);
	
		array.set(arr2);

		serializeJson(array, Serial);
		std::cout << Serial << std::endl;
	}

	//JsonArray::size()
	{
		Serial.clear();
		StaticJsonDocument<200> doc;
		JsonArray array = doc.to<JsonArray>();
		array.add("hello");
		array.add("world");

		serializeJson(array, Serial);
		std::cout << Serial << " serial size: " <<array.size() << std::endl;


	}
}

int main(){
	Deserialization();
	Serialization();

	testJsonArray();

	return 0;
}




//jsoncpp
int testJson()
{
    //std::string strValue = "{\"name\":\"json\",\"array\":[{\"cpp\":\"jsoncpp\"},{\"java\":\"jsoninjava\"},{\"php\":\"support\"}]}";    
    
    // 构建json数组
    Json::Value array;
    Json::Value root;
    Json::Value person;
    
    Json::FastWriter writer;
 
 
    person["name"] = "allen";
    person["age"] = 10; 
    person["sex"] = "male";
    root.append(person);
 
    person["name"] = "keiv";
    person["age"] = 20; 
    person["sex"] = "female";
    root.append(person);
    
    person["name"] = "lihua";
    person["age"] = 10; 
    person["sex"] = "female";
    root.append(person);
 
    // 添加数组格式
    //array["array"].append(root);
    
    // 子节点挂到根节点上
    array["array"] = Json::Value(root);
 
    string data = writer.write(array);
 
    cout<<data<<endl;  
    //cout<<array.toStyledString()<<endl;
    
    
// 解析Json字符串
    string strValue = array.toStyledString();      // json对象转变为json字符串
    cout<<strValue<<endl;
 
    Json::Reader reader;
    Json::Value value;
 
    if (reader.parse(strValue, value))            // json字符串转为json对象
    {   
        for (unsigned int i = 0; i < value["array"].size(); i++)
        {   
            string name = value["array"][i]["name"].asString();
            int     age = value["array"][i]["age"].asInt();
            string sex  = value["array"][i]["sex"].asString();
 
            cout<<name<<" "<<age<<" "<<sex<<endl;
        }
    }
 
    return 0;
}


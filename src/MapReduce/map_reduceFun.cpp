/*
 * @Author: monifeng 1098264843@qq.com
 * @Date: 2023-05-01 20:00:05
 * @LastEditors: monifeng 1098264843@qq.com
 * @LastEditTime: 2023-05-01 21:16:55
 * @FilePath: /6.824/src/MapReduce/map_reduceFun.cpp
 * @Description: 
 * 
 * Copyright (c) 2023 by monifeng, All Rights Reserved. 
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <bits/stdc++.h>
using namespace std;

class KeyValue{
public:
    string key;
    string value;
};

/**
 * @brief 自己写的字符串按照单词分割函数，因为mapReduce中经常用到
 * 
 * @param text 传入的文本内容，数据量极大
 * @param length 传入的字符串长度
 * @return vector<string> 返回各个分割后的单词列表
 */
vector<string> split(char* text, int length){
    vector<string> str;
    string tmp = "";
    for(int i = 0; i < length; i++){
        if((text[i] >= 'A' && text[i] <= 'Z') || (text[i] >= 'a' && text[i] <= 'z')){
            tmp += text[i];
        }else{
            if(tmp.size() != 0) str.push_back(tmp);      
            tmp = "";
        }
    }
    if(tmp.size() != 0) str.push_back(tmp);
    return str;
}

/**
 * @brief mapFunc，需要打包成动态库，并在worker中通过dlopen以及dlsym运行时加载
 * @param kv 将文本按单词划分并以出现次数代表value长度存入keyValue
 * @return 类似{{"my", "1"}, {"my", "1"}, {"you", "1"}} 即文章中my出现了2次，you出现了1次
 */
extern "C" vector<KeyValue> mapF(KeyValue kv){
    vector<KeyValue> kvs;
    int len = kv.value.size();
    char content[len + 1];
    strcpy(content, kv.value.c_str());
    vector<string> str = split(content, len);
    for(const auto& s : str){
        KeyValue tmp;
        tmp.key = s;
        tmp.value = "1";
        kvs.emplace_back(tmp);
    }
    return kvs;
}

/**
 * @brief reduceFunc，也是动态加载，输出对特定keyValue的reduce结果
 * @param reduceTaskIdx
 * @return vector<string>, {"my", "1111"}, {"you", "11"}, my出现4次，you有2次
 */
extern "C" vector<string> reduceF(vector<KeyValue> kvs, int reduceTaskIdx){
    vector<string> str;
    for(const auto& kv : kvs){
        str.push_back(to_string(kv.value.size()));
    }
    return str;
}


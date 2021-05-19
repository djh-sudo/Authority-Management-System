#include"Object.h"
#include<string>
#include<iostream>
using namespace std;
Object::Object(int) {
    Delegate_Read_Pool.clear();//
    Delegate_Insert_Pool.clear();
    Delegate_Update_Pool.clear();
    Delegate_Delete_Pool.clear();
}

Object::Object(string filename, int id, bool read, bool update, bool insert , bool _delete , bool Delegate_delete , bool Delegate_Insert, bool Delegate_Read , bool Delegate_Update, int Source_Read, int Source_Update, int Source_Insert, int Source_Delete) {
    this->Read = read;
    this->Update = update;
    this->Insert = insert;
    this->Delete = _delete;
    this->FileName = filename;
    this->Delegate_delete = Delegate_delete;
    this->Delegate_Insert = Delegate_Insert;
    this->Delegate_Read = Delegate_Read;
    this->Delegate_Update = Delegate_Update;
    this->id = id;
    this->Source_Read = Source_Read;
    this->Source_Update = Source_Update;
    this->Source_Insert = Source_Insert;
    this->Source_Delete = Source_Delete;
    Delegate_Read_Pool.clear();
    Delegate_Insert_Pool.clear();
    Delegate_Update_Pool.clear();
    Delegate_Delete_Pool.clear();
}

void Object::Modify_Read(bool read) {
    this->Read = read;
}
void Object::Modify_Insert(bool insert) {
    this->Insert = insert;
}
void Object::Modify_Update(bool update) {
    this->Update = update;
}
void Object::Modify_Delete(bool _delete) {
    this->Delete = _delete;
}
void Object::Modify_Source_Read(int s) {
    Source_Read = s;
}
void Object::Modify_Source_Update(int s) {
    Source_Update = s;
}
void Object::Modify_Source_Insert(int s) {
    Source_Insert = s;
}
void Object::Modify_Source_Delete(int s) {
    Source_Delete = s;
}
void Object::Modify_Delegate_Read(bool Delegate_Read) {
    this->Delegate_Read = Delegate_Read;
}
void Object::Modify_Delegate_Update(bool Delegate_Update) {
    this->Delegate_Update = Delegate_Update;
}
void Object::Modify_Delegate_Insert(bool Delegate_Insert) {
    this->Delegate_Insert = Delegate_Insert;
}
void Object::Modify_Delegate_Delete(bool Delegate_delete) {
    this->Delegate_delete = Delegate_delete;
}
string Object::Get_Filename() {
    return this->FileName;
}
int Object::Get_Id() {
    return this->id;
}
int Object::Get_Source_Read() {
    return Source_Read;
}
int Object::Get_Source_Update() {
    return Source_Update;
}
int Object::Get_Source_Insert() {
    return Source_Insert;
}
int Object::Get_Source_Delete() {
    return Source_Delete;
}
bool Object::Get_Read() {
    return Read;
}
bool Object::Get_Update() {
    return Update;
}
bool Object::Get_Insert() {
    return Insert;
}
bool Object::Get_Delete() {
    return Delete;
}
bool Object::Get_Delegate_Read() {
    return Delegate_Read;
}
bool Object::Get_Delegate_Update() {
    return Delegate_Update;
}
bool Object::Get_Delegate_Insert() {
    return Delegate_Insert;
}
bool Object::Get_Delegate_Delete() {
    return Delegate_delete;
}
Object::~Object() {}
istream& operator >> (istream& in, Object& obj)//重载in[便于文件读入]
{
    in >> obj.id >> obj.FileName;
    return in;
}

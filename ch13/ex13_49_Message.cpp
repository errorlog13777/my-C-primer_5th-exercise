#include "Message.h"
#include <iostream>

void swap(Message &lhs, Message &rhs)
{
    using std::swap;
    lhs.remove_from_Folders(); // use existing member function to avoid duplicate code.
    rhs.remove_from_Folders(); // use existing member function to avoid duplicate code.

    swap(lhs.folders, rhs.folders);
    swap(lhs.contents, rhs.contents);

    lhs.add_to_Folders(lhs);  // use existing member function to avoid duplicate code.
    rhs.add_to_Folders(rhs);  // use existing member function to avoid duplicate code.
}

// Message Implementation

void Message::save(Folder &f)
{
    // folders.insert(&f);    // 將給定的 Folder 添加到我們的 Folder 列表中
    addFldr(&f);           // use existing member function to avoid duplicate code.
    f.addMsg(this);        // 將本 Message 添加到 f 的 Message 集合中
}

void Message::remove(Folder &f)
{
    // folders.erase(&f);    // 將給定的 Folder 從我們的 Folder 列表中刪除
    remFldr(&f);           // use existing member function to avoid duplicate code.
    f.remMsg(this);        // 將本 Message 從 f 的 Message 集合中刪除
}

// 將本 Message 添加到指向 m 的 Folder 中
void Message::add_to_Folders(const Message &m)
{
    for(auto f : m.folders) // 對每個包含 m 的 Folder
        f->addMsg(this);    // 向該 Folder 添加一個指向本 Message 的指針
}

Message::Message(const Message &m) : contents(m.contents), folders(m.folders)
{
    add_to_Folders(m);    // 將本消息添加到指向 m 的 Folder 中
}

void Message::remove_from_Folders()
{
    for(auto f : folders)    // 對 folders 中每個指針
        f->remMsg(this);     // 從該 Folder 中刪除本 Message
}

Message::~Message()
{
    remove_from_Folders();
}

Message& Message::operator=(const Message &rhs)
{
    // 通過先刪除指針再插入他們來處理自賦值情況
    remove_from_Folders();    // 更新已有 Folder
    contents = rhs.contents;  // 從 rhs 拷貝消息內容
    folders = rhs.folders;    // 從 rhs 拷貝 Folder 指針
    add_to_Folders(rhs);      // 將本 Message 添加到那些 Folder 中
    return *this;
}

void Message::move_Folders(Message *m)
{
    folders = m->folders;
    for(auto f : folders){
        f->remMsg(m);
        f->addMsg(this);
    }
}

Message::Message(Message &&rm) : contents(std::move(rm.contents)) // use string move constructor
{
    move_Folders(&rm);
}

Message& Message::operator=(Message &&rm)
{
    if(this != &rm){
        remove_from_Folders();
        contents = rm.contents;
        move_Folders(&rm);
    }
    return *this;
}

void Message::print_debug(void)
{
    std::cout << this->contents << std::endl;
}


// Folder Implementation

void swap(Folder &lhs, Folder &rhs)
{
    using std::swap;
    lhs.remove_from_Message();
    rhs.remove_from_Message();

    swap(lhs.Messages, rhs.Messages);

    lhs.add_to_Message(lhs);
    rhs.add_to_Message(rhs);
}

void Folder::add_to_Message(const Folder &f)
{
    for(Message *m : f.Messages)
        m->addFldr(this);
}

Folder::Folder(const Folder &f) : Messages(f.Messages)
{
    add_to_Message(f);
}

void Folder::remove_from_Message(void)
{
    for(Message *m : this->Messages)
        m->remFldr(this);
}

Folder::~Folder()
{
    remove_from_Message();
}

Folder &Folder::operator=(const Folder &rhs)
{
    remove_from_Message();
    this->Messages = rhs.Messages;
    add_to_Message(rhs);
    return *this;
}

void Folder::move_Message(Folder *f)
{
    Messages = std::move(f->Messages); // use set move assignment operator
    for(auto m : Messages){
        m->remFldr(f);
        m->addFldr(this);
    }
    f->Messages.clear();
}

Folder::Folder(Folder &&rf) : name(std::move(rf.name))
{
    move_Message(&rf);
}

Folder& Folder::operator=(Folder &&rf)
{
    if(this != &rf){
        remove_from_Message();
        name = std::move(rf.name);
        move_Message(&rf);
    }
    std::cout << "Message members moved" << std::endl; // debug
    return *this;
}

void Folder::print_debug(void)
{
    for(Message *m : Messages)
        std::cout << m->contents << " ";
    std::cout << std::endl;
}

int main(void)
{
    return 0;
}
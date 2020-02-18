#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <string>
#include <set>
class Message;

class Folder{
    friend void swap(Folder &, Folder &);
    friend void swap(Message *, Message *);
    friend class Message;
public:
    explicit Folder(const std::string &str = "") :name(str) { }
    Folder(const Folder &);
    Folder &operator=(const Folder &);
    Folder(Folder &&rm);
    Folder &operator=(Folder &&rf);
    ~Folder();

    const std::string& fldr() const { return name; }
    void print_debug();

private:
    void add_to_Message(const Folder &);
    void remove_from_Message();

    void addMsg(Message *msg) { this->Messages.insert(msg); }
    void remMsg(Message *msg) { this->Messages.erase(msg); }

    void move_Message(Folder *rf);

    std::string name;
    std::set<Message*> Messages;
};

class Message{
    friend void swap(Message&, Message&);
    friend class Folder;
public:
    // folders 被 implicit 初始化為空集合
    explicit Message(const std::string &str = "") : contents(str) { }
    // 拷貝控制成員, 用來管理指向本 Message 的指針
    Message(const Message &);              // copy constructor
    Message &operator=(const Message &);   // copy-assignment operator
    Message(Message &&rm);
    Message &operator=(Message &&rm);
    ~Message();
    
    // 從給定 Folder 集合中添加/刪除本 Message
    void save(Folder&);
    void remove(Folder&);

    void print_debug(void);

private:
    std::string contents;      // 實際消息文本
    std::set<Folder*> folders; // 指向本 Message 的 Folder
    // copy constructo, copy-assigment operator 和 destructor 所使用的工具函數
    void add_to_Folders(const Message&);
    // 從 folders 中的每個 Folder 中刪除本 Message
    void remove_from_Folders();
    void addFldr(Folder *f) { folders.insert(f); }
    void remFldr(Folder *f) { folders.erase(f); }

    void move_Folders(Message *m);
};

#endif
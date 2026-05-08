#ifndef PERSON_H
#define PERSON_H

class Person {
protected:
    int id;
    char name[51];
    char password[51];
public:
    Person();
    Person(int pId, const char* pName, const char* pPassword);
    virtual ~Person();

    int getId() const;
    const char* getName() const;
    const char* getPassword() const;

    void setId(int value);
    void setName(const char* value);
    void setPassword(const char* value);

    virtual const char* getRole() const = 0;
    virtual void display() const = 0;
};

#endif

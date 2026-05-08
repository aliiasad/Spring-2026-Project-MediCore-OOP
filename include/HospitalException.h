#ifndef HOSPITALEXCEPTION_H
#define HOSPITALEXCEPTION_H

class HospitalException {
protected:
    char message[200];
public:
    HospitalException();
    explicit HospitalException(const char* msg);
    virtual const char* what() const;
    virtual ~HospitalException() {}
};

#endif

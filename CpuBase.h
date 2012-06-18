#ifndef _CPU_BASE_H_
#define _CPU_BASE_H_

class CpuBase
{
public:
    virtual void init() = 0;
    virtual void shutDown() = 0;
    virtual void run() = 0;
    virtual void step() = 0;
};

#endif

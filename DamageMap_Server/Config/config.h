#ifndef CONFIG_H
#define CONFIG_H

class Config
{
public:
    const static unsigned short PORT_NUMBER = 31400;
    const static unsigned short MAX_BUFFER_SIZE = 128;

public:
    static void init();
};

#endif // CONFIG_H

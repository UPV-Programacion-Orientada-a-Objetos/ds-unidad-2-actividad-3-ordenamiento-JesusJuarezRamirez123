#ifndef DATASOURCE_H
#define DATASOURCE_H

// Clase Base Abstracta para fuentes de datos
class DataSource {
public:
    virtual int getNext() = 0;
    virtual bool hasMoreData() = 0;
    virtual ~DataSource() {}
};

#endif

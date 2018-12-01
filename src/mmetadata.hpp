#ifndef MMETADATA_HPP
#define MMETADATA_HPP

class mMetadata
{
public:
    mMetadata(int id, int parent, std::string creation_time, size_t size,
              std::string lastmodify_time, std::string name, int type):
              id(id), parent(parent), creation_time(creation_time),
              lastmodify_time(lastmodify_time), size(size), name(name),
              type(type){}

    void updata_parent(int parent)
    {
        this->parent = parent;
    }

    void update_lastmodify_time(std::string lastmodify_time)
    {
        this->lastmodify_time = lastmodify_time;
    }

    void update_name(std::string name)
    {
        this->name = name;
    }

    std::string summary()
    {
        return std::to_string(id)+" "+std::to_string(parent)+" "+creation_time+" "
        +lastmodify_time+std::to_string(size)+name+std::to_string(type);
    }

private:
    const int id;
    int parent;
    const std::string creation_time;
    std::string lastmodify_time;
    size_t size;
    std::string name;
    const int type;
};

#endif

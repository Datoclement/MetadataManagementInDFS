#ifndef MSYSTEMTREE_HPP
#define MSYSTEMTREE_HPP

#include "msystemtree.hpp"
#include "mfilesystem.hpp"

#include <vector>
#include <string>

class mFileSystem;

class mSystemTree
{
public:
    mSystemTree(mFileSystem* mfs);
    ~mSystemTree();
    void get_working_directory(std::string& placeholder);
    void make_directory(const std::string& path, std::string& placeholder);
    void list(const std::string& path, const bool recursive, std::string& placeholder);
    void change_directory(const std::string& path, std::string& placeholder);
    void move(const std::string& src, const std::string& dest, std::string& placeholder);
    void touch(const std::string& path, std::string& placeholder);
    void state(const std::string& path, std::string& placeholder);
    void remov(const std::string& path, const bool recursive, std::string& placeholder);

private:
    class mNode
    {
    public:

        mNode(mSystemTree* mst);
        mNode(mSystemTree* mst, mNode* parent, const std::string name, const bool is_file, const int size, std::string& placeholder);

        mNode* const& addchild(const std::string& name, const bool is_file, const int size, std::string& placeholder);
        void reattachto(mNode* new_parent);
        void setname(const std::string& name);
        void updatenothing();
        void releasechild(int i);
        void releasechild(mNode* ch);

        void moveto(const std::string& name, mNode*& dest);
        int contain(const std::string& name) const;
        const std::string& name() const;
        mNode* const& parent() const;
        const std::vector<mNode*>& children() const;
        const int object_id() const;
        bool is_file() const;
        void asstring(std::string& placeholder) const;

    private:

        static int _node_count;

        mSystemTree* const mst;
        const int _object_id;
        const time_t _creation_time;
        int _size;
        time_t _modification_time;

        std::string _name;
        const bool _is_file;
        mNode* _parent;
        std::vector<mNode*> _children;

        mNode(mSystemTree* mst, bool is_file);
        void detach();
        void attachto(mNode* new_parent);
        void update();
        std::string creation_message() const;
        void systemcall(const std::string& message, std::string& placeholder) const;
    };

    mFileSystem* mfs;
    mNode* root;
    mNode* current_node;
    bool valid_path(const std::vector<std::string>& paths, mNode* src, mNode*& dest);
    void recursive_clean_memory(mNode* node);
    void recursive_list(const mNode* const node, std::vector<std::string>& collected, int offset);
    void dirpaths(const std::vector<std::string>& paths, std::vector<std::string>& pathholder);
    bool create_directory(const std::vector<std::string>& paths, mNode*& nodeholder, std::string& placeholder);
};
#endif

#include <string>
#include <iostream>
#include <ctime>

#include "msystemtree.hpp"
#include "merror.hpp"
#include "api.hpp"
#include "mfilesystem.hpp"

using namespace std;

void ptokenize(const string& path, vector<string>& paths)
{
    tokenize(path, paths, "/");
}

int mSystemTree::mNode::_node_count = 0;

mSystemTree::mNode::mNode(const bool is_file):
    _object_id(mNode::_node_count++),
    _is_file(is_file),
    _creation_time(time(0))
{
    this->_modification_time = this->_creation_time;
}

mSystemTree::mNode::mNode():
    mNode(false)
{
    this->_parent = nullptr;
    this->_name = "~";
}

mSystemTree::mNode::mNode(mNode* parent, const string name, const bool is_file, const int size):
    mNode(is_file)
{
    if (parent == nullptr)
    {
        error("Error: parent should never be nullptr. debug it.");
    }
    this->_parent = parent;
    this->_name = name;
    this->_size = size;
}

const string& mSystemTree::mNode::name() const
{
    return this->_name;
}
mSystemTree::mNode* const& mSystemTree::mNode::parent() const
{
    return this->_parent;
}
const vector<mSystemTree::mNode*>& mSystemTree::mNode::children() const
{
    return this->_children;
}

int mSystemTree::mNode::contain(const string& name)
{
    int len = this->_children.size();
    for (int i=0;i<len;i++)
    {
        if (this->_children[i]->_name == name)
        {
            return i;
        }
    }
    return -1;
}

void mSystemTree::mNode::moveto(const string& name, mNode*& dest)
{
    if (name == "..")
    {
        dest = this->_parent;
    }
    else if (name == ".")
    {
        dest = this;
    }
    else
    {
        int id = this->contain(name);
        if (id < 0)
        {
            dest = nullptr;
        }
        else
        {
            dest = this->_children[id];
        }
    }
}

mSystemTree::mSystemTree()
{
    this->root = new mNode();
    this->current_node = this->root;
}

void mSystemTree::mNode::detach()
{
    int num_siblings = this->_parent->_children.size();
    for (int i=0;i<num_siblings;i++)
    {
        if (this->_parent->_children[i]->_name == this->_name)
        {
            this->_parent->_children.erase(this->_parent->_children.begin()+i);
            break;
        }
    }
    this->_parent = nullptr;
}

void mSystemTree::mNode::attachto(mNode* new_parent)
{
    if (new_parent == nullptr)
    {
        error("Error: new_parent should never be nullptr. debug it.\n");
    }
    else if (new_parent->_is_file)
    {
        error("Error: new_parent should never be a file. debug it.\n");
    }
    this->_parent = new_parent;
    this->_parent->_children.push_back(this);
}

bool mSystemTree::mNode::is_file() const
{
    return this->_is_file;
}

void mSystemTree::mNode::reattachto(mNode* new_parent)
{
    this->update();
    this->detach();
    this->attachto(new_parent);
}

mSystemTree::mNode* const& mSystemTree::mNode::addchild(const string& name, const bool is_file, const int size)
{
    if (this->_is_file)
    {
        error("Error: this should never be a file. debug it.\n");
    }
    this->update();
    mNode* child = new mNode(this, name, is_file, size);
    this->_children.push_back(child);
    return this->_children.back();
}

void mSystemTree::mNode::setname(const string& name)
{
    this->update();
    this->_name = name;
}

void mSystemTree::mNode::releasechild(int i)
{
    this->update();
    delete this->_children[i];
}

void mSystemTree::mNode::updatenothing()
{
    this->update();
}

void mSystemTree::mNode::update()
{
    this->_modification_time = time(0);
}

const int mSystemTree::mNode::object_id() const
{
    return this->_object_id;
}

string mSystemTree::mNode::asstring()
{
    string str;
    str += "object id: " + to_string(this->_object_id) + "\n";
    str += "created at: " + string(ctime(&this->_creation_time));
    str += "last modified at: " + string(ctime(&this->_modification_time));
    str += "type: " + string(this->_is_file?"file":"directory") + "\n";
    str += "size: " + to_string(this->_size) + "\n";
    return str;
}

void mSystemTree::mNode::remov(const int obj_id)
{
    mNode* deleted_node = nullptr;
    int num_children = this->_children.size();
    for (int i=0;i<num_children;i++)
    {
        if (obj_id == this->_children[i]->_object_id)
        {
            deleted_node = this->_children[i];
        }
    }
    if (deleted_node == nullptr)
    {
        error("Error: deleted_node should never be nullptr. debug it.\n");
    }
    else
    {
        deleted_node->disappear();
    }
}

void mSystemTree::mNode::disappear()
{
    this->update();
    this->detach();
    if (this->_is_file)
    {
        cout << "maybe need to call the metadata server to delete myself." << endl;
    }
    else
    {
        int num_children = this->_children.size();
        for (int i=0;i<num_children;i++)
        {
            this->_children[i]->disappear();
        }
    }
}

void mSystemTree::get_working_directory(string& placeholder)
{
    placeholder = "\n";
    mNode* local_node = this->current_node;
    while (true)
    {
        placeholder = local_node->name() + placeholder;
        local_node = local_node->parent();
        if (local_node == nullptr)
        {
            break;
        }
        else
        {
            placeholder = "/" + placeholder;
        }
    }
}

void mSystemTree::make_directory(const string& path, string& placeholder)
{
    vector<string> paths;
    ptokenize(path, paths);
    mNode* nodeholder = nullptr;
    if (!this->create_directory(paths, nodeholder))
    {
        placeholder = "Error: " + path + " is impossible because it passes by some file.\n";
    }
}

bool mSystemTree::create_directory(const vector<string>& paths, mNode*& nodeholder)
{
    int len_paths = paths.size();
    mNode* local_node = this->current_node;
    mNode* tmp_node = nullptr;
    for (int i=0;i<len_paths;i++)
    {
        if (local_node->is_file())
        {
            return false;
        }
        else
        {
            local_node->moveto(paths[i], tmp_node);
            if (tmp_node == nullptr)
            {
                tmp_node = local_node->addchild(paths[i]);
            }
            local_node = tmp_node;
        }
    }
    nodeholder = local_node;
    return true;
}

void mSystemTree::list(const string& path, const bool recursive, string& placeholder)
{
    vector<string> paths;
    ptokenize(path, paths);
    mSystemTree::mNode* dest = nullptr;
    if (valid_path(paths, this->current_node, dest))
    {
        if (recursive)
        {
            vector<string> strs;
            recursive_list(dest, strs, 0);
            placeholder = "";
            for (int i=0;i<strs.size();i++)
            {
                placeholder += strs[i] + "\n";
            }
        }
        else
        {
            int num_current_dirs = dest->children().size();
            for (int i=0;i<num_current_dirs;i++)
            {
                placeholder = placeholder + dest->children()[i]->name() + "\n";
            }
        }
    }
    else
    {
        placeholder = "Error: directory " + path + " is not found.\n";
    }
}

void mSystemTree::change_directory(const string& path, string& placeholder)
{
    vector<string> paths;
    ptokenize(path, paths);
    mNode* destholder = nullptr;
    if (this->valid_path(paths, this->current_node, destholder))
    {
        if (destholder->is_file())
        {
            placeholder = "Error: " + path + " is a file.\n";
        }
        else
        {
            this->current_node = destholder;
        }
    }
    else
    {
        placeholder = "Error: directory " + path + " is not found.\n";
    }
}

void mSystemTree::move(const std::string& src, const std::string& dest, std::string& placeholder)
{
    vector<string> src_paths, dest_paths;
    ptokenize(src, src_paths);
    ptokenize(dest, dest_paths);
    mNode* src_node = nullptr;
    mNode* dest_node = nullptr;
    if (this->valid_path(src_paths, this->current_node, src_node))
    {
        if (this->valid_path(dest_paths, this->current_node, dest_node))
        {
            if (dest_node->is_file())
            {
                placeholder = "Error: destination " + dest + " is a file.\n";
            }
            else
            {
                if (dest_node->contain(src_node->name()) >= 0)
                {
                    placeholder = "Error: destination " + dest + " has a child of the same name.\n";
                }
                else
                {
                    src_node->reattachto(dest_node);
                }
            }
        }
        else
        {
            vector<string> dest_dirpaths;
            this->dirpaths(dest_paths, dest_dirpaths);
            if(this->create_directory(dest_dirpaths, dest_node))
            {
                src_node->reattachto(dest_node);
                src_node->setname(dest_paths.back());
            }
            else
            {
                placeholder = "Error: " + dest + " is impossible because it passes by some file.\n";
            }
        }
    }
    else
    {
        placeholder = "Error: directory/file " + src + " is not found.\n";
    }
}

void mSystemTree::state(const string& path, string& placeholder)
{
    vector<string> paths;
    ptokenize(path, paths);
    mNode* dest_node = nullptr;
    if (this->valid_path(paths, this->current_node, dest_node))
    {
        placeholder = dest_node->asstring();
    }
    else
    {
        placeholder = "Error: directory/file " + path + " not found.\n";
    }
}

void mSystemTree::touch(const string& path, string& placeholder)
{
    vector<string> paths;
    ptokenize(path, paths);
    mNode* dest_node = nullptr;
    if (this->valid_path(paths, this->current_node, dest_node))
    {
        dest_node->updatenothing();
    }
    else
    {
        vector<string> dirpaths;
        this->dirpaths(paths, dirpaths);
        if(this->create_directory(dirpaths, dest_node))
        {
            dest_node->addchild(paths.back(), true, 0);
        }
        else
        {
            placeholder = "Error: " + path + " is impossible because it passes by some file.\n";
        }
    }
}

void mSystemTree::remov(const std::string& path, const bool recursive, std::string& placeholder)
{
    vector<string> paths;
    ptokenize(path, paths);
    mNode* node = nullptr;
    if (this->valid_path(paths, this->current_node, node))
    {
        if (node == this->root)
        {
            placeholder = "Error: cannot delete the root.\n";
        }
        else if (node->is_file() || recursive)
        {
            node->parent()->remov(node->object_id());
        }
        else
        {
            placeholder = "Error: " + path + " is a directory. use rm -r DIRNAME to remove a directory.\n";
        }
    }
    else
    {
        placeholder = "Error: file " + path + " not found.\n";
    }
}

bool mSystemTree::valid_path(const vector<string>& paths, mNode* src, mNode*& dest)
{
    dest = src;
    int num_part = paths.size();
    for (int i=0;i<num_part;i++)
    {
        dest->moveto(paths[i], dest);

        if (dest == nullptr)
        {

            return false;
        }
    }
    return true;
}

void mSystemTree::recursive_list(const mNode* const node, vector<string>& collected, int offset)
{
    string indented_name = string((size_t)(offset * 2), ' ') + node->name();
    if (!node->is_file())
    {
        indented_name += "/";
    }
    collected.push_back(indented_name);
    int num_current_dirs = node->children().size();
    for (int i=0;i<num_current_dirs;i++)
    {
        recursive_list(node->children()[i], collected, offset+1);
    }
}

void mSystemTree::recursive_clean_memory(mNode* node)
{
    for(int i=0;i<node->children().size();i++)
    {
        recursive_clean_memory(node->children()[i]);
        node->releasechild(i);
    }
}

void mSystemTree::dirpaths(const vector<string>& paths, vector<string>& pathholder)
{
    int len_path = paths.size()-1;
    for (int i=0;i<len_path;i++)
    {
        pathholder.push_back(paths[i]);
    }
}

mSystemTree::~mSystemTree()
{
    this->recursive_clean_memory(this->root);
    delete this->root;
}

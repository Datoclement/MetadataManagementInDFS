#include "mfilegenerator.hpp"

#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>

#include "api.hpp"
#include "merror.hpp"

using namespace std;

mBloc::mBloc(int bloc_id, int machine_id, int offset, int size):
    bloc_id(bloc_id),
    machine_id(machine_id),
    offset(offset),
    size(size)
{}

bool mBloc::operator==(const mBloc& o) const
{
    return (this->bloc_id == o.bloc_id)
        && (this->machine_id == o.machine_id)
        && (this->offset == o.offset)
        && (this->size == o.size);
}

mSegment::mSegment(int segment_id, int num_replicas, vector<mBloc*> replicas):
    segment_id(segment_id),
    num_replicas(num_replicas),
    replicas(replicas)
{}

bool mSegment::operator==(const mSegment& o) const
{
    bool same = (this->segment_id == o.segment_id)
            && (this->num_replicas == o.num_replicas);
    if (same)
    {
        for (int i=0;i<this->num_replicas;i++)
        {
            same = same && (*this->replicas[i] == *o.replicas[i]);
        }
    }
    return same;
}

mSegment::~mSegment()
{
    for (int i=0;i<num_replicas;i++)
    {
        delete this->replicas[i];
    }
}

mMetadata::mMetadata(int file_id, int num_segments, vector<mSegment*> segments):
    file_id(file_id),
    num_segments(num_segments),
    segments(segments)
{}

bool mMetadata::operator==(const mMetadata& o) const
{
    bool same = (this->file_id == o.file_id)
            && (this->num_segments == o.num_segments);
    if (same)
    {
        for (int i=0;i<this->num_segments;i++)
        {
            same = same && (*this->segments[i] == *o.segments[i]);
        }
    }
    return same;
}

mMetadata::~mMetadata()
{
    for (int i=0;i<num_segments;i++)
    {
        delete this->segments[i];
    }
}

int max_segments = 3;
int max_replicas = 3;
int num_machines = 5;
int max_size = 100000;
int max_offset = 100000;

int num_metas = 0;
void generate_random_metadata(mMetadata*& metaholder)
{
    int file_id = num_metas++;
    int num_segments = (rand() % max_segments) + 1;
    vector<mSegment *> segments;
    for (int segment_id=0;segment_id<num_segments;segment_id++)
    {
        int num_replicas = (rand() % max_replicas) + 1;
        vector<mBloc *> replicas;
        for (int bloc_id=0;bloc_id<num_replicas;bloc_id++)
        {
            int machine_id = (rand() % num_machines);
            int offset = (rand() % max_offset);
            int size = (rand() % max_size);
            replicas.push_back(new mBloc(bloc_id, machine_id, offset, size));
        }
        segments.push_back(new mSegment(segment_id, num_replicas, replicas));
    }
    metaholder = new mMetadata(file_id, num_segments, segments);
}

void metadata_to_string(const mMetadata* const& metadata, std::string& strholder)
{
    vector<int> data;
    data.push_back(metadata->file_id);
    data.push_back(metadata->num_segments);
    for (int i=0;i<metadata->num_segments;i++)
    {
        mSegment* segment = metadata->segments[i];
        data.push_back(segment->segment_id);
        data.push_back(segment->num_replicas);
        for (int j=0;j<segment->num_replicas;j++)
        {
            mBloc* bloc = segment->replicas[j];
            data.push_back(bloc->bloc_id);
            data.push_back(bloc->machine_id);
            data.push_back(bloc->offset);
            data.push_back(bloc->size);
        }
    }

    strholder = "";
    for (int i=0;i<data.size();i++)
    {
        strholder += to_string(data[i]) + " ";
    }
}

void string_to_metadata(const string& str, mMetadata*& metaholder)
{
    vector<string> tokens;
    tokenize(str, tokens, " ");
    vector<int> data;
    try
    {
        for (int i=0;i<tokens.size();i++)
        {
            data.push_back(stoi(tokens[i]));
        }
    }
    catch (invalid_argument e)
    {
        cout << "metadata string: " << str << endl;
        error("Error: the metadata string should only contains integers.\n");
    }

    try
    {
        int len = data.size();
        int pos = 0;
        int file_id = data[pos++];
        int num_segments = data[pos++];
        metaholder = new mMetadata(file_id, num_segments);
        for (int i=0;i<num_segments;i++)
        {
            int segment_id = data[pos++];
            int num_replicas = data[pos++];
            metaholder->segments.push_back(new mSegment(segment_id, num_replicas));
            for (int j=0;j<num_replicas;j++)
            {
                int bloc_id = data[pos++];
                int machine_id = data[pos++];
                int offset = data[pos++];
                int size = data[pos++];
                metaholder->segments.back()->replicas.push_back(new mBloc(bloc_id, machine_id, offset, size));
            }
        }
    }
    catch (out_of_range e)
    {
        cout << "metadata string is " << str << endl;
        error("Error: the format of the metadata string is not complete.\n");
    }
}

// int main(int argc, char** argv)
// {
//     cout << "Test Unit: file generator." << endl;
//     vector<mMetadata*> files;
//     for (int i=0;i<5;i++)
//     {
//         mMetadata* f = nullptr;
//         generate_random_metadata(f);
//         files.push_back(f);
//     }
//     vector<string> serializeds;
//     for (int i=0;i<5;i++)
//     {
//         string str;
//         metadata_to_string(files[i], str);
//         serializeds.push_back(str);
//     }
//     for (int i=0;i<5;i++)
//     {
//         cout << serializeds[i] << endl;
//     }
//     vector<mMetadata*> restoreds;
//     for (int i=0;i<5;i++)
//     {
//         mMetadata* f = nullptr;
//         string_to_metadata(serializeds[i], f);
//         restoreds.push_back(f);
//     }
//     bool equals = true;
//     for(int i=0;i<5;i++)
//     {
//         if (!((*restoreds[i]) == (*files[i])))
//         {
//             equals = false;
//         }
//     }
//     vector<string> strr;
//     for (int i=0;i<5;i++)
//     {
//         string str;
//         metadata_to_string(restoreds[i], str);
//         strr.push_back(str);
//     }
//     for (int i=0;i<5;i++)
//     {
//         cout << strr[i] << endl;
//     }
//     cout << "restoreds are equal to originals: " << (equals?"true":"false") << endl;
// }

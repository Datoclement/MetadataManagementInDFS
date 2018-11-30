#ifndef MFILEGENERATOR_HPP
#define MFILEGENERATOR_HPP

#include <string>
#include <vector>

struct mBloc
{
    int bloc_id;
    int machine_id;
    int offset;
    int size;
    mBloc(int bloc_id, int machine_id, int offset, int size);
    bool operator==(const mBloc& o) const;
};

struct mSegment
{
    int segment_id;
    int num_replicas;
    std::vector<struct mBloc *> replicas;
    mSegment(int segment_id, int num_replicas, std::vector<mBloc*> replicas=std::vector<mBloc*>());
    bool operator==(const mSegment& o) const;
    ~mSegment();
};

struct mMetadata
{
    int file_id;
    int num_segments;
    std::vector<struct mSegment *> segments;
    mMetadata(int file_id, int num_segments, std::vector<mSegment*> segments=std::vector<mSegment*>());
    bool operator==(const mMetadata& o) const;
    ~mMetadata();
};

void generate_random_metadata(struct mMetadata*& metaholder);
void metadata_to_string(const struct mMetadata* const& metadata, std::string& strholder);
void string_to_metadata(const std::string& str, struct mMetadata*& metaholder);

#endif

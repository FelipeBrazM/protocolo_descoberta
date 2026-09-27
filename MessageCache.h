#pragma once
#include <map>
#include <utility>
namespace veins {
// Cache retained until the original deadline, even after a frame was forwarded.
template<class Time> class MessageCache {
    std::map<std::pair<int,long>,Time> seen;
public:
    void expire(Time now) {
        for(auto i=seen.begin();i!=seen.end();) {
            if(i->second<=now) i=seen.erase(i); else ++i;
        }
    }
    bool insert(int origin,long sequence,Time deadline) {
        return seen.emplace(std::make_pair(origin,sequence),deadline).second;
    }
};
}

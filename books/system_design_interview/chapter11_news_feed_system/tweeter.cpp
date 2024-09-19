#include <iostream>
#include <chrono>
#include <deque>
#include <functional>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>


class Twitter {
    using TweetPost = std::pair<std::chrono::time_point<std::chrono::system_clock>, int>;
    using TweetPostTable = std::vector<TweetPost>;
    using TweetTimelineEntry = std::pair<int, TweetPostTable::reverse_iterator>;
    using TweetTimelineContainer = std::vector<std::pair<int, TweetPostTable::reverse_iterator>>;
    static const int NewsFeedMaxSize = 10;
public:
    Twitter() {}

    void postTweet(int userId, int tweetId) {
        mTweets[userId].push_back(std::make_pair(std::chrono::system_clock::now(), tweetId));
        mFollowees[userId].insert(userId);
    }

    std::vector<int> getNewsFeed(int userId) {
        std::vector<int> newsFeed;
        std::priority_queue<TweetTimelineEntry,
                            TweetTimelineContainer,
                            decltype(mComp)> pq{mComp};

        for (int follower : mFollowees[userId]) {
            if (mTweets[follower].size() > 0) {
                pq.push(std::make_pair(follower, mTweets[follower].rbegin()));
            }
        }

        while (!pq.empty() && newsFeed.size() < NewsFeedMaxSize) {
            auto [follower, tweetPostTableIter] = pq.top();
            pq.pop();

            newsFeed.push_back(tweetPostTableIter->second);
            if (++tweetPostTableIter != mTweets[follower].rend()) {
                pq.push(std::make_pair(follower, tweetPostTableIter));
            }
        }

        return newsFeed;
    }

    void follow(int followerId, int followeeId) {
        mFollowees[followerId].insert(followeeId);
        mFollowees[followerId].insert(followerId);
        mFollowees[followeeId].insert(followeeId);
    }

    void unfollow(int followerId, int followeeId) {
        mFollowees[followerId].erase(followeeId);
    }

private:
    std::function<bool (const TweetTimelineEntry &lhs, const TweetTimelineEntry &rhs)> mComp = [](const TweetTimelineEntry &lhs, const TweetTimelineEntry &rhs) {
        return lhs.second->first < rhs.second->first;
    };

private:
    std::unordered_map<int, std::set<int>> mFollowees;
    std::unordered_map<int, TweetPostTable> mTweets;
};

int main()
{
    return 0;
}

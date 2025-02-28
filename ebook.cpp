#include <iostream>
#include <iomanip>
#include <vector>
#include <numeric>

using namespace std;

class EBookManager {
public:

    EBookManager()
        : users_page_(MAX_USER_COUNT_ + 1, 0)
        , users_at_page_(MAX_PAGE_COUNT_ + 1, 0) {
    }

    void ReadPage(const int user, const int page) {

        if (CheckUserExists(user)) {
            int old_page = users_page_[user];
            users_page_[user] = page;
            --users_at_page_[old_page];
            ++users_at_page_[page];
        } else {
            ++user_count_;
            users_page_[user] = page;
            ++users_at_page_[page];
        }

        UpdatePrefixSum();
    }

    double CheerUser(const int user) const {
        if (!CheckUserExists(user)) {
            return 0.;
        }
        if (user_count_ == 1) {
            return 1.;
        }

        int page = users_page_[user];
        
        // Сумма от 0 до page - 1
        int less_users = prefix_sum_[page];

        return static_cast<double>(less_users) / (user_count_ - 1);
    }

private:
    static const int MAX_USER_COUNT_ = 100'000;
    static const int MAX_PAGE_COUNT_ = 1'000;
    vector<int> users_page_;
    vector<int> users_at_page_;
    vector<int> prefix_sum_;
    int user_count_ = 0;

    bool CheckUserExists(int user) const {
        if (user_count_ == 0) {
            return false;
        }
        if (user < users_page_.size()) {
            if (users_page_[user] != 0) {
                return true;
            }
            return false;
        }
        return false;
    }

    void UpdatePrefixSum() {
        prefix_sum_.resize(users_at_page_.size() + 1, 0);
        partial_sum(users_at_page_.begin(), users_at_page_.end(), prefix_sum_.begin() + 1);
    }
};

int main() {
    int query_number;
    cin >> query_number;

    string command;
    int user, page;

    EBookManager book;

    cout << setprecision(6);
    for (int i = 0; i < query_number; ++i) {
        cin >> command >> user;
        if (command == "READ"s) {
            cin >> page;

            book.ReadPage(user, page);

        } else if (command == "CHEER"s) {

            cout << book.CheerUser(user) << endl;
            
        }
    }
}

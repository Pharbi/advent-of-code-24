#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <stdexcept>
#include <queue>

class AdventDay {
    public:
        explicit AdventDay(const std::string& input_file_path) : input_file_path(input_file_path) {}
        virtual ~AdventDay() = default;

        virtual void parseInput() = 0;
        virtual void solve() = 0;

    protected:
        std::string input_file_path;
};

class DayOne : public AdventDay {
public:
    explicit DayOne(const std::string& input_file_path) : AdventDay(input_file_path) {}

    void parseInput() override {
        std::ifstream input_file(input_file_path);
        if (!input_file.is_open()) {
            throw std::runtime_error("Error: Could not open the file " + input_file_path);
        }

        std::string line;
        while (std::getline(input_file, line)) {
            std::istringstream lineStream(line);
            int col_one, col_two;
            if (lineStream >> col_one >> col_two) {
                first_queue.push(col_one);
                second_set.insert(col_two);
            }
        }
    }

    void solve() override {
        parseInput();
        int distance = calculateTotalDistance();
        int similarity_score = calculateSimilarityScore();

        std::cout << "Day 1 - Total Distance: " << distance << "\n";
        std::cout << "Day 1 - Similarity Score: " << similarity_score << "\n";
    }

private:
    std::priority_queue<int, std::vector<int>, std::greater<int>> first_queue;
    std::multiset<int> second_set;

    int calculateTotalDistance() {
        int total = 0;
        auto it = second_set.begin();
        auto temp_queue = first_queue;
        while (!temp_queue.empty() && it != second_set.end()) {
            total += std::abs(temp_queue.top() - *it);
            temp_queue.pop();
            ++it;
        }
        return total;
    }

    int calculateSimilarityScore() {
        std::map<int, int> appearance_count;
        auto temp_queue = first_queue;
        while (!temp_queue.empty()) {
            int current = temp_queue.top();
            temp_queue.pop();
            int count = second_set.count(current);
            appearance_count[current] += count;
        }

        int total = 0;
        for (const auto& pair : appearance_count) {
            total += pair.first * pair.second;
        }
        return total;
    }
};

class DayTwo : public AdventDay {
    public:
        explicit DayTwo(const std::string& input_file_path) : AdventDay(input_file_path) {}
        void parseInput() override {
            std::cout << "Day Two\n";
        };
        void solve() override {
            std::ifstream input_file(input_file_path);
            if (!input_file.is_open()) {
                throw std::runtime_error("Error: Could not open the file " + input_file_path);
            }
            int safe_report_count = 0;


            std::string line;
            while (std::getline(input_file, line)) {
                std::istringstream lineStream(line);
                std::vector<int> reports;
                int val;
                while (lineStream >> val) {
                    reports.push_back(val);
                }
                if (find_safe_reports(reports)) {
                    ++safe_report_count;
                }
            }

            std::cout << "Total safe reports: " << safe_report_count << std::endl;
        }

    private:

        bool is_increasing(const std::vector<int>& row) {
            for (size_t j = 1; j < row.size(); j++) {
                if (row[j] <= row[j - 1]) {
                    return false;
                }
            }
            return true;
        }

        bool is_decreasing(const std::vector<int>& row) {
            for (size_t j = 1; j < row.size(); j++) {
                if (row[j] >= row[j - 1]) {
                    return false;
                }
            }
            return true;
        }

        bool has_all_safe_levels(const std::vector<int>& row) {
            for (size_t j = 1; j < row.size(); j++) {
                int diff = std::abs(row[j] - row[j - 1]);
                if (diff < 1 || diff > 3) {
                    return false;
                }
            }
            return true;
        }

        bool find_safe_reports(const std::vector<int>& row) {
            // Check if the row satisfies all rules directly
            if ((is_increasing(row) || is_decreasing(row)) && has_all_safe_levels(row)) {
                return true;
            }

            // remove a single element at a time
            for (size_t i = 0; i < row.size(); i++) {
                std::vector<int> modified_row = row;
                modified_row.erase(modified_row.begin() + i);

                if ((is_increasing(modified_row) || is_decreasing(modified_row)) &&
                    has_all_safe_levels(modified_row)) {
                    return true;
                    }
            }

            return false;
        }
};


int main() {
    try {
        std::unique_ptr<AdventDay> dayOne = std::make_unique<DayOne>("../day-1.txt");
        dayOne->solve();
        std::unique_ptr<AdventDay> dayTwo = std::make_unique<DayTwo>("../day-2.txt");
        dayTwo->solve();
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}

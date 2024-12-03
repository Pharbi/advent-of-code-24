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
                firstQueue.push(col_one);
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
    std::priority_queue<int, std::vector<int>, std::greater<int>> firstQueue;
    std::multiset<int> second_set;

    int calculateTotalDistance() {
        int total = 0;
        auto it = second_set.begin();
        auto tempQueue = firstQueue;
        while (!tempQueue.empty() && it != second_set.end()) {
            total += std::abs(tempQueue.top() - *it);
            tempQueue.pop();
            ++it;
        }
        return total;
    }

    int calculateSimilarityScore() {
        std::map<int, int> appearance_count;
        auto tempQueue = firstQueue;
        while (!tempQueue.empty()) {
            int current = tempQueue.top();
            tempQueue.pop();
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

int main() {
    try {
        std::unique_ptr<AdventDay> dayOne = std::make_unique<DayOne>("../input.txt");
        dayOne->solve();
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}

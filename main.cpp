#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <stdexcept>
#include <queue>
#include <regex>

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

class DayThree: public AdventDay {
    public:
    explicit DayThree(const std::string& input_file_path) : AdventDay(input_file_path) {}
    void parseInput() override {
        std::cout << "Day Three\n";
    }
    void solve() override {
        std::ifstream input_file(input_file_path);
        if (!input_file.is_open()) {
            std::cerr << "Error: Could not open the file day-3.txt" << std::endl;
            return;
        }
        std::vector<int> products_v1 = parse_and_calculate_products_v1(input_file);
        int sum_of_products_v1 = vec_sum_calculator(products_v1);
        std::cout << "Parsed products v1 sum is " << sum_of_products_v1 << std::endl;


        input_file.clear();
        input_file.seekg(0);

        std::vector<int> products_v2 = parse_and_calculate_products_v2(input_file);
        int sum_of_products_v2 = vec_sum_calculator(products_v2);
        std::cout << "Parsed products v2 sum is " << sum_of_products_v2 << std::endl;
    }
    private:
    int vec_sum_calculator(const std::vector<int>& nums) {
        if (nums.empty()){ return 0; }
        int sum = 0;

        for(size_t i = 0; i < nums.size(); i++){
            sum += nums[i];
        }
        return sum;
    }
    std::vector<int> parse_and_calculate_products_v1(std::istream& input_stream) {
        std::vector<int> nums;
        int a, b;
        std::regex pattern(R"(mul\((\d+),(\d+)\))");
        std::string line;

        while (std::getline(input_stream, line)) {
            std::sregex_iterator begin(line.begin(), line.end(), pattern);
            std::sregex_iterator end;
            for (std::sregex_iterator it = begin; it != end; ++it) {
                std::smatch match = *it;
                std::string first_num = match[1];
                std::string second_num = match[2];

                std::stringstream f_n(first_num);
                f_n >> a;
                std::stringstream s_n(second_num);
                s_n >> b;

                nums.push_back(a * b);
            }
        }
        return nums;

    }

    std::vector<int> parse_and_calculate_products_v2(std::istream& input_stream) {
        std::vector<int> nums;
        int a, b;
        std::regex pattern_with_extra(R"(mul\((\d+),(\d+)\)|do\(\)|don\'t\(\))");
        std::string line;
        bool have_seen_do_not = false;

        while (std::getline(input_stream, line)) {
            std::sregex_iterator begin(line.begin(), line.end(), pattern_with_extra);
            std::sregex_iterator end;

            for (std::sregex_iterator it = begin; it != end; ++it){
                std::smatch match = *it;
                if (match[1].matched && match[2].matched) {
                    if (have_seen_do_not){
                        continue;
                    }
                    std::string first_num = match[1];
                    std::stringstream f_n(first_num);
                    f_n >> a;

                    std::string second_num = match[2];
                    std::stringstream s_n(second_num);
                    s_n >> b;

                    nums.push_back(a * b);
                } else if (match.str() == "do()") {
                    have_seen_do_not = false;
                } else if (match.str() == "don't()"){
                    have_seen_do_not = true;
                }
            }
        }
        return nums;
    }
};

class DayFour: public AdventDay {
public:
    explicit DayFour(const std::string& input_file_path) : AdventDay(input_file_path) {}

    void parseInput() override {
        std::ifstream input_file(input_file_path);
        if (!input_file.is_open()) {
            std::cerr << "Error: Could not open the file day-4.txt" << std::endl;
            return;
        }

        std::string line;
        while (std::getline(input_file, line)) {
            if (line.empty()) break;
            grid.push_back(line);
        }

        input_file.close();
    }

    void solve() override {
        parseInput();
        int res_one = countXMAS(grid);
        std::cout << "Found XMAS this many times " << res_one << std::endl;

        int res_two = countXMASPatterns(grid);
        std::cout << "Found XMAS this many times " << res_two << std::endl;
    }

    private:
    std::vector<std::string> grid;

    bool checkXMASPtOne(const std::vector<std::string>& grid, int row, int col, int drow, int dcol) {
        const std::string target = "XMAS";
        int R = grid.size();
        int C = grid[0].size();

        for (int i = 0; i < 4; i++) {
            int newRow = row + i * drow;
            int newCol = col + i * dcol;

            if (newRow < 0 || newRow >= R || newCol < 0 || newCol >= C) {
                return false;
            }

            if (grid[newRow][newCol] != target[i]) {
                return false;
            }
        }
        return true;
    }

    int countXMAS(const std::vector<std::string>& grid) {
        int count = 0;
        int R = grid.size();
        int C = grid[0].size();

        std::vector<std::pair<int, int>> directions = {
            {0, 1},
            {1, 0},
            {1, 1},
            {1, -1},
            {0, -1},
            {-1, 0},
            {-1, 1},
            {-1, -1}
        };

        for (int i = 0; i < R; i++) {
            for (int j = 0; j < C; j++) {
                for (const auto& dir : directions) {
                    if (checkXMASPtOne(grid, i, j, dir.first, dir.second)) {
                        count++;
                    }
                }
            }
        }
        return count;
    }

    bool inBounds(int row, int col, int R, int C) {
        return row >= 0 && row < R && col >= 0 && col < C;
    }

    bool checkDiagonalLine(const std::vector<std::string>& grid, int centerRow, int centerCol,
                          int drow, int dcol) {
        int R = grid.size();
        int C = grid[0].size();

        int row1 = centerRow + drow;
        int col1 = centerCol + dcol;
        int row2 = centerRow - drow;
        int col2 = centerCol - dcol;

        if (!inBounds(row1, col1, R, C) || !inBounds(row2, col2, R, C)) {
            return false;
        }

        char c1 = grid[row1][col1];
        char c2 = grid[row2][col2];

        return ((c1 == 'M' && c2 == 'S') || (c1 == 'S' && c2 == 'M'));
    }

    bool checkXMASPtTwo(const std::vector<std::string>& grid, int row, int col) {
        if (grid[row][col] != 'A') {
            return false;
        }

        std::vector<std::pair<int, int>> diagonals = {
            {1, 1},
            {1, -1}
        };

        for (int i = 0; i < diagonals.size(); i++) {
            for (int j = i + 1; j < diagonals.size(); j++) {
                if (checkDiagonalLine(grid, row, col, diagonals[i].first, diagonals[i].second) &&
                    checkDiagonalLine(grid, row, col, diagonals[j].first, diagonals[j].second)) {
                    return true;
                }
            }
        }
        return false;
    }

    int countXMASPatterns(const std::vector<std::string>& grid) {
        int count = 0;
        int R = grid.size();
        int C = grid[0].size();

        for (int i = 1; i < R - 1; i++) {
            for (int j = 1; j < C - 1; j++) {
                if (checkXMASPtTwo(grid, i, j)) {
                    count++;
                }
            }
        }
        return count;
    }

};


int main() {
    try {
        std::unique_ptr<AdventDay> dayOne = std::make_unique<DayOne>("../day-1.txt");
        dayOne->solve();
        std::unique_ptr<AdventDay> dayTwo = std::make_unique<DayTwo>("../day-2.txt");
        dayTwo->solve();
        std::unique_ptr<AdventDay> dayThree = std::make_unique<DayThree>("../day-3.txt");
        dayThree->solve();
        std::unique_ptr<AdventDay> dayFour = std::make_unique<DayFour>("../day-4.txt");
        dayFour->solve();
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}

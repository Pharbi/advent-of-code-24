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
#include <unordered_set>

using namespace std;

class AdventDay {
    public:
        explicit AdventDay(const std::string& input_file_path) : input_file_path(input_file_path) {}
        virtual ~AdventDay() = default;

        virtual void parseInput() = 0;
        virtual void solve() = 0;

    protected:
        std::string input_file_path;
};

class One : public AdventDay {
public:
    explicit One(const std::string& input_file_path) : AdventDay(input_file_path) {}

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

class Two : public AdventDay {
    public:
        explicit Two(const std::string& input_file_path) : AdventDay(input_file_path) {}
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

class Three: public AdventDay {
    public:
    explicit Three(const std::string& input_file_path) : AdventDay(input_file_path) {}
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

class Four: public AdventDay {
public:
    explicit Four(const std::string& input_file_path) : AdventDay(input_file_path) {}

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

class Five : public AdventDay {
public:
    explicit Five(const std::string& input_file_path)
        : AdventDay(input_file_path) {}

    void parseInput() override {
        std::ifstream file(input_file_path);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << input_file_path << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) {
                continue;
            }
            std::vector<char> row(line.begin(), line.end());
            grid.push_back(row);
        }

        file.close();
    }

    void solve() override {
        parseInput();

        int result = simulate_guard_patrol(grid);
        std::cout << "Number of distinct positions visited: " << result << std::endl;
    }

private:
    std::vector<std::vector<char>> grid;

    int simulate_guard_patrol(const std::vector<std::vector<char>>& grid) {
        int rows = static_cast<int>(grid.size());
        int cols = static_cast<int>(grid[0].size());

        std::pair<int,int> start_pos{-1, -1};
        char start_dir = '\0';

        bool found_start = false;
        for (int i = 0; i < rows && !found_start; ++i) {
            for (int j = 0; j < cols && !found_start; ++j) {
                char c = grid[i][j];
                if (c == '^' || c == 'v' || c == '<' || c == '>') {
                    start_pos = {i, j};
                    start_dir = c;
                    found_start = true;
                }
            }
        }

        if (start_dir == '\0') {
            return 0;
        }

        std::map<char, std::pair<int,int>> directions = {
            {'^', {-1,  0}},  // up
            {'v', { 1,  0}},  // down
            {'<', { 0, -1}},  // left
            {'>', { 0,  1}}   // right
        };

        // Right turn mapping
        std::map<char, char> turn_right = {
            {'^', '>'},
            {'>', 'v'},
            {'v', '<'},
            {'<', '^'}
        };

        std::set<std::pair<int,int>> visited;
        visited.insert(start_pos);

        std::pair<int,int> curr_pos = start_pos;
        char curr_dir = start_dir;

        while (true) {
            auto [dr, dc] = directions[curr_dir];
            int next_row = curr_pos.first + dr;
            int next_col = curr_pos.second + dc;

            if (next_row < 0 || next_row >= rows ||
                next_col < 0 || next_col >= cols) {
                break;
            }

            if (grid[next_row][next_col] == '#') {
                // Turn right
                curr_dir = turn_right[curr_dir];
            } else {
                // Move forward
                curr_pos = {next_row, next_col};
                visited.insert(curr_pos);
            }
        }

        return static_cast<int>(visited.size());
    }
};

class Six : public AdventDay {
public:
    explicit Six(const std::string& input_file_path)
        : AdventDay(input_file_path) {}

    void parseInput() override {
        grid_.clear();

        std::ifstream file(input_file_path);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << input_file_path << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) {
                continue;
            }
            std::vector<char> row(line.begin(), line.end());
            grid_.push_back(row);
        }
        file.close();

        rows_ = static_cast<int>(grid_.size());
        cols_ = rows_ > 0 ? static_cast<int>(grid_[0].size()) : 0;
    }

    void solve() override {
        parseInput();
        if (grid_.empty() || grid_[0].empty()) {
            std::cout << 0 << std::endl;
            return;
        }

        int result = findValidLoops();
        std::cout << result << std::endl;
    }

private:
    std::vector<std::vector<char>> grid_;
    int rows_ = 0;
    int cols_ = 0;

    const std::map<char, std::pair<int,int>> dirToVec_ = {
        {'^', {-1,  0}},
        {'v', { 1,  0}},
        {'<', { 0, -1}},
        {'>', { 0,  1}}
    };

    const std::map<char, char> rightTurns_ = {
        {'^', '>'},
        {'>', 'v'},
        {'v', '<'},
        {'<', '^'}
    };

    std::vector<std::tuple<std::pair<int,int>, char, char>> getCornerPatterns() const {
        // Example patterns, each is ((offsetRow, offsetCol), entryDir, exitDir)
        return {
            {{0, 0}, 'v', '>'},
            {{0, 0}, '<', '^'},
            {{0, 0}, '^', '<'},
            {{0, 0}, '>', 'v'}
        };
    }

    bool isValidCorner(const std::pair<int,int>& pos,
                       char entryDir,
                       const std::vector<std::vector<char>>& gridRef) const
    {
        int r = pos.first;
        int c = pos.second;

        if (r < 0 || r >= rows_ || c < 0 || c >= cols_) {
            return false;
        }

        auto it = dirToVec_.find(entryDir);
        if (it == dirToVec_.end()) {
            return false;
        }
        auto [dr, dc] = it->second;
        int prev_r = r - dr;
        int prev_c = c - dc;

        if (prev_r < 0 || prev_r >= rows_ || prev_c < 0 || prev_c >= cols_) {
            return false;
        }

        return (gridRef[prev_r][prev_c] != '#');
    }

    std::vector<std::tuple<std::pair<int,int>, char, char>>
    findPotentialCorners(const std::pair<int,int>& startPos,
                         char direction,
                         const std::vector<std::vector<char>>& gridRef) const
    {
        std::vector<std::tuple<std::pair<int,int>, char, char>> corners;
        int row = startPos.first;
        int col = startPos.second;

        if (direction == '\0') {
            static const std::vector<char> allDirs = {'^', '>', 'v', '<'};
            for (char testDir : allDirs) {
                auto it = dirToVec_.find(testDir);
                if (it == dirToVec_.end()) {
                    continue;
                }
                auto [dr, dc] = it->second;
                int tr = row + dr;
                int tc = col + dc;
                if (tr >= 0 && tr < rows_ && tc >= 0 && tc < cols_ &&
                    gridRef[tr][tc] != '#')
                {
                    auto subCorners = findPotentialCorners(startPos, testDir, gridRef);
                    corners.insert(corners.end(), subCorners.begin(), subCorners.end());
                }
            }
            return corners;
        }

        auto itDir = dirToVec_.find(direction);
        if (itDir == dirToVec_.end()) {
            return corners;
        }
        auto [dr, dc] = itDir->second;

        int currR = row;
        int currC = col;

        while (true) {
            currR += dr;
            currC += dc;

            if (currR < 0 || currR >= rows_ || currC < 0 || currC >= cols_) {
                break;
            }

            if (gridRef[currR][currC] == '#') {
                if (isValidCorner({currR, currC}, direction, gridRef)) {
                    auto itTurn = rightTurns_.find(direction);
                    if (itTurn != rightTurns_.end()) {
                        corners.push_back({{currR, currC}, direction, itTurn->second});
                    }
                }
                break;
            }

            if (gridRef[currR][currC] == '.') {
                if (isValidCorner({currR, currC}, direction, gridRef)) {
                    auto itTurn = rightTurns_.find(direction);
                    if (itTurn != rightTurns_.end()) {
                        corners.push_back({{currR, currC}, direction, itTurn->second});
                    }
                }
            }
        }

        return corners;
    }

    bool canFormLoop(const std::pair<int,int>& cornerPos,
                     const std::pair<int,int>& newObstaclePos,
                     const std::vector<std::vector<char>>& gridRef) const
    {
        std::vector<std::vector<char>> testGrid = gridRef;
        testGrid[newObstaclePos.first][newObstaclePos.second] = '#';

        std::pair<int,int> currPos = cornerPos;
        int cornersFound = 0;
        std::set<std::pair<int,int>> visited;
        char currDir = '\0';

        while (cornersFound < 4) {
            auto potCorners = findPotentialCorners(currPos, currDir, testGrid);
            bool foundNext = false;
            for (auto &cornerInfo : potCorners) {
                auto &nextPos  = std::get<0>(cornerInfo);
                char exitDir   = std::get<2>(cornerInfo);

                if (visited.find(nextPos) == visited.end()) {
                    visited.insert(nextPos);
                    currPos = nextPos;
                    currDir = exitDir;
                    cornersFound++;
                    foundNext = true;
                    break;
                }
            }
            if (!foundNext) {
                return false;
            }
        }

        return (currPos == cornerPos);
    }

    int findValidLoops()
    {
        std::pair<int,int> startPos{-1, -1};

        for (int i = 0; i < rows_; ++i) {
            bool found = false;
            for (int j = 0; j < cols_; ++j) {
                char c = grid_[i][j];
                if (c == '^' || c == 'v' || c == '<' || c == '>') {
                    startPos = {i, j};
                    grid_[i][j] = '.';
                    found = true;
                    break;
                }
            }
            if (found) {
                break;
            }
        }

        std::set<std::pair<int,int>> validPositions;

        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < cols_; ++j) {
                if (grid_[i][j] == '.' && std::make_pair(i, j) != startPos) {
                    if (canFormLoop({i, j}, {i, j}, grid_)) {
                        validPositions.insert({i, j});
                    }
                }
            }
        }

        return static_cast<int>(validPositions.size());
    }
};

class Seven : public AdventDay {
    int64_t sum = 0;

    int64_t concatenate(int64_t a, int64_t b) {
        std::string str_b = std::to_string(b);
        return a * static_cast<int64_t>(pow(10, str_b.length())) + b;
    }

    int64_t evaluate(const std::vector<int64_t>& numbers, const std::vector<char>& operators) {
        if (numbers.empty()) return 0;
        if (numbers.size() == 1) return numbers[0];

        int64_t result = numbers[0];
        for (size_t i = 0; i < operators.size(); i++) {
            if (operators[i] == '+') {
                result += numbers[i + 1];
            } else if (operators[i] == '*') {
                result *= numbers[i + 1];
            } else if (operators[i] == '|') {
                result = concatenate(result, numbers[i + 1]);
            }
        }
        return result;
    }

    bool can_match(const std::vector<int64_t>& numbers, int64_t target) {
        int ops_needed = numbers.size() - 1;
        int64_t total_combinations = static_cast<int64_t>(pow(3, ops_needed));

        for (int64_t i = 0; i < total_combinations; i++) {
            std::vector<char> operators(ops_needed);
            int64_t temp = i;

            for (int j = 0; j < ops_needed; j++) {
                int op = temp % 3;
                operators[j] = (op == 0) ? '+' : (op == 1) ? '*' : '|';
                temp /= 3;
            }

            if (evaluate(numbers, operators) == target) {
                return true;
            }
        }

        return false;
    }
public:
    explicit Seven(const std::string& input_file_path) : AdventDay(input_file_path) {}
    void parseInput() {
        std::ifstream input_file(input_file_path);
        if (!input_file.is_open()) {
            std::cerr << "Error: Could not open file " << input_file_path << std::endl;
            return;
        }

        std::string line;
        while (std::getline(input_file, line)) {
            if (line.empty()) continue;

            size_t colon = line.find(':');
            if (colon == std::string::npos) continue;

            int64_t target = std::stoll(line.substr(0, colon));

            std::vector<int64_t> numbers;
            std::istringstream iss(line.substr(colon + 1));
            int64_t num;

            while (iss >> num) {
                numbers.push_back(num);
            }
            if (can_match(numbers, target)) {
                sum += target;
            }
        }
    }
    void solve() {
        parseInput();
        std::cout << sum << std::endl;
    }
};

class Eight : public AdventDay {
private:
    struct Point {
        int x, y;

        Point(int x, int y) : x(x), y(y) {}

        bool isCollinearWith(const Point& p2, const Point& p3) const {
            // Using cross product to check collinearity
            return (int64_t)(p2.y - y) * (p3.x - p2.x) ==
                   (int64_t)(p2.x - x) * (p3.y - p2.y);
        }

        std::string toString() const {
            return std::to_string(x) + "," + std::to_string(y);
        }
    };

    std::vector<std::vector<char>> map;

    std::set<std::string> findAntinodes(const std::vector<Point>& antennas) {
        std::set<std::string> antinodes;
        if (antennas.size() < 2) return antinodes;

        for (const Point& antenna : antennas) {
            antinodes.insert(antenna.toString());
        }

        for (size_t x = 0; x < map.size(); x++) {
            for (size_t y = 0; y < map[0].size(); y++) {
                Point p(x, y);

                for (size_t i = 0; i < antennas.size(); i++) {
                    for (size_t j = i + 1; j < antennas.size(); j++) {
                        if (p.isCollinearWith(antennas[i], antennas[j])) {
                            antinodes.insert(p.toString());
                            break;
                        }
                    }
                }
            }
        }

        return antinodes;
    }

    std::string partTwo() {
        std::set<std::string> allAntinodes;
        std::map<char, std::vector<Point>> antennaMap;

        for (size_t i = 0; i < map.size(); i++) {
            for (size_t j = 0; j < map[i].size(); j++) {
                char freq = map[i][j];
                if (freq != '.') {
                    antennaMap[freq].emplace_back(i, j);
                }
            }
        }

        for (const auto& [freq, sameFreqAntennas] : antennaMap) {
            std::set<std::string> frequencyAntinodes = findAntinodes(sameFreqAntennas);
            allAntinodes.insert(frequencyAntinodes.begin(), frequencyAntinodes.end());
        }

        return std::to_string(allAntinodes.size());
    }

public:
    explicit Eight(const std::string& input_file_path) : AdventDay(input_file_path) {}

    void parseInput() override {
        std::ifstream input_file(input_file_path);
        std::string line;

        while (std::getline(input_file, line)) {
            if (!line.empty()) {
                std::vector<char> row(line.begin(), line.end());
                map.push_back(row);
            }
        }
    }

    void solve() override {
        std::cout << "Part Two: " << partTwo() << std::endl;
    }
};

class Nine : public AdventDay {
private:
    std::vector<std::string> input;
    long fileIdx;

    long sumBlocksAndIds(const std::vector<std::string>& blocks) {
        long total = 0;
        for (size_t i = 0; i < blocks.size(); i++) {
            if (blocks[i] != ".") {
                total += i * std::stol(blocks[i]);
            }
        }
        return total;
    }

    std::vector<std::string> moveBlocksV1(const std::vector<std::string>& blocks) {
        std::vector<std::string> copiedBlocks = blocks;
        bool movesMade;

        do {
            movesMade = false;
            int rightmostFile = copiedBlocks.size() - 1;

            while (rightmostFile >= 0 && copiedBlocks[rightmostFile] == ".") {
                rightmostFile--;
            }

            if (rightmostFile >= 0) {
                int leftmostSpace = 0;
                while (leftmostSpace < rightmostFile && copiedBlocks[leftmostSpace] != ".") {
                    leftmostSpace++;
                }

                if (leftmostSpace < rightmostFile && copiedBlocks[leftmostSpace] == ".") {
                    std::string fileId = copiedBlocks[rightmostFile];
                    copiedBlocks[leftmostSpace] = fileId;
                    copiedBlocks[rightmostFile] = ".";
                    movesMade = true;
                }
            }
        } while (movesMade);

        return copiedBlocks;
    }

    std::vector<std::string> moveBlocksV2(const std::vector<std::string>& blocks) {
        std::vector<std::string> copiedBlocks = blocks;
        std::map<int, std::pair<int, int>> fileInfo;
        int maxId = -1;

        for (size_t i = 0; i < copiedBlocks.size(); i++) {
            const std::string& block = copiedBlocks[i];
            if (block != ".") {
                int id = std::stoi(block);
                maxId = std::max(maxId, id);
                if (fileInfo.find(id) == fileInfo.end()) {
                    fileInfo[id] = {i, 1};
                } else {
                    fileInfo[id].second++;
                }
            }
        }

        for (int id = maxId; id >= 0; id--) {
            auto it = fileInfo.find(id);
            if (it == fileInfo.end()) continue;

            int fileStart = it->second.first;
            int fileSize = it->second.second;

            int consecutiveSpace = 0;
            int spaceStart = -1;

            for (int i = 0; i < fileStart; i++) {
                if (copiedBlocks[i] == ".") {
                    if (spaceStart == -1) spaceStart = i;
                    consecutiveSpace++;
                    if (consecutiveSpace >= fileSize) break;
                } else {
                    consecutiveSpace = 0;
                    spaceStart = -1;
                }
            }

            if (consecutiveSpace >= fileSize) {
                std::string idStr = std::to_string(id);
                for (int i = 0; i < fileSize; i++) {
                    copiedBlocks[spaceStart + i] = idStr;
                    copiedBlocks[fileStart + i] = ".";
                }
            }
        }

        return copiedBlocks;
    }

    long partOne() {
        std::vector<std::string> movedBlocks = moveBlocksV1(input);
        return sumBlocksAndIds(movedBlocks);
    }

    long partTwo() {
        std::vector<std::string> movedBlocks = moveBlocksV2(input);
        return sumBlocksAndIds(movedBlocks);
    }

public:
    explicit Nine(const std::string& input_file_path) : AdventDay(input_file_path) {}

    void parseInput() override {
        std::ifstream input_file(input_file_path);
        char c;
        int fileId = 0;
        bool isFile = true;

        while (input_file.get(c)) {
            int size = c - '0';
            if (size >= 0) {
                if (isFile) {
                    for (int i = 0; i < size; i++) {
                        input.push_back(std::to_string(fileId));
                    }
                    fileId++;
                } else {
                    for (int i = 0; i < size; i++) {
                        input.push_back(".");
                    }
                }
                isFile = !isFile;
            }
        }
        fileIdx = fileId - 1;
    }

    void solve() override {
        parseInput();
        std::cout << partOne() << std::endl;
        std::cout << partTwo() << std::endl;
    }
};

class Ten : public AdventDay {
public:
    explicit Ten(const std::string& input_file_path)
        : AdventDay(input_file_path){}

    void parseInput() override
    {
        std::ifstream file(input_file_path);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << input_file_path << std::endl;
            return;
        }

        std::vector<int> line;
        char character;
        while (file.get(character)) {
            if (character == '\n') {
                if (!line.empty()) {
                    input_.push_back(line);
                    line.clear();
                }
                continue;
            }
            int num = character - '0';
            line.push_back(num);
        }
        if (!line.empty()) {
            input_.push_back(line);
        }

        memo_.resize(input_.size(),
            std::vector<std::optional<int>>(input_[0].size(), std::nullopt));
    }

    void solve() override
    {
        parseInput();

        int peakCount = partOne();
        std::cout << peakCount << std::endl;

        int distinctTrails = partTwo();
        std::cout << distinctTrails << std::endl;
    }

private:
    static constexpr int DIRECTIONS[4][2] = {
        { -1,  0 },  // up
        {  0,  1 },  // right
        {  0, -1 },  // left
        {  1,  0 }   // down
    };

    std::vector<std::vector<int>> input_;

    std::vector<std::vector<std::optional<int>>> memo_;

    bool isInBounds(int row, int col) const
    {
        return (row >= 0 && row < static_cast<int>(input_.size())) &&
               (col >= 0 && col < static_cast<int>(input_[0].size()));
    }

    bool isValidNeighbor(int r, int c, int requiredHeight) const
    {
        if (!isInBounds(r, c)) {
            return false;
        }
        return input_[r][c] == (requiredHeight + 1);
    }

    void dfs(int row, int col,
             std::set<std::pair<int,int>>& visited,
             std::set<std::pair<int,int>>& peaks)
    {
        std::pair<int,int> currentPos = { row, col };

        if (visited.find(currentPos) != visited.end()) {
            return;
        }
        visited.insert(currentPos);

        int cellHeight = input_[row][col];
        if (cellHeight == 9) {
            peaks.insert(currentPos);
            return;
        }

        for (auto& direction : DIRECTIONS) {
            int newRow = row + direction[0];
            int newCol = col + direction[1];
            if (isValidNeighbor(newRow, newCol, cellHeight)) {
                dfs(newRow, newCol, visited, peaks);
            }
        }
    }

    std::set<std::pair<int,int>> findReachablePeaks(int startRow, int startCol)
    {
        std::set<std::pair<int,int>> visited;
        std::set<std::pair<int,int>> peaks;

        int startHeight = input_[startRow][startCol];
        if (startHeight != 0) {
            return peaks;
        }

        dfs(startRow, startCol, visited, peaks);
        return peaks;
    }

    int partOne()
    {
        int totalScore = 0;
        for (int row = 0; row < static_cast<int>(input_.size()); ++row) {
            for (int col = 0; col < static_cast<int>(input_[row].size()); ++col) {
                if (input_[row][col] == 0) {
                    std::set<std::pair<int,int>> peaks = findReachablePeaks(row, col);
                    totalScore += static_cast<int>(peaks.size());
                }
            }
        }
        return totalScore;
    }

    int countAllPaths(int row, int col)
    {
        if (memo_[row][col].has_value()) {
            return memo_[row][col].value();
        }

        int currHeight = input_[row][col];
        if (currHeight == 9) {
            memo_[row][col] = 1;
            return 1;
        }

        int totalPathsFromHere = 0;
        for (auto& direction : DIRECTIONS) {
            int newRow = row + direction[0];
            int newCol = col + direction[1];

            if (isInBounds(newRow, newCol) &&
                input_[newRow][newCol] == (currHeight + 1)) {
                totalPathsFromHere += countAllPaths(newRow, newCol);
            }
        }

        memo_[row][col] = totalPathsFromHere;
        return totalPathsFromHere;
    }

    int partTwo()
    {
        int totalRating = 0;
        for (int row = 0; row < static_cast<int>(input_.size()); ++row) {
            for (int col = 0; col < static_cast<int>(input_[row].size()); ++col) {
                if (input_[row][col] == 0) {
                    totalRating += countAllPaths(row, col);
                }
            }
        }
        return totalRating;
    }
};


class Eleven : public AdventDay {
public:
    explicit Eleven(const std::string& input_file_path)
        : AdventDay(input_file_path)
    {
    }

    void parseInput() override
    {
        std::map<std::string, long long> distribution;

        std::ifstream file(input_file_path);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << input_file_path << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (trim(line).empty()) {
                continue;
            }
            std::istringstream iss(line);
            std::string token;
            while (iss >> token) {
                distribution[token] += 1;
            }
        }

        input_ = distribution;
    }

    void solve() override
    {
        parseInput();

        long long partOne = simulateBlinks(25, false);
        std::cout << "Part One: " << partOne << std::endl;
        long long partOneDebug = simulateBlinks(25, true);
        std::cout << "Part One Debugging: " << partOneDebug << std::endl;

        long long partTwo = simulateBlinks(75, false);
        std::cout << "Part Two: " << partTwo << std::endl;
    }

private:
    std::map<std::string, long long> input_;

    static std::string trim(const std::string& s)
    {
        size_t start = 0;
        while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
            start++;
        }
        size_t end = s.size();
        while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
            end--;
        }
        return s.substr(start, end - start);
    }

    std::vector<std::string> transformStone(const std::string& stone)
    {
        if (stone == "0") {
            return { "1" };
        }

        if (stone.size() % 2 == 0) {
            int mid = static_cast<int>(stone.size()) / 2;
            std::string left  = stone.substr(0, mid);
            std::string right = stone.substr(mid);
            return { left, right };
        }

        std::string multiplied = multiplyStringByInt(stone, 2024);
        return { multiplied };
    }

    std::string multiplyStringByInt(const std::string& s, int multiplier)
    {
        if (s == "0" || multiplier == 0) {
            return "0";
        }

        int carry = 0;
        std::string result;
        result.reserve(s.size() + 10);

        for (int i = static_cast<int>(s.size()) - 1; i >= 0; i--) {
            int digit = s[i] - '0';
            long long prod = static_cast<long long>(digit) * multiplier + carry;
            int newDigit = static_cast<int>(prod % 10);
            carry        = static_cast<int>(prod / 10);
            result.push_back(static_cast<char>('0' + newDigit));
        }

        while (carry > 0) {
            int newDigit = carry % 10;
            result.push_back(static_cast<char>('0' + newDigit));
            carry /= 10;
        }

        std::reverse(result.begin(), result.end());
        return result;
    }

    long long simulateBlinks(int numBlinks, bool debug)
    {
        std::map<std::string, long long> dist = input_;

        for (int i = 0; i < numBlinks; i++) {
            std::map<std::string, long long> newDist;

            // For each (stone -> count) in dist, transform the stone
            for (auto& [stoneValue, count] : dist) {
                // transformStone returns 1 or 2 new "stones" (as strings)
                std::vector<std::string> newStones = transformStone(stoneValue);

                // Merge them into newDist
                for (auto& ns : newStones) {
                    newDist[ns] += count;
                }
            }

            dist = std::move(newDist);

            if (debug && ((i + 1) % 5 == 0)) {
                long long totalStones = 0;
                for (auto& kv : dist) {
                    totalStones += kv.second;
                }
                std::cout << "After " << (i + 1) << " blinks: "
                          << totalStones << " stones" << std::endl;
            }
        }

        long long total = 0;
        for (auto& kv : dist) {
            total += kv.second;
        }
        return total;
    }
};

class Twelve : public AdventDay {
public:
    explicit Twelve(const std::string& input_file_path) : AdventDay(input_file_path) {}
    void parseInput() override {}
    void solve() override {}
};

class Thirteen : public AdventDay {
public:
    explicit Thirteen(const std::string& input_file_path) : AdventDay(input_file_path) {}
    void parseInput() override {}
    void solve() override {}
};

class Fourteen : public AdventDay {
public:
    explicit Fourteen(const std::string& input_file_path) : AdventDay(input_file_path) {}
    void parseInput() override {}
    void solve() override {}
};

class Fifteen : public AdventDay {
public:
    explicit Fifteen(const std::string& input_file_path) : AdventDay(input_file_path) {}
    void parseInput() override {}
    void solve() override {}
};

class Sixteen : public AdventDay {
public:
    explicit Sixteen(const std::string& input_file_path) : AdventDay(input_file_path) {}
    void parseInput() override {}
    void solve() override {}
};

class Seventeen : public AdventDay {
public:
    explicit Seventeen(const std::string& input_file_path) : AdventDay(input_file_path) {}
    void parseInput() override {}
    void solve() override {}
};

class Eighteen : public AdventDay {
public:
    explicit Eighteen(const std::string& input_file_path) : AdventDay(input_file_path) {}
    void parseInput() override {}
    void solve() override {}
};

class Nineteen : public AdventDay {
public:
    explicit Nineteen(const std::string& input_file_path) : AdventDay(input_file_path) {}
    void parseInput() override {}
    void solve() override {}
};

class Twenty : public AdventDay {
private:
    struct Position {
        int row, col;

        Position(int r, int c) : row(r), col(c) {}

        bool operator==(const Position& other) const {
            return row == other.row && col == other.col;
        }
    };

    struct PositionHash {
        std::size_t operator()(const Position& pos) const {
            return std::hash<int>()(pos.row) ^ (std::hash<int>()(pos.col) << 1);
        }
    };

    struct CheatKey {
        Position start, end;

        CheatKey(const Position& s, const Position& e) : start(s), end(e) {}

        bool operator==(const CheatKey& other) const {
            return start == other.start && end == other.end;
        }
    };

    struct CheatKeyHash {
        std::size_t operator()(const CheatKey& key) const {
            PositionHash posHash;
            return posHash(key.start) ^ (posHash(key.end) << 1);
        }
    };

    std::vector<std::string> grid;
    Position start{0, 0};
    Position end{0, 0};
    int rows{0}, cols{0};

    using DistanceMap = std::unordered_map<Position, int, PositionHash>;
    using CheatSet = std::unordered_set<CheatKey, CheatKeyHash>;

    void parseInput() override {
        std::ifstream input_file(input_file_path);
        std::string line;
        while (std::getline(input_file, line)) {
            grid.push_back(line);
        }
        rows = grid.size();
        cols = grid[0].size();

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (grid[i][j] == 'S') {
                    start = Position(i, j);
                } else if (grid[i][j] == 'E') {
                    end = Position(i, j);
                }
            }
        }
    }

    DistanceMap computeDistances(const Position& pos) const {
        DistanceMap distances;
        std::queue<Position> q;

        distances[pos] = 0;
        q.push(pos);

        const std::vector<std::pair<int, int>> dirs = {{1,0}, {-1,0}, {0,1}, {0,-1}};

        while (!q.empty()) {
            Position current = q.front();
            q.pop();

            for (const auto& [dr, dc] : dirs) {
                Position next(current.row + dr, current.col + dc);

                if (next.row >= 0 && next.row < rows &&
                    next.col >= 0 && next.col < cols &&
                    grid[next.row][next.col] != '#' &&
                    !distances.count(next)) {

                    distances[next] = distances[current] + 1;
                    q.push(next);
                }
            }
        }

        return distances;
    }

    int findCheats(const DistanceMap& startDists, const DistanceMap& endDists,
                   int normalDist, int maxCheatLen, int minSaving) const {
        CheatSet uniqueCheats;

        for (const auto& [startPos, distToStart] : startDists) {
            for (int r = std::max(0, startPos.row - maxCheatLen);
                 r < std::min(rows, startPos.row + maxCheatLen + 1); ++r) {
                for (int c = std::max(0, startPos.col - maxCheatLen);
                     c < std::min(cols, startPos.col + maxCheatLen + 1); ++c) {

                    Position endPos(r, c);
                    if (!endDists.count(endPos)) continue;

                    int manhattanDist = std::abs(r - startPos.row) +
                                      std::abs(c - startPos.col);
                    if (manhattanDist > maxCheatLen) continue;

                    int totalDist = distToStart + manhattanDist + endDists.at(endPos);
                    if (normalDist - totalDist >= minSaving) {
                        uniqueCheats.insert(CheatKey(startPos, endPos));
                    }
                }
            }
        }

        return uniqueCheats.size();
    }

public:
    explicit Twenty(const std::string& inputFilePath)
        : AdventDay(inputFilePath) {}

    void solve() override {
        parseInput();
        DistanceMap startDistances = computeDistances(start);
        DistanceMap endDistances = computeDistances(end);

        if (!startDistances.count(end)) {
            std::cout << "No path exists!" << std::endl;
            return;
        }

        int normalDist = startDistances[end];

        int part1 = findCheats(startDistances, endDistances, normalDist, 2, 100);

        int part2 = findCheats(startDistances, endDistances, normalDist, 20, 100);

        std::cout << "Part 1: " << part1 << std::endl;
        std::cout << "Part 2: " << part2 << std::endl;
    }
};

class TwentyOne: public AdventDay {
public:
    explicit TwentyOne(const std::string& input_file_path) : AdventDay(input_file_path) {}
    void parseInput() override {}
    void solve() override {}
};

class TwentyTwo : public AdventDay {
public:
    explicit TwentyTwo(const std::string& input_file_path) : AdventDay(input_file_path) {}
    void parseInput() override {}
    void solve() override {}
};

class TwentyThree : public AdventDay {
public:
    explicit TwentyThree(const std::string& input_file_path) : AdventDay(input_file_path) {}
    void parseInput() override {}
    void solve() override {}
};

class TwentyFour : public AdventDay {
public:
    explicit TwentyFour(const std::string& input_file_path) : AdventDay(input_file_path) {}
    void parseInput() override {}
    void solve() override {}
};

class TwentyFive : public AdventDay {
public:
    explicit TwentyFive(const std::string& input_file_path) : AdventDay(input_file_path) {}
    void parseInput() override {}
    void solve() override {}
};

int main() {
    try {
        std::unique_ptr<AdventDay> one = std::make_unique<One>("../day-one.txt");
        one->solve();
        std::unique_ptr<AdventDay> two = std::make_unique<Two>("../day-two.txt");
        two->solve();
        std::unique_ptr<AdventDay> three = std::make_unique<Three>("../day-three.txt");
        three->solve();
        std::unique_ptr<AdventDay> four = std::make_unique<Four>("../day-four.txt");
        four->solve();
        std::unique_ptr<AdventDay> five = std::make_unique<Five>("../day-five.txt");
        five->solve();
        std::unique_ptr<AdventDay> six = std::make_unique<Six>("../day-six.txt");
        six->solve();
        std::unique_ptr<AdventDay> seven = std::make_unique<Seven>("../day-seven.txt");
        seven->solve();
        std::unique_ptr<AdventDay> eight = std::make_unique<Eight>("../day-eight.txt");
        eight->solve();
        std::unique_ptr<AdventDay> nine = std::make_unique<Nine>("../day-nine.txt");
        nine->solve();
        std::unique_ptr<AdventDay> ten = std::make_unique<Ten>("../day-ten.txt");
        ten->solve();
        std::unique_ptr<AdventDay> eleven = std::make_unique<Eleven>("../day-eleven.txt");
        eleven->solve();

        // unimplemented
        // std::unique_ptr<AdventDay> twelve = std::make_unique<Twelve>("../day-12.txt");
        // twelve->solve();
        // std::unique_ptr<AdventDay> thirteen = std::make_unique<Thirteen>("../day-13.txt");
        // thirteen->solve();
        // std::unique_ptr<AdventDay> fourteen = std::make_unique<Fourteen>("../day-14.txt");
        // fourteen->solve();
        // std::unique_ptr<AdventDay> fifteen = std::make_unique<Fifteen>("../day-15.txt");
        // Fifteen->solve();
        // std::unique_ptr<AdventDay> sixteen = std::make_unique<Sixteen>("../day-16.txt");
        // sixteen->solve();
        // std::unique_ptr<AdventDay> seventeen = std::make_unique<Seventeen>("../day-17.txt");
        // seventeen->solve();
        // std::unique_ptr<AdventDay> eighteen = std::make_unique<Eighteen>("../day-18.txt");
        // eighteen->solve();
        // std::unique_ptr<AdventDay> nineteen = std::make_unique<Nineteen>("../day-19.txt");
        // nineteen->solve();

        std::unique_ptr<AdventDay> twenty = std::make_unique<Twenty>("../day-20.txt");
        twenty->solve();

        // unimplemented
        // std::unique_ptr<AdventDay> twenty_one = std::make_unique<TwentyOne>("../day-21.txt");
        // twenty_one->solve();
        // std::unique_ptr<AdventDay> twenty_two = std::make_unique<TwentyTwo>("../day-22.txt");
        // twenty_two->solve();
        // std::unique_ptr<AdventDay> twenty_three = std::make_unique<TwentyThree>("../day-23.txt");
        // twenty_three->solve();
        // std::unique_ptr<AdventDay> twenty_four = std::make_unique<TwentyFour>("../day-24.txt");
        // twenty_four->solve();
        // std::unique_ptr<AdventDay> twenty_five = std::make_unique<TwentyFive>("../day-25.txt");
        // twenty_five->solve();
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}

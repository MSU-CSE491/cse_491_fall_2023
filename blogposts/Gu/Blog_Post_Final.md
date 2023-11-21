# ChatGPT Dynamic Programming Solution Analysis 

## Synopsis:

ChatGPT is a powerful tool that allows users to prompt a LLM (Large Language Model) about any questions that they may have. One way this AI is used in the Computer Science sector to produce solutions to various algoirhtmic problems. One category of problems that ChatGPT is still not proficient in generating solutions to is DP (Dynamic Programming). This blog post will analyze basic to advanced DP problems and analyze ChatGPT's solutions to them. 

## House Robbers:

The house robbers problem is a simple DP problem where you have to find the maximum sum of non-adjacent elements of a list. So, for example, if you have a list that contains [2, 10, 12, 5, 20], then the maximum sum will be 34. 

### ChatGPT's Solution

    #include <vector>
    #include <algorithm>
    
    int rob(std::vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        if (n == 1) return nums[0];
    
        std::vector<int> max_money(n, 0);
    
        // Base cases
        max_money[0] = nums[0];
        max_money[1] = std::max(nums[0], nums[1]);
    
        // Fill the array using a bottom-up approach
        for (int i = 2; i < n; ++i) {
            max_money[i] = std::max(max_money[i - 1], max_money[i - 2] + nums[i]);
        }
    
        // The maximum money that can be robbed is in the last cell of the array
        return max_money[n - 1];
    }
    
    int main() {
        std::vector<int> nums = {1, 2, 3, 1};
        int result = rob(nums);
        std::cout << "Maximum amount of money that can be robbed: " << result << std::endl;  // Output: 4
        return 0;
    }
    
House Robbers is famous for being the most simple DP problem and that's why I'm not surprised that ChatGPT's solution to it is completely correct. The base cases cover everything necessary and the list plus the loop calculate the max at each index of the input, leaving the maximum sum at the last index of the list. Since a problem this simple is easy for ChatGPT to solve, let's move on to a more difficult problem. 

## Edit Distance

Given two strings word1 and word2, return the minimum number of operations required to convert word1 to word2. You have the following three operations permitted on a word:

Insert a character
Delete a character
Replace a character

Example 1:

Input: word1 = "horse", word2 = "ros"
Output: 3
Explanation: 
horse -> rorse (replace 'h' with 'r')
rorse -> rose (remove 'r')
rose -> ros (remove 'e')

### ChatGPT's Solution
    
    #include <iostream>
    #include <vector>
    #include <algorithm>
    
    int minEditDistance(std::string word1, std::string word2) {
        int m = word1.length();
        int n = word2.length();
    
        // Initialize a 2D vector to store the minimum edit distances
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    
        // Base cases: cost of converting an empty string to a non-empty string
        for (int i = 0; i <= m; i++) {
            dp[i][0] = i;
        }
    
        for (int j = 0; j <= n; j++) {
            dp[0][j] = j;
        }
    
        // Fill the matrix using dynamic programming
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (word1[i - 1] == word2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                } else {
                    dp[i][j] = 1 + std::min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
                }
            }
        }
    
        return dp[m][n];
    }
    
    int main() {
        std::string word1 = "horse";
        std::string word2 = "ros";
        int result = minEditDistance(word1, word2);
        std::cout << "Minimum number of operations: " << result << std::endl;  // Output: 3
        return 0;
    }
    
This problem is a harder one that requires a 2D array that saves the edit distance from a substring of length i of word1 and a substring of length j of word2. I am not extremely surprised that ChatGPT was able to generate this one correctly, but I wasn't expecting it to be the optimal solution. The code does exactly what the solution requires, filling the 2D array with a decision between the four cases: the two letters being the same, in which case you just put the value at i - 1 and j - 1, and the insert, delete, and replace case where you take the minimum of 1 + i - 1 and j, 1 + i and j - 1, and 1 + i - 1 and j - 1. 

## Wildcard Matching

Given an input string (s) and a pattern (p), implement wildcard pattern matching with support for '?' and '*' where:

'?' Matches any single character.
'*' Matches any sequence of characters (including the empty sequence).
The matching should cover the entire input string (not partial).

Example 1:

Input: s = "aa", p = "a"
Output: false
Explanation: "a" does not match the entire string "aa".

### ChatGPT's Solution

    #include <iostream>
    #include <vector>
    
    bool isMatch(std::string s, std::string p) {
        int m = s.length();
        int n = p.length();
    
        // Initialize a 2D DP vector with false values
        std::vector<std::vector<bool>> dp(m + 1, std::vector<bool>(n + 1, false));
        dp[0][0] = true;
    
        // Fill the DP vector
        for (int j = 1; j <= n; j++) {
            if (p[j - 1] == '*') {
                dp[0][j] = dp[0][j - 1];
            }
        }
    
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (p[j - 1] == '*') {
                    dp[i][j] = dp[i - 1][j] || dp[i][j - 1];
                } else if (p[j - 1] == '?' || p[j - 1] == s[i - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                }
            }
        }
    
        return dp[m][n];
    }
    
    int main() {
        std::string s1 = "aa";
        std::string p1 = "a";
        std::cout << "Output: " << (isMatch(s1, p1) ? "True" : "False") << std::endl;  // Output: False
    
        std::string s2 = "adceb";
        std::string p2 = "*a*b";
        std::cout << "Output: " << (isMatch(s2, p2) ? "True" : "False") << std::endl;  // Output: True
    
        return 0;
    }


This problem is quite harder than the first two that I examined before, however, ChatGPT is still able to produce the most efficient solution. I am suspecting that this is because I am pulling the coding prompts from Leetcode. For the next few, I will stump ChatGPT and analyze where its solution went wrong and how to fix it. 

## Beautiful Numbers

You are given positive integers low, high, and k.

A number is beautiful if it meets both of the following conditions:

The count of even digits in the number is equal to the count of odd digits.
The number is divisible by k.
Return the number of beautiful integers in the range [low, high].

### ChatGPT's Solution

    #include <iostream>
    
    bool isBeautiful(int num) {
        int even_count = 0, odd_count = 0;
        while (num > 0) {
            int digit = num % 10;
            if (digit % 2 == 0) {
                even_count += 1;
            } else {
                odd_count += 1;
            }
            num /= 10;
        }
        return even_count == odd_count;
    }
    
    int countBeautiful(int low, int high, int k) {
        int beautiful_count = 0;
    
        for (int num = low; num <= high; num++) {
            if (num % k == 0 && isBeautiful(num)) {
                beautiful_count += 1;
            }
        }
    
        return beautiful_count;
    }
    
    int main() {
        int low = 10;
        int high = 20;
        int k = 3;
        int result = countBeautiful(low, high, k);
        std::cout << "Output: " << result << std::endl;  // Output: 2
        return 0;
    }


It seems like ChatGPT was stumped with this problem because the solution it produced is O(N), which isn't the most efficient solution. The most efficient solution requires very complicated DP. As for the ChatGPT solution, it is syntactically and logically accurate, but it is just the naive solution. So, I asked ChatGPT to give me a more efficient solution. 

### ChatGPT's Solution "More Optimized"

    #include <iostream>
    
    std::pair<int, int> countDigits(int num) {
        int even_count = 0, odd_count = 0;
        while (num > 0) {
            int digit = num % 10;
            if (digit % 2 == 0) {
                even_count += 1;
            } else {
                odd_count += 1;
            }
            num /= 10;
        }
        return std::make_pair(even_count, odd_count);
    }
    
    int countBeautiful(int low, int high, int k) {
        int beautiful_count = 0;
    
        // Iterate through numbers with equal even and odd digits
        for (int num = low; num <= high; num++) {
            std::pair<int, int> counts = countDigits(num);
            int even_count = counts.first;
            int odd_count = counts.second;
    
            if (even_count == odd_count && num % k == 0) {
                beautiful_count += 1;
            }
        }
    
        return beautiful_count;
    }
    
    int main() {
        int low = 10;
        int high = 20;
        int k = 3;
        int result = countBeautiful(low, high, k);
        std::cout << "Output: " << result << std::endl;  // Output: 2
        return 0;
    }


This is the solution produced by ChatGPT when I asked it for a more efficient solution. It claims that this solution reduces the search space, however, the for loop still runs for O(N) time. The only change of the function was to return the number of odd and even digits instead of returning a boolean, however, these numbers are used in an and statement to produce a boolean anyways, so this solution actually requires more computations than the one ChatGPT gave before. Both the syntax and logic are correct again, but it is obvious that ChatGPT does not automatically know about the DP solution when not prompted about it. 

## Count of Non-Consecutive Ones in Binary Range

Given two positive integers ‘L’ and ‘R‘, the task is to return the number of the integers in the range [L, R] inclusive, whose binary representations do not contain consecutive ones where 0 ≤ l ≤ r ≤ 10^9.

### ChatGPT's Solution

    #include <iostream>
    #include <vector>
    
    using namespace std;
    
    int countNonConsecutiveOnes(int L, int R) {
        int count = 0;
    
        for (int i = L; i <= R; ++i) {
            bool consecutiveOnes = false;
            int num = i;
    
            while (num > 0) {
                int lastBit = num & 1;
                num >>= 1;
                
                if (lastBit == 1 && (num & 1) == 1) {
                    consecutiveOnes = true;
                    break;
                }
            }
    
            if (!consecutiveOnes) {
                count++;
            }
        }
    
        return count;
    }
    
    int main() {
        // Example usage
        int L = 10;
        int R = 20;
    
        int result = countNonConsecutiveOnes(L, R);
    
        cout << "Number of integers with non-consecutive ones in binary representation: " << result << endl;
    
        return 0;
    }


The code that ChatGPT generates solves the problem, however, it is quite inefficient when compared with the most efficient solution. The most optimized solution for this problem uses a 3D array to save answers seen before, which makes the time complexity close to linear. For the ChatGPT solution, there is no use of any additional memory to store previous answers, so because of this, ChatGPT's solution has to search through the whole answer space for each number, which leads to a runtime closer to exponential than linear. 

### ChatGPT's More Optimal Solution

    #include <iostream>
    
    using namespace std;
    
    int countNonConsecutiveOnes(int L, int R) {
        auto getBit = [](int num, int pos) -> int {
            return (num >> pos) & 1;
        };
    
        int count = 0;
    
        for (int bitPos = 30; bitPos >= 0; --bitPos) {
            int bitL = getBit(L, bitPos);
            int bitR = getBit(R, bitPos);
    
            if (bitL == 1 && bitR == 1) {
                // If both bits are 1, no more valid numbers for this bit position
                break;
            }
    
            if (bitL == 0 && bitR == 0) {
                // If both bits are 0, the count remains the same
            } else if (bitL == 0 && bitR == 1) {
                // If transitioning from 0 to 1, valid numbers for this bit position are counted from both states
                count += (1 << bitPos);
            } else if (bitL == 1 && bitR == 0) {
                // If transitioning from 1 to 0, no more valid numbers for this bit position
                break;
            }
        }
    
        return count;
    }
    
    int main() {
        // Example usage
        int L = 10;
        int R = 20;
    
        int result = countNonConsecutiveOnes(L, R);
    
        cout << "Number of integers with non-consecutive ones in binary representation: " << result << endl;
    
        return 0;
    }

This solution is not that much more efficient than the last, it now just uses more space to keep track of numbers that end with 0 and numbers that end with 1. That is not the right intuition to approach this problem with DP. It is also interesting how ChatGPT states that it uses DP in the generated output, however, there is no DP readily apparent in the solution. It seems as if ChatGPT does not know what path to go down in order to approach this problem. I am not surprised as a DP problem requiring a 3D array is one that I did not suspect AI to be able to solve yet. 

## Probability of Winning In a Die-Throw Game

Given that 2 players are playing a die-throw game. The game is one player throws a die and he got a point, he can move forward accordingly to the point. All occurrences to get the point have equal probability. Let player1 start a point x and player2 start a point y. Player 1 can receive a point up to A, and Player 2 can receive a point up to B.  When a player in the position p and point got in the die q, he can move forward a minimum of (p+q) and W. The first player to reach W wins the game. Find the probability modulo that player 1 wins if he goes first modulo 998244353.

### ChatGPT's Solution

    #include <iostream>
    #include <vector>
    
    const int MOD = 998244353;
    
    int calculateProbability(int x, int y, int A, int B, int W, std::vector<std::vector<int>>& dp) {
        if (x >= W) return 1;
        if (y >= W) return 0;
        if (dp[x][y] != -1) return dp[x][y];
    
        long long totalWays = 0;
        for (int q = 1; q <= A; ++q) {
            if (x + q < W) {
                totalWays = (totalWays + calculateProbability(y, x + q, B, A, W, dp)) % MOD;
            }
        }
    
        return dp[x][y] = totalWays;
    }
    
    int main() {
        int x, y, A, B, W;
    
        // Example input values
        x = 1;
        y = 1;
        A = 2;
        B = 2;
        W = 5;
    
        // Create a memoization table
        std::vector<std::vector<int>> dp(W + 1, std::vector<int>(W + 1, -1));
    
        int result = calculateProbability(x, y, A, B, W, dp);
    
        std::cout << "Probability of player 1 winning: " << result << std::endl;
    
        return 0;
    }

This solution is unique because it uses a 2D array when a 3D array is required. It is not taking into consideration finding a dimension of possible solutions that are required to find the optimal solution, which is the probability of winning. To be frank, I would say this is a problem that ChatGPT itself was confused on how to approach. The time complexity for this solution is exponential, while the most efficient solution is one that is in the range of quadratic runtime. 

### ChatGPT's More Optimized Solution

    #include <iostream>
    #include <vector>
    
    const int MOD = 998244353;
    
    int calculateProbability(int x, int y, int A, int B, int W, std::vector<std::vector<int>>& dp) {
        // Base case: if player 1 is already at or beyond W, he wins
        if (x >= W) return 1;
    
        // If player 2 is already at or beyond W, player 1 cannot win from this state
        if (y >= W) return 0;
    
        // If the probability is already calculated, return it
        if (dp[x][y] != -1) return dp[x][y];
    
        long long totalWays = 0;
    
        // Iterate through all possible outcomes of player 1's throw
        for (int q = 1; q <= A; ++q) {
            if (x + q < W) {
                // If player 1's throw is valid, recursively calculate the probability
                totalWays = (totalWays + calculateProbability(y, x + q, B, A, W, dp)) % MOD;
            }
        }
    
        // Store the calculated probability in the memoization table
        return dp[x][y] = totalWays;
    }
    
    int main() {
        int x, y, A, B, W;
    
        // Example input values
        x = 1;
        y = 1;
        A = 2;
        B = 2;
        W = 5;
    
        // Create a memoization table
        std::vector<std::vector<int>> dp(W + 1, std::vector<int>(W + 1, -1));
    
        int result = calculateProbability(x, y, A, B, W, dp);
    
        std::cout << "Probability of player 1 winning: " << result << std::endl;
    
        return 0;
    }

I asked ChatGPT to generate a more efficient solution and it provided me with the same solution again, but with an explanation with how it's different. It claims that it now iterates through the states in reverse order, but there is nothing in the code that reflects that. In addition, it states that it uses a top-down dynamic programming approach, but the solution is the same as what was provided before. It seems as if ChatGPT is completely stumped on this problem. 

---

Link to the Chat: https://chat.openai.com/share/bc30b6b9-ee73-40ae-85fe-06df74f10b10

The first half contains Python solutions that I then converted to C++ and the second half contains the solutions directly generated as C++. 

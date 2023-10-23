# ChatGPT Dynamic Programming Solution Analysis 

## Synopsis:

ChatGPT is a powerful tool that allows users to prompt a LLM (Large Language Model) about any questions that they may have. One way this AI is used in the Computer Science sector to produce solutions to various algoirhtmic problems. One category of problems that ChatGPT is still not proficient in generating solutions to is DP (Dynamic Programming). This blog post will analyze basic to advanced DP problems and analyze ChatGPT's solutions to them. 

## House Robbers:

The house robeers problem is a simple DP problem where you have to find the maximum sum of non-adjacent elements of a list. So, for example, if you have a list that contains [2, 10, 12, 5, 20], then the maximum sum will be 34. 

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

## Add Another Example that Stumps ChatGPT

Place Review Here

---

Link to the Chat: https://chat.openai.com/share/5f6bd9fb-4934-4031-9e03-1c4276eb3fbb

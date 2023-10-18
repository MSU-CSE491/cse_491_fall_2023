# ChatGPT Dynamic Programming Solution Analysis 

# Link to the Chat: https://chat.openai.com/share/5f6bd9fb-4934-4031-9e03-1c4276eb3fbb

## Synopsis:

ChatGPT is a powerful tool that allows users to prompt a LLM (Large Language Model) about any questions that they may have. One way this AI is used in the Computer Science sector to produce solutions to various algoirhtmic problems. One category of problems that ChatGPT is still not proficient in generating solutions to is DP (Dynamic Programming). This blog post will analyze basic to advanced DP problems and analyze ChatGPT's solutions to them. 

## House Robbers:

The house robeers problem is a simple DP problem where you have to find the maximum sum of non-adjacent elements of a list. So, for example, if you have a list that contains [2, 10, 12, 5, 20], then the maximum sum will be 34. 

### ChatGPT's Solution

    def rob(nums):
        if not nums:
            return 0
    
        if len(nums) == 1:
            return nums[0]
    
        # Initialize an array to store the maximum money that can be robbed
        # at each house.
        max_money = [0] * len(nums)
    
        # Base cases
        max_money[0] = nums[0]
        max_money[1] = max(nums[0], nums[1])
    
        # Fill the array using a bottom-up approach
        for i in range(2, len(nums)):
            max_money[i] = max(max_money[i - 1], max_money[i - 2] + nums[i])
    
        # The maximum money that can be robbed is in the last cell of the array
        return max_money[-1]

    - Example usage
    nums = [1, 2, 3, 1]  # Houses with money in each
    print("Maximum amount of money that can be robbed:", rob(nums))  # Output: 4

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

    def min_edit_distance(word1, word2):
        m, n = len(word1), len(word2)
    
        # Initialize a matrix to store the minimum edit distances
        dp = [[0] * (n + 1) for _ in range(m + 1)]
    
        # Base cases: cost of converting an empty string to a non-empty string
        for i in range(m + 1):
            dp[i][0] = i
    
        for j in range(n + 1):
            dp[0][j] = j
    
        # Fill the matrix using dynamic programming
        for i in range(1, m + 1):
            for j in range(1, n + 1):
                if word1[i - 1] == word2[j - 1]:
                    dp[i][j] = dp[i - 1][j - 1]
                else:
                    dp[i][j] = 1 + min(dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1])
    
        return dp[m][n]
    
    # Example usage
    word1 = "horse"
    word2 = "ros"
    print("Minimum number of operations:", min_edit_distance(word1, word2))  # Output: 3

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

    def isMatch(s, p):
        m, n = len(s), len(p)
    
        # Initialize a 2D DP array with False
        dp = [[False] * (n + 1) for _ in range(m + 1)]
        dp[0][0] = True
    
        # Fill the DP array
        for j in range(1, n + 1):
            if p[j - 1] == '*':
                dp[0][j] = dp[0][j - 1]
    
        for i in range(1, m + 1):
            for j in range(1, n + 1):
                if p[j - 1] == '*':
                    dp[i][j] = dp[i - 1][j] or dp[i][j - 1]
                elif p[j - 1] == '?' or p[j - 1] == s[i - 1]:
                    dp[i][j] = dp[i - 1][j - 1]
    
        return dp[m][n]
    
    # Example usage
    s1 = "aa"
    p1 = "a"
    print(isMatch(s1, p1))  # Output: False
    
    s2 = "adceb"
    p2 = "*a*b"
    print(isMatch(s2, p2))  # Output: True

This problem is quite harder than the first two that I examined before, however, ChatGPT is still able to produce the most efficient solution. I am suspecting that this is because I am pulling the coding prompts from Leetcode. For the next few, I will stump ChatGPT and analyze where its solution went wrong and how to fix it. 

## Beautiful Numbers

You are given positive integers low, high, and k.

A number is beautiful if it meets both of the following conditions:

The count of even digits in the number is equal to the count of odd digits.
The number is divisible by k.
Return the number of beautiful integers in the range [low, high].

### ChatGPT's Solution

    def countBeautiful(low, high, k):
        def isBeautiful(num):
            # Function to check if a number is beautiful
            even_count = odd_count = 0
            while num > 0:
                digit = num % 10
                if digit % 2 == 0:
                    even_count += 1
                else:
                    odd_count += 1
                num //= 10
    
            return even_count == odd_count
    
        beautiful_count = 0
    
        for num in range(low, high + 1):
            if num % k == 0 and isBeautiful(num):
                beautiful_count += 1
    
        return beautiful_count
    
    # Example usage
    low = 10
    high = 20
    k = 3
    print(countBeautiful(low, high, k))  # Output: 2

It seems like ChatGPT was stumped with this problem because the solution it produced is O(N), which isn't the most efficient solution. The most efficient solution requires very complicated DP. As for the ChatGPT solution, it is syntactically and logically accurate, but it is just the naive solution. So, I asked ChatGPT to give me a more efficient solution. 

### ChatGPT's Solution "More Optimized"

    def countBeautiful(low, high, k):
        def countDigits(num):
            even_count = odd_count = 0
            while num > 0:
                digit = num % 10
                if digit % 2 == 0:
                    even_count += 1
                else:
                    odd_count += 1
                num //= 10
            return even_count, odd_count
    
        beautiful_count = 0
    
        # Iterate through numbers with equal even and odd digits
        for num in range(low, high + 1):
            even_count, odd_count = countDigits(num)
            if even_count == odd_count and num % k == 0:
                beautiful_count += 1
    
        return beautiful_count
    
    # Example usage
    low = 10
    high = 20
    k = 3
    print(countBeautiful(low, high, k))  # Output: 2

This is the solution produced by ChatGPT when I asked it for a more efficient solution. It claims that this solution reduces the search space, however, the for loop still runs for O(N) time. The only change of the function was to return the number of odd and even digits instead of returning a boolean, however, these numbers are used in an and statement to produce a boolean anyways, so this solution actually requires more computations than the one ChatGPT gave before. Both the syntax and logic are correct again, but it is obvious that ChatGPT does not automatically know about the DP solution when not prompted about it. 

## Add Another Example that Stumps ChatGPT

Place Review Here

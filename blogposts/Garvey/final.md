# Using AI to Solve AI
### Lauren Garvey, November 2023
##### For CSE 491 at MSU

---
In a previous semester, I took a course on introduction to AI. Rather than using an AI tool, we were given projects that were more about the math behind decision-making, all set against the backdrop of trying to guide Pacman to complete certain goals in a given world. The first project was about basic search algorithms and some simple alternative goals, such as hitting all four corners of the map or eating every food pellet. The second was on optimization, specifically with linear and integer programming as a means to look at feasible intersection points, constraints, and cost vectors. The third was about reinforcement learning, where variations on rewards and moves were utilized in getting Pacman to the best outcome. The fourth and final project was on Bayesian networks, where Pacman sensed the world around him to locate and chase invisible ghosts. All of these projects were developed at UC Berkeley, primarily by John DeNero and Dan Klein[^1].

Having now completed the course, I was curious about how an AI would fare against solving these AI problems. I specifically wanted to have ChatGPT solve one of these projects, so I could then compare it to my own solutions. I wasn't terribly interested in the first project, since the functions included searches like breadth-first, depth-first, A*, and uniform cost search, all of which are fairly common and not likely to be interesting if I asked ChatGPT to implement them. The second project involved a decent amount of problem formulation based on a given scenario, which seemed like it would be more focused on getting ChatGPT to come up with a relevant problem, instead of solving one. This left reinforcement learning and Bayesian networks, and I frankly think the former would be more exciting to solve with ChatGPT.

Since this project is still in use for the course, I won't go into the specifics of how ChatGPT solved it, but rather deconstruct what worked and what didn't, as well as some of the back-and-forth it took to reach the final output.

---

### Part 1 -  Value Iteration
As described in the project, value iteration computes *k*-step estimates of the optimal values, $V_k$. In simpler terms, it looks at Pacman's "score" at each possible step, and picks the direction that optimizes that score. This happens in a batch, meaning the best values at *every* possible state are calculated. Provided in the project specs was the following image, which demonstrates how taking certain steps from the bottom left starting position could result in better scores.

![ID: A 4 by 3 grid of squares ranging from black to light green, with values ranging from zero to one written on them respectively. There is also a single red square with a value of negative one.](https://inst.eecs.berkeley.edu/~cs188/sp12/projects/reinforcement/value.png)

To begin with, I prompted ChatGPT with the following:

`
Can you please write some C++ value iteration functions for me? The first would be 'runValueIteration'. This function should iterate over the values and update accordingly. It should call 'computeQValuefromValues(state, action)', which computes the Q-value of 'action' in 'state' from a given value function.
`

The resulting code was pretty solid, as it knew to keep track of the maximum Q-value, but it was slightly off in its method. Similar to my implementation, ChatGPT used a vector to store all of the values, and ran the `std::max_element` algorithm once per loop to update the values at that specific state. It was missing a final step, though, as the *overall* values also needed to keep track of a mapping from `state` to `max_Q_value` for that state. After pointing this out, ChatGPT added the appropriate line as needed.

I also asked ChatGPT to write the `computeActionFromValues(state)` and the `computeQValueFromValues(state, action)` functions, but because these were simple implementations that involved iterating over every possible Q-Value and action, things looked clean from the outset. The only sticking point was in the Q-Value function, where there was some confusion with the math being performed. Ideally, it would perform in accordance with the value iteration state update equation:

$$ V_{k+1}(s) \leftarrow \max _{a} \sum_{s^{\prime}} T\left(s, a, s^{\prime}\right)\left[R\left(s, a, s^{\prime}\right)+\gamma V_{k}\left(s^{\prime}\right)\right] $$

Or, equivalently:

`(The next value at state s) = maximum of the sum over s' of (probability * (reward + gamma * Vk(S')))`

In this equation, 'gamma' is the discount factor, ranging from 0 to 1. As gamma gets closer to 1, the agent values future rewards more closely to immediate rewards, while a gamma value of zero will make the agent only value immediate rewards. V<sub>k</sub>(S') represents the estimated value of the next state, S', in the Q-value computation. This equation is supposed to consider both the immediate reward and the potential future rewards, and return the final Q-Value to the `runValueIteration` function as a candidate for the best possible value.

In terms of how this mattered to Pacman, consider the following images:

![ID: A wide screen with Pacman at the far right, near some food pellets. There are two ghosts far away from him on the left.](https://www.researchgate.net/profile/Baihan-Lin-2/publication/341344004/figure/fig4/AS:890654011977729@1589359872283/Layout-of-an-ongoing-PacMan-game.ppm)
Pacman is in a very desirable position here - he has plenty of food pellets nearby, and with Inky and Clyde far away, he is not in any immediate danger. He could keep gathering food pellets and staying alive, both of which are worth points. Since this would result in a higher score for this sort of position (one where Pacman is near food pellets, but not near ghosts), positions (or "states") similar to this one would be correlated to a higher score, or "value."

![ID: A wide screen with Pacman at the right, directly adjacent to a ghost. A second ghost is very close by, and a third is far away from him on the left. A single large food pellet is diagonal to him.](https://inst.eecs.berkeley.edu/~cs188/sp12/projects/reinforcement/capsule.png)
Here, Pacman is much worse off. He is only one wrong step away from dying to Clyde, and a few moves away from being caught by another ghost. Additionally, he only has one food pellet readily available, even if it is a bigger one - these functions don't know that power pellets would normally let Pacman eat a ghost. All of this together would result in a position similar to this (near a ghost, and not near much food) having a very low score, or "value," so the corresponding state would not be prioritized as much in the functions above.

Taken together, if these two states were fed to the function, the one assigned to $V_{k+1}(s)$ would be the state with the better value - namely, the first of the two. Over a greater number of states with less obvious differences, it becomes more important to select one with a higher value, while also considering the probability of ever *reaching* that state.


#### Part 1 Critique

The trouble came when ChatGPT tried to write this function, and instead of keeping a running total of the Q-value for the single state and action, it initially did something similar to its behavior on the first function: It stored every intermediate total in a vector of doubles, then returned only the maximum element from that vector. I assume this is from trying to find the 'best' Q-Value, rather than recognizing that the running total applied to the entirety of the passed-in `state` and `action` arguments. Similarly to before, once I pointed this out, it was a simple fix of switching from the vector behavior to a single running total.

As an interesting aside, I later had to go back and re-prompt ChatGPT with the same question, because I'd misplaced its version of the code and wanted to compare another function with it. When I asked the same question with the same phrasing, it kept what I assume was a map of the current value at a state to that state's Q-value. I can't say for sure, since the type was not explicitly defined in the code, and no such map was given in the starter code. The reason this caught my attention is that, instead of making an additional vector to track the intermediate totals, it seemingly stored them in the 'current value' map as it went along, and swapped in the new Q-value when it was greater than the old value. I'm not sure how it landed on this different behavior, since both times, the prompts were only preceded by me saying 'hi!' before requesting it to write the code.

### Part 2 - Asynchronous Value Iteration
This version of the iteration is 'asynchronous' because it only updates one state per iteration, instead of a batch-style update like before. That is, on the *i*'th iteration, it should update the value of the *i*'th state in the states list. Following is the prompt I gave ChatGPT:

`Before, I had you write a C++ function called 'runValueIteration', which iterated over and updated the values, calling 'computeQValuefromValues' in the process. Can you please do this again, but instead of updating all of the values, have it only update the value of the i'th state in the states list on the i'th iteration?`

Consider again the first image provided in Part 1:

![ID: A 4 by 3 grid of squares ranging from black to light green, with values ranging from zero to one written on them respectively. There is also a single red square with a value of negative one. There is a path from the beginning to a state labeled 0.27, and from there to one labeled 0.51. There is also a path from the beginning to a state labelled 0.22.](https://inst.eecs.berkeley.edu/~cs188/sp12/projects/reinforcement/value.png)
With the value iteration function before, every state's possible values were considered. For asynchronous iteration, we can begin by looking at the squares labelled $0.27$ and $0.22$. On the first iteration, with $i=0$, we would look at state $0$, and calculate its value, which turns out to be $0.27$. We would ignore all the other states. The next time through with $i=1$, state $1$ would be considered, and given its value of $0.22$. All of the states are visited once per loop in this fashion, and after each has been seen once, the cycle begins again at $i=0$. It starts over like this because there is new information for each state about its neighbors. For example, suppose the square directly above the state happened to be valued at $0.7$ - then a state leading there would be worth more, right? So the state labelled $0.27$ would then earn a higher value on its turn through the iteration.

#### Part 2 Critique

Again, the general idea was correct, but there were a few small tweaks that needed to be made. First, ChatGPT was tracking whether the function had converged, which wasn't necessarily incorrect, but rather solved a different edge case than the one I'd intended. It was supposed to terminate once every iteration had been checked and updated as needed, since there was nowhere to progress if Pacman was at an end state. Instead, it checked for convergence based on whether the difference between the old and the new value at a given state was less than a specified value 'epsilon.' This is incorrect for a couple of reasons, not least of which is that no epsilon was provided, neither in my prompt nor in the starter code. My best guess is that this use of epsilon was as an early exit strategy, but the overall function was wrapped within the clause comparing the value difference to epsilon. Then, within that 'while' wrapper, there was a for-loop going over every iteration, as I specified. Given the epsilon condition, it would hit each iteration several times over, which is inconsistent with my prompt. I'm unsure whether I needed to be more clear, or if ChatGPT just pulled what it knew of generic asynchronous value iteration and spat that back out.

### Part 3 - Prioritized Sweeping Value Iteration
Similar to parts 1 and 2, this section required a new implementation of `runValueIteration`, now with an effort to focus updates of state values in such a way that the policy was likely to change. Where before it cared about every state's updated value, though, this part looked more at updating values if that seemed likely to matter overall. For example, in the grid used prior, that bottom-right square labeled $0.13$ is unreachable, so this function ought not waste its time calculating its value every time.

The instructions were rather specific in terms of the algorithm to follow when implementing the function, so instead of giving ChatGPT an easy way out, I prompted with a more general description:

`Could you please implement another version of 'runValueIteration' in C++, but this time, have it do a simplified version of the standard prioritized sweeping algorithm? We are given the following: predecessors of a state 's' are all states with a nonzero probability of reaching 's' by taking some action'a'. 'Theta' represents our tolerance for error when deciding whether to update a state's value.`

As before, the code was generally correct, but it was a little more basic than it ought to have been. The first step was to compute the predecessors of every state, which ChatGPT did correctly, but there could have been an additional check for whether a state was terminal, since those didn't need to be dealt with for our purposes. Similar to my second prompting with the first part, it assumed a variable 'current values' could be indexed by a state, and stored the running maximum Q-Value in that variable as needed. It used a priority queue and updated as expected, but there was a snag involving the variable 'theta.' This is perhaps due to the ambiguity regarding theta in the prompt, as ChatGPT decided that a state's priority needed to surpass the value of theta to merit updating, when in fact it should have depended on the difference between the maximum Q-Value at a state and the predecessor's value.

### Part 4 - Q-Learning

For all of the functions written so far, the reinforcement learning was in-place, such that Pacman was given the optimal path before taking his first move. The back half of the project focused on the case where Pacman did not necessarily have this information at the outset, and instead had to learn as he went—this is closer to the general idea of 'reinforcement learning' as I would conceptualize it.

Consider this image:

![ID: A four by three grid of black squares, each divided with a white X into four triangles. Most of the triangles are labelled with zeros. The top-right square is labeled 0.94, and the top row's eastward triangles have values from 0.05 to 0.62.](https://inst.eecs.berkeley.edu/~cs188/sp12/projects/reinforcement/q-learning.png)
Similar to the first three parts, Pacman begins in the bottom left, and the goal state is at the top right. The values along the top come from manually guiding Pacman directly upward, then directly to the right. The project specs mention how the agent "leaves learning in its wake" - that is, after reaching the goal with the north-then-east path, there's a brief trail showing how each state's value improved with their proximity to the goal. Since the rewards diminish with distance from the goal, the state directly above the start remained at zero, and since none of the other states were explored, they all remained zero as well. This is also why the goal is less than $1$ - Pacman doesn't *know* that this is a goal state with a value of $1$, only that he gets a good reward when he reaches that state.

I asked ChatGPT to implement the Q-Learning agent stub as it was provided in the starter code, largely by prompting it with each function declaration and giving it the details we had when doing the project in class. The functions of note were `update(state, action, nextState, reward)`, `computeValueFromQValues(state)`, `getQValue(state, action)`, `computeActionFromQValues(state)`, and `getAction(state)`. Individually, these functions were all rather simple, and the complexity came more from all of them needing to be operational in order to call each other.

#### Part 4 Critique

`getQValue` only needed to return the Q-Value at a given `(state, action)` tuple, which relied on storing the Q-Values for the agent somewhere in its class. The manner of storing those as a member variable is the only way I really expected ChatGPT's implementation to differ from mine, as there wasn't a specific instruction for how to do so, but there are a couple different data structures it could have conceivably used.

`getAction` was also straight-forward, as it simply needed to return a random action with a probability of the member value `epsilon`, and otherwise return an action using a pre-defined function.

`computeValueFromQValues` and `computeActionFromQValues` were similar to each other, as both relied on iterating over the possible actions and finding the maximum relevant value. I was a little surprised to see that ChatGPT did this directly in both functions, since to me, the fact that the functions were so similar would lend itself well to combining them in a helper function. I probably shouldn't be too put-off, because I did the same thing for my project, but I felt that ChatGPT could have had the combined fore- and hindsight to do less work with that sort of thing. I do also have the advantage of knowing that my code works from having previously turned it in, which makes it easier to see where ChatGPT's (and my) implementation could have been improved.

Finally, `update` had a fairly predictable solution, as it directly relied on calculating the reward transition and saving that as appropriate into the chosen storage member variable. The only error came in from certain member variables like `alpha` and `discount` not being known to ChatGPT, so it set them to a static number at the beginning of the function. On correction of this part, ChatGPT's code was again fine.


### Part 5 - Conclusion

Despite my more optimistic hopes, ChatGPT did pretty well on completing this project, and probably would've gotten at least a solid 75% before any corrective prompting or interpretation on my part. I knew better than to expect it would be a complete disaster, given that using AI for coding classes is becoming a more prevalent concern (or tool, depending on who you ask), but I thought it might not be quite so seamless. I imagine the moderate success is due to the fact that, as ChatGPT has gotten more popular, it's gotten better at producing something useful directly out of the gate, rather than after considerable re-prompting. A simple search like 'increased use of ChatGPT in coding classes' returns several tutorials on *how* to use the AI to one's advantage, and even some forum posts asking whether it's even worth it to bother learning coding anymore.

It would be foolish to ignore the obvious benefits of using ChatGPT for lower-level coding problems, such as a seasoned developer verifying that their syntax and behavior is correct, but plenty of people have already pointed out the concerns with it, too. This post alone is one such example, where the only actual coding on my part was *running* ChatGPT's code, and figuring out where and why it didn't work. The point of the original project obviously did include some user debugging, such that we could learn the reasoning behind what they were having us implement, but that wasn't all of it. *Only* doing the trouble-shooting, to the point that any actual coding on our part would have been what we fed ChatGPT, certainly wouldn't have been as beneficial to learning the material as us doing the work ourselves.

There isn't necessarily some grand point to be made here, especially when the same idea has already been restated and picked apart in more official capacities, but it still seems worthwhile to consider the difference in ChatGPT across disciplines. While only an anecdote, there is an article[^2] about a professor, C.W. Howell, who had his students generate an essay with ChatGPT, and then grade it themselves. According to Howell, all of the essays had errors, including citing sources that didn't even exist. The article goes more in-depth on the students' reactions to the essays and mistakes, but I bring it up because those errors seem much more egregious than the ones I encountered when having ChatGPT do this project. The errors I saw were all relatively minor, and often based on the fact that there was some variable ChatGPT didn't know about, or else a method it didn't know to call. It's hard to delineate where the line is, but I do think it's interesting that the AI seems more adept at certain topics than others, at least anecdotally.


[^1]: The Pacman AI projects were developed at UC Berkeley (http://ai.berkeley.edu). The core projects and autograders were primarily created by John DeNero (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
[^2]: Howell, C.W. “Don’t Want Students to Rely on ChatGPT? Have Them Use It.” _Wired_, Conde Nast, 6 June 2023, www.wired.com/story/dont-want-students-to-rely-on-chatgpt-have-them-use-it/.

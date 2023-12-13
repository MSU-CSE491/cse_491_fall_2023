# Experimenting with ChatGPT - Creating a simulation for DPS tests

As a long time MMO player (sufferer?), one of the greatest challenges I face on a regular basis is not some difficult boss or mechanic; \
it’s the dreaded Update Day. Because a new update means changes to gear, abilities, and more. And that means it’s  time to go through the \
process of testing all that stuff to see what performs best. I’ve often thought to myself, a far-off look in my eye, “Wouldn’t it be nice \
if I could speed this up?” Well, I finally decided to start on that project - and who better to help me out than my good old friend ChatGPT?

I decided that I would try to get ChatGPT to write the code from descriptions of what I wanted, without me directly telling it what code \
to write (spoiler: this would not last). So I loaded it up, and gave it this prompt:

<img src="images/Screenshot 2023-10-15 221108.png" width=800>

And this is what I got:

<img src="images/Screenshot 2023-10-15 221408.png" width=800>

Pretty good! It creates a target dummy, hits it til it’s dead, and then reports the time it took to kill. There’s one slight problem though - \
it doesn’t compile. The line that initializes the random number generator has three left parentheses, and two right parentheses. ChatGPT was \
more than happy to correct its mistake, and then I asked it to calculate the time taken based on some arbitrary tick, instead of wall-clock \
time. And this is the relevant code:

<img src="images/Screenshot 2023-10-15 222401.png" width=800>

Alright, but the whole point is that I want to speed up the testing process, not wait around while it runs! So I asked it to fix that, and \
it did so by introducing a tickDuration variable that could be used to calculate the time taken. Good. At this point I had a barebones \
framework that worked fine. Surely extending more functionality would be easy as pie, right?

Next I figured I’d have to change it from taking (damagePerHit) damage every tick, to having an ability that would fire off damage, but only \
once per second. Relevant ChatGPT code (Mismatched brackets from here on out are my own doing when copying code):

<img src="images/Screenshot 2023-10-15 223625.png" width=800>

Aaaand we’re back to using wall-clock time. This, I would find out, would be a recurring theme throughout this project. Upon requesting \
wall-clock time not be used, ChatGPT made it calculate time based on the number of ticks…but it also left in a line that had the loop sleep on\
 each iteration. I also noticed that it had, at some point, reintroduced the mismatched parentheses error on the random number generator line \
 (another recurring theme). So it fixed both issues, and I requested that it make the number of ticks in a second a variable instead of a \
 hard-coded value, so that it could be easily changed. It did so, and back came the sleeping on each iteration of the loop. Fixed that, and I \
 figured it was time for something a little more in-depth. I asked ChatGPT to implement an ability that would, upon being used, deal (x) damage \
 every (y) seconds, for a total duration of (z) seconds. Important code:

<img src="images/Screenshot 2023-10-15 224852.png" width=800>

This wasn’t too bad, but it did have a number of issues, such as the while loop now ending prematurely when the first ability ran out, instead \
of only when the dummy died. It also brought back sleeping on each loop iteration, to which I responded in no uncertain terms:

<img src="images/Screenshot 2023-10-15 225136.png" width=800>

This actually worked, and I did not see another sleep_for call for the rest of the project. This was about the point when I really had to start \
going back-and-forth with ChatGPT to fix the issues in the code. I was still trying not to tell it what code to write, and after a handful of \
exchanges, I made my first suggestion to to the AI:

<img src="images/Screenshot 2023-10-15 225711.png" width=800>

It promptly took my suggestion, and used it to create code that still didn’t work. After several more back-and-forths (including fixing some very \
simple syntax errors), with me being extremely specific about what I wanted, I finally got code that almost functioned correctly. The ability \
class now looked like this:

<img src="images/Screenshot 2023-10-15 230147.png" width=800>

And the relevant part of the loop body like this:

<img src="images/Screenshot 2023-10-15 230310.png" width=800>

The next issue I had to solve was that comparing doubles in the canDealDamage and updateActivity functions was prone to floating point errors. So \
I asked ChatGPT to fix this issue by doing the comparison using an epsilon. An epsilon, for those who have never had the absolute pleasure of \
dealing with floating point errors, is a small number that basically amounts to a tolerance for inequality. It’s a number that says, if you’re \
not exactly equal, but within “this” close of being equal, then that’s acceptable, you count as being equal. So this is what ChatGPT gave me:

<img src="images/Screenshot 2023-10-15 231122.png" width=800>

And this is where I made the biggest mistake of the entire project:

<img src="images/Screenshot 2023-10-15 231156.png" width=800>

After having to correct ChatGPT so many times so far, I got overconfident and corrected it without testing its code first, thinking it made a \
mistake in the calculation. It told me:

<img src="images/Screenshot 2023-10-15 231601.png" width=300>

And it made the change I requested. This would be the bane of my existence for the rest of the project. I tested the new code, and promptly \
realized I had made a mistake, and ChatGPT’s code was correct. I asked it to revert the change, and it happily agreed, and spat back out the \
same code with my error in it. I told it that it was right and I was wrong, and it agreed, and gave me the same incorrect code. Finally I \
resorted to straight-up telling it the exact code to use:

<img src="images/Screenshot 2023-10-15 231934.png" width=800>

Finally, mercifully, it fixed the functions, and the code worked more or less correctly to my specifications to that point. Progress!

The next step I figured wouldn’t be too bad - in a real game, you wouldn’t be able to cast two abilities at the same time, so I asked ChatGPT to \
make it so that only one ability could be cast at a time, and after an ability was cast, the loop would have to wait for one second before casting \
another ability. The results were…not great. It changed the ability class, giving each ability a member variable which limited that specific \
ability to being cast no more than once every second - an individual cooldown:

<img src="images/Screenshot 2023-10-15 233543.png" width=800>

This isn’t what I wanted, and also didn’t work right anyway. What I was looking for was a shared cooldown between any and all abilities. After some \
back-and-forths, I had this:

<img src="images/Screenshot 2023-10-15 233825.png" width=800>

And I made a simple request:

<img src="images/Screenshot 2023-10-15 233243.png" width=800>

Very easy. I told it exactly what to do. (At this point I had long given up on never telling it what code to write.) What it gave me was bizarre. \
It completely removed the if statement entirely, moving all of the cooldown related code back into the ability class, and basically going back to \
the code above. I told it that what it gave me wasn’t what I asked for, and it responded by apologizing, and nuking the cooldown code entirely so \
that there was nothing left of it, as well as removing good sections of code, like the updateActivity function. At this point, I could see the \
entire project crumbling before my eyes, and so I made the most drastic request of the project:

<img src="images/Screenshot 2023-10-15 234441.png" width=800>

And I provided ChatGPT with the full text of the code that it had generated before I asked for a shared cooldown. This worked, and I got back to a \
functioning version of the code. Seeing that I could resort to this if I needed to, I decided to try again. I asked it to give the abilities a \
shared cooldown…and it did so almost flawlessly. It gave me just about exactly what I wanted. This is the full text of what I had at this point, \
for anyone interested (there won’t be a quiz):

<img src="images/Screenshot 2023-10-15 235152.png" width=800>

<img src="images/Screenshot 2023-10-15 235356.png" width=800>

The only issue that I could see was that the comparison sharedCooldown <= 0.0 could run into floating point errors. And this is where my fatal \
mistake came back to haunt me. I asked it to fix the issue using an epsilon, which it did…and it simultaneously changed the comparisons in the \
canDealDamage and updateActivity functions back to the erroneous ones I had given it earlier. I asked it to fix those…and it broke the \
sharedCooldown comparison. I had to go back-and-forth with ChatGPT about a half-dozen times just to get these comparisons correct. And I would \
continue to battle with it over these comparisons for the rest of the project.

At this point, I was mostly happy with the functionality, but if I wanted this project to be even remotely useful, it would need to be able to \
handle more than two abilities at once. So I asked it to give me a vector of abilities instead of hard-coding in the functionality for two. I \
received this chunk of code in my main function (irrelevant parts omitted once again):

<img src="images/Screenshot 2023-10-16 000946.png" width=800>

I was quite happy with what ChatGPT gave me here; it worked flawlessly…other than breaking the epsilon comparisons again. I had it fix those again, \
and then I requested that each ability be given a priority, and for it to iterate over the abilities based on highest priority. Pertinent code:

<img src="images/Screenshot 2023-10-16 001617.png" width=800>

It gave me a nice custom sort and sorted the abilities for me - great! All I had to do was have it fix the epsilon comparisons again, fix an error, \
fix the epsilon comparisons again, and fix some edge-case bugs, and I had a properly functioning program. Lovely. At this point I decided to test \
ChatGPT’s knowledge of the program we had created. I asked it:

<img src="images/Screenshot 2023-10-16 002335.png" width=800>

Now I already knew that the answer was yes. The original specification for the damage over time ability was (x) damage, every (y) seconds, for a \
total of (z) seconds. The code we had created worked such that when an ability was cast it immediately dealt damage, so it would be a simple matter \
of having an ability where y and z were set to 0. But what would ChatGPT’s answer be? Well:

<img src="images/Screenshot 2023-10-16 002932.png" width=800>

…not exactly ideal. I pointed out:

<img src="images/Screenshot 2023-10-16 003123.png" width=800>

And it agreed that my way was better. I had it print out the entire body of code and everything looked good. I debated ending here, but decided \
there was one more bit of functionality I had to include. Remember the issue of individual vs. shared cooldowns? What I wanted back then was a \
shared cooldown, but it would be good to have both of them implemented. So I asked the AI, and it gave me what I can only describe as garbage. It \
broke the working code, and when I told it what was wrong, it broke the code even further. I ended up going back to the last resort method of \
telling it the entire body of code that it had given me before. After a second attempt I got it working right and I was ready to wrap the whole \
thing up. There were just a handful of smaller issues with code structure and quality that I wanted to fix. During these smaller fixes, ChatGPT \
often responded to me with small snippets of code instead of the entirety of the code, like so:

<img src="images/Screenshot 2023-10-16 004428.png" width=800>

“No problem,” I thought. “This works well, and it doesn’t print out a hundred lines of code for one small change.” If only I had known what was \
coming next…because after making all the small changes I wanted, I had it print out the whole thing all together to make sure everything was right…\
and it had forgotten a bunch of the code. It took a good twenty or so requests to get it back to something close to what it should have been. But \
finally, I had something that resembled a proper simulation. You can change the list of abilities, and it will simulate hitting a target dummy with \
them based on the priorities you set. Other things can be changed as well, such as the tick rate and the shared cooldown. In an accurate simulation, \
of course, there’s a lot more that goes into the damage of an ability than just a flat number. Buffs, debuffs, critical hits - I have a lot more to \
improve on before I can use it to do my testing for me - but that’s a project for another day.

What did I learn, though, about using ChatGPT for projects like this? In no particular order:
- ChatGPT does great with smaller projects and individual snippets, but struggles to keep larger projects cohesive
- It does not tell you you’re wrong - if you tell it to do something a certain way, it will absolutely do it
- Being very specific with what you want is more likely to get you what you want
- Asking it to print out the entire body of code instead of snippets seems to help it remember the whole thing
- Asking it to stop generating code using sleep_for worked, asking it to always use a certain epsilon comparison did not work
- Even if you can get it to generate code that works, it can’t always give you good answers about how or why it works
- The style and readability are generally quite good; it loves to put helpful comments in the code
- On the other hand, it seems to like hard-coded values, so if you want those values to be easily changed, watch out for that

ChatGPT conversation log:

https://chat.openai.com/share/61b56d0c-6599-4926-8e01-4a4d26a7eb27

Full body of code:

<img src="images/Screenshot 2023-10-16 011240.png" width=800>

<img src="images/Screenshot 2023-10-16 011303.png" width=800>
C++ Advanced Feature: std::ranges
By Mui Pham

Introduction
The Ranges Library was added to the standard template library in c++20. But before we get into the details of the library, 
we’d like to ask a few questions to underline the relevance of Ranges. Why should you care about Ranges? How does learning 
about Ranges further enrich your experience as a programmer? How does this connect to the broader picture?

So why should you care about Ranges? Well, let's take at a block of code and see how a c++ efficient programmer would have written
this program before c++20 Ranges.

~~~
#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>
#include <numeric>

int main() {
    std::vector<int> vectorOfInt = {5,4,3,2,1,6,7,8,9,10};
    std::vector<int> vec;

    std::transform(vectorOfInt.begin(),vectorOfInt.end(), 
                   std::back_inserter(vec),[](int i){return i*i;});
    auto sumsq = std::accumulate(vec.begin(),vec.end(), 0);
}
~~~

Summarizing the code:
We used a lamda to square the elements of the vector of ints and copy the results into another vector.
Then, we used the accumulate algorithm to find the sum of the squares.

Now lets see how we write this code in c++20 Ranges

~~~
///Credit to Tristan Brindle (cppnorth 2022)
#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>
#include <numeric>

int main() {
    std::vector<int> vectorOfInt = {5,4,3,2,1,6,7,8,9,10};
    std::vector<int> vec;

    std::ranges::transform(vectorOfInt,vec.begin(),
                           [](int i){return i*i;});
    auto sumsq = std::accumulate(vec.begin(),vec.end(), 0);
}
~~~~

Summary:
This code functions the same way as the code before it. However, there are two qualities of life
to mention about the range version. One, the programmer doesn't have to pass the begin and end
iterator of the containers that are being operated on. They simply have to pass the container as 
a whole. Two, the programmer no longer have to use a back_inserter to insert elements into the 
container storing the results. They just have to pass the beginning iterator. With those two 
changes, perhaps you can agree that the code is somewhat more legible.

Note:
The range version of the STL algorithm are overloaded to take the beginning and ending iterator
of the container that is being operated on. In this case, it returns the same type of iterator
that the STL algorithm would return. Here is where we start to introduce esoteric terminology to
you. When we pass a beginning and ending iterator to an algorithm, the implication is 
that this container is present somewhere. This container is an Lvalue range. It has an address 
in memory where values are stored. Another way to think of it is that Lvalue ranges are allowed 
to be on the left side of the assignment operator, hence, its name: Lvalue. 

~~~
    ///example of a single argument range
    std::vector<int> vec{1,2,3};
    std::vector<int> vec2;
    std::ranges::transform(vec, vec2.begin(),
                           [](int i){ return i*i;});
    
    ///example of a range being passed it's beginning and ending iterator
    std::vector<int> vec{1,2,3};
    std::vector<int> vec2;
    std::ranges::transform(vec.begin(),vec.end(), vec2.begin(),
                           [](int i){ return i*i;});
~~~

The reason this topic is broach is because when we pass a single argument for the container being 
operated on, it may not always be the case that the "container" is an L-value range. Container
is in quotation because container is defined as a range that owns the elements. The reason why
we are precise with definition of container is because R-value range can be passed as arguments in the 
range-ified version of the STL algorithms. You might see where this is going. R-value ranges don't own the
elements. The elements from an R-value range only exist for the full expression. A full expression
is a line of code that ends at the semi-colon. C++ doesn't have a guideline of where these temporary
values have to exist temporarily. Every compiler define their own rules of where temporary
values are stored. So, in the case that one argument range is passed, the iterator that returns is a 
borrowed iterator. But std::ranges::dangling is used with the template aliases of borrowed iterator and 
is the object that is instantiated if the range argument doesn't model borrowed ranges. This means if 
the range argument isn't an L-value range, the STL algorithm that returns an iterator, will return 
a std::range::dangling object. This is important because the iterator returned from the STL algorithm 
should never dereference to a value that no longer exists in memory. That would be undefined 
behavior. The dangling object is a wrapper object with no dereference operator defined, 
so if we tried to deference the dangling object, it will throw a compiler error. This error is ideal. 
We caught undefined behavior at compile time.

But why go through the trouble of allowing R-value ranges to be passed as arguments? We use the concept
of full expression to operate on a range that doesn't have to exist in memory for any other point in time, 
we skip the process of storing that range in memory and work on the values immediately and 
discard it after the full expression. We take the above single range argument example and show the power
of passing R-value ranges:

~~~
    ///example of a single argument range
    std::vector<int> vec{1,2,3};
    std::vector<int> vec2;
    std::ranges::transform(vec, vec2.begin(),
                           [](int i){ return i*i;});

    ///example of an R-value Range being passed as an argument
    std::vector<int> vec2;
    std::ranges::transform(std::vector<int>{1,2,3}, vec2.begin(),
                           [](int i){ return i*i;});
~~~

Summary:
In the single argument range implementation, we store the int values 1,2, and 3
into a vector of ints and then pass that vector into std::ranges::transform.
If we assume we don't use vec for anything other than calculating its elements squared,
we can skip assigning those values to an L-value range and pass those
values immediately to std::ranges::transform on line 117. The logic in this implementation
saves us copy operations into a range.

The last topic to cover is std::views. A view is a range that doesn't own 
the elements that the begin/end points to. A view is cheap to create, copy and move. 
The idea of views is to lazily evaluate elements in a range on demand. This means 
the value of a view element isn't computed until it is accessed. 
To illustrate, we present a snippet of code:

~~~
///Credit to Hannes Hauswedell for code snippet
#include <vector>
#include <iostream>
#include <ranges>

int main() {
    std::vector vec{1, 2, 3, 4, 5, 6};
    auto v = vec | std::views::reverse | std::views::drop(2);

    std::cout << *v.begin() << '\n';
}
~~~

Note: the | symbol is a pipe operation similarly to what you would find in the unix command line.

The creation of the view object is done in these sequence of steps: vec which is a L-value range
is piped through std::views::reverse which in turn creates a view adaptor. The view adaptor is 
an intermediate object used to chain with other views to create a combined view adaptor. At the point
that v in line 143 is assigned to the result, the combined adaptor returns a view object to the assignment operator.
So v is a view object. Note that accessing 0th element of v is determined at the time of access which is on line 145,
after the view has been created. It is done by first creating a reverse view of vec and then creating another view by 
dropping the firsts 2 elements; hence, the element in v is the value 4. The idea of view is to conservatively iterate 
over a range, and once iteration is necessary, we apply a recipe of operations on the element.
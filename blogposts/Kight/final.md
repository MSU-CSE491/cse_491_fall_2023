# Bit Packing Blog Post : Matt Kight

### Fall 2023

# Introduction: Understanding Bit Packing in C++

Precision and efficiency are hallmarks in C++ programming. Bit packing is a technique that may seem perplexing at first,
but it wields a profound influence on how memory is allocated and manipulated within your programs. Welcome to the
fascinating world of bit packing where you can achieve a level of precision that breaks free from the byte boundaries to
saving memory, and boost performance.

In this blog post, we will embark on a journey to demystify bit packing. We will delve into why it matters, how it
works, and how you can leverage this technique to optimize your C++ code. By the end of this journey, you'll be equipped
with the knowledge and tools to harness the power of bit packing in your own projects.

So, let's roll up our sleeves and begin our exploration of bit packing in C++. We'll unlock the secrets of memory
efficiency and performance optimization, one bit at a time.

# Why Bit Packing Matters

In the world of C++ programming, efficiency is a critical concern. Every byte of memory and
every CPU cycle can make a substantial difference in the performance of your applications. This is where bit packing
shines as a valuable technique, and understanding why it matters is crucial.

#### 1. Memory Efficiency:

Bit packing is primarily employed to save memory. In C++, data structures are typically allocated in byte-sized units,
which can lead to memory wastage when dealing with data that doesn't neatly fit into these units. Consider a simple
Boolean value – it only needs one bit to represent true or false, but C++ allocates a whole byte (8 bits) for it. By bit
packing, you can precisely represent data in the smallest possible unit, reducing the memory overhead of your program.

This memory efficiency is especially critical in scenarios where resources are limited. For instance, in embedded
systems, where memory is scarce, bit packing can be a game-changer. It allows you to maximize the use of limited memory,
enabling the development of more efficient and cost-effective systems.

#### 2. Performance Optimization:

Bit packing isn't just about saving memory; it's also about improving performance. In modern computer architectures,
memory access times and cache efficiency are major determinants of how fast your code runs. When you use bit packing to
store data in smaller units, you can reduce memory access times. This means that your CPU can fetch and manipulate data
more quickly, leading to a noticeable performance boost.

Furthermore, compact data structures are cache-friendly. Caches store data in blocks, and when your data structures are
small due to bit packing, more of them can fit into a single
cache line. This results in fewer cache misses, which can have a dramatic impact on execution speed as accessing the
cache is 10-100 times faster than accessing via RAM.

Let's take the following program as an example:

```
// Note : Example program is modified from ChatGPT program
#include <iostream>
#include <vector>
#include <chrono>

const int NUM_FLAGS = 1000000;  // Number of boolean flags

struct UnpackedFlags {
  bool flags[NUM_FLAGS];
};

struct BitFieldFlags {
  struct {
    unsigned int flag1 : 1;
    unsigned int flag2 : 1;
    unsigned int flag3 : 1;
    unsigned int flag4 : 1;
  } bits[NUM_FLAGS/4];
};

// Function to process flags without bit fields
void ProcessUnpackedFlags(const UnpackedFlags& data) {
  for (int i = 0; i < NUM_FLAGS; ++i) {
    // Process individual flags
    bool flag = data.flags[i];
    // Do some work with the flag (e.g., a trivial operation)
    bool result = !flag;
  }
}

// Function to process flags with bit fields
void ProcessBitFieldFlags(const BitFieldFlags& data) {
  for (int i = 0; i < (NUM_FLAGS / 4); ++i) {
    auto & flagBits = data.bits[i];
    // Process bit fields
    bool flag1 = flagBits.flag1;
    bool flag2 = flagBits.flag2;
    bool flag3 = flagBits.flag3;
    bool flag4 = flagBits.flag4;

  }
}

int main() {
  int NUM_TRIALS = 1000;
  std::chrono::duration<double> elapsedUnpacked = std::chrono::duration<double>::zero();
  std::chrono::duration<double> elapsedBitField = std::chrono::duration<double>::zero();

  for (int i = 0; i < NUM_TRIALS; ++i){
    UnpackedFlags unpackedData;
    BitFieldFlags bitFieldData;

    // Initialize data
    for (int i = 0; i < NUM_FLAGS; ++i) {
      unpackedData.flags[i] = (i % 2 == 0);  // Set every other flag to true
      bitFieldData.bits[i / 4].flag1 = (i % 2 == 0);
      bitFieldData.bits[i / 4].flag2 = (i % 2 == 0);
      bitFieldData.bits[i / 4].flag3 = (i % 2 == 0);
      bitFieldData.bits[i / 4].flag4 = (i % 2 == 0);
      // ... Initialize more flags here
    }

    // Measure time for processing unpacked flags
    auto startUnpacked = std::chrono::high_resolution_clock::now();
    ProcessUnpackedFlags(unpackedData);
    auto endUnpacked = std::chrono::high_resolution_clock::now();
    elapsedUnpacked += endUnpacked - startUnpacked;

    // Measure time for processing bit field flags
    auto startBitField = std::chrono::high_resolution_clock::now();
    ProcessBitFieldFlags(bitFieldData);
    auto endBitField = std::chrono::high_resolution_clock::now();
    elapsedBitField += endBitField - startBitField;
  }


  std::cout << "Average time for processing << NUM_TRIALS << unpacked flags: " << elapsedUnpacked.count()/NUM_TRIALS << " seconds\n";
  std::cout << "Average time for processing << NUM_TRIALS << bit field flags: " << elapsedBitField.count()/NUM_TRIALS << " seconds\n";


  return 0;
}
```

The output on my following machine is as follows:

```
Average time for processing 1000 unpacked flags: 0.00173472 seconds
Average time for processing 1000 bit field flags: 0.00135095 seconds
``` 

The use of bitfields has sped up our program by ~25%. Note this may not be the same output depending on compiler and
hardware used.

#### 3. Data Serialization:

Bit packing plays a vital role in data serialization and deserialization. When you're transmitting data over a network
or storing it in a file, it's essential to minimize the data's size. Smaller data sizes mean faster transmission and
less storage space required. Bit packing helps you achieve this goal by efficiently encoding and decoding data for
communication or storage.

In conclusion, bit packing is not just a niche concept; it's a fundamental technique that allows C++ programmers to save
memory, boost performance, and efficiently work with data. It's a tool that becomes increasingly valuable in scenarios
where resource constraints and performance optimization are critical. In the following sections, we'll explore how to
implement bit packing in C++ and leverage its advantages in your projects.

# How to use Bit Packing

Now lets go over the syntax of bit packing. Things to note:

- Only integral data types are allowed to be used in bit packing
  - These are int, long, short, byte, and char
- Can work either with classes or structs

```
struct StructName {
    dataType fieldName : width;
    // more bit-field members...
};
```

Below is an example implementation of where bit packing can become useful.

```
struct PackedFlags {
    unsigned int flag1 : 1; // 1 bit for flag 1
    unsigned int flag2 : 1; // 1 bit for flag 2
    unsigned int flag3 : 1; // 1 bit for flag 3
    unsigned int flag4 : 1; // 1 bit for flag 4
    unsigned int flag5 : 1; // 1 bit for flag 5
    unsigned int flag6 : 1; // 1 bit for flag 6
    unsigned int flag7 : 1; // 1 bit for flag 7
    unsigned int flag8 : 1; // 1 bit for flag 8
};
```

```
struct Flags {
    bool flag1;
    bool flag2;
    bool flag3;
    bool flag4;
    bool flag5;
    bool flag6;
    bool flag7;
    bool flag8;
};
```

This will create 2 structs. Each struct can hold 8 boolean values. The top one employs bit-packing where the bottom one
does not.

```
PackedFlags packedFlags;
Flags flags;
std::cout << sizeof(packedFlags) << std::endl;  // Prints 4
std::cout << sizeof(flags) << std::endl;        // Prints 8
```

When printing out the size of each of these structs the bit packing was 50% smaller than the normal operation. Please
note that this will not always be the case and can be compiler and system dependent.

This space-saving is an essential reason as to why you would use bit packing.

# Downsides:

While there are many upsides to creating classes and structs that hold specify number of bits can be great to save space
it does not come without some downsides.

#### No pointers to member variables

Bit packing, while efficient in terms of memory utilization, does introduce certain limitations, one of which is the
challenge of creating pointers to individual member variables within a struct or class. The nature of bit packing blurs
the clear boundary between where one variable starts and another ends, as they share the same memory space. This makes
it difficult to create direct pointers to specific member variables, which is a common operation in C++. Pointers are
essential for various operations, such as passing references to specific data or for efficient memory manipulation. In
the context of bit packing, accessing individual variables often requires complex bit-level operations, making the code
less intuitive and potentially more error-prone when compared to conventional member access using pointers.

Using the structs above will create a compile time error

```
auto x = &packedFlags.flag1;
```

#### Possible speed loss due to time accessing bits

While the example above demonstrated an approximate 25% improvement in processing time by employing bit packing, it's
essential to acknowledge that this performance gain is not always guaranteed. The effectiveness of bit packing depends
on various factors, including the specific hardware architecture and the compiler being used. In certain situations,
accessing individual bits within packed data structures can be slower than dealing with larger byte-sized memory units.
This is because modern hardware is often designed to handle memory in larger chunks, such as bytes or words, and may not
be as optimized for bit-level operations. As a result, the benefits of bit packing may vary, and in some cases, it might
not provide a significant performance boost, or it could even lead to a slowdown due to the overhead introduced by bit
manipulation.

# Conclusion:

Unleashing the power of bit packing in the world of C++ programming, where every byte counts and efficiency
is paramount, bit packing stands as a beacon of ingenuity. Through our journey in this blog post, we've unlocked the
secrets of bit packing and discovered its pivotal role in memory efficiency and performance optimization. This seemingly
niche concept offers substantial advantages that transcend the world of game development and embedded systems, extending
its influence into various domains of software engineering. Bit packing is the tool that allows you to seize control
over your data, manipulating it with precision, and crafting lean, agile data structures.

Whether you're developing for
resource-constrained environments, seeking to maximize the performance of your applications, or simply looking for ways
to optimize memory usage, bit packing is a technique that you can't afford to ignore. It's not just a coding technique;
it's a game-changer, enabling you to make the most of your resources, reduce memory overhead, and achieve faster
execution. As you continue your journey in the world of C++, remember that bit packing is your ally, waiting to help you
craft more efficient and responsive code. So, go ahead, implement bit packing, and unleash its transformative power in
your C++ projects.

## Sources:

1. [Geeks For Geeks Article](https://www.geeksforgeeks.org/cpp-bit-fields/)
2. [CPP Reference for Bit Packing](https://en.cppreference.com/w/cpp/language/bit_field)
3. [ChatGPT Chat Log](https://chat.openai.com/share/c17794b3-eaa0-4982-a2cd-75f098b0b080)
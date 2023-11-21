FAISS in C++

For this C++ library review, we will be discussing the FAISS (Facebook AI Similarity Search) library. According to Meta's own website, it "allows developers to quickly search for embeddings of multimedia documents that are similar to each other" (https://engineering.fb.com/2017/03/29/data-infrastructure/faiss-a-library-for-efficient-similarity-search). This capability has many different applications, and in this review, we will be delving into similarity searches of text.

For text-based searches, FAISS can read text converted to numerical vectors (referred to as embeddings) in order to compare similar pieces of data. I will be demonstrating this capability with a dataset of 200 sentences.

To begin, I will discuss how I installed FAISS. As far as my IDE goes, I am using CLion with CMake, and my OS is Windows 10. I simply cloned the FAISS git repo and continued from there. A pattern that quickly emerges is that there are many other things to install that FAISS is dependent on. The first of these is the NVIDIA CUDA Compiler. If you attempt to run FAISS without CUDA, you will get this error: "Failed to find nvcc. Compiler requires the CUDA toolkit". After this, I needed to install BLAS (Basic Linear Algebra Subprograms), as FAISS needs BLAS libraries for certain operations. Installing Intel oneAPI Math Kernel Library accomplished this for me. Installing both of these took considerable effort, and I ran into plenty of errors while attempting to compile the library with them.

Next, I ran into this error: "Could NOT find SWIG". If you are unaware, FAISS is written in C++ (or else, why would I be writing about it?) -- with complete wrappers for _Python._ I was unaware until hours after getting everything else working, but FAISS is almost always used with a Python wrapper. All videos and guides online use FAISS with Python, and by default, FAISS expects you to do the same. Because we are working in C++, we do not need SWIG. Achieving this was simple enough: changing the CMake configuration to "-DFAISS\_ENABLE\_PYTHON=OFF".

At this point, I thought I would be good to go. However, upon attempting to build my project with FAISS, I kept running into an error that "unistd.h" not found. After doing some research, I found out that this is a Unix header file, and not available on Windows systems. Although the official FAISS GitHub repository states that "Faiss is supported on x86\_64 machines on Linux, OSX, and Windows" (https://github.com/facebookresearch/faiss/blob/main/INSTALL.md), I had a difficult time getting it working. I started to worry at this point, because I was afraid I would not be able to run FAISS on my system. My first attempt to fix this problem was to clone a git repo that ported FAISS for Windows. Unfortunately, that did not work for me.

My solution to this problem was using WSL (Windows Subsystem for Linux). This allowed me to work in a Linux environment and avoid running into Windows-specific compiler issues. At first, I was having a lot of issues with my CMake profile. Luckily, a WSL toolchain is provided in CLion. After modifying my CMake profile to use the WSL toolchain, and adding the appropriate links in my CMakeLists.txt, I was able to finally build my project with FAISS.

To rate my experience installing FAISS as a Windows and C++ user, I would give it a 1.5/5. Difficulty installing CUDA/ BLAS, not being able to work directly in Windows, and lack of support for C++ FAISS use online, were the biggest reasons for me. If you want to use this library, it may be a better idea to use it with Python if possible.

![](RackMultipart20231120-1-4yydme_html_cee7ab6edbf222bd.png)

Next, it is time to put FAISS to the test. In order to demonstrate its capabilities, I want to conduct a similarity search with text. First, I needed some text to work with, so I asked ChatGPT to generate 200 different sentences for me. I wrote these sentences into a .txt file line by line.

Next, I needed an embedding framework to transform text into vectors with numerical representations. I downloaded a 4GB file that contained a 300d pre-trained model for text, in order to perform this transformation. However, this part was quite difficult for me to accomplish. First of all, most of these embedding libraries were for Python, (e.g. the Sentence Transformers). I then tried to use the FastText for C++, but I had issues compiling and getting it to work with CLion/CMake.

![](RackMultipart20231120-1-4yydme_html_c911706b9cc2d6bb.png)

After running into many different problems, using FastText with Python was much easier. Working with Python in VSCode and WSL did the trick for me. Mind you, I just decided to use a Python file to transform the text into embeddings, and then write that all to a .csv file.

![](RackMultipart20231120-1-4yydme_html_63ca7c9159f21634.png)

The actual code that works with FAISS is still in C++ and is simply reading in the .csv file. As I previously explained, these vectors have 300 dimensions (so the .csv file has 300 rows of decimal values), and there are 200 vectors. These vectors are specifically pre-trained for words – if you wanted to work with images, you would want to find another model.

![](RackMultipart20231120-1-4yydme_html_b5896144d59b3dd6.png)

First, I read the embeddings into an array with a function I made called load\_vectors.

![](RackMultipart20231120-1-4yydme_html_8aca07a55d5bbc7c.png)

Then, I created a flat array to hold these embeddings in a format that is suitable for FAISS. I allocated memory for the array using the fact that it has 300 dimensions with 200 vectors. Then, with "faiss::IndexFlatL2 index(d);", a Faiss index was created for L2 (Euclidean) distance searches. This helps us figure out how similar two vectors (texts) are – there are 300 points in the Euclidean space for 1 vector, and the closer these points match up to other vectors, the closer the text is. "index.add(nb, data.get());" just adds all the vectors to the FAISS index.

![](RackMultipart20231120-1-4yydme_html_aeb9505895aa12fa.png)

Next, I created an array called query\_vector which has 300 elements in it. some\_index is a value that the user can modify to their liking – it takes the nth vector in the array of 200 (i.e., the nth sentence), and compares that to all the other ones. query\_vector is made to store all elements from the index we want to search with. We define k, which is the number of nearest neighbors we want. If we want the 5 most similar sentences, we set k to be 5. distances holds the distance in Euclidean space between the query and the neighbor, and indices simply holds the index of the neighbor. Then, the FAISS search function finally finds the closest vectors keeping all of these things in mind.

![](RackMultipart20231120-1-4yydme_html_95a764831a7b3e45.png)

To display the search results, I indexed the original .txt file of sentences I had, and indexed it for each corresponding neighbor. Let's see the results of searching by the 10th index, which corresponds to this sentence: "The envoy expressed optimism about future collaborations.":

![](RackMultipart20231120-1-4yydme_html_12b5e86f2608fa3b.png)

As you can see, there are 10 results, as I set the k value to 10. Off first-glance, you can see that the closest results all begin with "The". That's a good start, but not very specific. At first, I wasn't sure if FAISS was actually working or not. To test this, I searched with the 1st index: "The party's vision resonates with the aspirations of the youth.":

![](RackMultipart20231120-1-4yydme_html_c995c0ebd5caa9.png)

 And then I added a sentence to sentences.txt that was almost identical to that one, to see if FAISS was actually working. If it was, that sentence would be the closest one and have a very short distance. Here are the results:

![](RackMultipart20231120-1-4yydme_html_ffa7c3336d699967.png)

Luckily, this worked exactly how I wanted it to; the second result (index 6) is one that I manually added. Now, I knew my code was working.

Overall, I would say I enjoyed using the FAISS library. It was very difficult to install for me because a few different libraries were not working. However, the end result can be very useful depending on what you are working on. For example, my implementation of FAISS search can help to find documents similar to the one you are looking at. However, there are many more use cases. If you wanted to do a similarity search with faces instead, you can generate vectors based on image data using a different pre-trained model, and then follow the same steps I did. The same can even be done for audio clips! Overall, I would recommend the FAISS library for quick similarity searches in C++ on Mac and Linux, but would recommend another library for Windows users due to lack of support.

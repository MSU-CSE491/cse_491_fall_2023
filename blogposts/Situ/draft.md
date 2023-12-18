# Review of PyTorch C++ API

## Pre-requisites
* Knowledge of using PyTorch in Python
* Basic C++ knowledge

## Introduction
PyTorch is a library for Python for machine learning. It is written in C++ and Python is used to interface with the library. This makes it have the performance of C++ while being easy to use. So why use the C++ API? PyTorch gives the following [reasons](https://pytorch.org/tutorials/advanced/cpp_frontend.html):
* The ability to make use of C++ multi-threading, since Python has a global interpreter lock which prevents true multi-threading.
* It can be used to integrate with existing C++ code.
* Python's low performance is too much of a cost for your application.

Of course, You may just prefer writing C++ rather than Python.

The C++ API is designed to be extremely similar to how PyTorch is used in Python, so that it is straightforward to use. One of the tips that they give is to write the code like you would in Python, while abiding to C++ syntax [[2]](#Sources).

## Installation
The setup for the library is pretty straightforward if you only want to use the CPU for PyTorch. [These instructions worked without modification.](https://pytorch.org/cppdocs/installing.html)
The installation for GPU support is a lot more involved, and I was not able to install PyTorch with GPU support. GPU support requires additional dependencies such as CUDA and a different version of the library, and I had many errors trying to get CMake to find the CUDA related binaries. I use Windows, and there's a lot more information in various forums about installing PyTorch with CUDA on Linux distributions, so Linux users will have an easier time.

## Documentation
The documentation can be found [here.](https://pytorch.org/cppdocs/index.html) There is a significant portion of functions that are missing documentation.

## Minimal Example
The installation guide gives the following example:
```cpp
#include <torch/torch.h>
#include <iostream>

int main() {
  torch::Tensor tensor = torch::rand({2, 3});
  std::cout << tensor << std::endl;
}
```
Output:
```cpp
 0.3828  0.0383  0.8342
 0.3193  0.1213  0.6756
[ CPUFloatType{2,3} ]
```

This is the equivalent Python code:
```py
import torch

tensor = torch.rand(2, 3)
print(tensor)
```
Output:
```cpp
tensor([[0.5879, 0.5029, 0.1522],
        [0.0165, 0.1526, 0.8701]])
```

Aside from the random numbers, the results are pretty similar. If we use a large tensor size, such as [2,3,4], we see the following output:

C++ Output:
```cpp
(1,.,.) = 
  0.6969  0.9800  0.9234  0.7233
  0.3798  0.5593  0.0248  0.4228
  0.7216  0.3585  0.0960  0.8017

(2,.,.) =
  0.9224  0.8513  0.3186  0.1017
  0.5374  0.5361  0.1317  0.0175
  0.2660  0.0787  0.0405  0.9669
[ CPUFloatType{2,3,4} ]
```
Python Output:
```cpp
tensor([[[0.9271, 0.0307, 0.6839, 0.0347],
         [0.2656, 0.7255, 0.8338, 0.8732],
         [0.1196, 0.7630, 0.2806, 0.6407]],

        [[0.9917, 0.8121, 0.7952, 0.1987],
         [0.9438, 0.6141, 0.3507, 0.5229],
         [0.8222, 0.0517, 0.3564, 0.0864]]])
```
The C++ output includes the index for each 2D slice of the tensor, which can be useful for debugging. You can easily lose track of which slice you're looking at in Python, especially if you printed out a large tensor.

## Creating a Module
A Module is the base class for neural networks in PyTorch. It can hold other Modules as attributes, and PyTorch will keep track of them as children in a tree structure when we use `register_module`. This is useful for saving and loading models, moving models between types and devices, and most importantly for training. Here is an example Module with 3 fully connected layers, taking 1 value and outputting 2 values:
```cpp
struct Net : torch::nn::Module {
  torch::nn::Linear fc1, fc2, fc3;

  Net(int64_t dims) : fc1(1, dims), fc2(dims, dims), fc3(dims, 2) {
    register_module("fc1", fc1);
    register_module("fc2", fc2);
    register_module("fc3", fc3);
  }
};
```
Note: If you want it to be a `class` instead of a `struct`, you have to make the inheritance, constructor, and forward function public.

This above is the "C++ way" of creating a Module, which is best C++ practice. The API allows for the "Python way" of initializing and registering submodules:
```cpp
struct Net : torch::nn::Module {
  torch::nn::Linear fc1{nullptr}, fc2{nullptr}, fc3{nullptr};

  Net(int64_t dims) {
    fc1 = register_module("fc1", torch::nn::Linear(1, dims));
    fc2 = register_module("fc2", torch::nn::Linear(dims, dims));
    fc3 = register_module("fc3", torch::nn::Linear(dims, 2));
  }
};
```
The submodules are initialized to `nullptr`, and then assigned to the result of `register_module` inside the body of the constructor. This is at the cost of an extra constructor call for each submodule for initializing them to `nullptr`. Unless this is happening frequently, then the cost is pretty minimal. This is because `torch::nn::Linear` is an ["alias"](https://github.com/pytorch/pytorch/blob/4a388e70f2c7fedcdef598fb2144cc593452f04d/torch/csrc/api/include/torch/nn/pimpl.h#L206-L214) for a wrapper class template around a `shared_ptr`, called `torch::nn::ModuleHolder`. The real type "aliased" by `Linear` in this case would be ModuleHolder<LinearImpl>, where LinearImpl is the actual implementation of the Linear module. More details can be found [here](https://pytorch.org/tutorials/advanced/cpp_frontend.html#module-ownership), but overall this is to simplify the API and make it more similar to how it is done in Python. You can circumvent the use of the ModuleHolder wrapper class (or shared_ptr) by using the actual implementation class, but some features only support the wrapper class such as loading and saving. So there are options for advanced C++ users to be more efficient, while also being easy for users who are less interested in that to use the library.

You may have noticed that the features of the linear layer are specified by the int64_t type. This is a signed 64-bit integer. I'm not entirely sure why Linear is using a signed integer to specify its dimensions, because negative dimensions don't make sense. If you try making a linear layer in Python with negative dimensions, you get an error. I think it would make more sense to use an unsigned integer. I couldn't find any information on why this is the case, and many other values in the library use int64_t as well that could be unsigned. My best guess is that this is to be future-proofed, in case they want to use the negative values to mean something in the future. In any case, I can't imagine why you would need more than $2^{32} - 1$ features in a linear layer.

## Forward Function
We need to define what happens when we do a forward pass through the network. 
We do this by defining the forward function:
```cpp
struct Net : torch::nn::Module {
  torch::nn::Linear fc1, fc2, fc3;

  Net(int64_t dims) : fc1(1, dims), fc2(dims, dims), fc3(dims, 2) {
    register_module("fc1", fc1);
    register_module("fc2", fc2);
    register_module("fc3", fc3);
  }

  torch::Tensor forward(torch::Tensor x) {
    x = torch::relu(fc1(x));
    x = torch::relu(fc2(x));
    x = fc3(x);
    return x;
  }
};
```

This is pretty much the same as how it is done in Python, which looks pretty nice. We pass x through each layer and applying ReLU on the output of each layer, except for the last layer. We can call each layer like a function because they are ModuleHolders that define `operator()`, which calls the implementation's forward method. So `fc1(x)` is equivalent to `fc1->forward(x)`. `Module` doesn't have a virtual function named `forward`, so we don't use the `override` keyword and we can name this to something else, but if we want to use this `Module` in a `ModuleHolder`, it should be named `forward` so that it can be called like a function.

## Training
This will be a simple example of training Net to learn to output two arbitrary numbers from one.
```cpp
int main() {
  torch::nn::ModuleHolder<Net> net(10);
  std::cout << "net: " << net << std::endl;
  torch::optim::SGD optimizer(net->parameters(), 0.01);
  torch::Tensor x = torch::rand({1, 1});
  torch::Tensor y = torch::rand({1, 2});
  std::cout << "x: " << x << std::endl;
  std::cout << "y: " << y << std::endl;
  std::cout << "y_pred: " << net(x) << std::endl;
  for (size_t i = 0; i < 1000; ++i) {
    torch::Tensor y_pred = net(x);
    torch::Tensor loss = torch::mse_loss(y_pred, y);
    loss.backward();
    optimizer.step();
    optimizer.zero_grad();
  }
  std::cout << "Training finished!" << std::endl;
  std::cout << "y_pred: " << net(x) << std::endl;
}
```
Here's the output:
```cpp
net: Net(
  (fc1): torch::nn::Linear(in_features=1, out_features=10, bias=true)
  (fc2): torch::nn::Linear(in_features=10, out_features=10, bias=true)
  (fc3): torch::nn::Linear(in_features=10, out_features=2, bias=true)
)
x:  0.8651
[ CPUFloatType{1,1} ]
y:  0.8518  0.9766
[ CPUFloatType{1,2} ]
y_pred: -0.1346  0.4346
[ CPUFloatType{1,2} ]
Training finished!
y_pred:  0.8518  0.9766
[ CPUFloatType{1,2} ]
```
This is straight-forward too. We create a `ModuleHolder` for our `Net` so that we can write `net(x)`, which is completely optional. We could easily have defined `net` as `Net net(10)` and just use `net.forward(x)` instead. We create an optimizer with a learning rate of `0.01` and give it the parameters of our network. We then create arbitrary data. In the training loop, we compute the loss, propagate it backwards, update the parameters with the optimizer, and zero the gradients. After training is done, we see that the network has learned to output the same values as the target.

## Tensors
PyTorch works with tensors. In Python, they can have many different types such as floats, doubles, halfs, and many more. It is done in the same way in the C++ API. Tensors are dynamically typed, which is a striking contrast to the C++ philosophy of static typing. This allows for the user to change a tensor's type at runtime, using the `Tensor.to(<data type>)` method of tensors and modules. As a consequence, the program may perform an illegal operation between tensors with different types, but it will still compile and crash at runtime. This may not be a big deal for developers coming from Python, as they have likely gotten used to this problem. Crashing at runtime due to types is not something C++ developers typically deal with, if at all, so this can be seen as a disaster to some. In my opinion, it can be pretty frustrating to wait for it to compile just for it to crash due to a type error.

If you want to iterate over the values of a tensor frequently, dynamic dispatch will significantly slow down the access to the values. Dynamic dispatch is the process of deciding which code to call for a certain tensor data type and number of dimensions [[3]](#Sources). This is like the dynamic dispatch that happens when polymorphism and virtual functions are used. If this is done for every value in a tensor, it can get fairly expensive. The API provides "accessors" which can be used to index into the tensor values efficiently. We can perform a quick test:
```cpp
#include "torch/torch.h"
#include <chrono>
#include <iostream>
using namespace std::chrono;
int main() {
  torch::Tensor x = torch::rand({100, 100});
  auto x_a = x.accessor<float, 2>();
  float t1{0}, t2{0};

  auto start = high_resolution_clock::now();
  for (size_t i = 0; i < 100'000; ++i)
    for (int64_t j = 0; j < x.size(0); ++j)
      t1 += x[j][j].item<float>();
  std::cout << t1 << std::endl;
  auto time_secs = duration_cast<milliseconds>(high_resolution_clock::now() - start);
  std::cout << time_secs << std::endl;
  
  start = high_resolution_clock::now();
  for (size_t i = 0; i < 100'000; ++i)
    for (int64_t j = 0; j < x_a.size(0); ++j)
      t2 += x_a[j][j];
  std::cout << t2 << std::endl;
  time_secs = duration_cast<milliseconds>(high_resolution_clock::now() - start);
  std::cout << time_secs << std::endl;
}
```
For the first loop, when we use `x[j][j]` it returns a 1x1 tensor containing the value. The value must be extracted and converted to a float using `Tensor.item<float>()`. For the second loop, we use the accessor to index into the tensor values directly. Here's the output:
```cpp
5.43507e+06
27972ms
5.43507e+06
23ms
```
The accessor is significantly faster since checks are only done once when the accessor was created [[3]](#Sources).

## Conclusion
The PyTorch C++ API is not too difficult to use. The developers spent a lot of effort to be extremely easy to use by hiding away templates, memory management, and other C++ features. It is practically a direct port of the Python API, so if you know how to use PyTorch in Python, you can use it in C++ with minimal difficulty. If you are an avid C++ programmer, you may be frustrated at the lack of compile-time type safety with tensors. The documentation has room for improvement, but I suppose the functionality should be the same as the Python interface, so the Python documentation can be used as a reference.


# Sources:
1. https://pytorch.org/cppdocs/installing.html
2. https://pytorch.org/tutorials/advanced/cpp_frontend.html
3. https://pytorch.org/cppdocs/notes/tensor_basics.html
4. https://github.com/pytorch/pytorch/
5. https://pytorch.org/tutorials/advanced/cpp_frontend.html
6. https://pytorch.org/cppdocs/index.html

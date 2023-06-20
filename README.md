# mnist_class_data

The C code is used to read the MNIST handwritten image database and generates
tab-spaced input / output tables as ".dat" file (ASCII text file).

This input/output relation is used to train an MLP in an external program, and
the external program generates C or MATLAB function that represents the output
of the model.

The MATLAB code visualizes this model, and compares to the original data.

Most of the programs rely on the existence of a "c/data" folder, which contains
the files to be read/manipulated. The MNIST data set should be placed in
"c/data/mnist"

get MNIST dataset files here: http://yann.lecun.com/exdb/mnist/

Isaac was here

<h1 align="center"> Welcome to ImageToLatex 👋</h1>
A neural network capable of translating handwriting into latex. The project also provides A-Z tools for generating raw latex, producing images and transforming images as if they were written by a human.
Project scheme: <br>

`Raw ==> Set ==> Visual ==> Model` <br>
Where `raw`, `set`, `visual` are tools and model is a neural network for recognizing latex.

## Model [Python]
Model is based on paper:
```
IMAGE TO LATEX VIA NEURAL NETWORKS
Avinash More
San Jose State University
```

### General Idea
ITL detects each character separately and merges them into one sequence.
### Details
Let `s` be a number of supported characters. ITL uses `s` clones of the same architecture. `J`'th neural network recognises `j`'th character using `one-hot encoding`. The project currently supports the following characters: `+`, `-`, `^`, `{`, `}`,`^`, `\cdot`, `a`, `x`, `1`, `2`, `3`, `4`, `5`, `6`, `7`, `8`, `9`, `0`
### Dataset
Input shape=`(64, 64)`. <br>
Format of input features: `eq_n_b_c`.
* `n` number of label
* `b` number of background
* `c` number of effect pack applied on a given feature. For given `n` and `b` all features `eq_n_b_{0...effects number}` represents the same label.

Link for the dataset used during training will be available soon on `mvxxx.github.io` as `exp.tar.gz`. <br>
The whole dataset was fully generated in use of `tool/{raw, set, visual}` in kind of pipe. <br> 

## Accuracy
For training dataset of length `7`, the mean accuracy was (depending on the difficulty of the dataset) ~`0.70-0.95` after `10` epochs of training.

## Tool/Raw [OCaml]
A functional tool which is written in OCaml. Random latex expression generators, with various syntactic levels and concepts describing exact behavior within the level. Create a set of generators capable of supplying the model with properly generated random latex expressions,
matching strict expectations, for training purposes. <br>


Performance:

|Type| Images/sec |
| :---: |     :---:      |
|Complex|~1.600.000  |
|Standard|~2.000.000  |
|Basic| ~4.000.000  |

## Tool/Set [Asymptote, Bash]
Script tool which gets several input files with raw LaTeX and converts them into basic .png expressions. This part executes the worker for each input file (kind of thread pooling). Using via bash script: `bash set.sh *.in`. <br>
It will produce all content inside temporary folders, then it moves all photos to the output folder. These images are input for `visual` part. All input `*.in` labels are concatenated and stacked into the `labels` file.<br>
It gets raw text like: `3-\frac{100}{88}` and returns: <br>
<p align="center">
  <img src="https://i.imgur.com/9qK9hVc.png" width="100" height="100" title="hover text">
</p>

## Tool/Visual [C++]
The biggest tool, written in `C++` and using `OpenCV`, capable of creating millions of written like human math equations. It applies a lot of different effects in order to make math as if it was
written by people. It can be configured using `config.hpp`. The final result is the base of the dataset for machine learning.
It gets raw png like before and returns (in that case only rotate was applied): <br>
<p align="center">
  <img src="https://i.imgur.com/LtG8FL4.png" width="100" height="100" title="hover text">
</p>
<br><br>
In that part, there are predefined effects like:


|Type| Brief |
| :---: |     :---:      | 
| left rotate | rotates images by (-C,0)|
| right rotate | rotates images by (0,C)|
|symetric scaling upward | scales both x and y upward|
|symetric scaling downward | scales both x and y downward|
|non-symetric scaling upward | scales independently x and y upward|
|non-symetric scaling downward | scales independently x and y downward|

There are also effects applied outside effect manager:

|Type| Brief |
| :---: |     :---:      | 
|position| changes position of sprite on background|
|background| changes background |
|perlin| applies perlin noise mask (in progress) |

Each effect is taken or not. For each image, we apply all possible combinations of effects. Let say that we have effects `e1, e2, e3` and image `p`. Then the output will be <br>
`p ---(!e1,!e2,!e3)---> p0` <br>
`p ---(!e1,!e2,e3)----> p1` <br>
`p ---(!e1,e2,!e3)----> p2` <br>
`p ---(e1,!e2,!e3)----> p3` <br>
`p ---(e1,e2,!e3)-----> p4` <br>
`p ---(!e1,e2,e3)-----> p5` <br>
`p ---(e1,!e2,e3)-----> p6` <br>
`p ---(e1,e2,e3)------> p7` <br>

`!e` means that we don't take `e`.  So for each image, the output is `2^k` modified images. <br>
Example of use:
```
$ time ./visual -la ../data/ *.png
[INFO] Initializing module: PN3itl11FlagManagerE
[INFO] Initialized module: PN3itl11FlagManagerE
[INFO] Initializing module: PN3itl5StateE
[INFO] Program use multithreading. Threads number: 12
[INFO] Initializing module: PN3itl9TransformE
[INFO] Initialized module: PN3itl9TransformE
[INFO] Initialized module: PN3itl5StateE
[INFO] Initializing module: PN3itl13EffectManagerE
[INFO] Initializing module: PN3itl11PerlinNoiseE
[INFO] Initialized module: PN3itl11PerlinNoiseE
[INFO] Initialized module: PN3itl13EffectManagerE
[INFO] Work finished successfully

real	0m6.629s
user	0m48.500s
sys	0m3.654s
```
for 80.000 produced images `128x128`. Speed is about `12.000/s` on `AMD RYZEN 5 2600`. As you can see, there are multiple flags:
```
constexpr char const* testing = "-t";
constexpr char const* printing_steps = "-p";
constexpr char const* log_erros = "-le";
constexpr char const* log_info = "-li";
constexpr char const* log_suggestions = "-ls";
constexpr char const* log_warnings = "-lw";
constexpr char const* log_all = "-la";
constexpr char const* log_time = "-lt";
```
If you don't want to log, just run a program without any flags. <br>

# ImageToLatex
A neural network capable of translating handwritting into latex. Project also provides A-Z tools for generating raw latex, producing images and transformating images as if they were written by human.
Project scheme: <br>
`Raw ==> Set ==> Visual ==> Model` <br>
Where `raw`, `set`, `visual` are tools and model is neural network for recognizing latex.


## Tool/Raw
Functional tool written in OCaml. Random latex expression generators, with various syntactic levels and conspects describing exact behaviour within the level. Create a set of generators capable of supplying the model with properly generated random latex expressions,
matching strict expectations, for training purposes. <br>


Performance:

|Type| Images/sec |
| :---: |     :---:      |
|Complex|~1.600.000  |
|Standard|~2.000.000  |
|Basic| ~4.000.000  |

## Tool/Set
Script tool which gets several input files with raw LaTeX and convert them into basic .png expressions. This part executes worker for each input file. Using via bash script: `bash set.sh *.in`. <br>
It will produce all content inside temporary folders, then it moves all photos to output folder. These images are input for `visual` part. <br>

It gets raw text like (for simplicity): `7+5` and returns: <br>
<p align="center">
  <img src="https://i.imgur.com/EaPStPE.png" width="75" height="50" title="hover text">
</p>

## Tool/Visual
The biggest tool, written in `C++` and using `OpenCV`. It applies a lot of different effects in order to make math as if it was
written by people. Final result is base of dataset for machine learning.
<br><br>
In that part there are predefined effects like:


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
|perlin| applies perlin noise mask (in progress) |

Additional effects are:

|position| moves sprite relative to background |
|background| changes background |

Each effect is take or not. For each image we apply all possible combinations of effects. Let say that we have effects `e1, e2, e3` and image `p`. Then output will be <br>
`p ---(!e1,!e2,!e3)---> p0` <br>
`p ---(!e1,!e2,e3)----> p1` <br>
`p ---(!e1,e2,!e3)----> p2` <br>
`p ---(e1,!e2,!e3)----> p3` <br>
`p ---(e1,e2,!e3)-----> p4` <br>
`p ---(!e1,e2,e3)-----> p5` <br>
`p ---(e1,!e2,e3)-----> p6` <br>
`p ---(e1,e2,e3)------> p7` <br>

`!e` menas that we don't take `e`.  So for each image the output are `2^k` modified images. <br>
Example of use:
```
mvxxx@mvxxx:~/programming/source/ai/itl/ImageToLatex/tools/visual/release$ time ./visual -la ../data/ *.png
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
If you don't want logging, just run program without any flags. <br>

## Model
Model is based on paper:
```
IMAGE TO LATEX VIA NEURAL NETWORKS
Avinash More
San Jose State University
```

We detecting each character separately and merge them into one sequence.
